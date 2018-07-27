// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline LightAnimationComponent::~LightAnimationComponent()
	{
		// Nothing here
	}

	inline LightAnimationComponent::AnimationType LightAnimationComponent::getAnimationType() const
	{
		return mAnimationType;
	}

	inline void LightAnimationComponent::setAnimationType(LightAnimationComponent::AnimationType animationType)
	{
		assignAndPromoteChange(mAnimationType, animationType, ANIMATION_TYPE);
	}

	inline float LightAnimationComponent::getAnimationCycle() const
	{
		return mAnimationCycle;
	}

	inline void LightAnimationComponent::setAnimationCycle(float animationCycle)
	{
		assignAndPromoteChange(mAnimationCycle, animationCycle, ANIMATION_CYCLE);
	}

	inline float LightAnimationComponent::getMinIntensity() const
	{
		return mMinIntensity;
	}

	inline bool LightAnimationComponent::getFade() const
	{
		return mFade;
	}

	inline void LightAnimationComponent::setFade(bool fade)
	{
		assignAndPromoteChange(mFade, fade, FADE);
	}

	inline int LightAnimationComponent::getCustomAnimationStartOffset() const
	{
		return mCustomAnimationStartOffset;
	}

	inline void LightAnimationComponent::setCustomAnimationStartOffset(int offset)
	{
		assignAndPromoteChange(mCustomAnimationStartOffset, offset, CUSTOM_ANIMATION_START_OFFSET);
	}

	inline bool LightAnimationComponent::getRandomStart() const
	{
		return mRandomStart;
	}

	inline void LightAnimationComponent::setRandomStart(bool randomStart)
	{
		assignAndPromoteChange(mRandomStart, randomStart, RANDOM_START);
	}

	inline bool LightAnimationComponent::getRandomStartDelay() const
	{
		return mRandomStartDelay;
	}

	inline void LightAnimationComponent::setRandomStartDelay(bool randomStartDelay)
	{
		assignAndPromoteChange(mRandomStartDelay, randomStartDelay, RANDOM_START_DELAY);
	}

	inline float LightAnimationComponent::getStartDelay() const
	{
		return mAnimationCycleDelay;
	}

	inline void LightAnimationComponent::setStartDelay(float startDelay)
	{
		// Clamp the value to 0.0f - 1.0f
		assignAndPromoteChange(mAnimationCycleDelay, glm::clamp(0.0f, 1.0f, startDelay), START_DELAY);
	}

	inline bool LightAnimationComponent::getInvisibleAnimation() const
	{
		return mInvisibleAnimation;
	}

	inline void LightAnimationComponent::setRegisterAtObjectRenderingListener(bool registerAtObjectRenderingListener)
	{
		mRegisterAtMovableObjectRenderingListener = registerAtObjectRenderingListener;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
