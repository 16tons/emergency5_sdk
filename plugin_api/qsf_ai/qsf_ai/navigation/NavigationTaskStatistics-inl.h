// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/serialization/binary/BasicTypeSerialization.h>


namespace qsf
{
	namespace ai
	{
		inline NavigationTaskStatistics::NavigationTaskStatistics() :
			mNumFinishedSearches(0),
			mNumFailedSearches(0),
			mNumInterruptedSearches(0),
			mSummedExpandedSearchStates(0),
			mSummedExpandedSearchStatesDuringFailures(0),
			mPeakExpandedSearchStates(0),
			mCurrentlyScheduledMapChanges(0),
			mCurrentlyScheduledPathSearches(0),
			mNumSimpleMapUpdates(0),
			mNumVoronoiMapUpdates(0),
			mCurrentlyExecutedType(NavigationTask::DUMMY) // is used as a sign for nothing here
		{}
	}

	namespace serialization
	{
		// specializations for using the QSF binary serializer
		template <>
		struct serializer<ai::NavigationTaskStatistics>
		{
			inline static void serialize(BinarySerializer& serializer, ai::NavigationTaskStatistics& statistics)
			{
				serializer & statistics.mNumFinishedSearches;
				serializer & statistics.mNumFailedSearches;
				serializer & statistics.mNumInterruptedSearches;
				serializer & statistics.mSummedSearchTime;
				serializer & statistics.mSummedFailedSearchTime;
				serializer & statistics.mPeakWaitingTime;
				serializer.serializeAs<unsigned int>(statistics.mSummedExpandedSearchStates);
				serializer.serializeAs<unsigned int>(statistics.mSummedExpandedSearchStatesDuringFailures);
				serializer.serializeAs<unsigned int>(statistics.mPeakExpandedSearchStates);
				serializer & statistics.mNumSimpleMapUpdates;
				serializer & statistics.mSummedSimpleMapUpdateTime;
				serializer & statistics.mPeakSimpleMapUpdateTime;
				serializer & statistics.mNumVoronoiMapUpdates;
				serializer & statistics.mSummedVoronoiMapUpdateTime;
				serializer & statistics.mPeakVoronoiMapUpdateTime;

				// no need to serialize currently scheduled variables as these are recreated anyways dynamically
			}
		};
	}
}
