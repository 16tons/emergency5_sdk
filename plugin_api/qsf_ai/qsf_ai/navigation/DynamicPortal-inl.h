// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/serialization/binary/BinarySerializer.h>
#include <qsf/serialization/binary/GlmTypeSerialization.h>

namespace qsf
{
	namespace ai
	{
		inline DynamicPortal::DynamicPortal()
		{}

		inline DynamicPortal::DynamicPortal(const glm::vec3& point, UnsignedFloat freeSpace) :
		mHomePosition(point),
		mFreeSpace(freeSpace)
		{}

		inline DynamicPortal DynamicPortal::interpolateLinearly(const DynamicPortal& previous, const DynamicPortal& current, Percentage factor)
		{
			const glm::vec3 interpolatedHomePos = previous.mHomePosition + ((current.mHomePosition - previous.mHomePosition) * *factor);
			// result has to be positive logically, may only get negative due to round-off errors
			const UnsignedFloat interpolatedFreeSpace = UncheckedUnsignedFloat(std::max(*previous.mFreeSpace + ((current.mFreeSpace - previous.mFreeSpace) * *factor), 0.f));

			return DynamicPortal(interpolatedHomePos, interpolatedFreeSpace);
		}

		inline bool  DynamicPortal::operator ==(const DynamicPortal& other) const
		{
			return mHomePosition == other.mHomePosition && mFreeSpace == other.mFreeSpace;
		}
	}

	namespace serialization
	{
		template <>
		struct serializer<ai::DynamicPortal>
		{
			inline static void serialize(BinarySerializer& serializer, ai::DynamicPortal& portal)
			{
				serializer & portal.mHomePosition;
				serializer & portal.mFreeSpace;
			}
		};
	}
}
