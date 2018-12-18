// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/plugin/pluginable/Pluginable.h"
#include "qsf/renderer/compositor/OgreCompositorPassProvider.h"


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
	*    Abstract OGRE compositor pass factory pluginable
	*/
	class QSF_API_EXPORT OgreCompositorPassFactory : public Pluginable<OgreCompositorPassProvider>
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class OgreCompositorPassProvider;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		virtual ~OgreCompositorPassFactory();


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*/
		explicit OgreCompositorPassFactory(OgreCompositorPassProvider* ogreCompositorPassProvider);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::OgreCompositorPassFactory methods ]
	//[-------------------------------------------------------]
	protected:
		virtual Ogre::CompositorPassDef* addPassDef(Ogre::uint32 rtIndex, Ogre::CompositorNodeDef* parentNodeDef) = 0;

		// Will return a null pointer if the OGRE compositor pass definition does not match
		virtual Ogre::CompositorPass* addPass(const Ogre::CompositorPassDef* definition, Ogre::Camera* defaultCamera, Ogre::CompositorNode* parentNode, const Ogre::CompositorChannel& target, Ogre::SceneManager* sceneManager) = 0;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::OgreCompositorPassFactory)
