// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/base/DebugSetting.h"

// This file contains functionality for selecting the debugging options to use with the Voronoi generation
namespace qsf
{
	namespace ai
	{
		namespace voronoi
		{
			// The single options that can be debugged visually
			enum DebugOption
			{
				SHOW_OBSTACLES,
				SHOW_EQUIDISTANT_CELLS,
				SHOW_EQUIDISTANT_OBSTACLE_INFLUENCES,
				SHOW_RAW_SEGMENTS, // raw segments are the close cells that are identified as being equidistant to the same obstacles
				SHOW_SEGMENT_CONNECTIONS, // segment connections are created between neighbouring segment endpoints from different raw segments
			};

			const unsigned int NUM_DEBUG_OPTIONS = SHOW_SEGMENT_CONNECTIONS + 1;

			// A very simple struct to track a setting for all the available debug options.
			// Each option is either disabled or has a color and time assigned.
			// By default all options are disabled.
			class DebugSettings
			{
			public:
				ai::DebugSetting mOptions[NUM_DEBUG_OPTIONS];
			};
		}
	}
}
