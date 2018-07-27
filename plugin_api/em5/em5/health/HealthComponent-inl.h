// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline HealthComponent::HealthComponent(qsf::Prototype* prototype) :
		qsf::game::HealthComponent(prototype),
		mHealthEnergy(1000.0f),
		mMaximumHealthEnergy(1000.0f),
		mLifeEnergy(1000.0f),
		mMaximumLifeEnergy(1000.0f),
		mDeathEnergy(100.0f),
		mMaximumDeathEnergy(100.0f),
		mState(STATE_HEALTHY),
		mEnergyChangeThisUpdate(0.0f),
		mFireDamageThisUpdate(0.0f),
		mHealthBarShown(StatusOverlayComponent::BAR_TYPE_NONE),
		mInjury(nullptr),
		mContamination(Injury::CONTAMINATION_NONE),
		mInvincible(false),
		mCanDoContaminationQuickTest(true),
		mElapsedTimeAfterLastHeatEnergyReceived(qsf::Time::ZERO),
		mCanSendReceivedHeatEnergyMessage(true),
		mAttackHintWaitingTime(qsf::Time::ZERO),
		mHealthIconType(StatusOverlayComponent::STATUS_ICON_UNDEFINED)
	{
		// Set all resistances to zero
		for (int i = 0; i < damagetype::NUM_DAMAGETYPES; ++i)
		{
			mResistances[i] = 0.0f;
		}
	}

	inline HealthComponent::~HealthComponent()
	{
		mSpreadInfectionJobProxy.unregister();
	}

	inline const Injury* HealthComponent::getInjury() const
	{
		return mInjury;
	}

	inline uint32 HealthComponent::getInjuryId() const
	{
		return (nullptr != mInjury) ? mInjury->getInjuryId() : qsf::getUninitialized<uint32>();
	}

	inline const HealthComponent::ImmunityBitmask& HealthComponent::getImmunities() const
	{
		return mImmunities;
	}

	inline bool HealthComponent::getCanDoContaminationQuickTest() const
	{
		return mCanDoContaminationQuickTest;
	}

	inline void HealthComponent::setCanDoContaminationQuickTest(bool canDoContaminationQuickTest)
	{
		mCanDoContaminationQuickTest = canDoContaminationQuickTest;
	}

	inline bool HealthComponent::isInvincible() const
	{
		return mInvincible;
	}

	inline void HealthComponent::setInvincible(bool invincible)
	{
		mInvincible = invincible;

		// In case the persons is invincible, reset all incoming fire damage
		if (mInvincible)
		{
			mFireDamageThisUpdate = 0.0f;
			mRegenerationBlockedTime = qsf::Time::ZERO;
		}
	}

	inline float HealthComponent::getHealthEnergy() const
	{
		return mHealthEnergy;
	}

	inline float HealthComponent::getMaximumHealthEnergy() const
	{
		return mMaximumHealthEnergy;
	}

	inline float HealthComponent::getLifeEnergy() const
	{
		return mLifeEnergy;
	}

	inline float HealthComponent::getMaximumLifeEnergy() const
	{
		return mMaximumLifeEnergy;
	}

	inline float HealthComponent::getDeathEnergy() const
	{
		return mDeathEnergy;
	}

	inline float HealthComponent::getMaximumDeathEnergy() const
	{
		return mMaximumDeathEnergy;
	}

	inline HealthComponent::State HealthComponent::getState() const
	{
		return mState;
	}

	inline bool HealthComponent::isHealthy() const
	{
		return (mState == STATE_HEALTHY);
	}

	inline bool HealthComponent::isInjured() const
	{
		return (nullptr != mInjury);
	}

	inline bool HealthComponent::isDead() const
	{
		return (mState == STATE_DEAD);
	}

	inline bool HealthComponent::isMovable() const
	{
		// The entity can move on default so return "true"on default

		// Check the persons condition
		if (isInjured() || isDead() || isContaminated())
		{
			// If the person is unable to move return "false"
			return false;
		}

		// This person is fine so return true
		return true;
	}

	inline Injury::InjuryBitmask HealthComponent::getInjuryBitmask() const
	{
		return (nullptr != mInjury) ? mInjury->getInjuryBitmask() : Injury::InjuryBitmask();
	}

	inline VisualVitalSigns& HealthComponent::getVisualVitalSigns()
	{
		return mVisualVitalSigns;
	}

	inline bool HealthComponent::getIsDrowning() const
	{
		return (nullptr != mInjury) ? mInjury->getIsDrowning() : false;
	}

	inline bool HealthComponent::getIsKneeing() const
	{
		if (mState == HealthComponent::STATE_DEAD)
			return false;

		return (nullptr != mInjury) ? mInjury->getIsKneeing() : false;
	}

	inline bool HealthComponent::isTreatmentActive() const
	{
		return mInternalFlags.isSet(HEALTH_ACTIVE_TREATMENT);
	}

	inline bool HealthComponent::isDiagnosisActive() const
	{
		return mInternalFlags.isSet(HEALTH_ACTIVE_DIAGNOSIS);
	}

	inline bool HealthComponent::isDiagnosisComplete() const
	{
		return mInternalFlags.isSet(HEALTH_DIAGNOSIS_COMPLETE);
	}

	inline void HealthComponent::setTreatmentActive(bool treatment)
	{
		mInternalFlags.set(HEALTH_ACTIVE_TREATMENT, treatment);
	}

	inline void HealthComponent::setDiagnosisActive(bool diagnosis)
	{
		mInternalFlags.set(HEALTH_ACTIVE_DIAGNOSIS, diagnosis);
	}

	inline void HealthComponent::setMedicalCareActive(bool care)
	{
		mInternalFlags.set(HEALTH_ACTIVE_MEDICAL_CARE, care);
	}

	inline void HealthComponent::setReanimationActive(bool reanimation)
	{
		mInternalFlags.set(HEALTH_ACTIVE_REANIMATION, reanimation);
	}

	inline void HealthComponent::setInjuredHealed()
	{
		mInternalFlags.set(HEALTH_DIAGNOSIS_COMPLETE, true);
		mInternalFlags.set(HEALTH_TREATMENT_COMPLETE, true);
		mInternalFlags.set(HEALTH_INJURY_HEALED, true);
	}

	inline uint32 HealthComponent::getDiagnosisId() const
	{
		// Get the right diagnosis from the injury
		return (nullptr != mInjury) ? static_cast<uint32>(mInjury->getDiagnosisId()) : qsf::getUninitialized<uint32>();
	}

	inline bool HealthComponent::isContaminated() const
	{
		return (mContamination != Injury::CONTAMINATION_NONE);
	}

	inline Injury::ContaminationType HealthComponent::getContaminationType() const
	{
		return mContamination;
	}

	inline bool HealthComponent::isReadyForCleanup() const
	{
		// Condition for cleanup: person is dead
		return isDead();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
