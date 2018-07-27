// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/error/ErrorHandling.h>

namespace qsf
{
	namespace ai
	{
		namespace steering
		{
			// Several reasons for why we would want to brake at a specific waypoint
			enum BrakingReason
			{
				NO_STOP,
				PATH_ENDS,
				NODE_BLOCKED,
				AREA_BLOCKED,
				RESERVATION_MISSED,
				AVOID_COLLISION,
				PATH_FAIL
			};

			inline const char* getTextFor(BrakingReason annotation)
			{
				switch (annotation)
				{
				case NO_STOP:
					return "";
				case PATH_ENDS:
					return "Path ends.";
				case NODE_BLOCKED:
					return "Node blocked";
				case AREA_BLOCKED:
					return "Area blocked";
				case RESERVATION_MISSED:
					return "Reservation missed";
				case AVOID_COLLISION:
					return "Collision ahead";
				case PATH_FAIL:
					return "Path failed!";
				}

				QSF_ERROR("SteeringControlPoint::DebugAnnotation had an unexpected value, returning empty description", return "";);
			}
		}
	}
}
