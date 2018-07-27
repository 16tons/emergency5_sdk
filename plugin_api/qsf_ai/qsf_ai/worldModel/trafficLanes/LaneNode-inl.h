// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/navigation/DynamicPortal.h"

#include <qsf/serialization/binary/BinarySerializer.h>

#include <glm/glm.hpp>


namespace qsf
{
	namespace ai
	{
		template <typename Vector>
		LaneNode<Vector>::LaneNode()
		{}

		template <typename Vector>
		LaneNode<Vector>::LaneNode(const Vector& pos, UnsignedFloat freeSpace) :
			mPosition(pos),
			mFreeSpace(freeSpace)
		{}

		template <typename Vector>
		bool LaneNode<Vector>::operator ==(const LaneNode<Vector>& other) const
		{
			return mPosition == other.mPosition && mFreeSpace == other.mFreeSpace;
		}

		// Typedefs for the typical use cases
		typedef LaneNode<glm::vec3> LaneNode3D;
		typedef LaneNode<glm::vec2> LaneNode2D;

		// helper function for converting a lane node 3D to a circle portal during pathfinding which holds the same data
		inline DynamicPortal toCirclePortal(const LaneNode3D& node)
		{
			return DynamicPortal(node.mPosition, node.mFreeSpace);
		}
	}

	namespace serialization
	{
		template<typename Vector>
		struct serializer<ai::LaneNode<Vector>>
		{
			inline static void serialize(BinarySerializer& serializer, ai::LaneNode<Vector>& node)
			{
				serializer & node.mPosition;
				serializer & node.mFreeSpace;
			}
		};
	}
}
