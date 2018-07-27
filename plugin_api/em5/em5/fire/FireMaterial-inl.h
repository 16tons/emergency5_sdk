// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline FireMaterial::FireMaterial(uint32 key) :
		mFireMaterialId(key),
		mCombustionsValue(100.0f),
		mBurnTime(qsf::Time::fromSeconds(1.0f)),
		mStartEnergyValue(0.0f),
		mMaxEnergyValue(100.0f),
		mChargeTime(qsf::Time::fromSeconds(1.0f)),
		mSelfCoolingValue(10.0f),
		mFireResistance(0.0f),
		mBlastForce(0.0f),
		mBlastRadius(0.0f)
	{
		// Nothing to do in here
	}

	inline FireMaterial::~FireMaterial()
	{
		// Nothing to do in here
	}

	inline uint32 FireMaterial::getFireMaterialId() const
	{
		return mFireMaterialId;
	}

	inline void FireMaterial::setFireMaterialId(uint32 id)
	{
		mFireMaterialId = id;
	}

	inline const std::string& FireMaterial::getName() const
	{
		return mName;
	}

	inline void FireMaterial::setName(const std::string& name)
	{
		mName = name;
	}

	inline float FireMaterial::getCombustionValue() const
	{
		return mCombustionsValue;
	}

	inline void FireMaterial::setCombustionValue(float combustionValue)
	{
		mCombustionsValue = combustionValue;
	}

	inline qsf::Time FireMaterial::getBurnTime() const
	{
		return mBurnTime;
	}

	inline void FireMaterial::setBurnTime(qsf::Time burnTime)
	{
		// The new burn time must be greater than the charge time
		mBurnTime = (burnTime > mChargeTime) ? burnTime : mChargeTime;
	}

	inline float FireMaterial::getStartEnergyValue() const
	{
		return mStartEnergyValue;
	}

	inline void FireMaterial::setStartEnergyValue(float startEnergyValue)
	{
		mStartEnergyValue = startEnergyValue;
	}

	inline float FireMaterial::getMaxEnergyValue() const
	{
		return mMaxEnergyValue;
	}

	inline void FireMaterial::setMaxEnergyValue(float maxEnergyValue)
	{
		mMaxEnergyValue = maxEnergyValue;
		// TODO(mk) recalculate life value, and chargevalue because its depends
	}

	inline qsf::Time FireMaterial::getChargeTime() const
	{
		return mChargeTime;
	}

	inline void FireMaterial::setChargeTime(qsf::Time chargeTime)
	{
		mChargeTime = chargeTime;
	}

	inline float FireMaterial::getChargeValue() const
	{
		// We don't keep this value as member, we calculate it on the fly
		return (mMaxEnergyValue - mStartEnergyValue) / mChargeTime.getSeconds();
	}

	inline void FireMaterial::setChargeValue(float chargeValue)
	{
		// TODO(mk) temporary the setter is disabled
		// ChargeTime and ChargeValue are the same Value in different display, chargeValue is no member
		setChargeTime(qsf::Time::fromSeconds(mMaxEnergyValue/chargeValue));
	}

	inline float FireMaterial::getSelfCoolingValue() const
	{
		return mSelfCoolingValue;
	}

	inline void FireMaterial::setSelfCoolingValue(float selfCoolingValue)
	{
		mSelfCoolingValue = selfCoolingValue;
	}

	inline float FireMaterial::getFireResistance() const
	{
		return mFireResistance;
	}

	inline void FireMaterial::setFireResistance(float fireResistance)
	{
		mFireResistance = fireResistance;
	}

	inline float FireMaterial::getBlastForce() const
	{
		return mBlastForce;
	}

	inline void FireMaterial::setBlastForce(float blastForce)
	{
		mBlastForce = blastForce;
	}

	inline float FireMaterial::getBlastRadius() const
	{
		return mBlastRadius;
	}

	inline void FireMaterial::setBlastRadius(float blastRadius)
	{
		mBlastRadius = blastRadius;
	}

	inline bool FireMaterial::canExplode() const
	{
		return (mBlastForce > 0.0f && mBlastRadius > 0.0f);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
