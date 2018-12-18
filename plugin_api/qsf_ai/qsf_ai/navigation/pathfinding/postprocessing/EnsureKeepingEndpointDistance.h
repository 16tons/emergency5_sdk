// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/navigation/goal/NavigationGoal.h"

namespace qsf
{
	namespace ai
	{
		class Path;

		namespace EnsureKeepingEndpointDistance
		{
			/**
			* Implements a method to make sure the desired distance to the goal is kept.
			* It modifies the waypoints such that the last waypoint is -distance- units away.
			*/
			void ensureKeepingEndpointDistance(Path& path, const NavigationGoal::GoalConfigurationList& goalConfigs);

			//internal helper function, called by ensureKeepingEndpointDistance
			void keepDistanceToSingleWaypoint(Path& path, const logic::TargetPoint& targetPoint);
			void cutoffPathAfterIndex(Path& path, unsigned int index, const logic::TargetPoint& targetPoint);
			Waypoint modifyWaypointToMatchCircleLinearly(Path& path, unsigned int toModify, const logic::TargetPoint& targetPoint);
			Waypoint modifyWaypointToMatchCircleCircular(Path& path, unsigned int toModify, const logic::TargetPoint& targetPoint);
		}
	}
}
