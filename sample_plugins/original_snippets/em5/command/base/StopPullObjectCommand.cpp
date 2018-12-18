// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/base/StopPullObjectCommand.h"
#include "em5/action/base/PullObjectAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"
#include "em5/ai/MovementModes.h"
#include "em5/command/CommandContext.h"
#include "em5/command/mode/DefaultCommandMode.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/map/EntityHelper.h"
#include "em5/network/NetworkManager.h"
#include "em5/EM5Helper.h"

#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>

#include <qsf/component/placement/WalkableComponent.h>
#include <qsf/renderer/terrain/TerrainComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 StopPullObjectCommand::PLUGINABLE_ID = qsf::StringHash("em5::StopPullObjectCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 StopPullObjectCommand::ACTION_PRIORITY = action::COMMAND_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	StopPullObjectCommand::StopPullObjectCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "stop_pull_object";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool StopPullObjectCommand::checkAvailable()
	{
		return true;
	}

	bool StopPullObjectCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
			return false;


		// Check if unit state is valid
		EntityHelper entityHelper(caller);
		if (!entityHelper.isSquadPersonInValidState())
			return false;

		// Must pulling a other object
		uint64 pulledObjectId = EntityHelper(caller).getCarryTarget(EntityHelper::PULL_OBJECT);
		if (qsf::isUninitialized(pulledObjectId))
			return false;

		if (nullptr != EM5_NETWORK.getMultiplayerClient())
		{
			// Check if the object exists and is not a person
			// This additional check is needed for multiplay client side, because we misuse the carry person logic(which is already synchronized) as the information holder if the unit is carring/pulling something
			qsf::Entity* pulledEntity = caller.getMap().getEntityById(pulledObjectId);
			if (nullptr == pulledEntity || nullptr != pulledEntity->getComponent<PersonComponent>())
			{
				return false;
			}
		}

		return true;
	}

	bool StopPullObjectCommand::checkContext(const qsf::game::CommandContext& context)
	{
		CommandContext::EvaluationReason evaluationReason = static_cast<const CommandContext&>(context).mEvaluationReason;
		if (evaluationReason == CommandContext::EVALUATIONREASON_SHOWCOMMAND_ABOVE_ENTITY)
		{
			// Don't show the icon in case we only show preview icons, it looks strange for this command
			return false;
		}

		// We need a valid target position
		if (!context.mTargetPosition.is_initialized())
			return false;

		// Only allow targets for walking
		if (nullptr == context.mTargetEntity->getComponent<qsf::WalkableComponent>() &&
			nullptr == context.mTargetEntity->getComponent<qsf::TerrainComponent>())
			return false;

		return true;
	}

	void StopPullObjectCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);
		actionComponent.clearPlan();

		// move there and put the object down
		qsf::logic::TargetPoint targetPoint(context.mTargetPosition.get(), false, 0.5f, 0.5f);
		actionComponent.pushAction<MoveAction>(ACTION_PRIORITY).init(new qsf::ai::ReachSinglePointGoal(targetPoint), MovementModes::MOVEMENT_MODE_WALK_BACKWARDS);
		actionComponent.pushAction<PullObjectAction>(ACTION_PRIORITY).init(*context.mTargetEntity, PullObjectAction::STOP, true);

		DefaultCommandMode::showVisualCommandFeedback(context);
	}

	bool StopPullObjectCommand::highlightTarget() const
	{
		// No command target highlighting please
		return false;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
