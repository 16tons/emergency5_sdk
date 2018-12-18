// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline ActionRangeSpecsGroup::ActionRangeSpecsGroup(SpecsGroupManager* specsGroupManager) :
		SpecsGroup(specsGroupManager),
		mCutTreeDistance(5.0f),
		mCutHedgeDistance(5.0f),
		mCutCarDistance(5.0f),
		mCarryPersonDistance(5.0f),
		mGuideGangsterDistance(5.0f),
		mPlacePersonDistance(5.0f),
		mTreatPersonDistance(5.0f),
		mEnterVehicleDistance(5.0f),
		mEnterBuildingDistance(5.0f),
		mPullObjectDistance(5.0f),
		mPullPersonDistance(5.0f),
		mRepairObjectDistance(5.0f),
		mTakeEquipmentDistance(1000.0f),
		mExtinguishFireDistance(5.0f),
		mUsePepperSprayDistance(5.0f),
		mArrestGangsterDistance(5.0f),
		mGotoHydrantDistance(5.0f),
		mSearchRoadVehicleDistance(5000.0f),
		mPickPocketDistance(5.0f),
		mRescueDogSearchDistance(50.0f),
		mStunGrenadeRadius(15.0f),
		mStunGrenadeThrowDistance(35.0f),
		mStunGrenadeNoFollowDistance(200.f),
		mStunGrenadeStunTime(5.0f),
		mGangsterSearchDistance(40.0f),
		mGangsterFearDistance(20.0f),
		mTowCarRange(10.0f),
		mHeavyCraneRange(14.0f),
		mDlkRange(24.0f)
	{
		// Nothing to do in here
	}

	inline ActionRangeSpecsGroup::~ActionRangeSpecsGroup()
	{
		// Nothing to do in here
	}

	inline float ActionRangeSpecsGroup::getCutTreeDistance() const
	{
		return mCutTreeDistance;
	}

	inline float ActionRangeSpecsGroup::getCutHedgeDistance() const
	{
		return mCutHedgeDistance;
	}

	inline float ActionRangeSpecsGroup::getCutCarDistance() const
	{
		return mCutCarDistance;
	}

	inline float ActionRangeSpecsGroup::getCarryPersonDistance() const
	{
		return mCarryPersonDistance;
	}

	inline float ActionRangeSpecsGroup::getGuideGangsterDistance() const
	{
		return mGuideGangsterDistance;
	}

	inline float ActionRangeSpecsGroup::getPlacePersonDistance() const
	{
		return mPlacePersonDistance;
	}

	inline float ActionRangeSpecsGroup::getTreatPersonDistance() const
	{
		return mTreatPersonDistance;
	}

	inline float ActionRangeSpecsGroup::getEnterBuildingDistance() const
	{
		return mEnterBuildingDistance;
	}

	inline float ActionRangeSpecsGroup::getEnterVehicleDistance() const
	{
		return mEnterVehicleDistance;
	}

	inline float ActionRangeSpecsGroup::getPullObjectDistance() const
	{
		return mPullObjectDistance;
	}

	inline float ActionRangeSpecsGroup::getPullPersonDistance() const
	{
		return mPullPersonDistance;
	}

	inline float ActionRangeSpecsGroup::getRepairObjectDistance() const
	{
		return mRepairObjectDistance;
	}

	inline float ActionRangeSpecsGroup::getTakeEquipmentDistance() const
	{
		return mTakeEquipmentDistance;
	}

	inline float ActionRangeSpecsGroup::getExtinguishFireDistance() const
	{
		return mExtinguishFireDistance;
	}

	inline float ActionRangeSpecsGroup::getArrestGangsterDistance() const
	{
		return mArrestGangsterDistance;
	}

	inline float ActionRangeSpecsGroup::getUsePepperSprayDistance() const
	{
		return mUsePepperSprayDistance;
	}

	inline float ActionRangeSpecsGroup::getGotoHydrantDistance() const
	{
		return mGotoHydrantDistance;
	}

	inline float ActionRangeSpecsGroup::getSearchRoadVehicleDistance() const
	{
		return mSearchRoadVehicleDistance;
	}

	inline float ActionRangeSpecsGroup::getPickPocketDistance() const
	{
		return mPickPocketDistance;
	}

	inline float ActionRangeSpecsGroup::getRescueDogSearchDistance() const
	{
		return mRescueDogSearchDistance;
	}

	inline float ActionRangeSpecsGroup::getStunGrenadeRadius() const
	{
		return mStunGrenadeRadius;
	}

	inline float ActionRangeSpecsGroup::getStunGrenadeThrowDistance() const
	{
		return mStunGrenadeThrowDistance;
	}

	inline float ActionRangeSpecsGroup::getStunGrenadeNoFollowDistance() const
	{
		return mStunGrenadeNoFollowDistance;
	}

	inline float ActionRangeSpecsGroup::getStunGrenadeStunTime() const
	{
		return mStunGrenadeStunTime;
	}

	inline float ActionRangeSpecsGroup::getGangsterSearchDistance() const
	{
		return mGangsterSearchDistance;
	}

	inline float ActionRangeSpecsGroup::getGangsterFearDistance() const
	{
		return mGangsterFearDistance;
	}

	inline float ActionRangeSpecsGroup::getTowcarDistance() const
	{
		return mTowCarRange;
	}

	inline float ActionRangeSpecsGroup::getHeavyCraneDistance() const
	{
		return mHeavyCraneRange;
	}

	inline float ActionRangeSpecsGroup::getDlkDistance() const
	{
		return mDlkRange;
	}

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
