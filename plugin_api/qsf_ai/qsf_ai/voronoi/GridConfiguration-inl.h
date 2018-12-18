// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/serialization/binary/GlmTypeSerialization.h>


namespace qsf
{
	namespace ai
	{
		inline glm::vec3 GridConfiguration::convertPositionToMidpoint(const GridCoordinates& position) const
		{
			return mOrigin + glm::vec3(position.x * mCellSize, 0.f, position.y * mCellSize) + glm::vec3(*mCellSize / 2.f, 0.f, *mCellSize / 2.f);
		}

		inline std::pair<glm::vec3, glm::vec3>GridConfiguration::convertPositionToCorners(const GridCoordinates& position) const
		{
			const glm::vec3 corner1 = mOrigin + glm::vec3(position.x * mCellSize, 0.f, position.y * mCellSize);
			const glm::vec3 corner2 = corner1 + glm::vec3(mCellSize, 0.f, mCellSize);
			return std::make_pair(corner1, corner2);
		}

		inline GridCoordinates GridConfiguration::convertPosition(const glm::vec3& position) const
		{
			const glm::vec3 localizedPos = (position - mOrigin);
			return GridCoordinates(localizedPos.x / mCellSize, localizedPos.z / mCellSize);
		}
	}

	namespace serialization
	{
		template<>
		struct serializer<ai::GridConfiguration>
		{
			inline static void serialize(BinarySerializer& serializer, ai::GridConfiguration& data)
			{
				serializer & data.mOrigin;
				serializer & data.mDimensions;
				serializer & data.mCellSize;
			}
		};
	}
}
