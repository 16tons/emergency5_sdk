// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/serialization/binary/BasicTypeSerialization.h>

namespace qsf
{
	namespace ai
	{
		inline AreaConfiguration::AreaConfiguration(unsigned int mapId, unsigned int areaId) :
			mAreaId(areaId),
			mMapId(mapId)
		{}

		inline bool AreaConfiguration::isInitialized() const
		{
			// This checks for mData != 0xffffffffffffffff, so it's the same as checking both mMapId and mAreaId against 0xffffffff individually
			return qsf::isInitialized(mData);
		}

		inline bool operator< (const AreaConfiguration& lhs, const AreaConfiguration& rhs)
		{
			return lhs.mData < rhs.mData;
		}

		inline bool operator== (const AreaConfiguration& lhs, const AreaConfiguration& rhs)
		{
			return lhs.mData == rhs.mData;
		}

		inline bool operator!= (const AreaConfiguration& lhs, const AreaConfiguration& rhs)
		{
			return lhs.mData != rhs.mData;
		}
	}

	// serialization specialization
	namespace serialization
	{
		template <>
		struct serializer<ai::AreaConfiguration>
		{
			inline static void serialize(BinarySerializer& serializer, ai::AreaConfiguration& data)
			{
				serializer & data.mMapId;
				serializer & data.mAreaId;
			}
		};
	}
}
