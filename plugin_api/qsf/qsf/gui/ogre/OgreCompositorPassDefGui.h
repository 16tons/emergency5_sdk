// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <OGRE/Compositor/Pass/OgreCompositorPassDef.h>


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
	*    GUI OGRE compositor pass definition
	*/
	class OgreCompositorPassDefGui : public Ogre::CompositorPassDef
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		OgreCompositorPassDefGui(Ogre::uint32 rtIndex, Ogre::CompositorNodeDef* parentNodeDef);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~OgreCompositorPassDefGui();


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
