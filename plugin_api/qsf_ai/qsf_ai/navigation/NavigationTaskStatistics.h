// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/time/Time.h>

#include <cstddef>


namespace qsf
{
	namespace ai
	{
		/**
		* Class holding statistics about calculated path searches.
		*/
		class NavigationTaskStatistics
		{
		public:
			NavigationTaskStatistics();

			// path search statistics
			unsigned int mNumFinishedSearches;
			unsigned int mNumFailedSearches;
			unsigned int mNumInterruptedSearches;
			Time mSummedSearchTime;
			Time mSummedFailedSearchTime;
			Time mPeakWaitingTime;
			std::size_t mSummedExpandedSearchStates;
			std::size_t mSummedExpandedSearchStatesDuringFailures;
			std::size_t mPeakExpandedSearchStates;
			std::size_t mCurrentlyScheduledMapChanges;
			std::size_t mCurrentlyScheduledPathSearches;

			// map update statistics
			unsigned int mNumSimpleMapUpdates;
			Time mSummedSimpleMapUpdateTime;
			Time mPeakSimpleMapUpdateTime;

			unsigned int mNumVoronoiMapUpdates;
			Time mSummedVoronoiMapUpdateTime;
			Time mPeakVoronoiMapUpdateTime;

			// what is currently calculated?
			NavigationTask::Type mCurrentlyExecutedType;
		};
	}
}

#include "qsf_ai/navigation/NavigationTaskStatistics-inl.h"
