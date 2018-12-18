// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/navigation/pathfinding/CombinedSearchConnectionPoint.h"

#include <deque>


namespace qsf
{
	namespace ai
	{
		class TrafficLaneWorld;

		/**
		* Calculates intersections between two traffic lane worlds
		* Writes the result as a list of CombinedSearchConnectionPoints.
		* May optionally be limited to test only some lanes by passing a reference parameter.
		*/
		void calculateIntersections(const TrafficLaneWorld& connectingWorld, const TrafficLaneWorld& connectedToWorld, std::deque<CombinedSearchConnectionPoint>& connections,
			std::vector<unsigned int>* limitedSelection = nullptr);

		/**
		* Calculates the intersections between the lanes of one map.
		* Writes the result as a list of pairs of area ids that are intersecting.
		*/
		void calculateIntersections(const TrafficLaneWorld& navMap, std::deque<std::pair<unsigned int, unsigned int>>& intersections);
	}
}
