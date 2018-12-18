// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/gangster/RunningAmokEvent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/freeplay/objective/ObjectiveHelper.h"
#include "em5/logic/local/gangsters/GangsterCivilArmedLogic.h"
#include "em5/logic/ObserverHelper.h"
#include "em5/logic/observer/PersonDiedObserver.h"
#include "em5/logic/observer/hint/HintPoliceLeftVehicleObserver.h"
#include "em5/health/HealthComponent.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"

#include <qsf/audio/component/DynamicMusicCompositorComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/math/Random.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 RunningAmokEvent::FREEPLAY_EVENT_ID = qsf::StringHash("em5::RunningAmokEvent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	RunningAmokEvent::RunningAmokEvent() :
		mGangsterId(qsf::getUninitialized<uint64>()),
		mAttacksLeft(0),
		mPoliceExited(false)
	{
		// Nothing here
	}

	RunningAmokEvent::~RunningAmokEvent()
	{
		// Nothing here
	}

	uint64 RunningAmokEvent::getGangsterId() const
	{
		return mGangsterId;
	}

	void RunningAmokEvent::setGangster(const qsf::Entity& entity)
	{
		mGangsterId = entity.getId();
	}

	void RunningAmokEvent::setPeopleToAttack(uint32 peopleToAttack)
	{
		mAttacksLeft = peopleToAttack;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	bool RunningAmokEvent::onStartup()
	{
		qsf::Entity* gangsterEntity = getMap().getEntityById(mGangsterId);
		QSF_CHECK(gangsterEntity, "Can't find gangster entity; RunningAmokEvent method: onStartup", return false);

		qsf::ActionComponent& actionComponent = gangsterEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
		actionComponent.clearPlan();

		EntityHelper(*gangsterEntity).turnIntoGangster(qsf::StringHash("em5::GangsterKillerPsychoLogic"), false);
		EventIdComponent::addToEvent(*gangsterEntity, *this, eventspreadreason::GANGSTER);

		startObjectives(*gangsterEntity);

		showSupervisorMessage("EM5_EVENT_RUNNING_AMOK_HINT_01");

		// Event is now running
		setRunning();

		// Done
		return true;
	}

	void RunningAmokEvent::onShutdown()
	{
		// Nothing here
	}

	void RunningAmokEvent::onRun()
	{
		// Activate the hint observers
		startHintObservers();
	}

	bool RunningAmokEvent::onFailure(EventResult& eventResult)
	{
		Objective* failConditionPersonEscapedObjective = eventResult.mObjectives.getObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_FAIL_PERSONSESCAPE);
		if (nullptr != failConditionPersonEscapedObjective && failConditionPersonEscapedObjective->checkAccomplished())
		{
			showHint("EM5_EVENT_GANGSTER_ESCAPING_FAIL_HINT_02", false);
		}
		else
		{
			const int i = qsf::Random::getRandomUint(0, 1);
			showHint((i == 0) ? "EM5_EVENT_TOO_MANY_PERSONS_DIED_HINT_01" : "EM5_EVENT_TOO_MANY_PERSONS_DIED_HINT_02", false);
		}

		// Call base implementation
		return FreeplayEvent::onFailure(eventResult);
	}

	void RunningAmokEvent::updateFreeplayEvent(const qsf::Time& timePassed)
	{
		// Check objectives for success or failure
		checkObjectivesState();
	}

	void RunningAmokEvent::hintCallback(Observer& hintObserver)
	{
		if (!mPoliceExited && hintObserver.getTypeId() == HintPoliceLeftVehicleObserver::GAMELOGIC_TYPE_ID)
		{
			showHint("EM5_EVENT_RUNNING_AMOK_HINT_01", false);
			mPoliceExited = true;

			// Music
			getDynamicMusicCompositor().setMusicLevel(3);
		}
	}

	const qsf::Entity* RunningAmokEvent::getFocusEntity()
	{
		qsf::Entity* gangster = getMap().getEntityById(mGangsterId);
		if (nullptr != gangster)
		{
			// High chance to show the gangster
			EntityHelper entityHelper(*gangster);
			if (!entityHelper.isArrested() && !entityHelper.isPersonInjured() && qsf::Random::getRandomChance(0.5f))
			{
				return gangster;
			}
		}

		// Show random event entity
		return getRandomEventEntity();
	}

	bool RunningAmokEvent::checkEventConfiguration()
	{
		return (nullptr != getMap().getEntityById(mGangsterId));
	}

	void RunningAmokEvent::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base implementation
		FreeplayEvent::serialize(serializer);

		serializer.serialize(mGangsterId);
	}

	bool RunningAmokEvent::addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason)
	{
		if (eventSpreadReason == eventspreadreason::INJURY)
		{
			qsf::Entity* gangsterEntity = getMap().getEntityById(mGangsterId);
			if (&targetEntity != gangsterEntity)
			{
				--mAttacksLeft;
				if (mAttacksLeft == 0)
				{
					// If number of injured persons is reached, escape gangster
					escape();
				}
			}
		}

		return FreeplayEvent::addEntityToEvent(targetEntity, eventSpreadReason, newReason);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void RunningAmokEvent::startObjectives(const qsf::Entity& targetEntity)
	{
		// Create the fail condition
		Objective& failConditionPersonDiedObjective = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_FAIL_DEADPERSONS);
		failConditionPersonDiedObjective.setNeededNumber(2); // Hard scripted value
	}

	void RunningAmokEvent::startHintObservers()
	{
		ObserverHelper(*this).createObserver<HintPoliceLeftVehicleObserver>(mGangsterId).initialize();
	}

	bool RunningAmokEvent::checkIfIsHealthy(const qsf::Entity& personEntity) const
	{
		return personEntity.getComponent<HealthComponent>()->isHealthy();
	}

	void RunningAmokEvent::escape()
	{
		qsf::Entity* gangsterEntity = getMap().getEntityById(mGangsterId);
		QSF_CHECK(nullptr != gangsterEntity, "No Gangster Entity found!", return);
		PersonComponent* personComponent = gangsterEntity->getOrCreateComponent<PersonComponent>();
		personComponent->setGangsterFlag(true);
		personComponent->changeGangsterLogic(qsf::StringHash("em5::GangsterCivilArmedLogic"));

		qsf::ActionComponent& actionComponent = gangsterEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
		actionComponent.clearPlan();

		qsf::GameLogicComponent* gameLogicComponent = gangsterEntity->getComponent<qsf::GameLogicComponent>();
		gameLogicComponent->getGameLogic<GangsterCivilArmedLogic>()->escape(GangsterBaseLogic::ESCAPE_FOOT_LONG);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
