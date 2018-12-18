// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/fire/AreaExtinguishCommand.h"
#include "em5/action/ActionPriority.h"
#include "em5/command/CommandContext.h"
#include "em5/command/mode/AreaExtinguishCommandMode.h"
#include "em5/logic/local/firefighters/CommandoCarLogic.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/QsfGameHelper.h>

#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>
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
	const uint32 AreaExtinguishCommand::PLUGINABLE_ID = qsf::StringHash("em5::AreaExtinguishCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 AreaExtinguishCommand::ACTION_PRIORITY = action::COMMAND_STD;
	const uint32 AreaExtinguishCommand::ACTION_PRIORITY_AUTOMATISM = action::AUTOMATISM_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	AreaExtinguishCommand::AreaExtinguishCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		mIconSettings.mShowAsGuiButton = true;
		mIconSettings.mButtonIconPath = "firefighters_plane";
		mIconSettings.mTooltip = "EM5_Command_11";

		// No mouse cursor for this command, its only a button what triggers an command mode

		// Set the help page name
		mHelpPageName = "TUTORIAL_HELPTEXT_SEND_FIREFIGHTER_PLANE";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool AreaExtinguishCommand::checkAvailable()
	{
		return true;
	}

	bool AreaExtinguishCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
			return false;

		// Check if the command cool down is finish
		qsf::GameLogicComponent* gameLogicComponent = caller.getComponent<qsf::GameLogicComponent>();
		if (nullptr == gameLogicComponent)
			return false;

		CommandoCarLogic* commandoCarLogic = gameLogicComponent->getGameLogic<CommandoCarLogic>();
		if (nullptr == commandoCarLogic)
			return false;

		if (!commandoCarLogic->isAreaExtinguishSkillReady())
			return false;

		return true;
	}

	bool AreaExtinguishCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// Self-executed command check
		if (!context.mAllowSelfExecution)
			return false;

		return true;
	}

	void AreaExtinguishCommand::execute(const qsf::game::CommandContext& context)
	{
		// TODO(mk) check if the command mode is currently active

		// Create a command mode, this has future validation checks
		AreaExtinguishCommandMode* areaExtinguishCommandMode = new AreaExtinguishCommandMode();
		areaExtinguishCommandMode->initialize(context.mCaller);
		QSFGAME_COMMAND.setCommandMode(*areaExtinguishCommandMode);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
