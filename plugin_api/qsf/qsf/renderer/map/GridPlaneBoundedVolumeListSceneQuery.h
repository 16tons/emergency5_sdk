// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <OGRE/OgreSceneManager.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class GridCellSceneNode;
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
	*    Grid implementation of "Ogre::PlaneBoundedVolumeListSceneQuery"
	*/
	class GridPlaneBoundedVolumeListSceneQuery : public Ogre::DefaultPlaneBoundedVolumeListSceneQuery
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit GridPlaneBoundedVolumeListSceneQuery(Ogre::SceneManager* creator);
		virtual ~GridPlaneBoundedVolumeListSceneQuery();


	//[-------------------------------------------------------]
	//[ Public virtual Ogre::RaySceneQuery methods            ]
	//[-------------------------------------------------------]
	public:
		virtual void execute(Ogre::SceneQueryListener* listener) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void execute(Ogre::SceneQueryListener* listener, GridCellSceneNode& gridCellSceneNode);
		void execute(Ogre::SceneQueryListener* listener, Ogre::ObjectMemoryManager& ogreObjectMemoryManager);


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
