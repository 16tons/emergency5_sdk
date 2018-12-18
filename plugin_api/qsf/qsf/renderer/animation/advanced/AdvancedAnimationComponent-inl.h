// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline AdvancedAnimationComponent::AdvancedAnimationComponent(Prototype* prototype) :
		Component(prototype),
		mRootAnimationNode(nullptr),
		mSkeletonGroup(nullptr),
		mUpdatedOnNotRendered(true)
	{
		// Nothing here
	}

	inline const SkeletonHandle* AdvancedAnimationComponent::getRootOutputSkeleton() const
	{
		return (mRootAnimationNode != nullptr) ? mRootAnimationNode->getOutputSkeleton() : nullptr;
	}

	inline uint16 AdvancedAnimationComponent::getBoneId(uint16 ogreHandle) const
	{
		return (mSkeletonGroup != nullptr) ? mSkeletonGroup->getBoneIdFromOgreHandle(ogreHandle) : getUninitialized<uint16>();
	}

	inline BoneMaskHandle AdvancedAnimationComponent::createBoneMask()
	{
		return (mSkeletonGroup != nullptr) ? BoneMaskHandle(*mSkeletonGroup, mSkeletonGroup->registerBoneMaskId()) : BoneMaskHandle();
	}

	inline void AdvancedAnimationComponent::removeBoneMask(BoneMaskHandle& boneMask)
	{
		// Remove can be called from the bone mask directly now
		boneMask.remove();
	}

	inline bool AdvancedAnimationComponent::checkBoneMask(const BoneMaskHandle& boneMask) const
	{
		return boneMask.isValid() && boneMask.getSkeletonGroup() == mSkeletonGroup;
	}

	inline bool AdvancedAnimationComponent::isUpdatedOnNotRendered() const
	{
		return mUpdatedOnNotRendered;
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	inline uint16 AdvancedAnimationComponent::getAnimationNodeNumber() const
	{
		return static_cast<uint16>(mAnimationNodes.size());
	}

	inline const std::vector<AnimationNode*>& AdvancedAnimationComponent::getAnimationNodesVector() const
	{
		return mAnimationNodes;
	}

	template<class T> T& AdvancedAnimationComponent::createAnimationNode()
	{
		T* node = new T(*this);

		mAnimationNodes.push_back(node);

		return *node;
	}

	template<class T> T& AdvancedAnimationComponent::createAnimationNode(uint16 numberOfInputSlots)
	{
		T* node = new T(*this, numberOfInputSlots);

		mAnimationNodes.push_back(node);

		return *node;
	}

	inline bool AdvancedAnimationComponent::checkProblem(Problem problem) const
	{
		return mProblemBitMask.isClear(problem);
	}

	inline qsf::Time AdvancedAnimationComponent::getAnimationTimeLength(const std::string& animationName) const
	{
		return qsf::Time::fromSeconds(getKeyFrameSkeletonTimeLength(animationName));
	}


	//[-------------------------------------------------------]
	//[ Private methods for qsf::AnimationNode methods        ]
	//[-------------------------------------------------------]
	inline uint16 AdvancedAnimationComponent::registerSkeletonId()
	{
		return (mSkeletonGroup != nullptr) ? mSkeletonGroup->registerSkeletonId() : getUninitialized<uint16>();
	}

	inline void AdvancedAnimationComponent::unregisterSkeletonId(uint16 skeletonId)
	{
		if (mSkeletonGroup != nullptr)
		{
			mSkeletonGroup->unregisterSkeletonId(skeletonId);
		}
	}

	inline SkeletonGroup* AdvancedAnimationComponent::getSkeletonGroup()
	{
		return mSkeletonGroup;
	}

	inline void AdvancedAnimationComponent::tellProblem(Problem problem)
	{
		mProblemBitMask.set(problem);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	inline Ogre::v1::Entity* AdvancedAnimationComponent::getOgreEntity() const
	{
		if (!mMeshComponent.valid())
		{
			mMeshComponent = getEntity().getComponent<MeshComponent>();
		}
		return mMeshComponent.valid() ? mMeshComponent->getOgreEntity() : nullptr;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
