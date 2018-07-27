// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline OgreManualObjectCacheManager::OgreManualObjectCacheManager(MapSceneManager& mapSceneManager) :
		mMapSceneManager(mapSceneManager)
	{
		// Nothing to do here
	}

	inline OgreManualObjectCacheManager::~OgreManualObjectCacheManager()
	{
		clear();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
