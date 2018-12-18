// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/movement/PathMovementComponent.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/plugin/Jobs.h"
#include "em5/plugin/Messages.h"

#include <qsf_ai/navigation/NavigationComponent.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/move/MovableComponent.h>
#include <qsf/component/nodes/PathComponent.h>
#include <qsf/job/JobArguments.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/map/Map.h>
#include <qsf/math/Math.h>
#include <qsf/log/LogSystem.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 PathMovementComponent::COMPONENT_ID = qsf::StringHash("em5::PathMovementComponent");


	//[-------------------------------------------------------]
	//[ Public methods                                    ]
	//[-------------------------------------------------------]
	PathMovementComponent::PathMovementComponent(qsf::Prototype* prototype):
		qsf::Component(prototype),
		mMaximumSpeed(1.0f), // Default one unit per second
		mMaximumAcceleration(1000.0f),	// "Instant"
		mMaximumDeceleration(1000.0f),	// "Instant"
		mCurveSpeedReduction(0.0f),
		mLooped(false),
		mCurrentSpeed(0.0f),
		mCurrentPathOffset(0.0f),
		mPathEntityId(qsf::getUninitialized<uint64>()),
		mAiNavigationWasActiveBefore(false)
	{
		// Nothing here
	}

	PathMovementComponent::~PathMovementComponent()
	{
		// Nothing here
	}

	void PathMovementComponent::setPath(qsf::PathComponent& pathComponent, bool activate)
	{
		// Clear path segmenter
		mPathSegmenter.buildFromNodeArray(std::vector<qsf::Node>());

		QSF_CHECK(pathComponent.getNumberOfNodes() >= 2, "The PathMovementComponent needs a path with at least two nodes to work", return);

		// Setup the path segmenter
		mPathSegmenter.buildFromNodeArray(pathComponent.getNodes());

		mPathComponent = pathComponent.getEntity().getComponent<qsf::TransformComponent>();

		if (activate && isSimulating())
		{
			onSetActive(isActive());
		}
	}

	void PathMovementComponent::setPath(const qsf::Entity& entityContainingPath, bool activate)
	{
		// Clear path segmenter
		mPathSegmenter.buildFromNodeArray(std::vector<qsf::Node>());

		qsf::PathComponent* pathComponent = entityContainingPath.getComponent<qsf::PathComponent>();

		QSF_CHECK(nullptr != pathComponent, "The PathMovementComponent needs a path with at least two nodes to work", QSF_REACT_NONE);

		if (nullptr != pathComponent)
		{
			setPath(*pathComponent, activate);
		}
	}

	void PathMovementComponent::setSpeed(float speed)
	{
		mMaximumSpeed = speed;
	}

	void PathMovementComponent::setSpeedByTime(const qsf::Time& time)
	{
		const float pathLength = mPathSegmenter.getLength();
		if (pathLength > 0.0f)
		{
			mMaximumSpeed = pathLength / time.getSeconds();
		}
	}

	void PathMovementComponent::initializeConfiguration(float speed, float maximumAcceleration, float maximumDeceleration, float curveSpeedReduction)
	{
		mMaximumSpeed = speed;
		mMaximumAcceleration = maximumAcceleration;
		mMaximumDeceleration = maximumDeceleration;
		mCurveSpeedReduction = curveSpeedReduction;
	}

	bool PathMovementComponent::isLooped() const
	{
		return mLooped;
	}

	void PathMovementComponent::setLooped(bool looped)
	{
		mLooped = looped;
	}

	float PathMovementComponent::getPathProgressPercentage() const
	{
		const float pathLength = mPathSegmenter.getLength();
		if (pathLength > 0.0f)
		{
			return mCurrentPathOffset / pathLength;
		}

		return 0.0f;
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	void PathMovementComponent::onSetSimulating(bool simulating)
	{
		if (simulating && isActive())
		{
			onSetActive(true);
		}
		else
		{
			internalActivate(false);
		}
	}

	void PathMovementComponent::onSetActive(bool active)
	{
		if (isRunning())
		{
			if (isSimulating() && active)
			{
				if (qsf::isInitialized(mPathEntityId) && mPathSegmenter.getLength() <= 0.0f)
				{
					qsf::Entity* entity = getEntity().getMap().getEntityById(mPathEntityId);
					if (nullptr != entity)
					{
						setPath(*entity, false);
					}
				}

				if (mPathSegmenter.getLength() > 0.0f)
				{
					internalActivate(true);
					mCurrentPathOffset = 0.0f;
				}
			}
			else
			{
				internalActivate(false);
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void PathMovementComponent::updateJob(const qsf::JobArguments& jobArguments)
	{
		QSF_CHECK(mPathComponent.valid(), "Path component is invalid", mCurrentPathOffset = mPathSegmenter.getLength())

		if (mCurrentPathOffset >= mPathSegmenter.getLength())
		{
			if (mLooped)
			{
				// Start another loop from the beginning
				mCurrentPathOffset = std::fmod(mCurrentPathOffset, mPathSegmenter.getLength());
			}
			else
			{
				// Stop path movement
				internalActivate(false);

				// We have reached the end of the path inform anyone who want to know about it
				qsf::MessageParameters messageParameters;
				QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_FOLLOW_PATH_REACHED_END, getEntityId()), messageParameters);
			}
		}

		const float secondsPassed = jobArguments.getSecondsPassed();

		const qsf::Transform& pathTransform = mPathComponent->getTransform();

		const glm::vec3 position = pathTransform.vec3PositionLocalToWorld(mPathSegmenter.evaluatePositionAt(mCurrentPathOffset));
		const glm::vec3 direction = pathTransform.vec3DirectionLocalToWorld(mPathSegmenter.evaluateDirectionAt(mCurrentPathOffset));
		const glm::quat rotation = qsf::Math::getLookAtQuaternion(direction, glm::vec3(0, 1, 0));

		// Look ahead for curves
		float targetSpeed = mMaximumSpeed;
		if (mCurveSpeedReduction > 0.0f)
		{
			// TODO(fw): The hard-coded constants here should better be configurable
			const float aheadDistance = 30.0f;
			const glm::vec3 directionAhead = pathTransform.vec3DirectionLocalToWorld(mPathSegmenter.evaluateDirectionAt(mCurrentPathOffset + aheadDistance));
			const float angle = glm::degrees(std::acosf(std::min(glm::dot(direction, directionAhead), 1.0f)));
			const float speedReduction = std::min(angle / 60.0f, 0.75f) * mCurveSpeedReduction;
			targetSpeed = mMaximumSpeed * (1.0f - speedReduction);
		}

		// Accelerate
		if (mCurrentSpeed < targetSpeed)
		{
			mCurrentSpeed += mMaximumAcceleration * secondsPassed;
			if (mCurrentSpeed > targetSpeed)
				mCurrentSpeed = targetSpeed;
		}
		// Slow down
		else if (mCurrentSpeed > targetSpeed)
		{
			mCurrentSpeed -= mMaximumDeceleration * secondsPassed;
			if (mCurrentSpeed < targetSpeed)
				mCurrentSpeed = targetSpeed;
		}

		qsf::MovableComponent* movableComponent = getEntity().getComponent<qsf::MovableComponent>();
		if (nullptr != movableComponent)
		{
			movableComponent->moveToPositionAndRotation(position, rotation);

			// TODO(fw): We need to set velocity *and* angular velocity for smooth tick extrapolation
			movableComponent->setVelocity(rotation * glm::vec3(0, 0, -1) * mCurrentSpeed);
		}
		else
		{
			qsf::TransformComponent& transformComponent = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();
			transformComponent.setPositionAndRotation(position, rotation);
		}

		mCurrentPathOffset += mCurrentSpeed * secondsPassed;
	}

	void PathMovementComponent::internalActivate(bool activate)
	{
		qsf::ai::NavigationComponent* navigationComponent = getPrototype().getComponent<qsf::ai::NavigationComponent>();

		if (activate)
		{
			// Init update
			qsf::jobs::JobConfiguration jobConfig;
			mUpdateJobProxy.registerAt(Jobs::SIMULATION_LOGIC, boost::bind(&PathMovementComponent::updateJob, this, _1), jobConfig);

			mAiNavigationWasActiveBefore = false;
			if (nullptr != navigationComponent)
			{
				mAiNavigationWasActiveBefore = navigationComponent->isActive();
				navigationComponent->setActive(false);
			}
		}
		else
		{
			mUpdateJobProxy.unregister();

			if (nullptr != navigationComponent && mAiNavigationWasActiveBefore)
			{
				navigationComponent->setActive(true);
				mAiNavigationWasActiveBefore = false;
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
