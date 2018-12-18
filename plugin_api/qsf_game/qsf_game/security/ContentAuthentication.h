// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/Export.h"

#include <qsf/base/NamedIdentifier.h>

#include <boost/noncopyable.hpp>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class FileCache;
	class FileListing;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Content authentication class
		*/
		class QSF_GAME_API_EXPORT ContentAuthentication : public boost::noncopyable
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			// Checksum data type
			typedef uint64 Checksum;

			struct FileEntry
			{
				std::string mFilePath;			///< UTF-8 local file path
				Checksum mChecksum;				///< Checksum for that file
				bool mWasChecked;				///< "true" in case a checksum was created at least once; note that it may be out-dated nevertheless if the file was changed in the meantime
				int64 mLastModificationTime;	///< Date of last modification of the file; must be identical to actual file's modification date for the checksum to be valid

				FileEntry(const std::string& filepath) : mFilePath(filepath), mChecksum(getUninitialized<Checksum>()), mWasChecked(false), mLastModificationTime(0) {}
			};

			struct Category
			{
				NamedIdentifier mName;		///< Unique name of this category
				std::vector<FileEntry> mFileEntries;
				Checksum mChecksum;			///< Accumulated checksum for all files contained in this category

				Category() : mChecksum(0) {}
			};
			typedef std::map<StringHash, Category> CategoryMap;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			ContentAuthentication();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~ContentAuthentication();

			void clear();

			void addFiles(const StringHash& categoryName, const std::vector<std::string>& fileList);
			void addFiles(const StringHash& categoryName, const FileListing& fileListing);

			void addHashProvider(FileCache& fileCache);

			void setCheckFileModificationDates(bool check);

			Checksum getChecksum(const StringHash& categoryName);
			bool testChecksum(const StringHash& categoryName, Checksum checksum);

			void debugLogChecksums();


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			uint64 getFileHash(const std::string& filename) const;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			CategoryMap mCategories;
			std::vector<FileCache*> mHashProviders;		///< List of class instances we can ask for cached file hashes; do not destroy the instances
			bool mCheckFileModificationDates;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf
