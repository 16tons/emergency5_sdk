// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/crime/MissingPersonEventFactory.h"
#include "em5/event/standard/crime/MissingPersonEvent.h"
#include "em5/base/ComponentsRandomEnumerator.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/component/spawnpoint/civilist/PersonSpawnPointComponent.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/game/player/PlayerManager.h"
#include "em5/game/units/UnitPool.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/component/event/EventTagManagerComponent.h>

#include <qsf/component/link/LinkComponent.h>
#include <qsf/map/Entity.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Local definitions                                     ]
	//[-------------------------------------------------------]
	// Event tags
	namespace detail
	{
		const std::string& EVENT_TAG_MISSING_PERSON_SPAWN = "MISSING_PERSON_SPAWN";
		const std::string& EVENT_TAG_PERSON_SEARCHPOINT = "MISSING_PERSON_SEARCHPOINT";
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	MissingPersonEventFactory::MissingPersonEventFactory() :
		mIsVictimDeadPercent(0.5f),
		mUseOnlyWaterVictimPercent(0.0f)
	{
		// Nothing here
	}

	MissingPersonEventFactory::~MissingPersonEventFactory()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	FreeplayEvent* MissingPersonEventFactory::tryTriggerEvent()
	{
		// Calculate person damage
		const bool isVictimDead = qsf::Random::getRandomChance(mIsVictimDeadPercent);
		const bool useOnlyWaterVictim = qsf::Random::getRandomChance(mUseOnlyWaterVictimPercent);

		// 2 for person damage 1 for no person damage
		const uint32 variationIndex = (isVictimDead ? 2 : 1);

		// Check if the needed units to finish the event are available (Sek drone and rescue dog)
		if (!checkIfNeededUnitsAreAvailable(isVictimDead))
		{
			setTriggerFailReason("Needed units are not available");
			return nullptr;
		}

		// Find spawn point
		qsf::Entity* spawnPointEntity = findSpawnPoint(useOnlyWaterVictim);
		if (nullptr == spawnPointEntity)
		{
			setTriggerFailReason("Could not find spawn point by event tag " + getEventTag());
			return nullptr;
		}

		// Create a candidate entity
		qsf::Entity* targetEntity = createCandidate(*spawnPointEntity);
		if (nullptr == targetEntity)
		{
			setTriggerFailReason("Could not find spawn entity from spawn point with event tag " + getEventTag());
			return nullptr;
		}

		// Create the event instance
		MissingPersonEvent& freeplayEvent = createAndInitializeEvent<MissingPersonEvent>();
		deserializeFreeplayVariantByIndex(freeplayEvent, variationIndex);

		// If set, the dog will go this spot instead to the person directly For water events the search point is on the ground,
		const uint64 dogSearchPointEntityId = findDogSearchPointEntityFromSpawn(*spawnPointEntity);

		// Initialize the event
		freeplayEvent.init(*targetEntity, dogSearchPointEntityId);

		// To avoid having the same spawn point in multiple events, register the spawn point to the event
		spawnPointEntity->getOrCreateComponent<EventIdComponent>()->setEvent(freeplayEvent, eventspreadreason::NO_REASON);

		// Done
		return &freeplayEvent;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	qsf::Entity* MissingPersonEventFactory::findSpawnPoint(bool useOnlyWaterVictim) const
	{
		std::string eventTag = getEventTag();
		if (eventTag.empty())
		{
			eventTag = detail::EVENT_TAG_MISSING_PERSON_SPAWN;
		}

		// Get a listing of all spawnpoints for the missing persons
		const std::vector<qsf::game::EventTagComponent*>& tagComponentsInMap = qsf::game::EventTagManagerComponent::getEventTagComponentsByTag(qsf::StringHash(eventTag), QSF_MAINMAP);

		// No entities with event tag found
		if (tagComponentsInMap.empty())
		{
			setTriggerFailReason("Could not find spawn point by event tag '" + eventTag + "'");
			return nullptr;
		}

		ComponentsRandomEnumerator<qsf::game::EventTagComponent> randomEnumerator(tagComponentsInMap);
		while (qsf::Entity* entity = randomEnumerator.getNextEntity())
		{
			// Check if the candidate is appropriate
			if (checkSpawnPoint(*entity, useOnlyWaterVictim))
			{
				return entity;
			}
		}

		setTriggerFailReason("None of the tested spawn points with event tag '" + eventTag + "' were valid (e.g. wrong editing as spawn point, or outside event boundaries)");
		return nullptr;
	}

	qsf::Entity* MissingPersonEventFactory::createCandidate(const qsf::Entity& spawnPointEntity) const
	{
		// Create the missing person
		qsf::Entity* personEntity = nullptr;

		PersonSpawnPointComponent* spawnComponent = spawnPointEntity.getComponent<PersonSpawnPointComponent>();
		if (nullptr == spawnComponent)
			return personEntity;

		for (int tries = 0; tries < 10; ++tries)
		{
			personEntity = spawnComponent->spawnRandom();
			if (nullptr == personEntity)
				continue;

			if (EntityHelper(*personEntity).isCivilPerson())
				return personEntity;
		}

		// If no person could be created
		return nullptr;
	}

	bool MissingPersonEventFactory::checkSpawnPoint(qsf::Entity& targetEntity, bool useOnlyWaterVictim) const
	{
		EntityHelper entityHelper(targetEntity);

		// Check if spawnpoint has event ID component
		if (entityHelper.checkForEventIdComponent())
			return false;

		if (!entityHelper.isInEventBoundaries())
			return false;

		// Check if entity has a spawn component
		if (nullptr == targetEntity.getComponent<PersonSpawnPointComponent>())
			return false;

		qsf::game::EventTagComponent* eventTagComponent = targetEntity.getComponent<qsf::game::EventTagComponent>();
		if (eventTagComponent != nullptr)
		{
			// Check the linked childs for the event tag
			if (useOnlyWaterVictim != eventTagComponent->hasTag(detail::EVENT_TAG_PERSON_SEARCHPOINT, true))
			{
				// Config is not matching, only water or not
				return false;
			}
		}

		// Accepted
		return true;
	}

	bool MissingPersonEventFactory::checkIfNeededUnitsAreAvailable(bool isVictimDead) const
	{
		Player* localPlayer = PlayerManager::getLocalPlayer();
		if (nullptr == localPlayer)
			return false;

		// Check if rescue dog leader is available
		UnitPool& unitPool = localPlayer->getUnitPool();
		if (unitPool.getNumberMaxAvailableByName(CommandableComponent::UNITTAG_AMBULANCE_RESCUEDOG_LEADER) <= 0)
		{
			return false;
		}

		// Check if rescue SEK drone is available (only for murder search)
		if (isVictimDead && unitPool.getNumberMaxAvailableByName(CommandableComponent::UNITTAG_POLICE_DRONE_TRANSPORT) <= 0)
		{
			return false;
		}

		return true;
	}

	uint64 MissingPersonEventFactory::findDogSearchPointEntityFromSpawn(qsf::Entity& spawnPoolEntity) const
	{
		// Ask all linked childs for entity with the right event tag
		qsf::LinkComponent* linkComponent = spawnPoolEntity.getComponent<qsf::LinkComponent>();
		if (nullptr != linkComponent)
		{
			for (const qsf::LinkComponent* childComponent : linkComponent->getChildLinks())
			{
				qsf::game::EventTagComponent* eventTagComponent = childComponent->getEntity().getComponent<qsf::game::EventTagComponent>();
				if (nullptr != eventTagComponent && eventTagComponent->hasTag(detail::EVENT_TAG_PERSON_SEARCHPOINT))
				{
					return eventTagComponent->getEntity().getId();
				}
			}
		}

		// No linked entity or no tag
		return qsf::getUninitialized<uint64>();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
