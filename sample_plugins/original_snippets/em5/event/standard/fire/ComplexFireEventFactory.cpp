// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/fire/ComplexFireEventFactory.h"
#include "em5/event/standard/fire/ComplexFireEvent.h"
#include "em5/event/standard/fire/helper/FireEventCandidateSearchHelper.h"
#include "em5/base/ComponentsRandomEnumerator.h"
#include "em5/freeplay/factory/FreeplayEventTriggerInfo.h"
#include "em5/component/spawnpoint/civilist/PersonSpawnPointComponent.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/component/base/HiddenComponent.h>
#include <qsf_game/component/event/EventTagManagerComponent.h>
#include <qsf_game/component/event/EventMetaEntityComponent.h>

#include <qsf/QsfHelper.h>
#include <qsf/map/Map.h>
#include <qsf/map/Entity.h>
#include <qsf/math/Random.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	ComplexFireEventFactory::ComplexFireEventFactory() :
		mPersonDamageChance(0.5f),
		mPersonMissingChance(0.5f)
	{
		// Nothing here
	}

	ComplexFireEventFactory::~ComplexFireEventFactory()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	FreeplayEvent* ComplexFireEventFactory::tryTriggerEvent()
	{
		qsf::Entity* metaEntity = nullptr;
		if (getTriggerInfo().mEnforcedLocation.empty())
		{
			// Find a candidate entity
			metaEntity = getMetaEntity();
			if (nullptr == metaEntity)
			{
				return nullptr;
			}
		}
		else
		{
			try
			{
				// Use provided entity and ensure that it's not already part of an event
				const uint64 metaEntityId = boost::lexical_cast<uint64>(getTriggerInfo().mEnforcedLocation);
				metaEntity = QSF_MAINMAP.getEntityById(metaEntityId);
				if (nullptr == metaEntity)
				{
					setTriggerFailReason("Could not find entity by event ID " + std::to_string(metaEntityId));
					return nullptr;
				}
				EventIdComponent* eventIdComponent = metaEntity->getComponent<EventIdComponent>();
				if (nullptr != eventIdComponent && nullptr != eventIdComponent->getFreeplayEvent())
				{
					setTriggerFailReason("Event ID " + std::to_string(metaEntityId) + " is already part of another event");
					return nullptr;
				}
			}
			catch (const std::exception&)
			{
				setTriggerFailReason("Enforced location \"" + getTriggerInfo().mEnforcedLocation + "\" is no entity ID");
				return nullptr;
			}
		}

		// Calculate person damage
		bool personDamage = qsf::Random::getRandomChance(mPersonDamageChance);
		bool personMissing = (personDamage ? qsf::Random::getRandomChance(mPersonMissingChance) : false);

		// 2 for person damage, 3 for person damage with hidden persons, 1 for no person damage
		uint32 variationIndex = (personDamage ? (personMissing ? 3 : 2) : 1);

		// Create the event instance
		ComplexFireEvent& freeplayEvent = createAndInitializeEvent<ComplexFireEvent>();
		deserializeFreeplayVariantByIndex(freeplayEvent, variationIndex);

		// Set if there is initial person damage
		freeplayEvent.setPersonDamage(personDamage);
		freeplayEvent.setPersonMissing(personMissing);

		// Initialize the event
		ComplexFireEvent::EntityArray& inflameEntities = freeplayEvent.mInflameEntities;
		ComplexFireEvent::EntityArray& spawnPointEntities = freeplayEvent.mSpawnPointEntities;

		const qsf::game::EventMetaEntityComponent* eventMetaEntityComponent = metaEntity->getComponent<qsf::game::EventMetaEntityComponent>();
		if (nullptr != eventMetaEntityComponent)	// Check if there is a meta entity component
		{
			const qsf::Map& map = QSF_MAINMAP;
			const std::vector<uint64>& entityIdArray = eventMetaEntityComponent->getEntityIdArray();
			for (uint64 entityId : entityIdArray)
			{
				qsf::Entity* candidateEntity = map.getEntityById(entityId);
				if (nullptr != candidateEntity)
				{
					if (checkEntity(*candidateEntity))
					{
						inflameEntities.push_back(candidateEntity);
					}
					else if (checkPersonSpawnEntity(*candidateEntity))
					{
						spawnPointEntities.push_back(candidateEntity);
					}
				}
			}
		}
		else
		{
			if (checkEntity(*metaEntity))
			{
				inflameEntities.push_back(metaEntity);
			}
			else
			{
				// The meta entity is not a valid target
				return nullptr;
			}
		}

		if (inflameEntities.empty())
			return nullptr;	// Can't start event without candidates

		// Set meta entity ID
		freeplayEvent.mMetaEntityId = metaEntity->getId();

		// Done
		return &freeplayEvent;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	qsf::Entity* em5::ComplexFireEventFactory::getMetaEntity() const
	{
		// Start the helper
		FireEventCandidateSearchHelper fireEventCandidateSearchHelper;
		fireEventCandidateSearchHelper.setDifficultyTagsByString(mDifficultyTags);

		// Get a listing of all tagged entities
		const std::vector<qsf::game::EventTagComponent*>& tagComponentsInMap = qsf::game::EventTagManagerComponent::getEventTagComponentsByTag(qsf::StringHash(mEventTag), QSF_MAINMAP);
		if (tagComponentsInMap.empty())
			return nullptr;

		ComponentsRandomEnumerator<qsf::game::EventTagComponent> openTagComponents(tagComponentsInMap);
		while (const qsf::game::EventTagComponent* eventTagComponent = openTagComponents.getNext())
		{
			const qsf::game::EventMetaEntityComponent* eventMetaEntityComponent = eventTagComponent->getEntity().getComponent<qsf::game::EventMetaEntityComponent>();
			if (nullptr == eventMetaEntityComponent)
				continue;

			if (!fireEventCandidateSearchHelper.hasDifficultyTag(*eventTagComponent))
				continue;

			// Got one
			return &eventTagComponent->getEntity();
		}

		// None found
		return nullptr;
	}

	bool ComplexFireEventFactory::checkEntity(qsf::Entity& entity) const
	{
		const EntityHelper entityHelper(entity);

		// If there is an event tag check if its the right one
		const qsf::game::EventTagComponent* eventTagComponent = entity.getComponent<qsf::game::EventTagComponent>();
		if (nullptr != eventTagComponent && !eventTagComponent->hasTag(mEventTag))
			return false;

		// Check if person has an event ID component
		if (entityHelper.checkForInitializedEventIdComponent())
			return false;

		if (qsf::game::HiddenComponent::isHidden(entity))
			return false;

		if (entityHelper.isEntityVisibleForAnyPlayer())
			return false;

		if (!entityHelper.isInEventBoundaries())
			return false;

		// Check if this entity is damaged
		if (entityHelper.isEntityDamaged())
			return false;

		// Check if entity can be set on fire
		if (!entityHelper.canBeSetOnFireNow())
			return false;

		// Accepted
		return true;
	}

	bool ComplexFireEventFactory::checkPersonSpawnEntity(qsf::Entity& entity) const
	{
		const EntityHelper entityHelper(entity);

		// If there is an event tag check if its the right one
		const qsf::game::EventTagComponent* eventTagComponent = entity.getComponent<qsf::game::EventTagComponent>();
		if (nullptr != eventTagComponent && !eventTagComponent->hasTag(mEventTag))
			return false;

		// Check if spawnpoint has event ID component
		if (entityHelper.checkForEventIdComponent())
			return false;

		if (!entityHelper.isInEventBoundaries())
			return false;

		// Check if entity has a spawn component
		if (nullptr == entity.getComponent<PersonSpawnPointComponent>())
			return false;

		// Accepted
		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
