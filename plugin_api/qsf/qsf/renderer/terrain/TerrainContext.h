// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/Context.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class TerrainGlobalOptions;
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
	*    Static terrain context context
	*/
	class QSF_API_EXPORT TerrainContext : public Context
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Add a context reference
		*
		*  @note
		*    - If this is the first reference, the internal global terrain options are created automatically
		*/
		static void addContextReference();

		/**
		*  @brief
		*    Release a context reference
		*
		*  @note
		*    - If this is the last reference, the internal global terrain options are destroyed automatically
		*/
		static void releaseContextReference();

		/**
		*  @brief
		*    Return the global OGRE terrain options instance
		*
		*  @return
		*    The global OGRE terrain options instance, can be a null pointer, do not destroy the instance
		*/
		inline static Ogre::TerrainGlobalOptions* getOgreTerrainGlobalOptions();


	//[-------------------------------------------------------]
	//[ Private static data                                   ]
	//[-------------------------------------------------------]
	private:
		static uint32					   mContextCounter;	///< Terrain context counter
		static Ogre::TerrainGlobalOptions* mTerrainGlobals;	///< OGRE terrain globals instance, can be a null pointer


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/terrain/TerrainContext-inl.h"
