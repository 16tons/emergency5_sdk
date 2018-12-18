// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/police/RedirectTrafficAction.h"
#include "em5/action/base/DisappearAction.h"
#include "em5/action/move/GettingRedirectedAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/ai/NavigationMaps.h"
#include "em5/ai/MoverType.h"
#include "em5/game/collision/CollisionTypes.h"
#include "em5/command/police/RedirectTrafficCommand.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/map/CollisionHelper.h"
#include "em5/map/EntityHelper.h"

#include <qsf_ai/navigation/NavigationComponent.h>
#include <qsf_ai/worldModel/trafficLanes/ClosestTrafficLanesHelper.h>

#include <qsf_game/base/DistanceHelper.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/move/MovableComponent.h>
#ifndef ENDUSER
	#include <qsf/debug/request/CircleDebugDrawRequest.h>
#endif
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/math/Math.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier RedirectTrafficAction::ACTION_ID = "em5::RedirectTrafficAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	RedirectTrafficAction::RedirectTrafficAction() :
		Action(ACTION_ID),
		mState(STATE_INIT),
		mLastRedirected(qsf::getUninitialized<uint64>()),
		mHasCheckedRedirectionPath(false),
		mMinimalRedirectionPathLength(0.0f)
		#ifndef ENDUSER
			, mDebugEnabled(false)
		#endif
	{
		// Nothing here
	}

	RedirectTrafficAction::~RedirectTrafficAction()
	{
		EntityHelper(getEntity()).setFlareActive(false);
	}

	void RedirectTrafficAction::init()
	{
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void RedirectTrafficAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serializeAs<uint8>(mState);
		serializer.serialize(mUnitTarget.mPosition);
		serializer.serialize(mUnitTarget.mDirection);
		serializer.serialize(mLastRedirected);
		serializer.serialize(mHasCheckedRedirectionPath);
		serializer.serialize(mMinimalRedirectionPathLength);
		serializer.serialize(mLastRedirectionTarget.mPosition);
		serializer.serialize(mLastRedirectionTarget.mDirection);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool RedirectTrafficAction::onStartup()
	{
		// We do not care about our original target, but we should at least be close to some street now.
		if (RedirectTrafficCommand::getTargetStreet(getEntity(), mUnitTarget))
		{
			float MAX_DIST_TO_STREET = 1.5f;	// TODO(mz): should be a spec
			qsf::TransformComponent& transformComponent = getEntity().getComponentSafe<qsf::TransformComponent>();
			if (qsf::game::DistanceHelper::get2dDistance(transformComponent.getPosition(), mUnitTarget.mPosition) < MAX_DIST_TO_STREET)
			{
				return true;
			}
		}

		return false;
	}

	qsf::action::Result RedirectTrafficAction::updateAction(const qsf::Clock&)
	{
		switch (mState)
		{
			case STATE_INIT:
			{
				EntityHelper(getEntity()).setFlareActive(true);

				glm::quat rotation = qsf::Math::getLookAtQuaternion(-mUnitTarget.mDirection, qsf::CoordinateSystem::getUp());
				getComponent().pushAction<TurnToAction>(getPriority(), qsf::action::INSERT_AT_FRONT).initWithRotation(rotation);

				mState = STATE_TURNED;

				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_TURNED:
			{
				AnimationHelper animationHelper(getEntity());
				animationHelper.playAnimation(animationHelper.getAnimationPoliceRegulateTrafficLoop(), true, false, qsf::Time::fromSeconds(0.3f));

				mState = STATE_REDIRECT;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_REDIRECT:
			{
				// Find a vehicle approaching the position where the police unit should stand
				const float SEARCH_RADIUS = 2.f;	// TODO(mz): could be a spec
				const float SEARCH_DISTANCE = 1.5f;	// Stop before reaching police or cars can't turn. TODO(mz): could be a spec
				glm::vec3 target(mUnitTarget.mPosition);
				glm::vec3 direction(mUnitTarget.mDirection.x, 0.f, mUnitTarget.mDirection.z);
				target -= glm::normalize(direction)*(SEARCH_RADIUS + SEARCH_DISTANCE);

				#ifndef ENDUSER
					// Debug output
					if (mDebugEnabled)
					{
						mDebugDrawProxy.registerAt(QSF_DEBUGDRAW);
						mDebugDrawProxy.addRequest(qsf::CircleDebugDrawRequest(target, glm::vec3(0.0f, 1.0f, 0.0f), SEARCH_RADIUS, qsf::Color4::WHITE, true));
					}
				#endif

				// Find vehicle(s) in front of the unit.
				qsf::ComponentMapQuery componentMapQuery(getMap());
				std::vector<RoadVehicleComponent*> vehiclesInRange;
				componentMapQuery.getInstancesInCircle<RoadVehicleComponent>(target, SEARCH_RADIUS, vehiclesInRange);

				for (RoadVehicleComponent* vehicleComponent : vehiclesInRange)
				{
					// Use only intact vehicles
					if (!vehicleComponent->isIntact() || vehicleComponent->isDestroyed())
						continue;

					qsf::Entity& vehicleEntity = vehicleComponent->getEntity();

					// Should have a movable component
					qsf::MovableComponent* movableComponent = vehicleEntity.getComponent<qsf::MovableComponent>();
					if (movableComponent == nullptr)
						continue;

					// Is an already redirected vehicle still in front of us?
					if (vehicleEntity.getId() == mLastRedirected)
					{
						// Check if the navigationpath seems sane or really bad.
						if (!mHasCheckedRedirectionPath)
						{
							qsf::ActionComponent& actionComponent = vehicleEntity.getComponentSafe<qsf::ActionComponent>();
							qsf::Action* vehicleCurrentAction = actionComponent.getCurrentAction();
							if (nullptr != vehicleCurrentAction && vehicleCurrentAction->getTypeId() == MoveAction::ACTION_ID)
							{
								// Note that we can only check the path after it has started (as it's not calculated before that).
								// So we might need a few tries here.
								float distance = static_cast<MoveAction*>(vehicleCurrentAction)->getPathLength();
								if (distance > 0.f)
								{
									mHasCheckedRedirectionPath = true;
									const float MAX_REDIRECTION_FACTOR = 2.f;	// TODO(mz): could be a spec
									// Does this look like a bad path (too long)?
									if (mMinimalRedirectionPathLength * MAX_REDIRECTION_FACTOR < distance)
									{
										actionComponent.clearPlan();

										qsf::Transform transform;
										transform.setPosition(mLastRedirectionTarget.mPosition);
										transform.setRotation(qsf::Math::getLookAtQuaternion(mLastRedirectionTarget.mDirection, qsf::CoordinateSystem::getUp()));
										glm::vec3 outPosition;
										if (CollisionHelper(getMap()).findFreePositionEM3(vehicleEntity, transform, 1.0f, 0.1f, outPosition))
										{
											movableComponent->warpToPositionAndRotation(transform.getPosition(), transform.getRotation());
										}
										else
										{
											// Kill it
											actionComponent.pushAction<DisappearAction>(getPriority()).init(qsf::Time::fromSeconds(1.f));
										}
									}
								}
							}
						}

						return qsf::action::RESULT_CONTINUE;
					}

					// No squads, no gangsters
					EntityHelper vehicleEntityHelper(vehicleEntity);
					if (vehicleEntityHelper.isCommandable() || vehicleEntityHelper.isGangsterRoadVehicle())
						continue;

					// Should have a navigation component
					const qsf::ai::NavigationComponent* navigationComponent = vehicleEntity.getComponent<qsf::ai::NavigationComponent>();
					if (navigationComponent == nullptr || !navigationComponent->isActive())
						continue;

					// Find a target which should be a lane going in the opposite direction
					// We start looking from the position where the vehicle is now (and not the police unit) so there's a chance it can reach it without driving over the police unit.
					qsf::TransformComponent& vehicleTransformComponent = vehicleEntity.getOrCreateComponentSafe<qsf::TransformComponent>();
					qsf::ai::SpatialConfiguration3D vehiclePosition = mUnitTarget;
					vehiclePosition.mPosition = vehicleTransformComponent.getPosition();
					std::vector<qsf::ai::SpatialConfiguration3D> redirectionTargets;
					findClosestConfigurationsOnOppositeDirectionIdealLanes(vehiclePosition, NavigationMap::STREET_NETWORK, MoverType::CIVIL_VEHICLE, redirectionTargets);
					if (redirectionTargets.empty())
					{
						// If we get no target to redirect vehicles here then we're on the wrong position and should give up.
						return qsf::action::RESULT_DONE;
					}

					mLastRedirectionTarget = redirectionTargets[0];

					#ifndef ENDUSER
					// Debug output
						if (mDebugEnabled)
						{
							mDebugDrawProxy.registerAt(QSF_DEBUGDRAW);
							mDebugDrawProxy.addRequest(qsf::CircleDebugDrawRequest(mLastRedirectionTarget.mPosition, glm::vec3(0, 1, 0), 1.f, qsf::Color4::RED, true));
						}
					#endif

					qsf::ActionComponent& actionComponent = vehicleEntity.getComponentSafe<qsf::ActionComponent>();
					actionComponent.clearPlan();
					actionComponent.pushAction<GettingRedirectedAction>(getPriority()).init(mLastRedirectionTarget);
					mLastRedirected = vehicleEntity.getId();
					mHasCheckedRedirectionPath = false;
					mMinimalRedirectionPathLength = qsf::game::DistanceHelper::getDistance(vehiclePosition.mPosition, mLastRedirectionTarget.mPosition);
				}

				return qsf::action::RESULT_CONTINUE;
			}
		}

		return qsf::action::RESULT_DONE;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
