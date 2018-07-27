// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/police/AttackBuildingCommand.h"
#include "em5/action/police/AttackBuildingAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/game/targetpoint/GetIntoBuildingTargetPointProvider.h"
#include "em5/game/units/UnitHelper.h"
#include "em5/logic/local/police/GangsterInBuildingLogic.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/command/CommandContext.h>

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
	const uint32 AttackBuildingCommand::PLUGINABLE_ID = qsf::StringHash("em5::AttackBuildingCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 AttackBuildingCommand::ACTION_PRIORITY = action::COMMAND_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	AttackBuildingCommand::AttackBuildingCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "arrest_gangster";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool AttackBuildingCommand::checkAvailable()
	{
		return true;
	}

	bool AttackBuildingCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
			return false;

		// Check if unit state is valid
		EntityHelper entityHelper(caller);
		if (!entityHelper.isSquadPersonInValidState())
			return false;

		if (entityHelper.isCarryingAnotherPerson())
			return false;

		return true;
	}

	bool AttackBuildingCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// We need a target entity
		qsf::Entity* targetEntity = context.mTargetEntity;
		if (nullptr == targetEntity)
			return false;

		// Check if target is building
		const BuildingComponent* targetBuildingComponent = targetEntity->getComponent<BuildingComponent>();
		if (nullptr == targetBuildingComponent)
			return false;

		qsf::GameLogicComponent* gameLogicComponent = targetEntity->getComponent<qsf::GameLogicComponent>();
		if (nullptr == gameLogicComponent)
			return false;

		GangsterInBuildingLogic* gangsterInBuildingLogic = gameLogicComponent->getGameLogic<GangsterInBuildingLogic>();
		if (nullptr == gangsterInBuildingLogic)
			return false;

		if (!gangsterInBuildingLogic->getFakeObject())
		{
			// Target needs gangsters inside or being a fake object
			if (!targetBuildingComponent->isContainingGangsters())
				return false;
		}

		if (gangsterInBuildingLogic->getCanSpeakWithHostageTaker())
			return false; // Speak with normal policeman before attack the building

		if (UnitHelper(*context.mCaller).isPolicePerson())
		{
			if (!gangsterInBuildingLogic->getAllowNormalPolice())
			{
				// Police is not allowed. Maybe we have a hostage in building or the gangster is armed.
				return false;
			}
		}

		if (gangsterInBuildingLogic->isPoliceTeamComplete())
			return false;	// Enough police

		return true;
	}

	void AttackBuildingCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const AttackBuildingAction* attackBuildingAction = actionComponent.getAction<AttackBuildingAction>();
		if (nullptr != attackBuildingAction)
		{
			if (attackBuildingAction->getTargetEntityId() == context.mTargetEntity->getId())
			{
				// Don't push the same command twice
				return;
			}
		}
		actionComponent.clearPlan();

		// Get the transform component for the position
		actionComponent.pushAction<MoveAction>(ACTION_PRIORITY).init(new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, GetIntoBuildingTargetPointProvider::TARGET_POINT_ID));
		actionComponent.pushAction<AttackBuildingAction>(ACTION_PRIORITY).init(*context.mTargetEntity);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
