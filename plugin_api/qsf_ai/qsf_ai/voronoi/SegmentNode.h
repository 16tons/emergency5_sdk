// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/voronoi/GridCoordinates.h"

namespace qsf
{
	namespace ai
	{
		namespace voronoi
		{
			/** Minor helper struct that defines a node that makes up a segment.
			* Each such node is defined by a position and the available space here which is tracked as a squared value.
			* This is a raw version of the LaneNode of a traffic lane world and will be converted at a later point.
			* TODO(vs) It is a code copy of the traffic lane world node class and should be unified if possible.
			*/
			class SegmentNode
			{
			public:
				SegmentNode(const GridCoordinates& pos, unsigned int freeSpaceSquared);

				GridCoordinates mPosition;
				unsigned int mFreeSpaceSquared;
			};


			// inline implementation
			inline SegmentNode::SegmentNode(const GridCoordinates& pos, unsigned int freeSpaceSquared) :
				mPosition(pos),
				mFreeSpaceSquared(freeSpaceSquared)
			{}
		}
	}
}
