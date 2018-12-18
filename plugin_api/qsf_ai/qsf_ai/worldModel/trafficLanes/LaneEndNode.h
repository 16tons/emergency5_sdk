// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <glm/glm.hpp>

#include <cstddef>

namespace qsf
{
	namespace ai
	{
		/** A lane end node for the traffic lane world is the end point of a lane.
		* Each lane has two end nodes.
		*/
		class LaneEndNode
		{
		public:
			friend class std::allocator<LaneEndNode>;
			friend struct serialization::serializer<LaneEndNode>;

			LaneEndNode(const glm::vec3& pos);

			void setPosition(const glm::vec3& position);
			const glm::vec3& getPosition() const;

		private:
			LaneEndNode(); // needed for binary serialization

			glm::vec3 mPosition;
		};
	}
}

#include "qsf_ai/worldModel/trafficLanes/LaneEndNode-inl.h"
