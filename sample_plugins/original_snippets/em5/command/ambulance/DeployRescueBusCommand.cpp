// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/ambulance/DeployRescueBusCommand.h"
#include "em5/action/ambulance/DeployRescueBusAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/command/CommandContext.h"
#include "em5/component/vehicle/parts/RescueBusComponent.h"
#include "em5/game/units/UnitHelper.h"
#include "em5/game/player/player.h"
#include "em5/logic/HintHelper.h"
#include "em5/map/EntityHelper.h"

#include <qsf/component/link/LinkComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/link/link/prototype/ContainerLink.h>
#include <qsf/input/device/KeyboardDevice.h>
#include <qsf/input/InputSystem.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 DeployRescueBusCommand::PLUGINABLE_ID = qsf::StringHash("em5::DeployRescueBusCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 DeployRescueBusCommand::ACTION_PRIORITY = action::COMMAND_LOW;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	DeployRescueBusCommand::DeployRescueBusCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		mPriority = 50;

		mIconSettings.mShowAsGuiButton = true;
		mIconSettings.mButtonIconPath = "ambulance_deploybus";
		mIconSettings.mTooltip = "EM5_Command_78";
	}

	bool DeployRescueBusCommand::checkCallerWithoutPriority(qsf::Entity& caller)
	{
		// Check if unit state is valid
		EntityHelper entityHelper(caller);
		if (!entityHelper.isSquadVehicleInValidState())
			return false;

		qsf::LinkComponent* linkComponent = caller.getComponent<qsf::LinkComponent>();
		if (nullptr == linkComponent)
			return false;

		RescueBusComponent* rescueBusComponent = linkComponent->getComponentFromEntityOrLinkedChild<RescueBusComponent>();
		if (nullptr == rescueBusComponent)
			return false;

		return true;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool DeployRescueBusCommand::checkAvailable()
	{
		return true;
	}

	bool DeployRescueBusCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, action::COMMAND_STD))
			return false;

		if (!checkCallerWithoutPriority(caller))
			return false;

		return true;
	}

	bool DeployRescueBusCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// Allow this command only via button, i.e. no clicked target position
		return (!context.hasTargetPosition());
	}

	void DeployRescueBusCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const DeployRescueBusAction* deployRescueBusAction = actionComponent.getAction<DeployRescueBusAction>();
		if (nullptr != deployRescueBusAction)
		{
			// Don't push the same command twice
			return;
		}

		qsf::LinkComponent* linkComponent = context.mCaller->getComponent<qsf::LinkComponent>();
		if (nullptr == linkComponent)
			return;

		RescueBusComponent* rescueBusComponent = linkComponent->getComponentFromEntityOrLinkedChild<RescueBusComponent>();
		if (nullptr == rescueBusComponent)
			return;

		if (!rescueBusComponent->isExtended() && rescueBusComponent->checkCollisionForExtending())
		{
			// Collision found
			const CommandContext& em5CommandContext = static_cast<const CommandContext&>(context);
			if (!em5CommandContext.mIsAutomatism)
			{
				// Trigger a hint for the player
				uint32 playerIndex = (nullptr != em5CommandContext.mPlayer ? em5CommandContext.mPlayer->getPlayerIndex() : qsf::getUninitialized<uint32>());
				HintHelper::showHint("EM5X_HINT_IMT_NOSPACE", playerIndex, qsf::Time::fromSeconds(30.0f));
			}
			return; // No action is pushed
		}

		if (isNoQueueingCommand(*context.mCaller))
		{
			// Clear plan before the actions are pushed
			actionComponent.clearPlan();
		}

		actionComponent.pushAction<DeployRescueBusAction>(action::BLOCKING);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	bool DeployRescueBusCommand::isNoQueueingCommand(qsf::Entity& firefighterEntity)
	{
		if (QSF_INPUT.getKeyboard().anyShiftPressed())
		{
			return false;
		}

		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
