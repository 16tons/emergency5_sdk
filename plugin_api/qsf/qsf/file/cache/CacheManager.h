// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/manager/Manager.h"

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
	*    Cache manager class
	*/
	class QSF_API_EXPORT CacheManager : public Manager
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		CacheManager();

		/**
		*  @brief
		*    Destructor
		*/
		~CacheManager();

		/**
		*  @brief
		*    Get the cache directory
		*/
		const std::string& getCacheDirectory() const;

		/**
		*  @brief
		*    Get the startup cache directory
		*
		*  @remarks
		*    The startup cache directory is only readable and serves as a fallback.
		*    In contrast to the writable cache directory, the startup cache it is usually part of the application installation.
		*/
		const std::string& getStartupCacheDirectory() const;

		/**
		*  @brief
		*    Set the cache directory
		*/
		void setupCacheDirectory(const std::string& cacheDirectory, const std::string& startupCacheDirectory);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::string mCacheDirectory;		///< The UTF-8 cache directory of the application in platform-independent notation (e.g. on Windows: "C:/MyApplication")
		std::string mStartupCacheDirectory;	///< The UTF-8 startup cache directory of the application in platform-independent notation (e.g. on Windows: "C:/MyApplication")


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
