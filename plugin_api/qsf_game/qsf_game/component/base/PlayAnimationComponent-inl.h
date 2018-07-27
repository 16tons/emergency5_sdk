// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline PlayAnimationComponent::PlayAnimationComponent(Prototype* prototype) :
			Component(prototype),
			mSpeed(1.0f),
			mInternalFlags(LOOP_ANIMATION),
			mBoneTranslationRetargeting(MeshAnimationComponent::ANIMATION_BONE_SCALE_RETARGETING)
		{
			// Nothing to do in here
		}

		inline PlayAnimationComponent::~PlayAnimationComponent()
		{
			// Nothing to do in here
		}

		inline const AssetProxy& PlayAnimationComponent::getSkeleton() const
		{
			return mSkeleton;
		}

		inline bool PlayAnimationComponent::getLoop() const
		{
			return mInternalFlags.isSet(LOOP_ANIMATION);
		}

		inline bool PlayAnimationComponent::getReverseAnimation()
		{
			return mInternalFlags.isSet(REVERSE_ANIMATION);
		}

		inline float PlayAnimationComponent::getSpeed() const
		{
			return mSpeed;
		}

		inline MeshAnimationComponent::BoneRetargeting PlayAnimationComponent::getBoneTranslationRetargeting() const
		{
			return mBoneTranslationRetargeting;
		}

		inline void PlayAnimationComponent::setBoneTranslationRetargeting(MeshAnimationComponent::BoneRetargeting boneRetargeting)
		{
			if (mBoneTranslationRetargeting != boneRetargeting)
			{
				mBoneTranslationRetargeting = boneRetargeting;
				restartAnimation();
			}
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf
