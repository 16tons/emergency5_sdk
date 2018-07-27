// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"

// Contains enums and associated helper functions for describing directions within the AI
namespace qsf
{
	namespace ai
	{
		namespace direction
		{
			// Describes possible directions in one dimension
			enum OneDimensional
			{
				ANY, // also means don't care when used as a requirement
				FORWARD,
				BACKWARD,
				BOTH,
			};
			const unsigned int NUM_ONE_DIRECTIONAL_OPTIONS = BOTH + 1;

			// Describes possible general direction in 2D
			// The directions start with north and advance in clockwise order in 45 degree steps.
			enum TwoDimensional
			{
				NORTH,
				NORTH_EAST,
				EAST,
				SOUTH_EAST,
				SOUTH,
				SOUTH_WEST,
				WEST,
				NORTH_WEST,
			};
			const unsigned int NUM_TWO_DIRECTIONAL_OPTIONS = NORTH_WEST + 1;

			// Calculates the distance in direction changes given that the range is cyclic
			QSF_AI_API_EXPORT unsigned int getDistance(TwoDimensional dirA, TwoDimensional dirB);
			bool isFourConnected(TwoDimensional dir);
		}
	}
}
