// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/base/Math.h"

#include <algorithm>

namespace qsf
{
	namespace ai
	{
		namespace voronoi
		{
			inline SegmentFinder::ClusterResult::ClusterResult(unsigned int numClusters, unsigned int maxClusterSize, uint8 memberFlags, uint8 edgeExitsFlags) :
				mNumClusters(numClusters),
				mMaxClusterSize(maxClusterSize),
				mMaxClusterMembers(memberFlags),
				mHubExits(edgeExitsFlags)
			{}

			inline SegmentFinder::SegmentFinder(DynamicGraph& graph, DynamicGraph::TweakedGraphDataCollection* erasedDataTracker, unsigned int minimumDistanceNeeded) :
				mGraph(graph),
				mTweakedDataCollection(erasedDataTracker),
				mCellConsidered(graph.mGrid.getConfiguration().mDimensions.x * graph.mGrid.getConfiguration().mDimensions.y, false),
				mMinimumDistanceSquaredNeeded(minimumDistanceNeeded * minimumDistanceNeeded)
			{}

			inline bool SegmentFinder::wasCellConsidered(unsigned int cellIndex) const
			{
				// Unchecked access should be safe here as it is not a public function
				return mCellConsidered[cellIndex];
			}

			inline void SegmentFinder::setCellConsidered(unsigned int cellIndex)
			{
				// Unchecked access should be safe here as it is not a public function
				mCellConsidered[cellIndex] = true;
			}

			inline float SegmentFinder::getFreeSpaceDelta(unsigned int cellFromIndex, unsigned int cellToIndex) const
			{
				 const float freeSpaceAtStart = std::sqrt(static_cast<float>(mGraph.mGrid.calculateClosestDistanceSquared(cellFromIndex)));
				 const float freeSpaceAtEnd = std::sqrt(static_cast<float>(mGraph.mGrid.calculateClosestDistanceSquared(cellToIndex)));

				 return freeSpaceAtEnd - freeSpaceAtStart;
			}

			inline const std::deque<SegmentFinder::GridSegment>& SegmentFinder::getCreatedLanes() const
			{
				return mSegmentsFound;
			}

			inline const std::multimap<unsigned int, unsigned int>& SegmentFinder::getNodeToLaneConnections() const
			{
				return mNodeToLaneConnections;
			}
		}
	}
}
