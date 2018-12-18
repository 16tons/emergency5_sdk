// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/navigation/goal/NavigationGoal.h"


namespace qsf
{
	namespace ai
	{
		namespace path
		{
			// enum to signal the result of several path adaptation algorithms
			enum AdaptionResult
			{
				NO_ADAPTATION_REQUIRED,
				ADAPTATION_SUCCESSFUL,
				ADAPTATION_FAILED,
			};
		}
	}
}
