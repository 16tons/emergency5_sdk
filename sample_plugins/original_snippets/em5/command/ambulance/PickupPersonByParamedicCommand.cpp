// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/ambulance/PickupPersonByParamedicCommand.h"
#include "em5/command/CommandContext.h"
#include "em5/health/HealthComponent.h"
#include "em5/logic/local/base/CarryPersonLogic.h"
#include "em5/action/ambulance/PickupPersonByParamedicAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/game/targetpoint/PickupPersonWithParamedicTargetPointProvider.h"
#include "em5/map/CollisionHelper.h"
#include "em5/map/EntityHelper.h"

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/map/Map.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/math/CoordinateSystem.h>
#include <qsf/logic/action/ActionComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 PickupPersonByParamedicCommand::PLUGINABLE_ID = qsf::StringHash("em5::PickupPersonByParamedicCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 PickupPersonByParamedicCommand::ACTION_PRIORITY = action::COMMAND_STD;
	const uint32 PickupPersonByParamedicCommand::ACTION_PRIORITY_AUTOMATISM = action::AUTOMATISM_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PickupPersonByParamedicCommand::PickupPersonByParamedicCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Does not start a special command mode (triggered by right-click)
		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "pickup_person_by_paramedic";
	}

	bool PickupPersonByParamedicCommand::checkCallerWithoutPriority(qsf::Entity& caller)
	{
		// Check if unit state is valid
		EntityHelper entityHelper(caller);
		if (!entityHelper.isSquadPersonInValidState())
			return false;

		// The person can only carry one person
		if (entityHelper.isCarryingAnotherPerson())
			return false;

		return true;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool PickupPersonByParamedicCommand::checkAvailable()
	{
		return true;
	}

	bool PickupPersonByParamedicCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
			return false;

		if (!checkCallerWithoutPriority(caller))
			return false;

		return true;
	}

	bool PickupPersonByParamedicCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// Target entity needed
		qsf::Entity* targetEntity = context.mTargetEntity;
		if (nullptr == targetEntity)
			return false;

		// Check if the target is in container
		EntityHelper entityHelper(*targetEntity);
		if (nullptr != entityHelper.getContainerEntity())
			return false;

		// Check if target is hard linked
		if (entityHelper.isEntityHardLinked())
			return false;

		if (entityHelper.isCarriedByOtherPerson())
			return false;

		// Target needs to have a person component
		const HealthComponent* healthComponent = targetEntity->getComponent<HealthComponent>();
		if (nullptr == healthComponent)
			return false;

		// Target must be injured or dead
		if (healthComponent->getState() == HealthComponent::STATE_HEALTHY || healthComponent->getState() == HealthComponent::STATE_DEATHFIGHT)
			return false;

		// Person must have in the injured state the maximum life energy
		if (healthComponent->getState() == HealthComponent::STATE_INJURED && (healthComponent->getLifeEnergy() < healthComponent->getMaximumLifeEnergy()))
			return false;

		// Person must have a complete diagnosis to be carried
		if (healthComponent->isDiagnosisComplete() == false)
			return false;

		// Is target reserved
		const CommandContext& em5CommandContext = static_cast<const CommandContext&>(context);
		if (em5CommandContext.mIsAutomatism)
		{
			if (!ReserveLogic::checkReservation(*targetEntity, context.mCaller->getId(), ReserveLogic::MOVETO))
				return false;

			// Don't lead unit automatically in dangerous situations
			if (healthComponent->isReceivingFireDamage())
				return false;
		}

		// Check if target is potential reachable
		if (CollisionHelper(targetEntity->getMap()).isInBlockedArea(*context.mCaller, *targetEntity))
			return false;

		return true;
	}

	void PickupPersonByParamedicCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const PickupPersonByParamedicAction* pickupPersonByParamedicAction = actionComponent.getAction<PickupPersonByParamedicAction>();
		if (nullptr != pickupPersonByParamedicAction)
		{
			if (pickupPersonByParamedicAction->getTargetEntityId() == context.mTargetEntity->getId())
			{
				// Don't push the same command twice
				return;
			}
		}

		// Clear plan before the actions are pushed
		actionComponent.clearPlan();

		const uint32 usedActionPriority = (static_cast<const CommandContext&>(context).mIsAutomatism ? ACTION_PRIORITY_AUTOMATISM : ACTION_PRIORITY);

		// Turn 0° or 180° to the target
		const glm::quat& targetRotation1 = context.mTargetEntity->getOrCreateComponentSafe<qsf::TransformComponent>().getRotation();
		const glm::quat& targetRotation2 = targetRotation1 * qsf::Math::getRotationAroundAxis(qsf::CoordinateSystem::getUp(), glm::pi<float>());

		actionComponent.pushAction<MoveAction>(usedActionPriority).init(new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, PickupPersonWithParamedicTargetPointProvider::TARGET_POINT_ID));
		actionComponent.pushAction<TurnToAction>(usedActionPriority).initWithRotation(targetRotation1, targetRotation2);
		actionComponent.pushAction<PickupPersonByParamedicAction>(action::BLOCKING).init(*context.mTargetEntity);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
