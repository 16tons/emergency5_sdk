// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/base/UnsignedFloat.h"

#include <glm/glm.hpp>


namespace qsf
{
	namespace ai
	{
		/**
		* A contact point to an ai navigation map is a closest point on the defined area that makes up the specific navigation map.
		* Simple class with public variables that is used like a struct.
		*/
		class MapContactPoint
		{
		public:
			MapContactPoint(const glm::vec3& closestPosition, unsigned int worldElement, unsigned int worldElementSegment, UnsignedFloat distanceToLegalSpace);

			// public data
			glm::vec3 mClosestPosition; // The actual position that is closest to the request on the AI map
			unsigned int mWorldElementId; // Unique id of the specific AI world element that is closest
			unsigned int mWorldElementSegmentId; // unique id of the specific part of the AI world element that is closest for navigation structures with such a fine grained solution.
			UnsignedFloat mDistanceToLegalSpace; // The distance that the closest point is away from the original request. This may be zero even if the positions differ because of valid distance inside the request
		};


		// inline implementation
		inline MapContactPoint::MapContactPoint(const glm::vec3& closestPosition, unsigned int worldElement, unsigned int worldElementSegment, UnsignedFloat distanceToLegalSpace) :
			mClosestPosition(closestPosition),
			mWorldElementId(worldElement),
			mWorldElementSegmentId(worldElementSegment),
			mDistanceToLegalSpace(distanceToLegalSpace)
		{}
	}
}
