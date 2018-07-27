// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/police/SearchBombInBuildingCommand.h"
#include "em5/action/police/SearchBombInBuildingAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/component/objects/PoliceDogTargetComponent.h"
#include "em5/logic/local/ambulance/RescueDogLeaderLogic.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/game/targetpoint/GetIntoBuildingTargetPointProvider.h"
#include "em5/specs/ActionRangeSpecsGroup.h"
#include "em5/map/EntityHelper.h"
#include "em5/network/NetworkManager.h"
#include "em5/EM5Helper.h"

#include "em5/action/move/MoveAction.h"

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
	const uint32 SearchBombInBuildingCommand::PLUGINABLE_ID = qsf::StringHash("em5::SearchBombInBuildingCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 SearchBombInBuildingCommand::ACTION_PRIORITY = action::COMMAND_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	SearchBombInBuildingCommand::SearchBombInBuildingCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "send_policedog_for_search";
	}

	bool SearchBombInBuildingCommand::checkCallerWithoutPriority(qsf::Entity& caller)
	{
		// Check if unit state is valid
		if (!EntityHelper(caller).isSquadPersonInValidState())
		{
			return false;
		}

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


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool SearchBombInBuildingCommand::checkAvailable()
	{
		return true;
	}

	bool SearchBombInBuildingCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
		{
			return false;
		}

		if (!checkCallerWithoutPriority(caller))
		{
			return false;
		}

		return true;
	}

	bool SearchBombInBuildingCommand::checkContext(const qsf::game::CommandContext& context)
	{
		if (context.mTargetEntity == nullptr)
		{
			return false;
		}

		EntityHelper targetHelper(*context.mTargetEntity);
		if (!targetHelper.isEntityBuilding())
			return false;

		PoliceDogTargetComponent* policeDogTargetComponent = context.mTargetEntity->getComponent<PoliceDogTargetComponent>();
		if (nullptr == policeDogTargetComponent)
			return false;

		return true;
	}

	void SearchBombInBuildingCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const SearchBombInBuildingAction* searchBombInBuildingAction = actionComponent.getAction<SearchBombInBuildingAction>();
		if (nullptr != searchBombInBuildingAction && searchBombInBuildingAction->getTargetEntityId() == context.mTargetEntity->getId())
		{
			// Don't push the same command twice
			return;
		}

		// Clear plan before the actions are pushed
		actionComponent.clearPlan();

		actionComponent.pushAction<MoveAction>(ACTION_PRIORITY).init(new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, GetIntoBuildingTargetPointProvider::TARGET_POINT_ID));
		actionComponent.pushAction<SearchBombInBuildingAction>(action::BLOCKING).init(*context.mTargetEntity);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
