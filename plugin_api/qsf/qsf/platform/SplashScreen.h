// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

#include <boost/noncopyable.hpp>

#include <string>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class SplashScreenImpl;
}


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
	*    Splash screen class
	*/
	class QSF_API_EXPORT SplashScreen : public boost::noncopyable
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
		*  @param[in] showInfoTexts
		*    Show or hide info texts?
		*/
		explicit SplashScreen(const std::string& splashScreen, bool showInfoTexts = true);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SplashScreen();

		/**
		*  @brief
		*    Show the splash screen
		*
		*  @param[in] statusMessage
		*    The UTF-8 status message to show
		*  @param[in] millisecondsToShow
		*    The milliseconds to show the splash screen
		*  @param[in] nativeParentWindowHandle
		*    The native parent window handle, can be a null handle
		*/
		inline void show(const std::string& statusMessage, uint64 millisecondsToShow = 0, handle nativeParentWindowHandle = NULL_HANDLE) const;

		/**
		*  @brief
		*    Hide the splash screen
		*/
		inline void hide() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		SplashScreenImpl* mSplashScreen;	///< Splash screen implementation, always valid, we're responsible for destroying the instance in case we no longer need it
		bool			  mShowInfoTexts;	///< If "false", don't show text messages


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/platform/SplashScreen-inl.h"
