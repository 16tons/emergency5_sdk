// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/error/ErrorHandling.h>
#include <qsf/serialization/binary/BinarySerializer.h>
#include <qsf/serialization/binary/BasicTypeSerialization.h>

#include <glm/glm.hpp>


namespace qsf
{
	namespace ai
	{
		inline GridCoordinates advanceInDirection(const GridCoordinates& pos, direction::TwoDimensional direction)
		{
			switch (direction)
			{
			case direction::NORTH:
				return GridCoordinates(pos.x, pos.y - 1);
			case direction::NORTH_EAST:
				return GridCoordinates(pos.x + 1, pos.y - 1);
			case direction::EAST:
				return GridCoordinates(pos.x + 1, pos.y);
			case direction::SOUTH_EAST:
				return GridCoordinates(pos.x + 1, pos.y + 1);
			case direction::SOUTH:
				return GridCoordinates(pos.x, pos.y + 1);
			case direction::SOUTH_WEST:
				return GridCoordinates(pos.x - 1, pos.y + 1);
			case direction::WEST:
				return GridCoordinates(pos.x - 1, pos.y);
			case direction::NORTH_WEST:
				return GridCoordinates(pos.x - 1, pos.y - 1);
			default:
				QSF_ERROR("unexpected direction indicator " << direction,
					QSF_REACT_THROW);
			}
		}

		inline GridDelta operator - (const GridCoordinates& lhs, const GridCoordinates& rhs)
		{
			// TODO(vs) Is this the safest way to avoid a wrap-around?
			return GridDelta((lhs.x >= rhs.x ? static_cast<int>(lhs.x - rhs.x) : -static_cast<int>(rhs.x - lhs.x)),
							 (lhs.y >= rhs.y ? static_cast<int>(lhs.y - rhs.y) : -static_cast<int>(rhs.y - lhs.y)));
		}

		inline GridCoordinates operator +(const GridCoordinates& lhs, const GridDelta& rhs)
		{
			return GridCoordinates(static_cast<unsigned int>(lhs.x + rhs.x), static_cast<unsigned int>(lhs.y + rhs.y));
		}
	}

	namespace serialization
	{
		template <>
		struct serializer<ai::GridCoordinates>
		{
			inline static void serialize(BinarySerializer& serializer, ai::GridCoordinates& data)
			{
				serializer & data.x;
				serializer & data.y;
			}
		};
	}
}
