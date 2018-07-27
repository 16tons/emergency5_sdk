// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline BoneLookAtNode::~BoneLookAtNode()
	{
		// Nothing to do
	}

	inline uint16 BoneLookAtNode::getBoneId() const
	{
		return mBoneId;
	}

	inline const glm::vec3& BoneLookAtNode::getTargetPosition() const
	{
		return mTargetPosition;
	}

	inline const glm::vec3& BoneLookAtNode::getRotationConstrain() const
	{
		return mRotationConstrain;
	}

	inline float BoneLookAtNode::getTimeStep() const
	{
		return mTimeStep;
	}

	inline void BoneLookAtNode::setTimeStep(float timeStep)
	{
		mTimeStep = timeStep;
	}

	inline const DampedSpring& BoneLookAtNode::getDampedYawAngleSpring() const
	{
		return mDampedYawAngleSpring;
	}

	inline void BoneLookAtNode::setDampedYawAngleSpring(const DampedSpring& dampedSpring)
	{
		float value = mDampedYawAngleSpring.getValue();	// We retain the value
		mDampedYawAngleSpring = dampedSpring;
		mDampedYawAngleSpring.setValue(value);
	}

	inline const DampedSpring& BoneLookAtNode::getDampedPitchAngleSpring() const
	{
		return mDampedPitchAngleSpring;
	}

	inline void BoneLookAtNode::setDampedPitchAngleSpring(const DampedSpring& dampedSpring)
	{
		float value = mDampedPitchAngleSpring.getValue();	// We retain the value
		mDampedPitchAngleSpring = dampedSpring;
		mDampedPitchAngleSpring.setValue(value);
	}

	inline bool BoneLookAtNode::isMovingToDefault() const
	{
		return mMovingToDefault;
	}

	inline bool BoneLookAtNode::isAtDefaultPosition() const
	{
		return mFinishedAtDefault;
	}

	inline const glm::vec3& BoneLookAtNode::getMovingToDefaultRotationConstrain() const
	{
		return mMovingToDefaultRotationConstrain;
	}

	inline float BoneLookAtNode::getAnimationInfluence() const
	{
		return mAnimationInfluence;
	}

	inline void BoneLookAtNode::setAnimationInfluence(float animationInfluence)
	{
		mAnimationInfluence = animationInfluence;
	}

	inline float BoneLookAtNode::getAnimationInfluenceBlendSpeed() const
	{
		return mAnimationInfluenceBlendSpeed;
	}

	inline void BoneLookAtNode::setAnimationInfluenceBlendSpeed(float animationInfluenceBlendSpeed)
	{
		mAnimationInfluenceBlendSpeed = animationInfluenceBlendSpeed;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
