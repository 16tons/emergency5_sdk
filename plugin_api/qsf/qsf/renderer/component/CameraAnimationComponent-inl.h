// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline CameraAnimationComponent::~CameraAnimationComponent()
	{
		// Nothing here
	}

	inline CameraAnimationComponent::AnimationType CameraAnimationComponent::getAnimationType() const
	{
		return mAnimationType;
	}

	inline void CameraAnimationComponent::setAnimationType(CameraAnimationComponent::AnimationType animationType)
	{
		assignAndPromoteChange(mAnimationType, animationType, ANIMATION_TYPE);
	}

	inline float CameraAnimationComponent::getAnimationCycle() const
	{
		return mAnimationCycle;
	}

	inline void CameraAnimationComponent::setAnimationCycle(float animationCycle)
	{
		assignAndPromoteChange(mAnimationCycle, animationCycle, ANIMATION_CYCLE);
	}

	inline float CameraAnimationComponent::getRotationIntensity() const
	{
		return mRotationIntensity;
	}

	inline void CameraAnimationComponent::setPositionIntensity(float positionIntensity)
	{
		assignAndPromoteChange(mPositionIntensity, positionIntensity, POSITION_INTENSITY);
	}

	inline float CameraAnimationComponent::getPositionIntensity() const
	{
		return mPositionIntensity;
	}

	inline void CameraAnimationComponent::setRotationIntensity(float rotationIntensity)
	{
		assignAndPromoteChange(mRotationIntensity, rotationIntensity, ROTATION_INTENSITY);
	}

	inline const qsf::Time& CameraAnimationComponent::getPlayTime() const
	{
		return mPlayTime;
	}

	inline void CameraAnimationComponent::setPlayTime(const qsf::Time& playTime)
	{
		assignAndPromoteChange(mPlayTime, playTime, PLAYTIME);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
