// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once

namespace qsf
{
	namespace ai
	{
		inline EdgePortal::EdgePortal()
		{}

		inline EdgePortal::EdgePortal(const glm::vec3& point) :
		mLeft(point),
		mRight(point)
		{}

		inline EdgePortal::EdgePortal(const glm::vec3& left, const glm::vec3& right) :
		mLeft(left),
		mRight(right)
		{}
	}

	namespace serialization
	{
		template <>
		struct serializer<ai::EdgePortal>
		{
			inline static void serialize(qsf::BinarySerializer& serializer, ai::EdgePortal& portal)
			{
				serializer & portal.mLeft;
				serializer & portal.mRight;
			}
		};
	}
}
