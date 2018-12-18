// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/tech/PickupDrowningPersonByBoatCommand.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/tech/PickupDrowningPersonByBoatAction.h"
#include "em5/component/vehicle/BoatComponent.h"
#include "em5/component/objects/CallForRescueComponent.h"
#include "em5/game/targetpoint/BoatPickupDrowningTargetPointProvider.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/CollisionHelper.h"
#include "em5/health/HealthComponent.h"

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
	const uint32 PickupDrowningPersonByBoatCommand::PLUGINABLE_ID = qsf::StringHash("em5::PickupDrowningPersonByBoatCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 PickupDrowningPersonByBoatCommand::ACTION_PRIORITY = action::COMMAND_STD;
	const uint32 PickupDrowningPersonByBoatCommand::ACTION_PRIORITY_AUTOMATISM = action::AUTOMATISM_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PickupDrowningPersonByBoatCommand::PickupDrowningPersonByBoatCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "pickup_drowning_person_by_boat";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool PickupDrowningPersonByBoatCommand::checkAvailable()
	{
		return true;
	}

	bool PickupDrowningPersonByBoatCommand::checkCallerWithoutPriority(qsf::Entity& caller)
	{
		if (!EntityHelper(caller).isBoat())
			return false;

		// TODO(mk) Check if there is enough space for the person

		return true;
	}

	bool PickupDrowningPersonByBoatCommand::checkCaller(qsf::Entity& caller)
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

	bool PickupDrowningPersonByBoatCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// Validate target
		qsf::Entity* targetEntity = context.mTargetEntity;
		if (nullptr == targetEntity)
			return false;

		EntityHelper entityHelper(*targetEntity);
		if (!entityHelper.isEntityPerson())
			return false;

		HealthComponent* healthComponent = targetEntity->getComponent<HealthComponent>();
		if (nullptr == healthComponent || (!healthComponent->isInjured() || !healthComponent->getIsDrowning()))
			return false;

		CallForRescueComponent* callForRescueComponent = targetEntity->getComponent<CallForRescueComponent>();
		if (nullptr != callForRescueComponent)
		{
			// The target is no valid target for the motorboat
			return false;
		}

		// Only safe persons in water
		qsf::TransformComponent& transform = targetEntity->getOrCreateComponentSafe<qsf::TransformComponent>();
		if (!CollisionHelper(targetEntity->getMap()).isInWater(transform.getPosition()))
			return false;

		// Only safe persons who aren't in a container or any other child link connection
		qsf::LinkComponent* linkComponent = targetEntity->getComponent<qsf::LinkComponent>();
		if (nullptr != linkComponent && linkComponent->getParentId() != qsf::getUninitialized<uint64>())
			return false;

		if (!EntityHelper(*targetEntity).checkVehicleHasFreeSeats(*context.mCaller))
			return false;

		// Check if succeeded
		return true;
	}

	void PickupDrowningPersonByBoatCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const PickupDrowningPersonByBoatAction* pickupDrowningPersonByBoatAction = actionComponent.getAction<PickupDrowningPersonByBoatAction>();
		if (nullptr != pickupDrowningPersonByBoatAction && pickupDrowningPersonByBoatAction->getTargetId() == context.mTargetEntity->getId())
		{
			// Don't push the same command twice
			return;
		}

		// Clear plan before the actions are pushed
		actionComponent.clearPlan();

		actionComponent.pushAction<MoveAction>(ACTION_PRIORITY).init(new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, BoatPickupDrowningTargetPointProvider::TARGET_POINT_ID));
		actionComponent.pushAction<PickupDrowningPersonByBoatAction>(action::BLOCKING).init(*context.mTargetEntity);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
