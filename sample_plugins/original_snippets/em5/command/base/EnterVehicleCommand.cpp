// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/base/EnterVehicleCommand.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/action/ambulance/EnterRescueHeliAction.h"
#include "em5/action/base/EnterVehicleAction.h"
#include "em5/action/base/WaitForEnterVehicleAction.h"
#include "em5/action/fire/EnterBoatAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/action/fire/FireHoseCheckAction.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/component/vehicle/BoatComponent.h"
#include "em5/component/vehicle/VehicleComponent.h"
#include "em5/component/vehicle/HelicopterComponent.h"
#include "em5/logic/local/police/SEKRobotLogic.h"
#include "em5/game/targetpoint/EnterVehicleTargetPointProvider.h"
#include "em5/game/targetpoint/EnterCoDriverDoorTargetPointProvider.h"
#include "em5/game/targetpoint/EnterPassengerDoorTargetPointProvider.h"
#include "em5/game/targetpoint/EnterBoatTargetPointProvider.h"
#include "em5/game/targetpoint/EnterTrunkDoorTargetPointProvider.h"
#include "em5/game/targetpoint/EnterRescueHeliTargetPointProvider.h"
#include "em5/game/units/UnitHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/EM5Helper.h"

#include <qsf_game/command/CommandContext.h>

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/logic/action/ActionComponent.h>
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
	const uint32 EnterVehicleCommand::PLUGINABLE_ID = qsf::StringHash("em5::EnterVehicleCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 EnterVehicleCommand::ACTION_PRIORITY = action::COMMAND_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	EnterVehicleCommand::EnterVehicleCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "enter_vehicle";
	}

	bool EnterVehicleCommand::checkCallerWithoutPriority(qsf::Entity& caller)
	{
		EntityHelper entityHelper(caller);
		if (entityHelper.isCarryingAnotherPerson())
			return false;

		if (entityHelper.isSquadPerson())
		{
			// Check if unit state is valid
			if (!entityHelper.isSquadPersonInValidState())
				return false;
		}
		else
		{
			// Check if unit state is valid
			// TODO(mk) This code here can't be working, isUnitInValidState() works only for units. Maybe the checks there are necessary, I don't know. Commented it out to support the motorboat
			//if (!entityHelper.isUnitInValidState())
			//	return false;
		}

		return true;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool EnterVehicleCommand::checkAvailable()
	{
		return true;
	}

	bool EnterVehicleCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
			return false;

		if (!checkCallerWithoutPriority(caller))
			return false;

		// Hands are full
		if (EntityHelper(caller).isCarrying())
			return false;

		return true;
	}

	bool EnterVehicleCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// Target entity needed
		if (nullptr == context.mTargetEntity || nullptr == context.mCaller)
			return false;

		// Check if the caller and the target are units
		CommandableComponent* commandableComponentCaller = context.mCaller->getComponent<CommandableComponent>();
		CommandableComponent* commandableComponentTarget = context.mTargetEntity->getComponent<CommandableComponent>();
		if (nullptr == commandableComponentCaller || nullptr == commandableComponentTarget)
			return false;

		// Make sure they belong to the same player
		if (commandableComponentCaller->getPlayerId() != commandableComponentTarget->getPlayerId())
			return false;

		// Target needs to have a vehicle component and be in a valid state for entering
		VehicleComponent* vehicleComponent = context.mTargetEntity->getComponent<VehicleComponent>();
		if (nullptr == vehicleComponent || vehicleComponent->isBurning() || vehicleComponent->isDestroyed())
			return false;

		// Can't enter flying helicopters
		HelicopterComponent* helicopterComponent = context.mTargetEntity->getComponent<HelicopterComponent>();
		if (nullptr != helicopterComponent && helicopterComponent->isFlying())
			return false;

		// There must be free seats in vehicle for this person unit type
		if (!EntityHelper(*context.mCaller).checkVehicleHasFreeSeats(*context.mTargetEntity))
			return false;

		return true;
	}

	void EnterVehicleCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		if (isDiverAndBoat(context))
		{
			// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
			const EnterBoatAction* enterBoatAction = actionComponent.getAction<EnterBoatAction>();
			if (nullptr != enterBoatAction && enterBoatAction->getTargetId() == context.mTargetEntity->getId())
			{
				// Don't push the same command twice
				return;
			}

			// Clear plan before the actions are pushed
			actionComponent.clearPlan();

			// Push the commands actions
			pushEnterBoatActions(actionComponent, context);
		}
		else if (isSEKRobotAndTransporter(context))
		{
			// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
			const EnterVehicleAction* enterVehicleAction = actionComponent.getAction<EnterVehicleAction>();
			if (nullptr != enterVehicleAction && enterVehicleAction->getTargetId() == context.mTargetEntity->getId() && enterVehicleAction->getTargetDoorType() == DoorComponent::DOORTYPE_TRUNK)
			{
				// Don't push the same command twice
				return;
			}

			// Clear plan before the actions are pushed
			actionComponent.clearPlan();

			// Push the commands actions
			pushEnterTSEKRActions(actionComponent, context);
		}
		else if (isParamedicAndRescueHeli(context))
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
			const EnterVehicleAction* enterVehicleAction = actionComponent.getAction<EnterVehicleAction>();
			if (nullptr != enterVehicleAction && enterVehicleAction->getTargetId() == context.mTargetEntity->getId() && enterVehicleAction->getTargetDoorType() == DoorComponent::DOORTYPE_CODRIVER)
			{
				// Don't push the same command twice
				return;
			}

			// Clear plan before the actions are pushed
			actionComponent.clearPlan();

			// Push the commands actions
			pushEnterVehicleActions(actionComponent, context);
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void EnterVehicleCommand::pushEnterVehicleActions(qsf::ActionComponent& actionComponent, const qsf::game::CommandContext& context)
	{
		uint32 actionPriority = ACTION_PRIORITY;

		// React to action on command execute
		qsf::MessageParameters parameters;
		QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ON_COMMAND_EXECUTE, PLUGINABLE_ID, context.mCaller->getId(), context.mTargetEntity->getId()), parameters);

		// Set the new priority by the respond (if there is one)
		//  -> Also handle the special case that the response is "action::NONE", which means we should just stop here
		parameters.tryGetResponse(actionPriority);
		if (actionPriority == action::NONE)
		{
			return;
		}

		actionComponent.pushAction<FireHoseCheckAction>(actionPriority).init(*context.mTargetEntity, true);

		// In case the person is really close to the vehicle, we don't push additional "move" to make everything more smooth here
		if (!EnterVehicleAction::isInDoorRange(*context.mCaller, *context.mTargetEntity, DoorComponent::DOORTYPE_CODRIVER, 0.5f))
		{
			actionComponent.pushAction<MoveAction>(actionPriority).init(new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, EnterVehicleTargetPointProvider::TARGET_POINT_ID));
		}
		// Add the enter vehicle action
		actionComponent.pushAction<EnterVehicleAction>(action::BLOCKING).init(*context.mTargetEntity);

		if (context.mTargetEntity->getComponent<HelicopterComponent>() != nullptr)
		{
			// Let helicopter wait for the other (mostly the paramedic autostarts the helicopter and the medic wants to enter the helicopter)
			context.mTargetEntity->getOrCreateComponentSafe<qsf::ActionComponent>().pushAction<WaitForEnterVehicleAction>(action::Priority::BLOCKING, qsf::action::INSERT_AT_FRONT).init(context.mCaller->getId(), qsf::Time::fromSeconds(5.0f));
		}
	}

	void EnterVehicleCommand::pushEnterBoatActions(qsf::ActionComponent& actionComponent, const qsf::game::CommandContext& context)
	{
		uint32 actionPriority = ACTION_PRIORITY;

		// React to action on command execute
		qsf::MessageParameters parameters;
		QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ON_COMMAND_EXECUTE, PLUGINABLE_ID, context.mCaller->getId(), context.mTargetEntity->getId()), parameters);

		// TODO(mk) Currently the respond is not used. Is something missing?
		// Set the new priority by the respond (If there is one)
		parameters.tryGetResponse(actionPriority);

		// Add the enter boat action
		actionComponent.pushAction<MoveAction>(actionPriority).init(new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, EnterBoatTargetPointProvider::TARGET_POINT_ID));
		actionComponent.pushAction<TurnToAction>(ACTION_PRIORITY).init(context.mTargetEntity->getId());
		actionComponent.pushAction<EnterBoatAction>(action::BLOCKING).init(*context.mTargetEntity);
	}

	void EnterVehicleCommand::pushEnterTSEKRActions(qsf::ActionComponent& actionComponent, const qsf::game::CommandContext& context)
	{
		uint32 actionPriority = ACTION_PRIORITY;

		// React to action on command execute
		qsf::MessageParameters parameters;
		QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ON_COMMAND_EXECUTE, PLUGINABLE_ID, context.mCaller->getId(), context.mTargetEntity->getId()), parameters);

		// TODO(mk) Currently the respond is not used. Is something missing?
		// Set the new priority by the respond (If there is one)
		parameters.tryGetResponse(actionPriority);

		// Add the enter vehicle action
		actionComponent.pushAction<MoveAction>(actionPriority).init(new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, EnterTrunkDoorTargetPointProvider::TARGET_POINT_ID));
		actionComponent.pushAction<EnterVehicleAction>(action::BLOCKING).init(*context.mTargetEntity, DoorComponent::DOORTYPE_TRUNK);
	}

	bool EnterVehicleCommand::isDiverAndBoat(const qsf::game::CommandContext& context)
	{
		// Caller must be a person (we don't need to explicitly check if it's really a diver)
		if (nullptr != context.mCaller && nullptr != context.mCaller->getComponent<PersonComponent>())
		{
			// Target must be a boat
			if (nullptr != context.mTargetEntity->getComponent<BoatComponent>())
			{
				return true;
			}
		}
		return false;
	}

	bool EnterVehicleCommand::isSEKRobotAndTransporter(const qsf::game::CommandContext& context)
	{
		if (context.mCaller != nullptr)
		{
			CommandableComponent* commandableComponentCaller = context.mCaller->getComponent<CommandableComponent>();
			CommandableComponent* commandableComponentTarget = context.mTargetEntity->getComponent<CommandableComponent>();

			if (commandableComponentCaller && commandableComponentCaller->isPoliceSEKR() &&
				commandableComponentTarget && commandableComponentTarget->isPoliceTSEKR())
			{
				return true;
			}
		}
		return false;
	}

	bool EnterVehicleCommand::isParamedicAndRescueHeli(const qsf::game::CommandContext& context)
	{
		// Caller must be a person and carrying an injured
		if (nullptr != context.mCaller && qsf::isInitialized(EntityHelper(*context.mCaller).getCarryTarget(EntityHelper::CARRY_PERSON)))
		{
			// Target must be a helicopter and able to transport injureds
			if (nullptr != context.mTargetEntity->getComponent<HelicopterComponent>())
			{
				CommandableComponent* commandableComponentTarget = context.mTargetEntity->getComponent<CommandableComponent>();
				if (commandableComponentTarget->canTransportInjured())
				{
					return true;
				}
			}
		}
		return false;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
