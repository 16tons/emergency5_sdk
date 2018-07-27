// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/voronoi/GridCoordinates.h"
#include "qsf_ai/voronoi/SegmentNode.h"

#include <deque>
#include <vector>


namespace qsf
{
	namespace ai
	{
		namespace voronoi
		{
			/**
			* A segment is an intermediate data structure during the creation process of the Voronoi graph.
			* It is a raw version of a traffic lane (see class Lane) and is also only handled in a compound structure, the SegmentCollection defined below.
			*/
			class Segment
			{
			public:
				explicit Segment(unsigned int nodeEndIndex);
				// An index into a vector of segment nodes that define this segment.
				// To access all nodes for segment x you need to access the elements in the range [segment[x - 1].mNodeEndIndex, segment[x].mNodeEndIndex)
				unsigned int mNodeEndIndex;
			};

			/**
			* A segment collection is an intermediate data structure during the creation process of the Voronoi graph.
			* It contains raw versions of the final traffic lanes and therefore shares a similar structure like a LaneCollection.
			* Most importantly the individual coordinates making up a segment are stored in one linear memory structure that is shared by all segments.
			* Unlike the lane collection it still holds only 2D elements with coordinates defined by the grid cell length and not real world coordinates.
			* TODO(vs) Very ugly code copying from LaneCollection - unify the code structure or avoid the duplication altogether
			*/
			class SegmentCollection
			{
			public:
				// Automatic conversion between segment and node id relying on the fact that during automatic creation the node ids are created in a very predictable manner.
				// These functions don't check whether these ids are actually used for an actual instance.
				//@{
				static unsigned int getSegmentIdFromAnyEndNode(unsigned int endNodeId);
				static unsigned int getFirstNodeIdFromSegment(unsigned int segmentId);
				static unsigned int getLastNodeIdFromSegment(unsigned int segmentId);
				static bool isFirstNodeId(unsigned int nodeId); // else it is a potential end node id
				//@}

				// Add a new segment to the collection, returns the unique id assigned
				std::size_t addSegment(const std::vector<SegmentNode>& nodes);
				std::size_t getNumSegments() const;
				std::size_t getTotalNumberOfContainedNodes() const; // returns the size of the internal node collection for memory allocation purposes. These nodes may contain duplicates

				// Access a registered segment by id.
				// The nodes are written to the optional grid coordinates parameter as a pointer to the first node inside an array structure.
				// To access all nodes you need to iterate through the range [nodes[0], nodes[numNodes])
				const Segment& getSegment(unsigned int index, std::deque<SegmentNode>::const_iterator& begin, unsigned int& numNodes) const;

				// Comfort accessor for the free space available at some segment node with the given id which is tracked as a squared value.
				// This is a shortcut for finding the correct segment, determining whether this is the start or the end node, accessing that node and reading the free space property.
				unsigned int getFreeSpaceSquaredForNode(unsigned int nodeIndex) const;

			private:
				std::deque<SegmentNode> mSegmentNodes;
				std::deque<Segment> mSegments;
			};
		}
	}
}

#include "qsf_ai/voronoi/SegmentCollection-inl.h"
