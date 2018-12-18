// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/map/GridSceneNode.h"


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
	*    Specialized top level root QSF OGRE scene node
	*
	*  @note
	*    - At the moment just there for easier debugging
	*/
	class GridRootSceneNode : public GridSceneNode
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class GridSceneManager;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		GridRootSceneNode(Ogre::IdType id, Ogre::SceneManager* creator, Ogre::NodeMemoryManager* nodeMemoryManager, Ogre::SceneNode* parent);
		explicit GridRootSceneNode(const Ogre::Transform& transformPtrs);
		virtual ~GridRootSceneNode();


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
