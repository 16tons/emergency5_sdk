// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/gangster/GrandTheftAutoEvent.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/base/EnterVehicleAction.h"
#include "em5/action/base/ExitVehicleAction.h"
#include "em5/action/gangster/ReachVictimAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/move/FullBrakeAction.h"
#include "em5/ai/MoverType.h"
#include "em5/base/GameplayAssets.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/component/spawnpoint/UnitSpawnPointComponent.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include "em5/component/movement/MoveToDespawnComponent.h"
#include "em5/freeplay/objective/ObjectiveHelper.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/game/targetpoint/EnterDriverDoorTargetPointProvider.h"
#include "em5/game/targetpoint/GotoObjectTargetPointProvider.h"
#include "em5/logic/local/gangsters/GangsterCarHijackerLogic.h"
#include "em5/logic/ObserverHelper.h"
#include "em5/logic/observer/ForcePersonFromVehicleObserver.h"
#include "em5/logic/observer/TransportedToHqObserver.h"
#include "em5/logic/HintHelper.h"
#include "em5/health/HealthComponent.h"
#include "em5/base/ContainerCategory.h"
#include "em5/map/CollisionHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"

#include <qsf_ai/navigation/steering/stuckResolving/StuckResolverComponent.h>
#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/audio/component/DynamicMusicCompositorComponent.h>
#include <qsf/math/Random.h>
#include <qsf/link/link/prototype/ContainerLink.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/action/WaitAction.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
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
	const uint32 GrandTheftAutoEvent::FREEPLAY_EVENT_ID = qsf::StringHash("em5::GrandTheftAutoEvent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GrandTheftAutoEvent::GrandTheftAutoEvent() :
		mVictimId(qsf::getUninitialized<uint64>()),
		mState(PERPETRATING)
	{
		// Nothing here
	}

	GrandTheftAutoEvent::~GrandTheftAutoEvent()
	{
		// Nothing here
	}

	uint64 GrandTheftAutoEvent::getCarId() const
	{
		return (mCar.valid() ? mCar->getId() : qsf::getUninitialized<uint64>());
	}

	void GrandTheftAutoEvent::setCar(qsf::Entity& entity)
	{
		mCar = entity;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	bool GrandTheftAutoEvent::onStartup()
	{
		QSF_CHECK(mCar.valid(), "GrandTheftAutoEvent::onStartup(): Can't find car entity, abort event", return false);

		RoadVehicleComponent& roadVehicleComponent = mCar->getOrCreateComponentSafe<RoadVehicleComponent>();
		qsf::ai::NavigationComponent& carNavigationComponent = mCar->getOrCreateComponentSafe<qsf::ai::NavigationComponent>();
		qsf::ActionComponent& carActionComponent = mCar->getOrCreateComponentSafe<qsf::ActionComponent>();
		qsf::TransformComponent& carTransformComponent = mCar->getOrCreateComponentSafe<qsf::TransformComponent>();

		std::vector<qsf::Transform> driverDoors;
		roadVehicleComponent.getDoorPositions(DoorComponent::DOORTYPE_DRIVER, driverDoors, carTransformComponent.getTransform());
		QSF_CHECK(!driverDoors.empty(), "GrandTheftAutoEvent::onStartup(): Car entity has no doors, abort event", return false);

		mGangster = MapHelper(QSF_MAINMAP).createGangsterFromPoolAndSpawnItInGangsterEventArea(GangsterPool::DEFAULT);

		QSF_CHECK(mGangster.valid(), "GrandTheftAutoEvent::onStartup(): could not create a gangster, abort event", return false);

		qsf::TransformComponent& gangsterTransformComponent = mGangster->getOrCreateComponentSafe<qsf::TransformComponent>();
		qsf::ActionComponent& gangsterActionComponent = mGangster->getOrCreateComponentSafe<qsf::ActionComponent>();

		gangsterTransformComponent.setPosition(driverDoors.begin()->getPosition());

		// Register both entities to the event
		EventIdComponent::addToEvent(mGangster.getSafe(), *this, eventspreadreason::EVENT_SPECIFIC);
		EventIdComponent::addToEvent(mCar.getSafe(), *this, eventspreadreason::EVENT_SPECIFIC);

		carNavigationComponent.clearGoal();
		carNavigationComponent.clearMovementMode();
		carNavigationComponent.setNativeMaxForwardSpeed(3.0f);

		qsf::ai::StuckResolverComponent& stuckResolverComponent = mCar->getOrCreateComponentSafe<qsf::ai::StuckResolverComponent>();
		stuckResolverComponent.setActive(false);

		MoveToDespawnComponent& moveToDespawnComponent = mCar->getOrCreateComponentSafe<MoveToDespawnComponent>();
		moveToDespawnComponent.setActive(false);

		// Stop the car
		carActionComponent.clearPlan();
		carActionComponent.pushAction<qsf::WaitAction>(action::COMMAND_HIGH).init(qsf::Time::MAX);

		// Enter gangster into the car
		gangsterActionComponent.clearPlan();
		gangsterActionComponent.pushAction<EnterVehicleAction>(action::COMMAND_HIGH).init(*mCar, DoorComponent::DOORTYPE_DRIVER);

		// Done
		return true;
	}

	void GrandTheftAutoEvent::onShutdown()
	{
		// Nothing here
	}

	void GrandTheftAutoEvent::onRun()
	{
		// Activate the hint observers
		startHintObservers();
	}

	bool GrandTheftAutoEvent::onFailure(EventResult& eventResult)
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

	void GrandTheftAutoEvent::updateFreeplayEvent(const qsf::Time& timePassed)
	{
		// Check objectives for success or failure
		checkObjectivesState();
		if (getState() != State::STATE_RUNNING && getState() != State::STATE_HIDDEN)
			return;

		if (!mGangster.valid())
			return;	// Happens if the gangster is arrested or injured and transported to hq, valid case, no future update

		if (EntityHelper(mGangster.getSafe()).isArrested())
			return;

		switch (mState)
		{
			case PERPETRATING:
			{
				qsf::ActionComponent& actionComponent = mGangster->getOrCreateComponentSafe<qsf::ActionComponent>();
				if (nullptr == actionComponent.getAction<EnterVehicleAction>())
				{
					setRunning();

					// First setup it as gangster
					setupGangster(mGangster.getSafe(), mCar.getSafe());

					// Lets check that our gangster is inside the car
					if (!EntityHelper(mGangster).isInContainer(*mCar))
					{
						EntityHelper(mGangster).enterContainer(*mCar, container::CONTAINERTYPE_PASSENGER);
					}
					else
					{
						// When enter vehicle action was successfull make sure the gangster in vehicle icon is visible.
						// EntityHelper::enterContainer (used by the enter vehicle action) would do this for us. But when the action executes the person isn't an gangster and thus doesn't add the icpn
						// We change the person to an gangster when the person has entered the vehicle.
						StatusOverlayComponent::showIcon(*mCar, StatusOverlayComponent::STATUS_ICON_GANGSTER_IN_VEHICLE, true);
					}

					qsf::ActionComponent& carActionComponent = mCar->getOrCreateComponentSafe<qsf::ActionComponent>();
					qsf::WaitAction* waitAction = carActionComponent.getAction<qsf::WaitAction>();
					if (nullptr != waitAction)
					{
						carActionComponent.removeAction(*waitAction);
					}

					mState = ESCAPING;
				}
				break;
			}

			case ESCAPING:
			{
				if (EntityHelper(mGangster.getSafe()).isPersonInjured())
				{
					ObjectiveHelper(*this).removeObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_ARRESTPERSONS);
					mState = RESCUE;
				}
				break;
			}

			case RESCUE:
				break;
		}
	}

	void GrandTheftAutoEvent::hintCallback(Observer& hintObserver)
	{
		showHint(hintObserver.getName(), false);
	}

	const qsf::Entity* GrandTheftAutoEvent::getFocusEntity()
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

		// No gangster exists anymore, check for injured victim to return
		qsf::Entity* victim = getMap().getEntityById(mVictimId);
		if (nullptr != victim)
		{
			// Victim exists
			if (EntityHelper(*victim).isPersonInjured())
			{
				// Victim still injured
				return victim;
			}
		}

		// No (treatable) victim either. Check for the car
		qsf::Entity* car = mCar.get();

		// Return car, might be nullptr
		return car;
	}

	bool GrandTheftAutoEvent::checkEventConfiguration()
	{
		return (mCar.valid());
	}

	void GrandTheftAutoEvent::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base implementation
		FreeplayEvent::serialize(serializer);

		// TODO(mk) Serialize this class

		serializer.serialize(mVictimId);
		serializer.serializeAs<uint32>(mState);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void GrandTheftAutoEvent::startHintObservers()
	{
		if (mGangster.valid())
		{
			HintHelper(*this).createAndInitializeHintsAtInjured(mGangster.getSafe());
		}
	}

	bool GrandTheftAutoEvent::checkIfReachedTarget(qsf::Entity& gangster)
	{
		qsf::ActionComponent& actionComponent = gangster.getOrCreateComponentSafe<qsf::ActionComponent>();
		return actionComponent.getCurrentAction()->getTypeId() != MoveAction::ACTION_ID;
	}

	void GrandTheftAutoEvent::setupGangster(qsf::Entity& gangster, qsf::Entity& car)
	{
		showSupervisorMessage("EM5_EVENT_SUPERV_OBJ_CARTHEFT_02_01");
		EntityHelper(gangster).turnIntoGangster(qsf::StringHash("em5::GangsterCarHijackerLogic"), true);

		// Enter the vehicle
		qsf::ai::NavigationComponent& navigationComponent = gangster.getOrCreateComponentSafe<qsf::ai::NavigationComponent>();
		qsf::ai::NavigationComponent& carNavigationComponent = car.getOrCreateComponentSafe<qsf::ai::NavigationComponent>();

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

		escape(gangster);

		// Set all observers and objectives for the target
		EventIdComponent::addToEvent(gangster, *this, eventspreadreason::GANGSTER);
		Objective& stopGangster = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_STOPCARTHIEF);
		stopGangster.setNeededNumber(1);

		ForcePersonFromVehicleObserver& observerStopped = ObserverHelper(*this).createObserver<ForcePersonFromVehicleObserver>(gangster.getId());
		observerStopped.connectToObjective(stopGangster);

		Objective& retrieveVehicle = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_RETRIEVEVEHICLE);
		retrieveVehicle.setNeededNumber(1);

		TransportedToHqObserver& observerTransported = ObserverHelper(*this).createObserver<TransportedToHqObserver>(car.getId());
		observerTransported.connectToObjective(retrieveVehicle);

		RoadVehicleComponent& roadVehicleComponent = car.getOrCreateComponentSafe<RoadVehicleComponent>();
		qsf::Entity* victimEntity = nullptr;
		for (auto &p : roadVehicleComponent.getLinks())
		{
			qsf::ContainerLink* containerLink = p.second;
			qsf::Entity& target = containerLink->getTargetEntity();
			EntityHelper entityHelper(target);

			if (!entityHelper.isValidGangsterEventTarget(gangster, true))
				continue;

			target.getOrCreateComponent<qsf::ActionComponent>()->clearPlan();
			target.getComponent<qsf::ActionComponent>()->pushAction<ExitVehicleAction>();
			victimEntity = &target;
			break;
		}

		if (nullptr == victimEntity)
		{
			// No victim candidate found, create one!
			victimEntity = SpawnPointComponent::spawnRandomAt(qsf::Transform(), getMap(), assets::SPAWNPOOL_DEFAULT_PERSON, false);

			if (nullptr != victimEntity)
			{
				qsf::TransformComponent& vehicleTransformComponent = car.getOrCreateComponentSafe<qsf::TransformComponent>();
				qsf::TransformComponent& transformComponent = victimEntity->getOrCreateComponentSafe<qsf::TransformComponent>();

				std::vector<qsf::Transform> driverDoors;
				roadVehicleComponent.getDoorPositions(DoorComponent::DOORTYPE_DRIVER, driverDoors, vehicleTransformComponent.getTransform());

				// Position and rotation, take the vehicle rotation as fallback
				glm::vec3 startPosition = car.getOrCreateComponent<qsf::TransformComponent>()->getPosition();
				glm::quat startRotation = car.getOrCreateComponent<qsf::TransformComponent>()->getRotation();
				if (!driverDoors.empty())
				{
					startPosition = driverDoors.begin()->getPosition();
					startRotation = driverDoors.begin()->getRotation();
				}

				transformComponent.setRotation(startRotation);

				CollisionHelper(victimEntity->getMap()).findFreePositionEM3(*victimEntity, startPosition, 10, 0.5, startPosition);

				transformComponent.setPosition(startPosition);


				qsf::MovableComponent* movableComponent = victimEntity->getComponent<qsf::MovableComponent>();
				if (movableComponent != nullptr)
					movableComponent->warpToPositionAndRotation(startPosition, startRotation);

				qsf::ActionComponent& actionComponent = victimEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
				actionComponent.clearPlan();
			}
		}

		if (nullptr == victimEntity)
		{
			// Error
			QSF_ERROR("GrandTheftAutoEvent::setupGangster() Couldn't create victim entity, abort event", QSF_REACT_NONE);
			abortEvent();
			return;
		}

		// TODO(mk) Only one hard coded injury? sounds a little bit less.
		HealthComponent* healthComponent = victimEntity->getComponent<HealthComponent>();
		healthComponent->injurePersonByEventById(injury::BROKEN_LEG, this);
		mVictimId = victimEntity->getId();
	}

	void GrandTheftAutoEvent::escape(qsf::Entity& gangster)
	{
		qsf::ActionComponent& actionComponent = gangster.getOrCreateComponentSafe<qsf::ActionComponent>();
		actionComponent.clearPlan();

		qsf::GameLogicComponent* gameLogicComponent = gangster.getComponent<qsf::GameLogicComponent>();
		if (nullptr != gameLogicComponent)
		{
			GangsterBaseLogic* gameLogic = gameLogicComponent->getGameLogic<GangsterCarHijackerLogic>();
			if (nullptr != gameLogic)
			{
				gameLogic->escape(GangsterBaseLogic::ESCAPE_CAR_LONG);
			}
		}

		// Music
		getDynamicMusicCompositor().setMusicLevel(3);
	}

	qsf::Entity* GrandTheftAutoEvent::findCar(qsf::Entity& gangster, qsf::Entity& trafficLight)
	{
		std::vector<RoadVehicleComponent*> roadVehicleComponents;
		qsf::ComponentMapQuery(trafficLight.getMap()).getInstancesInCircle(trafficLight.getOrCreateComponent<qsf::TransformComponent>()->getPosition(), 20.f, roadVehicleComponents);
		for (RoadVehicleComponent* roadVehicleComponents : roadVehicleComponents)
		{
			qsf::Entity& vehicle = roadVehicleComponents->getEntity();
			qsf::ai::NavigationComponent* navComponent = vehicle.getComponent<qsf::ai::NavigationComponent>();
			if (nullptr == navComponent)	// No parking vehicle
				continue;

			if (!EntityHelper(vehicle).isCivilRoadVehicle())
				continue;

			return &vehicle;
		}
		return nullptr;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
