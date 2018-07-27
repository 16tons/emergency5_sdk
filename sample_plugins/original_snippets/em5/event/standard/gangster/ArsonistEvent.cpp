// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/gangster/ArsonistEvent.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/ai/MovementModes.h"
#include "em5/base/ContainerCategory.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/fire/component/ComplexFireComponent.h"
#include "em5/freeplay/objective/ObjectiveHelper.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/game/targetpoint/PullPersonTargetPointProvider.h"
#include "em5/health/HealthComponent.h"
#include "em5/logic/local/gangsters/GangsterCivilUnarmedLogic.h"
#include "em5/logic/observer/PersonDiedObserver.h"
#include "em5/logic/observer/hint/HintShotAtObserver.h"
#include "em5/map/CollisionHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"

#include <qsf_game/component/event/EventTagComponent.h>
#include <qsf_game/base/DistanceHelper.h>

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>
#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>

#include <qsf/audio/component/DynamicMusicCompositorComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/action/WaitAction.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/math/CoordinateSystem.h>
#include <qsf/math/Random.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 ArsonistEvent::FREEPLAY_EVENT_ID = qsf::StringHash("em5::ArsonistEvent");

	const uint32 OBJECTIVE_FAIL_DEAD_GANGSTER = qsf::StringHash("ArsonistDeadGangster");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	ArsonistEvent::ArsonistEvent() :
		mGangsterId(qsf::getUninitialized<uint64>()),
		mCurrentTargetId(qsf::getUninitialized<uint64>()),
		mBuildingsBurnt(0),
		mFirstBuildingMustHaveBalcony(true),
		mPerpetrated(false),
		mRescue(false),
		mDontShootTriggered(false),
		mNextAction(UNDEFINED)
	{
		// Nothing here
	}

	ArsonistEvent::~ArsonistEvent()
	{
		// Nothing here
	}

	uint64 ArsonistEvent::getGangsterId() const
	{
		return mGangsterId;
	}

	void ArsonistEvent::setArsonist(const qsf::Entity& entity)
	{
		mGangsterId = entity.getId();
	}

	void ArsonistEvent::setFirstBuildingMustHaveBalcony(bool firstBuildingMustHaveBalcony)
	{
		mFirstBuildingMustHaveBalcony = firstBuildingMustHaveBalcony;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	bool ArsonistEvent::onStartup()
	{
		qsf::Entity* gangsterEntity = getMap().getEntityById(mGangsterId);
		QSF_CHECK(gangsterEntity, "Can't find gangster entity; ArsonistEvent method: onStartup", QSF_REACT_NONE);
		if (nullptr == gangsterEntity)
			return false;

		// Clear all actions
		qsf::ActionComponent& actionComponent = gangsterEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
		actionComponent.clearPlan();
		EventIdComponent::addToEvent(*gangsterEntity, *this, eventspreadreason::NO_REASON);

		mNextAction = GO_TO_NEXT_BUILDING;

		// Done
		return true;
	}

	void ArsonistEvent::onShutdown()
	{
		// Nothing here
	}

	void ArsonistEvent::onRun()
	{
		// Activate the hint observers
		startHintObservers();
	}

	void ArsonistEvent::updateFreeplayEvent(const qsf::Time& timePassed)
	{
		checkObjectivesState();
		if (getState() != State::STATE_RUNNING && getState() != State::STATE_HIDDEN)
			return;

		qsf::Entity* gangsterEntity = getMap().getEntityById(mGangsterId);

		// Gangster arrested / transported to HQ / ... ?
		if (nullptr == gangsterEntity)
			return;

		// Gangster arrested?
		if (EntityHelper(*gangsterEntity).isArrested())
			return;

		qsf::ActionComponent& actionComponent = gangsterEntity->getOrCreateComponentSafe<qsf::ActionComponent>();

		// Check if gangster still got work to do
		if (!actionComponent.getPlan().isEmpty())
		{
			// Gangster still busy, so we do nothing
			return;
		}

		switch (mNextAction)
		{
			case GO_TO_NEXT_BUILDING:
			{
				// Find a valid building
				BuildingComponent* nextBuildingToSetOnFire = findBuildingCandidate(*gangsterEntity);
				if (nullptr == nextBuildingToSetOnFire)
				{
					// No building to set one fire
					if (mPerpetrated)
					{
						// Already committed a crime, better get the hell out of here
						escape(*gangsterEntity);
					}
					else
					{
						// Nothing this person has done yet
						mNextAction = DO_NOTHING;

						// In case that no buildings exist, the person is not even able to commit a crime.
						// Aborting the event (before showing any objectives etc.) creates the impression the event has not even started yet)
						abortEvent();
					}

					mCurrentTargetId = qsf::getUninitialized<uint64>();
					break;
				}

				qsf::Entity& currentTargetEntity = nextBuildingToSetOnFire->getEntity();
				QSF_CHECK(qsf::isInitialized(currentTargetEntity.getId()), "ArsonistEvent: Invalid target to set on fire!", QSF_REACT_NONE);
				mCurrentTargetId = currentTargetEntity.getId();

				if (mPerpetrated)
				{
					// The gangster already set a building on fire and is invulnerable (concerning fire) right now; make him vulnerable again.
					FireReceiverComponent* fireReceiverComponent = gangsterEntity->getComponent<FireReceiverComponent>();
					QSF_CHECK(fireReceiverComponent, "No FireReceiverComponent attached to gangster entity!", QSF_REACT_NONE);

					if (fireReceiverComponent)
					{
						// In case the arsonist has already set a building on fire, he might be invulnerable to fire. Change that back.
						// See case "SetBuildingOnFire" for more information
						fireReceiverComponent->setActive(true);
					}
				}
				else
				{
					// Place the gangster near to the building. We have trouble with gangsters need ages to reach there goals, place them near the goals.
					glm::vec3 targetPosition = nextBuildingToSetOnFire->getDoorPosition();
					glm::vec3 newGangsterPosition = targetPosition;
					CollisionHelper(gangsterEntity->getMap()).findFreePositionEM3(*gangsterEntity, targetPosition, 10, 0.5, newGangsterPosition);
					gangsterEntity->getOrCreateComponentSafe<qsf::TransformComponent>().setPosition(newGangsterPosition);
				}

				// Push a new movement action to make the person/gangster walk/run to the next target
				MoveAction* moveAction = &actionComponent.pushAction<MoveAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT);
				// TODO(mk) I think here is an wrong target point used
				qsf::ai::ReachObjectGoal* goal = new qsf::ai::ReachObjectGoal(*gangsterEntity, currentTargetEntity, PullPersonTargetPointProvider::TARGET_POINT_ID);

				// Movement speed depends on whether a crime was already committed (run) or not (default speed)
				moveAction->init(goal, mPerpetrated ? MovementModes::MOVEMENT_MODE_RUN : MovementModes::MOVEMENT_MODE_DEFAULT);
				actionComponent.pushAction<TurnToAction>(action::COMMAND_HIGH, qsf::action::APPEND_TO_BACK).init(mCurrentTargetId);

				// Play gangster animation (manipulate object while kneeling down), simulating setting a building on fire
				actionComponent.pushAction<PlayAnimationAction>(action::COMMAND_HIGH, qsf::action::APPEND_TO_BACK).init(AnimationHelper(*gangsterEntity).getAnimationManipulateObjectKneeling(), true);

				// Now that the gangster reached the building and turned to it, she/he will set it on fire
				mNextAction = SET_BUILDING_ON_FIRE;
				break;
			}

			case SET_BUILDING_ON_FIRE:
			{
				// This should not happen at all!
				QSF_CHECK(qsf::isInitialized(mCurrentTargetId), "ArsonistEvent: Invalid target to set on fire!", mNextAction = GO_TO_NEXT_BUILDING; break;);

				FireReceiverComponent* fireReceiverComponent = gangsterEntity->getComponent<FireReceiverComponent>();
				QSF_CHECK(fireReceiverComponent, "No FireReceiverComponent attached to gangster entity!", QSF_REACT_NONE);

				if (fireReceiverComponent)
				{
					// Make arsonist invulnerable for the time of cheering etc.
					fireReceiverComponent->setActive(false);
				}

				qsf::Entity* currentBuildingEntity = getMap().getEntityById(mCurrentTargetId);
				if (nullptr == currentBuildingEntity)
				{
					mNextAction = GO_TO_NEXT_BUILDING;
					break;
				}

				// Set location entity
				if (!isEventLocationEntitySet())
				{
					createEventLocationEntity(*currentBuildingEntity);
				}

				// Set building on fire (ID is valid at this point, so the pointer has to as well)
				EntityHelper(*currentBuildingEntity).startFire(this);

				++mBuildingsBurnt;
				if (mBuildingsBurnt == 3)
				{
					int i = qsf::Random::getRandomUint(0, 1);
					showHint((i == 0) ? "EM5_EVENT_ARSONIST_HINT_01" : "EM5_EVENT_ARSONIST_HINT_02", false);

					// Music
					getDynamicMusicCompositor().setMusicLevel(3);
				}

				PersonComponent* personComponent = gangsterEntity->getComponent<PersonComponent>();

				// Check whether a person component is attached
				QSF_CHECK(nullptr != personComponent, "ArsonistEvent: Gangster does not have a person component!", QSF_REACT_NONE);

				// Check whether the person has not yet committed a crime
				if (!mPerpetrated)
				{
					// The person just set a building on fire... that makes her/him a gangster.
					mPerpetrated = true;
					setRunning();
					EntityHelper(*gangsterEntity).turnIntoGangster(qsf::StringHash("em5::GangsterCivilUnarmedLogic"), false);
					EventIdComponent::addToEvent(*gangsterEntity, *this, eventspreadreason::GANGSTER);

					if (!mFirstBuildingMustHaveBalcony)
					{
						showSupervisorMessage("EM5_EVENT_SUPERV_OBJ_ARSONIST_01_01");
					}
					startObjectives(*gangsterEntity);

					// Event Arsonist02: We need at least one person to be inside the first building and wave out of window/from balcony
					if (mFirstBuildingMustHaveBalcony)
					{
						// Special supervisor message for Arsonist02
						showSupervisorMessage("EM5_EVENT_SUPERV_OBJ_ARSONIST_02_01");

						BuildingComponent* buildingWithBalcony = currentBuildingEntity->getComponent<BuildingComponent>();
						if (nullptr != buildingWithBalcony)
						{
							// TODO(mk) Creting new entity via hardcoded prefab looks strange, why is here no spawnpoint or edited person?
							// TODO(ks) different person models / prefabs?
							qsf::Entity* personThatWavesOutOfWindow = MapHelper(getMap()).createObjectByLocalPrefabAssetId(qsf::StringHash("em5/prefab/person/civil_casual_female01"));

							QSF_CHECK(nullptr != personThatWavesOutOfWindow, "Could not create person from prefab! No one is going to wave out of window!", QSF_REACT_NONE);

							if (nullptr != personThatWavesOutOfWindow)
							{
								EntityHelper(*personThatWavesOutOfWindow).enterContainer(*currentBuildingEntity, container::CONTAINERTYPE_PASSENGER);
								buildingWithBalcony->letPersonWaveOutOfWindow(*personThatWavesOutOfWindow, this);
							}
						}
					}
				}

				// The building started burning, so the next step for the gangster is to step away and watch the work / cheer
				mNextAction = WATCH_AND_CHEER;
				break;
			}

			case WATCH_AND_CHEER:
			{
				// Fire receiver component -> setActive(false)
				gangsterEntity->getComponent<FireReceiverComponent>();

				qsf::TransformComponent* gangsterTransformComponent = gangsterEntity->getOrCreateComponent<qsf::TransformComponent>();
				const glm::vec3 pos = gangsterTransformComponent->getPosition();
				const glm::quat rot = gangsterTransformComponent->getRotation();
				const glm::quat rotationXZ = qsf::Math::getRotationByUpVector(qsf::CoordinateSystem::getUp(), rot);

				qsf::Entity* building = getMap().getEntityById(mCurrentTargetId);

				float radius = 0.0f;

				const ComplexFireComponent* complexFireComponent = building->getComponent<ComplexFireComponent>();
				if (nullptr != complexFireComponent)
				{
					std::vector<FireComponent*> allFireComponents;
					complexFireComponent->getAllFireComponents(allFireComponents);
					for (FireComponent *fireComp : allFireComponents)
					{
						// Find the radius in which fire damage is taken
						float dangerRadius = glm::max(fireComp->getSoftRadius(), fireComp->getHardRadius());
						radius = glm::max(radius, dangerRadius);
					}
				}
				else
				{
					// Only relevant when no complex fire component exists
					const FireComponent* fireComp = building->getComponent<FireComponent>();
					if (nullptr != fireComp)
					{
						radius = glm::max(radius, fireComp->getSoftRadius());
					}
				}

				// Fallback solution in case no reasonable soft/hard-radius exists
				radius = glm::min(radius, 5.0f);

				glm::vec3 cheerVector = radius * (rotationXZ * qsf::CoordinateSystem::getIn());

				// Step back from building to cheer
				glm::vec3 cheerPos = pos - cheerVector;
				qsf::logic::TargetPoint cheerPosition = qsf::logic::TargetPoint(cheerPos, false, 0.0f, 0.0f, rotationXZ);

				// Move away from building
				actionComponent.pushAction<MoveAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(new qsf::ai::ReachSinglePointGoal(cheerPosition), MovementModes::MOVEMENT_MODE_RUN, nullptr);

				// Turn to building
				actionComponent.pushAction<TurnToAction>(action::COMMAND_HIGH, qsf::action::APPEND_TO_BACK).init(mCurrentTargetId);

				// Cheer
				AnimationHelper animationHelper(*gangsterEntity);
				actionComponent.pushAction<PlayAnimationAction>(action::COMMAND_HIGH, qsf::action::APPEND_TO_BACK).init(animationHelper.getAnimationCheer(), false, true, false, qsf::Time::ZERO, mCheeringTime);
				actionComponent.pushAction<qsf::WaitAction>(action::COMMAND_HIGH, qsf::action::APPEND_TO_BACK).init(mCheeringTime);

				// The gangster is done with that building, reset the target
				mCurrentTargetId = qsf::getUninitialized<uint64>();

				// One building is not enough; the gangster shall repeat the whole sequence, i.e. search for another burnable building
				mNextAction = GO_TO_NEXT_BUILDING;
				break;
			}

			case DO_NOTHING:
			{
				break;
			}

			default:
				// Should not happen, but please prove me wrong if so
				QSF_CHECK(false, "ArsonistEvent: Gangster has invalid next action!", QSF_REACT_NONE);
		}

		if (!mRescue && !checkIfIsHealthy(*gangsterEntity) && DO_NOTHING != mNextAction)
		{
			// Gangster not healthy!
			mNextAction = DO_NOTHING;

			ObjectiveHelper(*this).removeObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_ARRESTPERSONS);
			mRescue = true;
		}

		checkObjectivesState();
	}

	void ArsonistEvent::hintCallback(Observer& hintObserver)
	{
		showHint(hintObserver.getName(), false);
	}

	const qsf::Entity* ArsonistEvent::getFocusEntity()
	{
		qsf::Entity* gangster = getMap().getEntityById(mGangsterId);
		if (nullptr != gangster)
		{
			// High chance to show the gangster
			EntityHelper entityHelper(*gangster);
			if (!entityHelper.isArrested() && !entityHelper.isPersonInjured())
			{
				if (qsf::Random::getRandomChance(0.5f))
				{
					return gangster;
				}
			}
		}

		EntityIdArray const& victims = getEntityIds();
		for (int i = 0; i < int(victims.size()); ++i)
		{
			qsf::Entity* victim = getMap().getEntityById(victims.at(i));
			if (nullptr != victim)
			{
				EntityHelper entityHelper(*victim);

				if (entityHelper.isBurning())
				{
					// Probably a burning building or vehicle
					return victim;
				}

				if (entityHelper.isPersonInjured())
				{
					// Injured person
					return victim;
				}

				// Not a burning vehicle/building or an injured person
				continue;
			}
		}

		// Return event location entity
		return getEventLocationEntity();
	}

	bool ArsonistEvent::checkEventConfiguration()
	{
		return (nullptr != getMap().getEntityById(mGangsterId));
	}

	void ArsonistEvent::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base implementation
		FreeplayEvent::serialize(serializer);

		serializer.serialize(mGangsterId);
		serializer.serialize(mCurrentTargetId);
		serializer.serialize(mFirstBuildingMustHaveBalcony);
		serializer.serializeAs<uint32>(mNextAction);
		serializer.serialize(mPerpetrated);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	BuildingComponent* ArsonistEvent::findBuildingCandidate(qsf::Entity& arsonist) const
	{
		// Get a listing of all building components (cannot be empty since findCandidate already checked)
		std::vector<BuildingComponent*> buildingComponents;

		// Pick those buildings that fit our needs
		for (BuildingComponent* buildingComponent : qsf::ComponentMapQuery(getMap()).getAllInstances<BuildingComponent>())
		{
			if (checkBuildingCandidate(buildingComponent->getEntity()))
			{
				buildingComponents.push_back(buildingComponent);	// We found a suitable candidate!
			}
		}

		// No building fits our needs
		if (buildingComponents.empty())
			return nullptr;

		// Find the building that is closest to the arsonist
		const qsf::TransformComponent& selfTransformComponent = arsonist.getComponentSafe<qsf::TransformComponent>();
		qsf::game::IsLeftComponentCloserToTargetPosition compareObject(selfTransformComponent.getPosition());
		std::sort(buildingComponents.begin(), buildingComponents.end(), compareObject);

		return *buildingComponents.begin();
	}

	bool ArsonistEvent::checkBuildingCandidate(qsf::Entity& building) const
	{
		const EntityHelper entityHelper(building);

		// Test if the building fits our requirements

		// Check if building is part of another event
		if (entityHelper.checkForEventIdComponent())
			return false;

		if (!entityHelper.isInEventBoundaries())
			return false;

		// Check if entity is a industrial plant
		if (entityHelper.isEntityIndustrialPlant())
			return false;

		// Check if entity is a normal building
		if (!entityHelper.isEntityBuilding())
			return false;

		// Check if this building is damaged
		if (entityHelper.isBuildingDamaged())
			return false;

		// Check if building can be set on fire
		if (!entityHelper.canBeSetOnFireNow())
			return false;

		// Check is building is forbidden by difficulty tag
		qsf::game::EventTagComponent* eventTagComponent = building.getComponent<qsf::game::EventTagComponent>();

		if (nullptr != eventTagComponent && eventTagComponent->hasTag(qsf::StringHash("EVENT_BURNING_HARD")))
			return false;

		// Only relevant for the first building, in this case mPerpetrated is not set yet as no crime was committed till now
		if (mFirstBuildingMustHaveBalcony && !mPerpetrated)
		{
			// Check if this building has a balcony
			BuildingComponent* buildingComponent = building.getComponent<BuildingComponent>();
			if (!buildingComponent)
			{
				// Something went wrong here, this should not happen as this entity has to have a building component attached
				return false;
			}

			if (qsf::isUninitialized(buildingComponent->getEventWindowId()))
			{
				// Event window ID is not initialized, i.e. there is no window/balcony
				return false;
			}
		}
		else // Empty building needed
		{
			// Check if this building is empty
			if (!entityHelper.isContainerEmpty())
				return false;
		}

		// Accepted
		return true;
	}

	void ArsonistEvent::startObjectives(const qsf::Entity& targetEntity)
	{
		// Create the fail conditions
		Objective& failConditionPersonDiedObjective = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_FAIL_DEADPERSONS);
		failConditionPersonDiedObjective.setNeededNumber(2); // Hard scripted value

		addFailCondition(OBJECTIVE_FAIL_DEAD_GANGSTER, 1, "Dead Arsonist");

		Objective& failConditionGangsterEscapedObjective = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_FAIL_PERSONSESCAPE);
		failConditionGangsterEscapedObjective.setNeededNumber(1); // Hard scripted value
	}

	void ArsonistEvent::startHintObservers()
	{
		if (mDontShootTriggered)
		{
			return;
		}

		mDontShootTriggered = true;

		createObserver<HintShotAtObserver>(mGangsterId, "EM5_EVENT_DONT_SHOOT_HINT_02").initialize();
	}

	void ArsonistEvent::escape(qsf::Entity& gangster) const
	{
		qsf::ActionComponent& actionComponent = gangster.getOrCreateComponentSafe<qsf::ActionComponent>();
		actionComponent.clearPlan();
		qsf::GameLogicComponent* gameLogicComponent = gangster.getComponent<qsf::GameLogicComponent>();
		GangsterBaseLogic* gameLogic = gameLogicComponent->getGameLogic<GangsterCivilUnarmedLogic>();
		gameLogic->escape(GangsterBaseLogic::ESCAPE_FOOT_LONG);

		// Music
		getDynamicMusicCompositor().setMusicLevel(3);
	}

	bool ArsonistEvent::checkIfIsHealthy(const qsf::Entity& personEntity) const
	{
		return !personEntity.getComponent<HealthComponent>()->isInjured();
	}

	void ArsonistEvent::setCheeringTime(const qsf::Time& cheeringTime)
	{
		mCheeringTime = cheeringTime;
	}

	bool ArsonistEvent::addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason)
	{
		if (eventSpreadReason == eventspreadreason::INJURY)
		{
			Objective& failConditionPersonDiedObjective = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_FAIL_DEADPERSONS);
			createObserver<PersonDiedObserver>(targetEntity.getId()).connectToObjective(failConditionPersonDiedObjective);
		}
		return FreeplayEvent::addEntityToEvent(targetEntity, eventSpreadReason, newReason);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5L
