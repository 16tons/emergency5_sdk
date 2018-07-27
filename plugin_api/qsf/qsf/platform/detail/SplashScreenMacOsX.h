// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/platform/detail/SplashScreenImpl.h"

#include <string>
#include <thread>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Mac OS X splash screen implementation
	*/
	class SplashScreenMacOsX : public SplashScreenImpl
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] splashScreen
		*    The UTF-8 filename in platform-independent notation of the splash screen to show
		*/
		explicit SplashScreenMacOsX(const std::string& splashScreen);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SplashScreenMacOsX();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::SplashScreenImpl methods          ]
	//[-------------------------------------------------------]
	public:
		virtual void show(const std::string& statusMessage, uint64 millisecondsToShow = 0, handle nativeParentWindowHandle = NULL_HANDLE) override;
		virtual void hide() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void hideSplashScreen();
		void splashScreenThread();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		handle mSplashScreenImageHandle;	///< The image handle of the loaded splash screen image
		handle mWindowHandle;				///< The window handle of the splash screen
		handle mParentWindowHandle;			///< The given parent window handle
		std::string mStatusMessage;			///< The UTF-8 status message to show
		std::thread mThread;				///< The thread that is used to hide the window again after the given time has passed
		std::mutex mMutex;
		std::chrono::system_clock::time_point mHideTime;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
