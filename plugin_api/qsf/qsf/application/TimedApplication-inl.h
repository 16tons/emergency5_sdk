// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline TimedApplication::~TimedApplication()
	{
		// Nothing to do in here
	}

	inline RealTimeClock* TimedApplication::getRealTimeClock() const
	{
		return mRealTimeClock;
	}

	inline float TimedApplication::getFpsLimitation() const
	{
		return mFpsLimitation;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
