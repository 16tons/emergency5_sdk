// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline FreeplaySpecsGroup::FreeplaySpecsGroup(SpecsGroupManager* specsGroupManager) :
		SpecsGroup(specsGroupManager),
		mStartWaitDuration_Min(80.0f),
		mStartWaitDuration_Max(120.0f),
		mWaitDuration_Min(60.0f),
		mWaitDuration_Max(80.0f),
		mMinEventLimit(2),
		mMaxEventLimit(2),
		mWonEventsForNotDecrease(3),
		mWonEventsForIncrease(10),
		mWonEventsForBuyOptions(1),
		mWorldEventWaitTime(qsf::Time::fromMinutes(90.0f)),
		mWorldEventTriggerChance(0.5f),
		mWorldEventMinDurationTime(qsf::Time::fromMinutes(90.0f)),
		mWorldEventMaxDurationTime(qsf::Time::fromMinutes(90.0f)),
		mWorldEventWaitAfterEndTime(qsf::Time::fromMinutes(90.0f)),
		mGlobalEventTryCount(1),
		mDensityLimitFreeplay(50.0f),
		mDensityLimitChallenge(50.0f),
		mDensityLimitMultiplay(50.0f),
		mDensityLimitCampaign(50.0f),
		mChallengeAbortRatingDifference(10.0f)
	{
		// Nothing to do in here
	}

	inline FreeplaySpecsGroup::~FreeplaySpecsGroup()
	{
		// Nothing to do in here
	}

	inline float FreeplaySpecsGroup::getStartWaitDuration_Min() const
	{
		return mStartWaitDuration_Min;
	}

	inline float FreeplaySpecsGroup::getStartWaitDuration_Max() const
	{
		return mStartWaitDuration_Max;
	}

	inline float FreeplaySpecsGroup::getWaitDuration_Min() const
	{
		return mWaitDuration_Min;
	}

	inline float FreeplaySpecsGroup::getWaitDuration_Max() const
	{
		return mWaitDuration_Max;
	}

	inline uint32 FreeplaySpecsGroup::getMinEventLimit() const
	{
		return mMinEventLimit;
	}

	inline uint32 FreeplaySpecsGroup::getMaxEventLimit() const
	{
		return mMaxEventLimit;
	}

	inline uint32 FreeplaySpecsGroup::getWonEventsForNotDecrease() const
	{
		return mWonEventsForNotDecrease;
	}

	inline uint32 FreeplaySpecsGroup::getWonEventsForIncrease() const
	{
		return mWonEventsForIncrease;
	}

	inline uint32 FreeplaySpecsGroup::getWonEventsForBuyOptions() const
	{
		return mWonEventsForBuyOptions;
	}

	inline const qsf::Time& FreeplaySpecsGroup::getWorldEventWaitTime() const
	{
		return mWorldEventWaitTime;
	}

	inline float FreeplaySpecsGroup::getWorldEventTriggerChance() const
	{
		return mWorldEventTriggerChance;
	}

	inline const qsf::Time& FreeplaySpecsGroup::getWorldEventMinDurationTime() const
	{
		return mWorldEventMinDurationTime;
	}

	inline const qsf::Time& FreeplaySpecsGroup::getWorldEventMaxDurationTime() const
	{
		return mWorldEventMaxDurationTime;
	}

	inline const qsf::Time& FreeplaySpecsGroup::getWorldEventWaitAfterEndTime() const
	{
		return mWorldEventWaitAfterEndTime;
	}

	inline uint32 FreeplaySpecsGroup::getGlobalEventTryCount() const
	{
		return mGlobalEventTryCount;
	}

	inline float FreeplaySpecsGroup::getDensityLimitFreeplay() const
	{
		return mDensityLimitFreeplay;
	}

	inline void FreeplaySpecsGroup::setDensityLimitFreeplay(float densityLimit)
	{
		mDensityLimitFreeplay = densityLimit;
	}

	inline float FreeplaySpecsGroup::getDensityLimitChallenge() const
	{
		return mDensityLimitChallenge;
	}

	inline void FreeplaySpecsGroup::setDensityLimitChallenge(float densityLimit)
	{
		mDensityLimitChallenge = densityLimit;
	}

	inline float FreeplaySpecsGroup::getDensityLimitMultiplay() const
	{
		return mDensityLimitMultiplay;
	}

	inline void FreeplaySpecsGroup::setDensityLimitMultiplay(float densityLimit)
	{
		mDensityLimitMultiplay = densityLimit;
	}

	inline float FreeplaySpecsGroup::getDensityLimitCampaign() const
	{
		return mDensityLimitCampaign;
	}

	inline void FreeplaySpecsGroup::setDensityLimitCampaign(float densityLimit)
	{
		mDensityLimitCampaign = densityLimit;
	}

	inline float FreeplaySpecsGroup::getChallengeAbortRatingDifference() const
	{
		return mChallengeAbortRatingDifference;
	}

	inline void FreeplaySpecsGroup::setChallengeAbortRatingDifference(float difference)
	{
		mChallengeAbortRatingDifference = difference;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
