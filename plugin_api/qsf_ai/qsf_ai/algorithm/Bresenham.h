// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/base/UnsignedFloat.h"
#include "qsf_ai/voronoi/GridCoordinates.h"
#include "qsf_ai/voronoi/GridConfiguration.h"

#include <glm/glm.hpp>


namespace qsf
{
	namespace ai
	{
		/**
		* Interface definition for a functor to be passed to the bresenham functions below to integrate results and react with a custom action to it.
		* This is a dummy interface definition without an implementation available.
		*/
		class GridCellFunctor
		{
		public:
			// Receive the information about an intersected cell.
			// Return whether we should continue the bresenham algorithm.
			// Returning false here terminates the algorithm prematurely
			bool onGridCellIntersected(const GridCoordinates& coordinates);
		};

		/**
		* Calculate the indices of intersected cells when going from lineStart to lineEnd in a grid defined by the grid configuration.
		* The passed action is called for each entered segment and needs to provide the GridCellFunctor interface defined above.
		* The snap delta determines a range when a diagonal step is taken instead of intersecting very close to the edge.
		* This is necessary because due to round off errors, the edge is otherwise never exactly hit.
		*/
		template <typename Action>
		void calculateIntersectedCells(glm::vec2 lineStart, glm::vec2 lineEnd, const GridConfiguration& gridConfig, Action& action, UnsignedFloat snapVertexDelta = UncheckedUnsignedFloat(1e-2f));
		template <typename Action>
		void calculateIntersectedCells(const GridCoordinates& startIndices, const GridCoordinates& goalIndices, Action& action, UnsignedFloat snapVertexDelta = UncheckedUnsignedFloat(1e-2f));
	}
}

#include "qsf_ai/algorithm/Bresenham-inl.h"
