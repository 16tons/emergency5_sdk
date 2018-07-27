// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/police/EvacuateBuildingCommand.h"
#include "em5/base/ContainerCategory.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/police/EvacuateBuildingAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/game/targetpoint/GetIntoBuildingTargetPointProvider.h"
#include "em5/logic/local/base/PersonInBuildingLogic.h"
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
	const uint32 EvacuateBuildingCommand::PLUGINABLE_ID = qsf::StringHash("em5::EvacuateBuildingCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 EvacuateBuildingCommand::ACTION_PRIORITY = action::COMMAND_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	EvacuateBuildingCommand::EvacuateBuildingCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "evacuate_building";
	}

	bool EvacuateBuildingCommand::checkCallerWithoutPriority(qsf::Entity& caller)
	{
		// Check if unit is squad vehicle
		EntityHelper entityHelper(caller);
		if (!entityHelper.isSquadPersonInValidState())
			return false;

		return true;
	}

	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool EvacuateBuildingCommand::checkAvailable()
	{
		return true;
	}

	bool EvacuateBuildingCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
			return false;

		if (!checkCallerWithoutPriority(caller))
			return false;

		return true;
	}

	bool EvacuateBuildingCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// Target entity needed
		if (nullptr == context.mTargetEntity)
			return false;

		EntityHelper entityHelper(*context.mTargetEntity);

		if (entityHelper.isBurning())
			return false;

		// Check if target is building
		const BuildingComponent* targetBuildingComponent = context.mTargetEntity->getComponent<BuildingComponent>();
		if (nullptr == targetBuildingComponent)
			return false;

		if (targetBuildingComponent->getIsDoorBlocked())
			return false;

		// Need hostages or trapped persons to free
		if (!targetBuildingComponent->isContainingHostages() && !targetBuildingComponent->isContainingTrappedPersons())
			return false;

		// No gangster
		if (targetBuildingComponent->isContainingGangsters())
			return false;

		// Only allow the command if an non injured person is inside the building
		bool onlyInjuredPersonsInsideTheBuilding = true;
		std::vector<qsf::Entity*> trappedPersons = targetBuildingComponent->getAllLinksByContainerType(container::CONTAINERTYPE_TRAPPED);
		for (qsf::Entity* trappedPerson : trappedPersons)
		{
			if (!EntityHelper(*trappedPerson).isPersonInjured())
			{
				onlyInjuredPersonsInsideTheBuilding = false;
				continue;
			}
		}
		if (onlyInjuredPersonsInsideTheBuilding)
		{
			std::vector<qsf::Entity*> hostagePersons = targetBuildingComponent->getAllLinksByContainerType(container::CONTAINERTYPE_HOSTAGE);
			for (qsf::Entity* hostagePerson : hostagePersons)
			{
				if (!EntityHelper(*hostagePerson).isPersonInjured())
				{
					onlyInjuredPersonsInsideTheBuilding = false;
					continue;
				}
			}
		}

		if (onlyInjuredPersonsInsideTheBuilding)
			return false;

		PersonInBuildingLogic* personInBuildingLogic = entityHelper.getGameLogic<PersonInBuildingLogic>();
		if (nullptr != personInBuildingLogic)
		{
			// Currently we say the person in Building Logic is only for multiple units in a building, so this command here need to refactored to avoid interrupting with the RescuePersonEvent
			return false;
		}

		return true;
	}

	void EvacuateBuildingCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const EvacuateBuildingAction* evacuateBuildingAction = actionComponent.getAction<EvacuateBuildingAction>();
		if (nullptr != evacuateBuildingAction && evacuateBuildingAction->getTargetEntityId() == context.mTargetEntity->getId())
		{
			// Don't push the same command twice
			return;
		}

		// Clear plan before the actions are pushed
		actionComponent.clearPlan();

		// Add the move action
		actionComponent.pushAction<MoveAction>(ACTION_PRIORITY).init(new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, GetIntoBuildingTargetPointProvider::TARGET_POINT_ID));
		actionComponent.pushAction<EvacuateBuildingAction>(action::BLOCKING).init(*context.mTargetEntity);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
