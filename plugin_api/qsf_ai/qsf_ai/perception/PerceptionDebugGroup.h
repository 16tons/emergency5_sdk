// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/reflection/ExportedProperty.h"

#include <qsf/base/UniqueInstance.h>
#include <qsf/reflection/CampDefines.h>
#include <qsf/debug/group/DebugGroup.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Encapsulates the configurations for all available debugging options inside the AI sensor perception system.
		*    Noncopyable by inheritance.
		*/
		class QSF_AI_API_EXPORT PerceptionDebugGroup : public DebugGroup, public UniqueInstance<PerceptionDebugGroup>
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			// Unique CAMP Ids and initial values
			//@{
			QSF_DECLARE_CAMP_PLUGINABLE_ID;

			static const unsigned int SHOW_LINES_OF_SIGHT_PROPERTY_ID; // Flag to display the lines of sight from the perceiver to the respective perceived entity.
			static const unsigned int SHOW_OBSTRUCTED_LINES_OF_SIGHT_PROPERTY_ID; // Flag to display obstructed lines of sight from the perceiver to the respective entity whose ray-test failed.
			static const unsigned int MARK_PERCEIVED_ENTITIES_PROPERTY_ID; // Flag to hilite entities that are currently perceived by at least one other entity.
			static const unsigned int MARK_PERCEIVERS_PROPERTY_ID; // Flag to hilite entities with active sensor-components attached.
			static const unsigned int SHOW_SENSOR_AREA_PROPERTY_ID; // Flag to draw the spherical sensor scan area.
			static const unsigned int SHOW_FOR_ALL_ENTITIES_PROPERTY_ID; // Flag to force showing the debug output for all entities. Otherwise only the component debug state that is triggered on selection activates it.

			static const bool DEFAULT_SHOW_LINES_OF_SIGHT;
			static const bool DEFAULT_SHOW_OBSTRUCTED_LINES_OF_SIGHT;
			static const bool DEFAULT_MARK_PERCEIVED_ENTITIES;
			static const bool DEFAULT_MARK_PERCEIVERS;
			static const bool DEFAULT_SHOW_SENSOR_AREA;
			static const bool DEFAULT_SHOW_FOR_ALL_ENTITIES;
			//@}


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			// Constructor required by DebugGroup interface
			PerceptionDebugGroup(DebugGroupManager* dgm);

			// Getters and setters for the various debug options
			//@{
			bool getShowLinesOfSight() const;
			void setShowLinesOfSight(bool showLinesOfSight);
			bool getShowObstructedLinesOfSight() const;
			void setShowObstructedLinesOfSight(bool showObstructedLinesOfSight);
			bool getMarkPerceivedEntities() const;
			void setMarkPerceivedEntities(bool markPerceivedEntities);
			bool getMarkPerceivers() const;
			void setMarkPerceivers(bool markPerceivers);
			bool getShowSensorArea() const;
			void setShowSensorArea(bool showSensorArea);
			bool getShowForAllEntities() const;
			void setShowForAllEntities(bool showForAllEntities);
			//@}


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			bool mShowLinesOfSight;
			bool mShowObstructedLinesOfSight;
			bool mMarkPerceivedEntities;
			bool mMarkPerceivers;
			bool mShowSensorArea;
			bool mShowForAllEntities;


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI();

		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_ai/perception/PerceptionDebugGroup-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::ai::PerceptionDebugGroup);
