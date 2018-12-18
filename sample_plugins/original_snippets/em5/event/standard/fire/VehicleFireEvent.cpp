// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/fire/VehicleFireEvent.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/SignalAction.h"
#include "em5/action/move/FullBrakeAction.h"
#include "em5/action/base/ExitVehicleAction.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include "em5/command/fire/GetRescueCutterCommand.h"
#include "em5/freeplay/objective/ObjectiveHelper.h"
#include "em5/logic/ObserverHelper.h"
#include "em5/logic/HintHelper.h"
#include "em5/logic/observer/PersonDiedObserver.h"
#include "em5/logic/observer/hint/HintMessageGeneralObserver.h"
#include "em5/logic/observer/hint/HintLowEnergyObserver.h"
#include "em5/logic/observer/generic/MessageObserver.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Messages.h"

#include <qsf/audio/component/DynamicMusicCompositorComponent.h>
#include <qsf/component/move/MovableComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Local definitions                                     ]
	//[-------------------------------------------------------]
	// Hints
	const std::string BURNINGCAR_HINT_01 = "EM5_EVENT_BURNINGCAR_HINT_01";	// Hint vehicle has only X% lifenergy
	const std::string BURNINGCAR_HINT_03 = "EM5_EVENT_BURNINGCAR_HINT_03";
	const float BURNINGCAR_HINT_01_LIFENERGY(0.2f);


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 VehicleFireEvent::FREEPLAY_EVENT_ID = qsf::StringHash("em5::VehicleFireEvent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	VehicleFireEvent::VehicleFireEvent() :
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mDeadPersonsToFail(0),
		mPersonsDamage(false),
		mMessage(Messages::EM5_EVENT_TIMER_SIGNAL, getId())
	{
		// Nothing here
	}

	VehicleFireEvent::~VehicleFireEvent()
	{
		// Nothing here
	}

	uint64 VehicleFireEvent::getTargetEntityId() const
	{
		return mTargetEntityId;
	}

	void VehicleFireEvent::setTargetEntity(const qsf::Entity& entity)
	{
		mTargetEntityId = entity.getId();
	}

	uint32 VehicleFireEvent::getDeadPersonsToFail() const
	{
		return mDeadPersonsToFail;
	}

	void VehicleFireEvent::setDeadPersonsToFail(uint32 personsToFail)
	{
		mDeadPersonsToFail = personsToFail;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	bool VehicleFireEvent::onStartup()
	{
		// Event is now running
		mMessageProxy.registerAt(mMessage, boost::bind(&VehicleFireEvent::onSignal, this, _1));

		qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
		QSF_CHECK(nullptr != targetEntity, "Could not find entity with ID: " << mTargetEntityId, return false);

		// Mark the entity so it wont get effected by other events
		EventIdComponent& eventIdComponent = targetEntity->getOrCreateComponentSafe<EventIdComponent>();
		eventIdComponent.setEvent(*this, eventspreadreason::NO_REASON);

		// Check if vehicle has persons inside
		if (mPersonsDamage)
		{
			QSF_CHECK(!EntityHelper(*targetEntity).isContainerEmpty(), "No person inside vehicle but should be, because its a vehicle fire with person damage: " << mTargetEntityId, QSF_REACT_NONE);
		}

		// Stop the vehicle
		stopTheCar(*targetEntity);

		// Done
		return true;
	}

	void VehicleFireEvent::onShutdown()
	{
		mMessageProxy.unregister();
	}

	void VehicleFireEvent::onRun()
	{
		// No null pointer check in here. The factory already checked these pointers.

		// Get the vehicle to burn
		qsf::Entity& targetEntity = *getMap().getEntityById(mTargetEntityId);

		// The persons should get damaged than, well do it
		if (mPersonsDamage && !EntityHelper(targetEntity).isContainerEmpty())
		{
			RoadVehicleComponent& roadVehicleComponent = targetEntity.getComponentSafe<RoadVehicleComponent>();
			roadVehicleComponent.trapPersons(this);
			roadVehicleComponent.setCanSpawnDriver(false);
		}

		// Remove event ID component
		targetEntity.destroyComponent<EventIdComponent>();

		// Let the vehicle burn
		EntityHelper(targetEntity).startFire(this);

		// Set all observers and objectives for the first target
		startObjectives(targetEntity);

		// Set start hint observers after the event entity was added
		startHintObservers(targetEntity);

		// Show supervisor message
		showSupervisorMessage(HintHelper::getRandomStringOfIdString(HintHelper(*this).getHintParameters().mSupervisorMessageIds));

		createEventLocationEntity(targetEntity);
	}

	void VehicleFireEvent::updateFreeplayEvent(const qsf::Time& timePassed)
	{
		// Check objectives for success or failure
		checkObjectivesState();
	}

	bool VehicleFireEvent::addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason)
	{
		if (eventspreadreason::INJURY == eventSpreadReason)
		{
			// Get or create fail conditions
			Objective& failConditionPersonDiedObjective = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_FAIL_DEADPERSONS);

			// Create and connect observers
			PersonDiedObserver& observerDied = ObserverHelper(*this).createObserver<PersonDiedObserver>(targetEntity.getId());
			observerDied.connectToObjective(failConditionPersonDiedObjective);

			// Call base implementation
			FreeplayEvent::addEntityToEvent(targetEntity, eventSpreadReason, newReason);

			// Done
			return true;
		}

		// Call base implementation
		return FreeplayEvent::addEntityToEvent(targetEntity, eventSpreadReason, newReason);
	}

	const qsf::Entity* VehicleFireEvent::getFocusEntity()
	{
		const qsf::Map& map = getMap();

		{ // Is the first vehicle still burning?
			qsf::Entity* candidateEntity = map.getEntityById(mTargetEntityId);
			if (nullptr != candidateEntity && EntityHelper(*candidateEntity).isBurning())
			{
				return candidateEntity;
			}
		}

		const ObjectiveHelper objectiveHelper(*this);

		{ // Is there any other fire left?
			const Objective* extingushFireObjective = objectiveHelper.getObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_EXTINGUISHFIRES);
			if (nullptr != extingushFireObjective)
			{
				const qsf::Entity* candidateEntity = map.getEntityById(extingushFireObjective->getRandomNeededEntityId());
				if (nullptr != candidateEntity)
				{
					return candidateEntity;
				}
			}
		}

		{ // Are there any persons injured?
			const Objective* treatPersonsObjective = objectiveHelper.getObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_TREATPERSONS);
			if (nullptr != treatPersonsObjective)
			{
				const qsf::Entity* candidateEntity = map.getEntityById(treatPersonsObjective->getRandomNeededEntityId());
				if (nullptr != candidateEntity)
				{
					return candidateEntity;
				}
			}
		}

		return getEventLocationEntity();
	}

	bool VehicleFireEvent::checkEventConfiguration()
	{
		// TODO(mk) Add Check if event has right configuration an can start now

		return true;
	}

	void VehicleFireEvent::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base Implementation
		FreeplayEvent::serialize(serializer);

		serializer.serialize(mTargetEntityId);
		serializer.serialize(mDeadPersonsToFail);
		serializer.serialize(mPersonsDamage);

		mMessage.serialize(serializer);

		if (serializer.isReading())
		{
			mMessageProxy.registerAt(mMessage, boost::bind(&VehicleFireEvent::onSignal, this, _1));
		}
		else
		{
			mMessageProxy.unregister();
		}
	}

	void VehicleFireEvent::hintCallback(Observer& hintObserver)
	{
		showHint(hintObserver.getName(), false);

		// Music
		if (hintObserver.getName() == BURNINGCAR_HINT_01)
		{
			// Play short action music
			getDynamicMusicCompositor().setMusicLevel(3);
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void VehicleFireEvent::startObjectives(const qsf::Entity& targetEntity)
	{
		if (mDeadPersonsToFail > 0)
		{
			// Create the fail condition
			Objective& failConditionPersonDiedObjective = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_FAIL_DEADPERSONS);
			failConditionPersonDiedObjective.setNeededNumber(mDeadPersonsToFail);
		}
	}

	void VehicleFireEvent::startHintObservers(const qsf::Entity& targetEntity)
	{
		const HintHelper hintHelper(*this);
		hintHelper.createAndInitializeGeneralHintsAtInjured();
		hintHelper.createAndInitializeGeneralHintAtDamagedVehicle();

		// Hint 01: Vehicle has only X% lifenergy
		createObserver<HintLowEnergyObserver>(mTargetEntityId, BURNINGCAR_HINT_01).initialize(BURNINGCAR_HINT_01_LIFENERGY, HintLowEnergyObserver::HINTENERGYTYPE_LIFE);

		if (mPersonsDamage)
		{
			// Hint 03: Firefighter have rescue cutter equipped but the vehicle is still burning
			MessageObserver& observer = createGeneralObserver<MessageObserver>("INTERN_OBSERVER");
			observer.initialize(
				qsf::MessageConfiguration(Messages::EM5_POST_COMMAND_EXECUTE, GetRescueCutterCommand::PLUGINABLE_ID),
				qsf::MessageConfiguration(Messages::EM5_ACTION_EXTINGUISH_FIRE_STARTED, mTargetEntityId),
				MessageObserver::AND_LOGIC, MessageObserver::FALSE_CHECK);

			createGeneralObserver<HintMessageGeneralObserver>(BURNINGCAR_HINT_03).initialize(
				qsf::MessageConfiguration(Messages::EM5_OBSERVER_MESSAGE, observer.getId()));
		}
	}

	void VehicleFireEvent::onSignal(const qsf::MessageParameters& parameters)
	{
		qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
		if (nullptr != targetEntity)
		{
			if (0.0f == targetEntity->getComponentSafe<qsf::MovableComponent>().getSpeed())
			{
				// Get the show running
				setRunning();
			}
			else
			{
				// Try to stop the car again
				stopTheCar(*targetEntity);
			}
		}
		else
		{
			abortEvent();
		}
	}

	void VehicleFireEvent::stopTheCar(qsf::Entity& targetEntity)
	{
		qsf::ActionComponent* actionComponent = targetEntity.getComponent<qsf::ActionComponent>();
		if (nullptr != actionComponent)
		{
			// Let the vehicle stop
			actionComponent->clearPlan();
			actionComponent->pushAction<FullBrakeAction>(action::Priority::COMMAND_HIGH).init(false);
			actionComponent->pushAction<SignalAction>(action::Priority::BLOCKING).init(mMessage);

			if (mPersonsDamage == false)
			{
				// Add the exit vehicle action
				actionComponent->pushAction<ExitVehicleAction>(action::BLOCKING);
			}
		}
		else
		{
			// Immediately set running (instead of waiting for a signal action when the car stops, see above)
			setRunning();
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
