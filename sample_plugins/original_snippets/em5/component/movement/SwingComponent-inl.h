// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline SwingComponent::~SwingComponent()
	{
		// Nothing here
	}

	inline SwingComponent::AnimationType SwingComponent::getAnimationType() const
	{
		return mAnimationType;
	}

	inline void SwingComponent::setAnimationType(SwingComponent::AnimationType animationType)
	{
		assignAndPromoteChange(mAnimationType, animationType, ANIMATION_TYPE);
	}

	inline float SwingComponent::getAnimationCycle() const
	{
		return mAnimationCycle;
	}

	inline void SwingComponent::setAnimationCycle(float animationCycle)
	{
		assignAndPromoteChange(mAnimationCycle, animationCycle, ANIMATION_CYCLE);
	}

	inline float SwingComponent::getAnimationIntensity() const
	{
		return mAnimationIntensity;
	}

	inline void SwingComponent::setAnimationIntensity(float animationIntensity)
	{
		assignAndPromoteChange(mAnimationIntensity, animationIntensity, ANIMATION_INTENSITY);
	}

	inline float SwingComponent::getVelocitySkew() const
	{
		return mVelocitySkew;
	}

	inline void SwingComponent::setVelocitySkew(float velocitySkew)
	{
		assignAndPromoteChange(mVelocitySkew, velocitySkew, VELOCITY_SKEW);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
