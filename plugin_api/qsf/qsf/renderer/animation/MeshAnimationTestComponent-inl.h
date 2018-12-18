// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline MeshAnimationTestComponent::MeshAnimationTestComponent(Prototype* prototype) :
		MeshAnimationComponent(prototype),
		mSpeed(1.0f),
		mPrologLength(1.0f),
		mBlendLength(1.0f),
		mInternalFlags(LOOP_ANIMATION),
		mBlendSyncMode(SYNC_NONE),
		mBoneTranslationRetargeting(ANIMATION_BONE_SCALE_RETARGETING),
		mUseRandomOffset(false),
		mTestComponentUpdatesEnabled(true),
		mCurrentTime(0.0f),
		mBlendingStarted(false),
		mNeedsRestart(false),
		mMixAnimation(false)
	{
		for (int i = 0; i < 2; ++i)
		{
			mMovement[i] = 0.0f;
		}
	}

	inline MeshAnimationTestComponent::~MeshAnimationTestComponent()
	{
		// Nothing to do in here
	}

	inline const AssetProxy& MeshAnimationTestComponent::getSkeletonA() const
	{
		return mSkeleton[0];
	}

	inline const AssetProxy& MeshAnimationTestComponent::getSkeletonB() const
	{
		return mSkeleton[1];
	}

	inline bool MeshAnimationTestComponent::getLoop() const
	{
		return mInternalFlags.isSet(LOOP_ANIMATION);
	}

	inline bool MeshAnimationTestComponent::getReverseAnimation()
	{
		return mInternalFlags.isSet(REVERSE_ANIMATION);
	}

	inline float MeshAnimationTestComponent::getSpeed() const
	{
		return mSpeed;
	}

	inline MeshAnimationTestComponent::BoneRetargeting MeshAnimationTestComponent::getBoneTranslationRetargeting() const
	{
		return mBoneTranslationRetargeting;
	}

	inline void MeshAnimationTestComponent::setBoneTranslationRetargeting(BoneRetargeting boneRetargeting)
	{
		if (mBoneTranslationRetargeting != boneRetargeting)
		{
			mBoneTranslationRetargeting = boneRetargeting;

			if (isRunning())
			{
				resetAnimationBindingNextTime();
				restartAnimation();
			}
		}
	}

	inline float MeshAnimationTestComponent::getPrologLength() const
	{
		return mPrologLength;
	}

	inline void MeshAnimationTestComponent::setPrologLength(float prologLength)
	{
		mPrologLength = prologLength;
	}

	inline float MeshAnimationTestComponent::getBlendLength() const
	{
		return mBlendLength;
	}

	inline void MeshAnimationTestComponent::setBlendLength(float blendLength)
	{
		mBlendLength = blendLength;
	}

	inline bool MeshAnimationTestComponent::getMovementEnabled() const
	{
		return mInternalFlags.isSet(MOVEMENT_ENABLED);
	}

	inline void MeshAnimationTestComponent::setMovementEnabled(bool enable)
	{
		mInternalFlags.set(MOVEMENT_ENABLED, enable);
	}

	inline float MeshAnimationTestComponent::getMovementA() const
	{
		return mMovement[0];
	}

	inline void MeshAnimationTestComponent::setMovementA(float movement)
	{
		mMovement[0] = movement;
	}

	inline float MeshAnimationTestComponent::getMovementB() const
	{
		return mMovement[1];
	}

	inline void MeshAnimationTestComponent::setMovementB(float movement)
	{
		mMovement[1] = movement;
	}

	inline bool MeshAnimationTestComponent::getRestart() const
	{
		return (mCurrentTime < 0.25f);
	}

	inline void MeshAnimationTestComponent::setRestart(bool)
	{
		if (isRunning())
		{
			restartAnimation();
		}
	}

	inline MeshAnimationTestComponent::BlendSynchronization MeshAnimationTestComponent::getBlendSyncMode() const
	{
		return mBlendSyncMode;
	}

	inline void MeshAnimationTestComponent::setBlendSyncMode(BlendSynchronization blendSyncMode)
	{
		mBlendSyncMode = blendSyncMode;
	}

	inline bool MeshAnimationTestComponent::getMixAnimation() const
	{
		return mMixAnimation;
	}

	inline const std::string& MeshAnimationTestComponent::getMaskBones() const
	{
		return mMaskBones;
	}

	inline float MeshAnimationTestComponent::getCurrentTime() const
	{
		return mCurrentTime;
	}

	inline void MeshAnimationTestComponent::setCurrentTime(float currentTime)
	{
		mCurrentTime = currentTime;
	}

	inline bool MeshAnimationTestComponent::getUseRandomOffset() const
	{
		return mUseRandomOffset;
	}

	inline void MeshAnimationTestComponent::setUseRandomOffset(bool useRandomOffset)
	{
		mUseRandomOffset = useRandomOffset;
	}

	inline void MeshAnimationTestComponent::forceRestart(const qsf::Time& blendTime)
	{
		restartAnimation(blendTime);
	}

	inline bool MeshAnimationTestComponent::isTestComponentUpdatesEnabled() const
	{
		return mTestComponentUpdatesEnabled;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
