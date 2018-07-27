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
	*    Logic specs group implementation
	*/
	class EM5_API_EXPORT LogicSpecsGroup : public SpecsGroup, public qsf::UniqueInstance<LogicSpecsGroup>
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		// Pluginable
		static const uint32 PLUGINABLE_ID;						///< "em5::LogicSpecsGroup" unique pluginable specs group ID

		static const uint32 PARAMEDIC_SEARCH_RADIUS;			///< "ParamedicSearchRadius" unique class property ID inside the class
		static const uint32 PARAMEDICH_MINIMUM_WAITING_TIME;	///< "ParamedicMinimumWaitingTime" unique class property ID inside the class
		static const uint32 DOCTOR_SEARCH_RADIUS;				///< "DoctorSearchRadius" unique class property ID inside the class
		static const uint32 DOCTOR_MINIMUM_WAITING_TIME;		///< "DoctorMinimumWaitingTime" unique class property ID inside the class
		static const uint32 FIREMAN_SEARCH_RADIUS;				///< "FiremanSearchRadius" unique class property ID inside the class
		static const uint32 FIREMAN_MINIMUM_WAITING_TIME;		///< "FiremanMinimusWaitingTime" unique class property ID inside the class
		static const uint32 DIVER_IN_WATER_SPEED_FACTOR;		///< "DiverInWaterSpeedFactor" unique class property ID inside the class
		static const uint32 TOWCAR_SEARCH_RADIUS;				///< "TowcarSearchRadius" unique class property ID inside the class
		static const uint32 TOWCAR_MINIMUM_WAITING_TIME;		///< "TowcarMininumWaitingTime" unique class property ID inside the class
		static const uint32 DLK_SEARCH_RADIUS;					///< "DlkSearchRadius" unique class property ID inside the class
		static const uint32 DLK_MINIMUM_WAITING_TIME;			///< "DlkMininumWaitingTime" unique class property ID inside the class
		static const uint32 TLF_SEARCH_RADIUS;					///< "TlfSearchRadius" unique class property ID inside the class
		static const uint32 MOTORBOAT_MINIMUM_WAITING_TIME;		///< "MotorboatMininumWaitingTime" unique class property ID inside the class
		static const uint32 MOTORBOAT_SEARCH_RADIUS;			///< "MotorboatSearchRadius" unique class property ID inside the class
		static const uint32 FIREBOAT_SEARCH_RADIUS;				///< "FireboatSearchRadius" unique class property ID inside the class
		static const uint32 RESCUEDOG_NUMBER_FAIL_MOVES;		///< "RescueDogNumberFailMoves" unique class property ID inside the class
		static const uint32 CUTTREE_TIME;						///< "CutTreeTime" unique class property ID inside the class
		static const uint32 POLICEMAN_MINIMUM_WAITING_TIME;		///< "PolicemanMinimumWaitingTime" unique class property ID inside the class
		static const uint32 POLICE_SEARCH_VEHICLE_RADIUS;		///< "PoliceSearchVehicleRadius" unique class property ID inside the class
		static const uint32 POLICE_MEGAPHONE_RADIUS;			///< "PoliceMegaphoneRadius"
		static const uint32 POLICE_MEGAPHONE_PLACE_DIST_MIN;	///< "PoliceMegaphonePlaceDistMin"
		static const uint32 POLICE_MEGAPHONE_PLACE_DIST_MAX;	///< "PoliceMegaphonePlaceDistMax"
		static const uint32 POLICE_MEGAPHONE_MAXAFFECTED;		///< "PoliceMegaphoneMaxAffected"
		static const uint32 POLICE_MEGAPHONE_TALK_PAUSE;		///< "PoliceMegaphoneTalkPause"
		static const uint32 POLICE_HOSTAGE_TAKER_GIVEUP_CHANCE; ///< "PoliceHostageTakerGiveupChance"
		static const uint32 POLICE_BARRIER_MIN_LENGTH;			///< "PoliceBarrierMinLength"
		static const uint32 POLICE_BARRIER_MAX_LENGTH;			///< "PoliceBarrierMaxLength"
		static const uint32 POLICE_BARRIER_STOP_RADIUS;			///< "PoliceBarrierStopRadius"
		static const uint32 POLICE_BARRIER_STOP_MOVE;			///< "PoliceBarrierStopMove"
		static const uint32 POLICE_BARRIER_TRANSPARENCY;		///< "PoliceBarrierTransparency"
		static const uint32 SEK_PISTOL_SHOOT_RANGE;				///< "SEKPistolShootRange" unique class property ID inside the class
		static const uint32 SEK_GIVE_UP_VEHICLE_PURSUIT;		///< "SEKGiveUpVehiclePursuit" unique class property ID inside the class
		static const uint32 SEK_PISTOL_HEALTH_DAMAGE;			///< "SEKPistolHealthDamage" unique class property ID inside the class
		static const uint32 SEK_PISTOL_VEHICLE_DAMAGE;			///< "SEKPistolVehicleDamage" unique class property ID inside the class
		static const uint32 SEK_STAND_RANGE_MIN;				///< "SEKStandRangeMin" unique class property ID inside the class
		static const uint32 SEK_STAND_RANGE_MAX;				///< "SEKStandRangeMax" unique class property ID inside the class
		static const uint32 SEK_RIFLE_SHOOT_RANGE;				///< "SEKRifleShootRange" unique class property ID inside the class
		static const uint32 SEK_RIFLE_HEALTH_DAMAGE;			///< "SEKRifleHealthDamage" unique class property ID inside the class
		static const uint32 SEK_RIFLE_VEHICLE_DAMAGE;			///< "SEKRifleVehicleDamage" unique class property ID inside the class


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
		explicit LogicSpecsGroup(SpecsGroupManager* specsGroupManager);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~LogicSpecsGroup();

		inline float getParamedicSearchRadius() const;
		void setParamedicSearchRadius(float radius);

		inline float getParamedicMinimumWaitingTime() const;
		void setParamedicMinimumWaitingTime(float time);

		inline float getDoctorSearchRadius() const;
		void setDoctorSearchRadius(float radius);

		inline float getDoctorMinimumWaitingTime() const;
		void setDoctorMinimumWaitingTime(float time);

		inline float getFiremanSearchRadius() const;
		void setFiremanSearchRadius(float radius);

		inline float getFiremanMinimumWaitingTime() const;
		void setFiremanMinimumWaitingTime(float time);

		inline float getDiverInWaterSpeedFactor() const;
		void setDiverInWaterSpeedFactor(float factor);

		inline float getTowcarSearchRadius() const;
		void setTowcarSearchRadius(float radius);

		inline float getTowcarMinimumWaitingTime() const;
		void setTowcarMinimumWaitingTime(float time);

		inline float getDlkSearchRadius() const;
		void setDlkSearchRadius(float radius);

		inline float getDlkMinimumWaitingTime() const;
		void setDlkMinimumWaitingTime(float time);

		inline float getTlfSearchRadius() const;
		void setTlfSearchRadius(float radius);

		inline float getMotorboatMinimumWaitingTime() const;
		void setMotorboatMinimumWaitingTime(float time);

		inline float getMotorboatSearchRadius() const;
		void setMotorboatSearchRadius(float radius);

		inline float getFireboatSearchRadius() const;
		void setFireboatSearchRadius(float radius);

		inline int getRescueDogNumberFailMoves() const;
		void setRescueDogNumberFailMoves(int numberFailMoves);

		inline float getCutTreeTime() const;
		void setCutTreeTime(float cutTreeTime);

		inline float getPolicemanMinimumWaitingTime() const;
		void setPolicemanMinimumWaitingTime(float time);

		inline float getPoliceSearchVehicleRadius() const;
		void setPoliceSearchVehicleRadius(float radius);

		inline float getPoliceMegaphoneRadius() const;
		void setPoliceMegaphoneRadius(float radius);
		inline float getPoliceMegaphonePlaceDistMin() const;
		void setPoliceMegaphonePlaceDistMin(float range);
		inline float getPoliceMegaphonePlaceDistMax() const;
		void setPoliceMegaphonePlaceDistMax(float range);
		inline int getPoliceMegaphoneMaxAffected() const;
		void setPoliceMegaphoneMaxAffected(int num);
		inline float getPoliceMegaphoneTalkPause() const;
		void setPoliceMegaphoneTalkPause(float time);
		inline float getPoliceHostageTakerGiveUpChance() const;
		void setPoliceHostageTakerGiveUpChance(float chance);

		inline float getPoliceBarrierMinLength() const;
		void setPoliceBarrierMinLength(float value);
		inline float getPoliceBarrierMaxLength() const;
		void setPoliceBarrierMaxLength(float value);
		inline float getPoliceBarrierStopRadius() const;
		void setPoliceBarrierStopRadius(float value);
		inline float getPoliceBarrierStopMove() const;
		void setPoliceBarrierStopMove(float value);
		inline float getPoliceBarrierTransparency() const;
		void setPoliceBarrierTransparency(float value);

		inline float getSEKPistolShootRange() const;
		void setSEKPistolShootRange(float range);

		inline float getSEKGiveUpVehiclePursuit() const;
		void setSEKGiveUpVehiclePursuit(float range);

		inline float getSEKPistolHealthDamage() const;
		void setSEKPistolHealthDamage(float damage);

		inline float getSEKPistolVehicleDamage() const;
		void setSEKPistolVehicleDamage(float damage);

		inline float getSEKStandRangeMin() const;
		void setSEKStandRangeMin(float range);

		inline float getSEKStandRangeMax() const;
		void setSEKStandRangeMax(float range);

		inline float getSEKRifleShootRange() const;
		void setSEKRifleShootRange(float range);

		inline float getSEKRifleHealthDamage() const;
		void setSEKRifleHealthDamage(float damage);

		inline float getSEKRifleVehicleDamage() const;
		void setSEKRifleVehicleDamage(float damage);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system
		float mParamedicRadius;
		float mParamedicMinimumWaitingTime;
		float mDoctorRadius;
		float mDoctorMinimumWaitingTime;
		float mFiremanRadius;
		float mFiremanMinimumWaitingTime;
		float mDiverInWaterSpeedFactor;
		float mTowcarRadius;
		float mTowcarMinimumWaitingTime;
		float mDlkRadius;
		float mDlkMinimumWaitingTime;
		float mTlfRadius;
		float mMotorboatMinimumWaitingTime;
		float mMotorboatRadius;
		float mFireboatRadius;
		int	mRescueDogNumberFailMoves;
		float mCutTreeTime;

		float mPolicemanMinimumWaitingTime;
		float mPoliceSearchVehicleRadius;

		float mPoliceMegaphoneRadius;
		float mPoliceMegaphonePlaceDistMin;
		float mPoliceMegaphonePlaceDistMax;
		int   mPoliceMegaphoneMaxAffected;
		float mPoliceMegaphoneTalkPause;
		float mPoliceHostageTakerGiveUpChance;

		float mPoliceBarrierMinLength;
		float mPoliceBarrierMaxLength;
		float mPoliceBarrierStopRadius;
		float mPoliceBarrierStopMove;
		float mPoliceBarrierTransparency;

		float mSEKPistolShootRange;
		float mSEKGiveUpVehiclePursuit;
		float mSEKPistolHealthDamage;
		float mSEKPistolVehicleDamage;
		float mSEKStandRangeMin;
		float mSEKStandRangeMax;
		float mSEKRifleShootRange;
		float mSEKRifleHealthDamage;
		float mSEKRifleVehicleDamage;


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
#include "em5/specs/LogicSpecsGroup-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::LogicSpecsGroup)
