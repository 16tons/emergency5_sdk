// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/file/FileStream.h"
#include "qsf/base/NamedIdentifier.h"

#include <vector>
#include <map>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class FileListing;
	class FileCacheManager;
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
	*    File cache class
	*
	*  @remarks
	*    Holds copies of files in memory for fast access, and caches this data inside a single file.
	*    Use this only for frequently accessed small files like prefab definitions, so that not too much memory is wasted for caching.
	*/
	class QSF_API_EXPORT FileCache
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class FileCacheManager;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const std::string FORMAT_TYPE;		///< File cache format type string "FileCache"
		static const uint32		 FORMAT_VERSION;	///< Current file cache format major version

		enum class ComparisonPolicy
		{
			FILE_DATE_AND_SIZE,		///< Cache is up-to-date for a file if both modification date and file size are as cached (more secure, especially with modding support)
			FILE_SIZE_ONLY			///< Cache is up-to-date for a file if file size is as cached (less secure, but allows for shipping a file cache)
		};


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static void setComparisonPolicyForAllFileCaches(ComparisonPolicy comparisonPolicy);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		FileCache();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~FileCache();

		/**
		*  @brief
		*    Initialize the prefab cache
		*
		*  @remarks
		*    This will load the cache file in case there is one and make sure all cached file content is up-to-date.
		*    Also, the updated cache is written to the cache file again, if there were any changes required.
		*
		*  @param[in] cacheFilename
		*    UTF-8 absolute file name where to load and save the cache
		*  @param[in] filesToCache
		*    Listing of UTF-8 local file names to hold in the cache
		*  @param[in] calculateHashes
		*    If "true", calculate file hashes for all cached files
		*  @param[in] forceLoadAllFiles
		*    If "true", all files to cache will be loaded immediately; otherwise they are loaded on first access, if not cached already
		*/
		void initializeCache(const std::string& cacheFilename, const std::vector<std::string>& filesToCache, bool calculateHashes = false, bool forceLoadAllFiles = false);
		void initializeCache(const std::string& cacheFilename, const FileListing& fileListing, bool calculateHashes = false, bool forceLoadAllFiles = false);

		/**
		*  @brief
		*    Access content of a cached file
		*
		*  @param[in] filename
		*    UTF-8 local file name of the cached file to access, or load if necessary
		*
		*  @return
		*    The cached content of the file, or a null pointer in case there is no file content cache available
		*
		*  @note
		*    - If a null pointer is returned, you should manually open the file without using the file cache (e.g. using "qsf::FileStream")
		*/
		const std::vector<char>* getFileContent(const std::string& filename);

		/**
		*  @brief
		*    Get content hash of a cached file
		*
		*  @param[in] filename
		*    UTF-8 local file name of the cached file
		*
		*  @return
		*    The content hash of the file, may be "getUninitialized<uint64>()" in case no hash is available
		*/
		uint64 getFileHash(const std::string& filename);

		/**
		*  @brief
		*    Access processed content associated with a cached file
		*
		*  @param[in] filename
		*    UTF-8 local file name of the cached file
		*
		*  @return
		*    The processed content, or a null pointer in case there is no processed content cache available
		*
		*  @note
		*    - Usage of processed content is optional; this will always return a null pointer if no processed content is manually set
		*    - It can be useful if you can derive data from the file content that allows faster loading than the file content itself
		*    - If a null pointer is returned, you should get the file content, calculate the processed content and set it
		*    - Processed content gets removed automatically in case the cached file changes
		*/
		const std::vector<char>* getProcessedContent(const std::string& filename);

		/**
		*  @brief
		*    Set processed content associated with a cached file
		*
		*  @param[in] filename
		*    UTF-8 local file name of the cached file
		*  @param[in] content
		*    The processed content to set
		*
		*  @note
		*    - This will have no effect in case the cached file does not exist
		*/
		void setProcessedContent(const std::string& filename, const std::vector<char>& content);


	//[-------------------------------------------------------]
	//[ Protected definitions                                 ]
	//[-------------------------------------------------------]
	protected:
		struct Entry
		{
			enum State
			{
				NOT_LOADED,		///< Entry is not loaded, both mFileContent and mProcessedContent are empty
				BROKEN,			///< Entry refers to a file that does not exist
				READY,			///< Entry is loaded, mFileContent contains file content, mProcessedContent is empty
				PROCESSED		///< Entry is loaded, mProcessedContent contains processed content
			};

			std::vector<char> mFileContent;
			std::vector<char> mProcessedContent;
			State  mState;					///< Entry state
			int64  mLastModificationTime;	///< Date of last modification of the cached file
			int64  mFileSize;				///< Last known size of the cached file
			uint64 mFileHash;				///< Hash of the file's content; if not calculated, this is "qsf::getUninitialized<uint64>()"
			bool   mChecked;				///< Set to "true" if this entry is part of the cached file list passed to "initializeCache" (only used temporarily)

			Entry() : mState(NOT_LOADED), mLastModificationTime(getUninitialized<uint64>()), mFileSize(getUninitialized<uint64>()), mFileHash(getUninitialized<uint64>()), mChecked(false) {}
		};

		typedef std::map<NamedIdentifier, Entry> EntryMap;	// Use an std::map here by intent, as we need stable instances


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		void clearCache();
		bool loadCache();
		void saveCache();

		Entry* getEntry(const NamedIdentifier& filename);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		uint32 mCacheSubclassVersion;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		uint16 getSerializationVersion() const;
		void serializeCache(BinarySerializer& serializer);
		void accessEntry(EntryMap::value_type& pair);
		void deleteBrokenEntries();
		void invalidateFileCacheManager();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		FileCacheManager* mManager;
		std::string		  mCacheFilename;
		EntryMap		  mEntries;			///< Maps file names to "Entry" instance with stored file content
		bool			  mCalculateHashes;
		bool			  mDirtyCacheFile;	///< "true" if the cache file needs to be saved again

		static ComparisonPolicy sComparisonPolicy;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
