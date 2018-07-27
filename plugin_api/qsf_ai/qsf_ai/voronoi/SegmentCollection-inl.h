// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/error/ErrorHandling.h>


namespace qsf
{
	namespace ai
	{
		namespace voronoi
		{
			// inline implementation of class Segment
			inline Segment::Segment(unsigned int nodeEndIndex) :
				mNodeEndIndex(nodeEndIndex)
			{}

			// inline implementation of class SegmentCollection
			inline std::size_t SegmentCollection::addSegment(const std::vector<SegmentNode>& nodes)
			{
				QSF_CHECK(!nodes.empty(), "Trying to add empty segment",
					QSF_REACT_THROW);

				QSF_CHECK(mSegments.size() < std::numeric_limits<unsigned int>::max(), "Segment index out of 32 bit bounds",
					QSF_REACT_THROW);

				QSF_CHECK(mSegmentNodes.size() + nodes.size() < std::numeric_limits<unsigned int>::max(), "Segment node index out of 32 bit bounds",
					QSF_REACT_THROW);

				mSegmentNodes.insert(mSegmentNodes.end(), nodes.begin(), nodes.end());
				mSegments.emplace_back(static_cast<unsigned int>(mSegmentNodes.size()));

				return mSegments.size() - 1;
			}

			inline std::size_t SegmentCollection::getNumSegments() const
			{
				return mSegments.size();
			}

			inline const Segment& SegmentCollection::getSegment(unsigned int index, std::deque<SegmentNode>::const_iterator& begin, unsigned int& numNodes) const
			{
				QSF_CHECK(index < getNumSegments(), "Index " << index << " out of bounds when accessing segment",
					QSF_REACT_THROW);

				const Segment& segment = mSegments[index];

				// The node one past the last of the lane with index - 1 is the first one for this segment.
				// This can't be evaluated for the very first lane whose first node is obviously the first in the whole list.
				const unsigned int firstPosIndex = (index > 0 ? mSegments[index - 1].mNodeEndIndex : 0);
				const unsigned int endPosIndex = segment.mNodeEndIndex;

				QSF_CHECK(endPosIndex >= firstPosIndex, "Corruption in segment node data structure, endIndex < startIndex",
					QSF_REACT_THROW);

				begin = mSegmentNodes.begin() + firstPosIndex;
				numNodes = endPosIndex - firstPosIndex;

				return segment;
			}

			inline unsigned int SegmentCollection::getFreeSpaceSquaredForNode(unsigned int nodeId) const
			{
				const unsigned int segmentId = getSegmentIdFromAnyEndNode(nodeId);
				const bool useFirstNode = isFirstNodeId(nodeId);

				std::deque<SegmentNode>::const_iterator begin;
				unsigned int numNodes = 0;
				getSegment(segmentId, begin, numNodes);

				return (useFirstNode ? *begin : *(begin + numNodes - 1)).mFreeSpaceSquared;
			}

			inline unsigned int SegmentCollection::getSegmentIdFromAnyEndNode(unsigned int nodeId)
			{
				return nodeId / 2;
			}

			inline unsigned int SegmentCollection::getFirstNodeIdFromSegment(unsigned int segmentId)
			{
				return segmentId * 2;
			}

			inline unsigned int SegmentCollection::getLastNodeIdFromSegment(unsigned int segmentId)
			{
				return (segmentId * 2) + 1;
			}

			inline bool SegmentCollection::isFirstNodeId(unsigned int nodeId)
			{
				return nodeId % 2 == 0;
			}

			inline std::size_t SegmentCollection::getTotalNumberOfContainedNodes() const
			{
				return mSegmentNodes.size();
			}
		}
	}
}
