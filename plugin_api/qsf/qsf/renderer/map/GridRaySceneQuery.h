// Copyright (C) 2012-2017 Promotion Software GmbH


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
	*    Grid implementation of "Ogre::RaySceneQuery"
	*/
	class GridRaySceneQuery : public Ogre::DefaultRaySceneQuery
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit GridRaySceneQuery(Ogre::SceneManager* creator);
		virtual ~GridRaySceneQuery();


	//[-------------------------------------------------------]
	//[ Public virtual Ogre::RaySceneQuery methods            ]
	//[-------------------------------------------------------]
	public:
		virtual void execute(Ogre::RaySceneQueryListener* listener) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void execute(Ogre::RaySceneQueryListener* listener, GridCellSceneNode& gridCellSceneNode);
		void execute(Ogre::RaySceneQueryListener* listener, Ogre::ObjectMemoryManager& ogreObjectMemoryManager);


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
