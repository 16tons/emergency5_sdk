// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/error/ErrorHandling.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline AnimationNode::~AnimationNode()
	{
		clearSkeleton(mOutputSkeleton);

		for (uint16 i = 0; i < MAXIMAL_NUMBER_OF_INPUTS; ++i)
		{
			clearSkeleton(mInputSkeletons[i]);
		}
	}

	inline uint16 AnimationNode::getNumberOfInputSlots() const
	{
		return mNumberOfInputSlots;
	}

	inline bool AnimationNode::isFirstInputReadOnly() const
	{
		return mReadOnlyFirstInput;
	}

	inline AnimationNode* AnimationNode::getConnectedNode(uint16 inputId) const
	{
		if (inputId > mNumberOfInputSlots)
			return nullptr;

		return mInputNodes[inputId];
	}

	inline SkeletonHandle* AnimationNode::getOutputSkeleton() const
	{
		return mOutputSkeleton;
	}

	inline const SkeletonHandle* AnimationNode::getInputSkeleton(uint16 inputId) const
	{
		if (inputId > mNumberOfInputSlots)
			return nullptr;

		return mInputSkeletons[inputId];
	}

	inline AdvancedAnimationComponent& AnimationNode::getAdvancedAnimationComponent()
	{
		return mAdvancedAnimationComponent;
	}

	inline const AdvancedAnimationComponent& AnimationNode::getAdvancedAnimationComponent() const
	{
		return mAdvancedAnimationComponent;
	}


	//[-------------------------------------------------------]
	//[ Private methods for qsf::AdvancedAnimationComponent   ]
	//[-------------------------------------------------------]
	inline bool AnimationNode::isOutputConnectionAllUsed() const
	{
		return (mOutputConnectionsUsed >= mOutputConnectionNumber);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
