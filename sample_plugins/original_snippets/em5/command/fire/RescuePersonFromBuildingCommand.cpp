// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/fire/RescuePersonFromBuildingCommand.h"
#include "em5/base/ContainerCategory.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/fire/FireHoseCheckAction.h"
#include "em5/action/fire/RescuePersonFromBuildingAction.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/logic/local/base/PersonInBuildingLogic.h"
#include "em5/map/EntityHelper.h"
#include "em5/game/targetpoint/GetIntoBuildingTargetPointProvider.h"

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
	const uint32 RescuePersonFromBuildingCommand::PLUGINABLE_ID = qsf::StringHash("em5::RescuePersonFromBuildingCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 RescuePersonFromBuildingCommand::ACTION_PRIORITY = action::COMMAND_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	RescuePersonFromBuildingCommand::RescuePersonFromBuildingCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "enter_building";
	}

	bool RescuePersonFromBuildingCommand::checkCallerWithoutPriority(qsf::Entity& caller)
	{
		// Check if unit state is valid
		EntityHelper entityHelper(caller);
		if (!entityHelper.isSquadPersonInValidState())
			return false;

		return true;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool RescuePersonFromBuildingCommand::checkAvailable()
	{
		return true;
	}

	bool RescuePersonFromBuildingCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
			return false;

		if (!checkCallerWithoutPriority(caller))
			return false;

		return true;
	}

	bool RescuePersonFromBuildingCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// Validate target
		qsf::Entity* targetEntity = context.mTargetEntity;
		if (nullptr == targetEntity)
			return false;

		const EntityHelper targetHelper(*targetEntity);

		// Only doors of buildings can be break open with the axe
		if (!targetHelper.isEntityBuilding() && !targetHelper.isEntityIndustrialPlant())
			return false;

		// Only buildings with trapped persons can be used as target
		BuildingComponent& buildingComponent = targetEntity->getComponentSafe<BuildingComponent>();
		if (buildingComponent.isContainingGangsters())
			return false;

		// Take trapped Person and non injured hostages
		if (!buildingComponent.isContainingTrappedPersons())
		{
			bool onlyHealthyHostages = true;
			std::vector<qsf::Entity*> hostagePersons = buildingComponent.getAllLinksByContainerType(container::CONTAINERTYPE_HOSTAGE);
			for (qsf::Entity* hostagePerson : hostagePersons)
			{
				if (EntityHelper(*hostagePerson).isPersonInjured())
				{
					onlyHealthyHostages = false;
					continue;
				}
			}

			if (onlyHealthyHostages)
			{
				return false;
			}
		}

		if (buildingComponent.getIsDoorBlocked())
			return false;

		// There must be at least one person inside the building
		if (targetHelper.isContainerEmpty())
			return false;

		PersonInBuildingLogic* personInBuildingLogic = targetHelper.getGameLogic<PersonInBuildingLogic>();
		if (nullptr != personInBuildingLogic)
		{
			// PersonInBuildingLogic is optional, but if exist, we have to check the configuration
			if (!personInBuildingLogic->isUnitAllowed(*context.mCaller) || personInBuildingLogic->isTeamComplete())
			{
				// Configuration mismatch
				return false;
			}
		}

		// Check succeeded
		return true;
	}

	void RescuePersonFromBuildingCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const RescuePersonFromBuildingAction* rescuePersonFromBuildingAction = actionComponent.getAction<RescuePersonFromBuildingAction>();
		if (nullptr != rescuePersonFromBuildingAction/* && rescuePersonFromBuildingAction->getTargetEntityId() == context.mTargetEntity->getId()*/)
		{
			// Don't push the same command twice
			return;
		}

		// Access the caller's action plan
		actionComponent.clearPlan();
		actionComponent.pushAction<FireHoseCheckAction>(ACTION_PRIORITY).init(*context.mTargetEntity, true);
		actionComponent.pushAction<MoveAction>(ACTION_PRIORITY).init(new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, GetIntoBuildingTargetPointProvider::TARGET_POINT_ID));
		actionComponent.pushAction<RescuePersonFromBuildingAction>(ACTION_PRIORITY).init(*context.mTargetEntity);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
