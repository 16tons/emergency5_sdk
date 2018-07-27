// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/police/DefuseBombCommand.h"
#include "em5/action/police/DefuseBombAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/ai/MovementModes.h"
#include "em5/component/objects/BombComponent.h"
#include "em5/game/targetpoint/GotoBombTargetPointProvider.h"
#include "em5/map/EntityHelper.h"

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
	const uint32 DefuseBombCommand::PLUGINABLE_ID = qsf::StringHash("em5::DefuseBombCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 DefuseBombCommand::ACTION_PRIORITY = action::COMMAND_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	DefuseBombCommand::DefuseBombCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "defuse_bomb";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool DefuseBombCommand::checkAvailable()
	{
		return true;
	}

	bool DefuseBombCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
			return false;

		// Check if unit state is valid
		EntityHelper entityHelper(caller);
		if (!entityHelper.isUnitInValidState())
			return false;

		return true;
	}

	bool DefuseBombCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// We need a target entity
		if (nullptr == context.mTargetEntity)
			return false;

		// Check if target is a bomb
		const BombComponent* bombComponent = context.mTargetEntity->getComponent<BombComponent>();
		if (nullptr == bombComponent)
			return false;

		if (!bombComponent->canBeDefused())
			return false;

		// Check if bomb is reserved
		if (bombComponent->getIsDefuseBombActive())
			return false;

		return true;
	}

	void DefuseBombCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const DefuseBombAction* defuseBombAction = actionComponent.getAction<DefuseBombAction>();
		if (nullptr != defuseBombAction)
		{
			if (defuseBombAction->getTargetEntityId() == context.mTargetEntity->getId())
			{
				// Don't push the same command twice
				return;
			}
		}
		actionComponent.clearPlan();

		// Get the transform component for the position
		actionComponent.pushAction<MoveAction>(ACTION_PRIORITY).init(new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, GotoBombTargetPointProvider::TARGET_POINT_ID), MovementModes::MOVEMENT_MODE_PLAYER_VEHICLE_ON_EMERGENCY_OPERATIONS);
		actionComponent.pushAction<TurnToAction>(ACTION_PRIORITY).init(context.mTargetEntity->getId());
		actionComponent.pushAction<DefuseBombAction>(ACTION_PRIORITY).init(*context.mTargetEntity);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
