// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/logic/local/tech/EngineerLogic.h"
#include "em5/action/base/ReservateTargetAction.h"
#include "em5/action/base/SetInvincibleAction.h"
#include "em5/action/base/WaitForEnterVehicleAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/equipment/GetEquipmentAction.h"
#include "em5/action/SignalAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/WaitForSignalAction.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/command/base/EnterVehicleCommand.h"
#include "em5/game/targetpoint/GetEquipmentTargetPointProvider.h"
#include "em5/game/targetpoint/EnterCoDriverDoorTargetPointProvider.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/map/EntityHelper.h"
#include "em5/specs/RoadVehicleSpecsGroup.h"
#include "em5/plugin/Messages.h"

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

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
	const uint32 EngineerLogic::GAMELOGIC_TYPE_ID = qsf::StringHash("em5::EngineerLogic");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 EngineerLogic::ACTION_PRIORITY = action::AUTOMATISM_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	EngineerLogic::EngineerLogic() :
		qsf::GameLogic(GAMELOGIC_TYPE_ID)
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	bool EngineerLogic::onStartup()
	{
		// Register messages
		mEnterVehicleProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_ON_COMMAND_EXECUTE, EnterVehicleCommand::PLUGINABLE_ID, getEntityId()), boost::bind(&EngineerLogic::putToolboxInVehicle, this, _1));
		mExitVehicleProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_ACTION_PERSON_EXIT_VEHICLE, getEntityId()), boost::bind(&EngineerLogic::takeToolboxFromVehicle, this, _1));

		return true;
	}

	void EngineerLogic::onShutdown()
	{
		// Unregister job
		mExitVehicleProxy.unregister();
		mEnterVehicleProxy.unregister();
		mEnterVehicleWithoutEquipmentProxy.unregister();
		mEnterVehicleWithoutEquipmentAbortProxy.unregister();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void EngineerLogic::takeToolboxFromVehicle(const qsf::MessageParameters& parameters)
	{
		if (Messages::EM5_ACTION_PERSON_EXIT_VEHICLE.getHash() == parameters.getFilter(0))
		{
			// The unit is about to leave the vehicle
			const uint64 vehicleId = parameters.getFilter(2);
			qsf::Entity* vehicleEntity = getEntity()->getMap().getEntityById(vehicleId);
			QSF_CHECK(nullptr != vehicleEntity, "EngineerLogic::takeToolboxFromVehicle: There is no vehicle with the ID '" << vehicleId << "' where the unit '" << getEntityId() << "' could be inside", return);

			if (RoadVehicleSpecsGroup::getInstanceSafe().getStartWithEquipmentCase())
			{
				// Special case for the IMT, doctor has the medikit instantly or taking equipment from vehicle back door only if the settings are set
				EntityHelper(*getEntity()).showEquipment(equipment::TECH_TOOLBOX);
				return;
			}

			// Push unit actions
			qsf::ActionComponent& actionComponent = getEntity()->getOrCreateComponentSafe<qsf::ActionComponent>();
			actionComponent.clearPlan();

			ReserveLogic::createReservation<ReserveEnterVehicleLogic>(*vehicleEntity, getEntityId());
			EntityHelper(*getEntity()).setInvincible(true);	// Here immediately set the entity invincible

			if (EntityHelper(*vehicleEntity).isHelicopter())
			{
				actionComponent.pushAction<MoveAction>(action::BLOCKING, qsf::action::APPEND_TO_BACK).init(new qsf::ai::ReachObjectGoal(*getEntity(), *vehicleEntity, EnterCoDriverDoorTargetPointProvider::TARGET_POINT_ID));
				actionComponent.pushAction<GetEquipmentAction>(action::BLOCKING, qsf::action::APPEND_TO_BACK).init(equipment::TECH_TOOLBOX);
			}
			else
			{
				actionComponent.pushAction<MoveAction>(action::BLOCKING, qsf::action::APPEND_TO_BACK).init(new qsf::ai::ReachObjectGoal(*getEntity(), *vehicleEntity, GetEquipmentTargetPointProvider::TARGET_POINT_ID));
				actionComponent.pushAction<GetEquipmentAction>(action::BLOCKING, qsf::action::APPEND_TO_BACK).init(*vehicleEntity, equipment::TECH_TOOLBOX);
			}

			// For both cases, remove the reservation logic
			actionComponent.pushAction<ReservateTargetAction>(action::BLOCKING, qsf::action::APPEND_TO_BACK).initDeleteReservation(vehicleId);
			actionComponent.pushAction<SetInvincibleAction>(action::BLOCKING, qsf::action::APPEND_TO_BACK).init(false);

			// Signal the vehicle that we took the equipment
			qsf::MessageConfiguration message(Messages::EM5_ACTION_SIGNAL, getId());
			actionComponent.pushAction<SignalAction>(action::BLOCKING, qsf::action::APPEND_TO_BACK).init(message);

			// The vehicle should wait for the person has taken its equipment
			qsf::ActionComponent& vehicleActionComponent = vehicleEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
			vehicleActionComponent.pushAction<WaitForSignalAction>(action::BLOCKING).init(message, qsf::Time::fromSeconds(10.f));
		}
	}

	void EngineerLogic::putToolboxInVehicle(const qsf::MessageParameters& parameters)
	{
		if (Messages::EM5_ON_COMMAND_EXECUTE.getHash() == parameters.getFilter(0) && EnterVehicleCommand::PLUGINABLE_ID == parameters.getFilter(1) &&
			getEntityId() == parameters.getFilter(2))
		{
			// TODO(mk) Currently we reach this code if the unit wants to enter any other vehicle than its own

			if (RoadVehicleSpecsGroup::getInstanceSafe().getStartWithEquipmentCase())
			{
				return;
			}

			// Reset the action priority of the command actions
			parameters.respond(static_cast<uint32>(action::BLOCKING));

			const uint64 vehicleId = parameters.getFilter(3);
			qsf::Entity* vehicleEntity = getEntity()->getMap().getEntityById(vehicleId);
			QSF_CHECK(nullptr != vehicleEntity, "EngineerLogic::putToolboxInVehicle: There is no entity with the ID '" << vehicleId << "' for the enter vehicle command of the unit '" << getEntityId() << "'", return);

			// Push unit actions
			qsf::ActionComponent& actionComponent = getEntity()->getOrCreateComponentSafe<qsf::ActionComponent>();

			actionComponent.pushAction<SetInvincibleAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(false);

			// Go to the vehicle, pushing actions in reverse order, it's important to block this action
			if (EntityHelper(*vehicleEntity).isHelicopter())
			{
				actionComponent.pushAction<ReservateTargetAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(vehicleId, ReserveEnterVehicleLogic::GAMELOGIC_TYPE_ID);
				actionComponent.pushAction<GetEquipmentAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init("");
				actionComponent.pushAction<SetInvincibleAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(true);

				// We push the signal action after the move action so that the vehicle waits when the unit starts putting back its equipment into the vehicle
				pushSignalActionAndSetupMessageHandler(vehicleEntity->getId(), actionComponent);

				actionComponent.pushAction<MoveAction>(action::COMMAND_LOW, qsf::action::INSERT_AT_FRONT).init(new qsf::ai::ReachObjectGoal(*getEntity(), *vehicleEntity, EnterCoDriverDoorTargetPointProvider::TARGET_POINT_ID));
			}
			else
			{
				actionComponent.pushAction<ReservateTargetAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(vehicleId, ReserveEnterVehicleLogic::GAMELOGIC_TYPE_ID);
				actionComponent.pushAction<GetEquipmentAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(*vehicleEntity, "");
				actionComponent.pushAction<SetInvincibleAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(true);

				// We push the signal action after the move action so that the vehicle waits when the unit starts putting back its equipment into the vehicle
				pushSignalActionAndSetupMessageHandler(vehicleEntity->getId(), actionComponent);

				actionComponent.pushAction<MoveAction>(action::COMMAND_LOW, qsf::action::INSERT_AT_FRONT).init(new qsf::ai::ReachObjectGoal(*getEntity(), *vehicleEntity, GetEquipmentTargetPointProvider::TARGET_POINT_ID));
			}
		}
	}

	void EngineerLogic::startWaitForEnterVehicle(const qsf::MessageParameters& parameters)
	{
		// Block the vehicle the entity wants to enter
		uint64 vehicleId = qsf::getUninitialized<uint64>();
		parameters.getParameter("vehicleId", vehicleId);
		qsf::Entity* vehicleEntity = getMap().getEntityById(vehicleId);
		if (nullptr != vehicleEntity)
		{
			qsf::ActionComponent& vehicleActionComponent = vehicleEntity->getOrCreateComponentSafe<qsf::ActionComponent>();

			// If we have already an wait action, take this
			WaitForEnterVehicleAction* waitForEnterVehicleAction = vehicleActionComponent.getAction<WaitForEnterVehicleAction>();
			if (nullptr == waitForEnterVehicleAction)
			{
				waitForEnterVehicleAction = &vehicleActionComponent.pushAction<WaitForEnterVehicleAction>(action::BLOCKING);
			}

			// Init or re-init the wait action
			waitForEnterVehicleAction->init(getEntityId(), qsf::Time::fromSeconds(10.0f), true);
		}

		mEnterVehicleWithoutEquipmentProxy.unregister();
		mEnterVehicleWithoutEquipmentAbortProxy.unregister();
	}

	void EngineerLogic::abortWaitForEnterVehicle(const qsf::MessageParameters& parameters)
	{
		// The only job here is to unregister the "mEnterVehicleWithoutEquipmentProxy"
		mEnterVehicleWithoutEquipmentProxy.unregister();
		mEnterVehicleWithoutEquipmentAbortProxy.unregister();
	}

	void EngineerLogic::pushSignalActionAndSetupMessageHandler(uint64 vehicleEntityId, qsf::ActionComponent& actionComponent)
	{
		// At stuck resolving. Send a signal message and we listen to this message. Block the vehicle we want to enter
		qsf::MessageParameters parameters;
		parameters.setParameter("actorId", getEntityId());
		parameters.setParameter("vehicleId", vehicleEntityId);
		qsf::MessageConfiguration enterMessage(Messages::EM5_LOGIC_ENTER_VEHICLE_WITHOUT_EQUIPMENT, getEntityId());
		qsf::MessageConfiguration abortMessage("abortEnterVehicleWithoutEquipment", getEntityId());
		actionComponent.pushAction<SignalAction>(action::COMMAND_LOW, qsf::action::INSERT_AT_FRONT).init(enterMessage, abortMessage, parameters);

		mEnterVehicleWithoutEquipmentProxy.registerAt(enterMessage, boost::bind(&EngineerLogic::startWaitForEnterVehicle, this, _1));
		mEnterVehicleWithoutEquipmentAbortProxy.registerAt(abortMessage, boost::bind(&EngineerLogic::abortWaitForEnterVehicle, this, _1));
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
