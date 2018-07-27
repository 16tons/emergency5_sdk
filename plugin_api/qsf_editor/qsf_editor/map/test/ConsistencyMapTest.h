// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/map/test/MapTest.h"

#include <QtCore/qobject.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace camp
{
	class UserObject;
}
namespace Ogre
{
	class SceneNode;
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
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Map consistency test
		*
		*  @remarks
		*    Performed checks:
		*    - Warn: Odd map boundaries
		*    - Inform: Inform when an entity has no metadata component (core entity is excluded)
		*    - Warn: Entity is no instance of a prefab. To be able to maintain a huge mass of entities, please prefer using prefabs (core entity is excluded).
		*    - Error: Ensure that entities are inside a valid layer
		*    - Error: Entity is using a non-existent prefab
		*    - Error: Entity is using a non-existent prototype
		*    - Error: Entity with properties set to an invalid asset type
		*    - Error: Entity is outside the map boundaries
		*    - Warn: Entity is larger as the grid size (too tiny but also too large entities are no good friends of spatial visibility detection)
		*/
		class ConsistencyMapTest : public QObject, public MapTest
		{


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::ConsistencyMapTest" unique pluginable map test ID


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] mapTestManager
			*    The map test manager this map test is in, no null pointer allowed (the CAMP reflection system does not like references in constructors, so we have to use a pointer instead)
			*/
			explicit ConsistencyMapTest(MapTestManager* mapTestManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~ConsistencyMapTest();


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::MapTest methods        ]
		//[-------------------------------------------------------]
		protected:
			virtual void run() override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Check CAMP user object asset properties
			*
			*  @param[in] component
			*    Component instance we originally came from
			*  @param[in] campUserObject
			*    CAMP user object to check
			*/
			void checkCampUserObjectAssetProperties(const Component& component, const camp::UserObject& campUserObject);

			/**
			*  @brief
			*    Check CAMP user object asset properties
			*
			*  @param[in] component
			*    Component instance we originally came from
			*  @param[in] campUserObject
			*    CAMP user object to check
			*  @param[in] property
			*    CAMP property to check
			*/
			void checkCampUserObjectAssetProperties(const Component& component, const camp::UserObject& campUserObject, const camp::Property& campProperty);

			/**
			*  @brief
			*    Check bounding boxes
			*
			*  @param[in] component
			*    OGRE scene node to hierarchically check
			*
			*  @note
			*    - Search for NAN bounding box: A NAN bounding box can cause serious culling issues (in EMERGENCY 5, we had complete sectors disappearing)
			*    - Too tiny but also too large bounding boxes are no good friends of spatial visibility detection
			*/
			void checkBoundingBoxes(Ogre::SceneNode& ogreSceneNode);


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::ConsistencyMapTest)
