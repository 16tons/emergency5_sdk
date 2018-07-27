// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline MeshAnimationChannel::MeshAnimationChannel() :
		mBoneTranslationRetargeting(MeshAnimationComponent::ANIMATION_BONE_SCALE_RETARGETING),
		mBoneBlendMask(nullptr),
		mOffset(0.0f),
		mSpeed(1.0f),
		mWeight(1.0f),
		mWeightValue(1.0f),
		mWeightDelta(0.0f),
		mOgreAnimationState(nullptr),
		mOgreAnimationStateCreationFailed(false)
	{
		// Nothing here
	}

	inline MeshAnimationChannel::~MeshAnimationChannel()
	{
		delete mBoneBlendMask;
	}

	inline Time MeshAnimationChannel::getLength() const
	{
		return mLength;
	}

	inline bool MeshAnimationChannel::getLoop() const
	{
		return mInternalFlags.isSet(LOOP_ANIMATION);
	}

	inline bool MeshAnimationChannel::getReverse() const
	{
		return mInternalFlags.isSet(REVERSE_ANIMATION);
	}

	inline void MeshAnimationChannel::setReverse(bool reverse)
	{
		// Set flag
		mInternalFlags.set(REVERSE_ANIMATION, reverse);
	}

	inline float MeshAnimationChannel::getOffset() const
	{
		return mOffset;
	}

	inline Time MeshAnimationChannel::getTimePosition() const
	{
		return mLength * mOffset;
	}

	inline Time MeshAnimationChannel::getWaitTime() const
	{
		return mWaitTime;
	}

	inline float MeshAnimationChannel::getWeight() const
	{
		return mWeight;
	}

	inline float MeshAnimationChannel::getWeightValue() const
	{
		return mWeightValue;
	}

	inline void MeshAnimationChannel::setWeightValue(float weightValue)
	{
		mWeightValue = weightValue;
	}

	inline float MeshAnimationChannel::getWeightDelta() const
	{
		return mWeightDelta;
	}

	inline void MeshAnimationChannel::setWeightDelta(float weightDelta)
	{
		mWeightDelta = weightDelta;
	}

	inline float MeshAnimationChannel::getSpeed() const
	{
		return mSpeed;
	}

	inline bool MeshAnimationChannel::isFinished() const
	{
		return mInternalFlags.isSet(ANIMATION_FINISHED);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	inline bool MeshAnimationChannel::hasBoneMask() const
	{
		return (nullptr != mBoneBlendMask);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
