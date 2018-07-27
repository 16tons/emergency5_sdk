// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/voronoi/DistanceGrid.h"

#include <qsf/serialization/binary/BinarySerializer.h>

#include <vector>
#include <deque>
#include <map>


namespace qsf
{
	namespace ai
	{
		namespace voronoi
		{
			/**
			* A dynamic voronoi graph manages dynamic changes to a voronoi graph.
			* It tracks a grid of distances to closest obstacles and a voronoi graph based on it.
			*/
			class QSF_AI_API_EXPORT DynamicGraph
			{
			public:
				// helper data structure combining the cell id with the resulting unique node id of a hub cell
				class HubNodeIdentifier
				{
				public:
					HubNodeIdentifier(); // default c'tor necessary for creating it inside STL allocators
					HubNodeIdentifier(unsigned int cellId, unsigned int hubNodeId);

					// Compares only the cell id to allow sorting for it
					bool operator < (const HubNodeIdentifier& other) const;

					unsigned int mCellId;
					unsigned int mHubNodeId;
				};

				// Helper class that collects tweaked graph elements while an existing voronoi graph is being updated with new input
				// It is usually not needed when creating the initial map but necessary during dynamic updates later on
				class TweakedGraphDataCollection
				{
				public:
					std::deque<HubNodeIdentifier> mErasedHubNodes; // Unique hub nodes that where hubs before and were erased
					std::map<unsigned int, std::deque<unsigned int>> mErasedLanes; // Unique ids of lanes of which at least one member cell was erased during a reset mapped to the cell ids which were cleared
					std::deque<HubNodeIdentifier> mAddedHubs; // New hubs created during the update
					std::deque<HubNodeIdentifier> mReplacedHubIds; // Unique hub node ids that were erased and reused during the update.
					std::deque<unsigned int> mReplacedLanes;
				};

				// Specific reserved id for a recently discovered cell that is part of the voronoi graph but that is not yet categorized.
				// It is Uninitialized (== std::numeric_limits<unsigned int>::max()) - 1
				static const unsigned int UNCATEGORIZED_CELL_ID;

				DynamicGraph(const GridConfiguration& gridConfig);

				// debugging helper estimating how much memory is actually used
				std::size_t calculateMemoryConsumption() const;

				// Encapsulates setting the value of a cell and integrates the necessary change tracking
				void setCellValue(unsigned int cellIndex, unsigned int cellValue, TweakedGraphDataCollection* erastedElementTracker = nullptr);

				// Returns whether the cell with the given id is a hub
				bool isHubCell(unsigned int cellId) const;

				// Return the next number that should be used for the next hub node to be created
				unsigned int getNextFreeHubId() const;

				// Tell the system that we actually used the last requested free hub id to create a new node
				void onUsedLastRequestedHubId();

				// Retrieves the next id for a lane to use, be it reused or in general a new one. Not const because it might change the collection of unused lane ids
				unsigned int fetchNextFreeSegmentId();

				// Checks the whole distance grid voronoi line and sees whether any node has an UNCATEGORIZED or UNINITIALIZED value
				bool containsUninitializedVoronoiState() const;


				// Public data
				DistanceGrid mGrid;
				// Flag for every cell whether it is part of the voronoi diagram and if yes which lane. Is Uninitialized when not part of the voronoi line.
				// Otherwise contains the lane index. Temporarily contains UNCATEGORIZED_CELL_ID in case the cell is part of the voronoi line but no lanes have yet been defined
				std::vector<unsigned int> mVoronoiLine;

				// Contains the cell ids of node / hub cells. Remains sorted throughout so binary search can be used.
				std::deque<HubNodeIdentifier> mHubNodeIds;

				// Tracks unused ids for hub nodes that may be created during dynamic update.
				// These are nodes in the range [0 ... sizeof(mHubNodeIds)] which are currently not in use.
				// These are preferred to be reused if new node ids are requested.
				// If this is empty, all nodes in the above range are used and the next free id should be mHubNodeIds.size()
				std::deque<unsigned int> mUnusedHubNodeIds;

				// Tracks unused ids for segments which were used for lanes once but become deprecated during dynamic updates.
				// These are preferred to be reused if new segment ids are requested.
				std::deque<unsigned int> mUnusedSegmentIds;

				// Store the id for the next hub to be created
				unsigned int mNextFreeHubId;

				// Store the id for the next segment to be created from which we create segments in a linear fashion
				unsigned int mNextSegmentId;
			};
		}
	}

	namespace serialization
	{
		template <>
		struct serializer<ai::voronoi::DynamicGraph::HubNodeIdentifier>
		{
			static void serialize(BinarySerializer& serializer, ai::voronoi::DynamicGraph::HubNodeIdentifier& data);
		};

		template <>
		struct serializer<ai::voronoi::DynamicGraph>
		{
			static void serialize(BinarySerializer& serializer, ai::voronoi::DynamicGraph& graph);
		};
	}
}
