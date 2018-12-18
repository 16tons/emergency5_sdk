// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/base/StartPullObjectCommand.h"
#include "em5/command/CommandContext.h"
#include "em5/action/base/PullObjectAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/fire/FireHoseCheckAction.h"
#include "em5/action/equipment/GetEquipmentAction.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include "em5/component/objects/BuryComponent.h"
#include "em5/game/targetpoint/PullObjectTargetPointProvider.h"
#include "em5/game/targetpoint/GetEquipmentTargetPointProvider.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/equipment/EquipmentComponent.h>

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/component/link/LinkComponent.h>
#include <qsf/logic/action/ActionComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 StartPullObjectCommand::PLUGINABLE_ID = qsf::StringHash("em5::StartPullObjectCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 StartPullObjectCommand::ACTION_PRIORITY = action::COMMAND_STD;
	const uint32 StartPullObjectCommand::ACTION_PRIORITY_AUTOMATISM = action::AUTOMATISM_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	StartPullObjectCommand::StartPullObjectCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "start_pull_object";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool StartPullObjectCommand::checkAvailable()
	{
		return true;
	}

	bool StartPullObjectCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
			return false;

		// hands are full
		if (EntityHelper(caller).isCarrying())
			return false;

		return true;
	}

	bool StartPullObjectCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// Target entity needed
		qsf::Entity* targetEntity = context.mTargetEntity;
		if (nullptr == targetEntity)
			return false;

		// Check if the target is already linked to some parent
		qsf::LinkComponent* linkComponent = targetEntity->getComponent<qsf::LinkComponent>();
		if (nullptr != linkComponent && linkComponent->hasParent())
			return false;

		// Get the bury component to check the draggable flag
		const BuryComponent* buryComponent = targetEntity->getComponent<BuryComponent>();
		if (nullptr == buryComponent)
			return false;

		if (!buryComponent->getDraggable())
			return false;

		// Different handling for automatism
		const CommandContext& em5CommandContext = static_cast<const CommandContext&>(context);
		if (em5CommandContext.mIsAutomatism)
		{
			// Is target reserved
			if (!ReserveLogic::checkReservation(*targetEntity, context.mCaller->getId(), ReserveLogic::MOVETO))
				return false;
		}

		// Everything okay
		return true;
	}

	void StartPullObjectCommand::execute(const qsf::game::CommandContext& context)
	{
		const uint32 usedActionPriority = (static_cast<const CommandContext&>(context).mIsAutomatism ? ACTION_PRIORITY_AUTOMATISM : ACTION_PRIORITY);

		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const PullObjectAction* pullObjectAction = actionComponent.getAction<PullObjectAction>();
		if (nullptr != pullObjectAction && pullObjectAction->getTargetEntityId() == context.mTargetEntity->getId())
		{
			// Don't push the same command twice
			return;
		}

		// Clear plan before the actions are pushed
		actionComponent.clearPlan();

		actionComponent.pushAction<FireHoseCheckAction>(usedActionPriority).init(*context.mTargetEntity, true);
		actionComponent.pushAction<MoveAction>(usedActionPriority).init(new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, PullObjectTargetPointProvider::TARGET_POINT_ID));
		actionComponent.pushAction<TurnToAction>(getPriority()).init(context.mTargetEntity->getId(), false);
		actionComponent.pushAction<PullObjectAction>(usedActionPriority).init(*context.mTargetEntity, PullObjectAction::START, true);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
