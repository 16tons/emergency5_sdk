// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once

namespace qsf
{
	namespace ai
	{
		// A type to indicate an operation to execute.
		// Used as return value within AI algorithms
		namespace operation
		{
			enum Indicator
			{
				NO_OP,
				SCHEDULE,
				ESTIMATE_AND_SCHEDULE,
			};
		}
	}
}
