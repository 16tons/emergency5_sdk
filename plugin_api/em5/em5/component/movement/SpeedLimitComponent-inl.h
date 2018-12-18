// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline SpeedLimitComponent::~SpeedLimitComponent()
	{
		// Nothing here
	}

	inline float SpeedLimitComponent::getRadius() const
	{
		return mRadius;
	}

	inline void SpeedLimitComponent::setRadius(float radius)
	{
		assignAndPromoteChange(mRadius, radius, SpeedLimitComponent::RADIUS);
	}

	inline float SpeedLimitComponent::getSpeedLimit() const
	{
		return mSpeedLimit;
	}

	inline void SpeedLimitComponent::setSpeedLimit(float speedLimit)
	{
		assignAndPromoteChange(mSpeedLimit, speedLimit, SpeedLimitComponent::SPEED_LIMIT);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
