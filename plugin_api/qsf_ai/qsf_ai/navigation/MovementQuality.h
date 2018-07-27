// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once

namespace qsf
{
	namespace ai
	{
		namespace move
		{
			// Defines the quality of an atomic move step or several steps aggregated into a path (segment).
			// The quality is basically the worst problem encountered when creating this path.
			// To be able to use the semantics of greater quality being better it is ordered from worst to best value.
			// Respect this when adding new values.
			enum Quality
			{
				NEEDS_MORE_SPACE, // meaning we leave the legal area - almost always a reason to not use this option
				NEEDS_REDUCED_TURNING_RADIUS,
				NEEDS_MANEUVERING,
				IDEAL_SIMPLE_CURVE,
				IDEAL_STRAIGHT_SEGMENT,
			};
		}
	}
}
