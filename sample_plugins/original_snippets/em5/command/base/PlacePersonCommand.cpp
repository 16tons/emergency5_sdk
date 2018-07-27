// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/base/PlacePersonCommand.h"
#include "em5/action/ambulance/EnterRescueHeliAction.h"
#include "em5/action/base/PlacePersonAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/command/CommandContext.h"
#include "em5/component/vehicle/VehicleComponent.h"
#include "em5/component/vehicle/parts/RescueBusComponent.h"
#include "em5/health/HealthComponent.h"
#include "em5/logic/local/base/CarryPersonLogic.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/game/targetpoint/EnterRescueHeliTargetPointProvider.h"
#include "em5/game/targetpoint/PlacePersonTargetPointProvider.h"
#include "em5/map/EntityHelper.h"

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/map/Map.h>
#include <qsf/component/link/LinkComponent.h>
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
	const uint32 PlacePersonCommand::PLUGINABLE_ID = qsf::StringHash("em5::PlacePersonCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 PlacePersonCommand::ACTION_PRIORITY = action::COMMAND_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PlacePersonCommand::PlacePersonCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "place_person";
	}

	bool PlacePersonCommand::checkCallerWithoutPriority(qsf::Entity& caller)
	{
		EntityHelper entityHelper(caller);
		if (!entityHelper.isCarryingAnotherPerson())
			return false;

		// Check if unit state is valid
		if (!entityHelper.isSquadPersonInValidState())
			return false;

		return true;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool PlacePersonCommand::checkAvailable()
	{
		return true;
	}

	bool PlacePersonCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriorityNoBlocking(caller, action::BLOCKING))
			return false;

		if (!checkCallerWithoutPriority(caller))
			return false;

		return true;
	}

	bool PlacePersonCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// Target entity needed
		qsf::Entity* targetEntity = context.mTargetEntity;
		if (nullptr == targetEntity || nullptr == context.mCaller)
			return false;

		{ // Check if the caller and the target are units from the same player
			CommandableComponent* callerCommandableComponent = context.mCaller->getComponent<CommandableComponent>();
			CommandableComponent* targetCommandableComponent = context.mTargetEntity->getComponent<CommandableComponent>();
			if (nullptr != callerCommandableComponent && nullptr != targetCommandableComponent)
			{
				if (callerCommandableComponent->getPlayerId() != targetCommandableComponent->getPlayerId())
				{
					// The caller and the target are not from the same player
					return false;
				}
			}
		}

		EntityHelper entityHelper(*targetEntity);

		if (!entityHelper.isUnitInValidState())
			return false;

		// If target is a helicopter it should not be flying
		if (entityHelper.isFlying())
			return false;

		VehicleComponent* vehicleComponent = targetEntity->getComponent<VehicleComponent>();
		if (nullptr == vehicleComponent)
			return false;

		// Intact or damaged1 is valid, else not
		if (!(vehicleComponent->isIntact() || vehicleComponent->isDamagedLevel1()))
			return false;

		// Get the game logic component
		const qsf::GameLogicComponent* gameLogicComponent = context.mCaller->getComponent<qsf::GameLogicComponent>();
		if (nullptr == gameLogicComponent)
			return false;

		// Get the paramedic logic
		const CarryPersonLogic* carryPersonLogic = gameLogicComponent->getGameLogic<CarryPersonLogic>();
		if (nullptr == carryPersonLogic)
			return false;

		qsf::Entity* carriedPersonEntity = context.mCaller->getMap().getEntityById(carryPersonLogic->getCarryPerson());
		if (carriedPersonEntity == nullptr)
			return false;

		if (!checkMatchPersonForVehicle(carriedPersonEntity, targetEntity))
			return false;

		if (!EntityHelper(*carriedPersonEntity).checkVehicleHasFreeSeats(*targetEntity))
			return false;

		CommandableComponent* commandableComponentCaller = context.mCaller->getComponent<CommandableComponent>();
		if (nullptr != commandableComponentCaller && commandableComponentCaller->isAmbulanceParamedics())
		{
			// Only the paramedics has to enter the vehicle, check for space. No checks here for police
			if (!EntityHelper(*context.mCaller).checkVehicleHasFreeSeats(*context.mTargetEntity))
				return false;
		}

		// Caller needs to have a link component
		if (nullptr == context.mCaller->getOrCreateComponent<qsf::LinkComponent>())
			return false;

		qsf::LinkComponent* targetLinkComponent = targetEntity->getComponent<qsf::LinkComponent>();
		if (nullptr != targetLinkComponent)
		{
			RescueBusComponent* rescueBusComponent = targetLinkComponent->getComponentFromEntityOrLinkedChild<RescueBusComponent>();
			if (nullptr != rescueBusComponent && !rescueBusComponent->isExtended())
			{
				// Placing persons in rescue bus only for extended vehicles
				return false;
			}
		}

		// Is target reserved
		const CommandContext& em5CommandContext = static_cast<const CommandContext&>(context);
		if (em5CommandContext.mIsAutomatism)
		{
			if (!ReserveLogic::checkReservation(*targetEntity, context.mCaller->getId(), ReserveLogic::MOVETO))
				return false;
		}

		return true;
	}

	void PlacePersonCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		if (isParamedicAndRescueHeli(context))
		{
			// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
			const EnterRescueHeliAction* enterRescueHeliAction = actionComponent.getAction<EnterRescueHeliAction>();
			if (nullptr != enterRescueHeliAction && enterRescueHeliAction->getTargetId() == context.mTargetEntity->getId())
			{
				// Don't push the same command twice
				return;
			}

			// Clear plan before the actions are pushed
			actionComponent.clearPlan();
			// Add the enter vehicle action
			actionComponent.pushAction<MoveAction>(ACTION_PRIORITY).init(new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, EnterRescueHeliTargetPointProvider::TARGET_POINT_ID));
			actionComponent.pushAction<EnterRescueHeliAction>(action::BLOCKING).init(*context.mTargetEntity);
		}
		else
		{
			// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
			const PlacePersonAction* placePersonAction = actionComponent.getAction<PlacePersonAction>();
			if (nullptr != placePersonAction && placePersonAction->getTargetEntityId() == context.mTargetEntity->getId())
			{
				// Don't push the same command twice
				return;
			}

			// Clear plan before the actions are pushed
			actionComponent.clearPlan();

			actionComponent.pushAction<MoveAction>(ACTION_PRIORITY).init(new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, PlacePersonTargetPointProvider::TARGET_POINT_ID));
			actionComponent.pushAction<TurnToAction>(ACTION_PRIORITY).init(context.mTargetEntity->getId());
			actionComponent.pushAction<PlacePersonAction>(action::BLOCKING).init(*context.mTargetEntity);
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	bool PlacePersonCommand::checkMatchPersonForVehicle(qsf::Entity* carriedPerson, qsf::Entity* vehicle) const
	{
		if (nullptr != carriedPerson)
		{
			const CommandableComponent* commandableComponent = vehicle->getComponent<CommandableComponent>();
			if (nullptr != commandableComponent)
			{
				// Get the health component of the carried person
				const HealthComponent* healthComponent = carriedPerson->getComponent<HealthComponent>();
				if (nullptr != healthComponent)
				{
					// Check for gangster
					if (EntityHelper(*carriedPerson).isGangsterPerson() || EntityHelper(*carriedPerson).isArrested())
					{
						// Check if the vehicle is a police vehicle for transport for gangster
						if (commandableComponent->canTransportGangster())
						{
							return true;
						}
					}

					// Check the state of the carried person
					const HealthComponent::State state = healthComponent->getState();
					if (HealthComponent::State::STATE_INJURED == state || HealthComponent::State::STATE_DEAD == state)
					{
						// Check if the vehicle is an ambulance vehicle for injured transport
						if (commandableComponent->canTransportInjured())
						{
							return true;
						}
					}
				}
			}
		}
		return false;
	}

	bool PlacePersonCommand::isParamedicAndRescueHeli(const qsf::game::CommandContext& context)
	{
		if (context.mCaller != nullptr)
		{
			CommandableComponent* commandableComponentCaller = context.mCaller->getComponent<CommandableComponent>();
			CommandableComponent* commandableComponentTarget = context.mTargetEntity->getComponent<CommandableComponent>();

			if (commandableComponentCaller && commandableComponentCaller->isAmbulanceParamedics() &&
				commandableComponentTarget && commandableComponentTarget->isAmbulanceHeli())
			{
				return true;
			}
		}
		return false;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
