// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/tech/LiftUpByCraneCommand.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/tech/LiftUpByCraneAction.h"
#include "em5/ai/MovementModes.h"
#include "em5/command/CommandContext.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/component/objects/BuoyComponent.h"
#include "em5/component/vehicle/VehicleComponent.h"
#include "em5/component/objects/BuryComponent.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/game/targetpoint/LiftUpWithCraneTargetPointProvider.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/component/base/HiddenComponent.h>

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/map/Map.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/input/device/KeyboardDevice.h>
#include <qsf/input/InputSystem.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 LiftUpByCraneCommand::PLUGINABLE_ID = qsf::StringHash("em5::LiftUpByCraneCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 LiftUpByCraneCommand::ACTION_PRIORITY = action::COMMAND_STD;
	const uint32 LiftUpByCraneCommand::ACTION_PRIORITY_AUTOMATISM = action::AUTOMATISM_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	LiftUpByCraneCommand::LiftUpByCraneCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "use_crane";		// This once was "pickup_car_by_crane", which is only a good choice for lifting vehicles
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool LiftUpByCraneCommand::checkAvailable()
	{
		return true;
	}

	bool LiftUpByCraneCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
		{
			if (isNoQueueingCommand(caller))
			{
				return false;
			}
		}

		if (!EntityHelper(caller).isSquadRoadVehicle())
			return false;

		return true;
	}

	bool LiftUpByCraneCommand::checkContext(const qsf::game::CommandContext& context)
	{
		if (nullptr == context.mTargetEntity)
			return false;

		// Is target reserved
		if (!ReserveLogic::checkReservation(*context.mTargetEntity, context.mCaller->getId(), ReserveLogic::MOVETO))
			return false;

		// Crane can lift up two types of entities:
		//  - Debris objects with a bury component
		//  - Vehicles marked as recoverable
		const BuryComponent* buryComponent = context.mTargetEntity->getComponent<BuryComponent>();
		if (nullptr != buryComponent)
		{
			if (buryComponent->isBlocked())
				return false;

			// Do checks for the bury component
			for (uint64 buriedEntityId : buryComponent->BuriedEntitiesArray.getVariable())
			{
				qsf::Entity* buriedEntity = context.mTargetEntity->getMap().getEntityById(buriedEntityId);
				if (nullptr != buriedEntity)
				{
					qsf::game::HiddenComponent* hiddenComponent = buriedEntity->getComponent<qsf::game::HiddenComponent>();
					if (nullptr != hiddenComponent)
					{
						if (hiddenComponent->getHiddenType() == qsf::game::HiddenComponent::HIDDENTYPE_BLOCKED)
						{
							// Find target first via rescue dog
							return false;
						}
					}
				}
			}

			// Object must not be burning right now
			if (EntityHelper(context.mTargetEntity).isBurning())
				return false;
		}
		else
		{
			const VehicleComponent* vehicleComponent = context.mTargetEntity->getComponent<VehicleComponent>();
			if (nullptr != vehicleComponent)
			{
				// Do checks for the vehicle component
				if (!vehicleComponent->getMustBeRecovered())
					return false;
			}
			else
			{
				// Check if we have a Buoy
				if (getVehicleFromBuoyEntity(context.mTargetEntity) == nullptr)
					return false;
			}
		}

		return true;
	}

	void LiftUpByCraneCommand::execute(const qsf::game::CommandContext& context)
	{
		qsf::Entity* target = getVehicleFromBuoyEntity(context.mTargetEntity);

		if (nullptr == target)
			target = context.mTargetEntity;

		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const LiftUpByCraneAction* liftUpByCraneAction = actionComponent.getAction<LiftUpByCraneAction>();
		if (nullptr != liftUpByCraneAction && liftUpByCraneAction->getTargetId() == target->getId())
		{
			// Don't push the same command twice
			return;
		}

		if (isNoQueueingCommand(*context.mCaller))
		{
			// Clear plan before the actions are pushed
			actionComponent.clearPlan();
		}

		const uint32 usedActionPriority = (static_cast<const CommandContext&>(context).mIsAutomatism ? ACTION_PRIORITY_AUTOMATISM : ACTION_PRIORITY);

		actionComponent.pushAction<MoveAction>(usedActionPriority).init(new qsf::ai::ReachObjectGoal(*context.mCaller, *target, LiftUpWithCraneTargetPointProvider::TARGET_POINT_ID), MovementModes::MOVEMENT_MODE_PLAYER_VEHICLE_ON_EMERGENCY_OPERATIONS);
		actionComponent.pushAction<LiftUpByCraneAction>(action::BLOCKING).init(*target);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	qsf::Entity* LiftUpByCraneCommand::getVehicleFromBuoyEntity(qsf::Entity* entity)
	{
		if (nullptr == entity)
			return nullptr;

		const BuoyComponent* buoyComponent = entity->getComponent<BuoyComponent>();
		if (nullptr == buoyComponent)
			return nullptr;

		qsf::LinkComponent* linkComponent = entity->getComponent<qsf::LinkComponent>();
		if (nullptr == linkComponent)
			return nullptr;

		for (qsf::LinkComponent* childLink : linkComponent->getChildLinks())
		{
			qsf::Entity& childEntity = childLink->getEntity();

			const VehicleComponent* childVehicleComponent = childEntity.getComponent<VehicleComponent>();
			if (nullptr != childVehicleComponent)
			{
				return &childVehicleComponent->getEntity();
			}
		}
		return nullptr;
	}

	bool LiftUpByCraneCommand::isNoQueueingCommand(qsf::Entity& craneEntity)
	{
		qsf::ActionComponent& actionComponent = getActionComponent(craneEntity);

		// Check the actions on his plan, but not entering vehicle
		if (nullptr != actionComponent.getAction<LiftUpByCraneAction>() && nullptr == actionComponent.getAction<MoveAction>())
		{
			return false;
		}

		if (QSF_INPUT.getKeyboard().anyShiftPressed())
		{
			return false;
		}

		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
