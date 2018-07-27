// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/error/BoostAssertHandler.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	inline MaterialSystem& MaterialSystemCacheManager::getMaterialSystem() const
	{
		return mMaterialSystem;
	}

	inline MaterialSystemCacheMonitor& MaterialSystemCacheManager::getMaterialSystemCacheMonitor() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mMaterialSystemCacheMonitor, "The material system cache monitor instance is invalid", QSF_REACT_THROW);
		return *mMaterialSystemCacheMonitor;
	}

	inline void MaterialSystemCacheManager::markFileCacheAsDirty()
	{
		// The file cache is considered to be dirty now
		mDirtyFileCache = true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
