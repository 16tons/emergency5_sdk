// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/move/TurnToAction.h"

#include <qsf/map/Map.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/move/MovableComponent.h>
#include <qsf/math/Math.h>
#include <qsf/math/EulerAngles.h>
#include <qsf/math/CoordinateSystem.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier TurnToAction::ACTION_ID = "em5::TurnToAction";


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	bool TurnToAction::turnToTarget(qsf::Entity& entity, float timeSeconds, float targetYaw, float turnSpeed)
	{
		bool targetReached = false;

		qsf::TransformComponent& transformComponent = entity.getOrCreateComponentSafe<qsf::TransformComponent>();

		glm::vec3 newRotation = qsf::EulerAngles::quaternionToEuler(transformComponent.getRotation());
		float yawChangeRate = 0.0f;

		// Turn instant
		if (turnSpeed > 0.0f)
		{
			const float oldYaw = newRotation.x;

			float differenceYaw = qsf::Math::wrapToInterval(targetYaw - oldYaw, glm::radians(-180.0f), glm::radians(180.0f));

			// Calculate the time difference
			const float maxRotationStep = timeSeconds * glm::radians(turnSpeed);
			float newYaw = oldYaw;

			if (std::fabs(differenceYaw) <= maxRotationStep)
			{
				// Directly go to target yaw
				newYaw = targetYaw;
			}
			else
			{
				// Move towards target yaw
				if (differenceYaw < 0.0f)
				{
					newYaw -= maxRotationStep;
					yawChangeRate = -glm::radians(turnSpeed);
				}
				else
				{
					newYaw += maxRotationStep;
					yawChangeRate = glm::radians(turnSpeed);
				}
			}

			newRotation.x = newYaw;
		}
		else
		{
			// Turn instantly to target
			newRotation.x = targetYaw;
		}

		// Movable component is allowed to be nullptr to support many different kinds of entities
		qsf::MovableComponent* movableComponent = entity.getComponent<qsf::MovableComponent>();

		if (glm::abs(newRotation.x-targetYaw) < FLT_MIN)
		{
			// Finished turning
			targetReached = true;
			yawChangeRate = 0.0f;
		}

		if (nullptr != movableComponent)
		{
			movableComponent->moveToRotation(qsf::EulerAngles::eulerToQuaternion(newRotation));
			movableComponent->setAngularVelocity(qsf::EulerAngles::eulerToQuaternion(glm::vec3(yawChangeRate * 0.1f, 0.0f, 0.0f)), qsf::Time::fromSeconds(0.1f));
		}
		transformComponent.setRotation(qsf::EulerAngles::eulerToQuaternion(newRotation));

		return targetReached;
	}

	float TurnToAction::calcYawRotationToTarget(const qsf::Entity& sourceEntity, const qsf::Entity& targetEntity, bool reverse)
	{
		// Get the current positions
		const qsf::TransformComponent* transformComponent = sourceEntity.getComponent<qsf::TransformComponent>();
		const glm::vec3 callerPosition = transformComponent->getPosition();

		glm::quat finalRotation(qsf::Math::GLM_QUAT_IDENTITY);
		const qsf::TransformComponent* targetTransformComponent = targetEntity.getComponent<qsf::TransformComponent>();
		if (nullptr != targetTransformComponent)
		{
			// Calculate the new rotation
			const glm::vec3 targetPosition = targetTransformComponent->getPosition();

			// To avoid internal assert, check if the position is different
			if (targetPosition != callerPosition)
			{
				if (reverse)
				{
					finalRotation = qsf::Math::getLookAtQuaternion(targetPosition, callerPosition, qsf::CoordinateSystem::getUp());
				}
				else
				{
					finalRotation = qsf::Math::getLookAtQuaternion(callerPosition, targetPosition, qsf::CoordinateSystem::getUp());
				}
			}
			else
			{
				// fall back if the object has the same position, in is case don't turn
				finalRotation = transformComponent->getRotation();
			}
		}
		return qsf::EulerAngles::quaternionToEuler(finalRotation).x;
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	TurnToAction::TurnToAction() :
		Action(ACTION_ID),
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mTurnSpeed(270.0f),
		mReverse(false),
		mTargetYaw(0.0f)
	{
		// Nothing here
	}

	TurnToAction::~TurnToAction()
	{
		// Nothing here
	}

	void TurnToAction::init(uint64 targetEntityId, bool reverse, float turnSpeed)
	{
		mTargetEntityId = targetEntityId;
		mTurnSpeed = turnSpeed;
		mReverse = reverse;
	}

	void TurnToAction::init(const qsf::Entity& targetEntity, bool reverse, float turnSpeed)
	{
		init(targetEntity.getId(), reverse, turnSpeed);
	}

	void TurnToAction::initWithRotation(const glm::quat& finalRotation, float turnSpeed)
	{
		mTargetYaw = qsf::EulerAngles::quaternionToEuler(finalRotation).x;
		mTurnSpeed = turnSpeed;
	}

	void TurnToAction::initWithRotation(const glm::quat& rotation1, const glm::quat& rotation2, float turnSpeed)
	{
		initWithRotation(rotation1, turnSpeed);
		addAlternativeRotation(rotation2);
	}

	void TurnToAction::addAlternativeRotation(const glm::quat& rotation)
	{
		mAlternativeYaws.push_back(qsf::EulerAngles::quaternionToEuler(rotation).x);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void TurnToAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetEntityId);
		serializer.serialize(mTurnSpeed);
		serializer.serialize(mReverse);
		serializer.serialize(mTargetYaw);
	}

	bool TurnToAction::onStartup()
	{
		// In case we have an valid target id, update the final rotation to the target rotation
		qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
		if (targetEntity != nullptr)
		{
			mTargetYaw = calcYawRotationToTarget(getEntity(), *targetEntity, mReverse);
		}
		// In case there is no entity, we assume the orientation is set via initWithRotation()

		// If we have alternative yaws we will pick the best
		if (!mAlternativeYaws.empty())
		{
			const qsf::TransformComponent& transformComponent = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();
			const float oldYaw = qsf::EulerAngles::quaternionToEuler(transformComponent.getRotation()).x;

			float bestDifference = abs(qsf::Math::wrapToInterval(mTargetYaw - oldYaw, -glm::pi<float>(), glm::pi<float>()));

			for (float alternativeYaw : mAlternativeYaws)
			{
				float difference = abs(qsf::Math::wrapToInterval(alternativeYaw - oldYaw, -glm::pi<float>(), glm::pi<float>()));

				if (difference < bestDifference)
				{
					bestDifference = difference;
					mTargetYaw = alternativeYaw;
				}
			}
		}

		return true;
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	qsf::action::Result TurnToAction::updateAction(const qsf::Clock& clock)
	{
		if (turnToTarget(getEntity(), clock.getTimePassed().getSeconds(), mTargetYaw, mTurnSpeed))
			return qsf::action::RESULT_DONE;

		return qsf::action::RESULT_CONTINUE;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
