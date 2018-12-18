// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/navigation/Waypoint.h"

#include <deque>

namespace qsf
{
	namespace ai
	{
		// a typedef for the temporary path data structure to stay compatible to the old naming
		typedef std::deque<Waypoint> PathUnderConstruction;
	}
}
