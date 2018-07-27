// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/move/HelicopterFollowCommand.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/command/CommandContext.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/move/HelicopterFollowAction.h"
#include "em5/action/move/StartHelicopterAction.h"
#include "em5/game/targetpoint/FollowWithHeliTargetTargetPointProvider.h"
#include "em5/map/EntityHelper.h"

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf_game/command/CommandContext.h>

#include <qsf/map/Entity.h>
#include <qsf/logic/action/ActionComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 HelicopterFollowCommand::PLUGINABLE_ID = qsf::StringHash("em5::HelicopterFollowCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 HelicopterFollowCommand::ACTION_PRIORITY = action::COMMAND_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	HelicopterFollowCommand::HelicopterFollowCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "follow_gangster_with_heli";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool HelicopterFollowCommand::checkAvailable()
	{
		return true;
	}

	bool HelicopterFollowCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
			return false;

		// Check if unit is squad vehicle
		EntityHelper entityHelper(caller);
		if (!entityHelper.isSquadVehicle())
			return false;

		// Check if unit is helicopter
		if (!entityHelper.isHelicopter())
			return false;

		// Check if unit state is valid
		if (!entityHelper.isUnitInValidState())
			return false;

		return true;
	}

	bool HelicopterFollowCommand::checkContext(const qsf::game::CommandContext& context)
	{
		EntityHelper entityHelper(*context.mTargetEntity);

		// No player units
		if (entityHelper.isSquadVehicle() || entityHelper.isSquadPerson())
			return false;

		// Is target a vehicle or person
		if (!entityHelper.isEntityPerson() && !entityHelper.isRoadVehicle())
			return false;

		CommandContext::EvaluationReason evaluationReason = static_cast<const CommandContext&>(context).mEvaluationReason;
		if (evaluationReason == CommandContext::EVALUATIONREASON_SHOWCOMMAND_ABOVE_ENTITY)
		{
			// In case its no gangster, don't show the preview icons. But allow following civil persons or vehicles.
			if (!(entityHelper.isGangsterPerson() || entityHelper.isGangsterRoadVehicle()))
				return false;
		}

		return true;
	}

	void HelicopterFollowCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const HelicopterFollowAction* helicopterFollowAction = actionComponent.getAction<HelicopterFollowAction>();
		if (nullptr != helicopterFollowAction)
		{
			if (helicopterFollowAction->getTargetEntityId() == context.mTargetEntity->getId())
			{
				// Don't push the same command twice
				return;
			}
		}

		// Clear plan before the actions are pushed
		actionComponent.clearPlan();

		// Create the static target point for the AI to move towards
		qsf::logic::TargetPoint moveTargetConfig;
		moveTargetConfig.mPosition = *context.mTargetPosition;

		// Push actions
		actionComponent.pushAction<StartHelicopterAction>(ACTION_PRIORITY);
		actionComponent.pushAction<MoveAction>(ACTION_PRIORITY).init(new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, FollowWithHeliTargetTargetPointProvider::TARGET_POINT_ID));
		actionComponent.pushAction<HelicopterFollowAction>(ACTION_PRIORITY).init(context.mTargetEntity->getId());
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
