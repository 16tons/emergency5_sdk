// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	inline UpdateInterval::UpdateInterval(UnsignedFloat interval,UnsignedFloat intervalJitter) :
		mInterval(interval),
		mIntervalJitter(intervalJitter),
		mTimer(0.0f)
	{
	}

	inline void UpdateInterval::serialize(BinarySerializer& serializer)
	{
		serializer & mInterval;
		serializer & mIntervalJitter;
		serializer & mTimer;
	}

	inline UnsignedFloat UpdateInterval::getInterval() const
	{
		return mInterval;
	}

	inline void UpdateInterval::setInterval(UnsignedFloat interval)
	{
		if (mInterval != interval)
		{
			mInterval = interval;
			resetTimer();
		}
	}

	inline UnsignedFloat UpdateInterval::getIntervalJitter() const
	{
		return mIntervalJitter;
	}

	inline void UpdateInterval::setIntervalJitter(UnsignedFloat intervalJitter)
	{
		if (mIntervalJitter != intervalJitter)
		{
			mIntervalJitter = intervalJitter;
			resetTimer();
		}
	}

	inline bool UpdateInterval::isItTimeToUpdate() const
	{
		return mTimer <= 0.0f;
	}

	inline void UpdateInterval::updateTimer(float secondsPassed)
	{
		mTimer -= secondsPassed;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
