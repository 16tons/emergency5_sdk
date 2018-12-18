// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/vehicle/parts/HelicopterSearchLightComponent.h"
#include "em5/audio/AudioHelper.h"
#include "em5/audio/AudioProxy.h"

#include <qsf_game/component/base/LightControllerComponent.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/renderer/light/LightComponent.h>
#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>
#include <qsf/math/Math.h>
#include <qsf/math/CoordinateSystem.h>
#include <qsf/math/EulerAngles.h>
#include <qsf/math/Random.h>
#include <qsf/plugin/QsfJobs.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 HelicopterSearchLightComponent::COMPONENT_ID = qsf::StringHash("em5::HelicopterSearchLightComponent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	HelicopterSearchLightComponent::HelicopterSearchLightComponent(qsf::Prototype* prototype) :
		qsf::Component(prototype),
		mSearchLightEntityId(qsf::getUninitialized<uint64>()),
		mIsSearchLight(true),
		mCurrentYaw(0.0f),
		mCurrentPitch(0.0f),
		mFinalYaw(0.0f),
		mFinalPitch(0.0f),
		mCurrentRandYaw(0.0f),
		mCurrentRandPitch(0.0f),
		mFinalRandYaw(0.0f),
		mFinalRandPitch(0.0f),
		mIsFollowingTarget(0.0f),
		mIsSearchLightInFinalRotation(0.0f),
		mIsRandomInZeroRotation(0.0f)
	{
		// Nothing to do in here
	}

	inline HelicopterSearchLightComponent::~HelicopterSearchLightComponent()
	{
		// Nothing to do in here
	}

	void HelicopterSearchLightComponent::startFollowTarget()
	{
		activateJob();
		switchLight(true);
	}

	void HelicopterSearchLightComponent::stopFollowTarget()
	{
		deactivateJob();
		switchLight(false);
	}

	uint64 HelicopterSearchLightComponent::getSearchLightEntityId() const
	{
		return mSearchLightEntityId;
	}

	void HelicopterSearchLightComponent::setSearchLightEntityId(uint64 searchLightEntityId)
	{
		mSearchLightEntityId = searchLightEntityId;
	}

	bool HelicopterSearchLightComponent::getIsSearchLight() const
	{
		return mIsSearchLight;
	}

	void HelicopterSearchLightComponent::setIsSearchLight(bool isSearchLight)
	{
		mIsSearchLight = isSearchLight;
	}

	void HelicopterSearchLightComponent::setTargetEntityId(uint64 targetEntityId)
	{
		qsf::Map& map = getEntity().getMap();
		mTargetEntity = map.getEntityById(targetEntityId);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	void HelicopterSearchLightComponent::onSetSimulating(bool simulating)
	{
		if (simulating)
		{
			mBaseLinkComponent = getEntity().getOrCreateComponent<qsf::LinkComponent>();

			mSearchLightEntity = getEntity().getMap().getEntityById(mSearchLightEntityId);
			mSearchLightLinkComponent = mSearchLightEntity->getOrCreateComponent<qsf::LinkComponent>();

			mStartupJobProxy.registerAt(qsf::QsfJobs::ANIMATION_GENERAL, boost::bind(&HelicopterSearchLightComponent::startupLights, this, _1));
		}
	}

	void HelicopterSearchLightComponent::serialize(qsf::BinarySerializer& serializer)
	{
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void HelicopterSearchLightComponent::activateJob()
	{
		// Is job already active?
		if (!mUpdateJobProxy.isValid())
		{
			// Activate job by registering via the job proxy
			mUpdateJobProxy.registerAt(qsf::QsfJobs::ANIMATION_GENERAL, boost::bind(&HelicopterSearchLightComponent::updateJob, this, _1));

			mBaseLinkComponent = getEntity().getOrCreateComponent<qsf::LinkComponent>();

			qsf::Map& map = getEntity().getMap();
			mSearchLightEntity = map.getEntityById(mSearchLightEntityId);
			mSearchLightLinkComponent = mSearchLightEntity->getOrCreateComponent<qsf::LinkComponent>();
		}
	}

	void HelicopterSearchLightComponent::deactivateJob()
	{
		mUpdateJobProxy.unregister();
	}

	void HelicopterSearchLightComponent::updateJob(const qsf::JobArguments& jobArguments)
	{
		// Check for valid target
		if (mTargetEntity.valid())
		{
			// Get final yaw and pitch, sets mIsFollowingTarget true if it finds valid angles, false if something went wrong so return to otiginal position
			calculateFinalAngles();
		}
		else
		{
			// The default position is 0 0, so if there is no target anymore it returns to the original position
			mFinalYaw = 0;
			mFinalPitch = 0;

			// Flag that stores if its still following a target
			mIsFollowingTarget = false;
		}

		// Calculate current angles
		calculateCurrentAngles(jobArguments.getSecondsPassed());

		// Calculate random movement
		if (mIsSearchLight)
		{
			calculateFinalRandomAngles();
			calculateCurrentRandomAngles(jobArguments.getSecondsPassed());
		}

		// Check if light is in final position
		mIsSearchLightInFinalRotation = (mFinalYaw == mCurrentYaw && mFinalPitch == mCurrentPitch);

		// Check if the random is 0
		mIsRandomInZeroRotation = (mCurrentRandYaw == 0 && mCurrentRandPitch == 0);

		// Make Rotation
		//if (!mIsSearchLightInFinalRotation && !mIsRandomInZeroRotation)
		//{
			setLocalRotation(mCurrentYaw + mCurrentRandYaw, mCurrentPitch + mCurrentRandPitch);
		//}

		// Stop updating if there is no target, its moved to default position back and its not shaking anymore
		if (!mIsFollowingTarget && mIsSearchLightInFinalRotation && mIsRandomInZeroRotation)
		{
			deactivateJob();
			switchLight(false);
		}
	}

	void HelicopterSearchLightComponent::startupLights(const qsf::JobArguments& jobArguments)
	{
		// Deactivate the light on start
		switchLight(false);
		mStartupJobProxy.unregister();
	}

	void HelicopterSearchLightComponent::calculateFinalAngles()
	{
		if (!mSearchLightEntity.valid())
			return;

		// Get the transform components
		qsf::TransformComponent& searchLightTransformComponent = mSearchLightEntity->getOrCreateComponentSafe<qsf::TransformComponent>();
		qsf::TransformComponent& targetTransformComponent = mTargetEntity->getOrCreateComponentSafe<qsf::TransformComponent>();

		// Calculate final angles
		glm::quat worldspaceLookRotation = qsf::Math::getLookAtQuaternion(searchLightTransformComponent.getPosition(), targetTransformComponent.getPosition(), qsf::CoordinateSystem::getUp());

		// Get rotation from parent entity
		qsf::Prototype* parentEntity = getEntity().getOrCreateComponentSafe<qsf::LinkComponent>().getParentPrototype();
		if (nullptr != parentEntity)
		{
			qsf::Transform parentTransform = parentEntity->getOrCreateComponentSafe<qsf::TransformComponent>().getTransform();
			glm::quat finalLookRotation = parentTransform.quatRotationWorldToLocal(worldspaceLookRotation);

			glm::vec3 finalEulerAngles = qsf::EulerAngles::quaternionToEuler(finalLookRotation);
			mFinalYaw = finalEulerAngles.x;
			mFinalPitch = finalEulerAngles.y;

			// Returns true if the angles could be created
			mIsFollowingTarget = true;
			return;
		}

		// Returns flase if the angles couldnt be created
		mIsFollowingTarget = false;
	}

	void HelicopterSearchLightComponent::setLocalRotation(float yaw, float pitch)
	{
		// For search light only pitch
		glm::vec3 cannonAngles;
 		cannonAngles.y = pitch;
		mSearchLightLinkComponent->setLocalRotation(qsf::EulerAngles::eulerToQuaternion(cannonAngles));

		// For base entity only yaw
		glm::vec3 baseAngles;
		baseAngles.x = yaw;
		mBaseLinkComponent->setLocalRotation(qsf::EulerAngles::eulerToQuaternion(baseAngles));
	}

	void HelicopterSearchLightComponent::calculateCurrentAngles(float passedTime)
	{
		// Angles per seconds in seconds
		static const float SEARCHLIGHT_TURN_SPEED = glm::radians<float>(70);
		static const float SEARCHLIGHT_TILT_SPEED = glm::radians<float>(40);

		// Get local rotation from base link
		glm::quat currentBaseRotation = mBaseLinkComponent->getLocalRotation();
		glm::vec3 eulerBaseAngles = qsf::EulerAngles::quaternionToEuler(currentBaseRotation);
		mCurrentYaw = eulerBaseAngles.x;

		// Get local rotation from cannon link
		glm::quat currentSearchLightRotation = mSearchLightLinkComponent->getLocalRotation();
		glm::vec3 eulerSearchLightAngles = qsf::EulerAngles::quaternionToEuler(currentSearchLightRotation);
		mCurrentPitch = eulerSearchLightAngles.y;

		if (mCurrentYaw != mFinalYaw)
		{
			float turnSpeed = passedTime * SEARCHLIGHT_TURN_SPEED;
			float turnYawThisUpdate = qsf::Math::wrapToInterval(mFinalYaw - mCurrentYaw, glm::radians<float>(-180), glm::radians<float>(180));
			if (glm::abs(turnYawThisUpdate) < turnSpeed)
			{
				// Reach final rotation (consider floating point errors)
				mCurrentYaw = mFinalYaw;
			}
			else
			{
				// Move step by step to final rotation
				turnYawThisUpdate = glm::clamp(turnYawThisUpdate, -turnSpeed, turnSpeed);
				mCurrentYaw += turnYawThisUpdate;
			}
		}

		if (mCurrentPitch != mFinalPitch)
		{
			float turnSpeed = passedTime * SEARCHLIGHT_TILT_SPEED;
			float turnPitchThisUpdate = qsf::Math::wrapToInterval(mFinalPitch - mCurrentPitch, glm::radians<float>(-180), glm::radians<float>(180));
			if (glm::abs(turnPitchThisUpdate) < turnSpeed)
			{
				// Reach final rotation (consider floating point errors)
				mCurrentPitch = mFinalPitch;
			}
			else
			{
				// Move step by step to final rotation
				turnPitchThisUpdate = glm::clamp(turnPitchThisUpdate, -turnSpeed, turnSpeed);
				mCurrentPitch += turnPitchThisUpdate;
			}
		}
	}

	void HelicopterSearchLightComponent::calculateFinalRandomAngles()
	{
		if (!mIsFollowingTarget)
		{
			mFinalRandYaw = 0.0f;
			mFinalRandPitch = 0.0f;

			return;
		}

		// Max random angles
		static const float MAX_RAND_YAW = glm::radians<float>(1.0f);
		static const float MAX_RAND_PITCH = glm::radians<float>(1.0f);

		if (mCurrentRandYaw == mFinalRandYaw)
		{
			mFinalRandYaw = qsf::Random::getRandomFloat(-MAX_RAND_YAW, MAX_RAND_YAW);
		}

		if (mCurrentRandPitch == mFinalRandPitch)
		{
			mFinalRandPitch = qsf::Random::getRandomFloat(-MAX_RAND_PITCH, MAX_RAND_PITCH);
		}
	}

	void HelicopterSearchLightComponent::calculateCurrentRandomAngles(float passedTime)
	{
		// Angles per seconds in seconds
		static const float SEARCHLIGHT_TURN_SPEED = glm::radians<float>(0.4f);
		static const float SEARCHLIGHT_TILT_SPEED = glm::radians<float>(0.2f);

		// TODO(co) Review
		// float diff = glm::abs(mFinalRandYaw - mCurrentRandYaw);

		if (mCurrentRandYaw != mFinalRandYaw)
		{
			float turnSpeed = passedTime * SEARCHLIGHT_TURN_SPEED;
			float turnYawThisUpdate = qsf::Math::wrapToInterval(mFinalRandYaw - mCurrentRandYaw, glm::radians<float>(-180), glm::radians<float>(180));
			if (glm::abs(turnYawThisUpdate) < turnSpeed)
			{
				// Reach final rotation (consider floating point errors)
				mCurrentRandYaw = mFinalRandYaw;
			}
			else
			{
				// Move step by step to final rotation
				turnYawThisUpdate = glm::clamp(turnYawThisUpdate, -turnSpeed, turnSpeed);
				mCurrentRandYaw += turnYawThisUpdate;
			}

			// TODO(co) Review
			//if (diff < glm::abs(mFinalRandYaw - mCurrentRandYaw))
			//{
			//	diff = 0;
			//}
		}

		// TODO(co) Commented code because it has no effect, we might want to remove it
		// diff = glm::abs(mCurrentRandPitch - mCurrentRandPitch);

		if (mCurrentRandPitch != mFinalRandPitch)
		{
			float tiltSpeed = passedTime * SEARCHLIGHT_TILT_SPEED;
			float tiltPitchThisUpdate = qsf::Math::wrapToInterval(mFinalRandPitch - mCurrentRandPitch, glm::radians<float>(-180), glm::radians<float>(180));
			if (glm::abs(tiltPitchThisUpdate) < tiltSpeed)
			{
				// Reach final rotation (consider floating point errors)
				mCurrentRandPitch = mFinalRandPitch;
			}
			else
			{
				// Move step by step to final rotation
				tiltPitchThisUpdate = glm::clamp(tiltPitchThisUpdate, -tiltSpeed, tiltSpeed);
				mCurrentRandPitch += tiltPitchThisUpdate;
			}

			// TODO(co) Commented code because it has no effect, we might want to remove it
			/*
			if (diff < glm::abs(mCurrentRandPitch - mCurrentRandPitch))
			{
				diff = 0;
			}
			*/
		}
	}

	void HelicopterSearchLightComponent::switchLight(bool active)
	{
		if (!mIsSearchLight)
			return;

		if (active)
		{
			AudioProxy audioProxy;
			AudioHelper::playHelicopterSearchLightOn(audioProxy, getEntity());
			audioProxy.detach();
		}

		if (mSearchLightEntity.valid() && mSearchLightLinkComponent.valid())
		{
			qsf::LightComponent* lightComponent = mSearchLightLinkComponent->getComponentFromEntityOrLinkedChild<qsf::LightComponent>();
			if (nullptr != lightComponent)
			{
				lightComponent->setActive(active);
			}

			qsf::game::LightControllerComponent* lightConntrollerComponent = mSearchLightLinkComponent->getComponentFromEntityOrLinkedChild<qsf::game::LightControllerComponent>();
			if (nullptr != lightConntrollerComponent)
			{
				lightConntrollerComponent->setActive(active);
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
