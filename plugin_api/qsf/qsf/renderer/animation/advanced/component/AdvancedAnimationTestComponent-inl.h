// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline AdvancedAnimationTestComponent::AdvancedAnimationTestComponent(Prototype* prototype) :
		AdvancedAnimationComponent(prototype),
		mKeyFrameAnimationNode1(nullptr),
		mKeyFrameAnimationNode2(nullptr),
		mAdditiveBlendingNode(nullptr),
		mLocomotionNode(nullptr),
		mBoneLookAtNode(nullptr)
	{
		// Nothing to do in here
	}

	inline AdvancedAnimationTestComponent::~AdvancedAnimationTestComponent()
	{
		// Nothing to do in here
	}

	inline const AssetProxy& AdvancedAnimationTestComponent::getSkeletonAnimation1() const
	{
		return mAnimationProxy1;
	}

	inline const AssetProxy& AdvancedAnimationTestComponent::getSkeletonAnimation2() const
	{
		return mAnimationProxy2;
	}

	inline const std::string& AdvancedAnimationTestComponent::getLeftFootBone() const
	{
		return mLeftFootBone;
	}

	inline const std::string& AdvancedAnimationTestComponent::getRightFootBone() const
	{
		return mRightFootBone;
	}

	inline const std::string& AdvancedAnimationTestComponent::getNeckBone() const
	{
		return mNeckBone;
	}

	inline const glm::vec3& AdvancedAnimationTestComponent::getWalkingCycle() const
	{
		return mWalkingCycle;
	}

	inline uint64 AdvancedAnimationTestComponent::getHeadLookAtEntityId() const
	{
		return mHeadLookAtEntity.valid() ? mHeadLookAtEntity->getId() : getUninitialized<uint64>();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
