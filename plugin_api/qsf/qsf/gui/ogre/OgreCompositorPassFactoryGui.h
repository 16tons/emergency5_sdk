// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/compositor/OgreCompositorPassFactory.h"


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
	*    GUI OGRE compositor pass factory pluginable
	*/
	class OgreCompositorPassFactoryGui : public OgreCompositorPassFactory
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 PLUGINABLE_ID;	///< "qsf::OgreCompositorPassFactoryGui" unique pluginable view ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit OgreCompositorPassFactoryGui(OgreCompositorPassProvider* ogreCompositorPassProvider);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~OgreCompositorPassFactoryGui();


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::OgreCompositorPassFactory methods ]
	//[-------------------------------------------------------]
	protected:
		virtual Ogre::CompositorPassDef* addPassDef(Ogre::uint32 rtIndex, Ogre::CompositorNodeDef* parentNodeDef) override;
		virtual Ogre::CompositorPass* addPass(const Ogre::CompositorPassDef* definition, Ogre::Camera* defaultCamera, Ogre::CompositorNode* parentNode, const Ogre::CompositorChannel& target, Ogre::SceneManager* sceneManager) override;


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::OgreCompositorPassFactoryGui)
