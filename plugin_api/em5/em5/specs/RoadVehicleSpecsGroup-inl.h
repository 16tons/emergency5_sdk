// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline RoadVehicleSpecsGroup::RoadVehicleSpecsGroup(SpecsGroupManager* specsGroupManager) :
		SpecsGroup(specsGroupManager),
		mBrakeLightAccelerationThreshold(-0.5f),
		mBrakeLightDelay(0.2f),
		mSwingAccelerationThreshold(-1.0f),
		mSwingTime(3),
		mBlockedDestroyedUnitTillNextDay(false),
		mHelicopterFlyHeight(0.0f),
		mHelicopterFlyHeightSpeed(0.0f),
		mHelicoterStartLandingHeightUpper(8),
		mHelicoterStartLandingHeightLower(2),
		mHelicopterStartRaiseTillFlyHeightPercent(0.75f),
		mHelicopterFollowDistance(20.0f),
		mHelicopterExitSekHCSpeed(3),
		mDecontaminationTime(10.0f),
		mStartWithEquipmentCase(true)
	{
		// Nothing to do in here
	}

	inline RoadVehicleSpecsGroup::~RoadVehicleSpecsGroup()
	{
		// Nothing to do in here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
