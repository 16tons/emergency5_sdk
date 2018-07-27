// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/medical/DrunkenOverboardEvent.h"
#include "em5/action/base/DrunkenPersonOverboardAction.h"
#include "em5/action/base/DisappearAction.h"
#include "em5/component/objects/DivingSpotComponent.h"
#include "em5/component/person/PersonPlacementComponent.h"
#include "em5/freeplay/objective/ObjectiveHelper.h"
#include "em5/game/Game.h"
#include "em5/health/HealthComponent.h"
#include "em5/logic/HintHelper.h"
#include "em5/logic/ObserverHelper.h"
#include "em5/logic/observer/hint/HintMessageGeneralObserver.h"
#include "em5/logic/observer/hint/HintLowEnergyObserver.h"
#include "em5/plugin/Messages.h"
#include "em5/EM5Helper.h"

#include <qsf_game/timer/GameTimerManager.h>

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/Map.h>
#include <qsf/math/Random.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 DrunkenOverboardEvent::FREEPLAY_EVENT_ID = qsf::StringHash("em5::DrunkenOverboardEvent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	DrunkenOverboardEvent::DrunkenOverboardEvent() :
		mTargetPersonEntityId(qsf::getUninitialized<uint64>()),
		mTargetShipEntityId(qsf::getUninitialized<uint64>()),
		mDivingSpotEntityId(qsf::getUninitialized<uint64>()),
		mMaxRunningTime(),
		mMessageProxy(),
		mTimerMessageProxy(),
		mIsPersonUnderwater(false)
	{
		// Nothing here
	}

	DrunkenOverboardEvent::~DrunkenOverboardEvent()
	{
		// Nothing here
	}

	uint64 DrunkenOverboardEvent::getTargetPersonId() const
	{
		return mTargetPersonEntityId;
	}

	void DrunkenOverboardEvent::setTargetPersonId(uint64 targetPersonId)
	{
		mTargetPersonEntityId = targetPersonId;
	}

	uint64 DrunkenOverboardEvent::getTargetShipId() const
	{
		return mTargetShipEntityId;
	}

	void DrunkenOverboardEvent::setTargetShipId(uint64 targetShipId)
	{
		mTargetShipEntityId = targetShipId;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	bool DrunkenOverboardEvent::onStartup()
	{
		qsf::Entity* personEntity = getMap().getEntityById(mTargetPersonEntityId);
		QSF_CHECK(personEntity, "Can't find person entity; DrunkenOverboardEvent method: onStartup", QSF_REACT_NONE);
		if (nullptr == personEntity)
			return false;

		personEntity->destroyComponent<PersonPlacementComponent>();

		// Create DrunkenPersonOverboardAction
		qsf::ActionComponent& actionComponent = personEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
		actionComponent.pushAction<DrunkenPersonOverboardAction>().init(mTargetShipEntityId);

		// Create message listener for reaction to DrunkenPersonOverboardAction
		qsf::MessageConfiguration message(Messages::EM5_CREATED_DIVINGSPOT);
		mMessageProxy.registerAt(message, boost::bind(&DrunkenOverboardEvent::onDivingSpotCreated, this, _1));

		// Event is now running
		setRunning();

		// Done
		return true;
	}

	void DrunkenOverboardEvent::onShutdown()
	{
		mMessageProxy.unregister();
	}

	void DrunkenOverboardEvent::onRun()
	{
		// No null pointer check in here. The factory already checked these pointers.
		qsf::Entity* personEntity = getMap().getEntityById(mTargetPersonEntityId);

		// Start objectives
		startObjectives(*personEntity);

		// Play supervisor message
		showSupervisorMessage("EM5_EVENT_SUPERV_OBJ_OVERBOARD_01");
	}

	bool DrunkenOverboardEvent::onFailure(EventResult& eventResult)
	{
		if (qsf::Random::getRandomChance(0.5f))
		{
			showHint("EM5_EVENT_TOO_MANY_PERSONS_DIED_HINT_03", false);
		}
		else
		{
			showHint("EM5_EVENT_TOO_MANY_PERSONS_DIED_HINT_04", false);
		}

		// Call base implementation
		return FreeplayEvent::onFailure(eventResult);
	}

	void DrunkenOverboardEvent::updateFreeplayEvent(const qsf::Time& timePassed)
	{
		// Check objectives for success or failure
		checkObjectivesState();
	}

	bool DrunkenOverboardEvent::addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason)
	{
		ObserverHelper observerHelper(*this);

		// Call base implementation
		return FreeplayEvent::addEntityToEvent(targetEntity, eventSpreadReason, newReason);
	}

	const qsf::Entity* DrunkenOverboardEvent::getFocusEntity()
	{
		qsf::Entity* candidateEntity = nullptr;

		if (mIsPersonUnderwater)
		{
			candidateEntity = getMap().getEntityById(mDivingSpotEntityId);
		}
		else
		{
			candidateEntity = getMap().getEntityById(mTargetPersonEntityId);
		}

		return candidateEntity;
	}

	bool DrunkenOverboardEvent::checkEventConfiguration()
	{
		return true;
	}

	void DrunkenOverboardEvent::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base implementation
		FreeplayEvent::serialize(serializer);

		serializer.serialize(mTargetPersonEntityId);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void DrunkenOverboardEvent::startObjectives(const qsf::Entity& targetEntity)
	{
		// Create the need condition
		Objective& needConditionRescueDrowningObjective = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_RESCUEDROWNING);
		needConditionRescueDrowningObjective.setNeededNumber(1); // Hard scripted value

		Objective& needConditionBringPersonToLandObjective = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_BRINGPERSONTOLAND);
		needConditionBringPersonToLandObjective.setNeededNumber(1); // Hard scripted value

		// Create the fail condition
		Objective& failConditionPersonDiedObjective = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_FAIL_DEADPERSONS);
		failConditionPersonDiedObjective.setNeededNumber(1); // Hard scripted value

		// Get message if time is up
		// TODO(db) remove hardcoded max running time
		mMaxRunningTime = qsf::Time::fromSeconds(90.0f);

		qsf::MessageConfiguration message(Messages::EM5_EVENT_TIMER_SIGNAL, getId());
		mTimerMessageProxy.registerAt(message, boost::bind(&DrunkenOverboardEvent::onTimeIsUp, this, _1));
		EM5_GAMETIMERS.addTimer(message, mMaxRunningTime);
	}

	void DrunkenOverboardEvent::startHintObservers()
	{
		const HintHelper hintHelper(*this);
		hintHelper.createAndInitializeGeneralHintsAtInjured();

		// TODO(db) remove hardcoded value
		qsf::Time hint01WaitTime = mMaxRunningTime * 0.3f;

		// Hint 01    Person underwater for X sec.
		createGeneralObserver<HintMessageGeneralObserver>("EM5_EVENT_OVERBOARD_HINT_01").initialize(
			qsf::MessageConfiguration(Messages::EM5_ACTION_FIND_ENTITY_FROM_DIVINGSPOT, mTargetPersonEntityId), hint01WaitTime, false);

		// TODO(db) remove hardcoded value
		float hint02DamageEnergy = 15.0f;

		// Hint 02    Person has only 15% lifeenergy
		createObserver<HintLowEnergyObserver>(mTargetPersonEntityId, "EM5_EVENT_OVERBOARD_HINT_02").initialize(hint02DamageEnergy, HintLowEnergyObserver::HINTENERGYTYPE_LIFE);
	}

	void DrunkenOverboardEvent::onDivingSpotCreated(const qsf::MessageParameters& parameters)
	{
		uint64 divingSpotId = qsf::getUninitialized<uint64>();
		parameters.getParameter("DivingSpotId", divingSpotId);

		qsf::Entity* divingSpotEntity = getMap().getEntityById(divingSpotId);
		if (nullptr == divingSpotEntity)
			return;

		DivingSpotComponent* divingSpotComponent = divingSpotEntity->getComponent<DivingSpotComponent>();
		if (nullptr == divingSpotComponent)
			return;

		if (divingSpotComponent->getAlternativeTargetId() == mTargetPersonEntityId)
		{
			mDivingSpotEntityId = divingSpotEntity->getId();
			mIsPersonUnderwater = true;

			qsf::MessageConfiguration message(Messages::EM5_ACTION_FIND_ENTITY_FROM_DIVINGSPOT, divingSpotId, mTargetPersonEntityId);
			mMessageProxy.registerAt(message, boost::bind(&DrunkenOverboardEvent::findEntityInDivingSpot, this, _1));
		}
	}

	void DrunkenOverboardEvent::findEntityInDivingSpot(const qsf::MessageParameters& parameters)
	{
		mIsPersonUnderwater = false;

		// Update objectives
		ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_RESCUEDROWNING).increaseCurrentNumber();

		qsf::Entity* targetPersonEntity = getMap().getEntityById(mTargetPersonEntityId);
		if (nullptr != targetPersonEntity)
		{
			// Injure the person
			targetPersonEntity->getComponent<HealthComponent>()->injurePersonByEventById(injury::WATER_FILLED_LUNG, this);
		}

		qsf::Entity* divingSpotEntity = getMap().getEntityById(mDivingSpotEntityId);
		if (nullptr != divingSpotEntity)
		{
			// Create DisappearAction for DivingSpot
			qsf::ActionComponent& actionComponent = divingSpotEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
			actionComponent.pushAction<DisappearAction>().init(qsf::Time::fromSeconds(0.0f), true);
		}
	}

	void DrunkenOverboardEvent::onTimeIsUp(const qsf::MessageParameters& parameters)
	{
		if (mIsPersonUnderwater)
		{
			// Update objectives, if the person is still under water the event is failed
			ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_FAIL_DEADPERSONS).increaseCurrentNumber();

			qsf::Entity* divingSpotEntity = getMap().getEntityById(mDivingSpotEntityId);
			if (nullptr != divingSpotEntity)
			{
				// Create DisappearAction for DivingSpot
				qsf::ActionComponent& actionComponent = divingSpotEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
				actionComponent.pushAction<DisappearAction>().init(qsf::Time::fromSeconds(0.0f), true);
			}

			qsf::Entity* targetPersonEntity = getMap().getEntityById(mTargetPersonEntityId);
			if (nullptr != targetPersonEntity)
			{
				// Create DisappearAction for TargetPerson
				qsf::ActionComponent& actionComponent = targetPersonEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
				actionComponent.pushAction<DisappearAction>().init(qsf::Time::fromSeconds(0.0f), true);
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
