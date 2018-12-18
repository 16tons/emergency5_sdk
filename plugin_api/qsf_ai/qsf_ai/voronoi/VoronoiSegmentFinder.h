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

#include <qsf/platform/PlatformTypes.h>

#include <boost/noncopyable.hpp>
#include <boost/container/flat_set.hpp>

#include <vector>
#include <bitset>


namespace qsf
{
	namespace ai
	{
		class TrafficLaneWorld;

		namespace voronoi
		{
			/**
			* The functionality to create Voronoi diagrams is separated into distinct algorithmic steps.
			* The parts defined inside this file identify connected segments based on a grid where the distances have already been calculated by the Voronoi distance calculator.
			* It is an intermediate step to create an abstract graph inside a traffic lane world from the grid values.
			* It is implemented as a class purely for syntactic clarity inside the algorithm.
			* Instances are used like functions where the whole algorithm is executed with the creation of the object inside its constructor.
			*/
			class QSF_AI_API_EXPORT SegmentFinder : public boost::noncopyable
			{
			public:
				/**
				* The calculator is the primary input and needs to be handled by the distance calculator before.
				* The SegmentCollection and the segment nodes vector are the output parameters that are written as a result of the algorithm.
				* They only consist of the position and the unique id defined by their index inside the vector.
				* Make sure to keep this id valid while transferring them to their final representation.
				* The minimum distance parameter defines which minimal free space is required by the entities to move along to avoid creating unimportant lanes.
				* Segments that have less free space on some parts are cut into several pieces this way.
				*/
				explicit SegmentFinder(DynamicGraph& graph, DynamicGraph::TweakedGraphDataCollection* erasedDataTracker = nullptr, unsigned int minimumDistanceNeeded = 1);

				// main algorithmic function that does all the work
				void findSegments(const GridCoordinates& windowMin, const GridCoordinates& windowMax);

				// This part of the algorithms traces connecting edges after the hubs and edges have been found.
				// It is part of the static calculation triggered when findSegments is called but it is also used as a dedicated call to reenter the lane information upon loading
				// In the latter case, we pass a fully fledged traffic lane world as a parameter to look up the resulting segment ids
				void traceConnectionsByWavefront(const TrafficLaneWorld* resultReference);

				// helper class for an intermediate lane representation as a series of cell ids
				class GridSegment
				{
				public:
					explicit GridSegment(unsigned int id = getUninitialized<unsigned int>());

					std::deque<unsigned int> mCells;
					unsigned int mLaneId;
				};

				// Retrieve part of the result, the created lanes
				const std::deque<GridSegment>& getCreatedLanes() const;
				// Retrieve part of the result, a mapping of node id to connected lane ids
				const std::multimap<unsigned int, unsigned int>& getNodeToLaneConnections() const;

			private:
				// Helper functions to track which cells have already been considered.
				// The algorithm loops over the whole area linearly but leaves this route during the creation of a segment.
				//@{
				bool wasCellConsidered(unsigned int cellIndex) const;
				void setCellConsidered(unsigned int cellIndex);
				//@}

				// Simple classification of voronoi cells
				enum VoronoiCellImportance
				{
					OBSOLETE, // can be pruned, doesn't have real value
					REGULAR_EDGE, // important cell of a regular edge
					DEAD_END,
					PART_OF_HUB, // part of a more complex cluster at an intersection point, needs to be analyzed further
					UNIQUE_HUB, // a cell that needs to remain a hub at all costs because we would loose connectivity otherwise
				};

				typedef std::bitset<direction::NUM_TWO_DIRECTIONAL_OPTIONS> DirectionFlags; // Often used set of flags which of the up to eight direction entries are used

				// Inspect a cell that is part of the voronoi diagram and try to see whether this is a hub (split node where different parts connect) or may be pruned
				// The neighbourCellIds vector is always written and contains all voronoi neighbours.
				// The two bitsets contain flags as indices into the neighbour cell vector for which of these are part of a hub or are clean edge exits
				VoronoiCellImportance analyzeVoronoiCell(unsigned int cellIndex, std::vector<unsigned int>& hubNeighbourCellIds, DirectionFlags& hubMembers, DirectionFlags& edgeExits);

				// Test whether a cell can be pruned away given its neighbours.
				// We prune if the whole neighbourhood is completely eight connected among each other so we don't cut any connection
				// Returns whether the node was pruned away
				bool checkForPrune(unsigned int cellIndex, const std::vector<direction::TwoDimensional>& neighbourDirections);

				// Analyze a hub where we have identified a cell to be part
				void analyzeHub(unsigned int hubStartCellIndex, std::vector<unsigned int>& hubCandidates, DirectionFlags hubMembers, DirectionFlags edgeExits);

				// En bloc assign ids to newly discovered hub cells
				void assignUniqueHubIds();

				// Local class containing all information about a clustering call, see function below
				class ClusterResult
				{
				public:
					ClusterResult(unsigned int numClusters, unsigned int maxClusterSize, uint8 memberFlags, uint8 exitFlags);

					unsigned int mNumClusters;
					unsigned int mMaxClusterSize;
					DirectionFlags mMaxClusterMembers; // flags for which of the parameter values are part of the max cluster
					DirectionFlags mHubExits; // flags for which of the parameter values are unique exits of the hub towards
				};

				// Analyze the passed neighbourhood of a cell as vector of directions to obstructed cells into four-connected clusters.
				// Return the number of clusters found and modifies the passed directions so that only the directions that are part of a potential unique hub remain contained
				// Assumes that the vector is ordered in a linear fashion - that is how it is always created internally
				ClusterResult clusterNeighbourHood(const std::vector<direction::TwoDimensional>& directions);

				// Calculate an absolute delta in the free space available when moving between cells
				// Does not take into account the distance, so no relative slope or something
				float getFreeSpaceDelta(unsigned int cellFromIndex, unsigned int cellToIndex) const;

				// Remove cells without a direction change or free space change so that the number of cells describing a segment becomes minimal.
				// At the same time a line connecting each successive pair of positions still only moves through the original cells.
				void removeRedundantCells(GridSegment& segment);

				// Adds a hub cell id to the collection but makes sure that the collection remains sorted.
				void addHubCell(unsigned int hubcell);

				// Extends a segment by a cell. Basically a push_back or push_front plus some optimizations to prefer diagonal movement wherever possible
				void extendSegment(GridSegment& segment, unsigned int cellId, bool appendToBack) const;

				// Finally pushes a newly found segment onto the list, also adds node to lane connections
				// In recreationMode, we only care about updating the grid information as we have the result already available and therefore save some work
				void addSegmentFound(const GridSegment& segment, bool recreationMode);

				// During dynamic updates, some lanes might only get partially erased.
				// The shortest way to recreate them is to erase the remaining data and recreate them in full, although this is not the most efficient way
				void completelyRemovePartiallyErasedLanes();

				// During dynamic updates some nodes are erased and we also need to recreate all lanes that are incident to these nodes
				// The easiest way to ensure this is to damage these lanes here by erasing the entry node.
				// Afterwards the complete erase will set in like for the normal damaged lanes
				void eraseEntryNodesToLanesForErasedNodes();


				// Dynamic data created and / or tweaked during the algorithm
				DynamicGraph& mGraph;

				// Optionally track reset data during the update here
				DynamicGraph::TweakedGraphDataCollection* mTweakedDataCollection;

				// Stores for each grid cell by index whether this was already considered by the algorithm.
				std::vector<bool> mCellConsidered;

				// tracks all connections from node id to connected lane id
				std::multimap<unsigned int, unsigned int> mNodeToLaneConnections;

				// tracks all segments found
				std::deque<GridSegment> mSegmentsFound;

				// Stores the minimum free space needed to consider creating a segment.
				// This allows to avoid creating very narrow paths in areas that can never be used by any entity.
				// Using zero here is unusual and may lead to very strange segments in places where two obstacles meet directly without any space between them.
				// It is stored as a squared value as the segments also have squared distances stored with them and we avoid drawing square roots all through the algorithm
				unsigned int mMinimumDistanceSquaredNeeded;
			};
		}
	}
}

#include "qsf_ai/voronoi/VoronoiSegmentFinder-inl.h"
