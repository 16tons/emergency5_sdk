// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/tech/PickupPersonByFlyingHeliCommand.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/move/StartHelicopterAction.h"
#include "em5/action/tech/PickupPersonByFlyingHeliAction.h"
#include "em5/component/objects/CallForRescueComponent.h"
#include "em5/component/vehicle/HelicopterComponent.h"
#include "em5/component/vehicle/parts/HelicopterHatchComponent.h"
#include "em5/health/HealthComponent.h"
#include "em5/game/targetpoint/PickupPersonByHeliTargetPointProvider.h"
#include "em5/map/CollisionHelper.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/command/CommandContext.h>

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 PickupPersonByFlyingHeliCommand::PLUGINABLE_ID = qsf::StringHash("em5::PickupPersonByFlyingHeliCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 PickupPersonByFlyingHeliCommand::ACTION_PRIORITY = action::COMMAND_STD;
	const uint32 PickupPersonByFlyingHeliCommand::ACTION_PRIORITY_AUTOMATISM = action::AUTOMATISM_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PickupPersonByFlyingHeliCommand::PickupPersonByFlyingHeliCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "pickup_person_by_flying_heli";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool PickupPersonByFlyingHeliCommand::checkAvailable()
	{
		return true;
	}

	bool PickupPersonByFlyingHeliCommand::checkCallerWithoutPriority(qsf::Entity& caller)
	{
		if (!EntityHelper(caller).isHelicopter())
			return false;

		// Check if entity is right and get the helicopter hatch component
		HelicopterComponent* helicopterComponent = caller.getComponent<HelicopterComponent>();
		if (nullptr == helicopterComponent)
			return false;

		qsf::Entity* specialEntity = caller.getMap().getEntityById(helicopterComponent->getSpecialEntity());
		if (nullptr == specialEntity)
			return false;

		HelicopterHatchComponent* helicopterHatchComponent = specialEntity->getComponent<HelicopterHatchComponent>();
		if (nullptr == helicopterHatchComponent)
			return false;

		// An injured person blockade all other transports of the helicopter
		if (helicopterHatchComponent->isTransportingInjuredPerson())
			return false;

		return true;
	}

	bool PickupPersonByFlyingHeliCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
			return false;

		if (!checkCallerWithoutPriority(caller))
			return false;

		return true;
	}

	bool PickupPersonByFlyingHeliCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// Validate target
		qsf::Entity* targetEntity = context.mTargetEntity;
		if (nullptr == targetEntity)
			return false;

		EntityHelper entityHelper(*targetEntity);
		if (!entityHelper.isEntityPerson())
			return false;

		// Check if the target is in container
		if (nullptr != entityHelper.getContainerEntity())
			return false;

		// Check if target is hard linked
		if (entityHelper.isEntityHardLinked())
			return false;

		if (entityHelper.isCarriedByOtherPerson())
			return false;

		if (!entityHelper.isPersonInjured())
		{
			// The pickup action can handle both cases (but we need place for non injured persons)
			CallForRescueComponent* callForRescueComponent = targetEntity->getComponent<CallForRescueComponent>();
			if (nullptr == callForRescueComponent || !callForRescueComponent->getCanRescuedByHelicopter())
			{
				return false;
			}

			// Non injured persons need a seat in the helicopter
			if (!EntityHelper(*targetEntity).checkVehicleHasFreeSeats(*context.mCaller))
			{
				return false;
			}
		}
		else
		{
			// If we not definitely taking the person, check if it reachable, include also the height in the target position for the check
			if (CollisionHelper(targetEntity->getMap()).isInWater(*targetEntity, true))
				return false;

			// Don't pickup targets with "dlk target" flag.
			CallForRescueComponent* callForRescueComponent = targetEntity->getComponent<CallForRescueComponent>();
			if (nullptr != callForRescueComponent && (callForRescueComponent->getCanRescuedByDLK() || !callForRescueComponent->getCanRescuedByHelicopter()))
			{
				return false;
			}
		}

		// Check if succeeded
		return true;
	}

	void PickupPersonByFlyingHeliCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const PickupPersonByFlyingHeliAction* pickupPersonByFlyingHeliAction = actionComponent.getAction<PickupPersonByFlyingHeliAction>();
		if (nullptr != pickupPersonByFlyingHeliAction && pickupPersonByFlyingHeliAction->getTargetId() == context.mTargetEntity->getId())
		{
			// Don't push the same command twice
			return;
		}

		// Clear plan before the actions are pushed
		actionComponent.clearPlan();

		actionComponent.pushAction<StartHelicopterAction>(ACTION_PRIORITY);
		actionComponent.pushAction<MoveAction>(ACTION_PRIORITY).init(new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, PickupPersonByHeliTargetPointProvider::TARGET_POINT_ID));
		actionComponent.pushAction<PickupPersonByFlyingHeliAction>(action::BLOCKING).init(*context.mTargetEntity);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
