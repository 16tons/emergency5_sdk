// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/map/test/MapTestManager.h"

#include <qsf/plugin/pluginable/Pluginable.h>
#include <qsf/log/LogSystem.h>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Map;
	class Entity;
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
		*    Abstract map test class
		*
		*  @remarks
		*    Here are some general rules concrete map tests should follow:
		*    - In case a test noticed something, write a detailed message and point to the entity containing the found issue
		*    - In general, a test should just "look", not "touch". Do not destroy valuable user content or no one will use map tests.
		*
		*    Background for the "just look"-approach: Map tests are used during editing time where each operation has to be undoable
		*    and has to be send to the QSF server if one exists. If a map test would directly manipulate data, both use cases mentioned
		*    before wouldn't work.
		*/
		class QSF_EDITOR_API_EXPORT MapTest : public Pluginable<MapTestManager>
		{


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
		friend class MapTestManager;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~MapTest();

			/**
			*  @brief
			*    Return the map instance to perform the map test on
			*
			*  @return
			*    The map instance to perform the map test on, do no destroy the returned instance
			*/
			const Map& getMap() const;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::MapTest methods        ]
		//[-------------------------------------------------------]
		protected:
			/**
			*  @brief
			*    Run the map test
			*/
			virtual void run() = 0;


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] mapTestManager
			*    The map test manager this map test is in, no null pointer allowed (the CAMP reflection system does not like references in constructors, so we have to use a pointer instead)
			*/
			inline explicit MapTest(MapTestManager* mapTestManager);

			/**
			*  @brief
			*    Print a text into the log
			*
			*  @param[in] severityLevel
			*    Severity level
			*  @param[in] text
			*    UTF-8 text which should be written into the log
			*  @param[in] entity
			*    Optional entity related to the text, can be a null pointer
			*/
			void print(LogMessage::SeverityLevel severityLevel, const std::string& text, const Entity* entity = nullptr) const;

			/**
			*  @brief
			*    Return a list of serializable entities
			*
			*  @param[out] entities
			*    Receives the serializable entities, list is not cleared before adding new entries
			*/
			void getSerializableEntities(std::vector<const Entity*>& entities) const;


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
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor/map/test/MapTest-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::MapTest)
