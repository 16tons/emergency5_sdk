// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/navigation/steering/SteeringControlPoint.h"
#include "qsf_ai/navigation/TurningConfiguration.h"
#include "qsf_ai/base/UnsignedFloat.h"
#include "qsf_ai/base/SpatialConfiguration.h"

#include <qsf/time/Time.h>

#include <boost/optional.hpp>

namespace qsf
{
	namespace ai
	{
		class NavigationComponent;
		class SpeedLimitFunctor;

		namespace steering
		{
			// Helper function to move a turning configuration in global coordinates to a turning configuration in local coordinates.
			// Returns uninitialized optional if the original was also not initialized
			// TODO(vs) remove this if we decide the interpolate should work with global data
			boost::optional<TurningConfiguration> createLocalTurningConfiguration(const boost::optional<TurningConfiguration>& globalTurningConfiguration, const glm::vec3& newLocalCenter);

			// Calculates the time it takes, with a previous velocity v0 and constant acceleration a, to travel a distance d
			// Result is always positive in case acceleration is positive
			float calculateTimeForAcceleratedMovementAlongDistance(const UnsignedFloat distance, const float acceleration, const UnsignedFloat velocityAtBegin);

			//calculates the distance traveled when moving with a previous velocity v0 and constant acceleration a for a time t
			float calculateDistanceForAcceleratedMovementAlongTime(const UnsignedFloat time, const float acceleration, const UnsignedFloat velocityAtBegin);

			//calculates the acceleration needed to change from v0 to v within distance
			// A braking force is by default returned as a negative acceleration.
			// If you want a positive braking force you need to switch v and v0 so you calculate actually the speedup in the other direction
			float calculateNeededAcceleration(UnsignedFloat v, UnsignedFloat v0, UnsignedFloat distance);

			// calculates the time it takes to reach/cross a waypoint.
			Time calculateTimeToWaypointReached(const glm::vec3& waypointOffset, const glm::vec3& velocity, const UnsignedFloat maxSpeed, const UnsignedFloat maxAcceleration, const UnsignedFloat distance,
				glm::vec3& closestApproachToWaypoint, float& wpSpeed);

			// interpolates the current position (origin) towards the next waypoint, using the given velocity, time and acceleration
			// Depending on whether we pass a turning configuration it assumes linear or curved interpolation
			// All values including the turning configuration are in local coordinates.
			// The function use deltas to avoid round-off errors due to missing floating point resolution.
			// But this can get an entity stuck if we want to move an extremely small distance.
			// Therefore we may disable the lower bound delta no matter the distance via the flag useLowerBoundDelta.
			// @note May return an invalid zero direction if fed with a closestApproach that is zero itself
			SpatialConfiguration3D interpolateWaypoint(const glm::vec3& closestApproachToWaypoint, UnsignedFloat secondsLeftOnSegment, UnsignedFloat v0, float accelerationTowardsWP,
				const boost::optional<TurningConfiguration>& turningConfig, bool useLowerBoundDelta = true);

			// Variant of the above function with a shortcut if the new distance is already known rather than time left
			// @note May return an invalid zero direction if fed with a closestApproach that is zero itself
			SpatialConfiguration3D interpolateWaypoint(const glm::vec3& closestApproachToWaypoint, UnsignedFloat newDistance,
				const boost::optional<TurningConfiguration>& turningConfig, bool useLowerBoundDelta = true);

			// Functor class used to scan over the path (SteeringControlPoint structures)
			// iteration is done backwards, and each 2 consecutive points are given to the () operator.
			// waypoints are modified w.r.t. to time/desired speed so that a proper deceleration is possible.
			// additional insertion of waypoints between current and previous may be requested.
			class BrakingWaypointAdjuster
			{
			public:
				BrakingWaypointAdjuster(UnsignedFloat maxAcceleration, UnsignedFloat desiredAcceleration, UnsignedFloat maxTurningRate);

				// previous is the WP closer to the entity, current the one closer to the end.
				// if the returned optional includes a node, insert it between previous and current.
				boost::optional<SteeringControlPoint> updateSpeedData(SteeringControlPoint& current, SteeringControlPoint& previous);
			private:
				const UnsignedFloat mMaxAcceleration;
				const UnsignedFloat mDesiredAcceleration;
				const UnsignedFloat mMaxTurningRate;

				// calculates the time it takes to pass 'distance', having an initial velocity v0 and, after distance,
				// velocity v. handles the case in which v >= v0, i.e., no braking needed. in these cases, the vehicle
				// may first accelerate, then brake to reach v. vmax can not be reached, as otherwise the acceleration
				// functor would either have inserted a vmax point or the acceleration returned by the acceleration is
				// zero (not a positive value) -> we may accelerate as desired, then brake again. returns a steering control
				// point at the position of maximum velocity on this part (local vmax).
				SteeringControlPoint insertSpeedupPoint(SteeringControlPoint& current, SteeringControlPoint& previous, UnsignedFloat distance, UnsignedFloat v0, UnsignedFloat v, UnsignedFloat acceleration, UnsignedFloat brakingDistance, UnsignedFloat brakingTime, UnsignedFloat maxSegmentSpeed);
			};

			// Functor class used to scan over the path (SteeringControlPoint structures)
			// each 2 consecutive points are given to the () operator.
			// waypoints are modified with respect to desired speed so that a proper acceleration is performed.
			class AccelerationWaypointAdjuster
			{
			public:
				AccelerationWaypointAdjuster(const NavigationComponent& navi);

				// previous is the WP closer to the entity, current the one closer to the end.
				// Otherwise a direct line is assumed and the euclidean distance for the direct connection is calculated
				// This step actually makes the current data valid. Previous needs to have already valid data.
				// bool return value notes if the speed selected for either current or previous was lower than possible -
				// in this case, later the braking mechanism must be triggered to properly smooth out the speed decline.
				bool updateSpeedData(SteeringControlPoint& current, SteeringControlPoint& previous);

			private:
				const UnsignedFloat mMaxAcceleration;
				const UnsignedFloat mMaxTurningRate;
				UnsignedFloat mMaxSpeed;
				UnsignedFloat mBackwardsMaxSpeed;
			};
		}
	}
}
