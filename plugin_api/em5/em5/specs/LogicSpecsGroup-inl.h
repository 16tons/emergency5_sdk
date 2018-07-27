// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline LogicSpecsGroup::LogicSpecsGroup(SpecsGroupManager* specsGroupManager) :
		SpecsGroup(specsGroupManager),
		mParamedicRadius(50.0f),
		mParamedicMinimumWaitingTime(5.0f),
		mDoctorRadius(50.0f),
		mDoctorMinimumWaitingTime(5.0f),
		mFiremanRadius(50.0f),
		mFiremanMinimumWaitingTime(5.0f),
		mDiverInWaterSpeedFactor(2.5f),
		mTowcarRadius(50.0f),
		mTowcarMinimumWaitingTime(5.0f),
		mDlkRadius(50.0f),
		mDlkMinimumWaitingTime(5.0f),
		mTlfRadius(50.f),
		mMotorboatMinimumWaitingTime(5.0f),
		mMotorboatRadius(50.f),
		mFireboatRadius(50.f),
		mRescueDogNumberFailMoves(3),
		mCutTreeTime(10.0f),
		mPolicemanMinimumWaitingTime(5.0f),
		mPoliceSearchVehicleRadius(50.0f),
		mPoliceMegaphoneRadius(20.f),
		mPoliceMegaphonePlaceDistMin(1.f),
		mPoliceMegaphonePlaceDistMax(3.f),
		mPoliceMegaphoneMaxAffected(10),
		mPoliceMegaphoneTalkPause(20.f),
		mPoliceHostageTakerGiveUpChance(0.7f),
		mPoliceBarrierMinLength(1.f),
		mPoliceBarrierMaxLength(15.f),
		mPoliceBarrierStopRadius(5.f),
		mPoliceBarrierStopMove(3.f),
		mPoliceBarrierTransparency(0.5f),
		mSEKPistolShootRange(20.0f),
		mSEKGiveUpVehiclePursuit(100.0f),
		mSEKPistolHealthDamage(0.4f),
		mSEKPistolVehicleDamage(0.25f),
		mSEKStandRangeMin(3.0f),
		mSEKStandRangeMax(6.0f),
		mSEKRifleShootRange(40.0f),
		mSEKRifleHealthDamage(1.0f),
		mSEKRifleVehicleDamage(0.5f)
	{
		// Nothing to do in here
	}

	inline LogicSpecsGroup::~LogicSpecsGroup()
	{
		// Nothing to do in here
	}

	inline float LogicSpecsGroup::getParamedicSearchRadius() const
	{
		return mParamedicRadius;
	}

	inline float LogicSpecsGroup::getParamedicMinimumWaitingTime() const
	{
		return mParamedicMinimumWaitingTime;
	}

	inline float LogicSpecsGroup::getDoctorSearchRadius() const
	{
		return mDoctorRadius;
	}

	inline float LogicSpecsGroup::getDoctorMinimumWaitingTime() const
	{
		return mDoctorMinimumWaitingTime;
	}

	inline float LogicSpecsGroup::getFiremanSearchRadius() const
	{
		return mFiremanRadius;
	}

	inline float LogicSpecsGroup::getFiremanMinimumWaitingTime() const
	{
		return mFiremanMinimumWaitingTime;
	}

	inline float LogicSpecsGroup::getDiverInWaterSpeedFactor() const
	{
		return mDiverInWaterSpeedFactor;
	}

	inline float LogicSpecsGroup::getTowcarSearchRadius() const
	{
		return mTowcarRadius;
	}

	inline float LogicSpecsGroup::getTowcarMinimumWaitingTime() const
	{
		return mTowcarMinimumWaitingTime;
	}

	inline float LogicSpecsGroup::getDlkSearchRadius() const
	{
		return mDlkRadius;
	}

	inline float LogicSpecsGroup::getDlkMinimumWaitingTime() const
	{
		return mDlkMinimumWaitingTime;
	}

	inline float LogicSpecsGroup::getTlfSearchRadius() const
	{
		return mTlfRadius;
	}

	inline float LogicSpecsGroup::getMotorboatMinimumWaitingTime() const
	{
		return mMotorboatMinimumWaitingTime;
	}

	inline float LogicSpecsGroup::getMotorboatSearchRadius() const
	{
		return mMotorboatRadius;
	}

	inline float LogicSpecsGroup::getFireboatSearchRadius() const
	{
		return mFireboatRadius;
	}

	inline int LogicSpecsGroup::getRescueDogNumberFailMoves() const
	{
		return mRescueDogNumberFailMoves;
	}

	inline float LogicSpecsGroup::getCutTreeTime() const
	{
		return mCutTreeTime;
	}

	inline float LogicSpecsGroup::getPolicemanMinimumWaitingTime() const
	{
		return mPolicemanMinimumWaitingTime;
	}

	inline float LogicSpecsGroup::getPoliceSearchVehicleRadius() const
	{
		return mPoliceSearchVehicleRadius;
	}

	inline float LogicSpecsGroup::getPoliceMegaphoneRadius() const
	{
		return mPoliceMegaphoneRadius;
	}

	inline float LogicSpecsGroup::getPoliceMegaphonePlaceDistMin() const
	{
		return mPoliceMegaphonePlaceDistMin;
	}
	inline float LogicSpecsGroup::getPoliceMegaphonePlaceDistMax() const
	{
		return mPoliceMegaphonePlaceDistMax;
	}

	inline int LogicSpecsGroup::getPoliceMegaphoneMaxAffected() const
	{
		return mPoliceMegaphoneMaxAffected;
	}

	inline float LogicSpecsGroup::getPoliceMegaphoneTalkPause() const
	{
		return mPoliceMegaphoneTalkPause;
	}

	inline float LogicSpecsGroup::getPoliceHostageTakerGiveUpChance() const
	{
		return mPoliceHostageTakerGiveUpChance;
	}

	inline float LogicSpecsGroup::getPoliceBarrierMinLength() const
	{
		return mPoliceBarrierMinLength;
	}

	inline float LogicSpecsGroup::getPoliceBarrierMaxLength() const
	{
		return mPoliceBarrierMaxLength;
	}

	inline float LogicSpecsGroup::getPoliceBarrierStopRadius() const
	{
		return mPoliceBarrierStopRadius;
	}

	inline float LogicSpecsGroup::getPoliceBarrierStopMove() const
	{
		return mPoliceBarrierStopMove;
	}

	inline float LogicSpecsGroup::getPoliceBarrierTransparency() const
	{
		return mPoliceBarrierTransparency;
	}

	inline float LogicSpecsGroup::getSEKPistolShootRange() const
	{
		return mSEKPistolShootRange;
	}

	inline float LogicSpecsGroup::getSEKGiveUpVehiclePursuit() const
	{
		return mSEKGiveUpVehiclePursuit;
	}

	inline float LogicSpecsGroup::getSEKPistolHealthDamage() const
	{
		return mSEKPistolHealthDamage;
	}

	inline float LogicSpecsGroup::getSEKPistolVehicleDamage() const
	{
		return mSEKPistolVehicleDamage;
	}

	inline float LogicSpecsGroup::getSEKStandRangeMin() const
	{
		return mSEKStandRangeMin;
	}

	inline float LogicSpecsGroup::getSEKStandRangeMax() const
	{
		return mSEKStandRangeMax;
	}

	inline float LogicSpecsGroup::getSEKRifleShootRange() const
	{
		return mSEKRifleShootRange;
	}

	inline float LogicSpecsGroup::getSEKRifleHealthDamage() const
	{
		return mSEKRifleHealthDamage;
	}

	inline float LogicSpecsGroup::getSEKRifleVehicleDamage() const
	{
		return mSEKRifleVehicleDamage;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
