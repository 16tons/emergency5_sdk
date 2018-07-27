// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/navigation/MovementQuality.h"
#include "qsf_ai/navigation/Waypoint.h"
#include "qsf_ai/navigation/Path.h"
#include "qsf_ai/navigation/pathfinding/PathSearchConfiguration.h"
#include "qsf_ai/navigation/pathfinding/SearchContactPoint.h"

#include <boost/optional.hpp>

#include <deque>


namespace qsf
{
	namespace ai
	{
		// Simple path smoothing algorithm that combines curves along the ideal lane.
		// Returns the quality of movement along the path
		move::Quality smoothIdealLaneWithCurves(const SearchContactPoint& start, const boost::optional<DynamicPortal>& startSupportPortal, std::deque<Waypoint>& waypoints,
			const boost::optional<DynamicPortal>& endSupportPortal, const PathSearchConfiguration& searchConfig, Path& path);

		// Specific evasion to the side of the road optimized for traffic lane maps
		void smoothEvadeToTheSide(Path& path, const PathSearchConfiguration& searchConfig, unsigned int fromNodeIndex, unsigned int toNodeIndex, Percentage balanceFactor);
	}
}
