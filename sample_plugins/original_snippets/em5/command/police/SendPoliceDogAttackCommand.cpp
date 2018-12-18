// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/police/SendPoliceDogAttackCommand.h"
#include "em5/command/ambulance/SendDogForSearchCommand.h"
#include "em5/action/ambulance/SendDogForSearchAction.h"
#include "em5/action/police/ArrestGangsterAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/logic/local/ambulance/RescueDogLeaderLogic.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/map/EntityHelper.h"
#include "em5/network/NetworkManager.h"
#include "em5/EM5Helper.h"

#include <qsf_game/command/CommandContext.h>

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 SendPoliceDogAttackCommand::PLUGINABLE_ID = qsf::StringHash("em5::SendPoliceDogAttackCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 SendPoliceDogAttackCommand::ACTION_PRIORITY = action::BLOCKING;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	SendPoliceDogAttackCommand::SendPoliceDogAttackCommand(qsf::game::CommandManager* commandManager) :
		SendDogForSearchCommand(commandManager, PLUGINABLE_ID)
	{
		// Slightly lower priority than the more specialized commands like arresting and shooting
		mPriority = 90;

		// Needs to be after "em5::CallPoliceDogBackCommand" so the dog search hot keys are identical to the rescue dog
		mButtonPriority = -30;

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "send_policedog_to_attack";

		// Override the setting from the base class
		mIconSettings.mShowAsGuiButton = false;

		// Set the help page name
		mHelpPageName = "TUTORIAL_HELPTEXT_INTERACTION_POLICEDOG_ATTACK";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool SendPoliceDogAttackCommand::checkAvailable()
	{
		return true;
	}

	bool SendPoliceDogAttackCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
		{
			return false;
		}

		// Check if unit state is valid
		EntityHelper entityHelper(caller);
		if (!entityHelper.isSquadPersonInValidState())
		{
			return false;
		}

		// Not already carrying someone?
		if (entityHelper.isCarryingAnotherPerson())
			return false;

		// Make sure that no equipment for conflicting commands is active
		if (entityHelper.checkIsActiveEquipment(equipment::POLICE_HANDCUFFS_OPEN) || entityHelper.checkIsActiveEquipment(equipment::POLICE_HANDCUFFS))
			return false;

		if (nullptr != getActionComponent(caller).getAction<ArrestGangsterAction>())
			return false;	// Finish arresting

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

			if (!rescueDogLeaderLogic->getIsDogUnitedWithLeader())
			{
				return false;
			}
		}
		else
		{
			if (!RescueDogLeaderLogic::checkIsRescueDogUnitedWithLeader(caller))
			{
				return false;
			}
		}

		return true;
	}

	bool SendPoliceDogAttackCommand::checkContext(const qsf::game::CommandContext& context)
	{
		if (context.mTargetEntity == nullptr)
		{
			return false;
		}

		// Get the person and game logic component
		PersonComponent* targetPersonComponent = context.mTargetEntity->getComponent<PersonComponent>();
		if (nullptr == targetPersonComponent)
			return false;

		// Target person has to be a gangster (don`t use the entityHelper function here)
		if (!targetPersonComponent->getGangsterFlag())
			return false;

		EntityHelper targetHelper(*context.mTargetEntity);
		if (targetHelper.isPersonInjured())
			return false;

		if (targetHelper.isArrested())
			return false;

		return true;
	}

	void SendPoliceDogAttackCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const SendDogForSearchAction* sendDogForSearchAction = actionComponent.getAction<SendDogForSearchAction>();
		if (nullptr != sendDogForSearchAction)
		{
			// Don't push the same command twice
			// Don't abort or redirect the action to an other target, the dog is out, so he has already a job
			return;
		}

		// Clear plan before the actions are pushed
		actionComponent.clearPlan();

		qsf::Entity* targetEntity = context.mTargetEntity;
		if (nullptr != targetEntity)
		{
			actionComponent.pushAction<SendDogForSearchAction>(ACTION_PRIORITY).init(targetEntity, SendDogForSearchAction::UseCase::POLICE_DOG_ATTACK_GANGSTER);

			// Create reservation to avoid multiple targets
			qsf::Entity* rescueDog = RescueDogLeaderLogic::getRescueDogEntityFromLeader(*context.mCaller);
			if (nullptr == rescueDog)
			{
				return; // Error
			}

			ReserveLogic::createReservation<ReserveRescueDogLogic>(*targetEntity, rescueDog->getId());
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
