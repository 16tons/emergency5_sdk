// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


namespace qsf
{
	namespace ai
	{
		/**
		* Class holding data for a single cell in the data structure used by the Voronoi algorithms.
		* It keeps the index of the cell holding the closest obstacle that is known.
		* To optimize memory, a cell does not know its own index and so to see whether a cell is actually blocked, you need to have access to the grid containing it.
		* If two obstacles have the same distance within the granularity defined by the grid cell size they are considered equidistant regarding the grid cell.
		* This does usually not mean that the exact distance measured in continuous space is exactly the same.
		*/
		class DistanceGridCell
		{
		public:
			// Sets all members to INVALID
			DistanceGridCell();

			// Sets the index of the cell holding the closest known obstacle.
			// This may be the cell index itself in case the cell is directly blocked.
			void setClosestObstacleCellIndex(unsigned int index);

			// Shortcut for resetting the value to uninitialized
			void clearClosestObstacleCellIndex();

			// Returns the cell index of the closest known obstacle, uninitialized if not set
			unsigned int getClosestObstacleCellIndex() const;

			// Tests whether the closest known obstacle value is already set (== initialized)
			bool hasClosestKnownObstacle() const;

		private:
			unsigned int mClosestObstacleCell; // uninitialized if not set / known
		};
	}
}

#include "qsf_ai/voronoi/DistanceGridCell-inl.h"
