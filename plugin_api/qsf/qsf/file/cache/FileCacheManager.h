// Copyright (C) 2012-2018 Promotion Software GmbH


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
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class FileCache;
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
	*    File cache manager class
	*/
	class QSF_API_EXPORT FileCacheManager : public Manager
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class FileCache;		// Needs to register itself here


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		FileCacheManager();

		/**
		*  @brief
		*    Destructor
		*/
		~FileCacheManager();

		/**
		*  @brief
		*    Access content of a cached file
		*
		*  @param[in] filename
		*    UTF-8 local file name of the cached file to load
		*
		*  @return
		*    The cached content of the file, or a null pointer in case there is no content cache available
		*
		*  @note
		*    - If a null pointer is returned, you should manually open the file without using the file cache (e.g. using "qsf::FileStream")
		*/
		const std::vector<char>* getFileContent(const std::string& filename);

		/**
		*  @brief
		*    Clear cache
		*
		*  @note
		*    - Do only call this method if you know what you're doing
		*/
		void clearCache();


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void registerFileCache(FileCache& fileCache);
		void unregisterFileCache(FileCache& fileCache);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::vector<FileCache*> mFileCacheList;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
