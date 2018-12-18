// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/base/UnsignedFloat.h"
#include "qsf_ai/voronoi/GridCoordinates.h"

#include <qsf/serialization/binary/BinarySerializer.h>

#include <glm/glm.hpp>


namespace qsf
{
	namespace ai
	{
		/**
		* A GridConfiguration contains all the information to create a grid structure and transfer positions between 3D continuous space and 2D grid cspace.
		* It is implemented like a simple struct with public member variables and can be copied by the default mechanisms.
		* The coordinate system is hardcoded in here and the height is always positive Y.
		* TODO(vs) Use a planar converter to be transparent regarding the coordinate system here.
		*/
		class QSF_AI_API_EXPORT GridConfiguration
		{
		public:
			// Create from origin and the number of cells created from there
			GridConfiguration(const glm::vec3& origin, const GridCoordinates& dimensions, UnsignedFloat cellSize);
			// create from both corners of a bounding box and the cell size to apply
			GridConfiguration(const glm::vec3& corner1, const glm::vec3& corner2, UnsignedFloat cellSize);

			// Convert between 2D grid and 3D continuous coordinates.
			// These functions don't check the maximum dimensions for optimized performance.
			// This means they calculate where the position would be given the cell size but ignore whether this is out of range for the current grid.
			// There are several versions from 2D to 3D, returning the midpoint or the corners of the cell as a pair.
			//@{
			glm::vec3 convertPositionToMidpoint(const GridCoordinates& position) const;
			std::pair<glm::vec3, glm::vec3> convertPositionToCorners(const GridCoordinates& position) const;
			GridCoordinates convertPosition(const glm::vec3& position) const;
			//@}

			// The origin of the grid is the left bottom corner of the cell with index (0, 0).
			// It is used as a static offset that is added to all positions calculated from the grid coordinates.
			glm::vec3 mOrigin;

			// The number of grid coordinates in both ground dimensions.
			// The legal indices are in the range [0, mDimensions.<dimension>) for each dimension.
			GridCoordinates mDimensions;

			// The length of the grid cells is defined in world units and applied in both ground dimensions as the grid cells are squares.
			// It is used to convert positions between 2D grid coordinates and 3D continuous space coordinates.
			// The Variable is technically strictly positive.
			UnsignedFloat mCellSize;
		};
	}
}

#include "qsf_ai/voronoi/GridConfiguration-inl.h"
