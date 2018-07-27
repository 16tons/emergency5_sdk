// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/tech/RepairVehicleCommand.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include "em5/action/tech/RepairOrUseAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"
#include "em5/game/targetpoint/OnTouchTargetPointProvider.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/command/CommandContext.h>

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/logic/action/ActionComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 RepairVehicleCommand::PLUGINABLE_ID = qsf::StringHash("em5::RepairVehicleCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 RepairVehicleCommand::ACTION_PRIORITY = action::COMMAND_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	RepairVehicleCommand::RepairVehicleCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "repair_vehicle";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool RepairVehicleCommand::checkAvailable()
	{
		return true;
	}

	bool RepairVehicleCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
			return false;

		// Check if unit state is valid
		EntityHelper entityHelper(caller);
		if (!entityHelper.isSquadPersonInValidState())
			return false;

		return true;
	}

	bool RepairVehicleCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// Target must be a damaged vehicle
		const RoadVehicleComponent* vehicleComponent = context.mTargetEntity->getComponent<RoadVehicleComponent>();
		return (nullptr != vehicleComponent) ? vehicleComponent->canBeRepaired() : false;
	}

	void RepairVehicleCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const RepairOrUseAction* repairOrUseAction = actionComponent.getAction<RepairOrUseAction>();
		if (nullptr != repairOrUseAction && repairOrUseAction->getTargetEntityId() == context.mTargetEntity->getId())
		{
			// Don't push the same command twice
			return;
		}

		// Clear plan before the actions are pushed
		actionComponent.clearPlan();

		actionComponent.pushAction<MoveAction>(ACTION_PRIORITY).init(new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, OnTouchTargetPointProvider::TARGET_POINT_ID));
		actionComponent.pushAction<RepairOrUseAction>(ACTION_PRIORITY).init(*context.mTargetEntity, RepairOrUseAction::STANDING);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
