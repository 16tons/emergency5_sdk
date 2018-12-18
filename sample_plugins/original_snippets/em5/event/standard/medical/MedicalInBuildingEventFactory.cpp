// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/medical/MedicalInBuildingEventFactory.h"
#include "em5/event/standard/medical/MedicalInBuildingEvent.h"
#include "em5/event/standard/fire/helper/FireEventCandidateSearchHelper.h"
#include "em5/base/ComponentsRandomEnumerator.h"
#include "em5/component/objects/EboxComponent.h"
#include "em5/component/spawnpoint/UnitSpawnPointComponent.h"
#include "em5/game/player/PlayerManager.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/component/event/EventTagManagerComponent.h>

#include <qsf/map/Entity.h>


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
		void getSpawnPointsByEventTag2(const qsf::StringHash eventTag, std::vector<uint64>& spawnPointEntitiesInMap)
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
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	MedicalInBuldingEventFactory::MedicalInBuldingEventFactory() :
		mTrappedPersonWithPlague(false),
		mExplosion(false),
		mEvacuateNeighborHouseChance(0.0f),
		mSpawnFromConnectedSpawnpoolsChance(0.0f),
		mUseAmbientLayer(false)
	{
		// Nothing here
	}

	MedicalInBuldingEventFactory::~MedicalInBuldingEventFactory()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	FreeplayEvent* MedicalInBuldingEventFactory::tryTriggerEvent()
	{
		// Calculate the chance for needing a building with evacuate neighbor houses
		mSearchFlags.set(SearchFlag::EVACUATE_NEIGHBOR_HOUSE, qsf::Random::getRandomChance(mEvacuateNeighborHouseChance));

		// Define event tag
		std::string eventTag = getEventTag();
		if (mExplosion)
		{
			eventTag = "EXPLOSION_BUILDING";
		}

		// Find a candidate entity
		qsf::Entity* targetEntity = BuildingFireEventFactory::findCandidate(mDifficultyTags, eventTag, mSearchFlags);
		if (nullptr == targetEntity)
		{
			setTriggerFailReason("Could not find entity by event tag " + getDifficultyTags());
			return nullptr;
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
		MedicalInBuildingEvent& freeplayEvent = createAndInitializeEvent<MedicalInBuildingEvent>();
		deserializeRandomFreeplayVariant(freeplayEvent);

		// Initialize the event
		freeplayEvent.setTargetEntity(*targetEntity);
		freeplayEvent.setTrappedPersonWithPlague(mTrappedPersonWithPlague);
		bool spawnFromConnectedSpawnpools = qsf::Random::getRandomChance(mSpawnFromConnectedSpawnpoolsChance);
		freeplayEvent.setSpawnFromConnectedSpawnpools(spawnFromConnectedSpawnpools);

		// Get victim spawn points
		if (mSearchFlags.isSet(SearchFlag::INDUSTRIAL_PLANT) && !getDifficultyTags().empty())
		{
			detail::getSpawnPointsByEventTag2(qsf::StringHash(getDifficultyTags()), freeplayEvent.mVictimSpawnPoints);
		}
		if (mUseAmbientLayer && !mLayerName.empty())
		{
			freeplayEvent.setAmbientLayerName(mLayerName);
		}

		// Done
		return &freeplayEvent;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
