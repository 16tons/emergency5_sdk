// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/police/BarrierTape/PlaceBarrierTapeCommand.h"
#include "em5/command/mode/PlaceBarrierTapeCommandMode.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/police/ClearBarrierAction.h"
#include "em5/action/police/PlaceBarrierPoleAction.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/command/CommandContext.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/command/CommandContext.h>
#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/QsfGameHelper.h>

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
	const uint32 PlaceBarrierTapeCommand::PLUGINABLE_ID = qsf::StringHash("em5::PlaceBarrierTapeCommand");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PlaceBarrierTapeCommand::PlaceBarrierTapeCommand(qsf::game::CommandManager* commandManager) :
		GetEquipmentCommand(commandManager, PLUGINABLE_ID)
	{
		mButtonPriority = 0;

		// This command is triggered from a HUD element, we need an icon
		mIconSettings.mShowAsGuiButton = true;
		mIconSettings.mButtonIconPath = "police_barrier_tape";

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "barrier_tape";

		mEquipmentName = equipment::POLICE_BARRIER_TAPE_ROLL;

		mIconSettings.mTooltip = "EM5_Command_37";

		// Set the help page name
		mHelpPageName = "TUTORIAL_HELPTEXT_POLICEMAN_STOPPER";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool PlaceBarrierTapeCommand::checkAvailable()
	{
		return true;
	}

	bool PlaceBarrierTapeCommand::checkCallerWithoutPriority(qsf::Entity& caller)
	{
		// Check if command mode is already active
		if (QSFGAME_COMMAND.getCommandMode().getId() == PlaceBarrierTapeCommandMode::PLUGINABLE_ID)
			return false;

		qsf::ActionComponent* actionComponent = caller.getComponent<qsf::ActionComponent>();
		if (nullptr != actionComponent)
		{
			// Check if there is an action to place the barrier on the way (corresponding command mode ends already after sending second pole on it's way).
			if (nullptr != actionComponent->getActionByTypeId(PlaceBarrierPoleAction::ACTION_ID))
				return false;

			// Clearing the barrier is also not allowed (might mess up states).
			if (nullptr != actionComponent->getActionByTypeId(ClearBarrierAction::ACTION_ID))
				return false;
		}

		return true;
	}

	bool PlaceBarrierTapeCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriorityNoBlocking(caller, action::BLOCKING))
			return false;

		if (!checkCallerWithoutPriority(caller))
			return false;

		return true;
	}

	bool PlaceBarrierTapeCommand::checkContext(const qsf::game::CommandContext& context)
	{
		if (nullptr != context.mTargetEntity || context.mTargetPosition.is_initialized())
		{
			return false;
		}

		// Valid. This commando starts an commando mode. This checks if the target position is valid.
		return true;
	}

	void PlaceBarrierTapeCommand::execute(const qsf::game::CommandContext& context)
	{
		GetEquipmentCommand::execute(context);

		// Create a command mode, this has future validation checks
		PlaceBarrierTapeCommandMode* placeBarrierTapeCommandMode = new PlaceBarrierTapeCommandMode();
		placeBarrierTapeCommandMode->initialize(*context.mCaller);
		QSFGAME_COMMAND.setCommandMode(*placeBarrierTapeCommandMode);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
