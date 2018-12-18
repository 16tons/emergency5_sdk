// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline PlaySkeletalAnimationComponent::PlaySkeletalAnimationComponent(Prototype* prototype) :
		AdvancedAnimationComponent(prototype),
		mBlendingNode(nullptr),
		mTimeNotRendered(0.0f),
		mBlendLength(0.0f),
		mLoop(true),
		mReverse(false),
		mSpeed(1.0f),
		mBlendSynchronization(SYNC_NONE)
	{
		// Nothing to do in here
	}

	inline PlaySkeletalAnimationComponent::~PlaySkeletalAnimationComponent()
	{
		if (mInverseBoneMask.isValid())
		{
			removeBoneMask(mInverseBoneMask);
		}
	}

	inline const AssetProxy& PlaySkeletalAnimationComponent::getMainSkeletonAnimation() const
	{
		return mAnimationProxy;
	}

	inline float PlaySkeletalAnimationComponent::getBlendLength() const
	{
		return mBlendLength;
	}

	inline void PlaySkeletalAnimationComponent::setBlendLength(float blendLength)
	{
		assignAndPromoteChange(mBlendLength, blendLength, BLEND_LENGTH);
	}

	inline bool PlaySkeletalAnimationComponent::getLoop() const
	{
		return mLoop;
	}

	inline bool PlaySkeletalAnimationComponent::getReverse() const
	{
		return mReverse;
	}

	inline float PlaySkeletalAnimationComponent::getSpeed() const
	{
		return mSpeed;
	}

	inline PlaySkeletalAnimationComponent::BlendSynchronization PlaySkeletalAnimationComponent::getBlendSynchronization() const
	{
		return mBlendSynchronization;
	}

	inline bool PlaySkeletalAnimationComponent::hasAnimation(const AssetProxy& animationProxy) const
	{
		return (getChannel(animationProxy) != nullptr);
	}

	inline bool PlaySkeletalAnimationComponent::isAnimationLooped(const AssetProxy& animationProxy) const
	{
		const PlayChannel* channel = getChannel(animationProxy);
		return (channel != nullptr) ? channel->loop : false;
	}

	inline void PlaySkeletalAnimationComponent::setAnimationLoop(const AssetProxy& animationProxy, bool loop)
	{
		updateNonRenderedTimeToProgress();

		PlayChannel* channel = getChannel(animationProxy);
		if (channel != nullptr)
		{
			channel->pause = false;
			channel->loop = loop;
		}
	}

	inline bool PlaySkeletalAnimationComponent::isAnimationReversed(const AssetProxy& animationProxy) const
	{
		const PlayChannel* channel = getChannel(animationProxy);
		return ((channel != nullptr) ? glm::sign(channel->speed) != 1.0f : false);
	}

	inline void PlaySkeletalAnimationComponent::setAnimationReverse(const AssetProxy& animationProxy, bool reverse)
	{
		updateNonRenderedTimeToProgress();

		PlayChannel* channel = getChannel(animationProxy);
		if (channel != nullptr)
			channel->speed = (reverse ? -1.0f : 1.0f) * glm::abs(channel->speed);
	}

	inline float PlaySkeletalAnimationComponent::getAnimationSpeed(const AssetProxy& animationProxy) const
	{
		const PlayChannel* channel = getChannel(animationProxy);
		return (channel != nullptr) ? channel->speed : 0.0f;
	}

	inline void PlaySkeletalAnimationComponent::setAnimationSpeed(const AssetProxy& animationProxy, float speed)
	{
		updateNonRenderedTimeToProgress();

		PlayChannel* channel = getChannel(animationProxy);
		if (channel != nullptr)
			channel->speed = speed;
	}

	inline void PlaySkeletalAnimationComponent::setAllAnimationsSpeed(float speed)
	{
		updateNonRenderedTimeToProgress();

		for (PlayChannel& channel : mChannel)
		{
			channel.speed = speed;
		}
	}

	inline BoneMaskHandle PlaySkeletalAnimationComponent::getAnimationBoneMask(const AssetProxy& animationProxy)
	{
		const PlayChannel* channel = getChannel(animationProxy);
		return (channel != nullptr) ? channel->boneMask : BoneMaskHandle();
	}

	inline float PlaySkeletalAnimationComponent::getAnimationWeight(const AssetProxy& animationProxy) const
	{
		const PlayChannel* channel = getChannel(animationProxy);
		return (channel != nullptr) ? channel->weight : 0.0f;
	}

	inline void PlaySkeletalAnimationComponent::setAnimationWeight(const AssetProxy& animationProxy, float weight)
	{
		updateNonRenderedTimeToProgress();

		PlayChannel* channel = getChannel(animationProxy);
		if (channel != nullptr)
			channel->weight = weight;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
