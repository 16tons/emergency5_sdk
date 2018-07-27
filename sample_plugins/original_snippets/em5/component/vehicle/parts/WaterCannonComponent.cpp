// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/vehicle/parts/WaterCannonComponent.h"

#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/job/JobArguments.h>
#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>
#include <qsf/math/Math.h>
#include <qsf/math/CoordinateSystem.h>
#include <qsf/math/EulerAngles.h>
#include <qsf/plugin/QsfJobs.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 WaterCannonComponent::COMPONENT_ID = qsf::StringHash("em5::WaterCannonComponent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	void WaterCannonComponent::setCannonEntityId(uint64 cannonEntityId)
	{
		mCannonEntityId = cannonEntityId;
	}

	uint64 WaterCannonComponent::getTargetEntityId() const
	{
		return mTargetEntity.valid() ? mTargetEntity->getId() : qsf::getUninitialized<uint64>();
	}

	void WaterCannonComponent::setTargetEntityId(uint64 targetEntityId)
	{
		mTargetEntity = getEntity().getMap().getEntityById(targetEntityId);

		mTargetTransformComponent = (mTargetEntity.valid() ? mTargetEntity->getOrCreateComponent<qsf::TransformComponent>() : nullptr);
		mIsCannonInFinalRotation = false;
	}

	bool WaterCannonComponent::getIsCannonInFinalRotation() const
	{
		return mIsCannonInFinalRotation;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	void WaterCannonComponent::serialize(qsf::BinarySerializer& serializer)
	{
		// TODO(mk) Write serialize function
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	void WaterCannonComponent::onSetSimulating(bool simulating)
	{
		if (simulating)
		{
			mUpdateJobProxy.registerAt(qsf::QsfJobs::ANIMATION_GENERAL, boost::bind(&WaterCannonComponent::updateJob, this, _1));

			getEntity().getOrCreateComponent<qsf::TransformComponent>();	// TODO(co) Required?
			mBaseLinkComponent = getEntity().getOrCreateComponent<qsf::LinkComponent>();

			qsf::Entity* cannonEntity = getEntity().getMap().getEntityById(mCannonEntityId);
			if (nullptr != cannonEntity)
			{
				mCannonTransformComponent = cannonEntity->getOrCreateComponent<qsf::TransformComponent>();
				mCannonLinkComponent = cannonEntity->getOrCreateComponent<qsf::LinkComponent>();
			}
		}
		else
		{
			mUpdateJobProxy.unregister();
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void WaterCannonComponent::updateJob(const qsf::JobArguments& jobArguments)
	{
		bool takeDefault = true;
		if (mTargetEntity.valid() && mTargetTransformComponent.valid() && mCannonTransformComponent.valid())
		{
			// Calculate final angles
			glm::quat worldspaceLookRotation = qsf::Math::getLookAtQuaternion(mCannonTransformComponent->getPosition(), mTargetTransformComponent->getPosition(), qsf::CoordinateSystem::getUp());

			// Get rotation from parent entity
			qsf::Prototype* parentEntity = getEntity().getOrCreateComponentSafe<qsf::LinkComponent>().getParentPrototype();
			if (nullptr != parentEntity)
			{
				takeDefault = false;
				const qsf::Transform& parentTransform = parentEntity->getOrCreateComponentSafe<qsf::TransformComponent>().getTransform();
				glm::quat finalLookRotation = parentTransform.quatRotationWorldToLocal(worldspaceLookRotation);

				if (mIsOnBoat)
				{
					finalLookRotation *= qsf::EulerAngles::eulerToQuaternion(0.0f, -0.60f, 0.0f);
				}

				glm::vec3 finalEulerAngles = qsf::EulerAngles::quaternionToEuler(finalLookRotation);
				mFinalYaw = finalEulerAngles.x;

				// Take absolute value, in case the angle is <0, we raise the cannon. This looks more realistic.
				// TODO(tl): Boats need to be able to lower the cannons
				mFinalPitch = mIsOnBoat ? finalEulerAngles.y : glm::abs(finalEulerAngles.y);
			}
		}

		if (takeDefault)
		{
			mFinalYaw = 0.0f;
			mFinalPitch = 0.0f;
		}

		// Angles per seconds in seconds
		static const float CANNON_TURN_SPEED = glm::radians<float>(70);
		static const float CANNON_TILT_SPEED = glm::radians<float>(40);

		if (!mBaseLinkComponent.valid() || !mCannonLinkComponent.valid())
			return;

		// Get local rotation from base link
		float currentYaw = qsf::EulerAngles::quaternionToEuler(mBaseLinkComponent->getLocalRotation()).x;

		// Get local rotation from cannon link
		float currentPitch = qsf::EulerAngles::quaternionToEuler(mCannonLinkComponent->getLocalRotation()).y;

		// Check if cannon is already in final position
		mIsCannonInFinalRotation = true;
		if (std::fabsf(currentYaw - mFinalYaw) >= 0.001f || std::fabsf(currentPitch - mFinalPitch) >= 0.001f)
		{
			mIsCannonInFinalRotation = false;
			if (currentYaw != mFinalYaw)
			{
				float turnSpeed = jobArguments.getSecondsPassed() * CANNON_TURN_SPEED;
				float turnYawThisUpdate = qsf::Math::wrapToInterval(mFinalYaw - currentYaw, glm::radians<float>(-180), glm::radians<float>(180));
				if (glm::abs(turnYawThisUpdate) < turnSpeed)
				{
					// Reach final rotation (consider floating point errors)
					currentYaw = mFinalYaw;
				}
				else
				{
					// Move step by step to final rotation
					turnYawThisUpdate = glm::clamp(turnYawThisUpdate, -turnSpeed, turnSpeed);
					currentYaw += turnYawThisUpdate;
				}
			}

			if (currentPitch != mFinalPitch)
			{
				float turnSpeed = jobArguments.getSecondsPassed() * CANNON_TILT_SPEED;
				float turnPitchThisUpdate = qsf::Math::wrapToInterval(mFinalPitch - currentPitch, glm::radians<float>(-180), glm::radians<float>(180));
				if (glm::abs(turnPitchThisUpdate) < turnSpeed)
				{
					// Reach final rotation (consider floating point errors)
					currentPitch = mFinalPitch;
				}
				else
				{
					// Move step by step to final rotation
					turnPitchThisUpdate = glm::clamp(turnPitchThisUpdate, -turnSpeed, turnSpeed);
					currentPitch += turnPitchThisUpdate;
				}
			}

			setLocalRotation(currentYaw, currentPitch);
		}
	}

	void WaterCannonComponent::setLocalRotation(float yaw, float pitch)
	{
		QSF_CHECK(mCannonLinkComponent.valid(), "Water cannon link component is invalid", return);
		QSF_CHECK(mBaseLinkComponent.valid(), "Water cannon base link component is invalid", return);

		// For cannon only pitch
		glm::vec3 cannonAngles;
		cannonAngles.y = pitch;
		mCannonLinkComponent->setLocalRotation(qsf::EulerAngles::eulerToQuaternion(cannonAngles));

		// For base entity only yaw
		glm::vec3 baseAngles;
		baseAngles.x = yaw;
		mBaseLinkComponent->setLocalRotation(qsf::EulerAngles::eulerToQuaternion(baseAngles));
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
