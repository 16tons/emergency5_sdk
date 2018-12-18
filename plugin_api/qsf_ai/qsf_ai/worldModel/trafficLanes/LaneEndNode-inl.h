// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/GetUninitialized.h>
#include <qsf/serialization/binary/BinarySerializer.h>

namespace qsf
{
	namespace ai
	{
		inline LaneEndNode::LaneEndNode()
		{}

		inline LaneEndNode::LaneEndNode(const glm::vec3& pos) :
			mPosition(pos)
		{}

		inline const glm::vec3& LaneEndNode::getPosition() const
		{
			return mPosition;
		}

		inline void LaneEndNode::setPosition(const glm::vec3& position)
		{
			mPosition = position;
		}
	}

	namespace serialization
	{
		template <>
		struct serializer<ai::LaneEndNode>
		{
			inline static void serialize(BinarySerializer& serializer, ai::LaneEndNode& node)
			{
				serializer & node.mPosition;
			}
		};
	}
}
