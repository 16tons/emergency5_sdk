// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline Stopwatch::Stopwatch(bool startAtOnce) :
		mRunning(false),
		mStart(Time::ZERO),
		mStop(Time::ZERO)
	{
		if (startAtOnce)
		{
			start();
		}
	}

	inline Stopwatch::~Stopwatch()
	{
		// Nothing to do in here
	}

	inline void Stopwatch::reset()
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

	inline void Stopwatch::start()
	{
		mRunning = true;
		mStart   = getSystemTime();
	}

	inline Time Stopwatch::stop()
	{
		// Is the stopwatch currently running?
		if (!mRunning)
			return Time::ZERO;

		mStop    = getSystemTime();
		mRunning = false;
		return (mStop - mStart);
	}

	inline Time Stopwatch::getElapsed() const
	{
		return mRunning ? (getSystemTime() - mStart) : (mStop - mStart);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
