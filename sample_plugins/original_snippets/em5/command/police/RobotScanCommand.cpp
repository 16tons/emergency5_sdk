// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/police/RobotScanCommand.h"
#include "em5/action/police/RobotScanAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"
#include "em5/command/CommandContext.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/game/targetpoint/PullPersonTargetPointProvider.h"
#include "em5/map/EntityHelper.h"

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/logic/action/ActionComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 RobotScanCommand::PLUGINABLE_ID = qsf::StringHash("em5::RobotScanCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 RobotScanCommand::ACTION_PRIORITY = action::COMMAND_STD;
	const uint32 RobotScanCommand::ACTION_PRIORITY_AUTOMATISM = action::AUTOMATISM_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	RobotScanCommand::RobotScanCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "robot_scan";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool RobotScanCommand::checkAvailable()
	{
		return true;
	}

	bool RobotScanCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
			return false;

		return true;
	}

	bool RobotScanCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// Target entity needed
		if (nullptr == context.mTargetEntity)
			return false;

		// Check if target is building
		const BuildingComponent* targetBuildingComponent = context.mTargetEntity->getComponent<BuildingComponent>();
		if (nullptr == targetBuildingComponent)
			return false;

		// Target needs gangsters inside
		if (!targetBuildingComponent->isContainingGangsters())
			return false;

		return true;
	}

	void RobotScanCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const RobotScanAction* robotScanAction = actionComponent.getAction<RobotScanAction>();
		if (nullptr != robotScanAction)
		{
			if (robotScanAction->getTargetEntityId() == context.mTargetEntity->getId())
			{
				// Don't push the same command twice
				return;
			}
		}

		// Clear plan before the actions are pushed
		actionComponent.clearPlan();

		const uint32 usedActionPriority = (static_cast<const CommandContext&>(context).mIsAutomatism ? ACTION_PRIORITY_AUTOMATISM : ACTION_PRIORITY);

		// TODO(mk) Use here a other target point
		actionComponent.pushAction<MoveAction>(usedActionPriority).init(new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, PullPersonTargetPointProvider::TARGET_POINT_ID));
		actionComponent.pushAction<RobotScanAction>(usedActionPriority).init(context.mTargetEntity->getId());
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
