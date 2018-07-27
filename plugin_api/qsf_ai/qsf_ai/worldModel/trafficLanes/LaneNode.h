// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/base/UnsignedFloat.h"

namespace qsf
{
	namespace ai
	{
		/** Minor helper struct that defines a node that makes up a lane.
		* Each such node is defined by a position and the available space here.
		* The end nodes of a lane are special in that these are also the nodes of a graph structure and have a unique id that is used to identify their position on the graph.
		* These are called LaneEndNodes.
		*/
		template <typename Vector>
		class LaneNode
		{
		public:
			friend class std::allocator<LaneNode<Vector>>;

			LaneNode(const Vector& pos, UnsignedFloat freeSpace);

			Vector mPosition;
			UnsignedFloat mFreeSpace;

			bool operator == (const LaneNode<Vector>& other) const;

			LaneNode(); // needed public for binary serialization and allocation inside STL containers
		};
	}
}

#include "qsf_ai/worldModel/trafficLanes/LaneNode-inl.h"
