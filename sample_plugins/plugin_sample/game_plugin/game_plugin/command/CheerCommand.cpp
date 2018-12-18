// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "game_plugin/PrecompiledHeader.h"
#include "game_plugin/command/CheerCommand.h"
#include "game_plugin/action/CheerAction.h"

#include <em5/action/ActionPriority.h>
#include <em5/component/vehicle/RoadVehicleComponent.h>
#include <em5/command/component/CommandableComponent.h>
#include <em5/map/EntityHelper.h>

#include <qsf_game/command/CommandContext.h>

#include <qsf/log/LogSystem.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/link/link/prototype/ContainerLink.h>
#include <qsf/map/Map.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace user
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 CheerCommand::PLUGINABLE_ID = qsf::StringHash("user::CheerCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 CheerCommand::ACTION_PRIORITY = em5::action::COMMAND_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	CheerCommand::CheerCommand(qsf::game::CommandManager* commandManager) :
		em5::Command(commandManager, PLUGINABLE_ID)
	{
		mIconSettings.mShowAsGuiButton = true;
		mIconSettings.mButtonIconPath = "cheer";
	}

	bool CheerCommand::checkCallerWithoutPriority(qsf::Entity& caller)
	{
		// Check if unit state is valid
		em5::EntityHelper entityHelper(caller);
		if (!entityHelper.isSquadPersonInValidState())
			return false;

		// Not already carrying someone?
		if (entityHelper.isCarryingAnotherPerson())
			return false;

		return true;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool CheerCommand::checkAvailable()
	{
		// Command is always available
		return true;
	}

	bool CheerCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority ?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
			return false;
		
		if (!checkCallerWithoutPriority(caller))
			return false;

		return true;
	}

	bool CheerCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// Self-executed command check
		if (!context.mAllowSelfExecution)
			return false;
		
		return true;
	}

	void CheerCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const CheerAction* cheerAction = actionComponent.getAction<CheerAction>();
		if (nullptr != cheerAction)
		{
			// Don't push the same command twice
			return;
		}

		// Clear plan before the actions are pushed
		actionComponent.clearPlan();
		actionComponent.pushAction<CheerAction>(em5::action::BLOCKING, qsf::action::APPEND_TO_BACK).init();		// The init function here is empty but it is shown as an example of the pushing action workflow

		// Search for all RTWs and log what's inside
		const auto& commandables = qsf::ComponentMapQuery(QSF_MAINMAP).getAllInstances<em5::CommandableComponent>();
		for (em5::CommandableComponent* commandableComponent : commandables)
		{
			if (commandableComponent->isAmbulanceRTW())
			{
				em5::RoadVehicleComponent& roadVehicleComponent = commandableComponent->getEntity().getComponentSafe<em5::RoadVehicleComponent>();
				const auto& links = roadVehicleComponent.getLinks();

				QSF_LOG_PRINTS(INFO, "RTW " << commandableComponent->getEntityId() << " has " << links.size() << " persons inside");
				for (const auto& pair : links)
				{
					qsf::Entity& personInside = pair.second->getTargetEntity();
					em5::EntityHelper entityHelper(personInside);
					QSF_LOG_PRINTS(INFO, " - " << (entityHelper.isSquadPerson() ? "Unit person" : "Civilian") << " \"" << entityHelper.getName() << "\" with entity ID " << personInside.getId());
				}
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // user
