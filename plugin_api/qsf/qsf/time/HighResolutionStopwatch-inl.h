// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline HighResolutionStopwatch::HighResolutionStopwatch(bool startAtOnce) :
		mRunning(false),
		mStart(Time::ZERO),
		mStop(Time::ZERO)
	{
		if (startAtOnce)
		{
			start();
		}
	}

	inline HighResolutionStopwatch::~HighResolutionStopwatch()
	{
		// Nothing to do in here
	}

	inline void HighResolutionStopwatch::reset()
	{
		if (mRunning)
		{
			mStart = getSystemTime();
		}
		else
		{
			mStart = Time::ZERO;
			mStop = Time::ZERO;
		}
	}

	inline void HighResolutionStopwatch::start()
	{
		mRunning = true;
		mStart   = getSystemTime();
	}

	inline Time HighResolutionStopwatch::stop()
	{
		// Is the stopwatch currently running?
		if (!mRunning)
			return Time::ZERO;

		mStop    = getSystemTime();
		mRunning = false;
		return (mStop - mStart);
	}

	inline Time HighResolutionStopwatch::getElapsed() const
	{
		return mRunning ? (getSystemTime() - mStart) : (mStop - mStart);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
