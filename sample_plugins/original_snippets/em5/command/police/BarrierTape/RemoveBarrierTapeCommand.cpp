// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/police/BarrierTape/RemoveBarrierTapeCommand.h"
#include "em5/component/objects/BarrierTapeComponent.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/police/ClearBarrierAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/command/CommandContext.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/map/EntityHelper.h"

#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>

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
	const uint32 RemoveBarrierTapeCommand::PLUGINABLE_ID = qsf::StringHash("em5::RemoveBarrierTapeCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 RemoveBarrierTapeCommand::ACTION_PRIORITY = action::COMMAND_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	RemoveBarrierTapeCommand::RemoveBarrierTapeCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "barrier_tape";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool RemoveBarrierTapeCommand::checkAvailable()
	{
		return true;
	}

	bool RemoveBarrierTapeCommand::checkCallerWithoutPriority(qsf::Entity& caller)
	{
		EntityHelper entityHelper(caller);
		if (!entityHelper.isSquadPersonInValidState())
			return false;

		return true;
	}

	bool RemoveBarrierTapeCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriorityNoBlocking(caller, action::BLOCKING))
			return false;

		if (!checkCallerWithoutPriority(caller))
			return false;

		return true;
	}

	bool RemoveBarrierTapeCommand::checkContext(const qsf::game::CommandContext& context)
	{
		if (nullptr == context.mTargetEntity)
			return false;

		// We need Barriertapes as target
		BarrierTapeComponent* barriertapeComponent = context.mTargetEntity->getComponent<BarrierTapeComponent>();
		if (nullptr == barriertapeComponent)
			return false;

		// Only finished barriertapes
		if (!barriertapeComponent->isFinishedBuilding())
			return false;

		// Valid. This commando starts an commando mode. This checks if the target position is valid.
		return true;
	}

	void RemoveBarrierTapeCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const ClearBarrierAction* clearBarrierAction = actionComponent.getAction<ClearBarrierAction>();
		if (nullptr != clearBarrierAction)
		{
			if (clearBarrierAction->getTargetEntityId() == context.mTargetEntity->getId())
			{
				// Don't push the same command twice
				return;
			}
		}

		// We need the barriertapeComponent to get our target
		BarrierTapeComponent* barriertapeComponent = context.mTargetEntity->getComponent<BarrierTapeComponent>();
		if (nullptr == barriertapeComponent)
			return;

		// Clear plan before the actions are pushed
		actionComponent.clearPlan();

		actionComponent.pushAction<MoveAction>(ACTION_PRIORITY).init(new qsf::ai::ReachSinglePointGoal(qsf::logic::TargetPoint(barriertapeComponent->getBarrierCenter(), false, 0.5f, 0.5f)));
		actionComponent.pushAction<PlayAnimationAction>(action::BLOCKING).init(AnimationHelper(*context.mCaller).getAnimationPoliceBarrierTape03(), false);
		actionComponent.pushAction<ClearBarrierAction>(action::BLOCKING).init(*context.mTargetEntity, qsf::Time::fromSeconds(2.0f));
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
