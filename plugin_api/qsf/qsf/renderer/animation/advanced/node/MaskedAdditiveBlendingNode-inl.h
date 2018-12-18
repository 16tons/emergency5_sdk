// Copyright (C) 2012-2018 Promotion Software GmbH


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
	inline MaskedAdditiveBlendingNode::~MaskedAdditiveBlendingNode()
	{
		// Nothing to do
	}

	inline BoneMaskHandle MaskedAdditiveBlendingNode::getBoneMask(uint16 inputId) const
	{
		if (inputId >= getNumberOfInputSlots())
			return BoneMaskHandle();

		return mBoneMasks[inputId];
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
