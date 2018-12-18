// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/base/EnterBuildingCommand.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/action/base/EnterBuildingAction.h"
#include "em5/action/base/ExitBuildingAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"
#include "em5/game/targetpoint/GetIntoBuildingTargetPointProvider.h"

#include <qsf_game/command/CommandContext.h>

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
	const uint32 EnterBuildingCommand::PLUGINABLE_ID = qsf::StringHash("em5::EnterBuildingCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 EnterBuildingCommand::ACTION_PRIORITY = action::COMMAND_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	EnterBuildingCommand::EnterBuildingCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "enter_building";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool EnterBuildingCommand::checkAvailable()
	{
		return true;
	}

	bool EnterBuildingCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		return checkCallerActionPriority(caller, ACTION_PRIORITY);
	}

	bool EnterBuildingCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// Target entity needed
		if (nullptr == context.mTargetEntity)
			return false;

		// Target needs to have a building component
		if (nullptr == context.mTargetEntity->getComponent<BuildingComponent>())
			return false;

		return true;
	}

	void EnterBuildingCommand::execute(const qsf::game::CommandContext& context)
	{
		// TODO(mk) Add check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);
		actionComponent.clearPlan();
		actionComponent.pushAction<MoveAction>(ACTION_PRIORITY).init(new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, GetIntoBuildingTargetPointProvider::TARGET_POINT_ID));
		actionComponent.pushAction<EnterBuildingAction>(ACTION_PRIORITY).init(*context.mTargetEntity);
		actionComponent.pushAction<ExitBuildingAction>(ACTION_PRIORITY);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
