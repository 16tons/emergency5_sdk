// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/vehicle/parts/VehicleCraneComponent.h"
#include "em5/audio/AudioHelper.h"
#include "em5/game/animation/AnimationHelper.h"

#include <qsf_game/component/base/BoneToBoneLinkComponent.h>
#include <qsf_game/component/base/HiddenComponent.h>

#include <qsf/component/base/MetadataComponent.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>
#include <qsf/math/CoordinateSystem.h>
#include <qsf/math/EulerAngles.h>
#include <qsf/math/Math.h>
#include <qsf/renderer/animation/MeshAnimationChannel.h>
#include <qsf/renderer/mesh/MeshComponent.h>
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

		qsf::MeshAnimationChannel* getAnimationChannel(qsf::MeshAnimationComponent* meshAnimationComponent, const qsf::AssetProxy& proxy, float startOffset = 0.0f)
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
						result->setOffset(startOffset);
					}
				}
			}
			return result;
		}

		qsf::MeshAnimationChannel* getBlendAnimationChannel(qsf::MeshAnimationComponent* meshAnimationComponent, const qsf::AssetProxy& proxy)
		{
			qsf::MeshAnimationChannel* result = nullptr;

			if (nullptr != meshAnimationComponent)			// Get animation channel of given animation or create the animation and return the channel
			{
				if (meshAnimationComponent->isAnimationPlaying(proxy))
				{
					result = meshAnimationComponent->getAnimationChannel(proxy);
				}
				else
				{
					// TODO(fw): blendToMaskedAnimation returns a pair of channels now - consider this if the out-commented code gets used again
					//result = meshAnimationComponent->blendToMaskedAnimation(
						//playAnimation(proxy, false, true);
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

		inline float getOffset(float fmin, float fmax, float fx)
		{
			if (fmax - fmin == 0.0f)
				return 0.0f;

			return fmodf ((fx - fmin) / (fmax - fmin), 1.0f);
		}

		inline float distance(const glm::vec3& from, const glm::vec3& to)
		{
			return glm::length(to - from);
		}

		inline glm::vec3 moveByDistance(const glm::vec3& from, const glm::vec3& to, float distance)
		{
			return from + distance * glm::normalize(to - from);
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
	const uint32 VehicleCraneComponent::COMPONENT_ID = qsf::StringHash("em5::VehicleCraneComponent");

	const float VehicleCraneComponent::YAW_EPSILON = 0.001f;
	const float VehicleCraneComponent::ANIMATION_EPSILON = 0.001f;
	const float VehicleCraneComponent::HOOK_DISTANCE_EPSILON = 0.001f;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	void VehicleCraneComponent::setCraneEntityId(uint64 craneEntityId)
	{
		mCraneEntityId = craneEntityId;
	}

	bool VehicleCraneComponent::checkTargetPosition(const glm::vec3& position, glm::vec3& correctionOffset)
	{
		correctionOffset = glm::vec3(0.0f, 0.0f, 0.0f);
		if (getComponentPointers())
		{
			const glm::vec3 pivotPosition = mCraneTransformComponent->getPosition() + glm::vec3(0.0f, mCraneHeight, 0.0f);
			const float targetLength = glm::distance(position, pivotPosition);
			if (targetLength > mMaxLength)
			{
				correctionOffset = pivotPosition + glm::normalize(position - pivotPosition) * mMaxLength - position;
				return true;
			}
		}
		return false;
	}

	void VehicleCraneComponent::alignArm(const glm::vec3& position)
	{
		mLastAnimationState = STATE_ALIGN_ARM;

		mTargetPosition = position;

		startAnimation();
	}

	void VehicleCraneComponent::foldArm()
	{
		mLastAnimationState = STATE_FOLD_ARM;

		const qsf::TransformComponent& transformComponent = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();

		if (mCraneType == TOW_CAR || mCraneType == BOAT_TRANSPORTER)
		{
			mTargetPosition = worldSpacePositionWithLocalOffset(transformComponent, glm::vec3(0.0f, 2.2f, 1.0f));
		}
		else if (mCraneType == HEAVY_CRANE)
		{
			mTargetPosition = worldSpacePositionWithLocalOffset(transformComponent, glm::vec3(0.0f, 0.0f, 0.0f));
		}

		startAnimation();
	}

	void VehicleCraneComponent::loadCargoArm()
	{
		mLastAnimationState = STATE_LOAD_CARGO_ARM;

		const qsf::TransformComponent& transformComponent = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();

		mTargetPosition = worldSpacePositionWithLocalOffset(transformComponent, glm::vec3(0.0f, 3.2f, 1.0f));

		startAnimation();
	}

	void VehicleCraneComponent::hookMove(const glm::vec3& targetPosition, float distancePercentage)
	{
		mLastAnimationState = STATE_HOOK_MOVE;
		mTargetPosition = targetPosition;
		mDistancePercentage = distancePercentage;

		startHookAnimation();
	}

	void VehicleCraneComponent::hookMoveBack(float distancePercentage)
	{
		if (!getHookDummy())
			return;

		qsf::TransformComponent& dummyTransformComponent = mHookDummy->getOrCreateComponentSafe<qsf::TransformComponent>();

		mLastAnimationState = STATE_HOOK_MOVE_BACK;
		mTargetPosition = dummyTransformComponent.getPosition();
		mDistancePercentage = distancePercentage;

		startHookAnimation();
	}

	void VehicleCraneComponent::setVisibleStrap(bool visible)
	{
		mStrapVisible = visible;
		qsf::Entity* craneEntity = getEntity().getMap().getEntityById(mCraneEntityId);
		qsf::Entity* jibEntity = getFirstChildEntity(craneEntity);
		qsf::Entity* stageEntity = getFirstChildEntity(jibEntity);
		qsf::Entity* strapEntity = getFirstChildEntity(stageEntity);

		if (nullptr != strapEntity)
		{
			qsf::game::HiddenComponent::setHidden(*strapEntity, !visible);

			// TODO(ca) Do we really need the following lines?
			if (visible)	// Check if meshComponent is inactive
			{
				qsf::MeshComponent* meshComponent = strapEntity->getOrCreateComponent<qsf::MeshComponent>();
				if (nullptr != meshComponent)
				{
					meshComponent->setActive(true);
				}
			}
		}
	}

	void VehicleCraneComponent::linkTargetToHook(qsf::Entity& targetEntity)
	{
		mEntityIdLinkedToHook = targetEntity.getId();

		qsf::Entity* dummyEntity = getFirstChildEntityWithTag(mCraneEntity, "dummy");
		qsf::LinkComponent& targetLinkComponent = targetEntity.getOrCreateComponentSafe<qsf::LinkComponent>();

		if (nullptr != dummyEntity)
		{
			targetLinkComponent.setParentId(dummyEntity->getId());
			targetLinkComponent.setParentLinkType(qsf::LinkComponent::HARD_LINK);
		}
	}

	void VehicleCraneComponent::unlinkTargetFromHook()
	{
		mEntityIdLinkedToHook = qsf::getUninitialized<uint64>();

		qsf::Entity* dummyEntity = getFirstChildEntityWithTag(mCraneEntity, "dummy");
		qsf::Entity* targetEntity  = getFirstChildEntity(dummyEntity);

		if (nullptr != targetEntity)
		{
			qsf::LinkComponent& targetLinkComponent = targetEntity->getOrCreateComponentSafe<qsf::LinkComponent>();
			targetLinkComponent.setParentId(qsf::getUninitialized<uint64>());
		}

		AudioHelper::playCraneDeployObject(mHookAudioProxy, getEntity());
	}

	qsf::Entity* VehicleCraneComponent::getHookEntity() const
	{
		if (mCraneType != HEAVY_CRANE)
			return nullptr;

		VehicleCraneComponent* vehicleCraneComponent = getEntity().getComponent<VehicleCraneComponent>();
		if (nullptr == vehicleCraneComponent)
			return nullptr;

		qsf::Entity* craneEntity = getEntity().getMap().getEntityById(vehicleCraneComponent->getCraneEntityId());
		qsf::Entity* jibEntity = getFirstChildEntityWithTag(craneEntity, "jib");
		qsf::Entity* hookEntity = getFirstChildEntityWithTag(jibEntity, "hook");

		return hookEntity;
	}

	bool VehicleCraneComponent::isAnimationPlaying() const
	{
		return (mAnimationJobProxy.isValid() || mHookAnimationJobProxy.isValid());
	}

	void VehicleCraneComponent::stopAllAnimation()
	{
		if (!getComponentPointers())	// We stop aligning if there is a missing component
			return;

		mLastAnimationState = STATE_STOP_ALL_ANIMATION;

		mCraneMeshAnimationComponent->stopAllAnimations();
		mJibMeshAnimationComponent->stopAllAnimations();

		mCraneMeshAnimationComponent = nullptr;
		mJibMeshAnimationComponent = nullptr;
		mExtendAnimationChannel = nullptr;
		mLowToHighAnimationChannel = nullptr;

		mCraneEntity->destroyComponent<qsf::MeshAnimationComponent>();
		mJibEntity->destroyComponent<qsf::MeshAnimationComponent>();

		if (mCraneType == TOW_CAR || mCraneType == BOAT_TRANSPORTER)
		{
			mStageBoneToBoneLinkComponent->setLocalRotation(glm::inverse(mCraneLinkComponent->getLocalRotation()));
		}
		else if (mCraneType == HEAVY_CRANE)
		{
			mCockpitMeshAnimationComponent->stopAllAnimations();
			mCockpitAnimationChannel = nullptr;
			mCockpitEntity->destroyComponent<qsf::MeshAnimationComponent>();

			mStageBoneToBoneLinkComponent->setLocalRotation(glm::inverse(mCraneLinkComponent->getLocalRotation()));

			qsf::Entity* hookEntity = getHookEntity();
			if (nullptr != hookEntity)
			{
				AnimationHelper(*hookEntity).clearAnimation();
			}
		}
	}

	glm::vec3 VehicleCraneComponent::worldSpacePositionWithLocalOffset(const qsf::TransformComponent& transformComponent, const glm::vec3& localoffset)
	{
		return transformComponent.getPosition() + transformComponent.getRotation() * localoffset;
	}

	void VehicleCraneComponent::setSequenceType(SequenceType type)
	{
		if (mSequenceType != type)
		{
			mSequenceType = type;
			mSequence.clear();

			switch (type)
			{
				case SequenceType::SEQUENCE_PICKUP_BY_TOWCAR_ALIGN_ARM:
				{
					mSequence.reserve(4);
					mSequence.emplace_back(VehicleCraneComponent::PAUSE);
					mSequence.emplace_back(VehicleCraneComponent::YAW_MOVEMENT);
					mSequence.emplace_back(VehicleCraneComponent::PAUSE);
					mSequence.emplace_back(VehicleCraneComponent::PITCH_AND_EXTEND_MOVEMENT);
					break;
				}
				case SequenceType::SEQUENCE_PICKUP_BY_TOWCAR_LOAD_CARGO_ARM:
				{
					mSequence.reserve(4);
					mSequence.emplace_back(VehicleCraneComponent::PAUSE);
					mSequence.emplace_back(VehicleCraneComponent::PITCH_MOVEMENT);
					mSequence.emplace_back(VehicleCraneComponent::PAUSE);
					mSequence.emplace_back(VehicleCraneComponent::YAW_AND_EXTEND_MOVEMENT);
					break;
				}
				case SequenceType::SEQUENCE_LIFT_BY_CRANE_ALIGN_ARM:
				{
					mCockpitWaitTime = qsf::Time::fromSeconds(1.0f);
					mSequence.reserve(7);
					mSequence.emplace_back(VehicleCraneComponent::PITCH_AND_COCKPIT_MOVEMENT);
					mSequence.emplace_back(VehicleCraneComponent::PAUSE);
					mSequence.emplace_back(VehicleCraneComponent::YAW_MOVEMENT);
					mSequence.emplace_back(VehicleCraneComponent::PAUSE);
					mSequence.emplace_back(VehicleCraneComponent::EXTEND_MOVEMENT);

					// Release hook
					qsf::Entity* hookEntity = getHookEntity();
					if (nullptr != hookEntity)
					{
						AnimationHelper(*hookEntity).playAnimation(getAnimationToUseForCraneHook(), false, false);
					}
					break;
				}
				case SequenceType::SEQUENCE_LIFT_BY_CRANE_FOLD_ARM:
				{
					mCockpitWaitTime = qsf::Time::fromSeconds(1.0f);
					mSequence.reserve(6);
					mSequence.emplace_back(VehicleCraneComponent::PAUSE);
					mSequence.emplace_back(VehicleCraneComponent::EXTEND_MOVEMENT);
					mSequence.emplace_back(VehicleCraneComponent::PAUSE);
					mSequence.emplace_back(VehicleCraneComponent::YAW_MOVEMENT);
					mSequence.emplace_back(VehicleCraneComponent::PAUSE);
					mSequence.emplace_back(VehicleCraneComponent::PITCH_AND_COCKPIT_MOVEMENT);

					qsf::Entity* hookEntity = getHookEntity();
					if (nullptr != hookEntity)
					{
						qsf::MeshAnimationChannel* meshAnimationChannel = AnimationHelper(*hookEntity).playAnimation(getAnimationToUseForCraneHook(), false, false);
						if (nullptr != meshAnimationChannel)
						{
							meshAnimationChannel->setSpeed(0.0f);
							meshAnimationChannel->setOffset(1.0f);
						}
					}
					break;
				}
				case SequenceType::SEQUENCE_UNLOAD_BOAT_ALIGN_ARM:
				{
					mSequence.reserve(4);
					mSequence.emplace_back(VehicleCraneComponent::PAUSE);
					mSequence.emplace_back(VehicleCraneComponent::YAW_AND_EXTEND_MOVEMENT);
					mSequence.emplace_back(VehicleCraneComponent::PAUSE);
					mSequence.emplace_back(VehicleCraneComponent::PITCH_MOVEMENT);
					break;
				}
				case SequenceType::SEQUENCE_UNLOAD_BOAT_FOLD_ARM:
				{
					mSequence.reserve(4);
					mSequence.emplace_back(VehicleCraneComponent::PAUSE);
					mSequence.emplace_back(VehicleCraneComponent::PITCH_MOVEMENT);
					mSequence.emplace_back(VehicleCraneComponent::PAUSE);
					mSequence.emplace_back(VehicleCraneComponent::YAW_AND_EXTEND_MOVEMENT);
					break;
				}
				case SequenceType::SEQUENCE_LOAD_BOAT_ALIGN_ARM:
				{
					mSequence.reserve(4);
					mSequence.emplace_back(VehicleCraneComponent::PAUSE);
					mSequence.emplace_back(VehicleCraneComponent::YAW_AND_PITCH_MOVEMENT);
					mSequence.emplace_back(VehicleCraneComponent::PAUSE);
					mSequence.emplace_back(VehicleCraneComponent::EXTEND_MOVEMENT);
					break;
				}
				case SequenceType::SEQUENCE_LOAD_BOAT_LOAD_CARGO_ARM:
				{
					mSequence.reserve(4);
					mSequence.emplace_back(VehicleCraneComponent::PAUSE);
					mSequence.emplace_back(VehicleCraneComponent::PITCH_MOVEMENT);
					mSequence.emplace_back(VehicleCraneComponent::PAUSE);
					mSequence.emplace_back(VehicleCraneComponent::YAW_AND_EXTEND_MOVEMENT);
					break;
				}

				case SequenceType::SEQUENCE_NONE:
					// Nothing to do in this case
					break;
			}
		}
	}

	void VehicleCraneComponent::deleteSequence()
	{
		mSequence.clear();
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	void VehicleCraneComponent::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mCraneEntityId);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	void VehicleCraneComponent::onShutdown()
	{
		// In case of errors, cleanup the created entities
		if (mHookDummy)
		{
			getEntity().getMap().destroyEntityById(mHookDummy->getId());
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void VehicleCraneComponent::startAnimation()
	{
		if (!mAnimationJobProxy.isValid())
		{
			mTimePassed = qsf::Time::ZERO;
			mPauseTimePassed = qsf::Time::ZERO;
			mSequenceState = 0;
			mAnimationJobProxy.registerAt(qsf::QsfJobs::ANIMATION_GENERAL, boost::bind(&VehicleCraneComponent::updateAnimation, this, _1));

			// Start Sound
			if (mCraneType == TOW_CAR || mCraneType == BOAT_TRANSPORTER)
			{
				AudioHelper::playTowCraneMove(mAudioProxy, getEntity());
			}
			else if (mCraneType == HEAVY_CRANE)
			{
				AudioHelper::playCraneMove(mAudioProxy, getEntity());
			}
		}

		// Make sure hook animation job proxy is disabled when we move the arm
		mHookAnimationJobProxy.unregister();
	}

	void VehicleCraneComponent::clearAnimation()
	{
		mAudioProxy.stop();
		mCockpitAudioProxy.stop();
		mAnimationJobProxy.unregister();

		if (mCraneType == HEAVY_CRANE && mLastAnimationState == STATE_FOLD_ARM)
		{
			// Secure hook
			qsf::Entity* hookEntity = getHookEntity();
			if (nullptr != hookEntity)
			{
				AnimationHelper animationHelper = AnimationHelper(*hookEntity);
				animationHelper.clearAnimation();
				animationHelper.playAnimation(getAnimationToUseForCraneHook(), false, true);
			}
		}
	}

	void VehicleCraneComponent::startHookAnimation()
	{
		if (!mHookAnimationJobProxy.isValid())
		{
			mTimePassed = qsf::Time::ZERO;
			mPauseTimePassed = qsf::Time::ZERO;
			mSequenceState = 0;
			mHookAnimationJobProxy.registerAt(qsf::QsfJobs::ANIMATION_GENERAL, boost::bind(&VehicleCraneComponent::updateHookAnimation, this, _1));

			// Start Sound
			AudioHelper::playCraneHook(mHookAudioProxy, getEntity());
		}

		// Make sure animation job proxy is disabled when we move the hook
		mAnimationJobProxy.unregister();
	}

	bool VehicleCraneComponent::getComponentPointers()
	{
		mCraneEntity = getEntity().getMap().getEntityById(mCraneEntityId);
		if (nullptr == mCraneEntity)
			return false;

		mCraneTransformComponent = mCraneEntity->getOrCreateComponent<qsf::TransformComponent>();
		if (nullptr == mCraneTransformComponent)
			return false;

		mCraneLinkComponent = mCraneEntity->getComponent<qsf::LinkComponent>();
		if (nullptr == mCraneLinkComponent)
			return false;

		mCraneMeshAnimationComponent = mCraneEntity->getOrCreateComponent<qsf::MeshAnimationComponent>();
		if (nullptr == mCraneMeshAnimationComponent)
			return false;
		mCraneMeshAnimationComponent->setDoAlwaysUpdateAnimation(true);	// Avoid glitches when not looking while animation is playing

		mJibEntity = getFirstChildEntityWithTag(mCraneEntity, "jib");
		if (nullptr == mJibEntity)
			return false;

		mJibMeshAnimationComponent = mJibEntity->getOrCreateComponent<qsf::MeshAnimationComponent>();
		if (nullptr == mJibMeshAnimationComponent)
			return false;
		mJibMeshAnimationComponent->setDoAlwaysUpdateAnimation(true);	// Avoid glitches when not looking while animation is playing

		qsf::AssetProxy extendAsset = getAnimationToUseForCraneExtend();
		qsf::AssetProxy lowToHighAsset = getAnimationToUseForCraneLowToHigh();

		if (mCraneType == TOW_CAR || mCraneType == BOAT_TRANSPORTER)
		{
			mExtendAnimationChannel = detail::getAnimationChannel(mJibMeshAnimationComponent, extendAsset);
			if (nullptr == mExtendAnimationChannel)
				return false;

			mLowToHighAnimationChannel = detail::getAnimationChannel(mCraneMeshAnimationComponent, lowToHighAsset, detail::getOffset(mMinAngle, mMaxAngle, 0.0f));
			if (nullptr == mLowToHighAnimationChannel)
				return false;

			qsf::Entity* stageEntity = getFirstChildEntity(mJibEntity);
			if (nullptr == stageEntity)
				return false;

			mStageBoneToBoneLinkComponent = stageEntity->getComponent<qsf::game::BoneToBoneLinkComponent>();
			if (nullptr == mStageBoneToBoneLinkComponent)
				return false;
		}
		else if (mCraneType == HEAVY_CRANE)
		{
			qsf::AssetProxy cockpitAsset = getAnimationToUseForCraneCockpit();

			mCockpitEntity = getFirstChildEntityWithTag(mCraneEntity, "cockpit");
			if (nullptr == mCockpitEntity)
				return false;

			mCockpitMeshAnimationComponent = mCockpitEntity->getOrCreateComponent<qsf::MeshAnimationComponent>();
			if (nullptr == mCockpitMeshAnimationComponent)
				return false;
			mCockpitMeshAnimationComponent->setDoAlwaysUpdateAnimation(true);	// Avoid glitches when not looking while animation is playing

			mExtendAnimationChannel = detail::getAnimationChannel(mJibMeshAnimationComponent, extendAsset);
			if (nullptr == mExtendAnimationChannel)
				return false;

			mLowToHighAnimationChannel = detail::getAnimationChannel(mCraneMeshAnimationComponent, lowToHighAsset, detail::getOffset(mMinAngle, mMaxAngle, 0.0f));
			if (nullptr == mLowToHighAnimationChannel)
				return false;

			mCockpitAnimationChannel = detail::getAnimationChannel(mCockpitMeshAnimationComponent, cockpitAsset, 0.0f);
			if (nullptr == mCockpitAnimationChannel)
				return false;

			qsf::Entity* hookEntity = getFirstChildEntityWithTag(mJibEntity, "hook");
			if (nullptr == hookEntity)
				return false;

			// StageBoneToBoneLink doubleused as hookBoneToBoneLink, but used almost the same
			mStageBoneToBoneLinkComponent = hookEntity->getComponent<qsf::game::BoneToBoneLinkComponent>();
			if (nullptr == mStageBoneToBoneLinkComponent)
				return false;
		}
		return true;
	}

	void VehicleCraneComponent::calculateAngleAndOffsets()
	{
		// Calculate the direction and length to the target
		const glm::vec3 pivotPosition = mCraneTransformComponent->getPosition() + glm::vec3(0.0f, mCraneHeight, 0.0f);
		const glm::vec3 targetDirection = glm::normalize(mTargetPosition - pivotPosition);
		glm::vec3 localTargetDirection = mCraneTransformComponent->getTransform().vec3DirectionWorldToLocal(targetDirection);
		const float targetLength = glm::length(mTargetPosition - pivotPosition);

		// Calculate the yaw-rotation and animation-offset we still need to move
		if (mCraneType == TOW_CAR || mCraneType == BOAT_TRANSPORTER)
		{
			localTargetDirection.x *= -1.0f;	// Towcar and boat transporter crane looks in the opposite direction
			localTargetDirection.z *= -1.0f;
		}
		glm::vec3 rotate = qsf::EulerAngles::quaternionToEuler(qsf::Math::getLookAtQuaternion(localTargetDirection, qsf::CoordinateSystem::getUp()));
		mRotateYawAngle = rotate.x;

		mExtendAnimationOffset = detail::getAnimationOffsetFromValue(targetLength, mMinLength, mMaxLength) - mExtendAnimationChannel->getOffset();
		mLowToHighAnimationOffset = detail::getAnimationOffsetFromValue(rotate.y, mMinAngle, mMaxAngle) - mLowToHighAnimationChannel->getOffset();

		// Hardcoded angles for cockpit
		mCockpitAnimationOffset = mCraneType != HEAVY_CRANE ? 0.0f : detail::getAnimationOffsetFromValue(rotate.y, 0.2f, 0.8f) - mCockpitAnimationChannel->getOffset();
	}

	bool VehicleCraneComponent::isNotAligned()
	{
		if (!getComponentPointers())		// We stop aligning if there is a missing component
		{
			clearAnimation();

			return false;
		}

		calculateAngleAndOffsets();

		if (mTimePassed.getSeconds() > mTimeUltimatum || (
				detail::isZero(mRotateYawAngle, YAW_EPSILON) &&
				detail::isZero(mExtendAnimationOffset, ANIMATION_EPSILON) &&
				detail::isZero(mLowToHighAnimationOffset, ANIMATION_EPSILON) &&
				detail::isZero(mCockpitAnimationOffset, ANIMATION_EPSILON)))
		{
			clearAnimation();

			// Try to jump to a quick animation solution if animation took longer then mTimeUltimatum
			if (mTimePassed.getSeconds() > mTimeUltimatum)
			{
				mCraneTransformComponent->setRotation(mCraneTransformComponent->getRotation() * qsf::EulerAngles::eulerToQuaternion(mRotateYawAngle, 0.0f, 0.0f));
				mExtendAnimationChannel->setOffset(mExtendAnimationChannel->getOffset() + mExtendAnimationOffset);
				mLowToHighAnimationChannel->setOffset(mLowToHighAnimationChannel->getOffset() + mLowToHighAnimationOffset);
				if (mCraneType == HEAVY_CRANE)
				{
					mCockpitMeshAnimationComponent->stopAllAnimations();
				}
				alignStage();
			}

			// Delete animations we don't need anymore
			if (detail::isZero(mExtendAnimationChannel->getOffset(), ANIMATION_EPSILON) &&
				detail::isZero(mLowToHighAnimationChannel->getOffset() - detail::getOffset(mMinAngle, mMaxAngle, 0.0f), ANIMATION_EPSILON))
			{
				mCraneMeshAnimationComponent->stopAllAnimations();
				mJibMeshAnimationComponent->stopAllAnimations();

				if (mCraneType == HEAVY_CRANE)
				{
					mCockpitMeshAnimationComponent->stopAllAnimations();
				}
			}
			return false;
		}
		return true;
	}

	void VehicleCraneComponent::updateAnimation(const qsf::JobArguments& jobArguments)
	{
		const qsf::Time timeElapsed = jobArguments.getTimePassed();
		mTimePassed += timeElapsed;

		if (isNotAligned())
		{
			const float newRotateYaw = detail::limitBySpeed(mRotateYawAngle, mArmRotateSpeed, timeElapsed);
			const float newExtendAnimationOffset = mExtendAnimationChannel->getOffset() + detail::limitBySpeed(mExtendAnimationOffset, mExtendSpeed, timeElapsed);
			const float newLowToHighAnimationOffset = mLowToHighAnimationChannel->getOffset() + detail::limitBySpeed(mLowToHighAnimationOffset, mLowToHighSpeed, timeElapsed);
			float newCockpitAnimationOffset = 0.0f;

			if (mCraneType == HEAVY_CRANE)
			{
				if (mCockpitWaitTime > qsf::Time::ZERO)
				{
					mCockpitWaitTime = std::max(qsf::Time::ZERO, mCockpitWaitTime - timeElapsed);
				}
				else
				{
					newCockpitAnimationOffset = mCockpitAnimationChannel->getOffset() + detail::limitBySpeed(mCockpitAnimationOffset, mCockpitSpeed, timeElapsed);
				}
			}

			if (mSequenceState >= mSequence.size())  // Always true for empty sequence
			{
				if (!detail::isZero(mRotateYawAngle, YAW_EPSILON))
				{
					mCraneTransformComponent->setRotation(mCraneTransformComponent->getRotation() * qsf::EulerAngles::eulerToQuaternion(newRotateYaw, 0.0f, 0.0f));
				}
				if (!detail::isZero(mExtendAnimationOffset, ANIMATION_EPSILON))
				{
					mExtendAnimationChannel->setOffset(newExtendAnimationOffset);
				}
				if (!detail::isZero(mLowToHighAnimationOffset, ANIMATION_EPSILON))
				{
					mLowToHighAnimationChannel->setOffset(newLowToHighAnimationOffset);
				}
				if (mCraneType == HEAVY_CRANE && !detail::isZero(mCockpitAnimationOffset, ANIMATION_EPSILON))
				{
					mCockpitAnimationChannel->setOffset(newCockpitAnimationOffset);
				}
			}
			else
			{
				PivotMovement state = mSequence[mSequenceState];

				switch (state)
				{
					case PAUSE:
					{
						// Pause Audio
						if (mAudioProxy.isPlaying())
						{
							mAudioProxy.stop();
						}

						mPauseTimePassed += timeElapsed;

						if (mPauseTimePassed > mPauseTime)
						{
							mAudioProxy.restart();
							mPauseTimePassed = qsf::Time::ZERO;
							mSequenceState++;
						}
						break;
					}
					case YAW_MOVEMENT:
					{
						const bool yawZero = detail::isZero(mRotateYawAngle, YAW_EPSILON);
						if (yawZero)
						{
							mSequenceState++;
						}
						else
						{
							mCraneTransformComponent->setRotation(mCraneTransformComponent->getRotation() * qsf::EulerAngles::eulerToQuaternion(newRotateYaw, 0.0f, 0.0f));
						}
						break;
					}
					case PITCH_MOVEMENT:
					{
						const bool pitchZero = detail::isZero(mLowToHighAnimationOffset, ANIMATION_EPSILON);
						if (pitchZero)
						{
							mSequenceState++;
						}
						else
						{
							mLowToHighAnimationChannel->setOffset(newLowToHighAnimationOffset);
						}
						break;
					}
					case EXTEND_MOVEMENT:
					{
						const bool extendZero = detail::isZero(mExtendAnimationOffset, ANIMATION_EPSILON);
						if (extendZero)
						{
							mSequenceState++;
						}
						else
						{
							mExtendAnimationChannel->setOffset(newExtendAnimationOffset);
						}
						break;
					}
					case YAW_AND_PITCH_MOVEMENT:
					{
						const bool yawZero = detail::isZero(mRotateYawAngle, YAW_EPSILON);
						const bool pitchZero = detail::isZero(mLowToHighAnimationOffset, ANIMATION_EPSILON);
						if (yawZero && pitchZero)
						{
							mSequenceState++;
						}
						else
						{
							if (!yawZero)
							{
								mCraneTransformComponent->setRotation(mCraneTransformComponent->getRotation() * qsf::EulerAngles::eulerToQuaternion(newRotateYaw, 0.0f, 0.0f));
							}
							if (!pitchZero)
							{
								mLowToHighAnimationChannel->setOffset(newLowToHighAnimationOffset);
							}
						}
						break;
					}
					case PITCH_AND_EXTEND_MOVEMENT:
					{
						const bool pitchZero = detail::isZero(mLowToHighAnimationOffset, ANIMATION_EPSILON);
						const bool extendZero = detail::isZero(mExtendAnimationOffset, ANIMATION_EPSILON);

						if (pitchZero && extendZero)
						{
							mSequenceState++;
						}
						else
						{
							if (!pitchZero)
							{
								mLowToHighAnimationChannel->setOffset(newLowToHighAnimationOffset);
							}
							if (!extendZero)
							{
								mExtendAnimationChannel->setOffset(newExtendAnimationOffset);
							}
						}
						break;
					}
					case YAW_AND_EXTEND_MOVEMENT:
					{
						const bool yawZero = detail::isZero(mRotateYawAngle, YAW_EPSILON);
						const bool extendZero = detail::isZero(mExtendAnimationOffset, ANIMATION_EPSILON);

						if (yawZero && extendZero)
						{
							mSequenceState++;
						}
						else
						{
							if (!yawZero)
							{
								mCraneTransformComponent->setRotation(mCraneTransformComponent->getRotation() * qsf::EulerAngles::eulerToQuaternion(newRotateYaw, 0.0f, 0.0f));
							}
							if (!extendZero)
							{
								mExtendAnimationChannel->setOffset(newExtendAnimationOffset);
							}
						}
						break;
					}
					case COCKPIT_MOVEMENT:
					{
						// Pause audio
						if (mAudioProxy.isPlaying())
						{
							mAudioProxy.stop();
						}
						// Play cockpit audio
						if (!mCockpitAudioProxy.isPlaying())
						{
							if (mCockpitAnimationOffset > 0.0f)
							{
								AudioHelper::playCraneCockpitUp(mCockpitAudioProxy, getEntity());
							}
							else
							{
								AudioHelper::playCraneCockpitDown(mCockpitAudioProxy, getEntity());
							}
						}

						const bool cockpitZero = mCraneType != HEAVY_CRANE || detail::isZero(mCockpitAnimationOffset, ANIMATION_EPSILON);
						if (cockpitZero)
						{
							mCockpitAudioProxy.stop();
							mAudioProxy.restart();
							mSequenceState++;
						}
						else
						{
							mCockpitAnimationChannel->setOffset(newCockpitAnimationOffset);
						}
						break;
					}
					case PITCH_AND_COCKPIT_MOVEMENT:
					{
						// Play cockpit audio
						if (!mCockpitAudioProxy.isPlaying())
						{
							if (mCockpitAnimationOffset > 0.0f)
							{
								AudioHelper::playCraneCockpitUp(mCockpitAudioProxy, getEntity());
							}
							else
							{
								AudioHelper::playCraneCockpitDown(mCockpitAudioProxy, getEntity());
							}
						}

						const bool pitchZero = detail::isZero(mLowToHighAnimationOffset, ANIMATION_EPSILON);
						const bool cockpitZero = mCraneType != HEAVY_CRANE || detail::isZero(mCockpitAnimationOffset, ANIMATION_EPSILON);

						if (pitchZero && cockpitZero)
						{
							mCockpitAudioProxy.stop();
							++mSequenceState;
						}
						else
						{
							if (!pitchZero)
							{
								mLowToHighAnimationChannel->setOffset(newLowToHighAnimationOffset);
							}
							if (!cockpitZero)
							{
								mCockpitAnimationChannel->setOffset(newCockpitAnimationOffset);
							}
						}
						break;
					}
				}
			}
			alignStage();
		}
	}

	bool VehicleCraneComponent::getHookDummy()
	{
		if (mCraneType != HEAVY_CRANE)
			return false;

		VehicleCraneComponent* vehicleCraneComponent = getEntity().getComponent<VehicleCraneComponent>();
		if (nullptr == vehicleCraneComponent)
			return false;

		mCraneEntity = getEntity().getMap().getEntityById(vehicleCraneComponent->getCraneEntityId());
		if (nullptr == mCraneEntity)
			return false;

		qsf::TransformComponent& craneTransformComponent = mCraneEntity->getOrCreateComponentSafe<qsf::TransformComponent>();

		mJibEntity = getFirstChildEntityWithTag(mCraneEntity, "jib");
		if (nullptr == mJibEntity)
			return false;

		qsf::Entity* hookEntity = getFirstChildEntityWithTag(mJibEntity, "hook");
		if (nullptr == hookEntity)
			return false;

		mStageBoneToBoneLinkComponent = hookEntity->getComponent<qsf::game::BoneToBoneLinkComponent>();
		if (nullptr == mStageBoneToBoneLinkComponent)
			return false;

		// Calculate position we want as move target
		qsf::TransformComponent& hookTransformComponent = hookEntity->getOrCreateComponentSafe<qsf::TransformComponent>();
		mHookPosition = worldSpacePositionWithLocalOffset(hookTransformComponent, glm::vec3(0.0f, -1.8f, 0.5f));

		// Set the hock position in x/z direction to the target position. With this only changes on the y-axis are done (straight up/down movement)
		mHookPosition.x = mTargetPosition.x;
		mHookPosition.z = mTargetPosition.z;

		mHookMoveTargetPosition = glm::mix(mHookPosition, mTargetPosition, mDistancePercentage);

		mHookDummy = getFirstChildEntityWithTag(mCraneEntity, "dummy");
		if (nullptr == mHookDummy)
		{
			// Create hook
			mHookDummy = getEntity().getMap().createObjectByLocalPrefabAssetId(qsf::StringHash("em5/prefab/gamelogic_entity/dummy_bone"));
			if (nullptr == mHookDummy)
				return false;

			// Set metadata tag
			qsf::MetadataComponent& dummyMetadataComponent = mHookDummy->getOrCreateComponentSafe<qsf::MetadataComponent>();
			dummyMetadataComponent.setTags("dummy");

			// Set dummy position to the hook
			qsf::TransformComponent& dummyTransformComponent = mHookDummy->getOrCreateComponentSafe<qsf::TransformComponent>();
			dummyTransformComponent.setPositionAndRotation(mHookPosition, craneTransformComponent.getRotation() * mStageBoneToBoneLinkComponent->getLocalRotation());

			mDistance = detail::distance(mHookPosition, mHookMoveTargetPosition);

			// Link dummy to the hook
			qsf::LinkComponent& dummyLinkComponent = mHookDummy->getOrCreateComponentSafe<qsf::LinkComponent>();
			dummyLinkComponent.setParentId(mCraneEntity->getId());
			dummyLinkComponent.setParentLinkType(qsf::LinkComponent::HARD_LINK);

			// BoneLink the hookbone to the dummy
			mStageBoneToBoneLinkComponent->setLocalRotation(qsf::EulerAngles::eulerToQuaternion(0.0f, 0.0f, 0.0f)); // Reset hook offset
			mStageBoneToBoneLinkComponent->setParentEntity2(mHookDummy->getId());
			mStageBoneToBoneLinkComponent->setParentBoneName2("dummy_bone");
			mStageBoneToBoneLinkComponent->setChildBoneName2("crane_hook_bone");
			mStageBoneToBoneLinkComponent->setLocalRotation2(qsf::EulerAngles::eulerToQuaternion(0.0f, glm::half_pi<float>(), -glm::pi<float>()));

			// Offset so that the graphical hook moves straight down. Due the local rotation we have to modify x and y values for this
			// TODO(sw) These values where retrieved via trial and error
			mStageBoneToBoneLinkComponent->setLocalPosition2(glm::vec3(-0.595f, 0.57f, 0.0f));
		}
		else
		{
			qsf::TransformComponent& dummyTransformComponent = mHookDummy->getOrCreateComponentSafe<qsf::TransformComponent>();
			mDistance = detail::distance(dummyTransformComponent.getPosition(), mHookMoveTargetPosition);
		}
		return true;
	}

	bool VehicleCraneComponent::isHookMoving()
	{
		if (!getHookDummy())
		{
			mHookAudioProxy.stop();
			mHookAnimationJobProxy.unregister();
			return false;
		}

		if (mTimePassed.getSeconds() > mTimeUltimatum || detail::isZero(mDistance, HOOK_DISTANCE_EPSILON))
		{
			mHookAudioProxy.stop();
			mHookAnimationJobProxy.unregister();

			// Try to jump to a quick animation solution if animation took longer then mTimeUltimatum
			if (mTimePassed.getSeconds() > mTimeUltimatum)
			{
				mHookDummy->getOrCreateComponentSafe<qsf::TransformComponent>().setPosition(mHookMoveTargetPosition);
			}

			// Delete dummy entity if we are at the start
			if (detail::isZero(detail::distance(mHookPosition, mHookMoveTargetPosition), HOOK_DISTANCE_EPSILON))
			{
				qsf::Entity* hookEntity = getFirstChildEntityWithTag(mJibEntity, "hook");
				if (nullptr != hookEntity)
				{
					qsf::game::BoneToBoneLinkComponent& hookBoneToBoneLinkComponent = hookEntity->getOrCreateComponentSafe<qsf::game::BoneToBoneLinkComponent>();
					hookBoneToBoneLinkComponent.setParentBoneName2("");
					hookBoneToBoneLinkComponent.setChildBoneName2("");
					hookBoneToBoneLinkComponent.setParentEntity2(qsf::getUninitialized<uint64>());
				}
				getEntity().getMap().destroyEntityById(mHookDummy->getId());
				mHookDummy = nullptr;
			}
			return false;
		}
		return true;
	}

	void VehicleCraneComponent::updateHookAnimation(const qsf::JobArguments& jobArguments)
	{
		mTimePassed += jobArguments.getTimePassed();

		if (isHookMoving())
		{
			qsf::TransformComponent& dummyTransformComponent = mHookDummy->getOrCreateComponentSafe<qsf::TransformComponent>();
			float newDistance = detail::limitBySpeed(mDistance, mHookSpeed, jobArguments.getTimePassed());
			const glm::vec3 newPosition = detail::moveByDistance(dummyTransformComponent.getPosition(), mHookMoveTargetPosition, newDistance);

			dummyTransformComponent.setPosition(newPosition);
		}
	}

	qsf::AssetProxy VehicleCraneComponent::getAnimationToUseForCraneExtend() const
	{
		// Is there a user-defined animation?
		if (mAnimationCraneExtend.isValid())
		{
			return mAnimationCraneExtend;
		}

		if (mCraneType == TOW_CAR || mCraneType == BOAT_TRANSPORTER)
		{
			return qsf::AssetProxy(AnimationHelper(*mJibEntity).getAnimationCraneExtend());
		}
		else
		{
			return qsf::AssetProxy(AnimationHelper(*mJibEntity).getAnimationHeavyCraneExtend());
		}
	}

	qsf::AssetProxy VehicleCraneComponent::getAnimationToUseForCraneLowToHigh() const
	{
		// Is there a user-defined animation?
		if (mAnimationCraneLowToHigh.isValid())
		{
			return mAnimationCraneLowToHigh;
		}

		if (mCraneType == TOW_CAR)
		{
			return qsf::AssetProxy(AnimationHelper(*mCraneEntity).getAnimationCraneLowToHigh());
		}
		else if (mCraneType == BOAT_TRANSPORTER)
		{
			return qsf::AssetProxy(AnimationHelper(*mCraneEntity).getAnimationCraneLowToHigh_boat());
		}
		else
		{
			return qsf::AssetProxy(AnimationHelper(*mCraneEntity).getAnimationHeavyCraneLowToHigh());
		}
	}

	qsf::AssetProxy VehicleCraneComponent::getAnimationToUseForCraneCockpit() const
	{
		// Is there a user-defined animation?
		if (mAnimationCraneCockpit.isValid())
		{
			return mAnimationCraneCockpit;
		}

		// Only valid for heavy crane; this should not get called for other types anyway
		return qsf::AssetProxy(AnimationHelper(*mCraneEntity).getAnimationHeavyCraneCockpit());
	}

	qsf::AssetProxy VehicleCraneComponent::getAnimationToUseForCraneHook() const
	{
		// Is there a user-defined animation?
		if (mAnimationCraneHook.isValid())
		{
			return mAnimationCraneHook;
		}

		// Only valid for heavy crane; this should not get called for other types anyway
		return qsf::AssetProxy(AnimationHelper(*mCraneEntity).getAnimationHeavyCraneHook());
	}

	qsf::Entity* VehicleCraneComponent::getFirstChildEntity(qsf::Entity* entity) const
	{
		// Just give back the first child entity
		if (nullptr == entity)
			return nullptr;

		const qsf::LinkComponent* linkComponent = entity->getComponent<qsf::LinkComponent>();
		if (nullptr == linkComponent)
			return nullptr;

		boost::container::flat_set<qsf::LinkComponent*>::const_iterator iterator = linkComponent->getChildLinks().begin();
		if (iterator == linkComponent->getChildLinks().end())
			return nullptr;

		return &(*iterator)->getEntity();
	}

	qsf::Entity* VehicleCraneComponent::getFirstChildEntityWithTag(qsf::Entity* entity, const std::string& tag) const
	{
		// Just give back the first child entity with given tag in metadata component
		if (nullptr == entity)
			return nullptr;

		const qsf::LinkComponent* linkComponent = entity->getComponent<qsf::LinkComponent>();
		if (nullptr == linkComponent)
			return nullptr;

		for (qsf::LinkComponent* childLinkComponent : linkComponent->getChildLinks())
		{
			qsf::Entity& childEntity = childLinkComponent->getEntity();
			qsf::MetadataComponent& metadataComponent = childEntity.getOrCreateComponentSafe<qsf::MetadataComponent>();

			if (metadataComponent.getTags() == tag)
				return &childEntity;
		}
		return nullptr;
	}

	void VehicleCraneComponent::alignStage()
	{
		if (mCraneType == TOW_CAR || mCraneType == BOAT_TRANSPORTER)
		{
			const float pitchAngle = detail::lerp(mMinAngle, mMaxAngle, mLowToHighAnimationChannel->getOffset());
			const glm::quat inverseYawRotation = glm::inverse(mCraneLinkComponent->getLocalRotation());
			const glm::quat inversePitchRotation = qsf::EulerAngles::eulerToQuaternion(0.0f, pitchAngle, 0.0f);

			mStageBoneToBoneLinkComponent->setLocalRotation(inversePitchRotation * inverseYawRotation);
		}
		else if (mCraneType == HEAVY_CRANE)
		{
			const float pitchAngle = detail::lerp(mMinAngle, mMaxAngle, mLowToHighAnimationChannel->getOffset());

			// We need a 12.1 degree offset otherwise the hook is stays vertical while the arm goes up. Because the crane arm is initially already rotated a bit up
			// TODO(sw) make this configureable?
			const glm::quat& inversePitchRotation = qsf::EulerAngles::eulerToQuaternion(0.0f, (-pitchAngle)+glm::radians(12.1f), 0.0f);

			mStageBoneToBoneLinkComponent->setLocalRotation(inversePitchRotation);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
