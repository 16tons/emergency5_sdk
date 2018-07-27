// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/base/ExitBuildingCommand.h"
#include "em5/action/base/ExitBuildingAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/component/building/BuildingComponent.h"

#include <qsf_game/command/CommandContext.h>

#include <qsf/logic/action/ActionComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 ExitBuildingCommand::PLUGINABLE_ID = qsf::StringHash("em5::ExitBuildingCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 ExitBuildingCommand::ACTION_PRIORITY = action::COMMAND_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	ExitBuildingCommand::ExitBuildingCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "exit_building";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool ExitBuildingCommand::checkAvailable()
	{
		return true;
	}

	bool ExitBuildingCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		return checkCallerActionPriority(caller, ACTION_PRIORITY);
	}

	bool ExitBuildingCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// Self-executed command check
		if (!context.mAllowSelfExecution)
			return false;

		// Target needs to have a building component
		const BuildingComponent* building = context.mTargetEntity->getComponent<BuildingComponent>();
		if (nullptr == building)
			return false;

		// Anyone inside?
		if (building->getLinks().empty())
			return false;

		return true;
	}

	void ExitBuildingCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);
		actionComponent.clearPlan();

		// Add the exit building action
		actionComponent.pushAction<ExitBuildingAction>(ACTION_PRIORITY);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
