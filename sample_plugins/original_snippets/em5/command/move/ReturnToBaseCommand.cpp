// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/move/ReturnToBaseCommand.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/action/ambulance/DeployRescueBusAction.h"
#include "em5/action/move/ArrivedAtBaseAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/move/StartHelicopterAction.h"
#include "em5/ai/MovementModes.h"
#include "em5/base/ContainerCategory.h"
#include "em5/component/spawnpoint/UnitSpawnPointComponent.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include "em5/component/vehicle/HelicopterComponent.h"
#include "em5/component/vehicle/parts/HelicopterHatchComponent.h"
#include "em5/component/vehicle/parts/RescueBusComponent.h"
#include "em5/settings/GameSettingsGroup.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Messages.h"

#include <qsf_game/command/CommandContext.h>

#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/link/link/prototype/ContainerLink.h>
#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>
#include <qsf/message/MessageConfiguration.h>
#include <qsf/message/MessageParameters.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 ReturnToBaseCommand::PLUGINABLE_ID = qsf::StringHash("em5::ReturnToBaseCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 ReturnToBaseCommand::ACTION_PRIORITY = action::COMMAND_STD;


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	void ReturnToBaseCommand::pushReturnToBaseAction(qsf::Entity& caller, action::Priority actionPriority)
	{
		// Check if vehicle is in emergency mode
		const MovementModes::Modes movementMode = isInEmergencyMode(caller) ? MovementModes::MOVEMENT_MODE_PLAYER_VEHICLE_ON_EMERGENCY_OPERATIONS : MovementModes::MOVEMENT_MODE_DEFAULT;

		// Find the correct unit spawn point
		const CommandableComponent& commandableComponent = caller.getOrCreateComponentSafe<CommandableComponent>();
		const UnitSpawnPointComponent* unitSpawnPointComponent = UnitSpawnPointComponent::findUnitSpawnPoint(commandableComponent.getUnitType(), UnitSpawnPointComponent::UNITSPAWNPOINTTYPE_EXIT, caller.getMap(), EntityHelper(caller).getPosition());
		if (nullptr != unitSpawnPointComponent)
		{
			// Get components
			const qsf::TransformComponent& spawnPointTransformComponent = unitSpawnPointComponent->getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();
			qsf::ActionComponent& actionComponent = caller.getOrCreateComponentSafe<qsf::ActionComponent>();

			pushActionsBeforeMovement(caller, actionComponent, actionPriority);

			// Move to spawn point position
			qsf::logic::TargetPoint moveTargetConfig;
			moveTargetConfig.mPosition = spawnPointTransformComponent.getPosition();
			moveTargetConfig.mTolerance = 35.0f;	// We need a pretty high tolerance here, to make sure our player vehicles don't get blocked near/inside the HQ
			actionComponent.pushAction<MoveAction>(actionPriority).init(new qsf::ai::ReachSinglePointGoal(moveTargetConfig), movementMode);
			actionComponent.pushAction<ArrivedAtBaseAction>(actionPriority).init(moveTargetConfig);

			// Emit message
			qsf::MessageParameters messageParameters;
			QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_MOVEMENT_RETURN_TO_BASE_START, caller.getId()), messageParameters);
		}
	}

	void ReturnToBaseCommand::pushActionsBeforeMovement(qsf::Entity& caller, qsf::ActionComponent& actionComponent, action::Priority actionPriority)
	{
		// Try to start with helicopter
		if (nullptr != caller.getComponent<HelicopterComponent>())
		{
			actionComponent.pushAction<StartHelicopterAction>(actionPriority);
		}

		{ // Handle IMT
			const qsf::LinkComponent* linkComponent = caller.getComponent<qsf::LinkComponent>();
			if (nullptr != linkComponent)
			{
				const RescueBusComponent* rescueBusComponent = linkComponent->getComponentFromEntityOrLinkedChild<RescueBusComponent>();
				if (nullptr != rescueBusComponent && rescueBusComponent->isExtended())
				{
					actionComponent.pushAction<DeployRescueBusAction>(actionPriority);
				}
			}
		}
	}

	bool ReturnToBaseCommand::isInEmergencyMode(qsf::Entity& caller)
	{
		VehicleComponent* vehicleComponent = caller.getComponent<VehicleComponent>();
		if (nullptr == vehicleComponent)
			return false;

		// Check is the siren feature active in options
		if (GameSettingsGroup::getInstanceSafe().getDriveBackWithoutSirens())
		{
			// Special mode: use sirens only if necessary

			// Go over all entities in the container
			for (auto iterator : vehicleComponent->getLinks())
			{
				const qsf::ContainerLink& containerLink = *iterator.second;
				qsf::Entity& containedEntity = containerLink.getTargetEntity();

				// Are injured persons in vehicle?
				EntityHelper containedEntityHelper(containedEntity);
				if (containedEntityHelper.isPersonInjured())
				{
					return true;
				}
				// Are gangsters in vehicle?
				else if (containedEntityHelper.isArrested())
				{
					return true;
				}
				// Is this a towcar with a destroyed vehicle?
				else if (containedEntityHelper.isVehicleDestroyed())
				{
					return true;
				}
			}

			return false;
		}
		else
		{
			// Default mode: all vehicles use sirens
			return true;
		}
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	ReturnToBaseCommand::ReturnToBaseCommand(qsf::game::CommandManager* commandManager) :
		ReturnToBaseCommand(commandManager, PLUGINABLE_ID)
	{
		// Nothing to do here
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool ReturnToBaseCommand::checkAvailable()
	{
		return true;
	}

	bool ReturnToBaseCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
			return false;

		EntityHelper entityHelper(caller);
		if (!entityHelper.isSquadVehicle())
			return false;

		// If we have a boat transporter we need to check to have no boat in special entity
		if (entityHelper.isUnit(CommandableComponent::UNITTAG_FIREFIGHTERS_BOAT_TRANSPORT))
		{
			RoadVehicleComponent* roadVehicleComponent = caller.getComponent<RoadVehicleComponent>();
			if (nullptr == roadVehicleComponent)
				return false;

			if (qsf::isUninitialized(roadVehicleComponent->getSpecialEntity()))
				return false;
		}

		// Check case were helicopter transport persons on there hatch
		HelicopterComponent* helicopterComponent = caller.getComponent<HelicopterComponent>();
		if (nullptr != helicopterComponent)
		{
			qsf::Entity* specialEntity = caller.getMap().getEntityById(helicopterComponent->getSpecialEntity());
			if (nullptr != specialEntity)
			{
				HelicopterHatchComponent* helicopterHatchComponent = specialEntity->getComponent<HelicopterHatchComponent>();
				if (nullptr != helicopterHatchComponent)
				{
					if (helicopterHatchComponent->isTransportingInjuredPerson())
					{
						// Wait till the helicopter finished person transports
						return false;
					}

					// Only for bhc: Don't fly back while containing civil persons (squad persons are OK)
					VehicleComponent* vehicleComponent = caller.getComponent<VehicleComponent>();
					if (nullptr != vehicleComponent)
					{
						for (auto& iterator : vehicleComponent->getLinks())
						{
							if (iterator.second->mContainerCategory == container::CONTAINERTYPE_PASSENGER)
							{
								return false;
							}
						}
					}
				}
			}
		}

		qsf::GameLogicComponent* gameLogicComponent = caller.getComponent<qsf::GameLogicComponent>();
		if (nullptr != gameLogicComponent && gameLogicComponent->getGameLogic<ReserveEnterVehicleLogic>())
		{
			// Wait till the entity is entered the vehicle
			return false;
		}

		return true;
	}

	bool ReturnToBaseCommand::checkContext(const qsf::game::CommandContext& context)
	{
		if (context.hasTargetPosition())
		{
			// Only allow command via button, in case the target position is valid, use other commands instead
			return false;
		}

		// Commando is self executed
		return true;
	}

	void ReturnToBaseCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const ArrivedAtBaseAction* arrivedAtBaseAction = actionComponent.getAction<ArrivedAtBaseAction>();
		if (nullptr != arrivedAtBaseAction)
		{
			// Don't push the same command twice
			return;
		}

		// Clear plan before the actions are pushed
		actionComponent.clearPlan();

		pushReturnToBaseAction(*context.mCaller, static_cast<action::Priority>(ACTION_PRIORITY));
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	ReturnToBaseCommand::ReturnToBaseCommand(qsf::game::CommandManager* commandManager, uint32 commandId) :
		Command(commandManager, commandId)
	{
		// Priority must be lower than the move command, because we only allow this command via button
		mPriority = 20;

		// Lowest button priority, so the button is always at the right (with hotkey 'e')
		mButtonPriority = -50;

		mIconSettings.mShowAsGuiButton = true;
		mIconSettings.mButtonIconPath = "hq";

		// Set the help page name
		mHelpPageName = "TUTORIAL_HELPTEXT_BACK_TO_HQ";
		mIconSettings.mTooltip = "EM5_Command_03";
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
