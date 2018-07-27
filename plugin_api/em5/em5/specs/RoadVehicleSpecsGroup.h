// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/specs/SpecsGroup.h"

#include <qsf/base/PropertyHelper.h>
#include <qsf/base/UniqueInstance.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Road vehicle specs group implementation
	*/
	class EM5_API_EXPORT RoadVehicleSpecsGroup : public SpecsGroup, public qsf::UniqueInstance<RoadVehicleSpecsGroup>
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		// Pluginable
		static const uint32 PLUGINABLE_ID;									///< "em5::ActionRangeSpecsGroup" unique pluginable specs group ID

		// Specs for all vehicles
		static const uint32 BRAKE_LIGHT_ACCELERATION_THRESHOLD;				///< "BrakeLightAccelerationThreshold" unique class property ID inside the class
		static const uint32 BRAKE_LIGHT_DELAY;								///< "BrakeLightDelay" unique class property ID inside the class
		static const uint32 SWING_ACCELERATION_THRESHOLD;					///< "SwingAccelerationThreshold" unique class property ID inside the class
		static const uint32 SWING_TIME;										///< "SwingTime" unique class property ID inside the class
		static const uint32 BLOCKED_DESTROYEDUNITS;							///< "BlockedDestryoedUnits" unique class property ID inside the class
		// Specs for helicopter
		static const uint32 HELICOPTER_FLY_HEIGHT;							///< "HelicopterFlyHeight" unique class property ID inside the class
		static const uint32 HELICOPTER_FLY_HEIGHT_SPEED;					///< "HelicopterFlyHeightSpeed" unique class property ID inside the class
		static const uint32 HELICOPTER_STARTLANDING_HEIGHT_UPPER;			///< "HelicopterStartLandingHeightUpper" unique class property ID inside the class
		static const uint32 HELICOPTER_STARTLANDING_HEIGHT_LOWER;			///< "HelicopterStartLandingHeightLower" unique class property ID inside the class
		static const uint32 HELICOPTER_STARTRAISE_TILL_FLYHEIGHT_PERCENT;	///< "HelicopterStartRaiseTillFlyHeightPercent" unique class property ID inside the class
		static const uint32 HELICOPTER_FOLLOW_DISTANCE;						///< "HelicopterFollowingDistance" unique class property ID inside the class
		static const uint32 HELICOPTER_EXIT_SEKHC_SPEED;					///< "HelicopterExitSEKHCSpeed" unique class property ID inside the class
		// Specs for decontamination vehicle
		static const uint32 DECONTAMINATION_TIME;							///< "DecontaminationTime"
		// Specs for personal
		static const uint32 PERSONAL_STARTWITHEQUIPMENTCASE;				///< "PersonalStartWithEquipmentCase"


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] specsGroupManager
		*    Optional pointer to the specs group manager this specs group should be registered to, can be a null pointer
		*/
		explicit RoadVehicleSpecsGroup(SpecsGroupManager* specsGroupManager);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~RoadVehicleSpecsGroup();

		// Specs for all vehicles
		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(BrakeLightAccelerationThreshold, float, mBrakeLightAccelerationThreshold, BRAKE_LIGHT_ACCELERATION_THRESHOLD);
		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(BrakeLightDelay, float, mBrakeLightDelay, BRAKE_LIGHT_DELAY);
		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(SwingAccelerationThreshold, float, mSwingAccelerationThreshold, SWING_ACCELERATION_THRESHOLD);
		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(SwingTime, float, mSwingTime, SWING_TIME);
		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(BlockDestroyedUnits, bool, mBlockedDestroyedUnitTillNextDay, BLOCKED_DESTROYEDUNITS);
		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(HelicopterFlyHeight, float, mHelicopterFlyHeight, HELICOPTER_FLY_HEIGHT);
		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(HelicopterFlyHeightSpeed, float, mHelicopterFlyHeightSpeed, HELICOPTER_FLY_HEIGHT_SPEED);
		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(HelicopterStartLandingHeightUpper, float, mHelicoterStartLandingHeightUpper, HELICOPTER_STARTLANDING_HEIGHT_UPPER);
		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(HelicopterStartLandingHeightLower, float, mHelicoterStartLandingHeightLower, HELICOPTER_STARTLANDING_HEIGHT_LOWER);
		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(HelicopterStartRaiseTillFlyHeightPercent, float, mHelicopterStartRaiseTillFlyHeightPercent, HELICOPTER_STARTRAISE_TILL_FLYHEIGHT_PERCENT);
		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(HelicopterFollowDistance, float, mHelicopterFollowDistance, HELICOPTER_FOLLOW_DISTANCE);
		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(HelicopterExitSekHCSpeed, float, mHelicopterExitSekHCSpeed, HELICOPTER_EXIT_SEKHC_SPEED);
		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(DecontaminationTime, float, mDecontaminationTime, DECONTAMINATION_TIME);
		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(StartWithEquipmentCase, bool, mStartWithEquipmentCase, PERSONAL_STARTWITHEQUIPMENTCASE);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system

		// Specs for all vehicles
		float mBrakeLightAccelerationThreshold;			///< Threshold for activating the brake lights
		float mBrakeLightDelay;							///< Time of delay for deactivate the brake lights
		float mSwingAccelerationThreshold;				///< Threshold for activating the swinging animation
		float mSwingTime;								///< Time how long the vehicle plays the swinging animation
		bool mBlockedDestroyedUnitTillNextDay;			///< "true" if injured/destroyed units, transported in base are blocked till the end of the day

		// Specs for helicopter
		float	mHelicopterFlyHeight;		///< The height in meters above ground map
		float	mHelicopterFlyHeightSpeed;	///< The speed the helicopter change the height
		float	mHelicoterStartLandingHeightUpper;
		float	mHelicoterStartLandingHeightLower;
		float	mHelicopterStartRaiseTillFlyHeightPercent;	///< Start process is till the percent [0-1.0] of the max fly height. (eg 15m with 0.75 and mHelicopterFlyHeight = 20)
		float	mHelicopterFollowDistance;	///< The distance the helicopter is following a vehicle or person
		float	mHelicopterExitSekHCSpeed;	///< The speed in m/s the sek specialist is roping from sek hc

		// Specs for decontamination vehicle
		float   mDecontaminationTime;		///< Time in seconds it takes to decontaminated a person

		// Specs for personal
		bool	mStartWithEquipmentCase;	///< "true" if personal automatically starts with equipment cases in hands, instead of taking them back from vehicle back door (eg. Doctor or Engineer)


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/specs/RoadVehicleSpecsGroup-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::RoadVehicleSpecsGroup)
