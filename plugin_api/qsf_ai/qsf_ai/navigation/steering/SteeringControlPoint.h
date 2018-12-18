// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/base/UnsignedFloat.h"
#include "qsf_ai/navigation/TurningConfiguration.h"
#include "qsf_ai/navigation/reservation/ReservationContainer.h"
#include "qsf_ai/navigation/steering/BrakingReason.h"
#include <qsf/time/Time.h>

#include <boost/optional.hpp>


namespace qsf
{
	namespace ai
	{
		namespace steering
		{
			// Helper class encapsulating temporary information about waypoints in a path
			// holds all the information needed to navigate to a point
			class SteeringControlPoint
			{
			public:
				// Reaction types when reaching an intermediate node.
				// Can be used combined to a bitmask
				enum Reaction
				{
					NONE = 0, // for intermediate points
					PAUSE = 1 << 0, //for skipping steering updates (will continue with previous speed)
					DEMOTE_HALTING_POINT = 1 << 1,
					DO_NOT_REMOVE = 1 << 2,
					REGISTER_AT_NEXT_WAYPOINT = 1 << 3 | DO_NOT_REMOVE, //point was demoted, register at next physical waypoint with this info
					SELECT_NEXT_PATH_WAYPOINT = 1 << 4 | REGISTER_AT_NEXT_WAYPOINT | DO_NOT_REMOVE, // for the real path waypoints, also includes REGISTER_AT_NEXT_WAYPOINT and do not remove
					ADAPT_PATH = 1 << 5, // if we have left the path temporarily and need a corrected path
					FAIL_PATH = 1 << 6, // for blocked waypoints
				};

				// Delta to use for all checks whether we have left the ideal path due to round-off errors
				static const UnsignedFloat MAX_PATH_DEVIATION;
				static const UnsignedFloat MAX_SQUARED_PATH_DEVIATION;

				// Attention! The object is not completely initialized after the constructor call.
				// ...the speed is not final.
				// It is changed when the object is being handled by the AccelerationWaypointAdjuster.
				SteeringControlPoint(Time reached, UnsignedFloat desiredSpeed, const glm::vec3& wpPos, const glm::vec3& wpDirection, UnsignedFloat distance, UnsignedFloat maxSegmentSpeed, Reaction reaction,
					bool isMovingForward, float accelerationUsed, const detail::ReservationContainerEntry& reservation, const boost::optional<TurningConfiguration>& turningConfig);

				// the time it takes to reach this waypoint from the previous (estimated as accurately as possible)
				Time mTimeToReach;

				// the speed desired at the corresponding position, in order to navigate as fast as possible
				UnsignedFloat mDesiredSpeed;

				// the acceleration used in between previous and this point
				float mAccelerationUsed;

				// It holds the absolute positions taken from the path, or, for virtual points an interpolated point in between two waypoints
				glm::vec3 mPosition;
				void setPositionSafe(const glm::vec3& vec);

				// the orientation vector that the entity has when reaching this waypoint.
				glm::vec3 mDirection;

				// the distance between the previous waypoint and this one
				UnsignedFloat mDistance;

				//the max speed on the segment leading to this node
				UnsignedFloat mMaxSegmentSpeed;

				// notes a reaction that has to be done in order to safely stop at traffic lights etc.
				// Is a combination of values of the Reaction enum as a bitmask
				unsigned int mReaction;

				// true if moving forward towards this node, false if vehicle should move backwards.
				bool mIsMovingForward;

				// the reservation needed to travel this segment.
				detail::ReservationContainerEntry mReservation;

				// the configuration of a smooth turn if applicable. Uninitialized during straight segments.
				boost::optional<TurningConfiguration> mTurningConfiguration;

				// the reason why we want to brake here, NO_STOP if there is no need to brake
				BrakingReason mBrakingReason;
			};
		}
	}
}

#include "qsf_ai/navigation/steering/SteeringControlPoint-inl.h"
