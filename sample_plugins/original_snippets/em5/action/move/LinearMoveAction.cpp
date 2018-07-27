// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/move/LinearMoveAction.h"

#include <qsf_ai/navigation/NavigationComponent.h>

#include <qsf/component/move/MovableComponent.h>
#include <qsf/math/CoordinateSystem.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier LinearMoveAction::ACTION_ID = "em5::LinearMoveAction";
	const float LinearMoveAction::TOLERANCE = 0.01f;
	const float LinearMoveAction::DEFAULT_MAX_SPEED = 1.0f;
	const float LinearMoveAction::DEFAULT_ACCELERATION = 1.0f;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	LinearMoveAction::LinearMoveAction() :
		qsf::Action(ACTION_ID),
		mMaxDistance(std::numeric_limits<float>::max()),
		mMinDistance(0),
		mMaxSpeed(DEFAULT_MAX_SPEED),
		mAcceleration(DEFAULT_ACCELERATION)
	{
		// Nothing here
	}

	LinearMoveAction::~LinearMoveAction()
	{
		// Nothing here
	}

	void LinearMoveAction::init(const glm::vec3& targetPosition, float minDistance, float maxDistance)
	{
		mTargetPosition = targetPosition;
		mMinDistance = minDistance;
		mMaxDistance = maxDistance;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Action methods                    ]
	//[-------------------------------------------------------]
	void LinearMoveAction::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base implementation
		qsf::Action::serialize(serializer);

		serializer.serialize(mTargetPosition);
		serializer.serialize(mMinDistance);
		serializer.serialize(mMaxDistance);
		serializer.serialize(mMaxSpeed);
		serializer.serialize(mAcceleration);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	qsf::action::Result LinearMoveAction::updateAction(const qsf::Clock& clock)
	{
		qsf::MovableComponent& movableComponent = getEntity().getOrCreateComponentSafe<qsf::MovableComponent>();

		// Get our position
		const glm::vec3& position = movableComponent.getPosition();

		// Direction and distance to our target
		glm::vec3 direction = mTargetPosition - position;
		glm::vec3 directionXZ(direction.x, 0.0f, direction.z);
		const float distanceXZ = glm::length(directionXZ);
		direction /= distanceXZ;		// Normalization (ignoring y)
		directionXZ /= distanceXZ;		// Normalization

		// Use a tolerance to check if we are already at target and not 0
		if (distanceXZ < TOLERANCE)
			return qsf::action::RESULT_DONE;

		const float secondsPassed = clock.getTimePassed().getSeconds();

		// Only update when our time is moving forward
		if (secondsPassed <= 0.0f)
			return qsf::action::RESULT_CONTINUE;

		// Speed in m/s
		const float oldSpeed = glm::length(movableComponent.getVelocity());
		const float speed = getMovementSpeed(oldSpeed, secondsPassed, distanceXZ);

		const glm::vec3 velocity = direction * speed;
		const glm::vec3 deltaPosition = velocity * secondsPassed;

		// Update position and velocity
		if (distanceXZ * distanceXZ < glm::length2(deltaPosition))
		{
			// We move to target, but lets wait a tick to check if we are really there
			movableComponent.setVelocity(direction * (distanceXZ / secondsPassed));
			movableComponent.moveToPosition(mTargetPosition);
		}
		else
		{
			movableComponent.setVelocity(velocity);
			movableComponent.moveToPosition(position + deltaPosition);
		}

		return qsf::action::RESULT_CONTINUE;
	}

	bool LinearMoveAction::onStartup()
	{
		// Disable the navigation component steering for the moment
		qsf::ai::NavigationComponent* navigationComponent = getEntity().getComponent<qsf::ai::NavigationComponent>();
		if (nullptr != navigationComponent)
		{
			navigationComponent->setDisableSteering(true);
			mMaxSpeed = navigationComponent->getMaxForwardSpeed();
			mAcceleration = navigationComponent->getDesiredAcceleration();
		}

		qsf::MovableComponent& movableComponent = getEntity().getOrCreateComponentSafe<qsf::MovableComponent>();

		// Get our xz Position
		glm::vec3 position = movableComponent.getPosition();
		position.y = 0.0f;

		// Direction and distance to our target
		glm::vec3 direction = mTargetPosition - position;
		glm::vec3 directionXZ(direction.x, 0.0f, direction.z);
		const float distanceXZ = glm::length(directionXZ);
		directionXZ /= distanceXZ;		// Normalization

		// Check we are outside max distance and finish the action
		if (distanceXZ > mMaxDistance)
			return false;

		// Check we are inside min distance and finish the action
		if (distanceXZ <= mMinDistance)
			return false;

		// Move to the right rotation
		const glm::quat& targetRotation = qsf::Math::getLookAtQuaternion(direction, qsf::CoordinateSystem::getUp());
		movableComponent.moveToRotation(targetRotation);

		return true;
	}

	void LinearMoveAction::onShutdown()
	{
		// Re-enable the navigation component steering again
		qsf::ai::NavigationComponent* navigationComponent = getEntity().getComponent<qsf::ai::NavigationComponent>();
		if (nullptr != navigationComponent)
		{
			navigationComponent->setDisableSteering(false);
		}
	}

	void LinearMoveAction::onAbort()
	{
		onShutdown();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	float LinearMoveAction::getMovementSpeed(float oldSpeed, float secondsPassed, float distance)
	{
		float speed = oldSpeed;

		float acceleration = mAcceleration;

		// Check for deceleration
		const float distanceToBreak = speed * speed / mAcceleration;
		if (distance <= distanceToBreak)
		{
			acceleration = -mAcceleration;
		}

		speed = glm::clamp(speed + acceleration * secondsPassed, 0.0f, mMaxSpeed);

		return speed;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
