// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/base/Math.h"

#include <glm/fwd.hpp>

namespace qsf
{
	namespace ai
	{
		// Contains algorithms for intersection with convex polygons.
		// Not sure if we should port these to the QSF core - they are already pretty special.
		// Returns an area relation that means on the outer border is identified as its own state.
		// The polygon boundary pointers are both inclusive
		area::PointRelation isInsideConvexPolygon(const glm::vec2& point, const glm::vec2* polygonFirst, const glm::vec2* polygonLast);

		// Contains algorithms for intersection with convex polygons.
		// Returns the lowest distance from any segment to the point - negative distance if point is inside, positive if outside.
		// The polygon boundary pointers are both inclusive
		float distanceToConvexPolygon(const glm::vec2& point, const glm::vec2* polygonFirst, const glm::vec2* polygonLast);
	}
}
