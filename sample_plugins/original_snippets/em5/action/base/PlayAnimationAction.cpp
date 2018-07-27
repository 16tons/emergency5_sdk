// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/game/animation/AnimationHelper.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier PlayAnimationAction::ACTION_ID	= "em5::PlayAnimationAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PlayAnimationAction::PlayAnimationAction() :
		Action(ACTION_ID),
		mWaitForFinish(false),
		mLoopAnimation(false),
		mReverse(false),
		mBoneTranslationRetargeting(qsf::MeshAnimationComponent::ANIMATION_BONE_SCALE_RETARGETING),
		mClearPersonAnimation(false),
		mPreventUpdateAnimations(false),
		mEnforceGraphicsSynchronization(false)
	{
		// Nothing here
	}

	PlayAnimationAction::~PlayAnimationAction()
	{
		// Nothing here
	}

	void PlayAnimationAction::init(const std::string& animationPath, bool waitForFinish, bool loopAnimation, bool reverse, qsf::Time blendTime, boost::optional<qsf::Time> maxPlayTime)
	{
		mAnimationAssetProxy.setLocalAssetName(animationPath);
		mWaitForFinish = waitForFinish;
		mLoopAnimation = loopAnimation;
		mReverse = reverse;
		mBlendTime = blendTime;
		mMaxPlayTime = maxPlayTime;
	}

	void PlayAnimationAction::initWithAnimationRetargeting(const std::string& animationPath, qsf::MeshAnimationComponent::BoneRetargeting boneTranslationRetargeting, bool waitForFinish, bool loopAnimation, bool reverse)
	{
		mAnimationAssetProxy.setLocalAssetName(animationPath);
		mWaitForFinish = waitForFinish;
		mLoopAnimation = loopAnimation;
		mReverse = reverse;
		mBlendTime = qsf::Time::ZERO;
	}

	void PlayAnimationAction::initClearPersonAnimation()
	{
		mClearPersonAnimation = true;
	}

	void PlayAnimationAction::setAnimationOffsetRange(float startOffset, float endOffset)
	{
		mStartOffset = startOffset;
		mEndOffset = endOffset;
	}

	void PlayAnimationAction::setMaskedAnimation(const std::string& maskAnimationPath, const std::string& boneMask)
	{
		mMaskAnimationAssetProxy.setLocalAssetName(maskAnimationPath);
		mBoneMask = boneMask;
	}

	void PlayAnimationAction::setEnforceGraphicsSynchronization(bool value)
	{
		mEnforceGraphicsSynchronization = value;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void PlayAnimationAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mAnimationAssetProxy);
		serializer.serialize(mWaitForFinish);
		serializer.serialize(mLoopAnimation);
		serializer.serialize(mReverse);
		serializer.serialize(mBlendTime);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool PlayAnimationAction::onStartup()
	{
		if (startAnimation())
		{
			PersonComponent* personComponent = getEntity().getComponent<PersonComponent>();
			if (nullptr != personComponent)
			{
				mPreventUpdateAnimations = true;
				personComponent->preventUpdateAnimations(true);
			}

			return true;
		}

		return false;
	}

	void PlayAnimationAction::onShutdown()
	{
		if (mPreventUpdateAnimations)
		{
			PersonComponent* personComponent = getEntity().getComponent<PersonComponent>();
			if (nullptr != personComponent)
			{
				personComponent->preventUpdateAnimations(false);
				mPreventUpdateAnimations = false;
			}
		}
	}

	qsf::action::Result PlayAnimationAction::updateAction(const qsf::Clock& clock)
	{
		// No playing - just clearing up animations
		if (mClearPersonAnimation)
			return qsf::action::RESULT_DONE;

		// Check if the channel still exists
		if (!mMeshAnimationChannel.valid())
		{
			// Animation is finished
			return qsf::action::RESULT_DONE;
		}

		if (mEnforceGraphicsSynchronization)
		{
			// Work around for updating animations outside of the screen
			qsf::MeshAnimationComponent* meshAnimationComponent = getEntity().getOrCreateComponent<qsf::MeshAnimationComponent>();
			if (nullptr != meshAnimationComponent)
			{
				meshAnimationComponent->enforceGraphicsSynchronization();
			}
		}

		if (!mWaitForFinish)
		{
			// Don't wait, but abort immediately
			return qsf::action::RESULT_DONE;
		}

		if (mMeshAnimationChannel->isFinished())
		{
			// Animation is finished
			return qsf::action::RESULT_DONE;
		}

		if (mMaxPlayTime.is_initialized())
		{
			// Countdown
			*mMaxPlayTime -= clock.getTimePassed();

			if (*mMaxPlayTime <= qsf::Time::ZERO)
			{
				// Time over
				return qsf::action::RESULT_DONE;
			}
		}

		// Check for end offset
		if (mEndOffset.is_initialized())
		{
			if (mMeshAnimationChannel->getOffset() >= *mEndOffset)
			{
				return qsf::action::RESULT_DONE;
			}
		}

		// Wait for ending of the animation
		return qsf::action::RESULT_CONTINUE;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	bool PlayAnimationAction::startAnimation()
	{
		if (mClearPersonAnimation)
		{
			AnimationHelper(getEntity()).clearAnimation();
			return true;
		}

		// We need the mesh animation component
		qsf::MeshAnimationComponent* meshAnimationComponent = getEntity().getOrCreateComponent<qsf::MeshAnimationComponent>();
		if (nullptr == meshAnimationComponent)
		{
			return false;
		}

		// Search asset system for animation
		if (nullptr == mAnimationAssetProxy.getCachedAsset())
		{
			// Animation is not found in the asset system, abort action
			PersonComponent* personComponent = getEntity().getComponent<PersonComponent>();
			if (nullptr != personComponent)
			{
				// For persons, as fallback, "forget" the animation
				personComponent->clearAnimation();
			}
			return false;
		}

		qsf::MeshAnimationChannel* channel = nullptr;
		if (!mMaskAnimationAssetProxy.isValid() || nullptr == mMaskAnimationAssetProxy.getCachedAsset())
		{
			// Play animation with or without blending
			if (mBlendTime > qsf::Time::ZERO)
			{
				channel = meshAnimationComponent->blendToAnimation(mAnimationAssetProxy, mLoopAnimation, mBlendTime, mReverse, qsf::MeshAnimationComponent::SYNC_NONE, mBoneTranslationRetargeting);
			}
			else
			{
				channel = meshAnimationComponent->playAnimation(mAnimationAssetProxy, mLoopAnimation, mReverse, mBoneTranslationRetargeting);
			}
		}
		else
		{
			static std::vector<std::string> mask;
			mask.clear();
			mask.push_back(mBoneMask);

			if (mBlendTime > qsf::Time::ZERO)
			{
				channel = meshAnimationComponent->blendToMaskedAnimation(mMaskAnimationAssetProxy, mAnimationAssetProxy, mask, mLoopAnimation, mBlendTime, mReverse, mReverse, qsf::MeshAnimationComponent::SYNC_NONE, mBoneTranslationRetargeting, mBoneTranslationRetargeting).second;
			}
			else
			{
				channel = meshAnimationComponent->playMaskedAnimation(mMaskAnimationAssetProxy, mAnimationAssetProxy, mask, mLoopAnimation, mReverse, mReverse, mBoneTranslationRetargeting, mBoneTranslationRetargeting).second;
			}
		}

		if (nullptr != channel)
		{
			// Set start offset
			if (mStartOffset.is_initialized())
			{
				channel->setOffset(*mStartOffset);
			}

			mMeshAnimationChannel = channel;

			// Ready to go
			return true;
		}

		// Failed
		return false;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
