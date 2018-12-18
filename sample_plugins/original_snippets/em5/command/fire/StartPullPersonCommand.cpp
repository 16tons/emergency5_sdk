// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/fire/StartPullPersonCommand.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/equipment/GetEquipmentAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/action/fire/PullPersonAction.h"
#include "em5/action/fire/FireHoseCheckAction.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include "em5/health/HealthComponent.h"
#include "em5/game/targetpoint/GetEquipmentTargetPointProvider.h"
#include "em5/game/targetpoint/PullPersonTargetPointProvider.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/command/CommandContext.h>
#include <qsf_game/component/person/PersonComponent.h>
#include <qsf_game/equipment/EquipmentComponent.h>

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

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
	const uint32 StartPullPersonCommand::PLUGINABLE_ID = qsf::StringHash("em5::StartPullPersonCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 StartPullPersonCommand::ACTION_PRIORITY = action::COMMAND_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	StartPullPersonCommand::StartPullPersonCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "start_pull_person";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool StartPullPersonCommand::checkAvailable()
	{
		return true;
	}

	bool StartPullPersonCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
			return false;

		// Check if unit state is valid
		EntityHelper entityHelper(caller);
		if (!entityHelper.isSquadPersonInValidState())
			return false;

		// hands full?
		if (entityHelper.isCarrying())
			return false;

		return true;
	}

	bool StartPullPersonCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// Target entity needed
		qsf::Entity* targetEntity = context.mTargetEntity;
		if (nullptr == targetEntity)
			return false;

		// Get the health component of the target person
		// Use getComponent instead of getComponentSafe to avoid throwing an exception when the component doesn't exists in the entity.
		// Due the thrown exception e.g. the move command for a fireman isn't selected for execution anymore
		const HealthComponent* healthComponent = targetEntity->getComponent<HealthComponent>();

		// Check the health state of the person
		if (nullptr == healthComponent || !healthComponent->isInjured())
			return false;

		// No healthy persons (person with injury, but standing)
		if (healthComponent->getState() == HealthComponent::STATE_HEALTHY)
			return false;

		if (healthComponent->isContaminated())
			return false;

		if (EntityHelper(*targetEntity).isPersonBuried())
			return false;

		// Get the person and game logic component
		const qsf::game::PersonComponent* personComponent = context.mTargetEntity->getComponent<qsf::game::PersonComponent>();
		if (nullptr == personComponent)
			return false;

		const qsf::GameLogicComponent* gameLogic = context.mCaller->getOrCreateComponent<qsf::GameLogicComponent>();
		if (nullptr == gameLogic)
			return false;

		// On Target can pulled by only one
		if (!ReserveLogic::checkReservation(*context.mTargetEntity, context.mCaller->getId()))
			return false;

		return true;
	}

	void StartPullPersonCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const PullPersonAction* pullPersonAction = actionComponent.getAction<PullPersonAction>();
		if (nullptr != pullPersonAction && pullPersonAction->getTargetEntityId() == context.mTargetEntity->getId())
		{
			// Don't push the same command twice
			return;
		}

		// Clear plan before the actions are pushed
		actionComponent.clearPlan();

		actionComponent.pushAction<FireHoseCheckAction>(ACTION_PRIORITY).init(*context.mTargetEntity, true);
		actionComponent.pushAction<MoveAction>(ACTION_PRIORITY).init(new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, PullPersonTargetPointProvider::TARGET_POINT_ID));
		actionComponent.pushAction<TurnToAction>(getPriority()).init(context.mTargetEntity->getId(), false);
		actionComponent.pushAction<PullPersonAction>(ACTION_PRIORITY).init(context.mTargetEntity->getId(), PullPersonAction::START, true);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
