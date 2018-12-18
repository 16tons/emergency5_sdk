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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Factory for the QSF OGRE base scene manager
	*/
	class SceneManagerBaseFactory : public Ogre::SceneManagerFactory
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const Ogre::String FACTORY_TYPE_NAME;	///< Factory type name


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		SceneManagerBaseFactory();
		virtual ~SceneManagerBaseFactory();


	//[-------------------------------------------------------]
	//[ Public virtual Ogre::SceneManagerFactory methods      ]
	//[-------------------------------------------------------]
	public:
		virtual Ogre::SceneManager* createInstance(const Ogre::String& instanceName, size_t numWorkerThreads, Ogre::InstancingThreadedCullingMethod threadedCullingMethod) override;
		virtual void destroyInstance(Ogre::SceneManager* ogreSceneManager) override;


	//[-------------------------------------------------------]
	//[ Protected virtual Ogre::SceneManagerFactory methods   ]
	//[-------------------------------------------------------]
	protected:
		virtual void initMetaData() const override;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
