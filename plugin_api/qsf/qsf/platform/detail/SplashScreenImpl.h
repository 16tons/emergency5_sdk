// Copyright (C) 2012-2017 Promotion Software GmbH


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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract splash screen interface
	*/
	class QSF_API_EXPORT SplashScreenImpl : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline SplashScreenImpl();

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~SplashScreenImpl();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::SplashScreenImpl methods          ]
	//[-------------------------------------------------------]
	public:
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
		virtual void show(const std::string& statusMessage, uint64 millisecondsToShow = 0, handle nativeParentWindowHandle = NULL_HANDLE) = 0;

		/**
		*  @brief
		*    Hide the splash screen
		*/
		virtual void hide() = 0;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/platform/detail/SplashScreenImpl-inl.h"
