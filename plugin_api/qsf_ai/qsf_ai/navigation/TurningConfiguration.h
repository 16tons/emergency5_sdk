// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/base/SpatialConfiguration.h"
#include "qsf_ai/base/UnsignedFloat.h"

#include <glm/glm.hpp>

#include <boost/optional.hpp>

namespace qsf
{
	namespace ai
	{
		class Waypoint;

		// Minor helper class containing a circle configuration for turning.
		// It is stored with each waypoint where the entity needs to take a turn instead of moving straight towards the goal.
		class TurningConfiguration
		{
		public:
			TurningConfiguration(const glm::vec2& midpoint, UnsignedFloat radius, bool clockwise);

			glm::vec2 mMidpoint; // Whether this is in absolute or local coordinates depends on the use case. Currently, this value is almost always used as absolute and only transformed once to a local value
			UnsignedFloat mRadius;
			bool mClockwise; // turning direction, not view direction / orientation at the waypoint

			bool operator ==(const TurningConfiguration& other) const;
		};


		// inline implementation
		inline TurningConfiguration::TurningConfiguration(const glm::vec2& midpoint, UnsignedFloat radius, bool clockwise) :
			mMidpoint(midpoint),
			mRadius(radius),
			mClockwise(clockwise)
		{}

		inline bool TurningConfiguration::operator ==(const TurningConfiguration& other) const
		{
			return mMidpoint == other.mMidpoint && mRadius == other.mRadius && mClockwise == other.mClockwise;
		}
	}
}
