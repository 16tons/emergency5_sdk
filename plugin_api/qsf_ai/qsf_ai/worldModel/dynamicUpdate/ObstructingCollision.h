// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/base/UnsignedFloat.h"

#include <qsf/platform/PlatformTypes.h>

#include <glm/glm.hpp>


namespace qsf
{
	namespace ai
	{
		/**
		* An obstructing collision is a dynamic collision that hinders movement through some part of an AI navigation map.
		* It is stored with the information needed to calculate a movement around it.
		* Only boxes are allowed as dynamic obstructions.
		* Simple struct like data collection.
		*/
		class ObstructingCollision
		{
		public:
			static const unsigned int NUM_COLLISION_VERTICES = 4;
			// Default constructor needed for binary serialization. Sets dummy values
			ObstructingCollision();

			// Constructor optimized to the creation process.
			// The id values that are known from the start are set while the collision data like space and vertices are set afterwards.
			ObstructingCollision(uint64 entityId, unsigned int areaId, unsigned int segmentId, bool evadeToLeft);

			bool mEvadeToLeft; // Described from the point of view of the original ideal lane
			uint64 mEntityId; // The id of the object providing the collision information
			unsigned int mAreaId; // The id of the intersected area
			unsigned int mSegmentId; // The segment of the AI navigation map part that is obstructed
			UnsignedFloat mMaxLateralFreeSpace; // The restriction to free space introduced by this obstacle. This is the allowed lateral free space at the tightest point defined by this obstacle and the intersected area boundaries.
			glm::vec2 mCollisionBoxVertices[NUM_COLLISION_VERTICES]; // The 2D projection of the four box points that act as collision. These need to form a polygon when applied in the order they are stored here
		};
	}
}

#include "qsf_ai/worldModel/dynamicUpdate/ObstructingCollision-inl.h"
