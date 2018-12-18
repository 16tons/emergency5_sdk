// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/police/SEK/UnloadRobotFromTransporterCommand.h"
#include "em5/command/CommandContext.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/FullBrakeAction.h"
#include "em5/action/police/SEK/UnloadRobotFromTransporterAction.h"
#include "em5/component/vehicle/VehicleComponent.h"
#include "em5/map/EntityHelper.h"

#include <qsf/map/Map.h>
#include <qsf/component/move/MovableComponent.h>
#include <qsf/logic/action/ActionComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 UnloadRobotFromTransporterCommand::PLUGINABLE_ID = qsf::StringHash("em5::UnloadRobotFromTransporterCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 UnloadRobotFromTransporterCommand::ACTION_PRIORITY = action::BLOCKING;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	UnloadRobotFromTransporterCommand::UnloadRobotFromTransporterCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		mIconSettings.mShowAsGuiButton = true;
		mIconSettings.mButtonIconPath = "exit_vehicle";
		// TODO(sw) For now disabled for multiplay, because the visual aspects are untested if they show up correctly on multiplay client side
		mIconSettings.mEnabledInMultiplay = false;
		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "unload_sek_robot_from_transporter";
		mIconSettings.mTooltip = "EM5_Command_22";

		// Set the help page name
		mHelpPageName = "TUTORIAL_HELPTEXT_EXIT_TSEKR";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool UnloadRobotFromTransporterCommand::checkAvailable()
	{
		return true;
	}

	bool UnloadRobotFromTransporterCommand::checkCaller(qsf::Entity& caller)
	{
		VehicleComponent* vehicleComponent = caller.getComponent<VehicleComponent>();
		if (nullptr == vehicleComponent)
			return false;

		// Check if transport car is currently loaded with robot
		if (vehicleComponent->getLinks().empty())
			return false;

		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
			return false;

		if (!EntityHelper(caller).isSquadRoadVehicle())
			return false;

		return true;
	}

	bool UnloadRobotFromTransporterCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// Self-executed command check
		if (!context.mAllowSelfExecution)
			return false;

		// Check if there is enough space to unload

		return true;
	}

	void UnloadRobotFromTransporterCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command is already in the action plan, return in this case without clearing the action plan
		const UnloadRobotFromTransporterAction* unloadRobotFromTransporterAction = actionComponent.getAction<UnloadRobotFromTransporterAction>();
		if (nullptr != unloadRobotFromTransporterAction)
		{
			// Don't push the same command twice
			return;
		}

		// Clear plan before the actions are pushed
		actionComponent.clearPlan();

		// Is this entity moving?
		const qsf::MovableComponent* movableComponent = context.mCaller->getComponent<qsf::MovableComponent>();
		if (nullptr != movableComponent && movableComponent->isMoving())
		{
			actionComponent.pushAction<FullBrakeAction>(ACTION_PRIORITY);
		}

		actionComponent.pushAction<UnloadRobotFromTransporterAction>(ACTION_PRIORITY);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
