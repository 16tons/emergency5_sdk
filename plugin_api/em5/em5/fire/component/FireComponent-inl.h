// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	inline bool FireComponent::isBurningFireState(FireState fireState)
	{
		return (fireState == FIRESTATE_SMALLFIRE || fireState == FIRESTATE_LARGEFIRE);
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline ComplexFireComponent* FireComponent::getComplexFireComponent() const
	{
		return mComplexFireComponent;
	}

	inline float FireComponent::getEnergy() const
	{
		return mFireEnergy;
	}

	inline void FireComponent::setEnergy(float energy)
	{
		mFireEnergy = energy;
	}

	inline float FireComponent::getTotalDamage() const
	{
		return mTotalDamage;
	}

	inline void FireComponent::setTotalDamage(float damage)
	{
		mTotalDamage = damage;
	}

	inline FireComponent::FireState FireComponent::getFireState() const
	{
		return mFireState;
	}

	inline float FireComponent::getHardRadius() const
	{
		return mHardRadius;
	}

	inline void FireComponent::setHardRadius(float hardRadius)
	{
		if (mHardRadius != hardRadius)
		{
			// Soft radius must be bigger or equal than the hard radius
			if (isRunning() && hardRadius > mSoftRadius)
			{
				// Deny bigger values
				mHardRadius = mSoftRadius;
			}
			else
			{
				mHardRadius = hardRadius;
			}

			// Promote the property change
			promotePropertyChange(HARD_RADIUS);
		}
	}

	inline void FireComponent::setSoftRadius(float softRadius)
	{
		if (mSoftRadius != softRadius)
		{
			// Soft radius must be bigger or equal than the hard radius
			if (isRunning() && softRadius < mHardRadius)
			{
				// Deny smaller values
				mSoftRadius = mHardRadius;
			}
			else
			{
				mSoftRadius = softRadius;
			}

			// Promote the property change
			promotePropertyChange(SOFT_RADIUS);
		}
	}

	inline float FireComponent::getSoftRadius() const
	{
		return mSoftRadius;
	}

	inline bool FireComponent::getPlayAudio()
	{
		return mPlayAudio;
	}

	inline void FireComponent::setPlayAudio(bool playAudio)
	{
		mPlayAudio = playAudio;
	}

	inline bool FireComponent::isIntact() const
	{
		return mInternalFlags.isSet(FLAG_INTACT);
	}

	inline bool FireComponent::isBurning() const
	{
		return internalIsBurning();
	}

	inline bool FireComponent::isBurned() const
	{
		return internalIsBurned();
	}

	inline bool FireComponent::isBurnable() const
	{
		return isActive();
	}

	inline bool FireComponent::isExtinguishBlocked() const
	{
		return mExtinguishBlocked;
	}

	inline void FireComponent::setExtinguishBlocked(bool extinguishBlocked)
	{
		mExtinguishBlocked = extinguishBlocked;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	inline bool FireComponent::isExploding() const
	{
		return mIsExploding;
	}

	inline void FireComponent::setIsExploding(bool isExploded)
	{
		mIsExploding = isExploded;
	}

	inline bool FireComponent::internalIsBurning() const
	{
		return isBurningFireState(mFireState);
	}

	inline bool FireComponent::internalIsBurned() const
	{
		return (mFireState == FIRESTATE_DESTROYED);
	}


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	inline std::size_t FireComponent::EntityIdArrayInterface::size() const
	{
		return getVariable().size();
	}

	inline const uint64& FireComponent::EntityIdArrayInterface::get(std::size_t index) const
	{
		return getVariable()[index];
	}

	inline void FireComponent::EntityIdArrayInterface::set(std::size_t index, const uint64& value)
	{
		getVariable()[index] = value;
	}

	inline void FireComponent::EntityIdArrayInterface::insert(std::size_t before, const uint64& value)
	{
		getVariable().insert(getVariable().begin() + before, value);
	}

	inline void FireComponent::EntityIdArrayInterface::remove(std::size_t index)
	{
		getVariable().erase(getVariable().begin() + index);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
