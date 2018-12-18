// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

#include <OGRE/OgreSceneNode.h>


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
	*    Specialized QSF OGRE scene node
	*/
	class QSF_API_EXPORT GridSceneNode : public Ogre::SceneNode
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class GridSceneManager;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		GridSceneNode(Ogre::IdType id, Ogre::SceneManager* creator, Ogre::NodeMemoryManager* nodeMemoryManager, Ogre::SceneNode* parent);
		explicit GridSceneNode(const Ogre::Transform& transformPtrs);
		virtual ~GridSceneNode();
		void migrateToNodeMemoryManager(Ogre::NodeMemoryManager& nodeMemoryManager);


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
