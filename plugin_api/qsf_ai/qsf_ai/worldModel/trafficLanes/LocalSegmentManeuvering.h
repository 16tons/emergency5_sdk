// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/worldModel/trafficLanes/LaneNode.h"
#include "qsf_ai/navigation/Path.h"
#include "qsf_ai/navigation/MovementQuality.h"
#include "qsf_ai/base/SpatialConfiguration.h"
#include "qsf_ai/navigation/TurningConstraint.h"
#include "qsf_ai/base/Radians.h"

#include <glm/glm.hpp>

#include <boost/optional.hpp>

#include <vector>


// This file contains functionality to plan maneuvers on a single lane segment.
namespace qsf
{
	namespace ai
	{
		/** Function to estimate the steps for maneuvering to a desired orientation.
		* Parameters are the angle to change and the available free space and the turning constraint.
		* Returns an uninitialized value if the free space is too small to allow maneuvering at all.
		*/
		boost::optional<UnsignedFloat> estimateManeuverToOrientationSteps(const TurningConstraint& constraint, const Radians& angle, UnsignedFloat freeSpace);

		/** Estimate the costs it takes to maneuver onto the ideal lane.
		* Returns an uninitialized value if the setting indicates we can't maneuver at all.
		* Does not contain cost factor for the pure distance to move, only the maneuvering overhead itself.
		*/
		boost::optional<UnsignedFloat> estimateManeuverToIdealLaneCosts(const TurningConstraint& constraint, const glm::vec3& orientation3D, const glm::vec3& towardsGoal3D,
			UnsignedFloat freeSpaceAvailable, bool mayMoveThroughIllegalArea);

		/** Function to find a way onto the ideal lane of a segment.
		* We are moving to the goal node and the predecessor is the last node before that one.
		* This defines the direction as well as the available space.
		* The result is written into the path reference parameter.
		* Returns the quality of the created path.
		* For the movement options bitmask parameter see NavigationComponent::MovementFlags
		*/
		QSF_AI_API_EXPORT move::Quality maneuverToIdealLane(const LaneNode3D& predecessor, const LaneNode3D& goal, const SpatialConfiguration3D& startConfig,
			const TurningConstraint& constraint, short movementOptions, unsigned int worldElementId, unsigned int maxManeuverSteps, Path& path);

		/** Try to maneuver in place to reach a desired orientation.
		* The return value signals whether a solution was found and is equal to the solution optional being initialized inside the function.
		*/
		QSF_AI_API_EXPORT bool maneuverToDesiredOrientation(const LaneNode3D& center, const glm::vec3& startPosition, const glm::vec3& startOrientation, const glm::vec3& goalPosition,
			bool leaveForward, const TurningConstraint& constraint, unsigned int maxManeuverSteps, Path& solution);
	}
}
