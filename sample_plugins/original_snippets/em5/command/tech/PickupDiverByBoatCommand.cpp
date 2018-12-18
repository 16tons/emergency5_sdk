// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/tech/PickupDiverByBoatCommand.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/fire/PickupDiverAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/game/targetpoint/BoatPickupDrowningTargetPointProvider.h"
#include "em5/map/CollisionHelper.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/command/CommandContext.h>

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/component/link/LinkComponent.h>
#include <qsf/logic/action/ActionComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 PickupDiverByBoatCommand::PLUGINABLE_ID = qsf::StringHash("em5::PickupDiverByBoatCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 PickupDiverByBoatCommand::ACTION_PRIORITY = action::COMMAND_STD;
	const uint32 PickupDiverByBoatCommand::ACTION_PRIORITY_AUTOMATISM = action::AUTOMATISM_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PickupDiverByBoatCommand::PickupDiverByBoatCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "pickup_diver_by_boat";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool PickupDiverByBoatCommand::checkAvailable()
	{
		return true;
	}

	bool PickupDiverByBoatCommand::checkCallerWithoutPriority(qsf::Entity& caller) const
	{
		if (!EntityHelper(caller).isBoat())
			return false;

		return true;
	}

	bool PickupDiverByBoatCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
			return false;

		if (!checkCallerWithoutPriority(caller))
			return false;

		// Check if the boat is not on a transporter
		qsf::LinkComponent* linkComponent = caller.getComponent<qsf::LinkComponent>();
		if (nullptr != linkComponent && qsf::isInitialized(linkComponent->getParentId()))
			return false;

		return true;
	}

	bool PickupDiverByBoatCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// Validate target
		qsf::Entity* targetEntity = context.mTargetEntity;
		if (nullptr == targetEntity)
			return false;

		CommandableComponent* commandableComponent = targetEntity->getComponent<CommandableComponent>();
		if (nullptr == commandableComponent)
			return false;

		if (!commandableComponent->isFireFighterDiver())
			return false;

		if (!checkCallerActionPriority(*targetEntity, ACTION_PRIORITY))
			return false;

		// Only safe persons in water
		qsf::TransformComponent& transform = targetEntity->getOrCreateComponentSafe<qsf::TransformComponent>();
		if (!CollisionHelper(targetEntity->getMap()).isInWater(transform.getPosition()))
			return false;

		// Only safe persons who aren't in a container or any other childlink connection
		qsf::LinkComponent* linkComponent = targetEntity->getComponent<qsf::LinkComponent>();
		if (nullptr != linkComponent && linkComponent->getParentId() != qsf::getUninitialized<uint64>())
			return false;

		if (!EntityHelper(*targetEntity).checkVehicleHasFreeSeats(*context.mCaller))
			return false;

		// Check if succeeded
		return true;
	}

	void PickupDiverByBoatCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& callerActionComponent = getActionComponent(*context.mCaller);
		qsf::ActionComponent& targetActionComponent = getActionComponent(*context.mTargetEntity);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const PickupDiverAction* pickupAction = callerActionComponent.getAction<PickupDiverAction>();
		if (nullptr != pickupAction && pickupAction->getTargetId() == context.mTargetEntity->getId())
		{
			// Don't push the same command twice
			return;
		}

		callerActionComponent.clearPlan();
		callerActionComponent.pushAction<MoveAction>(ACTION_PRIORITY).init(new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, BoatPickupDrowningTargetPointProvider::TARGET_POINT_ID));
		callerActionComponent.pushAction<PickupDiverAction>(action::BLOCKING).init(*context.mTargetEntity);

		targetActionComponent.clearPlan();
		targetActionComponent.pushAction<TurnToAction>(ACTION_PRIORITY).init(context.mCaller->getId());
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
