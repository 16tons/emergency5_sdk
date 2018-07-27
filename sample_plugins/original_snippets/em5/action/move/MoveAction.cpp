// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/ai/MovementModes.h"
#include "em5/ai/NavigationMaps.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/component/vehicle/BoatComponent.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/logic/HintHelper.h"
#include "em5/game/component/GameComponent.h"
#include "em5/game/minimap/MinimapHelper.h"
#include "em5/game/units/UnitHelper.h"
#include "em5/game/Game.h"
#include "em5/map/CollisionHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/EM5Helper.h"

#include <qsf_game/base/DistanceHelper.h>

#include <qsf_ai/navigation/NavigationComponent.h>
#include <qsf_ai/navigation/em4Router/RouterComponent.h>
#include <qsf_ai/navigation/em4Router/wrapper/actor/EGenericObject.h>
#include <qsf_ai/navigation/goal/ReachObjectGoal.h>
#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>
#include <qsf_ai/navigation/pathfinding/PathfindingSystem.h>
#include <qsf_ai/worldModel/WorldModel.h>
#include <qsf_ai/worldModel/WorldModelManager.h>

#include <qsf/component/placement/PlacementComponent.h>
#include <qsf/component/polygon/GroundTypePolygonComponent.h>
#include <qsf/debug/request/TextDebugDrawRequest.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/action/WaitAction.h>
#include <qsf/map/Map.h>
#include <qsf/math/Random.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/renderer/mesh/MeshComponent.h> // For changing the bounding box of moving persons
#include <qsf/renderer/decal/trace/TraceDecalComponent.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Local helper functions                                ]
	//[-------------------------------------------------------]
	namespace
	{
		bool canEntityMoveOnWater(qsf::Entity& entity)
		{
			qsf::ai::RouterComponent* routerComponent = entity.getComponent<qsf::ai::RouterComponent>();
			if (nullptr != routerComponent && nullptr != routerComponent->getActor())
			{
				if (routerComponent->getActor()->GetType() & qsf::ai::EATF_GENERICOBJECT)
				{
					const qsf::ai::EObjectTerrainClass eotc = static_cast<qsf::ai::EGenericObject*>(routerComponent->getActor())->GetTerrainClass();
					return (eotc == qsf::ai::EOTC_BOAT || eotc == qsf::ai::EOTC_SHIP || eotc == qsf::ai::EOTC_DIVER || eotc == qsf::ai::EOTC_AIRPLANE);
				}
			}
			return false;
		}
	}


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier MoveAction::ACTION_ID = "em5::MoveAction";
	const float MoveAction::MIN_DISTANCE_FOR_RETRY = 50.0f;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	MoveAction::MoveAction() :
		qsf::ai::MoveAction(ACTION_ID),
		mIsPerson(false),
		mIsCivilPerson(false),
		mIsCivilRoadVehicle(false),
		mIsSquadVehicle(false),
		mDidStartMoveCheckForBluelightAndSiren(false),
		mStuckCheckMode(StuckCheckMode::NONE),
		mStuckTime(qsf::Time::ZERO),
		mPreviousTracePointPosition(0.0f),
		mIsOnTraceGround(false),
		mMaxMoveTime(qsf::Time::ZERO),
		mTimePassed(qsf::Time::ZERO),
		mForcedWaitSquaredDistance(0.0f),
		mForcedWaitTime(qsf::Time::ZERO),
		mNumberOfRetries(0)
	{
		// Nothing here
	}

	MoveAction::~MoveAction()
	{
		// Nothing here
	}

	void MoveAction::init(qsf::ai::NavigationGoal* goal, uint32 movementModeId, qsf::ai::Path* path, qsf::Time maxMoveTime)
	{
		// Find the right movement mode and give it to the AI
		movementModeId = getMovementMode(movementModeId);

		mMaxMoveTime = maxMoveTime;

		// Squads can retry a move action once
		if (EntityHelper(getEntity()).isCommandable())
		{
			mNumberOfRetries = 1;
		}

		// Call base class implementation
		qsf::ai::MoveAction::init(goal, movementModeId, path);
	}

	float MoveAction::getPathLength() const
	{
		float squaredDistance = 0.0f;

		// Check if we have a navigation path
		const qsf::WeakPtr<qsf::ai::NavigationComponent>& navigationComponent = getNavigationComponent();
		if (navigationComponent.valid() && navigationComponent->hasPath())
		{
			// We have a path calculate the squared distance to the target
			const std::deque<qsf::ai::Waypoint>& waypoints = navigationComponent->getPath().getNodes();
			if (!waypoints.empty())
			{
				glm::vec3 lastPosition = waypoints.front().mPosition;
				for (const qsf::ai::Waypoint& waypoint : waypoints)
				{
					squaredDistance += glm::distance2(lastPosition, waypoint.mPosition);
					lastPosition = waypoint.mPosition;
				}
			}
		}

		return (0.0f != squaredDistance) ? std::sqrt(squaredDistance) : 0.0f;
	}

	uint64 MoveAction::getFollowedEntity() const
	{
		const qsf::WeakPtr<qsf::ai::NavigationComponent>& navigationComponent = getNavigationComponent();
		if (!navigationComponent.valid())
			return qsf::getUninitialized<uint64>();

		qsf::ai::NavigationGoal& goal = navigationComponent->getGoal();

		if (!goal.isTargetDynamic())
			return qsf::getUninitialized<uint64>();

		qsf::ai::ReachObjectGoal* reachObjectGoal = dynamic_cast<qsf::ai::ReachObjectGoal*>(&goal);
		if (nullptr == reachObjectGoal)
			return qsf::getUninitialized<uint64>();

		return reachObjectGoal->getTargetEntityId();
	}

	void MoveAction::forceFollowedEntityToWait(float distance, qsf::Time waitTime)
	{
		mForcedWaitSquaredDistance = distance * distance;
		mForcedWaitTime = waitTime;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Action methods                    ]
	//[-------------------------------------------------------]
	void MoveAction::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base implementation
		qsf::ai::MoveAction::serialize(serializer);

		serializer.serialize(mIsCivilPerson);
		serializer.serialize(mIsCivilRoadVehicle);
		serializer.serialize(mStuckTime);
		serializer.serialize(mOldPosition);
		serializer.serialize(mPreviousTracePosition);
		serializer.serialize(mPreviousTracePointPosition);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	qsf::action::Result MoveAction::updateAction(const qsf::Clock& clock)
	{
		if (mMaxMoveTime > qsf::Time::ZERO)
		{
			mTimePassed += clock.getTimePassed();
			if (mTimePassed > mMaxMoveTime)
			{
				return qsf::action::RESULT_DONE;
			}
		}

		if (!checkAllowUpdate())
		{
			return qsf::action::RESULT_CONTINUE;
		}

		const glm::vec3& newPosition = EntityHelper(getEntity()).getPosition();

		// Call base implementation
		qsf::action::Result result = qsf::ai::MoveAction::updateAction(clock);

		if (qsf::action::RESULT_CONTINUE == result)
		{
			checkForSirenActivationOnStartMoving();
		}

		// In EM5, a failed AI movement should not lead to automatic clearing of the action plan
		if (qsf::action::RESULT_CLEARLIST == result)
		{
			result = qsf::action::RESULT_DONE;

			// TODO(mk) Somehow we reach this function for every finished helicopter movement (disable message for them)
			// Provide the player with some usability features regarding squad vehicles
			if (mIsSquadVehicle && !EntityHelper(getEntity()).isHelicopter())
			{
				CommandableComponent* commandableComponent = getEntity().getComponent<CommandableComponent>();
				if (nullptr != commandableComponent)
				{
					// Minimum distance to goal required
					std::auto_ptr<qsf::ai::NavigationGoal>& goal = getGoal();
					if (goal.get() != nullptr && goal->estimateDistanceToTarget(newPosition) > 10.0f)
					{
						// Provide the player with a hint, added time to avoid spamming the hint
						HintHelper::showHint("EM5_AI_PATH_FAIL", commandableComponent->getPlayerId(), qsf::Time::fromSeconds(15.0f));

						// Show on the minimap where this unit is located at, but please don't play a sound because it can get pretty fast pretty annoying
						MinimapHelper::registerPingByEntityId(getEntityId(), false, false);
					}
				}
			}

			// Let's try again
			if (mNumberOfRetries > 0)
			{
				std::auto_ptr<qsf::ai::NavigationGoal>& goal = getGoal();
				if (goal.get() != nullptr)
				{
					// We still have time to move
					const bool maxMoveTimeLeft = mMaxMoveTime == qsf::Time::ZERO || mTimePassed < mMaxMoveTime;

					// We need a reseted copy
					if (goal->estimateDistanceToTarget(newPosition) > MIN_DISTANCE_FOR_RETRY && maxMoveTimeLeft)
					{
						qsf::Time newMaxMoveTime = (mMaxMoveTime == qsf::Time::ZERO ? qsf::Time::ZERO : mMaxMoveTime - mTimePassed);

						MoveAction& moveAction = getComponent().pushAction<MoveAction>(getPriority(), qsf::action::INSERT_BEFORE_CURRENT);
						moveAction.init(goal->clone().release(), getMovementModeId(), nullptr, newMaxMoveTime);
						moveAction.mNumberOfRetries = mNumberOfRetries - 1;

						// TODO(tl): We can remove this warning later if it seems annoying
						QSF_WARN_ONCE("We are restating this move action of entity " << getEntity().getId() << " once as it seems we are stuck far from our target", QSF_REACT_NONE)
					}
				}
			}
		}

		const bool isOnTraceGround = (qsf::GroundTypePolygonComponent::getGroundTypeOfPosition(getMap(), newPosition) != 0);

		if (isOnTraceGround != mIsOnTraceGround)
		{
			if (isOnTraceGround)
			{
				mPreviousTracePosition = newPosition;
				addTraceNode(newPosition, ADD_NEW_NODE);
			}
			else
			{
				addTraceNode(newPosition, FINISH_OFF_TRACE);
			}

			mIsOnTraceGround = isOnTraceGround;
		}
		else if (mIsOnTraceGround)
		{
			if (glm::distance2(mPreviousTracePosition, newPosition) > 1.0f * 1.0f)
			{
				addTraceNode(newPosition, ADD_NEW_NODE);
				mPreviousTracePosition = newPosition;
			}
			else
			{
				addTraceNode(newPosition, UPDATE_EXISTING_NODE);
			}
		}


		// Check if entity got stuck
		if (checkIfStuck(clock.getTimePassed()))
		{
			// Just for debugging
			#if 0
				qsf::DebugDrawProxy ddp;
				ddp.registerAt(QSF_DEBUGDRAW);
				ddp.addRequest(qsf::TextDebugDrawRequest("STUCK", EntityHelper(getEntity()).getPosition()));
				ddp.setLifetimeInSeconds(10.0f);
				ddp.detachRequests();
			#endif

			result = qsf::action::RESULT_DONE;
		}
		mOldPosition = newPosition;

		// Force wait
		if (mForcedWaitSquaredDistance > 0.0f && mForcedWaitTime > qsf::Time::ZERO)
		{
			qsf::Entity* followedEntity = getEntity().getMap().getEntityById(getFollowedEntity());
			if (nullptr != followedEntity)
			{
				const float distanceToTarget = qsf::game::DistanceHelper::getSquaredDistance(newPosition, *followedEntity);
				if (distanceToTarget < mForcedWaitSquaredDistance)
				{
					qsf::ActionComponent* actionComponent = followedEntity->getComponent<qsf::ActionComponent>();
					if (nullptr != actionComponent)
					{
						qsf::Action* currentAction = actionComponent->getCurrentAction();
						if (nullptr != currentAction && currentAction->getTypeId() == MoveAction::ACTION_ID)
						{
							static_cast<MoveAction*>(currentAction)->pause();
							actionComponent->pushAction<qsf::WaitAction>(action::BLOCKING, qsf::action::INSERT_BEFORE_CURRENT).init(mForcedWaitTime);

							// Lets recalculate our path to a immobile target now
							getEntity().getOrCreateComponentSafe<qsf::ai::NavigationComponent>().setDynamicTargetChanged(true);
						}
					}

					// Disable forced wait, we already tried to push the wait action once
					mForcedWaitSquaredDistance = 0.0f;
				}
			}
		}

		return result;
	}

	bool MoveAction::onStartup()
	{
		// Call base class implementation
		if (!qsf::ai::MoveAction::onStartup())
			return false;

		qsf::Entity& entity = getEntity();

		// Init intern flags
		const EntityHelper entityHelper(entity);
		mIsPerson = (nullptr != entity.getComponent<PersonComponent>());
		mIsCivilPerson = mIsPerson && entityHelper.isCivilPerson();
		mIsCivilRoadVehicle = entityHelper.isCivilRoadVehicle();	// TODO(tl): Implement civil boats later
		mIsSquadVehicle = entityHelper.isSquadVehicle();

		// Determine stuck check mode, if any at all
		if (entityHelper.isSquadPerson() || mIsSquadVehicle || entityHelper.isGangsterPerson())
		{
			// Exclude boats and helicopters
			if (!entityHelper.isBoat() && !entityHelper.isShip() && !entityHelper.isHelicopter())
			{
				mStuckCheckMode = StuckCheckMode::SQUADS;
			}
		}
		else if (mIsCivilRoadVehicle)
		{
			const GameComponent* gameComponent = getMap().getCoreEntity().getComponent<GameComponent>();
			if (nullptr == gameComponent || gameComponent->getCivilVehiclesStuckCheck())
			{
				mStuckCheckMode = StuckCheckMode::CIVIL_VEHICLES;
			}
		}

		mOldPosition = entityHelper.getPosition();
		mTimePassed = qsf::Time::ZERO;

		// Switch blue lights on if applicable otherwise the light is off
		if (mIsSquadVehicle)
		{
			RoadVehicleComponent* roadVehicleComponent = entity.getComponent<RoadVehicleComponent>();
			if (nullptr != roadVehicleComponent)
			{
				roadVehicleComponent->setBlueLightState(isOnEmergencyOperations());
				roadVehicleComponent->setIsOnEmergencyOperations(isOnEmergencyOperations());
				roadVehicleComponent->closeAllDoors();
			}
		}

		// TODO(mk) I assume to reach the goal of perfect persons, we keep this line here
		if (mIsCivilPerson && nullptr == entity.getComponent<qsf::PlacementComponent>())
		{
			entity.getOrCreateComponentById("em5::PersonPlacementComponent");
		}

		if (mIsPerson)
		{
			// Extends bounding box of the person
			qsf::MeshComponent* meshComponent = entity.getComponent<qsf::MeshComponent>();
			if (nullptr != meshComponent)
			{
				meshComponent->setOgreEntityBoundingBox(glm::vec3(1.0f, 1.0f, 8.0f));
			}
		}

		if (mIsCivilPerson)
		{
			// Check if we want to move on street network
			qsf::ai::PathfindingSystem* pathfinder = qsf::ai::PathfindingSystem::tryGetInstance();
			QSF_CHECK(pathfinder != nullptr, "No pathfinding system found", QSF_REACT_THROW);

			const qsf::ai::NavigationComponent& navigationComponent = getEntity().getOrCreateComponentSafe<qsf::ai::NavigationComponent>();
			const uint32 navigationMap = navigationComponent.getPrimaryNavigationMapId();

			if (navigationMap == em5::NavigationMap::STREET_NETWORK)
			{
				// Check if we are on a pedestrian path or not
				qsf::ai::WorldModelManager* worldModelManager = qsf::ai::WorldModelManager::tryGetInstance();
				QSF_CHECK(worldModelManager != nullptr, "No world model manager found", QSF_REACT_THROW);

				const std::unique_ptr<qsf::ai::ManagedNavigationMapReadAccess> readAccess = worldModelManager->acquireReadAccess(navigationMap);
				const qsf::ai::WorldModel& laneWorld = readAccess.get()->get();

				const qsf::Transform& transform = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>().getTransform();
				const qsf::logic::TargetPoint requestedPoint(transform.getPosition(), false, 0.0f, 0.0f);

				std::vector<qsf::ai::MapContactPoint> contactPoints;
				laneWorld.writeClosestLegalPoints(requestedPoint, navigationComponent.getMoverType(), contactPoints);

				// Get closest point to our path we want to use
				float closestDistance = std::numeric_limits<float>::max();
				glm::vec3 closestPosition;
				for (const qsf::ai::MapContactPoint& mapContactPoints : contactPoints)
				{
					if (mapContactPoints.mDistanceToLegalSpace < closestDistance)
					{
						closestDistance = mapContactPoints.mDistanceToLegalSpace;
						closestPosition = mapContactPoints.mClosestPosition;
					}
				}

				// We use 1 meter tolerance where we don't calculate an extra path
				if (closestDistance > 1.0f && closestDistance != std::numeric_limits<float>::max())
				{
					// TODO(fw): This looks like a terrible hack; a move action should not push another move action (with possibly different settings!)

					// We will push another moveAction on our free area to reach the path
					qsf::logic::TargetPoint targetPoint;
					targetPoint.mPosition = closestPosition;
					targetPoint.mTolerance = 0.5f;

					bool run = false;
					if (qsf::isInitialized(getMovementModeId()))
					{
						const auto& speedFactorOptional = QSF_MAKE_REF(qsf::ai::PathfindingSystem::tryGetInstance()).getMovementMode(getMovementModeId()).mSpeedFactor;
						run = speedFactorOptional && (*speedFactorOptional > 1.0f);
					}
					const uint32 newMovementMode = run ? MovementModes::MOVEMENT_MODE_FREE_RUN : MovementModes::MOVEMENT_MODE_FREE_WALK;
					getComponent().pushAction<MoveAction>(getPriority(), qsf::action::INSERT_AT_FRONT).init(new qsf::ai::ReachSinglePointGoal(targetPoint), newMovementMode);
				}
			}
		}
		return true;
	}

	void MoveAction::onShutdown()
	{
		// Call base implementation
		qsf::ai::MoveAction::onShutdown();

		// Switch siren if applicable and reset on emergency operation
		if (mIsSquadVehicle)
		{
			qsf::Entity& entity = getEntity();

			// TODO(co) Review this: Can we move the following two component checks into an else-branch of "UnitHelper(entity).isFireFighterDrone()"?

			RoadVehicleComponent* roadVehicleComponent = entity.getComponent<RoadVehicleComponent>();
			if (nullptr != roadVehicleComponent)
			{
				// We have reached the target position -> Stop siren audio playback
				roadVehicleComponent->stopSirenLoop();
			}

			BoatComponent* boatComponent = entity.getComponent<BoatComponent>();
			if (nullptr != boatComponent)
			{
				// We have reached the target position -> Stop siren audio playback
				boatComponent->stopSirenLoop();
			}

			if (UnitHelper(entity).isFireFighterDrone())
			{
				// To avoid flooding the message pipeline with unnecessary messages. Send this message only for fire drone (currently needed only for the "fire drone unit")
				QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_FINISH_MOVE_ACTION, getEntityId()));
			}
		}
		else if (mIsPerson)
		{
			// Reset bounding box of the person to the default
			qsf::MeshComponent* meshComponent = getEntity().getComponent<qsf::MeshComponent>();
			if (nullptr != meshComponent)
			{
				meshComponent->resetCustomBoundingBox();
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	bool MoveAction::isOnEmergencyOperations() const
	{
		return (getMovementModeId() == MovementModes::MOVEMENT_MODE_PLAYER_VEHICLE_ON_EMERGENCY_OPERATIONS);
	}

	uint32 MoveAction::getMovementMode(uint32 movementModeId) const
	{
		// If argument is invalid, check for possible movement mode
		if (qsf::isUninitialized(movementModeId))
		{
			EntityHelper entityHelper(getEntity());
			if (entityHelper.isSquadPerson())
			{
				// TODO(mk) I build this with paramedic in head, for other carry persons, maybe firefighters or police/gangster, we need probability other checks
				if (entityHelper.isCarryingAnotherPerson())
				{
					return MovementModes::MOVEMENT_MODE_CARRY_PERSON;
				}
			}
		}
		return movementModeId;
	}

	bool MoveAction::checkAllowUpdate() const
	{
		if (mGameComponent.valid())
		{
			if (mIsCivilPerson)
			{
				return mGameComponent->getCivilPersonMovement();
			}
			else if (mIsCivilRoadVehicle)
			{
				return mGameComponent->getCivilVehiclesDrive();
			}
		}
		return true;
	}

	bool MoveAction::checkPositionStuck()
	{
		const glm::vec3& newPosition = EntityHelper(getEntity()).getPosition();
		return (qsf::getDistanceSquared(mOldPosition, newPosition) < 0.01f * 0.01f);
	}

	bool MoveAction::checkIfStuck(const qsf::Time& timePassed)
	{
		// TODO(tl): Shouldn't this be part of the stuck resolving system ?
		// (fw) Actually yes, but it's a bit tricky to get this EM5-specific stuff in there

		// No stuck check at all?
		if (mStuckCheckMode == StuckCheckMode::NONE)
		{
			// Early out
			return false;
		}

		if (mStuckCheckMode == StuckCheckMode::CIVIL_VEHICLES)
		{
			// Check if vehicle is stuck
			if (checkPositionStuck())
			{
				// Update time
				mStuckTime += timePassed;

				// Delete vehicles from game (but wait till we can't see it)
				if (mStuckTime > qsf::Time::fromSeconds(60.0f))
				{
					EntityHelper entityHelper(getEntity());
					if (nullptr != entityHelper.getFreeplayEvent())
					{
						// If entity is part of any event, we may not destroy it at all
						mStuckCheckMode = StuckCheckMode::NONE;
						return false;
					}

					if (!entityHelper.isEntityVisibleForAnyPlayer())
					{
						MapHelper::destroyEntity(getEntity());
						return true;
					}
				}
			}
			else
			{
				// Reset stuck time again
				mStuckTime = qsf::Time::ZERO;
			}
		}
		else  // For squads
		{
			if (checkPositionStuck())
			{
				// Update time
				mStuckTime += timePassed;

				if (mStuckTime > qsf::Time::fromSeconds(20.0f))
				{
					qsf::TransformComponent& transformComponent = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();
					CollisionHelper collisionHelper(getMap());

					// Stuck inside a collision or on water?
					bool isInInvalidPlace = false;
					{
						if (collisionHelper.hasCollisionEM3(getEntity(), transformComponent.getPosition(), 0.0f))
						{
							isInInvalidPlace = true;
						}
						else if (collisionHelper.isInWater(getEntity()) && !canEntityMoveOnWater(getEntity()))
						{
							isInInvalidPlace = true;
						}
					}

					if (isInInvalidPlace)
					{
						// Reset to the next lane
						qsf::Transform transform;
						if (MapHelper(getEntity().getMap()).getPointOnClosestLane(transformComponent.getPosition(), 0, mIsPerson ? 2 : 0, transform))
						{
							// In case position there is not free, try several times
							for (int tries = 0; tries < 5; ++tries)
							{
								// Check for collision
								if (!collisionHelper.hasCollisionEM3(getEntity(), transform.getPosition(), 0.0f))
									break;

								// Move position a bit randomly around
								const glm::vec3 randomOffset(qsf::Random::getRandomFloat(-5.0f, 5.0f), 0.0f, qsf::Random::getRandomFloat(-5.0f, 5.0f));
								transform.setPosition(transform.getPosition() + randomOffset);
								glm::vec3 newPosition;
								collisionHelper.findFreePositionEM3(getEntity(), transform, 5.0f, 0.0f, newPosition);
								transform.setPosition(newPosition);
							}

							transformComponent.setPositionAndRotation(transform.getPosition(), transform.getRotation());

							// Give it another try for another move action
							mNumberOfRetries = 1;
							return false;
						}
					}

					// Reset stuck time again
					mStuckTime = qsf::Time::ZERO;
				}
			}
			else
			{
				// Reset stuck time again
				mStuckTime = qsf::Time::ZERO;
			}
		}

		return false;
	}

	void MoveAction::checkForSirenActivationOnStartMoving()
	{
		if (!mDidStartMoveCheckForBluelightAndSiren && mIsSquadVehicle && isOnEmergencyOperations())
		{
			const float distance = getPathLength();
			if (distance > 0.0f)
			{
				// Check if the threshold is exceeded (any way >= 50 meters -> siren should be active)
				// TODO(sw) make this value configureable?
				const bool activateSiren = (distance >= 50.0f);

				// TODO(fw): This is just some refactored code -- it could be the better choice to just ask for the "VehicleComponent" directly
				VehicleComponent* vehicleComponent = getEntity().getComponent<RoadVehicleComponent>();
				if (nullptr == vehicleComponent)
					vehicleComponent = getEntity().getComponent<BoatComponent>();

				// Activate siren when requirements satisfied
				if (nullptr != vehicleComponent)
				{
					if (activateSiren)
						vehicleComponent->playSirenLoop();
					else
						vehicleComponent->stopSiren();
				}

				mDidStartMoveCheckForBluelightAndSiren = true;
			}
		}
	}

	void MoveAction::addTraceNode(const glm::vec3& position, TraceNodeAddMode addMode)
	{
		qsf::TraceDecalComponent* traceDecalComponent = nullptr;

		if (!mCurrentTraceEntity.valid())
		{
			mCurrentTraceEntity = getMap().createEntity();
			traceDecalComponent = mCurrentTraceEntity->createComponent<qsf::TraceDecalComponent>();
			traceDecalComponent->setTextureAtlas(qsf::AssetProxy(180120ULL));
		}
		else
		{
			traceDecalComponent = &mCurrentTraceEntity->getComponentSafe<qsf::TraceDecalComponent>();
		}

		if (addMode != FINISH_OFF_TRACE)
		{
			glm::vec3 prevNodePosition;

			if (traceDecalComponent->getNumberOfNodes() > 0)
			{
				prevNodePosition = traceDecalComponent->getNodes()[traceDecalComponent->getNumberOfNodes()-1].getPosition();
			}
			else
			{
				prevNodePosition = position;
			}

			const float distance = glm::distance(position, prevNodePosition);

			qsf::Node nodeValue(position, 10.0f);
			nodeValue.setScale(glm::vec3(mPreviousTracePointPosition + distance));

			if (addMode == ADD_NEW_NODE)
			{
				// We're abusing radius as the TTL here!
				traceDecalComponent->insertNode(traceDecalComponent->getNumberOfNodes(), nodeValue);
				mPreviousTracePointPosition = mPreviousTracePointPosition + distance;
			}
			else if (addMode == UPDATE_EXISTING_NODE)
			{
				traceDecalComponent->setNode(traceDecalComponent->getNumberOfNodes()-1, nodeValue);
			}
		}
		else
		{
			// Instruct the decal to delete itself when all nodes have been faded off
			traceDecalComponent->detachAndDeleteWhenNodesEmpty();

			// The trace entity is detached, it should delete itself when everything faded out
			mCurrentTraceEntity = nullptr;
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
