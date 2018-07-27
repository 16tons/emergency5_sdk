// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/fire/PutPersonInBoatCommand.h"
#include "em5/game/targetpoint/EnterBoatTargetPointProvider.h"
#include "em5/logic/local/base/CarryPersonLogic.h"
#include "em5/action/fire/PutPersonInBoatAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/map/EntityHelper.h"
#include "em5/ai/MovementModes.h"

#include <qsf_game/command/CommandContext.h>

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

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
	const uint32 PutPersonInBoatCommand::PLUGINABLE_ID = qsf::StringHash("em5::PutPersonInBoatCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 PutPersonInBoatCommand::ACTION_PRIORITY = action::COMMAND_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PutPersonInBoatCommand::PutPersonInBoatCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "place_person_from_boat";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool PutPersonInBoatCommand::checkAvailable()
	{
		return true;
	}

	bool PutPersonInBoatCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
			return false;

		EntityHelper entityHelper(caller);
		if (!entityHelper.isCarryingAnotherPerson())
			return false;

		// Check if unit state is valid
		if (!entityHelper.isSquadPersonInValidState())
			return false;

		return true;
	}

	bool PutPersonInBoatCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// Target entity needed
		if (nullptr == context.mTargetEntity)
			return false;

		EntityHelper entityHelper(*context.mTargetEntity);
		if (!entityHelper.isSquadVehicle() || !entityHelper.isBoat())
			return false;

		// Get the game logic component
		const qsf::GameLogicComponent* gameLogicComponent = context.mCaller->getComponent<qsf::GameLogicComponent>();
		if (nullptr == gameLogicComponent)
			return false;

		// Get the carry logic
		const CarryPersonLogic* carryPersonLogic = gameLogicComponent->getGameLogic<CarryPersonLogic>();
		if (nullptr == carryPersonLogic)
			return false;

		// TODO(mz): must be disabled currently as boats can't be ordered yet and so none has free seats
		if (!EntityHelper(const_cast<qsf::Entity&>(*context.mCaller)).checkVehicleHasFreeSeats(*context.mTargetEntity))
			return false;

		// Caller needs to have a link component
		if (nullptr == context.mCaller->getOrCreateComponent<qsf::LinkComponent>())
			return false;

		return true;
	}

	void PutPersonInBoatCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const PutPersonInBoatAction* putPersonAction = actionComponent.getAction<PutPersonInBoatAction>();
		if (nullptr != putPersonAction  && putPersonAction ->getTargetEntityId() == context.mTargetEntity->getId())
		{
			// Don't push the same command twice
			return;
		}

		// Clear plan before the actions are pushed
		actionComponent.clearPlan();

		actionComponent.pushAction<MoveAction>(ACTION_PRIORITY).init(new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, EnterBoatTargetPointProvider::TARGET_POINT_ID), MovementModes::MOVEMENT_MODE_WALK_BACKWARDS);
		actionComponent.pushAction<TurnToAction>(ACTION_PRIORITY).init(context.mTargetEntity->getId());
		actionComponent.pushAction<PutPersonInBoatAction>(ACTION_PRIORITY).init(*context.mTargetEntity);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
