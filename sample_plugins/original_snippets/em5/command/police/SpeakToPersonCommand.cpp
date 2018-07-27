// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/police/SpeakToPersonCommand.h"
#include "em5/action/police/SpeakToPersonAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/ai/MovementModes.h"
#include "em5/component/person/SpeakToPersonComponent.h"
#include "em5/command/CommandContext.h"
#include "em5/health/HealthComponent.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/game/targetpoint/GotoBombTargetPointProvider.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/command/CommandContext.h>

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/logic/action/ActionComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 SpeakToPersonCommand::PLUGINABLE_ID = qsf::StringHash("em5::SpeakToPersonCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 SpeakToPersonCommand::ACTION_PRIORITY = action::COMMAND_STD;
	const uint32 SpeakToPersonCommand::ACTION_PRIORITY_AUTOMATISM = action::AUTOMATISM_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	SpeakToPersonCommand::SpeakToPersonCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "question_witness";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool SpeakToPersonCommand::checkAvailable()
	{
		return true;
	}

	bool SpeakToPersonCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
			return false;

		// Check if unit state is valid
		EntityHelper entityHelper(caller);
		if (!entityHelper.isUnitInValidState())
			return false;

		return true;
	}

	bool SpeakToPersonCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// We need a target entity
		qsf::Entity* targetEntity = context.mTargetEntity;
		if (nullptr == targetEntity)
			return false;

		if (!targetEntity->getComponent<SpeakToPersonComponent>())
			return false;

		EntityHelper entityHelper(*targetEntity);
		if (entityHelper.isArrested())
			return false;

		if (entityHelper.isEntityHardLinked())
			return false;

		if (entityHelper.isPersonInjured())
			return false;

		// Different handling for automatism
		const CommandContext& em5CommandContext = static_cast<const CommandContext&>(context);
		if (em5CommandContext.mIsAutomatism)
		{
			// Is target reserved
			if (!ReserveLogic::checkReservation(*targetEntity, context.mCaller->getId(), ReserveLogic::MOVETO))
				return false;

			// Don't lead unit automatically in dangerous situations
			HealthComponent& healthComponent = targetEntity->getOrCreateComponentSafe<HealthComponent>();
			if (healthComponent.isReceivingFireDamage())
				return false;
		}

		return true;
	}

	void SpeakToPersonCommand::execute(const qsf::game::CommandContext& context)
	{
		const uint32 usedActionPriority = (static_cast<const CommandContext&>(context).mIsAutomatism ? ACTION_PRIORITY_AUTOMATISM : ACTION_PRIORITY);

		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const SpeakToPersonAction* speakToPersonAction = actionComponent.getAction<SpeakToPersonAction>();
		if (nullptr != speakToPersonAction)
		{
			if (speakToPersonAction->getTargetEntityId() == context.mTargetEntity->getId())
			{
				// Don't push the same command twice
				return;
			}
		}
		actionComponent.clearPlan();

		EntityHelper(*context.mCaller).storeActiveEquipmentInInventory();

		actionComponent.pushAction<MoveAction>(usedActionPriority).init(new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, GotoBombTargetPointProvider::TARGET_POINT_ID));
		actionComponent.pushAction<TurnToAction>(usedActionPriority).init(context.mTargetEntity->getId());
		actionComponent.pushAction<SpeakToPersonAction>(usedActionPriority).init(*context.mTargetEntity);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
