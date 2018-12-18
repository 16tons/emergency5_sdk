// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/time/clock/Clock.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline JobArguments::JobArguments() :
		mClock(nullptr),
		mJobProxy(nullptr)
	{
		// Nothing to do in here
	}

	inline JobArguments::JobArguments(const JobArguments& jobArguments, const Time& timePassed) :
		mClock(jobArguments.mClock),
		mTimePassed(timePassed),
		mJobProxy(jobArguments.mJobProxy)
	{
		// Nothing to do in here
	}

	inline const Clock& JobArguments::getClock() const
	{
		return *mClock;
	}

	inline const Time& JobArguments::getTimePassed() const
	{
		return mTimePassed;
	}

	inline float JobArguments::getSecondsPassed() const
	{
		return mTimePassed.getSeconds();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
