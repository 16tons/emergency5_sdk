// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline DamageComponent::DamageComponent(qsf::Prototype* prototype) :
		Component(prototype),
		mPeakFireEnergyPercent(0.0f),
		mDamageState(DAMAGESTATE_INTACT),
		mFireGlowIntensity(1.0f),
		mFireLifePercentage(1.0f),
		mIsDamageSimulationActive(true),
		mEmissiveMapStrength(0.0f)
	{
		// Nothing to do in here
	}

	inline DamageComponent::~DamageComponent()
	{
		// Nothing to do in here
	}

	inline DamageComponent::DamageState DamageComponent::getDamageState() const
	{
		return mDamageState;
	}

	inline float DamageComponent::getFireGlowIntensity() const
	{
		return mFireGlowIntensity;
	}

	inline void DamageComponent::setFireGlowIntensity(float fireGlowIntensity)
	{
		if (mFireGlowIntensity != fireGlowIntensity)
		{
			mFireGlowIntensity = fireGlowIntensity;
			if (isRunning())
			{
				setCustomParametersForFireGlowAndSmutOverlay();
			}
		}
	}

	inline bool DamageComponent::getDamageSimulation() const
	{
		return mIsDamageSimulationActive;
	}

	inline void DamageComponent::setDamageSimulation(bool damageSimulation)
	{
		assignAndPromoteChange(mIsDamageSimulationActive, damageSimulation, DAMAGE_SIMULATION);
	}

	inline float DamageComponent::getFireLifePercentage() const
	{
		return mFireLifePercentage;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::DamageComponent methods           ]
	//[-------------------------------------------------------]
	inline bool DamageComponent::isIntact() const
	{
		return (mDamageState == DAMAGESTATE_INTACT);
	}

	inline bool DamageComponent::isDamaged() const
	{
		return (mDamageState != DAMAGESTATE_INTACT && mDamageState != DAMAGESTATE_DESTROYED);
	}

	inline bool DamageComponent::isDamagedLevel1() const
	{
		return (mDamageState == DAMAGESTATE_DAMAGE_LEVEL1);
	}

	inline bool DamageComponent::isDamagedLevel2() const
	{
		return (mDamageState == DAMAGESTATE_DAMAGE_LEVEL2);
	}

	inline bool DamageComponent::isDestroyed() const
	{
		return (mDamageState == DAMAGESTATE_DESTROYED);
	}

	inline bool DamageComponent::isBurning() const
	{
		return (nullptr != getBurningFireComponent());
	}

	inline bool DamageComponent::isReadyForCleanup() const
	{
		const DamageState currentDamageState = getDamageState();
		return (currentDamageState != DamageState::DAMAGESTATE_INTACT);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual em5::DamageComponent methods        ]
	//[-------------------------------------------------------]
	inline void DamageComponent::onSetDamageState(DamageState)
	{
		// Default implementation is empty
	}

	inline void DamageComponent::onSetIntact()
	{
		// Default implementation is empty
	}

	inline void DamageComponent::onSetDamagedLevel1()
	{
		// Default implementation is empty
	}

	inline void DamageComponent::onSetDamagedLevel2()
	{
		// Default implementation is empty
	}

	inline void DamageComponent::onSetDestroyed()
	{
		// Default implementation is empty
	}

	inline void DamageComponent::onSetFireLifePercentage(float)
	{
		// Default implementation is empty
	}

	inline void DamageComponent::onExplosion(FireComponent*)
	{
		// Default implementation is empty
	}

	inline void DamageComponent::onFireDamageReceived(float, qsf::Component*, bool)
	{
		// Default implementation is empty
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
