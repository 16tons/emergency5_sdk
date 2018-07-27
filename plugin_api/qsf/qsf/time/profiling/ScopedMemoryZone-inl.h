// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline ScopedMemoryZone::ScopedMemoryZone(bool startAtOnce) :
		mRunning(false),
		mStart(0),
		mStop(0)
	{
		if (startAtOnce)
		{
			start();
		}
	}

	inline ScopedMemoryZone::~ScopedMemoryZone()
	{
		// Nothing to do in here
	}

	inline void ScopedMemoryZone::start()
	{
		mRunning = true;
		mStart   = getFreeVirtualMemory();
	}

	inline int64 ScopedMemoryZone::stop()
	{
		// Is the memory watch currently running?
		if (!mRunning)
			return 0;

		mStop    = getFreeVirtualMemory();
		mRunning = false;
		return (mStart - mStop);
	}

	inline int64 ScopedMemoryZone::getConsumed() const
	{
		return mRunning ? (mStart - getFreeVirtualMemory()) : (mStop - mStart);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
