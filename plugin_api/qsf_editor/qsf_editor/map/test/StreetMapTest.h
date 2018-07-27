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
		*    Map test searching for street issues
		*
		*  @remarks
		*    Performed checks:
		*    - Error: Street crossings without any connection to other street crossings (except there's just a single street crossing inside the map)
		*    - Warn: Street crossing with at least one street gateway which is not connected to another street gateway
		*    - Error: Two interconnected street gateways which have a different number of nodes
		*    - Error: Mirrored street crossings (at least one inverted scale axis)
		*    - Error: Street crossing with edges referencing none existing nodes
		*    - Warn: Every entity with a street crossing component should also have a transform component
		*    - Error: Streets with street gateways linked to non existing gateways due to non existing entity or invalid street gateway index
		*    - Warn: Entity with a street crossing component, which has entities with street crossing component linked to it
		*    - Warn: Street segment goes to same direction, where it comes from
		*/
		class StreetMapTest : public QObject, public MapTest
		{


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::StreetMapTest" unique pluginable map test ID


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
			explicit StreetMapTest(MapTestManager* mapTestManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~StreetMapTest();


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::MapTest methods        ]
		//[-------------------------------------------------------]
		protected:
			virtual void run() override;


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::StreetMapTest)
