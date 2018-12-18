// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline HealthSpecsGroup::HealthSpecsGroup(SpecsGroupManager* specsGroupManager) :
		SpecsGroup(specsGroupManager),
		mRegenerationDelta(1.0f),
		mReanimationDelta(5.0f),
		mDeathFightDelta(4.0f),
		mReanimationLife(1.0f),
		mFireDamageConvert(0.0027f),
		mInjuryAudioTimeMinimum(qsf::Time::fromSeconds(10.0f)),
		mInjuryAudioTimeMaximum(qsf::Time::fromSeconds(30.0f)),
		mPlagueQuickTestPositivChance(0.7f),
		mCQCGangsterPoliceAttackerDamage(0.1f),
		mCQCGangsterPoliceVictimDamage(0.2f),
		mCQCGangsterGangsterAttackerDamage(0.05f),
		mCQCGangsterGangsterVictimDamage(0.05f)
	{
		// Nothing to do in here
	}

	inline HealthSpecsGroup::~HealthSpecsGroup()
	{
		// Nothing to do in here
	}

	inline float HealthSpecsGroup::getRegenerationDelta() const
	{
		return mRegenerationDelta;
	}

	inline void HealthSpecsGroup::setRegenerationDelta(float regeneration)
	{
		if (regeneration > 0.0f)
			mRegenerationDelta = regeneration;
	}

	inline float HealthSpecsGroup::getReanimationDelta() const
	{
		return mReanimationDelta;
	}

	inline void HealthSpecsGroup::setReanimationDelta(float reanimation)
	{
		if (reanimation > 0.0f)
			mReanimationDelta = reanimation;
	}

	inline float HealthSpecsGroup::getDeathFightDelta() const
	{
		return mDeathFightDelta;
	}

	inline void HealthSpecsGroup::setDeathFightDelta(float deathFightDelta)
	{
		if (deathFightDelta > 0.0f)
			mDeathFightDelta = deathFightDelta;
	}

	inline float HealthSpecsGroup::getReanimationLife() const
	{
		return mReanimationLife;
	}

	inline void HealthSpecsGroup::setReanimationLife(float lifeEnergy)
	{
		if (lifeEnergy > 0.0f)
			mReanimationLife = lifeEnergy;
	}

	inline float HealthSpecsGroup::getFireDamageConvert() const
	{
		return mFireDamageConvert;
	}

	inline qsf::Time HealthSpecsGroup::getInjuryAudioTimeMinimum() const
	{
		return mInjuryAudioTimeMinimum;
	}

	inline qsf::Time HealthSpecsGroup::getInjuryAudioTimeMaximum() const
	{
		return mInjuryAudioTimeMaximum;
	}

	inline float HealthSpecsGroup::getPlagueQuickTestPositivChance() const
	{
		return mPlagueQuickTestPositivChance;
	}

	inline float HealthSpecsGroup::getCQCGangsterPoliceAttackerDamage() const
	{
		return mCQCGangsterPoliceAttackerDamage;
	}

	inline void HealthSpecsGroup::setCQCGangsterPoliceAttackerDamage(float damage)
	{
		mCQCGangsterPoliceAttackerDamage = damage;
	}

	inline float HealthSpecsGroup::getCQCGangsterPoliceVictimDamage() const
	{
		return mCQCGangsterPoliceVictimDamage;
	}

	inline void HealthSpecsGroup::setCQCGangsterPoliceVictimDamage(float damage)
	{
		mCQCGangsterPoliceVictimDamage = damage;
	}

	inline float HealthSpecsGroup::getCQCGangsterGangsterAttackerDamage() const
	{
		return mCQCGangsterGangsterAttackerDamage;
	}

	inline void HealthSpecsGroup::setCQCGangsterGangsterAttackerDamage(float damage)
	{
		mCQCGangsterGangsterAttackerDamage = damage;
	}

	inline float HealthSpecsGroup::getCQCGangsterGangsterVictimDamage() const
	{
		return mCQCGangsterGangsterVictimDamage;
	}

	inline void HealthSpecsGroup::setCQCGangsterGangsterVictimDamage(float damage)
	{
		mCQCGangsterGangsterVictimDamage = damage;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
