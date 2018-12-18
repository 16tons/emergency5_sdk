// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/base/SpatialConfiguration.h"
#include "qsf_ai/navigation/TurningConstraint.h"
#include "qsf_ai/navigation/Waypoint.h"
#include "qsf_ai/navigation/pathfinding/smoothing/EdgePortal.h"

#include <deque>


namespace qsf
{
	namespace ai
	{
		class SearchContactPoint;
		class PathSearchConfiguration;

		/**
		* Create a smooth path that starts in the start config, moves successively through the input and ends up at the final position in input.
		* Writes the result parameter.
		* The core idea of the funnel is to create a shortest path inside a halfway calculated path in the form of connected world areas.
		* It tries to create a minimal distance path with as few turns as possible.
		* See http://digestingduck.blogspot.de/2010/03/simple-stupid-funnel-algorithm.html for the core idea.
		* Our version is enhanced in several ways in that we need to pay attention to turning radius.
		* The input parameter is also changed in that an edge ordering is done as a preliminary step on the input data.
		*/
		//@{
		QSF_AI_API_EXPORT void funnelSmoothing(const SpatialConfiguration3D& startConfig, unsigned int startArea, bool startsOnPrimaryMap,
			const TurningConstraint& constraints, bool mayReduceTurningCircle, std::deque<std::pair<Waypoint, EdgePortal>>& input, std::deque<Waypoint>& result, bool drawDebug = false);
		// variant that extracts the data from the typical data structures that are available during path planning
		QSF_AI_API_EXPORT void funnelSmoothing(const SearchContactPoint& startData, bool startsOnPrimaryMap, const PathSearchConfiguration& searchConfig,
			std::deque<std::pair<Waypoint, EdgePortal>>& input, std::deque<Waypoint>& result);
		//@}
	}
}
