// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/voronoi/DynamicVoronoiGraph.h"
#include "qsf_ai/base/DebugSetting.h"

#include <deque>
#include <map>
#include <vector>


namespace qsf
{
	namespace ai
	{
		namespace voronoi
		{
			/**
			* The functionality to create Voronoi diagrams is separated into distinct algorithmic steps.
			* An instance of this class tracks distances to the closest obstacles and determines equidistant voronoi lines for the dynamic voronoi graph passed as a parameter.
			* This algorithm follows the lines of the paper "Improved Updating of Euclidean Distance Maps and Voronoi Diagrams" that is available in the Wiki.
			*/
			class QSF_AI_API_EXPORT DistanceCalculator
			{
			public:
				// Passing an ErasedGraphDataCollection is optional. Only provide one if you need to keep tracked of changed elements
				explicit DistanceCalculator(DynamicGraph& graph, DynamicGraph::TweakedGraphDataCollection* eraseTracker = nullptr, const DebugSetting& debugSetting = DebugSetting());

				// Optional initialization for the open list from the current grid state.
				// Makes only sense if the obstacles have been entered en bloc before.
				// Typically called after all obstacles have been entered via the bullet checks or after being serialized from file.
				void initializeOpenListFromGridState();

				// Mark a position as being blocked
				// Returns whether this was new information or there was already an obstacle there
				bool setObstacle(const GridCoordinates& coordinates);

				// Remove obstacle information
				// Returns whether there was actually an obstacle registered before or the false if the position was already known to be empty
				bool clearObstacle(const GridCoordinates& coordinates);

				// Main function triggering the work
				void updateDistanceMap();

				// Access a coordinate window for the region that was changed in the distance map
				//@{
				const GridCoordinates& getChangeWindowMin() const;
				const GridCoordinates& getChangeWindowMax() const;
				//@}

			private:
				// Raising means increasing the distance, typically a wavefront from an obstacle that was removed
				void raise(unsigned int cellIndex);
				// Lower means reducing the distance, typically a wavefront from an obstacle that was added
				void lower(unsigned int cellIndex);

				// Test whether any of the two neighbouring cells should be part of the voronoi diagram.
				void checkVoronoi(unsigned int cellAIndex, unsigned int cellBIndex);

				// Private data
				DynamicGraph& mGraph; // The grid to be changed

				// We always have only very few clusters of distances at each point in time.
				// To reduce the number of comparisons compared to a regular priority queue we use a map of deques (== bucketed priority queue).
				// The key of the map entry is the squared distance of all the contained indices
				typedef std::deque<unsigned int> CellIndexList;
				typedef std::map<unsigned int, CellIndexList> BrushfireOpenList; // Mapping a distance to a list of cell indices that have this closest distance

				BrushfireOpenList mOpenList; // The list of states that need to be propagated
				std::vector<bool> mShouldBeRaised; // Flag for every cell whether it should currently be raised.
				std::vector<bool> mToProcess; // Flag for every cell whether the next occurrence should be processed. It is actually used to avoid having to search for deprecated entries in the frontier

				DebugSetting mDebugDrawActivated;

				// Tracks a coordinate window for changes inside the grid
				GridCoordinates mChangedGridWindowMin;
				GridCoordinates mChangedGridWindowMax;

				// Optional tracking of changed graph elements
				DynamicGraph::TweakedGraphDataCollection* mErasedElementsTracker;
			};
		}
	}
}
