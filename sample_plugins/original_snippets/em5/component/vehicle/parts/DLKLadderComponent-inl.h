// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline float DlkLadderComponent::getVerticalRescueOffset() const
	{
		return mVerticalRescueOffset;
	}

	inline void DlkLadderComponent::setVerticalRescueOffset(float verticalRescueOffset)
	{
		mVerticalRescueOffset = verticalRescueOffset;
	}

	inline float DlkLadderComponent::getVerticalDeployOffset() const
	{
		return mVerticalDeployOffset;
	}

	inline void DlkLadderComponent::setVerticalDeployOffset(float verticalDeployOffset)
	{
		mVerticalDeployOffset = verticalDeployOffset;
	}

	inline float DlkLadderComponent::getLadderMaxRotateSpeed() const
	{
		return mLadderMaxRotateSpeed;
	}

	inline void DlkLadderComponent::setLadderMaxRotateSpeed(float ladderMaxRotateSpeed)
	{
		mLadderMaxRotateSpeed = ladderMaxRotateSpeed;
	}

	inline float DlkLadderComponent::getLadderMinRotateSpeed() const
	{
		return mLadderMinRotateSpeed;
	}

	inline void DlkLadderComponent::setLadderMinRotateSpeed(float ladderMinRotateSpeed)
	{
		mLadderMinRotateSpeed = ladderMinRotateSpeed;
	}

	inline float DlkLadderComponent::getLadderUpSpeed() const
	{
		return mLadderUpSpeed;
	}

	inline void DlkLadderComponent::setLadderUpSpeed(float ladderUpSpeed)
	{
		mLadderUpSpeed = ladderUpSpeed;
	}

	inline float DlkLadderComponent::getLadderExtendSpeed() const
	{
		return mLadderExtendSpeed;
	}

	inline void DlkLadderComponent::setLadderExtendSpeed(float ladderExtendSpeed)
	{
		mLadderExtendSpeed = ladderExtendSpeed;
	}

	inline float DlkLadderComponent::getMaxAngle() const
	{
		return mMaxAngle;
	}

	inline void DlkLadderComponent::setMaxAngle(float maxAngle)
	{
		mMaxAngle = maxAngle;
	}

	inline float DlkLadderComponent::getMinLength() const
	{
		return mMinLength;
	}

	inline void DlkLadderComponent::setMinLength(float minLength)
	{
		mMinLength = minLength;
	}

	inline float DlkLadderComponent::getMaxLength() const
	{
		return mMaxLength;
	}

	inline void DlkLadderComponent::setMaxLength(float maxLength)
	{
		mMaxLength = maxLength;
	}

	inline float DlkLadderComponent::getSynchronLength() const
	{
		return mSynchronLength;
	}

	inline void DlkLadderComponent::setSynchronLength(float synchronLength)
	{
		mSynchronLength = synchronLength;
	}

	inline float DlkLadderComponent::getTimeUltimatum() const
	{
		return mTimeUltimatum;
	}

	inline void DlkLadderComponent::setTimeUltimatum(float timeUltimatum)
	{
		mTimeUltimatum = timeUltimatum;
	}

	inline const qsf::AssetProxy& DlkLadderComponent::getAnimationLadderBaseUp() const
	{
		return mAnimationLadderBaseUp;
	}

	inline void DlkLadderComponent::setAnimationLadderBaseUp(const qsf::AssetProxy& skeleton)
	{
		mAnimationLadderBaseUp = skeleton;
	}

	inline const qsf::AssetProxy& DlkLadderComponent::getAnimationLadderExtend() const
	{
		return mAnimationLadderExtend;
	}

	inline void DlkLadderComponent::setAnimationLadderExtend(const qsf::AssetProxy& skeleton)
	{
		mAnimationLadderExtend = skeleton;
	}

	inline const DlkLadderComponent::AnimationState DlkLadderComponent::getLastAnimationState() const
	{
		return mLastAnimationState;
	}

	inline const glm::vec3& DlkLadderComponent::getTargetPosition() const
	{
		return mPostition;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
