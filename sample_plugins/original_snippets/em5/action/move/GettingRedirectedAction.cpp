// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/move/GettingRedirectedAction.h"
#include "em5/component/movement/MoveToDespawnComponent.h"
#include "em5/component/movement/RandomMovementComponent.h"
#include "em5/component/effect/FadeEffectComponent.h"
#include "em5/map/MapHelper.h"
#include "em5/map/CollisionHelper.h"

#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/move/MovableComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/math/Random.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier GettingRedirectedAction::ACTION_ID = "em5::GettingRedirectedAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GettingRedirectedAction::GettingRedirectedAction() :
		qsf::Action(ACTION_ID),
		mState(STATE_INIT),
		mHasActiveMoveToDespawnComponent(false),
		mHasActiveRandomMovementComponent(false)

	{
		// Nothing here
	}

	GettingRedirectedAction::~GettingRedirectedAction()
	{
		// Nothing here
	}

	void GettingRedirectedAction::init(const qsf::ai::SpatialConfiguration3D& target)
	{
		mTarget = target;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Action methods                    ]
	//[-------------------------------------------------------]
	void GettingRedirectedAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTarget.mDirection);
		serializer.serialize(mTarget.mPosition);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool GettingRedirectedAction::onStartup()
	{
		qsf::Entity& entity = getEntity();

		MoveToDespawnComponent* moveToDespawnComponent = entity.getComponent<MoveToDespawnComponent>();
		if (nullptr != moveToDespawnComponent && moveToDespawnComponent->isActive())
		{
			mHasActiveMoveToDespawnComponent = true;

			moveToDespawnComponent->setActive(false);
		}

		RandomMovementComponent* randomMovementComponent = entity.getComponent<RandomMovementComponent>();
		if (nullptr != randomMovementComponent && randomMovementComponent->isActive())
		{
			mHasActiveRandomMovementComponent = true;
			randomMovementComponent->setActive(false);
		}

		setWaitTimeForNextTry();

		return true;
	}

	void GettingRedirectedAction::onShutdown()
	{
		qsf::Entity& entity = getEntity();

		if (mHasActiveMoveToDespawnComponent)
		{
			entity.getOrCreateComponentSafe<MoveToDespawnComponent>().setActive(true);
		}
		if (mHasActiveRandomMovementComponent)
		{
			entity.getOrCreateComponentSafe<RandomMovementComponent>().setActive(true);
		}
	}

	qsf::action::Result GettingRedirectedAction::updateAction(const qsf::Clock& clock)
	{
		switch (mState)
		{
			case STATE_INIT:
			{
				mCurrentWaitTime -= clock.getTimePassed();
				if (mCurrentWaitTime <= qsf::Time::ZERO)
				{
					// Find next free lane (ideally in reverse direction) and teleport the vehicle
					qsf::Entity& entity = getEntity();

					qsf::TransformComponent& transformComponent = entity.getOrCreateComponentSafe<qsf::TransformComponent>();

					// We want a lane which as a reverse direction as the current direction of the entity
					const glm::vec3 wantedDirection = transformComponent.getTransform().vec3DirectionLocalToWorld(glm::vec3(0, 0, 1));

					CollisionHelper collisionHelper(getMap());

					// Reset to the next lane
					qsf::Transform transform;

					// We are an vehicle by design
					if (MapHelper(entity.getMap()).getPointOnClosestLane(transformComponent.getTransform().vec3PositionLocalToWorld(glm::vec3(-3, 0, 0)), 0, 0, transform, wantedDirection))
					{
						// Check for collision
						if (collisionHelper.hasCollisionEM3(entity, transform.getPosition(), 0.0f))
						{
							// Found location is not collision free wait some time and try again
							setWaitTimeForNextTry();
							return qsf::action::RESULT_CONTINUE;
						}

						mTargetTransform = transform;

						// Warp with fading
						FadeEffectComponent& fadeEffectComponent = entity.getOrCreateComponentSafe<FadeEffectComponent>();
						fadeEffectComponent.fadeOut();

						// Wait some time before warping
						mCurrentWaitTime = qsf::Time::fromSeconds(1.0f);

						mState = STATE_FADE_OUT;
					}
					else
					{
						mState = STATE_MOVE_TO_TARGET;
					}
				}
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_FADE_OUT:
			{
				mCurrentWaitTime -= clock.getTimePassed();
				if (mCurrentWaitTime <= qsf::Time::ZERO)
				{
					qsf::Entity& entity = getEntity();

					qsf::TransformComponent& transformComponent = entity.getOrCreateComponentSafe<qsf::TransformComponent>();
					qsf::MovableComponent* movableComponent = entity.getComponent<qsf::MovableComponent>();

					if (nullptr != movableComponent)
					{
						movableComponent->warpToPositionAndRotation(mTargetTransform.getPosition(), mTargetTransform.getRotation());
					}
					else
					{
						transformComponent.setPositionAndRotation(mTargetTransform.getPosition(), mTargetTransform.getRotation());
					}

					FadeEffectComponent& fadeEffectComponent = entity.getOrCreateComponentSafe<FadeEffectComponent>();
					fadeEffectComponent.fadeIn();

					mState = STATE_MOVE_TO_TARGET;
				}
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_MOVE_TO_TARGET:
			{
				mState = STATE_FADE_IN;
				break;
			}
		}

		return qsf::action::RESULT_DONE;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void GettingRedirectedAction::setWaitTimeForNextTry()
	{
		mCurrentWaitTime = qsf::Time::fromSeconds(5.0f);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
