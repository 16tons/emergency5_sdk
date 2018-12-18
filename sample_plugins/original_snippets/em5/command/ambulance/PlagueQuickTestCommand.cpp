// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/ambulance/PlagueQuickTestCommand.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/ambulance/PlagueQuickTestAction.h"
#include "em5/action/fire/FireHoseCheckAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/command/CommandContext.h"
#include "em5/health/HealthComponent.h"
#include "em5/game/targetpoint/TreatPersonTargetPointProvider.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/map/EntityHelper.h"

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/action/TurnToAction.h>
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
	const uint32 PlagueQuickTestCommand::PLUGINABLE_ID = qsf::StringHash("em5::PlagueQuickTestCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 PlagueQuickTestCommand::ACTION_PRIORITY = action::COMMAND_LOW;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PlagueQuickTestCommand::PlagueQuickTestCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// TODO(mk) Find the final cursor icon for "plaguequickTest"
		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "treat_person";
	}

	bool PlagueQuickTestCommand::checkCallerWithoutPriority(qsf::Entity& caller)
	{
		// Check if unit state is valid
		EntityHelper entityHelper(caller);
		if (!entityHelper.isSquadPersonInValidState())
			return false;

		if (entityHelper.isCarryingAnotherPerson())
			return false;

		return true;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool PlagueQuickTestCommand::checkAvailable()
	{
		return true;
	}

	bool PlagueQuickTestCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, action::COMMAND_STD))
			return false;

		if (!checkCallerWithoutPriority(caller))
			return false;

		return true;
	}

	bool PlagueQuickTestCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// Validate target
		qsf::Entity* targetEntity = context.mTargetEntity;
		if (nullptr == targetEntity)
			return false;

		// Target Persons
		HealthComponent* healthComponent = targetEntity->getComponent<HealthComponent>();
		if (nullptr == healthComponent)
			return false;

		// Take victims of Plague
		if (healthComponent->getContaminationType() != Injury::CONTAMINATION_PLAGUE)
			return false;

		// Do quicktest only once
		if (!healthComponent->getCanDoContaminationQuickTest())
			return false;

		// Only standing persons
		const Injury* targetInjury = healthComponent->getInjury();
		if (nullptr != targetInjury)
		{
			if (!targetInjury->isAffectingHealth())
			{
				return false;
			}
		}

		// Is target reserved
		const CommandContext& em5CommandContext = static_cast<const CommandContext&>(context);
		if (em5CommandContext.mIsAutomatism)
		{
			if (!ReserveLogic::checkReservation(*targetEntity, context.mCaller->getId(), ReserveLogic::MOVETO))
				return false;

			// Don't lead unit automatically in dangerous situations
			if (healthComponent->isReceivingFireDamage())
				return false;
		}

		// Check succeeded
		return true;
	}

	void PlagueQuickTestCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const PlagueQuickTestAction* plagueQuickTestAction = actionComponent.getAction<PlagueQuickTestAction>();
		if (nullptr != plagueQuickTestAction && plagueQuickTestAction->getTargetEntityId() == context.mTargetEntity->getId())
		{
			// Don't push the same command twice
			return;
		}

		if (isNoQueueingCommand(*context.mCaller))
		{
			// Clear plan before the actions are pushed
			actionComponent.clearPlan();
		}

		actionComponent.pushAction<FireHoseCheckAction>(ACTION_PRIORITY).init(*context.mTargetEntity, true);

		// The move aciton should catch the moving target
		MoveAction& moveAction = actionComponent.pushAction<MoveAction>(ACTION_PRIORITY);
		moveAction.init(new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, TreatPersonTargetPointProvider::TARGET_POINT_ID));
		moveAction.forceFollowedEntityToWait(4.0f, qsf::Time::fromSeconds(10.0f));

		actionComponent.pushAction<qsf::TurnToAction>(ACTION_PRIORITY).init(context.mTargetEntity->getId());
		actionComponent.pushAction<PlagueQuickTestAction>(action::BLOCKING).init(*context.mTargetEntity);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	bool PlagueQuickTestCommand::isNoQueueingCommand(qsf::Entity& firefighterEntity)
	{
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
