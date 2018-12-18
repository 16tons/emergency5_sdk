// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/fire/BuildingFireEventFactory.h"
#include "em5/event/standard/fire/SimpleFireEvent.h"
#include "em5/event/standard/fire/helper/FireEventCandidateSearchHelper.h"
#include "em5/event/mini/OilRefineryExplosionEvent.h"
#include "em5/freeplay/factory/FreeplayEventTriggerInfo.h"
#include "em5/base/ComponentsRandomEnumerator.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/component/objects/EboxComponent.h"
#include "em5/component/spawnpoint/UnitSpawnPointComponent.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/game/player/PlayerManager.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/PlayerHelper.h"

#include <qsf_game/component/event/EventTagManagerComponent.h>

#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Detail implementations                                ]
	//[-------------------------------------------------------]
	namespace detail
	{

		void getBuildingComponentsByEventTag(const qsf::StringHash eventTag, std::unordered_set<BuildingComponent*>& buildingComponentsInMap)
		{
			const std::vector<qsf::game::EventTagComponent*>& tagComponents = qsf::game::EventTagManagerComponent::getEventTagComponentsByTag(eventTag, QSF_MAINMAP);
			buildingComponentsInMap.reserve(tagComponents.size());

			for (const qsf::game::EventTagComponent* eventTagComponent : tagComponents)
			{
				BuildingComponent* buildingComponent = eventTagComponent->getEntity().getComponent<BuildingComponent>();
				if (buildingComponent != nullptr)
				{
					buildingComponentsInMap.insert(buildingComponent);
				}
			}
		}

		void getSpawnPointsByEventTag(const qsf::StringHash eventTag, std::vector<uint64>& spawnPointEntitiesInMap)
		{
			const std::vector<qsf::game::EventTagComponent*>& tagComponents = qsf::game::EventTagManagerComponent::getEventTagComponentsByTag(eventTag, QSF_MAINMAP);
			spawnPointEntitiesInMap.clear();

			for (const qsf::game::EventTagComponent* eventTagComponent : tagComponents)
			{
				SpawnPointComponent* spawnPointComponent = eventTagComponent->getEntity().getComponent<SpawnPointComponent>();
				if (spawnPointComponent != nullptr)
				{
					spawnPointEntitiesInMap.push_back(spawnPointComponent->getEntityId());
				}
			}
		}

		bool checkCandidate(qsf::Entity& entity, const BuildingFireEventFactory::SearchFlags& searchFlags)
		{
			const EntityHelper entityHelper(entity);

			// Check if entity is a valid event target
			if (!entityHelper.isEntityValidEventTarget())
				return false;

			// Check if this entity is a building or a industrial plant
			if (searchFlags.isSet(BuildingFireEventFactory::SearchFlag::INDUSTRIAL_PLANT))
			{
				// Check if entity is a industrial plant
				if (!entityHelper.isEntityIndustrialPlant())
					return false;
			}
			else if (!entityHelper.isEntityBuilding())
			{
				// Check if entity is a normal building
				return false;
			}

			// Check if this building is damaged
			if (entityHelper.isBuildingDamaged())
				return false;

			// Check if building can be set on fire
			if (!entityHelper.canBeSetOnFireNow())
				return false;

			// Check if this building is empty (if there are persons inside they will be spawned later)
			if (!entityHelper.isContainerEmpty())
				return false;

			BuildingComponent* buildingComponent = entity.getComponent<BuildingComponent>();
			if (buildingComponent == nullptr)
				return false;

			// Check if the building has a window
			if (searchFlags.isSet(BuildingFireEventFactory::SearchFlag::PERSONS_AT_WINDOW))
			{
				qsf::Entity* windowEntity = entity.getMap().getEntityById(buildingComponent->getEventWindowId());
				if (nullptr == windowEntity)
					return false;
			}

			// Check if the building has a door
			if (searchFlags.isSet(BuildingFireEventFactory::SearchFlag::PERSONS_INSIDE))
			{
				qsf::Entity* doorEntity = entity.getMap().getEntityById(buildingComponent->getMainDoorId());
				if (nullptr == doorEntity)
					return false;
			}

			if (searchFlags.isSet(BuildingFireEventFactory::SearchFlag::EVACUATE_NEIGHBOR_HOUSE))
			{
				if (qsf::isUninitialized(buildingComponent->getEvacuateBuildingId()))
					return false;
			}

			const std::vector<uint64>& eboxEntities = buildingComponent->EboxEntitiesArray.getVariable();
			if (searchFlags.isSet(BuildingFireEventFactory::SearchFlag::DEFECTIVE_EBOX))
			{
				// We need atleast one ebox
				if (eboxEntities.size() == 0)
					return false;

				// Check all eboxes
				for (uint64 entityId : eboxEntities)
				{
					qsf::Entity* eboxEntity = entity.getMap().getEntityById(entityId);
					if (nullptr == eboxEntity)
						return false;

					EboxComponent* eboxComponent = eboxEntity->getComponent<EboxComponent>();
					if (nullptr == eboxComponent)
						return false;

					// Check if the ebox is already burned or has another event (With correct editing this check is not needed, but lets be sure)
					EntityHelper eboxEntityHelper(*eboxEntity);
					if (eboxEntityHelper.isBurned() || eboxEntityHelper.isBurning() || !eboxEntityHelper.isEntityValidEventTarget())
						return false;
				}
			}
			else
			{
				// Not a valid building if we have eboxes
				if (!eboxEntities.empty())
					return false;
			}

			// Accepted
			return true;
		}

		qsf::Entity* findCandidateFromRandomEnumerator(ComponentsRandomEnumerator<BuildingComponent>& randomEnumerator, const std::string& difficultyTags, const std::string& eventTag, const BuildingFireEventFactory::SearchFlags& searchFlags)
		{
			// Start the helper
			FireEventCandidateSearchHelper fireEventCandidateSearchHelper;
			fireEventCandidateSearchHelper.setDifficultyTagsByString(difficultyTags);

			while (BuildingComponent* buildingComponent = randomEnumerator.getNext())
			{
				qsf::Entity& entity = buildingComponent->getEntity();

				// Check difficulty tag
				if (!fireEventCandidateSearchHelper.hasDifficultyTag(entity))
					continue;

				// Check if the building is appropriate
				if (checkCandidate(entity, searchFlags))
				{
					return &entity;
				}
			}

			// None found
			return nullptr;
		}

	}


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	qsf::Entity* BuildingFireEventFactory::findCandidate(const std::string& difficultyTags, const std::string& eventTag, const SearchFlags& searchFlags)
	{
		if (eventTag.empty())
		{
			// Take all building components in the map
			ComponentsRandomEnumerator<BuildingComponent> randomEnumerator;
			return detail::findCandidateFromRandomEnumerator(randomEnumerator, difficultyTags, eventTag, searchFlags);
		}
		else
		{
			// Take only buildings with the given event tag
			std::vector<BuildingComponent*> buildingComponentsInMap;

			std::vector<qsf::game::EventTagComponent*> tagComponentsInMap = qsf::game::EventTagManagerComponent::getEventTagComponentsStartsWithTag(eventTag, QSF_MAINMAP);
			for (qsf::game::EventTagComponent* eventTagComponent : tagComponentsInMap)
			{
				BuildingComponent* buildingComponent = eventTagComponent->getEntity().getComponent<BuildingComponent>();
				if (nullptr != buildingComponent)
				{
					buildingComponentsInMap.push_back(buildingComponent);
				}
			}

			ComponentsRandomEnumerator<BuildingComponent> randomEnumerator(constructionpolicy::SWAP, buildingComponentsInMap);
			return detail::findCandidateFromRandomEnumerator(randomEnumerator, difficultyTags, eventTag, searchFlags);
		}
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	BuildingFireEventFactory::BuildingFireEventFactory() :
		mTrappedPersonWithPlague(false),
		mExplosion(false),
		mEvacuateNeighborHouseChance(0.0f),
		mSpawnFromConnectedSpawnpoolsChance(0.0f),
		mUseAmbientLayer(false)
	{
		// Nothing here
	}

	BuildingFireEventFactory::~BuildingFireEventFactory()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	FreeplayEvent* BuildingFireEventFactory::tryTriggerEvent()
	{
		Player* localPlayer = PlayerManager::getLocalPlayer();
		if (localPlayer == nullptr)
			return nullptr;

		if (mSearchFlags.isSet(SearchFlag::INDUSTRIAL_PLANT))
		{
			// Player must already have the decontamination vehicle
			PlayerHelper playerHelper(*localPlayer);
			if (0 == playerHelper.getNumberOfUnit(CommandableComponent::UNITTAG_FIREFIGHTERS_DEKONP))
			{
				setTriggerFailReason("Needed units are not available, no dekonp for industrial fire.");
				return nullptr;
			}
		}

		if (mSearchFlags.isSet(SearchFlag::DEFECTIVE_EBOX))
		{
			// Check if engineer is available
			PlayerHelper playerHelper(*localPlayer);
			if (0 == playerHelper.getNumberOfUnit(CommandableComponent::UNITTAG_TECH_ENGINEER))
			{
				setTriggerFailReason("Needed units are not available, no engineer for a building fire with blocked extinguish by an ebox.");
				return nullptr;
			}
		}

		// Calculate the chance for needing a building with evacuate neighbor houses
		mSearchFlags.set(SearchFlag::EVACUATE_NEIGHBOR_HOUSE, qsf::Random::getRandomChance(mEvacuateNeighborHouseChance));

		// Define event tag
		std::string eventTag = getEventTag();
		if (mExplosion)
		{
			eventTag = "EXPLOSION_BUILDING";
		}

		// Find a candidate entity
		qsf::Entity* targetEntity = nullptr;
		if (getTriggerInfo().mEnforcedLocation.empty())
		{
			targetEntity = findCandidate(mDifficultyTags, eventTag, mSearchFlags);
			if (nullptr == targetEntity)
			{
				setTriggerFailReason("Could not find entity by event tag " + getDifficultyTags());
				return nullptr;
			}
		}
		else
		{
			try
			{
				// Use provided entity and ensure that it's not already part of an event
				const uint64 targetEntityId = boost::lexical_cast<uint64>(getTriggerInfo().mEnforcedLocation);
				targetEntity = QSF_MAINMAP.getEntityById(targetEntityId);
				if (nullptr == targetEntity)
				{
					setTriggerFailReason("Could not find entity by event ID " + std::to_string(targetEntityId));
					return nullptr;
				}
				EventIdComponent* eventIdComponent = targetEntity->getComponent<EventIdComponent>();
				if (nullptr != eventIdComponent && nullptr != eventIdComponent->getFreeplayEvent())
				{
					setTriggerFailReason("Event ID " + std::to_string(targetEntityId) + " is already part of another event");
					return nullptr;
				}
			}
			catch (const std::exception&)
			{
				setTriggerFailReason("Enforced location \"" + getTriggerInfo().mEnforcedLocation + "\" is no entity ID");
				return nullptr;
			}
		}

		if (mUseAmbientLayer)
		{
			const qsf::game::EventTagComponent* eventTagComponent = targetEntity->getComponent<qsf::game::EventTagComponent>();
			if (nullptr != eventTagComponent)
			{
				mLayerName = eventTagComponent->getTagStartsWithTag(eventTag);
			}
		}

		// Create the event instance
		//  -> We need to support the mini-event "Explosion in oil refinery" here as well
		SimpleFireEvent& freeplayEvent = (getEventName() == "Oil_Refinery_Explosion") ? createAndInitializeEvent<OilRefineryExplosionEvent>() : createAndInitializeEvent<SimpleFireEvent>();
		deserializeRandomFreeplayVariant(freeplayEvent);

		// Initialize the event
		freeplayEvent.setTargetEntity(*targetEntity);
		freeplayEvent.setTrappedPersonWithPlague(mTrappedPersonWithPlague);
		bool spawnFromConnectedSpawnpools = qsf::Random::getRandomChance(mSpawnFromConnectedSpawnpoolsChance);
		freeplayEvent.setSpawnFromConnectedSpawnpools(spawnFromConnectedSpawnpools);
		if (mUseAmbientLayer && !mLayerName.empty())
		{
			freeplayEvent.setAmbientLayerName(mLayerName);
		}

		// Get victim spawn points
		if (mSearchFlags.isSet(SearchFlag::INDUSTRIAL_PLANT) && !getDifficultyTags().empty())
		{
			detail::getSpawnPointsByEventTag(qsf::StringHash(getDifficultyTags()), freeplayEvent.mVictimSpawnPoints);
		}

		// Done
		return &freeplayEvent;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
