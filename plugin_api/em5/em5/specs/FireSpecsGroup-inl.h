// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline FireSpecsGroup::FireSpecsGroup(SpecsGroupManager* specsGroupManager) :
		SpecsGroup(specsGroupManager),
		mFullburnFactor(0.7f),
		mCoolingBonus(1.4f),
		mExtinguishPowerExtinguisher(20),
		mExtinguishPowerHose(40),
		mExtinguishPowerTLF(60),
		mExtinguishPowerDLK(60),
		mExtinguishPowerFireboat(60),
		mExtinguishPowerCommandoCar(60),
		mCoolingPowerExtinguisher(1000),
		mCoolingPowerHose(1500),
		mCoolingPowerTLF(2000),
		mCoolingPowerDLK(2000),
		mCoolingPowerFireboat(3000),
		mCoolingPowerCommandoCar(5000),
		mRangeExtinguisher(10.0f),
		mRangeHoseMin(5.0f),
		mRangeHoseMax(15.0f),
		mRangeTLF(5.0f),
		mRangeDLK(10.0f),
		mRangeFireboatMin(10),
		mRangeFireboatMax(30),
		mRadiusCommandoCarExtinguish(20),
		mDurationCommandoCarExtingiush(3.0f),
		mCooldownCommandoCarExtinguish(40.0f),
		mCommandoCarExtinguishOffsetPosition(3.0f),
		mCommandoCarExtinguishOffsetTime(0.3f),
		mCommandoCarExtinguishStartDelay(2.0f),
		mRangeHydrant(25.0f),
		mRemoveSuitTime(60.0f),
		mRemoveMaskTime(60.0f),
		mUseGroundHydrants(true),
		mUseDynamicHydrants(false)
	{
		// Nothing to do in here
	}

	inline FireSpecsGroup::~FireSpecsGroup()
	{
		// Nothing to do in here
	}

	inline float FireSpecsGroup::getFullburnFactor() const
	{
		return mFullburnFactor;
	}

	inline float FireSpecsGroup::getCoolingBonus() const
	{
		return mCoolingBonus;
	}

	inline float FireSpecsGroup::getPowerExtinguisher() const
	{
		return mExtinguishPowerExtinguisher;
	}

	inline float FireSpecsGroup::getPowerHose() const
	{
		return mExtinguishPowerHose;
	}

	inline float FireSpecsGroup::getPowerTLF() const
	{
		return mExtinguishPowerTLF;
	}

	inline float FireSpecsGroup::getPowerDLK() const
	{
		return mExtinguishPowerDLK;
	}

	inline float FireSpecsGroup::getPowerFireboat() const
	{
		return mExtinguishPowerFireboat;
	}

	inline float FireSpecsGroup::getPowerCommandoCar() const
	{
		return mExtinguishPowerCommandoCar;
	}

	inline float FireSpecsGroup::getCoolingExtinguisher() const
	{
		return mCoolingPowerExtinguisher;
	}

	inline float FireSpecsGroup::getCoolingHose() const
	{
		return mCoolingPowerHose;
	}

	inline float FireSpecsGroup::getCoolingTLF() const
	{
		return mCoolingPowerTLF;
	}

	inline float FireSpecsGroup::getCoolingDLK() const
	{
		return mCoolingPowerDLK;
	}

	inline float FireSpecsGroup::getCoolingFireboat() const
	{
		return mCoolingPowerFireboat;
	}

	inline float FireSpecsGroup::getCoolingCommandoCar() const
	{
		return mCoolingPowerCommandoCar;
	}

	inline float FireSpecsGroup::getRangeExtinguisher() const
	{
		return mRangeExtinguisher;
	}

	inline float FireSpecsGroup::getRangeHoseMin() const
	{
		return mRangeHoseMin;
	}

	inline float FireSpecsGroup::getRangeHoseMax() const
	{
		return mRangeHoseMax;
	}

	inline float FireSpecsGroup::getRangeTLF() const
	{
		return mRangeTLF;
	}

	inline float FireSpecsGroup::getRangeDLK() const
	{
		return mRangeDLK;
	}

	inline float FireSpecsGroup::getRangeFireboatMin() const
	{
		return mRangeFireboatMin;
	}

	inline float FireSpecsGroup::getRangeFireboatMax() const
	{
		return mRangeFireboatMax;
	}

	inline float FireSpecsGroup::getRadiusCommandoCarExtinguish() const
	{
		return mRadiusCommandoCarExtinguish;
	}

	inline float FireSpecsGroup::getDurationCommandoCarExtinguish() const
	{
		return mDurationCommandoCarExtingiush;
	}

	inline float FireSpecsGroup::getCooldownCommandoCarExtinguish() const
	{
		return mCooldownCommandoCarExtinguish;
	}

	inline float FireSpecsGroup::getCommandoCarExtinguishOffsetPosition() const
	{
		return mCommandoCarExtinguishOffsetPosition;
	}

	inline float FireSpecsGroup::getCommandoCarExtinguishOffsetTime() const
	{
		return mCommandoCarExtinguishOffsetTime;
	}

	inline float FireSpecsGroup::getCommandoCarExtinguishStartDelay() const
	{
		return mCommandoCarExtinguishStartDelay;
	}

	inline float FireSpecsGroup::getRangeHydrant() const
	{
		return mRangeHydrant;
	}

	inline float FireSpecsGroup::getRemoveSuitTime() const
	{
		return mRemoveSuitTime;
	}

	inline float FireSpecsGroup::getRemoveMaskTime() const
	{
		return mRemoveMaskTime;
	}

	inline bool FireSpecsGroup::getUseGroundHydrants() const
	{
		return mUseGroundHydrants;
	}

	inline bool FireSpecsGroup::getUseDynamicHydrants() const
	{
		return mUseDynamicHydrants;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
