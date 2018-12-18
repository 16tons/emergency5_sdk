// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/gangster/CarTheftEvent.h"
#include "em5/base/ContainerCategory.h"
#include "em5/component/movement/MoveToDespawnComponent.h"
#include "em5/freeplay/objective/ObjectiveHelper.h"
#include "em5/logic/local/gangsters/GangsterBaseLogic.h"
#include "em5/logic/ObserverHelper.h"
#include "em5/logic/observer/ForcePersonFromVehicleObserver.h"
#include "em5/logic/observer/TransportedToHqObserver.h"
#include "em5/logic/HintHelper.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/map/EntityHelper.h"
#include "em5/ai/MoverType.h"

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/audio/component/DynamicMusicCompositorComponent.h>
#include <qsf/math/Random.h>
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
	const uint32 CarTheftEvent::FREEPLAY_EVENT_ID = qsf::StringHash("em5::CarTheftEvent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	CarTheftEvent::CarTheftEvent() :
		mRescue(false)
	{
		// Nothing here
	}

	CarTheftEvent::~CarTheftEvent()
	{
		// Nothing here
	}

	uint64 CarTheftEvent::getGangsterId() const
	{
		return (mGangster.valid() ? mGangster->getId() : qsf::getUninitialized<uint64>());
	}

	void CarTheftEvent::setGangster(qsf::Entity& entity)
	{
		mGangster = entity;
	}

	uint64 CarTheftEvent::getCarId() const
	{
		return (mCar.valid() ? mCar->getId() : qsf::getUninitialized<uint64>());
	}

	void CarTheftEvent::setCar(qsf::Entity& entity)
	{
		mCar = entity;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	bool CarTheftEvent::onStartup()
	{
		QSF_CHECK(mGangster.valid(), "CarTheftEvent::onStartup(): Can't find gangster entity, abort event.", return false);
		QSF_CHECK(mCar.valid(), "CarTheftEvent::onStartup():Can't find vehicle entity for stealing, abort event", return false);

		// Let the thief spawn into the car and directly start fleeing with it
		setupScene();
		setRunning();

		// Done
		return true;
	}

	void CarTheftEvent::onShutdown()
	{
		// Nothing here
	}

	void CarTheftEvent::onRun()
	{
		// Activate the hint observers
		startHintObservers();
	}

	bool CarTheftEvent::onFailure(EventResult& eventResult)
	{
		const int i = qsf::Random::getRandomUint(0, 1);
		Objective* failConditionPersonEscapedObjective = eventResult.mObjectives.getObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_FAIL_PERSONSESCAPE);
		if (nullptr != failConditionPersonEscapedObjective && failConditionPersonEscapedObjective->checkAccomplished())
		{
			showHint("EM5_EVENT_GANGSTER_ESCAPING_FAIL_HINT_02", false);
		}
		else
		{
			showHint((i == 0) ? "EM5_EVENT_TOO_MANY_PERSONS_DIED_HINT_03" : "EM5_EVENT_TOO_MANY_PERSONS_DIED_HINT_04", false);
		}

		// Call base implementation
		return FreeplayEvent::onFailure(eventResult);
	}

	void CarTheftEvent::updateFreeplayEvent(const qsf::Time& timePassed)
	{
		// Check objectives for success or failure
		checkObjectivesState();
		if (getState() != State::STATE_RUNNING && getState() != State::STATE_HIDDEN)
			return;

		if (!mGangster.valid())
			return;	// Happens if the gangster is arrested or injured and transported to hq, valid case, no future update

		if (EntityHelper(*mGangster).isArrested())
			return;

		if (!mRescue && EntityHelper(mGangster.getSafe()).isPersonInjured())
		{
			ObjectiveHelper(*this).removeObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_ARRESTPERSONS);
			mRescue = true;
		}
	}

	void CarTheftEvent::hintCallback(Observer& hintObserver)
	{
		showHint(hintObserver.getName(), false);
	}

	const qsf::Entity* CarTheftEvent::getFocusEntity()
	{
		if (mGangster.valid())
		{
			// Gangster exists. Check whether gangster is already arrested
			GangsterBaseLogic* gangsterLogic = mGangster->getOrCreateComponent<qsf::GameLogicComponent>()->getGameLogic<GangsterBaseLogic>();
			if (nullptr != gangsterLogic)
			{
				if (GangsterBaseLogic::STATE_ARRESTED != gangsterLogic->getCurrentState())
				{
					// Gangster not (yet) arrested

					// Check whether she/he is fleeing in a car
					qsf::Entity* vehicle = EntityHelper(mGangster.getSafe()).getContainerEntity();

					if (nullptr != vehicle)
					{
						// Car exists, return it
						return vehicle;
					}

					// No car exists, return the gangster
					return mGangster.get();
				}
			}
			else
			{
				// Gangster entity exists, but there is no gangster logic, i.e. she/he did not yet commit a crime
				return mGangster.get();
			}
		}

		// No gangster exists anymore. Return car, might be nullptr
		return mCar.get();
	}

	bool CarTheftEvent::checkEventConfiguration()
	{
		return (mGangster.valid() && mCar.valid());
	}

	void CarTheftEvent::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base implementation
		FreeplayEvent::serialize(serializer);

		// TODO(mk) serialize this
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void CarTheftEvent::startHintObservers()
	{
		if (mGangster.valid())
		{
			HintHelper(*this).createAndInitializeHintsAtInjured(mGangster.getSafe());
		}
	}

	void CarTheftEvent::setupScene()
	{
		// Let the gangster enter the vehicle
		EntityHelper gangsterEntityHelper(mGangster.getSafe());
		showSupervisorMessage("EM5_EVENT_SUPERV_OBJ_CARTHEFT_01_01");
		gangsterEntityHelper.turnIntoGangster(qsf::StringHash("em5::GangsterCarHijackerLogic"), true);
		gangsterEntityHelper.enterContainer(mCar.getSafe(), container::CONTAINERTYPE_PASSENGER);	// Enter the vehicle after the gangster logic is set. so we have valid gangster car icon

		qsf::ai::NavigationComponent& navigationComponent = mGangster->getOrCreateComponentSafe<qsf::ai::NavigationComponent>();
		qsf::ai::NavigationComponent& carNavigationComponent = mCar->getOrCreateComponentSafe<qsf::ai::NavigationComponent>();

		// Change the car to behave like a player vehicle
		carNavigationComponent.clearGoal();
		carNavigationComponent.setMoverType(MoverType::GANGSTER_VEHICLE);
		carNavigationComponent.setMinWaitTimeAfterHalt(qsf::Time::ZERO);
		carNavigationComponent.setMaxWaitTimeAfterHalt(qsf::Time::ZERO);
		carNavigationComponent.setNativeSecondaryNavigationMapId(1);
		carNavigationComponent.setNativeCollisionAvoidanceMeasures(7);
		carNavigationComponent.setNativeMayMoveBackwards(true);
		carNavigationComponent.setNativeMayManeuver(true);
		carNavigationComponent.setLimitDirectionChange(false);
		carNavigationComponent.setNativeDistanceToKeepAhead(2.0f);
		carNavigationComponent.setCollisionMask(37);
		carNavigationComponent.setUsesPrioritySearch(true);
		carNavigationComponent.setNativeMoveToClosestInCaseOfFailedSearch(true);

		// Make sure the navigation component of the car is active
		carNavigationComponent.setActive(true);
		if (!navigationComponent.makeDriverOf(carNavigationComponent))
		{
			// If this fails, something went completely wrong
			abortEvent();
			return;
		}

		showHint("EM5_EVENT_GANGSTER_ESCAPING_HINT_01", false);

		// Remove the MoveToDespawnComponent from the car entity
		mCar->destroyComponent<MoveToDespawnComponent>();

		escape(mGangster.getSafe());

		// Set all observers and objectives for the target
		EventIdComponent::addToEvent(mGangster.getSafe(), *this, eventspreadreason::GANGSTER);
		EventIdComponent::addToEvent(mCar.getSafe(), *this, eventspreadreason::EVENT_SPECIFIC);

		Objective& stopGangster = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_STOPCARTHIEF);
		stopGangster.setNeededNumber(1);

		ForcePersonFromVehicleObserver& observerStopped = ObserverHelper(*this).createObserver<ForcePersonFromVehicleObserver>(mGangster->getId());
		observerStopped.connectToObjective(stopGangster);

		Objective& retrieveVehicleObjective = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_RETRIEVEVEHICLE);
		retrieveVehicleObjective.setNeededNumber(1);

		TransportedToHqObserver& observerTransported = ObserverHelper(*this).createObserver<TransportedToHqObserver>(mCar->getId());
		observerTransported.connectToObjective(retrieveVehicleObjective);
	}

	void CarTheftEvent::escape(qsf::Entity& gangster)
	{
		qsf::ActionComponent& actionComponent = gangster.getOrCreateComponentSafe<qsf::ActionComponent>();
		actionComponent.clearPlan();

		qsf::GameLogicComponent* gameLogicComponent = gangster.getComponent<qsf::GameLogicComponent>();
		if (nullptr != gameLogicComponent)
		{
			GangsterBaseLogic* gameLogic = gameLogicComponent->getGameLogic<GangsterBaseLogic>();
			if (nullptr != gameLogic)
			{
				gameLogic->escape(GangsterBaseLogic::ESCAPE_CAR_LONG);
			}
		}

		// Music
		getDynamicMusicCompositor().setMusicLevel(3);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
