// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <OGRE/OgreMovableObject.h>


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
	*    Specialized QSF OGRE entity factory
	*/
	class QsfOgreEntityFactory : public Ogre::MovableObjectFactory
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static Ogre::String FACTORY_TYPE_NAME;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		QsfOgreEntityFactory();
		virtual ~QsfOgreEntityFactory();


	//[-------------------------------------------------------]
	//[ Public virtual Ogre::MovableObjectFactory methods     ]
	//[-------------------------------------------------------]
	public:
		virtual const Ogre::String& getType() const override;
		virtual void destroyInstance(Ogre::MovableObject* obj) override;


	//[-------------------------------------------------------]
	//[ Protected virtual Ogre::MovableObjectFactory methods  ]
	//[-------------------------------------------------------]
	protected:
		virtual Ogre::MovableObject* createInstanceImpl(Ogre::IdType id, Ogre::ObjectMemoryManager* objectMemoryManager, Ogre::SceneManager* manager, const Ogre::NameValuePairList* params = nullptr) override;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
