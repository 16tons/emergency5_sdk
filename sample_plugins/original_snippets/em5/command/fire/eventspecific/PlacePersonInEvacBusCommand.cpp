// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/fire/eventspecific/PlacePersonInEvacBusCommand.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/base/PlacePersonAction.h"
#include "em5/action/fire/FireHoseCheckAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/ai/MovementModes.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/command/CommandContext.h"
#include "em5/game/targetpoint/EnterVehicleTargetPointProvider.h"
#include "em5/health/HealthComponent.h"
#include "em5/map/EntityHelper.h"

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Locally used helper functions                         ]
	//[-------------------------------------------------------]
	namespace
	{
		HealthComponent* getCarriedPersonHealthComponent(qsf::Entity& caller)
		{
			// Must carry a person at all
			const uint64 carriedId = EntityHelper(caller).getCarryTarget((EntityHelper::CarryLoad)(EntityHelper::CARRY_PERSON | EntityHelper::PULL_PERSON));
			if (qsf::isUninitialized(carriedId))
				return nullptr;

			const qsf::Entity* carriedPerson = caller.getMap().getEntityById(carriedId);
			if (nullptr == carriedPerson)
				return nullptr;

			return carriedPerson->getComponent<HealthComponent>();
		}
	}


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 PlacePersonInEvacBusCommand::PLUGINABLE_ID = qsf::StringHash("em5::PlacePersonInEvacBusCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 PlacePersonInEvacBusCommand::ACTION_PRIORITY = action::COMMAND_STD;
	const uint32 PlacePersonInEvacBusCommand::ACTION_PRIORITY_AUTOMATISM = action::AUTOMATISM_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PlacePersonInEvacBusCommand::PlacePersonInEvacBusCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "place_person";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool PlacePersonInEvacBusCommand::checkAvailable()
	{
		return true;
	}

	bool PlacePersonInEvacBusCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
			return false;

		// Check if unit state is valid
		EntityHelper entityHelper(caller);
		if (!entityHelper.isSquadPersonInValidState())
			return false;

		const HealthComponent* healthComponent = getCarriedPersonHealthComponent(caller);
		if (nullptr == healthComponent)
			return false;

		if (healthComponent->isContaminated())
			return false;

		return true;
	}

	bool PlacePersonInEvacBusCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// Target entity needed
		if (nullptr == context.mTargetEntity)
			return false;

		const CommandableComponent* commandableComponent = context.mTargetEntity->getComponent<CommandableComponent>();
		if (nullptr == commandableComponent)
			return false;

		const HealthComponent* healthComponent = getCarriedPersonHealthComponent(*context.mCaller);
		if (nullptr == healthComponent)
			return false;

		const bool isPersonInjured = healthComponent->isInjured();
		const uint32 unitType = commandableComponent->getUnitType();

		if (unitType == CommandableComponent::UNITTAG_MISC_AMBULANCEBUS)
		{
			// Ambulance bus is for injureds only
			if (!isPersonInjured)
				return false;
		}
		else if (unitType == CommandableComponent::UNITTAG_MISC_EVACBUS)
		{
			// Evac bus is for health people only
			if (isPersonInjured)
				return false;
		}
		else
		{
			// Other vehicles are not accepted at all
			return false;
		}

		if (!EntityHelper(healthComponent->getEntity()).checkVehicleHasFreeSeats(*context.mTargetEntity))
			return false;

		return true;
	}

	void PlacePersonInEvacBusCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		uint32 movementModeId = MovementModes::MOVEMENT_MODE_CARRY_PERSON;
		// Move backwards when pulling persons
		if (EntityHelper(*context.mCaller).isPullingPerson())
			movementModeId = MovementModes::MOVEMENT_MODE_WALK_BACKWARDS;

		const uint32 usedActionPriority = (static_cast<const CommandContext&>(context).mIsAutomatism ? ACTION_PRIORITY_AUTOMATISM : ACTION_PRIORITY);

		// Clear plan before the actions are pushed
		actionComponent.clearPlan();

		actionComponent.pushAction<FireHoseCheckAction>(usedActionPriority).init(*context.mTargetEntity, true);
		actionComponent.pushAction<MoveAction>(usedActionPriority).init(new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, EnterVehicleTargetPointProvider::TARGET_POINT_ID), movementModeId);
		actionComponent.pushAction<TurnToAction>(usedActionPriority).init(context.mTargetEntity->getId(), movementModeId == MovementModes::MOVEMENT_MODE_WALK_BACKWARDS);
		actionComponent.pushAction<PlacePersonAction>(usedActionPriority).init(*context.mTargetEntity, true);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
