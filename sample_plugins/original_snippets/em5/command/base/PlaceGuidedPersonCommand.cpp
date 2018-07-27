// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/base/PlaceGuidedPersonCommand.h"
#include "em5/action/base/PlacePersonAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/component/vehicle/parts/RescueBusComponent.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/command/CommandContext.h"
#include "em5/component/vehicle/VehicleComponent.h"
#include "em5/health/HealthComponent.h"
#include "em5/logic/local/base/CarryPersonLogic.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/game/targetpoint/PlacePersonTargetPointProvider.h"
#include "em5/game/targetpoint/EnterCoDriverDoorTargetPointProvider.h"
#include "em5/game/units/UnitHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/EM5Helper.h"

#include <qsf_game/command/CommandContext.h>

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/component/link/LinkComponent.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 PlaceGuidedPersonCommand::PLUGINABLE_ID = qsf::StringHash("em5::PlaceGuidedPersonCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 PlaceGuidedPersonCommand::ACTION_PRIORITY = action::COMMAND_LOW;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PlaceGuidedPersonCommand::PlaceGuidedPersonCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "place_person";
	}

	bool PlaceGuidedPersonCommand::checkCallerWithoutPriority(qsf::Entity& caller)
	{
		EntityHelper entityHelper(caller);
		if (!entityHelper.isCarryingAnotherPerson())
			return false;

		// Check if unit state is valid
		if (!entityHelper.isSquadPersonInValidState())
			return false;

		// Must carry another person
		uint64 carriedId = entityHelper.getCarryTarget((EntityHelper::CarryLoad)(EntityHelper::CARRY_PERSON | EntityHelper::PULL_PERSON));
		if (qsf::isUninitialized(carriedId))
			return false;

		const qsf::Entity* carriedPerson = caller.getMap().getEntityById(carriedId);
		if (nullptr == carriedPerson)
			return false;

		// Check if the person is contaminated with plague
		const HealthComponent* healthComponent = carriedPerson->getComponent<HealthComponent>();
		if (nullptr == healthComponent)
			return false;

		if (healthComponent->getContaminationType() != Injury::CONTAMINATION_PLAGUE)
			return false;

		return true;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool PlaceGuidedPersonCommand::checkAvailable()
	{
		return true;
	}

	bool PlaceGuidedPersonCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriorityNoBlocking(caller, action::BLOCKING))
			return false;

		if (!checkCallerWithoutPriority(caller))
			return false;

		return true;
	}

	bool PlaceGuidedPersonCommand::checkContext(const qsf::game::CommandContext& context)
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

		if (!entityHelper.isSquadVehicleInValidState())
			return false;

		UnitHelper unitHelper(*targetEntity);
		if (!(unitHelper.isAmbulanceRTW() || unitHelper.isAmbulanceIMT() || unitHelper.isAmbulanceHelicopter()))
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

		// Check if the person is contaminated
		const HealthComponent* healthComponent = carriedPersonEntity->getComponent<HealthComponent>();
		if (nullptr == healthComponent)
			return false;

		if (!healthComponent->isContaminated() || healthComponent->getContaminationType() != Injury::CONTAMINATION_PLAGUE)
			return false;

		// Check the vehicle for space
		if (!EntityHelper(*carriedPersonEntity).checkVehicleHasFreeSeats(*targetEntity))
			return false;

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

	void PlaceGuidedPersonCommand::execute(const qsf::game::CommandContext& context)
	{
		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);
		const PlacePersonAction* placePersonAction = actionComponent.getAction<PlacePersonAction>();
		if (nullptr != placePersonAction && placePersonAction->getTargetEntityId() == context.mTargetEntity->getId())
		{
			// Don't push the same command twice
			return;
		}

		// Clear plan before the actions are pushed
		actionComponent.clearPlan();

		const uint32 targetPointID = (UnitHelper(*context.mTargetEntity).isAmbulanceHelicopter() ? EnterCoDriverDoorTargetPointProvider::TARGET_POINT_ID : PlacePersonTargetPointProvider::TARGET_POINT_ID);
		actionComponent.pushAction<MoveAction>(ACTION_PRIORITY).init(new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, targetPointID));
		actionComponent.pushAction<TurnToAction>(ACTION_PRIORITY).init(context.mTargetEntity->getId());
		actionComponent.pushAction<PlacePersonAction>(action::BLOCKING).init(*context.mTargetEntity, true);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
