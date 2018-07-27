// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/fire/PickupDrowningPersonWithDiverCommand.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/action/fire/PickupDrowningPersonWithDiverAction.h"
#include "em5/game/targetpoint/DiverPickupDrowningTargetPointProvider.h"
#include "em5/health/HealthComponent.h"
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
	const uint32 PickupDrowningPersonWithDiverCommand::PLUGINABLE_ID = qsf::StringHash("em5::PickupDrowningPersonWithDiverCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 PickupDrowningPersonWithDiverCommand::ACTION_PRIORITY = action::COMMAND_LOW;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PickupDrowningPersonWithDiverCommand::PickupDrowningPersonWithDiverCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "pickup_drowning_person";
	}

	bool PickupDrowningPersonWithDiverCommand::checkCallerWithoutPriority(qsf::Entity& caller)
	{
		// Check if unit state is valid
		EntityHelper entityHelper(caller);
		if (!entityHelper.isSquadPersonInValidState())
			return false;

		return true;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool PickupDrowningPersonWithDiverCommand::checkAvailable()
	{
		return true;
	}

	bool PickupDrowningPersonWithDiverCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
			return false;

		if (!checkCallerWithoutPriority(caller))
			return false;

		return true;
	}

	bool PickupDrowningPersonWithDiverCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// Validate target
		qsf::Entity* targetEntity = context.mTargetEntity;
		if (nullptr == targetEntity)
			return false;

		EntityHelper entityHelper(*targetEntity);
		if (!entityHelper.isEntityPerson())
			return false;

		HealthComponent* healthComponent = targetEntity->getComponent<HealthComponent>();
		if (healthComponent == nullptr || !healthComponent->getIsDrowning())
			return false;

		// Check succeeded
		return true;
	}

	void PickupDrowningPersonWithDiverCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const PickupDrowningPersonWithDiverAction* pickupAction = actionComponent.getAction<PickupDrowningPersonWithDiverAction>();
		if (nullptr != pickupAction && pickupAction->getTargetEntityId() == context.mTargetEntity->getId())
		{
			// Don't push the same command twice
			return;
		}

		// Clear plan before the actions are pushed
		actionComponent.clearPlan();

		actionComponent.pushAction<MoveAction>(ACTION_PRIORITY).init(new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, DiverPickupDrowningTargetPointProvider::TARGET_POINT_ID));
		actionComponent.pushAction<TurnToAction>(ACTION_PRIORITY).init(context.mTargetEntity->getId());
		actionComponent.pushAction<PickupDrowningPersonWithDiverAction>(action::BLOCKING).init(context.mTargetEntity->getId());
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
