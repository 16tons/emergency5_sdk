// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/worldModel/trafficLanes/TrafficLaneWorld.h"
#include "qsf_ai/base/UnsignedFloat.h"
#include "qsf_ai/voronoi/GridCoordinates.h"
#include "qsf_ai/voronoi/GridConfiguration.h"
#include "qsf_ai/voronoi/DynamicVoronoiGraph.h"
#include "qsf_ai/voronoi/HeightRestriction.h"

#include <glm/glm.hpp>

#include <memory>
#include <map>

namespace qsf
{
	class GroundMapQuery;

	namespace ai
	{
		namespace voronoi
		{
			class SegmentFinder;

			/**
			* The functionality to create Voronoi diagrams is separated into distinct algorithmic steps.
			* This part creates the final traffic lanes from the result of identified and connected segments.
			* It is realized as a function object purely for coding reasons.
			* The whole logic is triggered with the convert call and the result can be fetched exactly once afterwards at any point.
			*/
			class TrafficLaneConverter
			{
			public:
				/**
				* The algorithm is executed by creating an instance of this class passing all relevant inputs to the constructor and calling convert().
				*
				* The segment collection and the segment connector are the results of previous steps of the algorithm.
				* The grid configuration needs to contain the values that were used to create the grid.
				* The y coordinate of the origin will be used as height for all positions created by the algorithm as it is now a strictly 2D algorithm.
				* TODO(vs) replace by using a getHeightAt(2DPos) function once we have terrain and similar stuff.
				* The lane type is assigned to all lanes created by this algorithm.
				*
				* @note The creation process assumes a bidirectional type as relying on a specific direction for the automatically created lanes doesn't really make sense.
				*/
				TrafficLaneConverter(TrafficLaneWorld* mapToWrite, const SegmentFinder& segmentFinder, const DynamicGraph& graph, unsigned int laneType, const HeightRestriction& restriction, int walkableLevel, bool testAtTerrainHeight);

				// Main function call creating the traffic lane world
				void convert();

				// The single parts of the creation algorithm realized as subroutines
				// For dynamic updates, these may be used individually. For a complete recreation, only the top level convert needs to be called
				//@{
				void convertGridSegments();
				void convertNodes();
				void convertNode(const DynamicGraph::HubNodeIdentifier& hubNodeId, glm::vec3& position, std::vector<unsigned int>& connectedLanes) const;
				//@}

			private:
				// Conversion helper functions
				//@{
				void prepareCoreMemoryStructures();
				UnsignedFloat convertFreeDistanceFromSquaredCellsToContinuousSpace(unsigned int squaredCellDistance) const;
				//@}

				// Helper function to convert a grid index position into continuous coordinates in 3D space
				glm::vec3 convertGridCoordinatesToVec3(const GridCoordinates& node) const;

				// Input data.
				//@{
				const SegmentFinder& mSegmentFinder;
				const DynamicGraph& mGraph;
				unsigned int mLaneType;
				HeightRestriction mHeightRestriction;
				int mWalkableLevel;
				bool mTestAtWalkable;
				//@}

				// temporary data that is created during the algorithm
				typedef std::multimap<unsigned int, unsigned int> NodeLaneConnectionMap;
				NodeLaneConnectionMap mNodeLaneConnections; // Connections from nodes (id as key) to many lanes (id as value) are tracked in this map

				// The map that is written during the conversion
				TrafficLaneWorld* mResultingMap;
			};
		}
	}
}
