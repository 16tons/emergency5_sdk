// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline RealTimeClock::RealTimeClock() :
		Clock()
	{
		// Nothing here
	}

	inline RealTimeClock::~RealTimeClock()
	{
		// Nothing here
	}

	inline void RealTimeClock::reset()
	{
		Clock::reset();
		mLastSystemTime.reset();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
