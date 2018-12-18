// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/ambulance/historic/SendPlagueVictimHomeCommand.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/ambulance/historic/SendPlagueVictimHomeAction.h"
#include "em5/action/base/EnterBuildingAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/game/targetpoint/UsePepperSprayTargetPointProvider.h"
#include "em5/specs/ActionRangeSpecsGroup.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/map/EntityHelper.h"
#include "em5/EM5Helper.h"

#include <qsf_game/command/CommandContext.h>

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

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
	const uint32 SendPlagueVictimHomeCommand::PLUGINABLE_ID = qsf::StringHash("em5::SendPlagueVictimHomeCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 SendPlagueVictimHomeCommand::ACTION_PRIORITY = action::COMMAND_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	SendPlagueVictimHomeCommand::SendPlagueVictimHomeCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "dismiss_patient";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool SendPlagueVictimHomeCommand::checkAvailable()
	{
		return true;
	}

	bool SendPlagueVictimHomeCommand::checkCaller(qsf::Entity& caller)
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

		return true;
	}

	bool SendPlagueVictimHomeCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// Self-executed command check
		if (context.mTargetEntity == nullptr)
			return false;

		EntityHelper entityHelper(*context.mTargetEntity);
		if (!entityHelper.isCivilPerson())
			return false;

		if (qsf::isUninitialized(entityHelper.getFreeplayEventId()))
			return false;

		// Check if the person was already sent back to his home
		qsf::ActionComponent& actionComponent = context.mTargetEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
		if (nullptr != actionComponent.getAction<EnterBuildingAction>())
			return false;

		qsf::GameLogicComponent* targetGameLogicComponent = context.mTargetEntity->getOrCreateComponent<qsf::GameLogicComponent>();
		if (nullptr != targetGameLogicComponent)
		{
			ReserveDoctorLogic* reserveDoctorLogic = targetGameLogicComponent->getGameLogic<ReserveDoctorLogic>();
			if (nullptr != reserveDoctorLogic)
			{
				// Some other entity is interacting with our target
				return false;
			}
		}

		return true;
	}

	void SendPlagueVictimHomeCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const SendPlagueVictimHomeAction* sendPlagueVictimHomeAction = actionComponent.getAction<SendPlagueVictimHomeAction>();
		if (nullptr != sendPlagueVictimHomeAction && sendPlagueVictimHomeAction->getTargetEntityId() == context.mTargetEntity->getId())
		{
			// Don't push the same command twice
			return;
		}

		// Clear plan before the actions are pushed
		actionComponent.clearPlan();
		MoveAction& moveAction = actionComponent.pushAction<MoveAction>(ACTION_PRIORITY);
		moveAction.init(new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, UsePepperSprayTargetPointProvider::TARGET_POINT_ID));
		moveAction.forceFollowedEntityToWait(ActionRangeSpecsGroup::getInstanceSafe().getUsePepperSprayDistance(), qsf::Time::fromSeconds(5.0f));
		actionComponent.pushAction<SendPlagueVictimHomeAction>(action::BLOCKING).init(*context.mTargetEntity);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
