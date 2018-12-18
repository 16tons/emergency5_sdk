// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/logic/local/ambulance/DoctorLogic.h"
#include "em5/action/base/ReservateTargetAction.h"
#include "em5/action/base/SetInvincibleAction.h"
#include "em5/action/base/WaitForEnterVehicleAction.h"
#include "em5/action/SignalAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/move/StartHelicopterAction.h"
#include "em5/action/equipment/GetEquipmentAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/WaitForSignalAction.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/command/CommandContext.h"
#include "em5/command/base/EnterVehicleCommand.h"
#include "em5/command/ambulance/TreatPersonCommand.h"
#include "em5/health/HealthComponent.h"
#include "em5/specs/LogicSpecsGroup.h"
#include "em5/specs/RoadVehicleSpecsGroup.h"
#ifndef ENDUSER
	#include "em5/debug/GameDebugGroup.h"
#endif
#include "em5/game/targetpoint/GetEquipmentTargetPointProvider.h"
#include "em5/game/targetpoint/EnterCoDriverDoorTargetPointProvider.h"
#include "em5/game/units/UnitHelper.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/logic/local/EntityLogicHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/network/NetworkManager.h"
#include "em5/settings/GameSettingsGroup.h"
#include "em5/plugin/Messages.h"
#include "em5/plugin/Jobs.h"
#include "em5/EM5Helper.h"

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf_game/base/DistanceHelper.h>

#include <qsf/job/JobArguments.h>
#ifndef ENDUSER
	#include <qsf/debug/DebugDrawManager.h>
	#include <qsf/debug/request/TextDebugDrawRequest.h>
#endif
#include <qsf/component/base/TransformComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/map/Map.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 DoctorLogic::GAMELOGIC_TYPE_ID = qsf::StringHash("em5::DoctorLogic");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 DoctorLogic::ACTION_PRIORITY = action::AUTOMATISM_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	DoctorLogic::DoctorLogic() :
		CarryPersonLogic(GAMELOGIC_TYPE_ID)
	{
		// Nothing here
	}

	void DoctorLogic::takeMedikitFromVehicle(const qsf::MessageParameters& parameters)
	{
		if (Messages::EM5_ACTION_PERSON_EXIT_VEHICLE.getHash() == parameters.getFilter(0))
		{
			// The doctor is about to leave the vehicle
			const uint64 vehicleId = parameters.getFilter(2);
			qsf::Entity* ambulanceVehicle = getEntity()->getMap().getEntityById(vehicleId);
			QSF_CHECK(nullptr != ambulanceVehicle, "DoctorLogic::takeMedikitFromVehicle(): There is no vehicle with the ID '" << vehicleId << "' where the doctor '" << getEntityId() << "' could be inside", return);

			if (UnitHelper(*ambulanceVehicle).isAmbulanceIMT() || RoadVehicleSpecsGroup::getInstanceSafe().getStartWithEquipmentCase())
			{
				// Special case for the IMT, doctor has the medikit instantly or taking equipment from vehicle back door only if the settings are set
				EntityHelper(*getEntity()).showEquipment(equipment::AMBULANCE_MEDIKIT);
				return;
			}

			ReserveLogic::createReservation<ReserveEnterVehicleLogic>(*ambulanceVehicle, getEntityId());
			EntityHelper(*getEntity()).setInvincible(true);	// Here immediately set the entity invincible

			// Push doctor actions
			qsf::ActionComponent& actionComponent = getEntity()->getOrCreateComponentSafe<qsf::ActionComponent>();
			if (EntityHelper(*ambulanceVehicle).isHelicopter())
			{
				actionComponent.pushAction<MoveAction>(action::BLOCKING, qsf::action::APPEND_TO_BACK).init(new qsf::ai::ReachObjectGoal(*getEntity(), *ambulanceVehicle, EnterCoDriverDoorTargetPointProvider::TARGET_POINT_ID));
				actionComponent.pushAction<GetEquipmentAction>(action::BLOCKING, qsf::action::APPEND_TO_BACK).init(equipment::AMBULANCE_MEDIKIT);
			}
			else
			{
				actionComponent.pushAction<MoveAction>(action::BLOCKING, qsf::action::APPEND_TO_BACK).init(new qsf::ai::ReachObjectGoal(*getEntity(), *ambulanceVehicle, GetEquipmentTargetPointProvider::TARGET_POINT_ID));
				actionComponent.pushAction<GetEquipmentAction>(action::BLOCKING, qsf::action::APPEND_TO_BACK).init(*ambulanceVehicle, equipment::AMBULANCE_MEDIKIT);
			}

			// For both cases, remove the reservation logic
			actionComponent.pushAction<ReservateTargetAction>(action::BLOCKING, qsf::action::APPEND_TO_BACK).initDeleteReservation(vehicleId);
			actionComponent.pushAction<SetInvincibleAction>(action::BLOCKING, qsf::action::APPEND_TO_BACK).init(false);

			// Signal the vehicle that we took the equipment
			qsf::MessageConfiguration message(Messages::EM5_ACTION_SIGNAL, getId());
			actionComponent.pushAction<SignalAction>(action::BLOCKING, qsf::action::APPEND_TO_BACK).init(message);

			// The vehicle should wait for the person has taken its equipment
			qsf::ActionComponent& vehicleActionComponent = ambulanceVehicle->getOrCreateComponentSafe<qsf::ActionComponent>();
			vehicleActionComponent.pushAction<WaitForSignalAction>(action::BLOCKING).init(message, qsf::Time::fromSeconds(10.f));
		}
	}

	void DoctorLogic::putMedikitInVehicle(const qsf::MessageParameters& parameters)
	{
		if (Messages::EM5_ON_COMMAND_EXECUTE.getHash() == parameters.getFilter(0) && EnterVehicleCommand::PLUGINABLE_ID == parameters.getFilter(1) &&
			getEntityId() == parameters.getFilter(2))
		{
			// TODO(mk) Currently we reach this code if the doctor wants to enter an other vehicle than the ambulance car!

			const uint64 vehicleId = parameters.getFilter(3);
			qsf::Entity* ambulanceVehicle = getEntity()->getMap().getEntityById(vehicleId);
			QSF_CHECK(nullptr != ambulanceVehicle, "DoctorLogic::putMedikitInVehicle(): There is no entity with the ID '" << vehicleId << "' for the enter vehicle command of the doctor '" << getEntityId() << "'", return);

			if (UnitHelper(*ambulanceVehicle).isAmbulanceIMT() || RoadVehicleSpecsGroup::getInstanceSafe().getStartWithEquipmentCase())
			{
				return;
			}

			// Reset the action priority of the command actions
			parameters.respond(static_cast<uint32>(action::BLOCKING));

			// Push doctor actions
			qsf::ActionComponent& actionComponent = getEntity()->getOrCreateComponentSafe<qsf::ActionComponent>();

			// Go to the vehicle, pushing actions in reverse order, its important to block this action
			actionComponent.pushAction<SetInvincibleAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(false);
			if (EntityHelper(*ambulanceVehicle).isHelicopter())
			{
				actionComponent.pushAction<ReservateTargetAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(vehicleId, ReserveEnterVehicleLogic::GAMELOGIC_TYPE_ID);
				actionComponent.pushAction<GetEquipmentAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init("");
				actionComponent.pushAction<SetInvincibleAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(true);

				// We push the signal action after the move action so that the vehicle waits when the unit starts putting back its equipment into the vehicle
				pushSignalActionAndSetupMessageHandler(vehicleId, actionComponent);

				actionComponent.pushAction<MoveAction>(action::COMMAND_LOW, qsf::action::INSERT_AT_FRONT).init(new qsf::ai::ReachObjectGoal(*getEntity(), *ambulanceVehicle, EnterCoDriverDoorTargetPointProvider::TARGET_POINT_ID));
			}
			else
			{
				actionComponent.pushAction<ReservateTargetAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(vehicleId, ReserveEnterVehicleLogic::GAMELOGIC_TYPE_ID);
				actionComponent.pushAction<GetEquipmentAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(*ambulanceVehicle, "");
				actionComponent.pushAction<SetInvincibleAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(true);

				// We push the signal action after the move action so that the vehicle waits when the unit starts putting back its equipment into the vehicle
				pushSignalActionAndSetupMessageHandler(vehicleId, actionComponent);

				actionComponent.pushAction<MoveAction>(action::COMMAND_LOW, qsf::action::INSERT_AT_FRONT).init(new qsf::ai::ReachObjectGoal(*getEntity(), *ambulanceVehicle, GetEquipmentTargetPointProvider::TARGET_POINT_ID));
			}
		}
	}

	void DoctorLogic::startWaitForEnterVehicle(const qsf::MessageParameters& parameters)
	{
		// Block the vehicle the entity wants to enter
		uint64 vehicleId = qsf::getUninitialized<uint64>();
		parameters.getParameter("vehicleId", vehicleId);
		qsf::Entity* vehicleEntity = getMap().getEntityById(vehicleId);
		if (nullptr != vehicleEntity)
		{
			qsf::ActionComponent& vehicleActionComponent = vehicleEntity->getOrCreateComponentSafe<qsf::ActionComponent>();

			if (EntityHelper(*vehicleEntity).isHelicopter() && vehicleActionComponent.getPlan().getActions().size() > 1 && vehicleActionComponent.getAction<StartHelicopterAction>() != nullptr)
			{
				// Helicopter is in air, stop every future movement
				getEntity()->getComponent<qsf::ActionComponent>()->clearPlan();
				return;
			}

			// If we have already an wait action, take this
			WaitForEnterVehicleAction* waitForEnterVehicleAction = vehicleActionComponent.getAction<WaitForEnterVehicleAction>();
			if (nullptr == waitForEnterVehicleAction)
			{
				waitForEnterVehicleAction = &vehicleActionComponent.pushAction<WaitForEnterVehicleAction>(action::BLOCKING);
			}

			// Init or re-init the wait action
			waitForEnterVehicleAction->init(getEntityId(), qsf::Time::fromSeconds(10.f), true);
		}

		mEnterVehicleWithoutEquipmentProxy.unregister();
		mEnterVehicleWithoutEquipmentAbortProxy.unregister();
	}

	void DoctorLogic::abortWaitForEnterVehicle(const qsf::MessageParameters& parameters)
	{
		// The only job here is to unregister the "mEnterVehicleWithoutEquipmentProxy"
		mEnterVehicleWithoutEquipmentProxy.unregister();
		mEnterVehicleWithoutEquipmentAbortProxy.unregister();
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::CarryPersonLogic methods          ]
	//[-------------------------------------------------------]
	void DoctorLogic::setCarryPerson(uint64 carryPersonId)
	{
		// Restore the doctor medicase, but not on multiplay client
		if (nullptr == EM5_NETWORK.getMultiplayerClient())
		{
			EntityHelper entityHelper(*getEntity());
			if (carryPersonId == qsf::getUninitialized<uint64>() && !entityHelper.checkIsActiveEquipment(equipment::AMBULANCE_MEDIKIT))
			{
				entityHelper.showEquipment(equipment::AMBULANCE_MEDIKIT);
			}
		}

		// Call the base class
		CarryPersonLogic::setCarryPerson(carryPersonId);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	bool DoctorLogic::onStartup()
	{
		// Don't do anything, when on multiplay client side
		if (nullptr == EM5_NETWORK.getMultiplayerClient())
		{
			// Register job
			// -> For performance reasons, do updates not in every tick, but in fixed time intervals
			qsf::jobs::JobConfiguration jobConfiguration;
			jobConfiguration.mTimeBetweenCalls = qsf::Time::fromSeconds(1.0f);
			mSimulationJobProxy.registerAt(Jobs::SIMULATION_LOGIC, boost::bind(&DoctorLogic::updateSimulation, this, _1), jobConfiguration);

			// Register logic for taking equipment from vehicle back door only if the settings are set
			//if (!RoadVehicleSpecsGroup::getInstanceSafe().getStartWithEquipmentCase())
			{
				// Register messages
				mEnterVehicleProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_ON_COMMAND_EXECUTE, EnterVehicleCommand::PLUGINABLE_ID, getEntityId()), boost::bind(&DoctorLogic::putMedikitInVehicle, this, _1));
				mExitVehicleProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_ACTION_PERSON_EXIT_VEHICLE, getEntityId()), boost::bind(&DoctorLogic::takeMedikitFromVehicle, this, _1));
			}
		}
		return true;
	}

	void DoctorLogic::onShutdown()
	{
		// Unregister job
		mSimulationJobProxy.unregister();
		mExitVehicleProxy.unregister();
		mEnterVehicleProxy.unregister();
		mEnterVehicleWithoutEquipmentProxy.unregister();
		mEnterVehicleWithoutEquipmentAbortProxy.unregister();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void DoctorLogic::updateSimulation(const qsf::JobArguments& jobArguments)
	{
		// Access the caller's action plan
		const qsf::ActionComponent& actionComponent = getEntity()->getOrCreateComponentSafe<qsf::ActionComponent>();

		// Check if the paramedic is currently busy
		bool isBusy = (actionComponent.getCurrentPriority() >= action::AUTOMATISM_STD);	// Overwrite idle actions and low-priority automatisms

		if (!MapHelper::isUnitAutomatismActive())
		{
			// No automatism active
			isBusy = true;
		}

		// Check if the person is inside a road vehicle
		EntityHelper entityHelper(*getEntity());
		if (entityHelper.isEntityInVehicle() || entityHelper.isPersonInjured())
		{
			isBusy = true;
		}

		if (isBusy)
		{
			// Reset wait time
			mTimeWaiting = qsf::Time::ZERO;
		}
		else
		{
			// Wait some time until getting active
			mTimeWaiting += jobArguments.getTimePassed();

			if (!entityHelper.checkIsActiveEquipment(equipment::AMBULANCE_MEDIKIT) && getCarryPerson() == qsf::getUninitialized<uint64>())
			{
				// Fall back if something goes wrong and the doctor has no medicase. should not reach this code
				// Only do this if the doctor is not guiding a person
				QSF_WARN("DoctorLogic::updateSimulation() Doctor without medikit, give it him back manually", QSF_REACT_NONE);
				entityHelper.showEquipment(equipment::AMBULANCE_MEDIKIT);
			}

			// Waited long enough now?
			if (mTimeWaiting >= qsf::Time::fromSeconds(LogicSpecsGroup::getInstanceSafe().getDoctorMinimumWaitingTime()))
			{
				// Perform the automatism now
				performAutomatism();

				// Reset wait time
				mTimeWaiting = qsf::Time::ZERO;
			}
		}
	}

	void DoctorLogic::performAutomatism()
	{
		GameSettingsGroup::AutomatismValue automatismValue = GameSettingsGroup::getInstanceSafe().getAutomatismValue();
		if (automatismValue >= GameSettingsGroup::AUTOMATISM_MEDIUM)
		{
			performTreatmentAutomatism();
		}
	}

	void DoctorLogic::performTreatmentAutomatism()
	{
		// Get the transform component for the position
		const qsf::TransformComponent* transformComponent = getEntity()->getComponent<qsf::TransformComponent>();
		if (nullptr == transformComponent)
			return;

		// Search for road vehicle components in the radius
		std::vector<HealthComponent*> healthComponentVector;
		qsf::ComponentMapQuery(getMap()).getInstancesInCircle(transformComponent->getPosition(), LogicSpecsGroup::getInstanceSafe().getDoctorSearchRadius(), healthComponentVector);

		// Sort the list for the distance
		qsf::game::IsLeftComponentCloserToTargetPosition compareObject(transformComponent->getPosition());
		std::sort(healthComponentVector.begin(), healthComponentVector.end(), compareObject);

		for (HealthComponent* healthComponent : healthComponentVector)
		{
			if (EntityLogicHelper::checkAndExecuteAutomatism<TreatPersonCommand>(getEntity(), &healthComponent->getEntity()))
			{
				#ifndef ENDUSER
					if (GameDebugGroup::getInstanceSafe().getShowAutomatismDebug())
					{
						QSF_DEBUGDRAW.requestDraw(qsf::TextDebugDrawRequest("auto:TreatPersonCommand", transformComponent->getPosition(), qsf::Color4::YELLOW), qsf::DebugDrawLifetimeData(qsf::Time::fromSeconds(5.0f)));
					}
				#endif
				break;
			}
		}
	}

	void DoctorLogic::pushSignalActionAndSetupMessageHandler(uint64 vehicleEntityId, qsf::ActionComponent& actionComponent)
	{
		// At stuck resolving. Send a signal message and we listen to this message. Block the vehicle we want to enter
		qsf::MessageParameters parameters;
		parameters.setParameter("actorId", getEntityId());
		parameters.setParameter("vehicleId", vehicleEntityId);
		qsf::MessageConfiguration enterMessage(Messages::EM5_LOGIC_ENTER_VEHICLE_WITHOUT_EQUIPMENT, getEntityId());
		qsf::MessageConfiguration abortMessage("abortEnterVehicleWithoutEquipment", getEntityId());
		actionComponent.pushAction<SignalAction>(action::COMMAND_LOW, qsf::action::INSERT_AT_FRONT).init(enterMessage, abortMessage, parameters);

		mEnterVehicleWithoutEquipmentProxy.registerAt(enterMessage, boost::bind(&DoctorLogic::startWaitForEnterVehicle, this, _1));
		mEnterVehicleWithoutEquipmentAbortProxy.registerAt(abortMessage, boost::bind(&DoctorLogic::abortWaitForEnterVehicle, this, _1));
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
