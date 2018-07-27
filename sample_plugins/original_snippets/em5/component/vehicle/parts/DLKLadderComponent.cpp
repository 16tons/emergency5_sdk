// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/vehicle/parts/DLKLadderComponent.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/audio/AudioHelper.h"

#include <qsf_game/component/base/BoneToBoneLinkComponent.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/map/Map.h>
#include <qsf/map/Entity.h>
#include <qsf/math/EulerAngles.h>
#include <qsf/math/Math.h>
#include <qsf/plugin/QsfJobs.h>


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
namespace
{
	namespace detail
	{


		//[-------------------------------------------------------]
		//[ Global functions                                      ]
		//[-------------------------------------------------------]
		inline float lerp(float f1, float f2, float a)
		{
			return f1 + (f2 - f1) * a;
		}

		inline float getAnimationOffsetFromValue(float value, float minValue, float maxValue)
		{
			if (value <= minValue)
				return 0.001f;		// TODO(tl): Hack to prevent the animation to restart from 1.0f

			if (value >= maxValue)
				return 0.999f;		// TODO(tl): Hack to prevent the animation to restart from 0.0f

			return lerp(0.001f, 0.999f, (value - minValue) / (maxValue - minValue));
		}

		qsf::Entity* getFirstChildEntity(qsf::Entity* entity)
		{
			if (nullptr == entity)
				return nullptr;

			const qsf::LinkComponent* linkComponent = entity->getComponent<qsf::LinkComponent>();
			if (nullptr == linkComponent)
				return nullptr;

			boost::container::flat_set<qsf::LinkComponent*>::const_iterator iterator = linkComponent->getChildLinks().begin();
			if (iterator == linkComponent->getChildLinks().end())
				return nullptr;

			return &((*iterator)->getEntity());
		}

		qsf::MeshAnimationChannel* getAnimationChannel(qsf::MeshAnimationComponent* meshAnimationComponent, const qsf::AssetProxy& proxy)
		{
			qsf::MeshAnimationChannel* result = nullptr;

			if (nullptr != meshAnimationComponent)
			{
				// Get animation channel of given animation or create the animation and return the channel
				if (meshAnimationComponent->isAnimationPlaying(proxy))
				{
					result = meshAnimationComponent->getAnimationChannel(proxy);
				}
				else
				{
					result = meshAnimationComponent->playAnimation(proxy, false, true);
					if (nullptr != result)
					{
						result->setSpeed(0.0f);
						result->setOffset(0.0f);
					}
				}
			}
			return result;
		}

		inline float limitBySpeed(float value, float speed, qsf::Time timePassed)
		{
			const float maxSpeed = speed * timePassed.getSeconds() * (value > 0.0f ? 1.0f : -1.0f);

			if (glm::abs(value) > glm::abs(maxSpeed))
				return maxSpeed;

			return value;
		}

		inline bool isZero(const float value, const float epsilon = glm::epsilon<float>())
		{
			return (glm::abs(value) < epsilon);
		}


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
	} // detail
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 DlkLadderComponent::COMPONENT_ID = qsf::StringHash("em5::DlkLadderComponent");

	const float DlkLadderComponent::YAW_EPSILON = 0.001f;
	const float DlkLadderComponent::ANIMATION_EPSILON = 0.001f;
	const glm::vec3 DlkLadderComponent::ARM_ORIENTATION = glm::vec3(0.0f, 0.0f, -1.0f);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	DlkLadderComponent::DlkLadderComponent(qsf::Prototype* prototype) :
		qsf::Component(prototype),
		mTimePassed(qsf::Time::ZERO),
		mLastAnimationState(STATE_NONE),
		mLadderBaseTransformComponent(nullptr),
		mStageBoneToBoneLinkComponent(nullptr),
		mLadderUpAnimation(nullptr),
		mLadderExtendAnimation(nullptr),
		mBaseUpAnimationChannel(nullptr),
		mExtendAnimationChannel(nullptr),
		mRotateYawAngle(0.0f),
		mLadderUpAnimationOffset(0.0f),
		mExtendAnimationOffset(0.0f),
		mVerticalRescueOffset(-1.0f),
		mVerticalDeployOffset(10.0f),
		mLadderMaxRotateSpeed(0.8f),
		mLadderMinRotateSpeed(0.3f),
		mLadderUpSpeed(0.12f),
		mLadderExtendSpeed(0.14f),
		mMaxAngle(1.3f),
		mMinLength(6.5f),
		mMaxLength(19.5f),
		mSynchronLength(8.0f),
		mTimeUltimatum(30.0f),
		mAnimationLadderBaseUp("em5/skeleton/default/em5_vs_laddertruck_ladder_base_up"),
		mAnimationLadderExtend("em5/skeleton/default/em5_vs_laddertruck_ladder_extend")
	{
		// Nothing to do in here
	}

	DlkLadderComponent::~DlkLadderComponent()
	{
		// Nothing to do in here
	}

	void DlkLadderComponent::alignLadder(const glm::vec3& position)
	{
		mLastAnimationState = STATE_ALIGN_LADDER;
		mPostition = position;

		// Little fine-tuning to send the ladder below the target
		mTargetPosition = position;
		mTargetPosition.y += mVerticalRescueOffset;

		if (!mAnimationJobProxy.isValid())
		{
			mTimePassed = qsf::Time::ZERO;
			mAnimationJobProxy.registerAt(qsf::QsfJobs::ANIMATION_GENERAL, boost::bind(&DlkLadderComponent::updateAnimation, this, _1));
			AudioHelper::playDLKLadderMove(mAudioProxy, getEntity());
		}
	}

	void DlkLadderComponent::deployLadder(const glm::vec3& position)
	{
		mLastAnimationState = STATE_DEPLOY_LADDER;
		mPostition = position;

		const qsf::TransformComponent& transformComponent = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();

		mTargetPosition = glm::mix(transformComponent.getPosition(), position, 0.2f);
		mTargetPosition.y += mVerticalDeployOffset;

		if (!mAnimationJobProxy.isValid())
		{
			mTimePassed = qsf::Time::ZERO;
			mAnimationJobProxy.registerAt(qsf::QsfJobs::ANIMATION_GENERAL, boost::bind(&DlkLadderComponent::updateAnimation, this, _1));
			AudioHelper::playDLKLadderMove(mAudioProxy, getEntity());
		}
	}

	void DlkLadderComponent::foldLadder()
	{
		mLastAnimationState = STATE_FOLD_LADDER;

		// We use the position of the DLK as target. Simple way to ensure a folded ladder.
		const qsf::TransformComponent& transformComponent = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();

		mTargetPosition = transformComponent.getPosition();

		if (!mAnimationJobProxy.isValid())
		{
			mTimePassed = qsf::Time::ZERO;
			mAnimationJobProxy.registerAt(qsf::QsfJobs::ANIMATION_GENERAL, boost::bind(&DlkLadderComponent::updateAnimation, this, _1));
			AudioHelper::playDLKLadderMove(mAudioProxy, getEntity());
		}
	}

	bool DlkLadderComponent::isAnimationPlaying() const
	{
		return mAnimationJobProxy.isValid();
	}

	qsf::Entity* DlkLadderComponent::getStageEntity() const
	{
		return (mStateEntity.valid() ? mStateEntity.get() : nullptr);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	void DlkLadderComponent::serialize(qsf::BinarySerializer& serializer)
	{
		//serializer.serialize(mTargetPosition);
		serializer.serialize(mTimePassed);

		serializer.serialize(mVerticalRescueOffset);
		serializer.serialize(mVerticalDeployOffset);
		serializer.serialize(mLadderMaxRotateSpeed);
		serializer.serialize(mLadderMinRotateSpeed);
		serializer.serialize(mLadderUpSpeed);
		serializer.serialize(mLadderExtendSpeed);
		serializer.serialize(mMaxAngle);
		serializer.serialize(mMinLength);
		serializer.serialize(mMaxLength);
		serializer.serialize(mSynchronLength);
		serializer.serialize(mTimeUltimatum);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	bool DlkLadderComponent::getComponentPointers()
	{
		const RoadVehicleComponent* roadVehicleComponent = getEntity().getComponent<RoadVehicleComponent>();
		if (nullptr == roadVehicleComponent)
			return false;

		qsf::Entity* ladderBaseEntity = getEntity().getMap().getEntityById(roadVehicleComponent->getSpecialEntity());
		if (nullptr == ladderBaseEntity)
			return false;

		mLadderBaseTransformComponent = ladderBaseEntity->getOrCreateComponent<qsf::TransformComponent>();
		if (nullptr == mLadderBaseTransformComponent)
			return false;

		qsf::Entity* ladderEntity = nullptr;
		{
			// Get the linked entity with the bone to bone link component
			const qsf::LinkComponent* linkComponent = ladderBaseEntity->getComponent<qsf::LinkComponent>();
			if (nullptr == linkComponent)
				return false;

			qsf::game::BoneToBoneLinkComponent* boneToBoneLinkComponent = linkComponent->getComponentFromEntityOrLinkedChild<qsf::game::BoneToBoneLinkComponent>();
			if (nullptr != boneToBoneLinkComponent)
			{
				ladderEntity = &boneToBoneLinkComponent->getEntity();
			}
		}

		if (nullptr == ladderEntity)
			return false;

		// The entity with the basket and the water guns
		mStateEntity = detail::getFirstChildEntity(ladderEntity);
		if (!mStateEntity.valid())
			return false;

		mLadderUpAnimation = ladderBaseEntity->getOrCreateComponent<qsf::MeshAnimationComponent>();
		if (nullptr == mLadderUpAnimation)
			return false;

		mLadderExtendAnimation = ladderEntity->getOrCreateComponent<qsf::MeshAnimationComponent>();
		if (nullptr == mLadderExtendAnimation)
			return false;

		mStageBoneToBoneLinkComponent = mStateEntity->getComponent<qsf::game::BoneToBoneLinkComponent>();
		if (nullptr == mStageBoneToBoneLinkComponent)
			return false;

		mBaseUpAnimationChannel = detail::getAnimationChannel(mLadderUpAnimation, mAnimationLadderBaseUp);
		if (nullptr == mBaseUpAnimationChannel)
			return false;

		mExtendAnimationChannel = detail::getAnimationChannel(mLadderExtendAnimation, mAnimationLadderExtend);
		if (nullptr == mExtendAnimationChannel)
			return false;

		return true;
	}

	void DlkLadderComponent::calculateAngleAndOffsets()
	{
		// Calculate the direction and length to the target
		const glm::vec3 targetDirection = glm::normalize((mTargetPosition - mLadderBaseTransformComponent->getPosition()));
		const glm::vec3 localTargetDirection = mLadderBaseTransformComponent->getTransform().vec3DirectionWorldToLocal(targetDirection);
		const float targetLength = glm::length(mTargetPosition - mLadderBaseTransformComponent->getPosition());

		// Calculate the yaw-rotation and animation-offset we still need to move
		glm::vec3 rotate = qsf::EulerAngles::quaternionToEuler(qsf::Math::getLookAtQuaternion(localTargetDirection, qsf::CoordinateSystem::getUp()));
		mRotateYawAngle = rotate.x;
		mLadderUpAnimationOffset = detail::getAnimationOffsetFromValue(rotate.y, 0.0f, mMaxAngle) - mBaseUpAnimationChannel->getOffset();
		mExtendAnimationOffset = detail::getAnimationOffsetFromValue(targetLength, mMinLength, mMaxLength) - mExtendAnimationChannel->getOffset();
	}

	bool DlkLadderComponent::isLadderNotAligned()
	{
		if (!getComponentPointers())		// We stop aligning if there is a missing component
			return false;

		calculateAngleAndOffsets();

		if (mTimePassed.getSeconds() > mTimeUltimatum || (
			detail::isZero(mRotateYawAngle, YAW_EPSILON) &&
			detail::isZero(mLadderUpAnimationOffset, ANIMATION_EPSILON) &&
			detail::isZero(mExtendAnimationOffset, ANIMATION_EPSILON)))
		{
			// Stop animation
			mAnimationJobProxy.unregister();

			mLastAnimationState = STATE_NONE;

			// Play the klonk
			AudioProxy audioProxy;
			AudioHelper::playDLKLadderKlonk(audioProxy, getEntity());
			audioProxy.detach();

			// Stop audio
			mAudioProxy.stop();

			// Try to jump to a quick animation solution if animation took longer then mTimeUltimatum
			if (mTimePassed.getSeconds() > mTimeUltimatum)
			{
				mLadderBaseTransformComponent->setRotation(mLadderBaseTransformComponent->getRotation() * qsf::EulerAngles::eulerToQuaternion(mRotateYawAngle, 0.0f, 0.0f));
				mBaseUpAnimationChannel->setOffset(mBaseUpAnimationChannel->getOffset() + mLadderUpAnimationOffset);
				mExtendAnimationChannel->setOffset(mExtendAnimationChannel->getOffset() + mExtendAnimationOffset);
				mStageBoneToBoneLinkComponent->setLocalRotation(qsf::EulerAngles::eulerToQuaternion(0.0f, -mMaxAngle * mBaseUpAnimationChannel->getOffset(), 0.0f));
			}

			// Delete animations we don't need anymore
			if (detail::isZero(mBaseUpAnimationChannel->getOffset(), ANIMATION_EPSILON))
			{
				mLadderUpAnimation->stopAllAnimations();
			}
			if (detail::isZero(mExtendAnimationChannel->getOffset(), ANIMATION_EPSILON))
			{
				mLadderExtendAnimation->stopAllAnimations();
			}
			return false;
		}
		return true;
	}

	void DlkLadderComponent::updateAnimation(const qsf::JobArguments& jobArguments)
	{
		mTimePassed += jobArguments.getTimePassed();

		if (isLadderNotAligned())
		{
			// Calculate new values
			const bool isExtending = mExtendAnimationOffset > 0.0f;
			const float newRotateYaw = detail::limitBySpeed(mRotateYawAngle, mLadderMinRotateSpeed
				+ mExtendAnimationOffset * (mLadderMaxRotateSpeed - mLadderMinRotateSpeed), jobArguments.getTimePassed());
			const float newLadderUpAnimationOffset = mBaseUpAnimationChannel->getOffset() + detail::limitBySpeed(mLadderUpAnimationOffset, mLadderUpSpeed, jobArguments.getTimePassed());
			const float newExtendAnimationOffset = mExtendAnimationChannel->getOffset() + detail::limitBySpeed(mExtendAnimationOffset, mLadderExtendSpeed, jobArguments.getTimePassed());

			if (!detail::isZero(mRotateYawAngle, YAW_EPSILON))
			{
				// If the ladder is folded back we only rotate after the extension is under a certain value
				if (isExtending || isAnimationSynchron(newExtendAnimationOffset))
				{
					mLadderBaseTransformComponent->setRotation(mLadderBaseTransformComponent->getRotation() * qsf::EulerAngles::eulerToQuaternion(newRotateYaw, 0.0f, 0.0f));
				}
			}
			if (!detail::isZero(mExtendAnimationOffset, ANIMATION_EPSILON))
			{
				// If the ladder is extended we only move over a certain value after rotate is finished
				if (!isExtending || isAnimationSynchron(newExtendAnimationOffset) || (detail::isZero(mRotateYawAngle, YAW_EPSILON) && detail::isZero(mLadderUpAnimationOffset, ANIMATION_EPSILON)))
				{
					mExtendAnimationChannel->setOffset(newExtendAnimationOffset);
				}
			}
			if (!detail::isZero(mLadderUpAnimationOffset, ANIMATION_EPSILON))
			{
				// If the ladder is folded back we change the hight only after the extension is under a certain value
				if (isExtending || isAnimationSynchron(newExtendAnimationOffset))
				{
					mBaseUpAnimationChannel->setOffset(newLadderUpAnimationOffset);
				}
			}
			// Stage needs to be readjusted
			mStageBoneToBoneLinkComponent->setLocalRotation(qsf::EulerAngles::eulerToQuaternion(0.0f, -mMaxAngle * mBaseUpAnimationChannel->getOffset(), 0.0f));
		}
	}

	float DlkLadderComponent::isAnimationSynchron(float extendOffset) const
	{
		return extendOffset < (mSynchronLength - mMinLength) / (mMaxLength - mMinLength);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
