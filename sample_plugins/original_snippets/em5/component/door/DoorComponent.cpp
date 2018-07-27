// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/door/DoorComponent.h"
#include "em5/audio/AudioHelper.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/map/EntityHelper.h"

#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/debug/request/CircleDebugDrawRequest.h>
#include <qsf/debug/request/SegmentDebugDrawRequest.h>
#include <qsf/job/JobArguments.h>
#include <qsf/map/Entity.h>
#include <qsf/math/Math.h>
#include <qsf/plugin/QsfJobs.h>
#include <qsf/renderer/animation/MeshAnimationComponent.h>
#include <qsf/serialization/binary/GlmTypeSerialization.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 DoorComponent::COMPONENT_ID	= qsf::StringHash("em5::DoorComponent");
	const uint32 DoorComponent::DOOR_OFFSET		= qsf::StringHash("DoorOffset");
	const uint32 DoorComponent::DOOR_ROTATION	= qsf::StringHash("DoorRotation");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	float DoorComponent::getDoorStateInPercent() const
	{
		if (mState == DOOR_CLOSED || mState == DOOR_OPEN)
			return 1.0f;

		// Check if the animation is triggered via an animation asset
		if (mAnimationChannel.valid())
		{
			const float offset = mAnimationChannel->getOffset();
			return mAnimationChannel->getReverse() ? (1.0f - offset) : offset;
		}

		// Handle case for manual animation
		return (mState == DOOR_OPENING) ? mAnimationPosition : (1.0f - mAnimationPosition);
	}

	float DoorComponent::getDoorAnimationPosition() const
	{
		if (mState == DOOR_CLOSED)
			return 0.0f;
		if (mState == DOOR_OPEN)
			return 1.0f;

		// Check if the animation is triggered via an animation asset
		if (mAnimationChannel.valid())
		{
			if (mState == DOOR_OPENING)
			{
				return mAnimationChannel->getOffset();
			}
			else  // Must be DOOR_CLOSING
			{
				const float offset = mAnimationChannel->getOffset();
				return mAnimationChannel->getReverse() ? offset : (1.0f - offset);
			}
		}

		// Handle case for manual animation
		return mAnimationPosition;
	}

	void DoorComponent::openDoor()
	{
		// Anything to do?
		if (mState == DOOR_OPENING || mState == DOOR_OPEN)
			return;

		// Playing a close animation right now?
		if (mAnimationChannel.valid())
		{
			startOpeningAnimation(getDoorAnimationPosition());
		}

		// Change state
		mState = DOOR_OPENING;

		// Set audio flag
		mIsPlayingAudioStarted = false;

		// Activate job so we get regular update calls
		activateJob();
	}

	void DoorComponent::closeDoor()
	{
		// Anything to do?
		if (mState == DOOR_CLOSING || mState == DOOR_CLOSED)
			return;

		// Playing an open animation right now?
		if (mAnimationChannel.valid())
		{
			startClosingAnimation(getDoorAnimationPosition());
		}

		// Change state
		mState = DOOR_CLOSING;

		// Set audio flag
		mIsPlayingAudioStarted = false;

		// Activate job so we get regular update calls
		activateJob();
	}

	void DoorComponent::cutDoor()
	{
		// Cutting animation
		if (mCuttingAnimation.isValid())
		{
			getEntity().getOrCreateComponent<qsf::MeshAnimationComponent>()->playAnimation(mCuttingAnimation, false);
		}
	}

	void DoorComponent::setDoorOffset(const glm::vec3& value)
	{
		mDoorOffset = value;
		updateDoorDebugDraw(isDebug());
	}

	void DoorComponent::setDoorRotation(const glm::quat& value)
	{
		mDoorRotation = value;
		updateDoorDebugDraw(isDebug());
	}

	void DoorComponent::getDoorWorldSpaceTransform(qsf::Transform& result, const qsf::Transform* vehicleWorldTransform) const
	{
		qsf::Transform resultTemp;
		if (nullptr != vehicleWorldTransform)
		{
			// Use given world transform as position for the vehicle
			resultTemp.fromLocalTransform(mOriginalLocalTransform, *vehicleWorldTransform);
		}
		else
		{
			// Use the current vehicle position
			getLocalToWorldTransform(resultTemp);
		}

		// Apply the door offset
		qsf::Transform offsetTransform;
		offsetTransform.setPositionAndRotation(mDoorOffset, mDoorRotation);
		result.fromLocalTransform(offsetTransform, resultTemp);
	}

	void DoorComponent::getLocalToWorldTransform(qsf::Transform& transform) const
	{
		const qsf::LinkComponent* linkComponent = getPrototype().getComponent<qsf::LinkComponent>();
		if (nullptr != linkComponent)
		{
			const qsf::Prototype* parentPrototype = linkComponent->getParentPrototype();
			if (nullptr != parentPrototype)
			{
				const qsf::TransformComponent* parentTransformComponent = parentPrototype->getComponent<qsf::TransformComponent>();
				if (nullptr != parentTransformComponent)
				{
					transform.fromLocalTransform(mOriginalLocalTransform, parentTransformComponent->getTransform());
					return;
				}
			}
		}

		QSF_ERROR("DoorComponent::getLocalToWorldTransform(): Can't finish position calculation because the entity is in unknown state.", QSF_REACT_NONE);
		transform = mOriginalLocalTransform;
	}

	glm::vec3 DoorComponent::getDoorOffsetWorldSpacePosition() const
	{
		// Calculate transform
		qsf::Transform originalLocalToWorld;
		getLocalToWorldTransform(originalLocalToWorld);
		return originalLocalToWorld.vec3PositionLocalToWorld(mDoorOffset);
	}

	glm::quat DoorComponent::getDoorRotationWorldSpaceRotation() const
	{
		// Calculate transform
		qsf::Transform originalLocalToWorld;
		getLocalToWorldTransform(originalLocalToWorld);
		return originalLocalToWorld.quatRotationLocalToWorld(mDoorRotation);
	}

	void DoorComponent::getWorldPositionInFrontOfDoor(float distance, glm::vec3& position, glm::quat& rotation) const
	{
		qsf::Transform originalLocalToWorld;
		getLocalToWorldTransform(originalLocalToWorld);
		position = originalLocalToWorld.vec3PositionLocalToWorld(glm::vec3(0, 0, -distance));
		rotation = getDoorRotationWorldSpaceRotation();
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	void DoorComponent::serialize(qsf::BinarySerializer& serializer)
	{
		// Internal
		serializer.serializeAs<uint8>(mState);
		int64 openingAnimationLength = 0;
		int64 closingAnimationLength = 0;
		if (serializer.isReading())
		{
			serializer.serialize(openingAnimationLength);
			mOpeningAnimationLength.fromMicroseconds(openingAnimationLength);
			serializer.serialize(closingAnimationLength);
			mClosingAnimationLength.fromMicroseconds(closingAnimationLength);
		}
		else
		{
			openingAnimationLength = mOpeningAnimationLength.getMicroseconds();
			serializer.serialize(openingAnimationLength);
			closingAnimationLength = mClosingAnimationLength.getMicroseconds();
			serializer.serialize(closingAnimationLength);
		}

		// Connected to CAMP
		serializer.serializeAs<uint16>(mDoorType);
		serializer.serialize(mLocalRotationAxis);
		serializer.serialize(mMaximumRotationAngle);
		serializer.serialize(mSlideOffset);
		serializer.serialize(mDoorOffset);
		serializer.serialize(mAnimationPosition);
		mOriginalLocalTransform.serialize(serializer);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	bool DoorComponent::onStartup()
	{
		// Get original local transformation
		const qsf::LinkComponent* linkComponent = getEntity().getComponent<qsf::LinkComponent>();
		if (nullptr != linkComponent && qsf::isInitialized(linkComponent->getParentId()))
		{
			// Copy local transformation, we consider this the "original" transformation
			mOriginalLocalTransform = linkComponent->getLocalTransform();
		}
		else
		{
			// Door entity is not attached to a parent, so take the world position instead
			const qsf::TransformComponent* transformComponent = getEntity().getComponent<qsf::TransformComponent>();
			if (nullptr != transformComponent)
			{
				mOriginalLocalTransform = transformComponent->getTransform();
			}
		}

		// Done
		return true;
	}

	void DoorComponent::onShutdown()
	{
		mUpdateJobProxy.unregister();
	}

	void DoorComponent::onSetDebug(bool debug)
	{
		updateDoorDebugDraw(debug);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void DoorComponent::activateJob()
	{
		// Is job already active?
		if (!mUpdateJobProxy.isValid())
		{
			// Activate job by registering via the job proxy
			mUpdateJobProxy.registerAt(qsf::QsfJobs::ANIMATION_GENERAL, boost::bind(&DoorComponent::updateJob, this, _1));
		}
	}

	void DoorComponent::updateJob(const qsf::JobArguments& jobArguments)
	{
		if (hasAnimation())
		{
			updateWithAnimation();
		}
		else
		{
			updateWithoutAnimation(jobArguments);
		}

		updateAudio();

		// Unregister the job proxy after the door is opened/closed
		if (mState == DOOR_CLOSED || mState == DOOR_OPEN)
		{
			mUpdateJobProxy.unregister();
		}

		// Enable this only temporarily, for debugging door positions
#if 0
		// When done, stop the job update
		if (mState == DOOR_OPEN || mState == DOOR_CLOSED)
		{
			mUpdateJobProxy.unregister();
			mDebugDrawProxy.unregister();
		}
		else
		{
			mDebugDrawProxy.registerForComponent(*this);
			mDebugDrawProxy.addRequest(qsf::CircleDebugDrawRequest(getDoorOffsetWorldSpacePosition(), mLocalRotationAxis, 0.5f, qsf::Color4::WHITE, true));
		}
#endif
	}

	void DoorComponent::applyTransformationByAnimation(qsf::Transform& transform) const
	{
		// Apply transformation offset from door animation
		if (mAnimationPosition > 0.0f)
		{
			// Use cosine curve for movement
			const float t = (1.0f - std::cosf(mAnimationPosition * glm::pi<float>())) * 0.5f;

			// Rotation around axis
			if (mMaximumRotationAngle != 0.0f)
			{
				const glm::quat additionalRotation = qsf::Math::getRotationAroundAxis(mLocalRotationAxis, glm::radians(mMaximumRotationAngle * t));
				transform.setRotation(transform.getRotation() * additionalRotation);
			}

			// Sliding
			transform.setPosition(transform.getPosition() + mSlideOffset * t);
		}
	}

	bool DoorComponent::hasAnimation() const
	{
		// Its enough to check if there is a opening animation because its also used for closing if there is no closing animation
		return mOpeningAnimation.isValid();
	}

	void DoorComponent::updateWithAnimation()
	{
		// Opening animation
		if (mState == DOOR_OPENING)
		{
			// Animation is not started yet
			if (!mAnimationChannel.valid())
			{
				startOpeningAnimation();
			}

			// Animation is finished
			else if (mAnimationChannel.valid() && mAnimationChannel->isFinished())
			{
				// Invalidate the weak-pointer
				mAnimationChannel.clear();
				mState = DOOR_OPEN;
			}
		}

		// Closing animation
		else if (mState == DOOR_CLOSING)
		{
			// Animation is not started yet
			if (!mAnimationChannel.valid())
			{
				startClosingAnimation();
			}

			// Animation is finished
			else if (mAnimationChannel->isFinished())
			{
				// Invalidate the weak-pointer
				mAnimationChannel.clear();
				mState = DOOR_CLOSED;
			}
		}
	}

	void DoorComponent::updateWithoutAnimation(const qsf::JobArguments& jobArguments)
	{
		if (DOOR_OPENING == mState)
		{
			// Open door
			const float animationLength = mOpeningAnimationLength.getSeconds();
			if (animationLength > 0.0f)
			{
				// Update animation progress
				mAnimationPosition += jobArguments.getSecondsPassed() / animationLength;
			}
			else
			{
				// Directly go to the animation end position
				mAnimationPosition = 1.0f;
			}

			if (mAnimationPosition >= 1.0f)
			{
				// Door is now fully open
				mAnimationPosition = 1.0f;

				// Change state
				mState = DOOR_OPEN;
			}
		}
		else if (DOOR_CLOSING == mState)
		{
			// Close door
			const float animationLength = mClosingAnimationLength.getSeconds();
			if (animationLength > 0.0f)
			{
				// Update animation progress
				mAnimationPosition -= jobArguments.getSecondsPassed() / animationLength;
			}
			else
			{
				// Directly go to the animation end position
				mAnimationPosition = 0.0f;
			}

			if (mAnimationPosition <= 0.0f)
			{
				// Door is now fully closed
				mAnimationPosition = 0.0f;

				// Change state
				mState = DOOR_CLOSED;
			}
		}

		// Update visual transformation of the door
		{
			// Get original transformation
			qsf::Transform transform(mOriginalLocalTransform);

			// Apply offset from door animation
			applyTransformationByAnimation(transform);

			// Set as new transformation
			qsf::LinkComponent* linkComponent = getEntity().getComponent<qsf::LinkComponent>();
			if (nullptr != linkComponent && linkComponent->hasParent())
			{
				// Update local transformation
				linkComponent->setLocalTransform(transform);
			}
			else
			{
				qsf::TransformComponent* transformComponent = getEntity().getComponent<qsf::TransformComponent>();
				if (nullptr != transformComponent)
				{
					// Update world transformation
					transformComponent->setTransform(transform);
				}
			}
		}
	}

	void DoorComponent::updateAudio()
	{
		if (mIsPlayingAudioStarted == true || mAudioProxy.isPlaying())
			return;

		// TODO(mk) The opening closing sounds have to be extra properties of the component. Its weird to have extra handling for all different vehicles hard coded

		qsf::Entity& entity = static_cast<qsf::Entity&>(qsf::LinkComponent::getTopmostAncestorPrototype(getEntity(), 0xff));
		CommandableComponent* commandableComponent = entity.getComponent<CommandableComponent>();

		if (EntityHelper(entity).isHelicopter() || (nullptr != commandableComponent && commandableComponent->isPoliceMTW()))
		{
			if (mState == DOOR_CLOSED)
			{
				AudioHelper::playCloseSlidingDoorSound(mAudioProxy, getEntity());
				mIsPlayingAudioStarted = true;
			}
			else if (mState == DOOR_OPENING)
			{
				AudioHelper::playOpenSlidingDoorSound(mAudioProxy, getEntity());
				mIsPlayingAudioStarted = true;
			}
		}
		// Trunk door
		else if (mDoorType == DOORTYPE_TRUNK)
		{
			if (mState == DOOR_CLOSED)
			{
				AudioHelper::playCloseTrunkDoorSound(mAudioProxy, getEntity());
				mIsPlayingAudioStarted = true;
			}
			else if (mState == DOOR_OPENING)
			{
				AudioHelper::playOpenTrunkDoorSound(mAudioProxy, getEntity());
				mIsPlayingAudioStarted = true;
			}
		}
		// Equipment
		else if (mDoorType == DOORTYPE_EQUIPMENTLEFT || mDoorType == DOORTYPE_EQUIPMENTRIGHT)
		{
			// Handle special doors
			qsf::Entity* vehicleEntity = EntityHelper(getEntity()).getParentEntity();
			if (nullptr != vehicleEntity)
			{
				CommandableComponent* vehicleCommandableComponent = vehicleEntity->getComponent<CommandableComponent>();
				if (nullptr != vehicleCommandableComponent && vehicleCommandableComponent->isFireFighterRW())
				{
					AudioHelper::playRWRollUpDoorSound(mAudioProxy, getEntity());
					mIsPlayingAudioStarted = true;
					return;
				}
			}

			// Use trunk door sounds
			if (mState == DOOR_CLOSED)
			{
				AudioHelper::playCloseTrunkDoorSound(mAudioProxy, getEntity());
				mIsPlayingAudioStarted = true;
			}
			else if (mState == DOOR_OPENING)
			{
				AudioHelper::playOpenTrunkDoorSound(mAudioProxy, getEntity());
				mIsPlayingAudioStarted = true;
			}
		}
		// IMT
		else if (nullptr != commandableComponent && commandableComponent->isAmbulanceIMT())
		{
			if (mState == DOOR_CLOSED)
			{
				AudioHelper::playCloseSlidingDoorSound(mAudioProxy, getEntity());
				mIsPlayingAudioStarted = true;
			}
			else if (mState == DOOR_OPENING)
			{
				AudioHelper::playOpenSlidingDoorSound(mAudioProxy, getEntity());
				mIsPlayingAudioStarted = true;
			}
		}
		// Normal door
		else
		{
			if (mState == DOOR_CLOSED)
			{
				AudioHelper::playCloseNormalDoorSound(mAudioProxy, getEntity());
				mIsPlayingAudioStarted = true;
			}
			else if (mState == DOOR_OPENING)
			{
				AudioHelper::playOpenNormalDoorSound(mAudioProxy, getEntity());
				mIsPlayingAudioStarted = true;
			}
		}
	}

	void DoorComponent::startOpeningAnimation(float animationPosition)
	{
		qsf::MeshAnimationComponent& meshAnimationComponent = getEntity().getOrCreateComponentSafe<qsf::MeshAnimationComponent>();

		// Start animation and save animation channel
		mAnimationChannel = meshAnimationComponent.playAnimation(mOpeningAnimation, false);

		// Jump to the right offset
		if (mAnimationChannel.valid() && animationPosition > 0.0f)
		{
			mAnimationChannel->setOffset(animationPosition);
		}
	}

	void DoorComponent::startClosingAnimation(float animationPosition)
	{
		qsf::MeshAnimationComponent& meshAnimationComponent = getEntity().getOrCreateComponentSafe<qsf::MeshAnimationComponent>();

		// Start animation and save animation channel
		if (mClosingAnimation.isValid())
		{
			// Closing animation
			mAnimationChannel = meshAnimationComponent.playAnimation(mClosingAnimation, false);
		}
		else if (mOpeningAnimation.isValid())
		{
			// Opening animation reverse
			mAnimationChannel = meshAnimationComponent.playAnimation(mOpeningAnimation, false, true);
		}

		// Jump to the right offset
		if (mAnimationChannel.valid() && animationPosition < 1.0f)
		{
			mAnimationChannel->setOffset(mAnimationChannel->getReverse() ? animationPosition : (1.0f - animationPosition));
		}
	}

	void DoorComponent::updateDoorDebugDraw(bool enabled)
	{
		// Cancel previous debug draw request
		mDebugDrawProxy.unregister();

		if (enabled)
		{
			const float radius = 0.4f;
			mDebugDrawProxy.registerForComponent(*this);

			const qsf::Transform transform(getDoorOffsetWorldSpacePosition(), getDoorRotationWorldSpaceRotation());
			mDebugDrawProxy.setTransform(transform);

			mDebugDrawProxy.addRequest(qsf::CircleDebugDrawRequest(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), radius, qsf::Color4::CYAN));
			mDebugDrawProxy.addRequest(qsf::SegmentDebugDrawRequest(qsf::Segment::fromTwoPoints(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -radius)), qsf::Color4::CYAN));
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
