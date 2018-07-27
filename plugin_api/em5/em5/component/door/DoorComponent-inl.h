// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/math/CoordinateSystem.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline DoorComponent::DoorComponent(qsf::Prototype* prototype) :
		qsf::Component(prototype),
		mLocalRotationAxis(qsf::CoordinateSystem::getUp()),
		mMaximumRotationAngle(75.0f),
		mDoorType(DOORTYPE_UNDEFINED),
		mOpeningAnimationLength(qsf::Time::fromSeconds(1.0f)),
		mClosingAnimationLength(qsf::Time::fromSeconds(1.0f)),
		mState(DOOR_CLOSED),
		mAnimationPosition(0.0f),
		mIsPlayingAudioStarted(false)
	{
		// Nothing to do in here
	}

	inline DoorComponent::~DoorComponent()
	{
		// Nothing to do in here
	}

	inline DoorComponent::DoorState DoorComponent::getDoorState() const
	{
		return mState;
	}

	inline const qsf::AssetProxy& DoorComponent::getOpeningAnimation() const
	{
		return mOpeningAnimation;
	}

	inline void DoorComponent::setOpeningAnimation(const qsf::AssetProxy& animation)
	{
		mOpeningAnimation = animation;
	}

	inline const qsf::AssetProxy& DoorComponent::getClosingAnimation() const
	{
		return mClosingAnimation;
	}

	inline void DoorComponent::setClosingAnimation(const qsf::AssetProxy& animation)
	{
		mClosingAnimation = animation;
	}

	inline const qsf::AssetProxy& DoorComponent::getCuttingAnimation() const
	{
		return mCuttingAnimation;
	}

	inline void DoorComponent::setCuttingAnimation(const qsf::AssetProxy& animation)
	{
		mCuttingAnimation = animation;
	}

	inline const glm::vec3& DoorComponent::getDoorOffset() const
	{
		return mDoorOffset;
	}

	inline const glm::quat& DoorComponent::getDoorRotation() const
	{
		return mDoorRotation;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
