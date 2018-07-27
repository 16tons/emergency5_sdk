// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/movement/SwingComponent.h"
#include "em5/component/vehicle/BoatComponent.h"
#include "em5/game/units/UnitHelper.h"
#include "em5/plugin/Jobs.h"

#include <qsf/component/move/MovableComponent.h>
#include <qsf/renderer/component/RendererComponent.h>
#include <qsf/math/EulerAngles.h>
#include <qsf/math/Random.h>
#include <qsf/math/Math.h>
#include <qsf/map/Entity.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Detail implementation                                 ]
	//[-------------------------------------------------------]
	namespace detail
	{
		/**
		*  @brief
		*    Attenuation of a value between 0 and 1 around 0.5, with factor "att"
		*/
		inline float attenuation(float value, float att)
		{
			return value * att + (1.0f - att) * 0.5f;
		}
	}


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 SwingComponent::COMPONENT_ID			= qsf::StringHash("em5::SwingComponent");
	const uint32 SwingComponent::ANIMATION_TYPE			= qsf::StringHash("AnimationType");
	const uint32 SwingComponent::ANIMATION_CYCLE		= qsf::StringHash("AnimationCycle");
	const uint32 SwingComponent::ANIMATION_INTENSITY	= qsf::StringHash("AnimationIntensity");
	const uint32 SwingComponent::VELOCITY_SKEW			= qsf::StringHash("VelocitySkew");

	const float SwingComponent::MINIMUM_SQUARED_VELOCITY = 0.01f * 0.01f;
	const float SwingComponent::ACCELERATION_SPEED		 = 2.0f;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	SwingComponent::SwingComponent(qsf::Prototype* prototype):
		qsf::Component(prototype),
		mAnimationType(SwingComponent::PULSE),
		mAnimationCycle(1.0f),
		mAnimationIntensity(1.0f),
		mVelocitySkew(0.0f),
		mCyclePassed(0.0f),
		mOldSkew(0.0f)
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Protected virtual em5::Component methods              ]
	//[-------------------------------------------------------]
	void SwingComponent::onSetActive(bool active)
	{
		if (isRunning())
		{
			if (active)
			{
				registerJobProxy();
			}
			else
			{
				unregisterMovableObjectRenderingListener();

				// Reset transform bias
				if (mMovableComponent.valid())
				{
					mMovableComponent->resetTransformBias();
				}
			}
		}
	}

	bool SwingComponent::onStartup()
	{
		if (isActive())
		{
			registerJobProxy();
		}
		return true;
	}

	void SwingComponent::onShutdown()
	{
		if (isActive())
		{
			unregisterMovableObjectRenderingListener();
		}
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::MovableObjectRenderingListener methods ]
	//[-------------------------------------------------------]
	qsf::Component* SwingComponent::getMovableObjectRenderingListenerComponent()
	{
		return getEntity().getComponent<qsf::RendererComponent>();
	}

	void SwingComponent::onObjectRenderingStartup()
	{
		// Register job
		qsf::jobs::JobConfiguration jobConfiguration;
		jobConfiguration.mTimeBetweenCalls = qsf::Time::fromMilliseconds(33);
		mAnimationJobProxy.registerAt(Jobs::ANIMATION_VEHICLE, boost::bind(&SwingComponent::onUpdateMovableObjectRenderingJob, this, _1), jobConfiguration);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::JobProxyMovableObjectRenderingListener methods ]
	//[-------------------------------------------------------]
	void SwingComponent::onUpdateMovableObjectRenderingJob(const qsf::JobArguments& jobArguments)
	{
		mCyclePassed += (mAnimationCycle == 0.0f) ? 0.0f : jobArguments.getSecondsPassed() / mAnimationCycle;
		mCyclePassed = std::fmod(mCyclePassed, 1.0f);	// Clamp to 0.0f - 1.0f

		glm::vec3 position(0.5f, 0.5f, 0.5f);
		glm::vec3 rotation(0.5f, 0.5f, 0.5f);
		glm::quat skewRotation(qsf::Math::GLM_QUAT_IDENTITY);

		// Get the movable component instance
		qsf::MovableComponent* movableComponent = nullptr;
		if (mMovableComponent.valid())
		{
			movableComponent = mMovableComponent.get();
		}
		else
		{
			mMovableComponent = movableComponent = getEntity().getComponent<qsf::MovableComponent>();
		}

		switch (mAnimationType)
		{
			case PULSE:
			{
				rotation.y = (1.0f + cos(mCyclePassed * 2.0f * glm::pi<float>())) / 2.0f;
				break;
			}

			case BOATBOBBING:
			{
				const float cycleRotationX = std::fmod(mCyclePassed * 11.0f, 1.0f);
				const float cycleRotationY = std::fmod(mCyclePassed * 10.0f, 1.0f);
				const float cycleRotationZ = std::fmod(mCyclePassed *  9.0f, 1.0f);
				const float cyclePositionX = std::fmod(mCyclePassed *  8.0f, 1.0f);
				const float cyclePositionY = std::fmod(mCyclePassed *  7.0f, 1.0f);
				const float cyclePositionZ = std::fmod(mCyclePassed *  6.0f, 1.0f);

				rotation.x = detail::attenuation((1.0f + cos(cycleRotationX * 2.0f * glm::pi<float>())) / 2.0f, 0.200f);
				rotation.y = detail::attenuation((1.0f + cos(cycleRotationY * 2.0f * glm::pi<float>())) / 2.0f, 0.800f);
				rotation.z = detail::attenuation((1.0f + cos(cycleRotationZ * 2.0f * glm::pi<float>())) / 2.0f, 1.000f);
				position.x = detail::attenuation((1.0f + cos(cyclePositionX * 2.0f * glm::pi<float>())) / 2.0f, 0.007f);
				position.y = detail::attenuation((1.0f + cos(cyclePositionY * 2.0f * glm::pi<float>())) / 2.0f, 0.020f);
				position.z = detail::attenuation((1.0f + cos(cyclePositionZ * 2.0f * glm::pi<float>())) / 2.0f, 0.004f);

				const float velocity = (nullptr != movableComponent) ? getVelocity(*movableComponent) : mVelocitySkew;
				skewRotation = qsf::EulerAngles::eulerToQuaternion(0.0f, velocity, 0.0f);

				// Set propeller particles
				if (nullptr != movableComponent)
				{
					BoatComponent* boatComponent = getEntity().getComponent<BoatComponent>();
					if (nullptr != boatComponent)
					{
						boatComponent->enablePropellerParticles(glm::length2(movableComponent->getVelocity()) > MINIMUM_SQUARED_VELOCITY);
					}
				}

				// TODO(tl): Hack to let the boat sink into water (don't sink fast boats)
				position.y -= 0.4f / mAnimationIntensity * pow(20.0f, -velocity);

				break;
			}

			case HELICOPTERSWING:
			{
				const float cycleRotationX = std::fmod(mCyclePassed * 11.0f, 1.0f);
				const float cycleRotationY = std::fmod(mCyclePassed * 10.0f, 1.0f);
				const float cycleRotationZ = std::fmod(mCyclePassed *  9.0f, 1.0f);
				const float cyclePositionX = std::fmod(mCyclePassed *  8.0f, 1.0f);
				const float cyclePositionY = std::fmod(mCyclePassed *  7.0f, 1.0f);
				const float cyclePositionZ = std::fmod(mCyclePassed *  6.0f, 1.0f);
				const float cycleRotation  = std::fmod(mCyclePassed *  5.0f, 1.0f);
				const float cyclePosition  = std::fmod(mCyclePassed *  2.0f, 1.0f);

				rotation.x = detail::attenuation((1.0f + cosf(cycleRotationX * 2.0f * glm::pi<float>())) / 2.0f, 1.000f);
				rotation.y = detail::attenuation((1.0f + cosf(cycleRotationY * 2.0f * glm::pi<float>())) / 2.0f, 0.900f);
				rotation.z = detail::attenuation((1.0f + cosf(cycleRotationZ * 2.0f * glm::pi<float>())) / 2.0f, 0.800f);
				position.x = detail::attenuation((1.0f + cosf(cyclePositionX * 2.0f * glm::pi<float>())) / 2.0f, 0.040f);
				position.y = detail::attenuation((1.0f + cosf(cyclePositionY * 2.0f * glm::pi<float>())) / 2.0f, 0.030f);
				position.z = detail::attenuation((1.0f + cosf(cyclePositionZ * 2.0f * glm::pi<float>())) / 2.0f, 0.020f);

				rotation = (1.0f + (rotation - 0.5f) * cosf(cycleRotation * 2.0f * glm::pi<float>())) / 2.0f;
				position = (1.0f + (position - 0.5f) * cosf(cyclePosition * 2.0f * glm::pi<float>())) / 2.0f;

				skewRotation = qsf::EulerAngles::eulerToQuaternion(0.0f, (nullptr != movableComponent) ? getVelocity(*movableComponent) : mVelocitySkew, 0.0f);

				break;
			}

			case CARACCELERATION:
			{
				float acceleration = (nullptr != movableComponent) ? (movableComponent->getForwardAcceleration() * mVelocitySkew * 0.1f) : mVelocitySkew;

				// Smooth the skew a little
				float diff = acceleration - mOldSkew;
				float maxSkew = jobArguments.getSecondsPassed() * mVelocitySkew * ACCELERATION_SPEED * (diff > 0.0f ? 1.0f : -1.0f);
				acceleration = mOldSkew + (std::abs(diff) > std::abs(maxSkew) ? maxSkew : diff);
				mOldSkew = acceleration;

				skewRotation = qsf::EulerAngles::eulerToQuaternion(0.0f, acceleration, 0.0f);

				break;
			}
		}

		if (nullptr != movableComponent)
		{
			qsf::Transform offsetTransform;
			offsetTransform.setPositionAndRotation((position* 2.0f - 1.0f) * mAnimationIntensity, skewRotation * qsf::EulerAngles::eulerToQuaternion((rotation * 2.0f - 1.0f) * 0.01f * mAnimationIntensity));
			movableComponent->setTransformBias(offsetTransform);
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void SwingComponent::registerJobProxy()
	{
		{ // TODO(mk) The swing effect is broken. Linked entities are flickering between two different states. We can't give this out to player
			const UnitHelper unitHelper(getEntity());
			if (unitHelper.isFireFighterFLB() || unitHelper.isFireFighterBoat())
				return;
		}

		mCyclePassed = qsf::Random::getRandomFloat(0.0f, 1.0f);

		registerMovableObjectRenderingListener();
	}

	float SwingComponent::getVelocity(const qsf::MovableComponent& movableComponent) const
	{
		return glm::length(movableComponent.getSpeed()) * mVelocitySkew * 0.1f;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
