// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/tech/UnloadBoatFromTransporterCommand.h"
#include "em5/command/CommandContext.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/tech/UnloadBoatFromTransporterAction.h"
#include "em5/ai/MovementModes.h"
#include "em5/component/objects/BoatDropPointComponent.h"
#include "em5/game/targetpoint/ApproachBoatDropPointTargetPointProvider.h"
#include "em5/game/targetpoint/UnloadBoatFromTransporterTargetPointProvider.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include "em5/map/EntityHelper.h"

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/map/Map.h>
#include <qsf/logic/action/ActionComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 UnloadBoatFromTransporterCommand::PLUGINABLE_ID = qsf::StringHash("em5::UnloadBoatFromTransporterCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 UnloadBoatFromTransporterCommand::ACTION_PRIORITY = action::COMMAND_STD;
	const uint32 UnloadBoatFromTransporterCommand::ACTION_PRIORITY_AUTOMATISM = action::AUTOMATISM_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	UnloadBoatFromTransporterCommand::UnloadBoatFromTransporterCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "unload_boat_from_transporter";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool UnloadBoatFromTransporterCommand::checkAvailable()
	{
		return true;
	}

	bool UnloadBoatFromTransporterCommand::checkCaller(qsf::Entity& caller)
	{
		RoadVehicleComponent* roadVehicleComponent = caller.getComponent<RoadVehicleComponent>();

		if (nullptr == roadVehicleComponent)
			return false;

		// Check if we have a boat in special entity
		if (qsf::isUninitialized(roadVehicleComponent->getSpecialEntity()))
			return false;

		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
			return false;

		if (!EntityHelper(caller).isSquadRoadVehicle())
			return false;

		return true;
	}

	bool UnloadBoatFromTransporterCommand::checkContext(const qsf::game::CommandContext& context)
	{
		if (nullptr == context.mTargetEntity)
			return false;

		// Is the target an unload spot?
		BoatDropPointComponent* boatDropPointComponent = context.mTargetEntity->getComponent<BoatDropPointComponent>();
		if (nullptr == boatDropPointComponent)
			return false;

		// TODO(ag) Check if reserved

		return true;
	}

	void UnloadBoatFromTransporterCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const UnloadBoatFromTransporterAction* unloadBoatFromTransporterAction = actionComponent.getAction<UnloadBoatFromTransporterAction>();
		if (nullptr != unloadBoatFromTransporterAction && unloadBoatFromTransporterAction->getTargetId() == context.mTargetEntity->getId())
		{
			// Don't push the same command twice
			return;
		}

		// Clear plan before the actions are pushed
		actionComponent.clearPlan();

		const uint32 usedActionPriority = (static_cast<const CommandContext&>(context).mIsAutomatism ? ACTION_PRIORITY_AUTOMATISM : ACTION_PRIORITY);

		// First only get near the boat drop point
		qsf::ai::NavigationGoal* goal = new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, ApproachBoatDropPointTargetPointProvider::TARGET_POINT_ID, false);
		actionComponent.pushAction<MoveAction>(usedActionPriority).init(goal, MovementModes::MOVEMENT_MODE_PLAYER_VEHICLE_ON_EMERGENCY_OPERATIONS);

		// Explicitly set the parameter "checkTargetPositionForLocalRouter" to "false"
		//  -> We don't want to check the target position, as this would most probably fail with our narrow boat drop points
		goal = new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, UnloadBoatFromTransporterTargetPointProvider::TARGET_POINT_ID, false);
		actionComponent.pushAction<MoveAction>(usedActionPriority).init(goal, MovementModes::MOVEMENT_MODE_PLAYER_VEHICLE_ON_EMERGENCY_OPERATIONS);
		actionComponent.pushAction<UnloadBoatFromTransporterAction>(action::BLOCKING).init(*context.mTargetEntity);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
