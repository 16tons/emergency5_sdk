// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/fire/UseDivingSpotCommand.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/fire/UseDivingSpotAction.h"
#include "em5/component/objects/DivingSpotComponent.h"
#include "em5/game/targetpoint/OnTouchTargetPointProvider.h"
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
	const uint32 UseDivingSpotCommand::PLUGINABLE_ID = qsf::StringHash("em5::UseDivingSpotCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 UseDivingSpotCommand::ACTION_PRIORITY = action::COMMAND_LOW;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	UseDivingSpotCommand::UseDivingSpotCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "use_diving_spot";
	}

	bool UseDivingSpotCommand::checkCallerWithoutPriority(qsf::Entity& caller)
	{
		// Check if unit state is valid
		EntityHelper entityHelper(caller);
		if (!entityHelper.isSquadPersonInValidState())
			return false;

		// TODO(mz): Not carrying a person
		// TODO(mz): Not linked to a person (being carried)

		return true;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool UseDivingSpotCommand::checkAvailable()
	{
		return true;
	}

	bool UseDivingSpotCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
			return false;

		if (!checkCallerWithoutPriority(caller))
			return false;

		return true;
	}

	bool UseDivingSpotCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// Validate target
		qsf::Entity* targetEntity = context.mTargetEntity;
		if (nullptr == targetEntity)
			return false;

		DivingSpotComponent* divingSpotComponent = targetEntity->getComponent<DivingSpotComponent>();
		if (nullptr == divingSpotComponent)
			return false;

		if (!divingSpotComponent->isIntact())
			return false;

		// Check succeeded
		return true;
	}

	void UseDivingSpotCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const UseDivingSpotAction* useDivingSpotAction = actionComponent.getAction<UseDivingSpotAction>();
		if (nullptr != useDivingSpotAction && useDivingSpotAction->getTargetEntityId() == context.mTargetEntity->getId())
		{
			// Don't push the same command twice
			return;
		}

		// Clear plan before the actions are pushed
		actionComponent.clearPlan();

		actionComponent.pushAction<MoveAction>(ACTION_PRIORITY).init(new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, OnTouchTargetPointProvider::TARGET_POINT_ID));
		actionComponent.pushAction<UseDivingSpotAction>(action::BLOCKING).init(*context.mTargetEntity);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
