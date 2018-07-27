// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/tech/PickupVehicleByTowcarCommand.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/tech/PickupVehicleByTowcarAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/ai/MovementModes.h"
#include "em5/command/CommandContext.h"
#include "em5/component/vehicle/VehicleComponent.h"
#include "em5/game/targetpoint/PickupVehicleByTowcarTargetPointProvider.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/map/EntityHelper.h"

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/map/Map.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/link/link/prototype/ContainerLink.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 PickupVehicleByTowcarCommand::PLUGINABLE_ID = qsf::StringHash("em5::PickupVehicleByTowcarCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 PickupVehicleByTowcarCommand::ACTION_PRIORITY = action::COMMAND_STD;
	const uint32 PickupVehicleByTowcarCommand::ACTION_PRIORITY_AUTOMATISM = action::AUTOMATISM_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PickupVehicleByTowcarCommand::PickupVehicleByTowcarCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "pickup_vehicle_by_towcar";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool PickupVehicleByTowcarCommand::checkAvailable()
	{
		return true;
	}

	bool PickupVehicleByTowcarCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
			return false;

		EntityHelper entityHelper(caller);
		if (!entityHelper.isSquadRoadVehicle())
			return false;

		if (!entityHelper.isContainerEmpty())
			return false;

		return true;
	}

	bool PickupVehicleByTowcarCommand::checkContext(const qsf::game::CommandContext& context)
	{
		qsf::Entity* targetEntity = context.mTargetEntity;
		if (nullptr == targetEntity)
			return false;

		// Must be a vehicle (road/helicopter/shipvehicle)
		const VehicleComponent* vehicleComponent = targetEntity->getComponent<VehicleComponent>();
		if (nullptr == vehicleComponent)
			return false;

		// Let the vehicle tell us whether it can be picked up
		if (!vehicleComponent->getCanBeTowed())
			return false;

		// No persons inside, vehicles are ok (eg the motorboat on the motorboat transporter)
		for (auto& iterator : vehicleComponent->getLinks())
		{
			// Get the contained entity in the vehicle
			qsf::ContainerLink& containerLink = *iterator.second;
			qsf::Entity& personEntity = containerLink.getTargetEntity();

			// Check if person is inside
			if (EntityHelper(personEntity).isEntityPerson())
				return false;
		}

		// Different handling for automatism
		const CommandContext& em5CommandContext = static_cast<const CommandContext&>(context);
		if (em5CommandContext.mIsAutomatism)
		{
			// Is target reserved
			if (!ReserveLogic::checkReservation(*targetEntity, context.mCaller->getId(), ReserveLogic::MOVETO))
				return false;
		}

		return true;
	}

	void PickupVehicleByTowcarCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const PickupVehicleByTowcarAction* pickupVehicleByTowcarAction = actionComponent.getAction<PickupVehicleByTowcarAction>();
		if (nullptr != pickupVehicleByTowcarAction && pickupVehicleByTowcarAction->getTargetId() == context.mTargetEntity->getId())
		{
			// Don't push the same command twice
			return;
		}

		// Clear plan before the actions are pushed
		actionComponent.clearPlan();

		const uint32 usedActionPriority = (static_cast<const CommandContext&>(context).mIsAutomatism ? ACTION_PRIORITY_AUTOMATISM : ACTION_PRIORITY);

		actionComponent.pushAction<MoveAction>(usedActionPriority).init(new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, PickupVehicleByTowcarTargetPointProvider::TARGET_POINT_ID), MovementModes::MOVEMENT_MODE_PLAYER_VEHICLE_ON_EMERGENCY_OPERATIONS);
		actionComponent.pushAction<PickupVehicleByTowcarAction>(action::BLOCKING).init(*context.mTargetEntity);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
