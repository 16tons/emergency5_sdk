// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class SceneManager;
	class MovableObject;
}
namespace qsf
{
	class Component;
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
	*    OGRE selection user data class
	*/
	class QSF_API_EXPORT OgreSelectionUserData
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const std::string USER_ANY_KEY;	///< OGRE user any key "MovableObject"


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		inline OgreSelectionUserData();
		inline explicit OgreSelectionUserData(Component* component);

		void applyToOgreMovableObject(Ogre::MovableObject& ogreMovableObject, Ogre::SceneManager* ogreSceneManager = nullptr) const;


	//[-------------------------------------------------------]
	//[ Public data                                           ]
	//[-------------------------------------------------------]
	public:
		Component*  mComponent;
		uint32		mDebugDrawId;


	};


	//[-------------------------------------------------------]
	//[ Overloading of operators                              ]
	//[-------------------------------------------------------]
	inline std::ostream& operator<<(std::ostream& out, OgreSelectionUserData& userData);


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/helper/OgreSelectionUserData-inl.h"
