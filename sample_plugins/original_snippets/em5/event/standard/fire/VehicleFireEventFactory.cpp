// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/fire/VehicleFireEventFactory.h"
#include "em5/event/standard/fire/VehicleFireEvent.h"
#include "em5/event/standard/fire/helper/FireEventCandidateSearchHelper.h"
#include "em5/base/ComponentsRandomEnumerator.h"
#include "em5/base/GameplayAssets.h"
#include "em5/base/ContainerCategory.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include "em5/game/Game.h"
#include "em5/game/GameSimulation.h"
#include "em5/game/spawn/SpawnPoolManager.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/EM5Helper.h"

#include <qsf_game/component/event/EventTagComponent.h>

#include <qsf/QsfHelper.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/math/Random.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Local definitions                                     ]
	//[-------------------------------------------------------]
	// Event names
	const std::string EVENT_NAME_VEHICLE_FIRE	= "EM5_EVENT_TITLE_10";
	const std::string EVENT_NAME_VEHICLE_FIRE_P	= "EM5_EVENT_TITLE_11";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	VehicleFireEventFactory::VehicleFireEventFactory() :
		mPersonDamage(false),
		mPersonDamageChance(0)
	{
		// Nothing here
	}

	VehicleFireEventFactory::~VehicleFireEventFactory()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	FreeplayEvent* VehicleFireEventFactory::tryTriggerEvent()
	{
		// Calculate person damage
		mPersonDamage = qsf::Random::getRandomChance(mPersonDamageChance);

		// Find a candidate entity
		qsf::Entity* targetEntity = findCandidate();
		if (nullptr == targetEntity)
		{
			setTriggerFailReason("Could not find entity by event tag " + getDifficultyTags());
			return nullptr;
		}

		// 2 for person damage 1 for no person damage
		uint32 variationIndex = mPersonDamage ? 2 : 1;

		if (!spawnDriverPerson(*targetEntity))
		{
			setTriggerFailReason("Can't spawn driver in car with event tag " + getDifficultyTags());
			return nullptr;
		}

		// Create the event instance
		VehicleFireEvent& freeplayEvent = createAndInitializeEvent<VehicleFireEvent>();
		deserializeFreeplayVariantByIndex(freeplayEvent, variationIndex);

		// TODO(jm): Comment: This overrides the JSON file value to make the event random. Remove this if u want to set it manually
		// Set a value per random factor
		freeplayEvent.setPersonsDamage(mPersonDamage);

		// Initialize the event
		freeplayEvent.setTargetEntity(*targetEntity);

		// Set event name
		if (mPersonDamage)
		{
			freeplayEvent.setEventName(EVENT_NAME_VEHICLE_FIRE_P);
		}
		else
		{
			freeplayEvent.setEventName(EVENT_NAME_VEHICLE_FIRE);
		}

		// Done
		return &freeplayEvent;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	qsf::Entity* VehicleFireEventFactory::findCandidate() const
	{
		// Start the helper
		FireEventCandidateSearchHelper fireEventCandidateSearchHelper;
		fireEventCandidateSearchHelper.setDifficultyTagsByString(mDifficultyTags);

		// Get a listing of all road vehicle components
		ComponentsRandomEnumerator<RoadVehicleComponent> randomEnumerator;
		while (RoadVehicleComponent* roadVehicleComponent = randomEnumerator.getNext())
		{
			qsf::Entity& entity = roadVehicleComponent->getEntity();

			// Check difficulty tag
			if (!fireEventCandidateSearchHelper.hasDifficultyTag(entity))
				continue;

			// Check if the vehicle is appropriate
			if (checkCandidate(entity))
				return &entity;
		}

		// None found
		return nullptr;
	}

	bool VehicleFireEventFactory::checkCandidate(qsf::Entity& entity) const
	{
		const EntityHelper entityHelper(entity);

		// Check if entity is a valid event target
		if (!entityHelper.isEntityValidEventTarget())
			return false;

		// Is road vehicle damaged?
		if (entityHelper.isRoadVehicleDamaged())
			return false;

		// Check if vehicle can be set on fire
		if (!entityHelper.canBeSetOnFireNow())
			return false;

		// Is the vehicle a civil vehicle?
		if (!entityHelper.isCivilRoadVehicle())
			return false;

		// The vehicle must be empty, the driver person will be spawned (because civil vehicles are usually already empty)
		if (!entityHelper.isContainerEmpty())
			return false;

		// TODO(co) Why is there commented code?
		//{ // Is the vehicle in movement? // TODO(jm): Is there a chance of getting an error?
		//	const qsf::MovableComponent* movableComponent = entity.getComponent<qsf::MovableComponent>();
		//	if (nullptr != movableComponent || 0.0f != movableComponent->getSpeed())
		//		return false;
		//}

		//{ // Is there a action component at all
		//	const qsf::ActionComponent* actionComponent = entity.getComponent<qsf::ActionComponent>();
		//	if (nullptr != actionComponent)
		//	{
		//		const qsf::Action* currentAction = actionComponent->getCurrentAction();
		//		// TODO(co) Avoid negotiations: Please simplify this expression
		//		if (!(nullptr != currentAction && (currentAction->getTypeId() == em5::MoveAction::ACTION_ID || currentAction->getTypeId() == qsf::ai::MoveAction::ACTION_ID)))
		//			return false;
		//	}
		//}

		// Accepted
		return true;
	}

	bool VehicleFireEventFactory::spawnDriverPerson(qsf::Entity& vehicleEntity)
	{
		const std::string randomName = EM5_GAME.getSimulation().getSpawnPoolManager().getRandomNameFromSpawnPool(assets::SPAWNPOOL_DEFAULT_PERSON);
		if (!randomName.empty())
		{
			qsf::Entity* entity = MapHelper(QSF_MAINMAP).createObjectByLocalPrefabAssetId(qsf::StringHash(randomName));
			if (nullptr != entity)
			{
				EntityHelper(*entity).enterContainer(vehicleEntity, container::CONTAINERTYPE_PASSENGER);
				return true;
			}
		}
		return false;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
