// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/fire/StopPullPersonCommand.h"
#include "em5/action/fire/PullPersonAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"
#include "em5/ai/MovementModes.h"
#include "em5/command/CommandContext.h"
#include "em5/command/mode/DefaultCommandMode.h"
#include "em5/map/EntityHelper.h"

#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>

#include <qsf/component/placement/WalkableComponent.h>
#include <qsf/renderer/terrain/TerrainComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 StopPullPersonCommand::PLUGINABLE_ID = qsf::StringHash("em5::StopPullPersonCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 StopPullPersonCommand::ACTION_PRIORITY = action::COMMAND_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	StopPullPersonCommand::StopPullPersonCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "stop_pull_person";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool StopPullPersonCommand::checkAvailable()
	{
		return true;
	}

	bool StopPullPersonCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
			return false;

		// Check if unit state is valid
		EntityHelper entityHelper(caller);
		if (!entityHelper.isSquadPersonInValidState())
			return false;

		// Must pulling a other person
		uint64 pulledPersonId = EntityHelper(caller).getCarryTarget(EntityHelper::PULL_PERSON);
		qsf::Entity* pulledEntity = caller.getMap().getEntityById(pulledPersonId);
		if (pulledEntity == nullptr)
			return false;

		// ... but not a contaminated person
		if (EntityHelper(*pulledEntity).isPersonContaminated())
			return false;

		return true;
	}

	bool StopPullPersonCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// We need a valid target position or click onto the firefighter himself to just let go in place
		if (!context.mAllowSelfExecution)
		{
			CommandContext::EvaluationReason evaluationReason = static_cast<const CommandContext&>(context).mEvaluationReason;
			if (evaluationReason == CommandContext::EVALUATIONREASON_SHOWCOMMAND_ABOVE_ENTITY)
			{
				// Don't show the icon in case we only show preview icons, it looks strange for this command
				return false;
			}

			if (context.hasTargetPosition())
			{
				// Only allow targets for walking
				if (nullptr == context.mTargetEntity->getComponent<qsf::WalkableComponent>() &&
					nullptr == context.mTargetEntity->getComponent<qsf::TerrainComponent>())
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}

		return true;
	}

	void StopPullPersonCommand::execute(const qsf::game::CommandContext& context)
	{
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const PullPersonAction* pullPersonAction = actionComponent.getAction<PullPersonAction>();
		if (nullptr != pullPersonAction && pullPersonAction->getTargetEntityId() == context.mTargetEntity->getId())
		{
			// Don't push the same command twice
			return;
		}

		// Clear plan before the actions are pushed
		actionComponent.clearPlan();

		if (!context.mAllowSelfExecution && context.hasTargetPosition())
		{
			DefaultCommandMode::showVisualCommandFeedback(context);

			qsf::logic::TargetPoint targetPoint(context.mTargetPosition.get(), false, 0.5f, 0.5f);
			actionComponent.pushAction<MoveAction>(ACTION_PRIORITY).init(new qsf::ai::ReachSinglePointGoal(targetPoint), MovementModes::MOVEMENT_MODE_WALK_BACKWARDS);
		}
		actionComponent.pushAction<PullPersonAction>(action::BLOCKING).init(context.mTargetEntity->getId(), PullPersonAction::STOP);
	}

	bool StopPullPersonCommand::highlightTarget() const
	{
		// No command target highlighting please
		return false;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
