// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline Clock::Clock()
	{
		reset();
	}

	inline Clock::~Clock()
	{
		// Nothing here
	}

	inline void Clock::reset()
	{
		mCurrentTime = Time::ZERO;
		mTimePassed = Time::ZERO;
		mSignalCounter = 0;
		mRemainingTimeForSignals = Time::ZERO;

		mSpeedMultiplier = 1.0f;
		mFixedTimeStep.reset();
		mMaxTimeStep.reset();
	}

	inline const Time& Clock::getCurrentTime() const
	{
		return mCurrentTime;
	}

	inline void Clock::setCurrentTime(const Time& time)
	{
		mCurrentTime = time;
	}

	inline const Time& Clock::getTimePassed() const
	{
		return mTimePassed;
	}

	inline void Clock::setTimePassed(const Time& timePassed)
	{
		mTimePassed = timePassed;
	}

	inline int Clock::getSignalCounter() const
	{
		return mSignalCounter;
	}

	inline void Clock::setSignalCounter(int signalCounter)
	{
		mSignalCounter = signalCounter;
	}

	inline float Clock::getSpeedMultiplier() const
	{
		return mSpeedMultiplier;
	}

	inline void Clock::setSpeedMultiplier(float multiplier)
	{
		mSpeedMultiplier = multiplier;
	}

	inline const boost::optional<Time>& Clock::getFixedTimeStep() const
	{
		return mFixedTimeStep;
	}

	inline void Clock::setFixedTimeStep(const boost::optional<Time>& fixedTimeStep)
	{
		mFixedTimeStep = fixedTimeStep;
	}

	inline const boost::optional<Time>& Clock::getMaxTimeStep() const
	{
		return mMaxTimeStep;
	}

	inline void Clock::setMaxTimeStep(const boost::optional<Time>& maxTimeStep)
	{
		mMaxTimeStep = maxTimeStep;
	}

	inline Time Clock::getRemainingTime() const
	{
		return mRemainingTimeForSignals;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
