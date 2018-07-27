// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline GangsterBaseLogic::WeaponFlagSet GangsterBaseLogic::getEquippedWeaponTypes() const
	{
		return mEquippedWeaponTypes;
	}

	inline void GangsterBaseLogic::setEquippedWeaponTypes(GangsterBaseLogic::WeaponFlagSet weapons)
	{
		mEquippedWeaponTypes = weapons;
	}

	inline GangsterBaseLogic::State GangsterBaseLogic::getCurrentState() const
	{
		return mCurrentState;
	}

	inline void GangsterBaseLogic::setEscapeTargetTag(const std::string& escapeTargetTag)
	{
		mEscapeTargetTag = escapeTargetTag;
	}

	inline GangsterBaseLogic::AutoReaction GangsterBaseLogic::getIdleReaction() const
	{
		return mIdleReaction;
	}

	inline void GangsterBaseLogic::setIdleReaction(AutoReaction autoReaction)
	{
		mIdleReaction = autoReaction;
	}

	inline GangsterBaseLogic::AutoReaction GangsterBaseLogic::getAutoTargetReaction() const
	{
		return mAutoTargetReaction;
	}

	inline void GangsterBaseLogic::setAutoTargetReaction(AutoReaction autoReaction)
	{
		mAutoTargetReaction = autoReaction;
	}

	inline bool GangsterBaseLogic::getAutoAggression() const
	{
		return mAutoAggression;
	}

	inline void GangsterBaseLogic::setAutoAggression(bool autoAggression)
	{
		mAutoAggression = autoAggression;
	}

	inline float GangsterBaseLogic::getAutoAggroRange() const
	{
		return mAutoAggroRange;
	}

	inline void GangsterBaseLogic::setAutoAggroRange(float autoAggroRange)
	{
		mAutoAggroRange = autoAggroRange;
	}

	inline float GangsterBaseLogic::getAutoDeAggroRange() const
	{
		return mAutoDeAggroRange;
	}

	inline void GangsterBaseLogic::setAutoDeAggroRange(float autoAggroRange)
	{
		mAutoDeAggroRange = autoAggroRange;
	}

	inline bool GangsterBaseLogic::getResistArrest() const
	{
		return mResistArrest;
	}

	inline void GangsterBaseLogic::setResistArrest(bool resistArrest)
	{
		mResistArrest = resistArrest;
	}

	inline float GangsterBaseLogic::getWeaponAttackDistance(weapon::Weapon weapon) const
	{
		const weapon::WeaponConfiguration* weaponConfiguration = getConfigurationForWeapon(weapon);
		if (nullptr != weaponConfiguration)
		{
			return weaponConfiguration->mShootRange;
		}

		return 0.0f;
	}

	inline void GangsterBaseLogic::setWeaponAttackDistance(weapon::Weapon weapon, float distance)
	{
		QSF_WARN_IF(weapon != weapon::PISTOL && weapon != weapon::RIFLE,"Only RIFLE and PISTOL can have custom attack distances", return);

		weapon::WeaponConfiguration* weaponConfiguration = getConfigurationForWeapon(weapon);
		if (nullptr != weaponConfiguration)
		{
			weaponConfiguration->mShootRange = distance;
		}
	}

	inline qsf::Time GangsterBaseLogic::getCelebrateWinTime() const
	{
		return mCelebrateWinTime;
	}

	inline void GangsterBaseLogic::setCelebrateWinTime(qsf::Time celebrateWinTime)
	{
		mCelebrateWinTime = celebrateWinTime;
	}

	inline bool GangsterBaseLogic::getChainReaction()
	{
		return mChainReaction;
	}

	bool GangsterBaseLogic::getPatrolOnWaypoints() const
	{
		return mPatrolOnWaypoints;
	}

	void GangsterBaseLogic::setPatrolOnWaypoints(bool patrol)
	{
		mPatrolOnWaypoints = patrol;
	}


	//[-------------------------------------------------------]
	//[ Public virtual methods                                ]
	//[-------------------------------------------------------]
	inline float GangsterBaseLogic::getMovingSpeed() const
	{
		// Default return 3.0f = running
		return 3.0f;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
