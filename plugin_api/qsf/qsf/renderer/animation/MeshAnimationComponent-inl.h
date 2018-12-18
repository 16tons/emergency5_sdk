// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline MeshAnimationComponent::MeshAnimationComponent(Prototype* prototype) :
		Component(prototype),
		mOgreEntity(nullptr),
		mSkeletonAnimationBlendMode(0),	// "Ogre::v1::SkeletonAnimationBlendMode::ANIMBLEND_AVERAGE"
		mUseBaseKeyFrame(false),
		mResetAnimationBinding(false),
		mDoAlwaysUpdateAnimation(false),
		mMeshChangedStateCount(0)
	{
		// Nothing here
	}

	inline MeshAnimationComponent::~MeshAnimationComponent()
	{
		// Nothing here
	}

	inline MeshAnimationChannel* MeshAnimationComponent::playAnimation(const AssetProxy& animationAsset, bool loop, bool reverse, BoneRetargeting boneTranslationRetargeting)
	{
		return playAnimationInternal(animationAsset, loop, reverse, boneTranslationRetargeting, isVisibleAnimation());
	}

	inline std::pair<MeshAnimationChannel*, MeshAnimationChannel*> MeshAnimationComponent::playMaskedAnimation(
		const AssetProxy& animationAsset1,
		const AssetProxy& animationAsset2,
		const std::vector<std::string>& boneMask,
		bool loop,
		bool reverse1,
		bool reverse2,
		BoneRetargeting boneTranslationRetargeting1,
		BoneRetargeting boneTranslationRetargeting2)
	{
		return playMaskedAnimationInternal(animationAsset1, animationAsset2, boneMask, loop, reverse1, reverse2, boneTranslationRetargeting1, boneTranslationRetargeting2, isVisibleAnimation());
	}

	inline MeshAnimationChannel* MeshAnimationComponent::blendToAnimation(
		const AssetProxy& animationAsset,
		bool loop,
		Time blendLength,
		bool reverse,
		BlendSynchronization blendSync,
		BoneRetargeting boneTranslationRetargeting)
	{
		return blendToAnimationInternal(animationAsset, loop, blendLength, reverse, blendSync, boneTranslationRetargeting, isVisibleAnimation());
	}

	inline std::pair<MeshAnimationChannel*, MeshAnimationChannel*> MeshAnimationComponent::blendToMaskedAnimation(
		const AssetProxy& animationAsset1,
		const AssetProxy& animationAsset2,
		const std::vector<std::string>& boneMask,
		bool loop,
		Time blendLength,
		bool reverse1,
		bool reverse2,
		BlendSynchronization blendSync,
		BoneRetargeting boneTranslationRetargeting1,
		BoneRetargeting boneTranslationRetargeting2)
	{
		return blendToMaskedAnimationInternal(animationAsset1, animationAsset2, boneMask, loop, blendLength, reverse1, reverse2, blendSync, boneTranslationRetargeting1, boneTranslationRetargeting2, isVisibleAnimation());
	}

	inline const MeshAnimationComponent::ChannelMap& MeshAnimationComponent::getChannels() const
	{
		return mChannels;
	}

	inline void MeshAnimationComponent::resetAnimationBindingNextTime()
	{
		mResetAnimationBinding = true;
	}

	inline bool MeshAnimationComponent::isDoAlwaysUpdateAnimation() const
	{
		return mDoAlwaysUpdateAnimation;
	}

	inline const MeshAnimationComponent::LastAnimatonData& MeshAnimationComponent::getLastAnimationData() const
	{
		return mLastAnimationData;
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	inline void MeshAnimationComponent::onShutdown()
	{
		stopAllAnimations();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	inline void MeshAnimationComponent::synchronizeOgreAnimationStates()
	{
		if (mAnimationJobProxy.isValid())
		{
			updateAnimation(0.0f, false);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
