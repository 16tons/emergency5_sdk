// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/map/GridSceneNode.h"

#include <OGRE/Math/Array/OgreNodeMemoryManager.h>
#include <OGRE/Math/Array/OgreObjectMemoryManager.h>

#include <boost/container/flat_set.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class LightComponent;
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
	*    Specialized top level cell QSF OGRE scene node
	*/
	class QSF_API_EXPORT GridCellSceneNode : public GridSceneNode
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class GridSceneManager;
		friend class LightComponent;	///< Updates "qsf::GridCellSceneNode::mActiveLightComponents"
		friend class GridRaySceneQuery;
		friend class GridSphereSceneQuery;
		friend class GridPlaneBoundedVolumeListSceneQuery;


	//[-------------------------------------------------------]
	//[ Public declarations                                   ]
	//[-------------------------------------------------------]
	public:
		typedef boost::container::flat_set<LightComponent*> ActiveLightComponents;


	//[-------------------------------------------------------]
	//[ Public  methods                                       ]
	//[-------------------------------------------------------]
	public:
		const ActiveLightComponents& getActiveLightComponents() const;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		GridCellSceneNode(Ogre::IdType id, Ogre::SceneManager* creator, Ogre::NodeMemoryManager* nodeMemoryManager, Ogre::SceneNode* parent);
		virtual ~GridCellSceneNode();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Ogre::Aabb				  mAabb;
		Ogre::NodeMemoryManager   mNodeMemoryManager[Ogre::NUM_SCENE_MEMORY_MANAGER_TYPES];
		Ogre::ObjectMemoryManager mEntityMemoryManager[Ogre::NUM_SCENE_MEMORY_MANAGER_TYPES];
		ActiveLightComponents     mActiveLightComponents;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
