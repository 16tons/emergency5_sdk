// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/base/Direction.h"

#include <glm/fwd.hpp>

#include <cstddef>


// Definition of a coordinates in a 2D grid based on a forward declaration of camp headers
namespace qsf
{
	namespace ai
	{
		typedef glm::tvec2<unsigned int, glm::defaultp> GridCoordinates;
		typedef glm::tvec2<int, glm::defaultp> GridDelta; // Signed coordinate type to allow representing negative deltas

		// Create a new cell coordinates object by advancing in the direction passed
		GridCoordinates advanceInDirection(const GridCoordinates& pos, direction::TwoDimensional direction);

		// Create a grid delta when subtracting grid coordinates
		GridDelta operator -(const GridCoordinates& lhs, const GridCoordinates& rhs);
		// Does not provide safety against overrun, clamps silently by casting to unsigned int
		GridCoordinates operator +(const GridCoordinates& lhs, const GridDelta& rhs);
	}
}

#include "qsf_ai/voronoi/GridCoordinates-inl.h"
