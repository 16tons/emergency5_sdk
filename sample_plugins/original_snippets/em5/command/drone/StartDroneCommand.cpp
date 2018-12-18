// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/drone/StartDroneCommand.h"
#include "em5/action/drone/StartDroneAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/component/vehicle/VehicleComponent.h"
#include "em5/command/CommandContext.h"
#include "em5/logic/local/drone/DroneTransportLogic.h"

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 StartDroneCommand::PLUGINABLE_ID = qsf::StringHash("em5::StartDroneCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 StartDroneCommand::ACTION_PRIORITY = action::BLOCKING;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	StartDroneCommand::StartDroneCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		mIconSettings.mShowAsGuiButton = true;
		// TODO(sw) For now disabled for multiplay, because the visual aspects are untested if they show up correctly on multiplay client side
		mIconSettings.mEnabledInMultiplay = false;
		mIconSettings.mButtonIconPath = "police_sek-drone_up";
		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "drone_start";
		mIconSettings.mTooltip = "EM5_Command_12";

		// Set the help page name
		mHelpPageName = "TUTORIAL_HELPTEXT_START_DRONE";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool StartDroneCommand::checkAvailable()
	{
		return true;
	}

	bool StartDroneCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
			return false;

		{
			// Check if we currently have loaded an drone
			qsf::GameLogicComponent* gameLogicComponent = caller.getComponent<qsf::GameLogicComponent>();
			if (nullptr == gameLogicComponent)
				return false;

			DroneTransportLogic* droneTransportLogic = gameLogicComponent->getGameLogic<DroneTransportLogic>();
			if (nullptr == droneTransportLogic)
				return false;

			if (!droneTransportLogic->getIsDroneUnitedWithLeader())
				return false;
		}

		VehicleComponent* vehicleComponent = caller.getComponent<VehicleComponent>();
		if (nullptr != vehicleComponent && qsf::isUninitialized(vehicleComponent->getSpecialEntity()))
		{
			return false;
		}

		return true;
	}

	bool StartDroneCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// Self-executed command check
		if (!context.mAllowSelfExecution)
			return false;

		return true;
	}

	void StartDroneCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const StartDroneAction* startDroneAction = actionComponent.getAction<StartDroneAction>();
		if (nullptr != startDroneAction)
		{
			// Don't push the same command twice
			return;
		}

		// Clear plan before the actions are pushed
		actionComponent.clearPlan();
		actionComponent.pushAction<StartDroneAction>(action::BLOCKING, qsf::action::APPEND_TO_BACK);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
