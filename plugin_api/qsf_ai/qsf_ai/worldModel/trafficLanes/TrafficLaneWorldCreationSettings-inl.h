// Copyright (C) 2012-2018 Promotion Software GmbH

#include <qsf/serialization/binary/BinarySerializer.h>
#include <qsf/serialization/binary/GlmTypeSerialization.h>

namespace qsf
{
	namespace ai
	{
		inline TrafficLaneWorldCreationSettings::TrafficLaneWorldCreationSettings() :
			mCorner1(DEFAULT_CORNER1),
			mCorner2(DEFAULT_CORNER2),
			mCellSize(DEFAULT_CELLSIZE),
			mLaneType(DEFAULT_LANE_TYPE),
			mMapId(DEFAULT_MAP_ID),
			mAvoidCollisionFilter(DEFAULT_AVOID_COLLISION_FILTER),
			mRequireCollisionFilter(DEFAULT_REQUIRE_COLLISION_FILTER),
			mWalkableLevel(DEFAULT_WALKABLE_LEVEL_SETTING),
			mMixWithMapId(DEFAULT_MIX_WITH_MAPID_SETTING),
			mIncludeWater(DEFAULT_INCLUDE_WATER_SETTING),
			mTestAtWalkable(DEFAULT_TEST_AT_WALKABLE_SETTING)
		{
			// Nothing to do
		}

		inline std::auto_ptr<TrafficLaneWorldCreationSettings> TrafficLaneWorldCreationSettings::clone() const
		{
			std::auto_ptr<TrafficLaneWorldCreationSettings> result(new TrafficLaneWorldCreationSettings);

			result->mCorner1 = mCorner1;
			result->mCorner2 = mCorner2;
			result->mCellSize = mCellSize;
			result->mLaneType = mLaneType;
			result->mWalkableLevel = mWalkableLevel;
			result->mMixWithMapId = mMixWithMapId;
			result->mIncludeWater = mIncludeWater;
			result->mTestAtWalkable = mTestAtWalkable;
			result->mAvoidCollisionFilter = mAvoidCollisionFilter;
			result->mRequireCollisionFilter = mRequireCollisionFilter;
			result->mMapId = mMapId;

			return result;
		}
	}

	namespace serialization
	{
		template <>
		struct serializer<ai::TrafficLaneWorldCreationSettings>
		{
			inline static void serialize(BinarySerializer& serializer, ai::TrafficLaneWorldCreationSettings& settings)
			{
				serializer & settings.mCorner1;
				serializer & settings.mCorner2;
				serializer & settings.mCellSize;
				serializer & settings.mLaneType;
				serializer & settings.mMapId;
				serializer & settings.mAvoidCollisionFilter;
				serializer & settings.mRequireCollisionFilter;
				if (serializer.getFormatVersion() >= 15)
				{
					serializer & settings.mWalkableLevel;	// Version 15 only
					serializer & settings.mMixWithMapId;
				}
				if (serializer.getFormatVersion() >= 16)
				{
					serializer & settings.mIncludeWater;	// Version 16 only
				}
				serializer & settings.mTestAtWalkable;
				if (serializer.getFormatVersion() < 17)		// until Version 17
				{
					// We need to be able to read older versions
					bool showObstacles = false;
					bool showEquidistantCells = false;
					bool showEquidistanceObstacleInfluence = false;
					UnsignedFloat secondsToShowDebugData = 0.0f;

					serializer & showObstacles;
					serializer & showEquidistantCells;
					serializer & showEquidistanceObstacleInfluence;
					serializer & secondsToShowDebugData;
				}
			}
		};
	}
}
