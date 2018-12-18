// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/navigation/Path.h"


namespace qsf
{
	class GroundMapQuery;

	namespace ai
	{
		// Function to correct the heights of waypoints with the ground map heights in mind.
		// It looks for sudden jumps in height and sees whether this was probably an incorrect switch onto / away from the bridge
		// It is an emergency measure to avoid some ugly effects of a missing feature (Support for different height levels in automatically build navigation maps).
		// It can completely be removed once the feature is available and working reliably
		void correctWaypointHeights(Path& path, GroundMapQuery& heightQuery);
	}
}
