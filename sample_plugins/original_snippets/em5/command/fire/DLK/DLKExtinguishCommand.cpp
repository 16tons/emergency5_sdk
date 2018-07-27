// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/fire/DLK/DLKExtinguishCommand.h"
#include "em5/action/fire/ExtinguishFireAction.h"
#include "em5/action/fire/DLK/DLKDeployAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"
#include "em5/ai/MovementModes.h"
#include "em5/game/targetpoint/ExtinguishTargetPointProvider.h"
#include "em5/map/EntityHelper.h"
#include "em5/command/CommandContext.h"
#include "em5/logic/local/firefighters/DLKLogic.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/plugin/Messages.h"
#include "em5/specs/FireSpecsGroup.h"
#include "em5/EM5Helper.h"

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/component/move/MovableComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/map/query/GroundMapQuery.h>
#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Local definitions                                     ]
	//[-------------------------------------------------------]
	const uint32 ACTION_PRIORITY = action::COMMAND_STD;
	const uint32 ACTION_PRIORITY_AUTOMATISM = action::COMMAND_LOW;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 DLKExtinguishCommand::PLUGINABLE_ID = qsf::StringHash("em5::DLKExtinguishCommand");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	DLKExtinguishCommand::DLKExtinguishCommand(qsf::game::CommandManager* commandManager) :
		ExtinguishCoolBaseCommand(commandManager, PLUGINABLE_ID)
	{
		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "extinguish_fire";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool DLKExtinguishCommand::checkAvailable()
	{
		return true;
	}

	bool DLKExtinguishCommand::checkCaller(qsf::Entity& caller)
	{
		return checkCallerActionPriority(caller, ACTION_PRIORITY);
	}

	bool DLKExtinguishCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// Check in ExtinguishCoolBaseCommand class
		if (!checkTarget(context))
			return false;

		// Target entity needed
		qsf::Entity* targetEntity = context.mTargetEntity;
		if (nullptr == targetEntity)
			return false;

		// Check if entity is on fire or not
		const EntityHelper entityHelper(*targetEntity);
		if (!EntityHelper(*targetEntity).isBurning())
			return false;

		return true;
	}

	void DLKExtinguishCommand::execute(const qsf::game::CommandContext& context)
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

		// Clear plan before the actions are pushed
		actionComponent.clearPlan();

		const uint32 usedActionPriority = (static_cast<const CommandContext&>(context).mIsAutomatism ? ACTION_PRIORITY_AUTOMATISM : ACTION_PRIORITY);

		QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ON_COMMAND_EXECUTE, PLUGINABLE_ID, context.mCaller->getId(), context.mTargetEntity->getId()));

		// TODO(tl): Different Target Point Provider
		actionComponent.pushAction<MoveAction>(usedActionPriority).init(new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, ExtinguishTargetPointProvider::TARGET_POINT_ID), MovementModes::MOVEMENT_MODE_PLAYER_VEHICLE_ON_EMERGENCY_OPERATIONS);
		actionComponent.pushAction<DLKDeployAction>(action::BLOCKING, qsf::action::APPEND_TO_BACK).init(context.mTargetEntity->getId());

		const float extinguishPower = FireSpecsGroup::getInstanceSafe().getPowerDLK();
		const float coolingPower = FireSpecsGroup::getInstanceSafe().getCoolingDLK();

		actionComponent.pushAction<ExtinguishFireAction>(usedActionPriority, qsf::action::APPEND_TO_BACK).init(*context.mTargetEntity, extinguishPower, coolingPower);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
