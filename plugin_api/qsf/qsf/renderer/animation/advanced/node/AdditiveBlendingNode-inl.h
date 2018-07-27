// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline AdditiveBlendingNode::~AdditiveBlendingNode()
	{
		// Nothing to do
	}

	inline float AdditiveBlendingNode::getWeight(uint16 inputId) const
	{
		return (inputId >= getNumberOfInputSlots()) ? 0.0f : mWeights[inputId];
	}

	inline bool AdditiveBlendingNode::getAnimationAverage() const
	{
		return mAnimationAverage;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
