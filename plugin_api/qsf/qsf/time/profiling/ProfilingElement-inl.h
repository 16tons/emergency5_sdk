// Copyright (C) 2012-2018 Promotion Software GmbH
#ifdef QSF_PROFILING


	//[-------------------------------------------------------]
	//[ Namespace                                             ]
	//[-------------------------------------------------------]
	namespace qsf
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline ProfilingElement::ProfilingElement() :
			mUpdates(0),
			mTotalMemory(0),
			mPeakMemory(0)
		{
			// Nothing here
		}

		inline ProfilingElement::~ProfilingElement()
		{
			// Nothing here
		}

		inline uint32 ProfilingElement::getNumberOfUpdates() const
		{
			return mUpdates;
		}

		inline void ProfilingElement::reset()
		{
			mUpdates = 0;

			// Time
			mTotalTime = Time::ZERO;
			mPeakTime = Time::ZERO;

			// Memory
			mTotalMemory = 0;
			mPeakMemory = 0;
		}

		inline void ProfilingElement::registerUpdate(const HighResolutionStopwatch& stopwatch, const ScopedMemoryZone& scopedMemoryZone)
		{
			registerUpdate(stopwatch.getElapsed(), scopedMemoryZone.getConsumed());
		}

		inline Time ProfilingElement::getTotalTime() const
		{
			return mTotalTime;
		}

		inline Time ProfilingElement::getAverageTime() const
		{
			return (mUpdates > 0) ? mTotalTime / static_cast<float>(mUpdates) : Time::ZERO;
		}

		inline Time ProfilingElement::getPeakTime() const
		{
			return mPeakTime;
		}

		inline int64 ProfilingElement::getTotalMemory() const
		{
			return mTotalMemory;
		}

		inline int64 ProfilingElement::getAverageMemory() const
		{
			return (mUpdates > 0) ? mTotalMemory / mUpdates : 0;
		}

		inline int64 ProfilingElement::getPeakMemory() const
		{
			return mPeakMemory;
		}


		//[-------------------------------------------------------]
		//[ Public virtual qsf::ProfilingElement methods          ]
		//[-------------------------------------------------------]
		inline void ProfilingElement::registerUpdate(const Time& timeElapsed, int64 memoryConsumed)
		{
			++mUpdates;

			// Time
			mTotalTime += timeElapsed;
			if (timeElapsed > mPeakTime)
				mPeakTime = timeElapsed;

			// Memory
			mTotalMemory += memoryConsumed;
			if (memoryConsumed > mPeakMemory)
				mPeakMemory = memoryConsumed;
		}


	//[-------------------------------------------------------]
	//[ Namespace                                             ]
	//[-------------------------------------------------------]
	} // qsf
#endif
