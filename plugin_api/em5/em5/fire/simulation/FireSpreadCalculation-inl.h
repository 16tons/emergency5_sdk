// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline const std::vector<FireSpreadCalculation::DebugRequestData>& FireSpreadCalculation::getDebugDrawRequests() const
	{
		return mDebugDrawRequestsCache;
	}

	inline void FireSpreadCalculation::setDebugActive(bool isActive)
	{
		mShowDebug = isActive;
	}

	inline int64 FireSpreadCalculation::getCalculationTime() const
	{
		return mCalculationTime;
	}

	inline const qsf::Time FireSpreadCalculation::getTimePassed() const
	{
		return qsf::Time::fromSeconds(mSecondsPassed);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
