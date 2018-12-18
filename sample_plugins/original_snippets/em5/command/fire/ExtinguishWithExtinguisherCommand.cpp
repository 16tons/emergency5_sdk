// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/fire/ExtinguishWithExtinguisherCommand.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/equipment/GetEquipmentAction.h"
#include "em5/action/fire/ExtinguishFireAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/game/targetpoint/ExtinguishTargetPointProvider.h"
#include "em5/specs/FireSpecsGroup.h"
#include "em5/map/EntityHelper.h"
#include "em5/EM5Helper.h"

#include <qsf_game/command/CommandContext.h>

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

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
	const uint32 ExtinguishWithExtinguisherCommand::PLUGINABLE_ID = qsf::StringHash("em5::ExtinguishWithExtinguisherCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 ExtinguishWithExtinguisherCommand::ACTION_PRIORITY = action::COMMAND_LOW;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	ExtinguishWithExtinguisherCommand::ExtinguishWithExtinguisherCommand(qsf::game::CommandManager* commandManager) :
		ExtinguishCoolBaseCommand(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "extinguish_fire";
	}

	ExtinguishWithExtinguisherCommand::ExtinguishWithExtinguisherCommand(qsf::game::CommandManager* commandManager, uint32 commandId) :
		ExtinguishCoolBaseCommand(commandManager, commandId)
	{
		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "extinguish_fire";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool ExtinguishWithExtinguisherCommand::checkAvailable()
	{
		return true;
	}

	bool ExtinguishWithExtinguisherCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
		{
			if (!isQueueingCommand(caller))
			{
				return false;
			}
		}

		// Check if unit state is valid
		EntityHelper entityHelper(caller);
		if (!entityHelper.isSquadPersonInValidState())
			return false;

		if (!(entityHelper.checkIsActiveEquipment(equipment::FIRE_EXTINGUISHER) ||
			entityHelper.checkIsActiveEquipment(equipment::FIRE_EXTINGUISHER_HISTORICAL)))
		{
			if (!isQueueingCommand(caller))
			{
				return false;
			}
		}

		return true;
	}

	bool ExtinguishWithExtinguisherCommand::checkContext(const qsf::game::CommandContext& context)
	{
		return checkTarget(context);
	}

	void ExtinguishWithExtinguisherCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const ExtinguishFireAction* extinguishFireAction = actionComponent.getAction<ExtinguishFireAction>();
		if (nullptr != extinguishFireAction && extinguishFireAction->getTargetEntityId() == context.mTargetEntity->getId())
		{
			// Don't push the same command twice
			return;
		}

		if (!isQueueingCommand(*context.mCaller))
		{
			// Clear plan before the actions are pushed
			actionComponent.clearPlan();
		}

		actionComponent.pushAction<MoveAction>(ACTION_PRIORITY).init(new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, ExtinguishTargetPointProvider::TARGET_POINT_ID));
		const float extinguishPower = FireSpecsGroup::getInstanceSafe().getPowerExtinguisher();
		const float coolingPower = FireSpecsGroup::getInstanceSafe().getCoolingExtinguisher();
		actionComponent.pushAction<ExtinguishFireAction>(ACTION_PRIORITY).init(*context.mTargetEntity, extinguishPower, coolingPower);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	bool ExtinguishWithExtinguisherCommand::isQueueingCommand(qsf::Entity& firefighterEntity)
	{
		qsf::ActionComponent& actionComponent = getActionComponent(firefighterEntity);

		// Check the actions on his plan, but not entering vehicle
		const GetEquipmentAction* getEquipmentAction = actionComponent.getAction<GetEquipmentAction>();
		if (nullptr != getEquipmentAction)
		{
			const std::string& equipmentName = getEquipmentAction->getEquipmentName();
			if (equipmentName == equipment::FIRE_EXTINGUISHER ||
				equipmentName == equipment::FIRE_EXTINGUISHER_HISTORICAL)
			{
				return true;
			}
		}

		return false;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
