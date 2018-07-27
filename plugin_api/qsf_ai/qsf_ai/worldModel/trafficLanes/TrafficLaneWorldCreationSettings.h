// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/base/UnsignedFloat.h"
#include "qsf_ai/base/DebugSetting.h"
#include "qsf_ai/voronoi/HeightRestriction.h"

#include <qsf/reflection/object/Object.h>
#include <qsf/reflection/type/CampGlmVec3.h>
#include <qsf/math/Color4.h>

#include <memory>


namespace qsf
{
	namespace ai
	{
		/**
		* @brief
		*	An instance of this class keeps track the settings that were used to create a specific traffic lane world.
		*/
		class QSF_AI_API_EXPORT TrafficLaneWorldCreationSettings : public Object
		{
		public:
			// Default values for the exported properties
			//@{
			static const glm::vec3 DEFAULT_CORNER1;
			static const glm::vec3 DEFAULT_CORNER2;
			static const UnsignedFloat DEFAULT_CELLSIZE;
			static const unsigned int DEFAULT_LANE_TYPE;
			static const unsigned int DEFAULT_MAP_ID;
			static const short DEFAULT_AVOID_COLLISION_FILTER; // collision filter for the collisions to avoid, zero would mean don't avoid collisions which doesn't really make sense
			static const short DEFAULT_REQUIRE_COLLISION_FILTER; // collision filter for the required collisions, very special setting, usually zero, when set means only walk where you collide with something of this type
			static const int DEFAULT_WALKABLE_LEVEL_SETTING;
			static const unsigned int DEFAULT_MIX_WITH_MAPID_SETTING;
			static const bool DEFAULT_INCLUDE_WATER_SETTING;
			static const bool DEFAULT_TEST_AT_WALKABLE_SETTING;
						//@}

			TrafficLaneWorldCreationSettings();

			HeightRestriction createHeightRestriction() const;

			// Create a new instance with the same values.
			// Necessary because qsf::object is not copyable by default.
			std::auto_ptr<TrafficLaneWorldCreationSettings> clone() const;

			// Exported properties connected to the CAMP reflection system
			//@{
			glm::vec3 mCorner1;
			glm::vec3 mCorner2;
			UnsignedFloat mCellSize;
			unsigned int mLaneType;
			unsigned int mMapId; // The unique id of the map to be created
			short mAvoidCollisionFilter; // used as a positive filter which bullet obstacles count as collisions that should be avoided
			short mRequireCollisionFilter; // used as a positive filter which bullet obstacles define the area where we can walk at all
			int mWalkableLevel;
			unsigned int mMixWithMapId;
			bool mIncludeWater;
			bool mTestAtWalkable;
			//@}

			QSF_CAMP_RTTI();
		};
	}
}


#include "qsf_ai/worldModel/trafficLanes/TrafficLaneWorldCreationSettings-inl.h"

QSF_CAMP_TYPE_NONCOPYABLE(qsf::ai::TrafficLaneWorldCreationSettings)
