// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/base/ExitVehicleCommand.h"
#include "em5/action/base/ExitVehicleAction.h"
#include "em5/action/fire/DLK/DLKSavePersonAction.h"
#include "em5/action/fire/ExtinguishFireAction.h"
#include "em5/action/move/FullBrakeAction.h"
#include "em5/action/move/LandHelicopterAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/component/vehicle/VehicleComponent.h"
#include "em5/component/vehicle/HelicopterComponent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/command/police/SEK/RopingFromHelicopterCommand.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/game/units/UnitHelper.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/command/CommandContext.h>

#include <qsf/base/ScratchBuffer.h>
#include <qsf/component/move/MovableComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/link/link/prototype/ContainerLink.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 ExitVehicleCommand::PLUGINABLE_ID = qsf::StringHash("em5::ExitVehicleCommand");


	//[-------------------------------------------------------]
	//[ Static public methods                                 ]
	//[-------------------------------------------------------]
	void ExitVehicleCommand::getEntitiesToExit(qsf::Entity& callerEntity, std::vector<qsf::Entity*>& entitiesToExit)
	{
		getEntitiesToExitWithFilter(callerEntity, entitiesToExit, qsf::getUninitialized<uint64>());
	}

	void ExitVehicleCommand::getEntitiesToExitWithFilter(qsf::Entity& callerEntity, std::vector<qsf::Entity*>& entitiesToExit, uint64 filterId)
	{
		VehicleComponent* vehicleComponent = callerEntity.getComponent<VehicleComponent>();
		if (nullptr == vehicleComponent)
			return;

		// Go over all entities in the container
		// -> Do this in a separate step before actually exiting the entities, so we don't run into problems because of manipulation of the iterated list
		for (auto iterator : vehicleComponent->getLinks())
		{
			const qsf::ContainerLink& containerLink = *iterator.second;
			qsf::Entity& linkedEntity = containerLink.getTargetEntity();

			// If filter is set, accept only this id
			if (qsf::isInitialized(filterId) && filterId != linkedEntity.getId())
				continue;

			PersonComponent* personComponent = linkedEntity.getComponent<PersonComponent>();
			if (nullptr == personComponent || personComponent->getGender() == PersonComponent::GENDER_ANIMAL)
			{
				// Another child entity, maybe light or something else
				continue;
			}

			if (EntityHelper(linkedEntity).isPersonInjured())
			{
				// Injured person
				continue;
			}

			if (personComponent->getHandcuffedFlag())
			{
				// Gangsters don't leave their police vehicle
				continue;
			}

			// Special handling for paramedics, if there's also an injured person inside already
			CommandableComponent* commandableComponent = linkedEntity.getComponent<CommandableComponent>();
			if (nullptr != commandableComponent)
			{
				if (commandableComponent->isAmbulanceParamedics() && !UnitHelper(callerEntity).isAmbulanceIMT())
				{
					// Check if there's an injured person inside as well
					bool hasInjured = false;
					for (auto childIterator : vehicleComponent->getLinks())
					{
						if (EntityHelper(childIterator.second->getTargetEntity()).isPersonInjured())
						{
							hasInjured = true;
							break;
						}
					}

					if (hasInjured)
						continue;
				}
			}

			entitiesToExit.push_back(&linkedEntity);
		}
	}

	void ExitVehicleCommand::getEntitiesToExitUnfiltered(qsf::Entity& callerEntity, std::vector<qsf::Entity*>& entitiesToExit)
	{
		VehicleComponent* vehicleComponent = callerEntity.getComponent<VehicleComponent>();
		if (nullptr == vehicleComponent)
			return;

		// Go over all entities in the container
		// -> Do this in a separate step before actually exiting the entities, so we don't run into problems because of manipulation of the iterated list
		for (auto iterator : vehicleComponent->getLinks())
		{
			const qsf::ContainerLink& containerLink = *iterator.second;
			qsf::Entity& linkedEntity = containerLink.getTargetEntity();

			PersonComponent* personComponent = linkedEntity.getComponent<PersonComponent>();
			if (nullptr == personComponent || personComponent->getGender() == PersonComponent::GENDER_ANIMAL)
			{
				// Another child entity, maybe light or something else
				continue;
			}

			entitiesToExit.push_back(&linkedEntity);
		}
	}


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 ExitVehicleCommand::ACTION_PRIORITY = action::BLOCKING;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	ExitVehicleCommand::ExitVehicleCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Second lowest button priority (ReturnToBase is lower), so the button is always the second place at right (with hotkey 'r')
		mButtonPriority = -40;

		mIconSettings.mShowAsGuiButton = true;
		mIconSettings.mButtonIconPath = "exit_vehicle";
		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "exit_vehicle";
		mIconSettings.mTooltip = "EM5_Command_02";

		// Set the help page name
		mHelpPageName = "TUTORIAL_HELPTEXT_EXIT";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool ExitVehicleCommand::checkAvailable()
	{
		return true;
	}

	bool ExitVehicleCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriorityNoBlocking(caller, ACTION_PRIORITY))
		{
			// Extra check, if the dlk saves a person (it blocking priority, still allow the exiting persons
			qsf::ActionComponent& actionComponent = getActionComponent(caller);
			if (nullptr == actionComponent.getAction<DLKSavePersonAction>())
			{
				return false;
			}
		}

		// Caller (= target) needs to have a vehicle component
		const VehicleComponent* vehicleComponent = caller.getComponent<VehicleComponent>();
		if (nullptr == vehicleComponent)
			return false;

		// Anyone inside? Improved check, don't call injured persons or gangster
		qsf::EntityVectorScratchBuffer entitiesToExit;
		getEntitiesToExit(caller, entitiesToExit);
		if (entitiesToExit.empty())
			return false;

		if (!ReserveLogic::checkReservation(caller, caller.getId()))
			return false;

		HelicopterComponent* helicopterComponent = caller.getComponent<HelicopterComponent>();
		if (nullptr != helicopterComponent)
		{
			if (helicopterComponent->isFlying() && !RopingFromHelicopterCommand::allowRoping(caller))
			{
				return false;
			}
		}

		// Done
		return true;
	}

	bool ExitVehicleCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// Allow self-executed commands
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
		// else right click on the caller or button pressed

		return true;
	}

	void ExitVehicleCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const ExitVehicleAction* exitVehicleAction = actionComponent.getAction<ExitVehicleAction>();
		if (nullptr != exitVehicleAction)
		{
			// Don't push the same command twice
			return;
		}

		// Check the current action, in special cases, do it parallel
		if (nullptr == actionComponent.getAction<ExtinguishFireAction>() &&
			nullptr == actionComponent.getAction<DLKSavePersonAction>())
		{
			// Clear plan before the actions are pushed
			actionComponent.clearPlan();
		}

		// Is this entity moving?
		const qsf::MovableComponent* movableComponent = context.mCaller->getComponent<qsf::MovableComponent>();
		if (nullptr != movableComponent && movableComponent->isMoving())
		{
			actionComponent.pushAction<FullBrakeAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT);
		}

		// Add the exit vehicle action

		uint64 personIdToExit = qsf::getUninitialized<uint64>();
		if (nullptr != context.mTargetEntity && context.mCaller != context.mTargetEntity)
		{
			if (nullptr != context.mTargetEntity->getComponent<PersonComponent>())
			{
				// Only when the target is a person set the person id to exit...
				personIdToExit = context.mTargetEntity->getId();
			}
		}
		actionComponent.pushAction<ExitVehicleAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(personIdToExit);

		EntityHelper entityHelper(*context.mCaller);
		if (entityHelper.isHelicopter() && entityHelper.isFlying())
		{
			actionComponent.pushAction<LandHelicopterAction>(action::COMMAND_LOW, qsf::action::INSERT_AT_FRONT);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
