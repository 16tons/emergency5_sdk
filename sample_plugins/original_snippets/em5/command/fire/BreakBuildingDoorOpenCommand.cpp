// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/fire/BreakBuildingDoorOpenCommand.h"
#include "em5/action/equipment/GetEquipmentAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/fire/BreakBuildingDoorOpenAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/game/targetpoint/GetIntoBuildingTargetPointProvider.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/command/CommandContext.h>

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
	const uint32 BreakBuildingDoorOpenCommand::PLUGINABLE_ID = qsf::StringHash("em5::BreakBuildingDoorOpenCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 BreakBuildingDoorOpenCommand::ACTION_PRIORITY = action::COMMAND_LOW;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	BreakBuildingDoorOpenCommand::BreakBuildingDoorOpenCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "break_building_door_open";
	}

	bool BreakBuildingDoorOpenCommand::checkCallerWithoutPriority(qsf::Entity& caller)
	{
		// Check if unit state is valid
		EntityHelper entityHelper(caller);
		if (!entityHelper.isSquadPersonInValidState())
			return false;

		// Check if caller equipment matches
		if (!entityHelper.checkIsActiveEquipment(equipment::AXE))
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
	bool BreakBuildingDoorOpenCommand::checkAvailable()
	{
		return true;
	}

	bool BreakBuildingDoorOpenCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, action::COMMAND_STD))
			return false;

		if (!checkCallerWithoutPriority(caller))
			return false;

		return true;
	}

	bool BreakBuildingDoorOpenCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// Validate target
		qsf::Entity* targetEntity = context.mTargetEntity;
		if (nullptr == targetEntity)
			return false;

		const EntityHelper targetHelper(*targetEntity);

		// Only doors of buildings can be break open with the axe
		if (!targetHelper.isEntityBuilding() && !targetHelper.isEntityIndustrialPlant())
			return false;

		// Only buildings with trapped persons can be used as target
		BuildingComponent& buildingComponent = targetEntity->getComponentSafe<BuildingComponent>();
		if (!buildingComponent.getIsDoorBlocked())
			return false;

		if (!buildingComponent.isContainingTrappedPersons())
			return false;

		// There must be at least one person inside the building
		if (targetHelper.isContainerEmpty())
			return false;

		// Check succeeded
		return true;
	}

	void BreakBuildingDoorOpenCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const BreakBuildingDoorOpenAction* breakBuildingDoorOpenAction = actionComponent.getAction<BreakBuildingDoorOpenAction>();
		if (nullptr != breakBuildingDoorOpenAction && breakBuildingDoorOpenAction->getTargetEntityId() == context.mTargetEntity->getId())
		{
			// Don't push the same command twice
			return;
		}

		if (isNoQueueingCommand(*context.mCaller))
		{
			// Clear plan before the actions are pushed
			actionComponent.clearPlan();
		}

		actionComponent.pushAction<MoveAction>(ACTION_PRIORITY).init(new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, GetIntoBuildingTargetPointProvider::TARGET_POINT_ID));
		actionComponent.pushAction<BreakBuildingDoorOpenAction>(action::BLOCKING).init(*context.mTargetEntity);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	bool BreakBuildingDoorOpenCommand::isNoQueueingCommand(qsf::Entity& firefighterEntity)
	{
		qsf::ActionComponent& actionComponent = getActionComponent(firefighterEntity);

		// Check the actions on his plan, but not entering vehicle
		GetEquipmentAction* getEquipmentAction = actionComponent.getAction<GetEquipmentAction>();
		if (nullptr != getEquipmentAction && getEquipmentAction->getEquipmentName() == equipment::AXE)
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
