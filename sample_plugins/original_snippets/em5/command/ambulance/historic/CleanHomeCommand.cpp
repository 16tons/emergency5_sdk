// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/ambulance/historic/CleanHomeCommand.h"
#include "em5/component/objects/HistoricPlagueBuildingComponent.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/ambulance/historic/CleanHomeAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/game/targetpoint/GetIntoBuildingTargetPointProvider.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/map/EntityHelper.h"
#include "em5/EM5Helper.h"

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

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
	const uint32 CleanHomeCommand::PLUGINABLE_ID = qsf::StringHash("em5::CleanHomeCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 CleanHomeCommand::ACTION_PRIORITY = action::COMMAND_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	CleanHomeCommand::CleanHomeCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "enter_building";

		// TODO(mk) Find final helptext or remove this code
		// Set the help page name
		mHelpPageName = "TUTORIAL_HELPTEXT_INTERACTION_RESCUEDOG_CALL";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool CleanHomeCommand::checkAvailable()
	{
		return true;
	}

	bool CleanHomeCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
		{
			return false;
		}

		// Check if unit state is valid
		if (!EntityHelper(caller).isUnitInValidState())
			return false;

		return true;
	}

	bool CleanHomeCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// Self-executed command check
		if (context.mTargetEntity == nullptr)
			return false;

		EntityHelper entityHelper(*context.mTargetEntity);
		if (!entityHelper.isBuildingInValidState())
			return false;

		// Check if the building is able to spawn an entity
		if (!HistoricPlagueBuildingComponent::getInstance().getMustClean(context.mTargetEntity->getId()))
			return false;

		qsf::GameLogicComponent* targetGameLogicComponent = context.mTargetEntity->getOrCreateComponent<qsf::GameLogicComponent>();
		if (nullptr != targetGameLogicComponent)
		{
			ReserveWaitForFinishLogic* reserveWaitForFinishLogic = targetGameLogicComponent->getGameLogic<ReserveWaitForFinishLogic>();
			if (nullptr != reserveWaitForFinishLogic)
			{
				// Some other entity is interacting with our target
				return false;
			}
		}
		return true;
	}

	void CleanHomeCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const CleanHomeAction* cleanHomeAction = actionComponent.getAction<CleanHomeAction>();
		if (nullptr != cleanHomeAction && cleanHomeAction->getTargetEntityId() == context.mTargetEntity->getId())
		{
			// Don't push the same command twice
			return;
		}

		// Clear plan before the actions are pushed
		actionComponent.clearPlan();
		actionComponent.pushAction<MoveAction>(ACTION_PRIORITY).init(new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, GetIntoBuildingTargetPointProvider::TARGET_POINT_ID));
		actionComponent.pushAction<CleanHomeAction>(action::BLOCKING).init(*context.mTargetEntity);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
