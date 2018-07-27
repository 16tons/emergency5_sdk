// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/ambulance/ExitRescueHeliCommand.h"
#include "em5/action/ambulance/ExitRescueHeliAction.h"
#include "em5/action/move/LandHelicopterAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/component/vehicle/VehicleComponent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/command/CommandContext.h>

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 ExitRescueHeliCommand::PLUGINABLE_ID = qsf::StringHash("em5::ExitRescueHeliCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 ExitRescueHeliCommand::ACTION_PRIORITY = action::BLOCKING;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	ExitRescueHeliCommand::ExitRescueHeliCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		mIconSettings.mShowAsGuiButton = true;
		mIconSettings.mButtonIconPath = "exit_vehicle";
		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "exit_vehicle";
		mIconSettings.mTooltip = "EM5_Command_02";

		// Set the help page name
		mHelpPageName = "TUTORIAL_HELPTEXT_EXIT_RHC";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool ExitRescueHeliCommand::checkAvailable()
	{
		return true;
	}

	bool ExitRescueHeliCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriorityNoBlocking(caller, ACTION_PRIORITY))
			return false;

		// Caller (= target) needs to have a vehicle component
		const VehicleComponent* vehicleComponent = caller.getComponent<VehicleComponent>();
		if (nullptr == vehicleComponent)
			return false;

		// Anyone inside?
		if (vehicleComponent->getLinks().empty())
			return false;

		if (!ReserveLogic::checkReservation(caller, caller.getId()))
			return false;

		// Done
		return true;
	}

	bool ExitRescueHeliCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// Self-executed command check
		if (!context.mAllowSelfExecution)
		{
			// If gui button is pressed, we released only the target entity
			if (nullptr == context.mTargetEntity)
				return false;

			// Check if the target is really inside the vehicle
			EntityHelper entityHelper(*context.mTargetEntity);
			if (entityHelper.getContainerEntity() != context.mCaller)
			{
				return false;
			}
		}

		return true;
	}

	void ExitRescueHeliCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const ExitRescueHeliAction* exitRescueHeliAction = actionComponent.getAction<ExitRescueHeliAction>();
		if (nullptr != exitRescueHeliAction)
		{
			// Don't push the same command twice
			return;
		}
		actionComponent.clearPlan();

		// Add the exit vehicle action
		if (EntityHelper(*context.mCaller).isFlying())
		{
			actionComponent.pushAction<LandHelicopterAction>(action::COMMAND_LOW, qsf::action::INSERT_AT_FRONT);
		}

		// Support exiting only one unit
		uint64 personIdToExit = qsf::getUninitialized<uint64>();
		if (nullptr != context.mTargetEntity && context.mCaller != context.mTargetEntity)
		{
			if (nullptr != context.mTargetEntity->getComponent<PersonComponent>())
			{
				// Only when the target is a person set the person id to exit...
				personIdToExit = context.mTargetEntity->getId();
			}
		}
		actionComponent.pushAction<ExitRescueHeliAction>(action::BLOCKING).init(personIdToExit);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
