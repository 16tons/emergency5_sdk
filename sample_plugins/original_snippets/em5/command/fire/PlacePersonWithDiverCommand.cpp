// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/fire/PlacePersonWithDiverCommand.h"
#include "em5/action/fire/PlacePersonWithDiverAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/ai/MovementModes.h"
#include "em5/command/CommandContext.h"
#include "em5/command/mode/DefaultCommandMode.h"
#include "em5/logic/local/base/CarryPersonLogic.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/command/CommandContext.h>

#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>

#include <qsf/map/Map.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 PlacePersonWithDiverCommand::PLUGINABLE_ID = qsf::StringHash("em5::PlacePersonWithDiverCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 PlacePersonWithDiverCommand::ACTION_PRIORITY = action::COMMAND_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PlacePersonWithDiverCommand::PlacePersonWithDiverCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "place_person";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool PlacePersonWithDiverCommand::checkAvailable()
	{
		return true;
	}

	bool PlacePersonWithDiverCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriorityNoBlocking(caller, action::BLOCKING))
			return false;

		EntityHelper entityHelper(caller);
		if (!entityHelper.isCarryingAnotherPerson())
			return false;

		// Check if unit state is valid
		if (!entityHelper.isSquadPersonInValidState())
			return false;

		return true;
	}

	bool PlacePersonWithDiverCommand::checkContext(const qsf::game::CommandContext& context)
	{
		CommandContext::EvaluationReason evaluationReason = static_cast<const CommandContext&>(context).mEvaluationReason;
		if (evaluationReason == CommandContext::EVALUATIONREASON_SHOWCOMMAND_ABOVE_ENTITY)
		{
			// Don't show the icon in case we only show preview icons, it looks strange for this command
			return false;
		}

		// Get the game logic component
		const qsf::GameLogicComponent* gameLogicComponent = context.mCaller->getComponent<qsf::GameLogicComponent>();
		if (nullptr == gameLogicComponent)
			return false;

		// Get the carry logic
		const CarryPersonLogic* carryPersonLogic = gameLogicComponent->getGameLogic<CarryPersonLogic>();
		if (nullptr == carryPersonLogic)
			return false;

		// Caller needs to have a link component
		if (nullptr == context.mCaller->getOrCreateComponent<qsf::LinkComponent>())
			return false;

		// Target needs to be on land
		if (nullptr != context.mTargetEntity &&
			nullptr == context.mTargetEntity->getComponentById("qsf::WalkableComponent") &&
			nullptr == context.mTargetEntity->getComponentById("qsf::TerrainComponent"))
		{
			return false;
		}

		return true;
	}

	void PlacePersonWithDiverCommand::execute(const qsf::game::CommandContext& context)
	{
		if (isAlreadyRunning(context))
			return;	// Don't push the exact same command twice (with same target)

		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Clear plan before the actions are pushed
		actionComponent.clearPlan();

		qsf::logic::TargetPoint targetPoint(context.mTargetPosition.get(), false, 0.f, 0.1f);
		actionComponent.pushAction<MoveAction>(ACTION_PRIORITY).init(new qsf::ai::ReachSinglePointGoal(targetPoint), MovementModes::MOVEMENT_MODE_WALK_BACKWARDS);
		actionComponent.pushAction<PlacePersonWithDiverAction>(action::BLOCKING).init(context.mTargetPosition.get());

		DefaultCommandMode::showVisualCommandFeedback(context);
	}

	bool PlacePersonWithDiverCommand::highlightTarget() const
	{
		// No command target highlighting please
		return false;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	bool PlacePersonWithDiverCommand::isAlreadyRunning(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const PlacePersonWithDiverAction* placePersonAction = actionComponent.getAction<PlacePersonWithDiverAction>();
		return (nullptr != placePersonAction && placePersonAction->getTargetPos() == context.mTargetPosition.get());
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
