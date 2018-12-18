// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/platform/WindowsHeader.h"
#include "qsf/platform/detail/SplashScreenImpl.h"

#include <string>


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
	*    Win32 splash screen implementation
	*/
	class SplashScreenWin32 : public SplashScreenImpl
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
		explicit SplashScreenWin32(const std::string& splashScreen);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SplashScreenWin32();


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
		/**
		*  @brief
		*    Paint the Windows splash screen
		*/
		void paint();

		/**
		*  @brief
		*    Paint the Windows splash screen status message
		*
		*  @param[in] paintDC
		*    Paint device context, must be valid
		*  @param[in] windowWidth
		*    Window width
		*/
		void paintStatusMessage(HDC paintDC, LONG windowWidth);


	//[-------------------------------------------------------]
	//[ Private static MS Windows callback function           ]
	//[-------------------------------------------------------]
	private:
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT nMessage, WPARAM wParam, LPARAM lParam);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::string mSplashScreen;				///< The UTF-8 filename in platform-independent notation of the splash screen to show
		HBITMAP		mSplashScreenBitmap;		///< Splash screen Windows bitmap, can be a null handle
		HWND		mNativeParentWindowHandle;	///< Native parent window handle, can be a null handle
		HWND		mNativeWindowHandle;		///< Native window handle, can be a null handle
		std::string mStatusMessage;				///< The UTF-8 status message to show


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
