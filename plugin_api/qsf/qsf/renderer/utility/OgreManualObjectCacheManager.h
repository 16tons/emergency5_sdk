// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

#include <queue>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class ManualObject;
}
namespace qsf
{
	class MapSceneManager;
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
	*    Meter box instance manager class
	*/
	class QSF_API_EXPORT OgreManualObjectCacheManager
	{


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		static const size_t MAXIMUM_FREE_INSTANCES;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] MapSceneManager
		*    Owner map scene manager instance
		*/
		explicit inline OgreManualObjectCacheManager(MapSceneManager& mapSceneManager);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~OgreManualObjectCacheManager();

		void clear();

		Ogre::ManualObject* getUnusedOgreManualObject();
		void putUnusedOgreManualObject(Ogre::ManualObject* ogreManualObject);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void freeUnusedOgreManualObject(Ogre::ManualObject* ogreManualObject);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		MapSceneManager&				mMapSceneManager;
		std::queue<Ogre::ManualObject*>	mUnusedOgreManualObjects;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/utility/OgreManualObjectCacheManager-inl.h"
