// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/fire/CutRoadVehicleCommand.h"
#include "em5/command/CommandContext.h"
#include "em5/action/equipment/GetEquipmentAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/fire/CutRoadVehicleAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include "em5/game/targetpoint/EnterDriverDoorTargetPointProvider.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/CollisionHelper.h"

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/link/link/prototype/ContainerLink.h>
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
	const uint32 CutRoadVehicleCommand::PLUGINABLE_ID = qsf::StringHash("em5::CutRoadVehicleCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 CutRoadVehicleCommand::ACTION_PRIORITY = action::COMMAND_LOW;
	const uint32 CutRoadVehicleCommand::ACTION_PRIORITY_AUTOMATISM = action::AUTOMATISM_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	CutRoadVehicleCommand::CutRoadVehicleCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "cut_road_vehicle";
	}

	bool CutRoadVehicleCommand::checkCallerWithoutPriority(qsf::Entity& caller)
	{
		// Check if unit state is valid
		EntityHelper entityHelper(caller);
		if (!entityHelper.isSquadPersonInValidState())
			return false;

		// Check if caller equipment matches
		if (!entityHelper.checkIsActiveEquipment(equipment::RESCUE_CUTTER))
		{
			if (isNoQueueingCommand(caller))
			{
				return false;
			}
		}

		return true;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool CutRoadVehicleCommand::checkAvailable()
	{
		return true;
	}

	bool CutRoadVehicleCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, action::COMMAND_STD))
			return false;

		if (!checkCallerWithoutPriority(caller))
			return false;

		return true;
	}

	bool CutRoadVehicleCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// Validate target
		qsf::Entity* targetEntity = context.mTargetEntity;
		if (nullptr == targetEntity)
			return false;

		// Only road vehicles could be cut open
		RoadVehicleComponent* roadVehicleComponent = targetEntity->getComponent<RoadVehicleComponent>();
		if (nullptr == roadVehicleComponent)
			return false;

		// Ask the road vehicle component if it is in the right state for being cut open
		//  -> This also checks for a person inside depending on use-case
		if (!roadVehicleComponent->getCanBeCut())
			return false;

		// Check if target is buried
		const EntityHelper targetHelper(*targetEntity);
		if (targetHelper.isEntityBuried())
			return false;

		// Check if in water
		//  -> But high enough above water is okay (needed for ME307)
		if (CollisionHelper(targetEntity->getMap()).isInWater(targetHelper.getPosition(), true))
			return false;

		// Is target reserved
		const CommandContext& em5CommandContext = static_cast<const CommandContext&>(context);
		if (em5CommandContext.mIsAutomatism)
		{
			if (!ReserveLogic::checkReservation(*targetEntity, context.mCaller->getId(), ReserveLogic::MOVETO))
				return false;

			// TODO(mk) Don't lead unit automatically in dangerous situations
		}

		// Check succeeded
		return true;
	}

	void CutRoadVehicleCommand::execute(const qsf::game::CommandContext& context)
	{
		const uint32 usedActionPriority = (static_cast<const CommandContext&>(context).mIsAutomatism ? ACTION_PRIORITY_AUTOMATISM : ACTION_PRIORITY);

		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const CutRoadVehicleAction* cutRoadVehicleAction = actionComponent.getAction<CutRoadVehicleAction>();
		if (nullptr != cutRoadVehicleAction && cutRoadVehicleAction->getTargetEntityId() == context.mTargetEntity->getId())
		{
			// Don't push the same command twice
			return;
		}

		if (isNoQueueingCommand(*context.mCaller))
		{
			// Clear plan before the actions are pushed
			actionComponent.clearPlan();
		}

		actionComponent.pushAction<MoveAction>(usedActionPriority).init(new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, EnterDriverDoorTargetPointProvider::TARGET_POINT_ID));
		actionComponent.pushAction<CutRoadVehicleAction>(action::BLOCKING).init(*context.mTargetEntity);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	bool CutRoadVehicleCommand::isNoQueueingCommand(qsf::Entity& firefighterEntity)
	{
		qsf::ActionComponent& actionComponent = getActionComponent(firefighterEntity);

		// Check the actions on his plan, but not entering vehicle
		GetEquipmentAction* getEquipmentAction = actionComponent.getAction<GetEquipmentAction>();
		if (nullptr != getEquipmentAction && getEquipmentAction->getEquipmentName() == equipment::RESCUE_CUTTER)
		{
			return false;
		}

		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
