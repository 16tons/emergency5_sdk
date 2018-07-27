// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/file/cache/FileCache.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Prototype;
	class PrototypeManager;
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
	*    Prefab cache class
	*
	*  @remarks
	*    Holds copies of prefab files in memory for fast access, and caches this data inside a single file.
	*/
	class QSF_API_EXPORT PrefabCache : public FileCache
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PrefabCache();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~PrefabCache();

		/**
		*  @brief
		*    Initialize the prefab cache
		*
		*  @remarks
		*    This will load the prefab cache file in case there is one and make sure all cached prefabs are up-to-date.
		*    Also, the updated cache is written to the cache file again, if there were any changes required.
		*
		*  @param[in] cacheFilename
		*    UTF-8 absolute file name where to load and save the cache
		*  @param[in] calculateHashes
		*    If "true", calculate file hashes for all cached files
		*  @param[in] forceLoadAllFiles
		*    If "true", all files to cache will be loaded immediately; otherwise they are loaded on first access, if not cached already
		*/
		void initialize(const std::string& cacheFilename, bool calculateHashes = false, bool forceLoadAllFiles = false);

		/**
		*  @brief
		*    Load a prefab from the cache
		*
		*  @param[out] prototypeManager
		*    Prototype manager to load the prototype into
		*  @param[in] filename
		*    UTF-8 local file name of the prefab file to load
		*
		*  @return
		*    A pointer to the prefab's main prototype, or a null pointer in case of error
		*
		*  @note
		*    - If a null pointer is returned, you should manually load the prefab file without using the file cache
		*/
		Prototype* loadPrefab(PrototypeManager& prototypeManager, const std::string& filename);


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
