// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline uint64 WaterjetComponent::getTargetEntityId() const
	{
		return mTargetEntityId;
	}

	inline void WaterjetComponent::setTargetEntityId(uint64 entityId)
	{
		if (entityId == 0)
		{
			qsf::setUninitialized(entityId);
		}

		assignAndPromoteChange(mTargetEntityId, entityId, TARGET_ENTITY_ID);
	}

	inline float WaterjetComponent::getAngleInRadians() const
	{
		return mAngleInRadians;
	}

	inline void WaterjetComponent::setAngleInRadians(float angle)
	{
		if (mAngleInRadians != angle)
		{
			mAngleInRadians = angle;

			// Update waterjet
			mLastDistance = 0.0f;

			promotePropertyChange(ANGLE_IN_RADIANS);
			promotePropertyChange(ANGLE_IN_DEGREES);
		}
	}

	inline float WaterjetComponent::getAngleInDegrees() const
	{
		return glm::degrees(mAngleInRadians);
	}

	inline void WaterjetComponent::setAngleInDegrees(float angle)
	{
		setAngleInRadians(glm::radians(angle));
	}

	inline float WaterjetComponent::getStartRadius() const
	{
		return mStartRadius;
	}

	inline void WaterjetComponent::setStartRadius(float startRadius)
	{
		if (mStartRadius != startRadius)
		{
			mStartRadius = startRadius;

			// Update waterjet
			mLastDistance = 0.0f;

			promotePropertyChange(START_RADIUS);
		}
	}

	inline float WaterjetComponent::getEndRadius() const
	{
		return mEndRadius;
	}

	inline void WaterjetComponent::setEndRadius(float endRadius)
	{
		if (mEndRadius != endRadius)
		{
			mEndRadius = endRadius;

			// Update waterjet
			mLastDistance = 0.0f;

			promotePropertyChange(END_RADIUS);
		}
	}

	inline float WaterjetComponent::getEndRadiusYFactor() const
	{
		return mEndRadiusYFactor;
	}

	inline void WaterjetComponent::setEndRadiusYFactor(float endRadiusYFactor)
	{
		if (mEndRadiusYFactor != endRadiusYFactor)
		{
			mEndRadiusYFactor = endRadiusYFactor;

			// Update waterjet
			mLastDistance = 0.0f;

			promotePropertyChange(END_RADIUS_Y_FACTOR);
		}
	}

	inline float WaterjetComponent::getEndBottomLapse() const
	{
		return mEndBottomLapse;
	}

	inline void WaterjetComponent::setEndBottomLapse(float endBottomLapse)
	{
		if (mEndBottomLapse != endBottomLapse)
		{
			mEndBottomLapse = endBottomLapse;

			// Update waterjet
			mLastDistance = 0.0f;

			promotePropertyChange(END_BOTTOM_LAPSE);
		}
	}

	inline bool WaterjetComponent::getDisableAllParticles() const
	{
		return mDisableAllParticles;
	}

	inline void WaterjetComponent::setDisableAllParticles(bool state)
	{
		if (mDisableAllParticles != state)
		{
			mDisableAllParticles = state;

			if (isRunning() && state)
			{
				destroyParticles();
			}

			promotePropertyChange(DISABLE_ALL_PARTICLES);
		}
	}

	inline bool WaterjetComponent::getDisableCollisionChecks() const
	{
		return mDisableCollisionChecks;
	}

	inline void WaterjetComponent::setDisableCollisionChecks(bool state)
	{
		if (mDisableCollisionChecks != state)
		{
			mDisableCollisionChecks = state;

			// Update waterjet
			mLastDistance = 0.0f;

			promotePropertyChange(DISABLE_COLLISION_CHECKS);
		}
	}

	inline uint64 WaterjetComponent::getHitEntityId() const
	{
		return mHitEntityId;
	}

	inline const glm::vec3& WaterjetComponent::getHitPosition() const
	{
		return mHitPosition;
	}

	inline const glm::vec3& WaterjetComponent::getHitGroundPosition() const
	{
		return mHitGroundPosition;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
