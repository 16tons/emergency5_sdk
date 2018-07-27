// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/fire/DecontaminatePersonCommand.h"
#include "em5/action/fire/DecontaminatePersonHelperAction.h"
#include "em5/action/fire/FireHoseCheckAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/command/CommandContext.h"
#include "em5/ai/MovementModes.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/game/targetpoint/EnterVehicleTargetPointProvider.h"
#include "em5/game/units/UnitHelper.h"
#include "em5/health/HealthComponent.h"
#include "em5/map/EntityHelper.h"

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 DecontaminatePersonCommand::PLUGINABLE_ID = qsf::StringHash("em5::DecontaminatePersonCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 DecontaminatePersonCommand::ACTION_PRIORITY = action::COMMAND_STD;
	const uint32 DecontaminatePersonCommand::ACTION_PRIORITY_AUTOMATISM = action::AUTOMATISM_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	DecontaminatePersonCommand::DecontaminatePersonCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "decontaminate_person";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool DecontaminatePersonCommand::checkAvailable()
	{
		return true;
	}

	bool DecontaminatePersonCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
			return false;

		// Check if unit state is valid
		EntityHelper entityHelper(caller);
		if (!entityHelper.isSquadPersonInValidState())
			return false;

		// Must carry another person
		uint64 carriedId = entityHelper.getCarryTarget((EntityHelper::CarryLoad)(EntityHelper::CARRY_PERSON|EntityHelper::PULL_PERSON));
		if (qsf::isUninitialized(carriedId))
			return false;

		const qsf::Entity* carriedPerson = caller.getMap().getEntityById(carriedId);
		if (nullptr == carriedPerson)
			return false;

		// Check if the person is contaminated
		const HealthComponent* healthComponent = carriedPerson->getComponent<HealthComponent>();
		if (nullptr == healthComponent)
			return false;

		if (!healthComponent->isContaminated())
			return false;

		if (healthComponent->getContaminationType() == Injury::CONTAMINATION_PLAGUE)
			return false;

		// Unit need an abc suit to do this
		if (!UnitHelper(caller).isFireFighterPersonWithABCSuitActive())
			return false;

		return true;
	}

	bool DecontaminatePersonCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// Target entity needed
		if (nullptr == context.mTargetEntity)
			return false;

		// Check if the car is a deconp
		bool isVehicleDECONP = false;
		{
			const CommandableComponent* commandableComponent = context.mTargetEntity->getComponent<CommandableComponent>();
			if (nullptr != commandableComponent)
			{
				isVehicleDECONP = commandableComponent->isFireFighterDECONP();
			}
		}
		if (!isVehicleDECONP)
			return false;

		// Check if vehicle is not reserved
		bool isVehicleFree = false;
		{
			const qsf::GameLogicComponent* gameLogicComponent = context.mTargetEntity->getOrCreateComponent<qsf::GameLogicComponent>();
			if (nullptr != gameLogicComponent)
			{
				// Check for the reserve logic is there, when not we can execute the decontamination
				isVehicleFree = (nullptr == gameLogicComponent->getGameLogic<ReserveLogic>());
			}
		}
		if (!isVehicleFree)
			return false;

		return true;
	}

	void DecontaminatePersonCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const DecontaminatePersonHelperAction* decontaminatePersonHelperAction = actionComponent.getAction<DecontaminatePersonHelperAction>();
		if (nullptr != decontaminatePersonHelperAction && decontaminatePersonHelperAction->getVehicleEntityId() == context.mTargetEntity->getId())
		{
			// Don't push the same command twice
			return;
		}

		uint32 movementModeId = MovementModes::MOVEMENT_MODE_CARRY_PERSON;
		// Move backwards when pulling persons
		if (EntityHelper(*context.mCaller).isPullingPerson())
			movementModeId = MovementModes::MOVEMENT_MODE_WALK_BACKWARDS;

		const uint32 usedActionPriority = (static_cast<const CommandContext&>(context).mIsAutomatism ? ACTION_PRIORITY_AUTOMATISM : ACTION_PRIORITY);

		// Clear plan before the actions are pushed
		actionComponent.clearPlan();

		actionComponent.pushAction<FireHoseCheckAction>(usedActionPriority).init(*context.mTargetEntity, true);
		actionComponent.pushAction<MoveAction>(usedActionPriority).init(new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, EnterVehicleTargetPointProvider::TARGET_POINT_ID), movementModeId);
		actionComponent.pushAction<TurnToAction>(usedActionPriority).init(context.mTargetEntity->getId(), movementModeId == MovementModes::MOVEMENT_MODE_WALK_BACKWARDS);
		actionComponent.pushAction<DecontaminatePersonHelperAction>(usedActionPriority).init(context.mTargetEntity->getId());
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
