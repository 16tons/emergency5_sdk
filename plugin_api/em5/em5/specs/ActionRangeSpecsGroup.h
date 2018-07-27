// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/specs/SpecsGroup.h"

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
	*    Action range specs group implementation
	*/
	class EM5_API_EXPORT ActionRangeSpecsGroup : public SpecsGroup, public qsf::UniqueInstance<ActionRangeSpecsGroup>
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		// Pluginable
		static const uint32 PLUGINABLE_ID;					///< "em5::ActionRangeSpecsGroup" unique pluginable specs group ID

		static const uint32 CUT_TREE_DISTANCE;				///< "CutTreeDistance" unique class property ID inside the class
		static const uint32 CUT_HEDGE_DISTANCE;				///< "CutHedgeDistance" unique class property ID inside the class
		static const uint32 CUT_CAR_DISTANCE;				///< "CutCarDistance" unique class property ID inside the class
		static const uint32 CARRY_PERSON_DISTANCE;			///< "CarryPersonDistance" unique class property ID inside the class
		static const uint32 GUIDE_GANGSTER_DISTANCE;		///< "GuideGangsterDistance" unique class property ID inside the class
		static const uint32 PLACE_PERSON_DISTANCE;			///< "PlacePersonDistance" unique class property ID inside the class
		static const uint32 TREAT_PERSON_DISTANCE;			///< "TreatPersonDistance" unique class property ID inside the class
		static const uint32 ENTER_VEHICLE_DISTANCE;			///< "EnterVehicleDistance" unique class property ID inside the class
		static const uint32 ENTER_BUILDING_DISTANCE;		///< "EnterBuildingDistance" unique class property ID inside the class
		static const uint32 PULL_OBJECT_DISTANCE;			///< "PullObjectDistance" unique class property ID inside the class
		static const uint32 PULL_PERSON_DISTANCE;			///< "PullPersonDistance" unique class property ID inside the class
		static const uint32 REPAIR_OBJECT_DISTANCE;			///< "RepairObjectDistance" unique class property ID inside the class
		static const uint32 TAKE_EQUIPMENT_DISTANCE;		///< "TakeEquipmentDistance" unique class property ID inside the class
		static const uint32 EXTINGUISH_FIRE_DISTANCE;		///< "ExtinguishFireDistance" unique class property ID inside the class
		static const uint32 USE_PEPPER_SPRAY_DISTANCE;		///< "UsePepperSprayDistance" unique class property ID inside the class
		static const uint32 ARREST_GANGSTER_DISTANCE;		///< "ArrestGangsterDistance" unique class property ID inside the class
		static const uint32 GOTO_HYDRANT_DISTANCE;			///< "GotoHydrantDistance" unique class property ID inside the class
		static const uint32 SEARCH_ROADVEHICLE_DISTANCE;	///< "SearchRoadVehicleDistance" unique class property ID inside the class
		static const uint32 PICK_POCKET_DISTANCE;			///< "PickPocketDistance" unique class property ID inside the class
		static const uint32 RESCUEDOG_SEARCH_DISTANCE;		///< "RescueDogSearchDistance" unique class property ID inside the class
		static const uint32 STUN_GRENADE_RADIUS;			///< "StunGrenadeRadius" unique class property ID inside the class
		static const uint32	STUN_GRENADE_THROW_DISTANCE;	///< "StunGrenadeThrowDistance" unique class property ID inside the class
		static const uint32 STUN_GRENADE_NO_FOLLOW_DISTANCE;///< "StunGrenadeNoFollowDistance" unique class property ID inside the class
		static const uint32 STUN_GRENADE_STUN_TIME;			///< "StunGrenadeStunTime" unique class property ID inside the class
		static const uint32 GANGSTER_SEARCH_DISTANCE;		///< "GangsterSearchDistance" unique class property ID inside the class
		static const uint32 GANGSTER_FEAR_DISTANCE;			///< "GangsterFearDistance" unique class property ID inside the class
		static const uint32 TOWCAR_DISTANCE;				///< "TowcarDistance" unique class property ID inside the class
		static const uint32 HEAVY_CRANE_DISTANCE;			///< "HeavyCraneDistance" unique class property ID inside the class
		static const uint32 DLK_DISTANCE;					///< "DlkDistance" unique class property ID inside the class


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
		explicit ActionRangeSpecsGroup(SpecsGroupManager* specsGroupManager);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ActionRangeSpecsGroup();

		inline float getCutTreeDistance() const;
		void setCutTreeDistance(float distance);

		inline float getCutHedgeDistance() const;
		void setCutHedgeDistance(float distance);

		inline float getCutCarDistance() const;
		void setCutCarDistance(float distance);

		inline float getCarryPersonDistance() const;
		void setCarryPersonDistance(float distance);

		inline float getGuideGangsterDistance() const;
		void setGuideGangsterDistance(float distance);

		inline float getPlacePersonDistance() const;
		void setPlacePersonDistance(float distance);

		inline float getTreatPersonDistance() const;
		void setTreatPersonDistance(float distance);

		inline float getEnterVehicleDistance() const;
		void setEnterVehicleDistance(float distance);

		inline float getEnterBuildingDistance() const;
		void setEnterBuildingDistance(float distance);

		inline float getPullObjectDistance() const;
		void setPullObjectDistance(float distance);

		inline float getPullPersonDistance() const;
		void setPullPersonDistance(float distance);

		inline float getRepairObjectDistance() const;
		void setRepairObjectDistance(float distance);

		inline float getTakeEquipmentDistance() const;
		void setTakeEquipmentDistance(float distance);

		inline float getExtinguishFireDistance() const;
		void setExtinguishFireDistance(float distance);

		inline float getUsePepperSprayDistance() const;
		void setUsePepperSprayDistance(float distance);

		inline float getArrestGangsterDistance() const;
		void setArrestGangsterDistance(float distance);

		inline float getGotoHydrantDistance() const;
		void setGotoHydrantDistance(float distance);

		inline float getSearchRoadVehicleDistance() const;
		void setSearchRoadVehicleDistance(float distance);

		inline float getPickPocketDistance() const;
		void setPickPocketDistance(float distance);

		inline float getRescueDogSearchDistance() const;
		void setRescueDogSearchDistance(float distance);

		inline float getStunGrenadeRadius() const;
		void setStunGrenadeRadius(float radius);

		inline float getStunGrenadeThrowDistance() const;
		void setStunGrenadeThrowDistance(float radius);

		inline float getStunGrenadeNoFollowDistance() const;
		void setStunGrenadeNoFollowDistance(float radius);

		inline float getStunGrenadeStunTime() const;
		void setStunGrenadeStunTime(float seconds);

		inline float getGangsterSearchDistance() const;
		void setGangsterSearchDistance(float distance);

		inline float getGangsterFearDistance() const;
		void setGangsterFearDistance(float distance);

		inline float getTowcarDistance() const;
		void setTowcarDistance(float distance);

		inline float getHeavyCraneDistance() const;
		void setHeavyCraneDistance(float distance);

		inline float getDlkDistance() const;
		void setDlkDistance(float distance);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system
		float mCutTreeDistance;					///< Maximal distance for the action to the tree
		float mCutHedgeDistance;				///< Maximal distance for the action to the hedge
		float mCutCarDistance;					///< Maximal distance for the action to the vehicle
		float mCarryPersonDistance;				///< Maximal distance for the action to carry the person
		float mGuideGangsterDistance;			///< Maximal distance for the action to guide a gangster
		float mPlacePersonDistance;				///< Maximal distance for the action to place the person
		float mTreatPersonDistance;				///< Maximal distance for the action to treat the person
		float mEnterVehicleDistance;			///< Maximal distance for the action to insert a vehicle
		float mEnterBuildingDistance;			///< Maximal distance for the action to insert a building
		float mPullObjectDistance;				///< Maximal distance for the action to pull a object
		float mPullPersonDistance;				///< Maximal distance for the action to pull a person
		float mRepairObjectDistance;			///< Maximal distance for the action to repair a object
		float mTakeEquipmentDistance;			///< Maximal distance for the action to take equipment
		float mExtinguishFireDistance;			///< Maximal distance for the action to extinguish fire
		float mUsePepperSprayDistance;			///< Maximal distance for the action to use pepper spray
		float mArrestGangsterDistance;			///< Maximal distance for the action to arrest a gangster
		float mGotoHydrantDistance;				///< Maximal distance for the action to connect/disconnect a hydrant
		float mSearchRoadVehicleDistance;		///< Maximal search radius for road vehicles
		float mPickPocketDistance;				///< Maximal distance for the action to pick pockets
		float mRescueDogSearchDistance;			///< Maximal distance for the rescue dog search
		float mStunGrenadeRadius;				///< Range affected by stun grenade
		float mStunGrenadeThrowDistance;		///< Maximal distance to throw a grenade
		float mStunGrenadeNoFollowDistance;		///< Maximal distance until unit gives up following a vehicle
		float mStunGrenadeStunTime;				///< Amount of seconds targets stay stunned
		float mGangsterSearchDistance;			///< Distance in which gangsters search for targets
		float mGangsterFearDistance;			///< Distance in which civil persons fear gangsters
		float mTowCarRange;						///< Maximal distance for the action to towcar
		float mHeavyCraneRange;					///< Maximal distance for heavy crane
		float mDlkRange;						///< Maximal distance for dlk


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
#include "em5/specs/ActionRangeSpecsGroup-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::ActionRangeSpecsGroup)
