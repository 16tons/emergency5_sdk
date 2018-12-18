// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/tech/UseWithEngineerCommand.h"
#include "em5/action/tech/RepairOrUseAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/component/objects/UsableByEngineerComponent.h"
#include "em5/game/targetpoint/GotoObjectFrontsideTargetPointProvider.h"
#include "em5/game/targetpoint/GetIntoBuildingTargetPointProvider.h"
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
	const uint32 UseWithEngineerCommand::PLUGINABLE_ID = qsf::StringHash("em5::UseWithEngineerCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 UseWithEngineerCommand::ACTION_PRIORITY = action::COMMAND_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	UseWithEngineerCommand::UseWithEngineerCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "repair_vehicle";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool UseWithEngineerCommand::checkAvailable()
	{
		return true;
	}

	bool UseWithEngineerCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
			return false;

		// Check if unit state is valid
		EntityHelper entityHelper(caller);
		if (!entityHelper.isSquadPersonInValidState())
			return false;

		return true;
	}

	bool UseWithEngineerCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// Target must be usable by the engineer
		const UsableByEngineerComponent* usableByEngineerComponent = context.mTargetEntity->getComponent<UsableByEngineerComponent>();
		if (nullptr == usableByEngineerComponent)
			return false;

		if (!usableByEngineerComponent->isActive())
			return false;

		if (usableByEngineerComponent->getUsed())
			return false;

		return true;
	}

	void UseWithEngineerCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const RepairOrUseAction* repairOrUseAction = actionComponent.getAction<RepairOrUseAction>();
		if (nullptr != repairOrUseAction && repairOrUseAction->getTargetEntityId() == context.mTargetEntity->getId())
		{
			// Don't push the same command twice
			return;
		}

		const UsableByEngineerComponent* usableByEngineerComponent = context.mTargetEntity->getComponent<UsableByEngineerComponent>();
		if (nullptr == usableByEngineerComponent || !usableByEngineerComponent->isActive())
			return;

		bool isBuilding = EntityHelper(*context.mTargetEntity).isEntityBuilding();

		// Clear plan before the actions are pushed
		actionComponent.clearPlan();

		uint32 targetPointProviderId = (isBuilding ? GetIntoBuildingTargetPointProvider::TARGET_POINT_ID : GotoObjectFrontsideTargetPointProvider::TARGET_POINT_ID);
		actionComponent.pushAction<MoveAction>(ACTION_PRIORITY).init(new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, targetPointProviderId));
		actionComponent.pushAction<TurnToAction>(ACTION_PRIORITY).init(context.mTargetEntity->getId());

		// TODO(mk) read the configuration from the UsableByEngineerComponent inside the repairOrUseAction. No need to flood the command with unnecessary informations
		RepairOrUseAction::Posture posture = (isBuilding ? RepairOrUseAction::INBULIDING : (usableByEngineerComponent->getPlacement() == UsableByEngineerComponent::HIGH ? RepairOrUseAction::STANDING : RepairOrUseAction::KNEELING));
		RepairOrUseAction::Target useTarget = (isBuilding ? RepairOrUseAction::BUILDING : RepairOrUseAction::OBJECT);
		actionComponent.pushAction<RepairOrUseAction>(ACTION_PRIORITY).init(*context.mTargetEntity, posture, useTarget);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
