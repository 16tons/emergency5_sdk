// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/GetUninitialized.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace jobs
	{


		inline JobConfiguration::JobConfiguration(Ordering ordering) :
			mOrdering(ordering),
			mNumberOfCalls(getUninitialized<uint32>()),
			mTimeBetweenCalls(Time::ZERO)
		{
			// Nothing here
		}

		inline JobDescriptor::JobDescriptor(JobProxy& jobProxy, const JobFunctionBinding& functionBinding, const JobConfiguration& jobConfiguration) :
			mJobProxy(&jobProxy),
			mFunctionBinding(functionBinding),
			mJobConfiguration(jobConfiguration),
			mValid(true),
			mForceUpdate(false),
			mCallsRemaining(jobConfiguration.mNumberOfCalls),
			mTimeSinceLastCall(Time::ZERO)
		{
			// Nothing here
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // jobs
} // qsf
