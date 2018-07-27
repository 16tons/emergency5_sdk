// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline CacheManager& FileSystem::getCacheManager()
	{
		return mCacheManager;
	}

	inline FileCacheManager& FileSystem::getFileCacheManager()
	{
		return mFileCacheManager;
	}

	inline std::string FileSystem::getBaseDirectory() const
	{
		return mBaseDirectory;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::System methods                    ]
	//[-------------------------------------------------------]
	inline const char* FileSystem::getName() const
	{
		return "File system";
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
