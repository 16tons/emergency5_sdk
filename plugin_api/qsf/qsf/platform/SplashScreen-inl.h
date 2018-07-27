// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/platform/detail/SplashScreenImpl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline void SplashScreen::show(const std::string& statusMessage, uint64 millisecondsToShow, handle nativeParentWindowHandle) const
	{
		// Redirect to the concrete implementation
		mSplashScreen->show(mShowInfoTexts ? statusMessage : "", millisecondsToShow, nativeParentWindowHandle);
	}

	inline void SplashScreen::hide() const
	{
		mSplashScreen->hide();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
