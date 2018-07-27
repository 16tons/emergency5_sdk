// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/ambulance/CancelDogSearchCommand.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/ambulance/CancelDogSearchAction.h"
#include "em5/action/ambulance/CombineDogWithLeaderAction.h"
#include "em5/logic/local/ambulance/RescueDogLeaderLogic.h"
#include "em5/map/EntityHelper.h"
#include "em5/network/NetworkManager.h"
#include "em5/EM5Helper.h"

#include <qsf_game/command/CommandContext.h>

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 CancelDogSearchCommand::PLUGINABLE_ID = qsf::StringHash("em5::CancelDogSearchCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 CancelDogSearchCommand::ACTION_PRIORITY = action::BLOCKING;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	CancelDogSearchCommand::CancelDogSearchCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Second lowest button priority (SendDogForSearchCommand is lower), so the button is always the second place at right (with hotkey 'r')
		mButtonPriority = -40;

		mIconSettings.mShowAsGuiButton = true;
		mIconSettings.mButtonIconPath = "send_dog_cancel";

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "cancel_dog_search";
		mIconSettings.mTooltip = "EM5_Command_32";	// TODO(fw): For police dog this must be "EM5_Command_83"

		// Set the help page name
		mHelpPageName = "TUTORIAL_HELPTEXT_INTERACTION_RESCUEDOG_CALL";
	}

	CancelDogSearchCommand::CancelDogSearchCommand(qsf::game::CommandManager* commandManager, uint32 pluginableID) :
		Command(commandManager, pluginableID)
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool CancelDogSearchCommand::checkAvailable()
	{
		return true;
	}

	bool CancelDogSearchCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
		{
			return false;
		}

		// Check if unit state is valid
		if (!EntityHelper(caller).isSquadPersonInValidState())
		{
			return false;
		}

		if (nullptr == EM5_NETWORK.getMultiplayerClient())
		{
			qsf::GameLogicComponent* gameLogicComponent = caller.getComponent<qsf::GameLogicComponent>();
			if (nullptr == gameLogicComponent)
			{
				return false;
			}

			RescueDogLeaderLogic* rescueDogLeaderLogic = gameLogicComponent->getGameLogic<RescueDogLeaderLogic>();
			if (nullptr == rescueDogLeaderLogic)
			{
				return false;
			}

			if (rescueDogLeaderLogic->getIsDogUnitedWithLeader())
			{
				return false;
			}

			if (nullptr == rescueDogLeaderLogic->getRescueDog())
			{
				return false;
			}
		}
		else
		{
			if (RescueDogLeaderLogic::checkIsRescueDogUnitedWithLeader(caller))
			{
				return false;
			}
		}

		return true;
	}

	bool CancelDogSearchCommand::checkContext(const qsf::game::CommandContext& context)
	{
		if (context.mCaller == nullptr)
			return false;

		qsf::Entity* rescueDog = RescueDogLeaderLogic::getRescueDogEntityFromLeader(*context.mCaller);
		if (nullptr != rescueDog)
		{
			qsf::ActionComponent& actionComponent = rescueDog->getOrCreateComponentSafe<qsf::ActionComponent>();
			const CombineDogWithLeaderAction* combineDogWithLeaderAction = actionComponent.getAction<CombineDogWithLeaderAction>();
			if (nullptr != combineDogWithLeaderAction && actionComponent.getPlan().getActions().size() <= 2)
			{
				// The dog is currently moving to the leader, don't push the action twice
				return false;
			}
		}

		// Self-executed command check
		if (!context.mAllowSelfExecution)
		{
			return false;
		}

		return true;
	}

	void CancelDogSearchCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const CancelDogSearchAction* cancelDogSearchAction = actionComponent.getAction<CancelDogSearchAction>();
		if (nullptr != cancelDogSearchAction)
		{
			// Don't push the same command twice
			return;
		}

		// Clear plan before the actions are pushed
		actionComponent.clearPlan();
		actionComponent.pushAction<CancelDogSearchAction>(ACTION_PRIORITY);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
