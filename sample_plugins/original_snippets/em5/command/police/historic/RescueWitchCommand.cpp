// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/police/historic/RescueWitchCommand.h"
#include "em5/action/police/historic/RescueWitchAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/ai/MovementModes.h"
#include "em5/component/objects/StakeComponent.h"
#include "em5/game/targetpoint/GotoObjectTargetPointProvider.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/command/CommandContext.h>

#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>

#include <qsf/component/link/LinkComponent.h>
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
	const uint32 RescueWitchCommand::PLUGINABLE_ID = qsf::StringHash("em5::RescueWitchCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 RescueWitchCommand::ACTION_PRIORITY = action::COMMAND_LOW;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	RescueWitchCommand::RescueWitchCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "rescue_witch";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool RescueWitchCommand::checkAvailable()
	{
		return true;
	}

	bool RescueWitchCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
			return false;

		// Check if unit state is valid
		EntityHelper entityHelper(caller);
		if (!entityHelper.isSquadPersonInValidState())
			return false;

		if (entityHelper.isCarryingAnotherPerson())
			return false;

		return true;
	}

	bool RescueWitchCommand::checkContext(const qsf::game::CommandContext& context)
	{
		if (context.mTargetEntity == nullptr)
		{
			return false;
		}

		EntityHelper targetHelper(*context.mTargetEntity);

		StakeComponent* stakeComponent = context.mTargetEntity->getComponent<StakeComponent>();
		if (nullptr == stakeComponent)
			return false;

		if (!stakeComponent->isActive())
			return false;

		if (stakeComponent->getWitchEntity() == qsf::getUninitialized<uint64>())
			return false;

		// Is target reserved
		if (!ReserveLogic::checkReservation(*context.mTargetEntity, context.mCaller->getId(), ReserveLogic::MOVETO))
			return false;

		return true;
	}

	void RescueWitchCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const RescueWitchAction* rescueWitchAction = actionComponent.getAction<RescueWitchAction>();
		if (nullptr != rescueWitchAction)
		{
			if (rescueWitchAction->getTargetEntityId() == context.mTargetEntity->getId())
			{
				// Don't push the same command twice
				return;
			}
		}

		actionComponent.clearPlan();

		// Move to given position
		qsf::Transform targetTransform = context.mTargetEntity->getComponent<StakeComponent>()->getActionTransform();
		qsf::logic::TargetPoint moveTargetConfig;
		moveTargetConfig.mPosition = targetTransform.getPosition();
		moveTargetConfig.mOrientation = targetTransform.getRotation();
		moveTargetConfig.mTolerance = 1.0f;

		actionComponent.pushAction<MoveAction>(action::COMMAND_LOW, qsf::action::APPEND_TO_BACK).init(new qsf::ai::ReachSinglePointGoal(moveTargetConfig), MovementModes::MOVEMENT_MODE_DEFAULT);
		actionComponent.pushAction<RescueWitchAction>(ACTION_PRIORITY).init(*context.mTargetEntity);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
