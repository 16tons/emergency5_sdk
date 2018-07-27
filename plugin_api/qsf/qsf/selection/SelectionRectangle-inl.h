// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/error/BoostAssertHandler.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline RenderWindow& SelectionRectangle::getRenderWindow() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mRenderWindow, "The render window instance is invalid", QSF_REACT_THROW);
		return *mRenderWindow;
	}

	inline bool SelectionRectangle::isDragInProgress() const
	{
		return mDragInProgress;
	}

	inline bool SelectionRectangle::hasSelectionRectangle() const
	{
		return mHasSelectionRectangle;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
