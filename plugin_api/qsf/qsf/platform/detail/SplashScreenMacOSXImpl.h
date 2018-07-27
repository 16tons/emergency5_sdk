// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/platform/PlatformTypes.h"

#include <string>


//[-------------------------------------------------------]
//[ C++ declarations and definitions                      ]
//[-------------------------------------------------------]
extern "C++"
{


	//[-------------------------------------------------------]
	//[ Namespace                                             ]
	//[-------------------------------------------------------]
	namespace qsf
	{
		namespace macosx
		{
			handle CreateNsImageFromBitmap(const std::string& filename);

			void FreeNsImage(handle nsImageHandle);

			handle CreateSplashScreen(handle nsImageHandle, handle parentWindowHandle);

			void SetSplashScreenStatusMessage(handle splashScreenWindow, const std::string& statusMessage, float marginLeft, float marginTop, float marginRight, float height);

			void FreeSplashScreen(handle splashScreenWindow);
		}
	}
}
