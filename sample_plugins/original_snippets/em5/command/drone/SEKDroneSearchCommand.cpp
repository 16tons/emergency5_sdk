// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/drone/SEKDroneSearchCommand.h"
#include "em5/action/drone/SEKDroneSearchAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/component/vehicle/VehicleComponent.h"
#include "em5/logic/local/drone/DroneTransportLogic.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/command/CommandContext.h>

#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/Entity.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 SEKDroneSearchCommand::PLUGINABLE_ID = qsf::StringHash("em5::SEKDroneSearchCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 SEKDroneSearchCommand::ACTION_PRIORITY = action::BLOCKING;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	SEKDroneSearchCommand::SEKDroneSearchCommand(qsf::game::CommandManager* commandManager) :
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
		mHelpPageName = "TUTORIAL_HELPTEXT_START_DRONE_SEK";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool SEKDroneSearchCommand::checkAvailable()
	{
		return true;
	}

	bool SEKDroneSearchCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
			return false;

		qsf::GameLogicComponent* gameLogicComponent = caller.getComponent<qsf::GameLogicComponent>();
		if (nullptr == gameLogicComponent)
			return false;

		DroneTransportLogic* droneTransportLogic = gameLogicComponent->getGameLogic<DroneTransportLogic>();
		if (nullptr == droneTransportLogic)
			return false;

		// Currently we are not interested in the united state between transporter and drone. Commando decide later what should happen.

		return true;
	}

	bool SEKDroneSearchCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// Self-executed command check
		if (!context.mAllowSelfExecution)
			return false;

		return true;
	}

	void SEKDroneSearchCommand::execute(const qsf::game::CommandContext& context)
	{
		// Here we have a other approach, in case we sek drone isn't started already, we start it.
		// Is the drone in air, we switch the target but let the current action on the action stack
		qsf::GameLogicComponent* gameLogicComponent = context.mCaller->getComponent<qsf::GameLogicComponent>();
		DroneTransportLogic* droneTransportLogic = gameLogicComponent->getGameLogic<DroneTransportLogic>();

		// Access the drone's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*droneTransportLogic->getDroneEntity());

		if (droneTransportLogic->getIsDroneUnitedWithLeader())
		{
			// Clear plan before the actions are pushed

			actionComponent.clearPlan();
			actionComponent.pushAction<SEKDroneSearchAction>(action::BLOCKING, qsf::action::APPEND_TO_BACK).init(context.mCaller);
		}
		else
		{
			// Else, pick the SEKDroneSearchAction and give new instruction
			SEKDroneSearchAction* sekDroneSearchAction = actionComponent.getAction<SEKDroneSearchAction>();
			if (nullptr != sekDroneSearchAction)
			{
				sekDroneSearchAction->searchNewTarget();
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
