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
	*    Factory for the QSF OGRE grid scene manager
	*/
	class GridSceneManagerFactory : public Ogre::SceneManagerFactory
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
		GridSceneManagerFactory();
		virtual ~GridSceneManagerFactory();


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
