// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/logic/local/gangsters/GangsterKillerPsychoLogic.h"
#include "em5/action/move/MoveAction.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/event/standard/gangster/RunningAmokEvent.h"
#include "em5/freeplay/event/FreeplayEvent.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/base/DistanceHelper.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/Map.h>
#include <qsf/map/query/ComponentMapQuery.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Local definitions                                     ]
	//[-------------------------------------------------------]
	namespace detail
	{
		// TODO(ks) Maybe replace it with a configurable value
		static const float SCAN_FOR_TARGETS_RANGE = 75.0f;
		static const float CHASE_VEHICLES_MAX_RANGE = 100.0f;
	}


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 GangsterKillerPsychoLogic::GAMELOGIC_TYPE_ID = qsf::StringHash("em5::GangsterKillerPsychoLogic");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GangsterKillerPsychoLogic::GangsterKillerPsychoLogic() :
		GangsterBaseLogic(GAMELOGIC_TYPE_ID),
		mCurrentTargetId(qsf::getUninitialized<uint64>())
	{
		mEquippedWeaponTypes.set(weapon::STONE);
		mEquippedWeaponTypes.set(weapon::PISTOL);
		mEquippedWeaponTypes.set(weapon::BASEBALLBAT);
		mEquippedWeaponTypes.set(weapon::KNIFE);
		mEquippedWeaponTypes.set(weapon::FIST);

		mAlwaysShowWeapon = true;

		// We want to receive "onUnitsSpotted" and "onNoUnitsSpotted" callbacks
		mUseOldSpottedUnitsLogic = true;
	}


	//[-------------------------------------------------------]
	//[ Protected virtual methods                             ]
	//[-------------------------------------------------------]
	void GangsterKillerPsychoLogic::onSimulationUpdate(const qsf::JobArguments& jobArguments)
	{
		qsf::Entity* entity = getEntity();
		if (!entity->getOrCreateComponent<qsf::ActionComponent>()->getPlan().isEmpty())
		{
			// Still got "work" to do (but he is not fleeing)
			return;
		}

		const qsf::Map& map = getMap();

		qsf::Entity* currentTarget = map.getEntityById(mCurrentTargetId);
		bool targetInvalid = false;

		if (nullptr == currentTarget)
		{
			targetInvalid = true;
		}
		else if (nullptr != currentTarget->getComponent<PersonComponent>())
		{
			// Target is person

			if (EntityHelper(*currentTarget).isPersonInjured())
			{
				targetInvalid = true;
			}
		}
		else
		{
			// Target is a car

			if (EntityHelper(*currentTarget).isEntityDamaged())
			{
				targetInvalid = true;
			}
			else if (EntityHelper(*currentTarget).isBurning())
			{
				targetInvalid = true;
			}
		}

		if (targetInvalid)
		{
			// Target has become invalid (e.g. despawned) or is injured. Need to find a new one.
			qsf::setUninitialized(mCurrentTargetId);
		}

		if (qsf::isInitialized(mCurrentTargetId))
		{
			// Check whether the current target is valid, close enough, attackable, etc.
			qsf::Entity* currentTarget = map.getEntityById(mCurrentTargetId);

			if (nullptr != currentTarget)
			{

				EntityHelper targetEntityHelper(*currentTarget);
				if (targetEntityHelper.isValidGangsterEventTarget(*getEntity(), false) && qsf::game::DistanceHelper::getDistance(*getEntity(), *currentTarget) < detail::CHASE_VEHICLES_MAX_RANGE)
				{
					// Target is valid. Get in fighting mode and attack it!
					attack(*currentTarget);
					return;
				}
				else
				{
					// Reset target id
					mCurrentTargetId = qsf::getUninitialized<uint64>();
				}
			}
		}

		// No valid target. Find one!

		// Get a listing of all person components in range
		std::vector<PersonComponent*> personComponentsInRange;
		qsf::ComponentMapQuery(map).getInstancesInCircle<PersonComponent>(mTransformComponent->getPosition(), detail::SCAN_FOR_TARGETS_RANGE, personComponentsInRange);

		glm::vec3 selfPosition = mTransformComponent->getPosition();
		float shortestSquaredDistance = std::numeric_limits<float>::max();
		uint64 closestTargetId = qsf::getUninitialized<uint64>();

		for (const PersonComponent* personComp : personComponentsInRange)
		{
			EntityHelper targetEntityHelper(personComp->getEntity());
			if (!targetEntityHelper.isValidGangsterEventTarget(*getEntity(), false))
			{
				// Not a valid target for this gangster event
				continue;
			}

			// Query victim TransformComponent
			qsf::TransformComponent* targetTC = personComp->getEntity().getComponent<qsf::TransformComponent>();
			if (nullptr == targetTC)
				continue;

			// Get target position
			const glm::vec3& targetPosition = targetTC->getPosition();

			// Check whether it is even closer than anything we found until now
			const float squaredDistance = glm::distance2(targetPosition, selfPosition);
			if (squaredDistance < shortestSquaredDistance)
			{
				// New closest target found
				shortestSquaredDistance = squaredDistance;
				closestTargetId = personComp->getEntityId();
			}
		}

		// Try to find an even closer vehicle

		// Get a listing of all roadVehicle components in range
		std::vector<RoadVehicleComponent*> roadVehicleComponentsInRange;
		qsf::ComponentMapQuery(map).getInstancesInCircle<RoadVehicleComponent>(mTransformComponent->getPosition(), detail::SCAN_FOR_TARGETS_RANGE, roadVehicleComponentsInRange);

		for (const RoadVehicleComponent* roadVehicleComp : roadVehicleComponentsInRange)
		{
			EntityHelper entityHelper(roadVehicleComp->getEntity());

			// Check whether target is destroyed, burning, etc...
			if (!entityHelper.isValidGangsterEventTarget(*entity, false))
				continue;

			// Query Vehicle TransformComponent
			qsf::TransformComponent* targetTC = roadVehicleComp->getEntity().getComponent<qsf::TransformComponent>();
			if (nullptr == targetTC)
				continue;

			// Get vehicle position
			const glm::vec3& targetPosition = targetTC->getPosition();

			// Check whether it is even closer than anything we found until now
			const float squaredDistance = glm::distance2(targetPosition, selfPosition);
			if (squaredDistance < shortestSquaredDistance)
			{
				// New closest target found
				shortestSquaredDistance = squaredDistance;
				closestTargetId = roadVehicleComp->getEntityId();
			}
		}

		// Is there a closest possible victim within the predefined radius?
		if (qsf::isInitialized(closestTargetId))
		{
			mCurrentTargetId = closestTargetId;
			return;
		}

		// Note: this might be an invalid ID (which is okay!)
		mCurrentTargetId = closestTargetId;
	}

	void GangsterKillerPsychoLogic::onUnitsSpotted(std::vector<SpottedUnit>& sightedPoliceUnits)
	{
		// This gangster type just does not care. He tries to harm everyone and everything. Except children.

		// This is important for the event "Running Amok"
		qsf::Entity* gangster = getEntity();
		if (nullptr == gangster)
			return;

		FreeplayEvent* event = gangster->getOrCreateComponent<EventIdComponent>()->getFreeplayEvent();
		if (nullptr != event && qsf::StringHash("em5::RunningAmokEvent") == event->getCampClass().id())
		{
			// RunningAmok Event! Special behavior for this gangster type

			bool anyPoliceUnitOnFootSighted = false;

			for (const SpottedUnit& policeUnit : sightedPoliceUnits)
			{
				if (policeUnit.mEventTag == CommandableComponent::UNITTAG_POLICE_POLICE ||
					policeUnit.mEventTag == CommandableComponent::UNITTAG_POLICE_SEK)
				{
					anyPoliceUnitOnFootSighted = true;
					break;
				}
			}

			if (anyPoliceUnitOnFootSighted)
			{
				// This is a special case of the RunningAmok event: The gangster stops whatever he is doing and attacks the police units!

				qsf::ActionComponent* actionComponent = gangster->getOrCreateComponent<qsf::ActionComponent>();
				if (!actionComponent->getPlan().isEmpty())
				{
					// Gangster got "work" to do

					if (MoveAction::ACTION_ID == actionComponent->getCurrentAction()->getTypeId())
					{
						// Gangster is probably running towards a target. Interrupt that.
						actionComponent->removeAction(*actionComponent->getCurrentAction());
					}
				}

				// Become a "killer taskforce", depending on what weapons are equipped the logic deals with the "fixed" or the "move" type
				changeLogic(qsf::StringHash("em5::GangsterKillerTaskforceLogic"));
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
