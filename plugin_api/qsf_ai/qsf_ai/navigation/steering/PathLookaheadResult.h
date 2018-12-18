// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/base/UnsignedFloat.h"

#include <qsf/time/Time.h>

namespace qsf
{
	namespace ai
	{
		namespace steering
		{
			/**
			* During steering path planning, a helper struct denoting the maximal acceleration needed
			* to brake any path segment, the total lookahead of the path in seconds and distance units
			* as well as a stopping reason.
			*/
			struct PathLookaheadResult
			{
				// a hint about why the vehicle wants to stop (if any)
				// values are ordered by severity, do not change without code adaption.
				enum StoppingReason
				{
					NO_STOP,
					PATH_ENDS,
					MANEUVERING,
					PATH_LEFT,
					MISCONFIGURATION,
					HALTING_NODE,
					NODE_BLOCKED,
					AREA_BLOCKED,
				};

				PathLookaheadResult(UnsignedFloat brakingForceNeeded = UnsignedFloat::ZERO, Time timeLookedAhead = Time::ZERO, UnsignedFloat distanceLookedAhead = UnsignedFloat::ZERO, StoppingReason stoppingReason = NO_STOP);

				UnsignedFloat mBrakingForceNeeded;
				Time mTimeLookedAhead;
				UnsignedFloat mDistanceLookedAhead;
				StoppingReason mStoppingReason;
			};


			// inline implementation
			inline PathLookaheadResult::PathLookaheadResult(UnsignedFloat brakingForceNeeded, Time timeLookedAhead, UnsignedFloat distanceLookedAhead, StoppingReason stoppingReason) :
				mBrakingForceNeeded(brakingForceNeeded),
				mTimeLookedAhead(timeLookedAhead),
				mDistanceLookedAhead(distanceLookedAhead),
				mStoppingReason(stoppingReason)
			{}
		}
	}
}
