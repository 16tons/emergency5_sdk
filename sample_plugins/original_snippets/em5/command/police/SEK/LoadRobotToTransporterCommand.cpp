// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/police/SEK/LoadRobotToTransporterCommand.h"
#include "em5/command/CommandContext.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/police/SEK/LoadRobotToTransporterAction.h"
#include "em5/action/police/SEK/WaitForRobotAction.h"
#include "em5/logic/local/police/SEKRobotLogic.h"
#include "em5/game/targetpoint/PickupRobotByTransporterTargetPointProvider.h"
#include "em5/map/EntityHelper.h"

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/map/Map.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 LoadRobotToTransporterCommand::PLUGINABLE_ID = qsf::StringHash("em5::LoadRobotToTransporterCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 LoadRobotToTransporterCommand::ACTION_PRIORITY = action::COMMAND_STD;
	const uint32 LoadRobotToTransporterCommand::ACTION_PRIORITY_AUTOMATISM = action::AUTOMATISM_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	LoadRobotToTransporterCommand::LoadRobotToTransporterCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "load_sek_robot_to_transporter";
		// TODO(sw) For now disabled for multiplay, because the visual aspects are untested if they show up correctly on multiplay client side
		mIconSettings.mEnabledInMultiplay = false;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool LoadRobotToTransporterCommand::checkAvailable()
	{
		return true;
	}

	bool LoadRobotToTransporterCommand::checkCaller(qsf::Entity& caller)
	{
		// Check if transporter has waiting action
		qsf::ActionComponent& actionComponent = getActionComponent(caller);
		const WaitForRobotAction* waitForRobotAction = actionComponent.getAction<WaitForRobotAction>();
		if (nullptr == waitForRobotAction)
			return false;

		// Caller has action with a too high priority?
		//if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
		//	return false;

		if (!EntityHelper(caller).isSquadRoadVehicle())
			return false;

		return true;
	}

	bool LoadRobotToTransporterCommand::checkContext(const qsf::game::CommandContext& context)
	{
		if (nullptr == context.mTargetEntity)
			return false;

		qsf::GameLogicComponent* gameLogicComponent = context.mTargetEntity->getComponent<qsf::GameLogicComponent>();
		if (nullptr == gameLogicComponent)
			return false;

		SEKRobotLogic* sekRobotLogic = gameLogicComponent->getGameLogic<SEKRobotLogic>();
		if (nullptr == sekRobotLogic)
			return false;

		// Check if this is the connected sek roboter
		if (sekRobotLogic->getConnectedTransporterId() != context.mCaller->getId())
			return false;

		return true;
	}

	void LoadRobotToTransporterCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const LoadRobotToTransporterAction* loadRobotToTransporterAction = actionComponent.getAction<LoadRobotToTransporterAction>();
		if (nullptr != loadRobotToTransporterAction)
		{
			// Don't push the same command twice
			return;
		}

		// Clear plan before the actions are pushed
		actionComponent.clearPlan();

		const uint32 usedActionPriority = (static_cast<const CommandContext&>(context).mIsAutomatism ? ACTION_PRIORITY_AUTOMATISM : ACTION_PRIORITY);

		actionComponent.pushAction<LoadRobotToTransporterAction>(usedActionPriority).init(*context.mTargetEntity, new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, PickupRobotByTransporterTargetPointProvider::TARGET_POINT_ID));
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
