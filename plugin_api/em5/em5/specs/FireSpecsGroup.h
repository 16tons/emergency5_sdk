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
	*    Fire specs group implementation
	*/
	class EM5_API_EXPORT FireSpecsGroup : public SpecsGroup, public qsf::UniqueInstance<FireSpecsGroup>
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		// Pluginable
		static const uint32 PLUGINABLE_ID;						///< "em5::FireSpecsGroup" unique pluginable specs group ID

		static const uint32 FULLBURN_FACTOR;					///< "FullburnFactor" unique class property ID inside the class
		static const uint32 COOLING_BONUS;						///< "CoolingBonus" unique class property ID inside the class

		static const uint32 POWER_EXTINGUISHER;					///< "PowerExtinguisher" unique class property ID inside the class
		static const uint32 POWER_HOSE;							///< "PowerHose" unique class property ID inside the class
		static const uint32 POWER_TLF;							///< "PowerTLF" unique class property ID inside the class
		static const uint32 POWER_DLK;							///< "PowerDLK" unique class property ID inside the class
		static const uint32 POWER_FIREBOAT;						///< "PowerFireboat" unique class property ID inside the class
		static const uint32 POWER_COMMANDOCAR;					///< "PowerCommandoCar" unique class property ID inside the class

		static const uint32 COOLING_POWER_EXTINGUISHER;			 ///< "CoolingPowerExtinguisher" unique class property ID inside the class
		static const uint32 COOLING_POWER_HOSE;					///< "CoolingPowerHose" unique class property ID inside the class
		static const uint32 COOLING_POWER_TLF;					///< "CoolingPowerTLF" unique class property ID inside the class
		static const uint32 COOLING_POWER_DLK;					///< "CoolingPowerDLK" unique class property ID inside the class
		static const uint32 COOLING_POWER_FIREBOAT;				///< "CoolingPowerFireboat" unique class property ID inside the class
		static const uint32 COOLING_POWER_COMMANDOCAR;			///< "CoolingCommandoCar" unique class property ID inside the class

		static const uint32 RANGE_EXTINGUISHER;					///< "RangeExtinguisher" unique class property ID inside the class
		static const uint32 RANGE_HOSE_MIN;						///< "RangeHoseMin" unique class property ID inside the class
		static const uint32 RANGE_HOSE_MAX;						///< "RangeHoseMax" unique class property ID inside the class
		static const uint32 RANGE_TLF;							///< "RangeTLF" unique class property ID inside the class
		static const uint32 RANGE_DLK;							///< "RangeDLK" unique class property ID inside the class
		static const uint32 RANGE_FIREBOAT_MIN;					///< "RangeFireboatMin" unique class property ID inside the class
		static const uint32 RANGE_FIREBOAT_MAX;					///< "RangeFireboatMax" unique class property ID inside the class

		static const uint32 RADIUS_COMMANDOCAR_EXTINGUISH;		///< "RadiusCommandoCarExtinguish" unique class property ID inside the class
		static const uint32 DURATION_COMMANDOCAR_EXTINGUISH;	///< "DurationCommandoCarExtinguish" unique class property ID inside the class
		static const uint32 COOLDOWN_COMMANDOCAR_EXTINGUISH;	///< "CooldownCommandoCarExtinguish" unique class property ID inside the class
		static const uint32 COMMANDOCAR_EXTINGUISH_EFFECT_OFFSET_POSITION;	///< "CommandoCarExtinguishEffectOffsetPosition" unique class property ID inside the class
		static const uint32 COMMANDOCAR_EXTINGUISH_EFFECT_OFFSET_TIME;	///< "CommandoCarExtinguishEffectOffsetTime" unique class property ID inside the class
		static const uint32 COMMANDOCAR_EXTINGUISH_START_DELAY;	///< "CommandoCarExtinguishStartDelay" unique class property ID inside the class

		static const uint32 RANGE_HYDRANT;						///< "RangeHydrant" unique class property ID inside the class
		static const uint32 REMOVE_SUIT_TIME;					///< "RemoveSuitTime" unique class property ID inside the class
		static const uint32 REMOVE_MASK_TIME;					///< "RemoveMaskTime" unique class property ID inside the class

		static const uint32 USE_GROUNDHYDRANTS;					///< "UseGroundHydrants" unique class property ID inside the class
		static const uint32 USE_DYNAMICHYDRANTS;				///< "UseDynamicHydrants" unique class property ID inside the class


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
		explicit FireSpecsGroup(SpecsGroupManager* specsGroupManager);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~FireSpecsGroup();

		inline float getFullburnFactor() const;
		void setFullburnFactor(float fullburnFactor);

		inline float getCoolingBonus() const;
		void setCoolingBonus(float coolingBonus);

		inline float getPowerExtinguisher() const;
		void setPowerExtinguisher(float power);

		inline float getPowerHose() const;
		void setPowerHose(float power);

		inline float getPowerTLF() const;
		void setPowerTLF(float power);

		inline float getPowerDLK() const;
		void setPowerDLK(float power);

		inline float getPowerFireboat() const;
		void setPowerFireboat(float power);

		inline float getPowerCommandoCar() const;
		void setPowerCommandoCar(float power);

		inline float getCoolingExtinguisher() const;
		void setCoolingExtinguisher(float power);

		inline float getCoolingHose() const;
		void setCoolingHose(float power);

		inline float getCoolingTLF() const;
		void setCoolingTLF(float power);

		inline float getCoolingDLK() const;
		void setCoolingDLK(float power);

		inline float getCoolingFireboat() const;
		void setCoolingFireboat(float power);

		inline float getCoolingCommandoCar() const;
		void setCoolingCommandoCar(float power);

		inline float getRangeExtinguisher() const;
		void setRangeExtinguisher(float range);

		inline float getRangeHoseMin() const;
		void setRangeHoseMin(float rangeMin);

		inline float getRangeHoseMax() const;
		void setRangeHoseMax(float rangeMax);

		inline float getRangeTLF() const;
		void setRangeTLF(float range);

		inline float getRangeDLK() const;
		void setRangeDLK(float range);

		inline float getRangeFireboatMin() const;
		void setRangeFireboatMin(float rangeMin);

		inline float getRangeFireboatMax() const;
		void setRangeFireboatMax(float rangeMax);

		inline float getRadiusCommandoCarExtinguish() const;
		void setRadiusCommandoCarExtinguish(float radius);

		inline float getDurationCommandoCarExtinguish() const;
		void setDurationCommandoCarExtinguish(float duration);

		inline float getCooldownCommandoCarExtinguish() const;
		void setCooldownCommandoCarExtinguish(float cooldownTimeInSec);

		inline float getCommandoCarExtinguishOffsetPosition() const;
		void setCommandoCarExtinguishOffsetPosition(float meter);

		inline float getCommandoCarExtinguishOffsetTime() const;
		void setCommandoCarExtinguishOffsetTime(float time);

		inline float getCommandoCarExtinguishStartDelay() const;
		void setCommandoCarExtinguishStartDelay(float time);

		inline float getRangeHydrant() const;
		void setRangeHydrant(float range);

		inline float getRemoveSuitTime() const;
		void setRemoveSuitTime(float time);

		inline float getRemoveMaskTime() const;
		void setRemoveMaskTime(float time);

		inline bool getUseGroundHydrants() const;
		void setUseGroundHydrants(bool useGroundHydrants);

		inline bool getUseDynamicHydrants() const;
		void setUseDynamicHydrants(bool useDynamicHydrants);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system
		float mFullburnFactor;					///< Fullburn factor; If fire reaches x%*100 of there maximum energy value the object starts his fullburn state (eg. FullburnFactor:0.7, mMaxEnergyValue:100 ->after fire has current energy >69, it starts his fullburn state)
		float mCoolingBonus;					///< Cooling bonus; After cooling fire component can reach a combustionValue of maximal = mCoolingBonus * firematerial->getCombustionValue()

		float mExtinguishPowerExtinguisher;		///< Extinguish power extinguisher; The power of the equipment extinguisher for extinguish fire
		float mExtinguishPowerHose;				///< Extinguish power Hose; The power of the equipment fire hose for extinguish fire
		float mExtinguishPowerTLF;				///< Extinguish power TLF; The power of the fire department vehicle TLF for extinguish fire
		float mExtinguishPowerDLK;				///< Extinguish power DLK; The power of the fire department vehicle DLK for extinguish fire
		float mExtinguishPowerFireboat;			///< Extinguish power Fireboat; The power of the fire department vehicle Fireboat for extinguish fire
		float mExtinguishPowerCommandoCar;		///< Extinguish power CommandoCar; The power of the fire department vehicle Commando Car (kf) for extinguish fire over time

		float mCoolingPowerExtinguisher;		///< Cooling power extinguisher; The power of the equipment extinguisher for cooling fire
		float mCoolingPowerHose;				///< Cooling power Hose; The power of the equipment fire hose for cooling fire
		float mCoolingPowerTLF;					///< Cooling power TLF; The power of the fire department vehicle TLF for cooling fire
		float mCoolingPowerDLK;					///< Cooling power TLF; The power of the fire department vehicle TLF for cooling fire
		float mCoolingPowerFireboat;			///< Cooling power Fireboat; The power of the fire department vehicle Fireboat for cooling fire
		float mCoolingPowerCommandoCar;			///< Cooling power CommandoCar; The power of the fire department vehicle Commando Car (kf) for cooling fire over time

		float mRangeExtinguisher;					///< Range of the equipment extinguisher
		float mRangeHoseMin;						///< Maximum range of the equipment Hose
		float mRangeHoseMax;						///< Minimum range of the equipment Hose
		float mRangeTLF;							///< Range of the TLF
		float mRangeDLK;							///< Range of the DLK
		float mRangeFireboatMin;					///< Maximum range of the Fireboat
		float mRangeFireboatMax;					///< Minimum range of the Fireboat
		float mRadiusCommandoCarExtinguish;			///< The radius of the area extinguish skill of the commando car
		float mDurationCommandoCarExtingiush;		///< The time in seconds the area extinguish skill of the commando car is taken
		float mCooldownCommandoCarExtinguish;		///< The time in seconds the area extinguish skill of the commando car needs to recharge
		float mCommandoCarExtinguishOffsetPosition;	///< The offset in meters the ground effect is spawning after each other
		float mCommandoCarExtinguishOffsetTime;		///< The offset in seconds the ground effect is spawning after each other
		float mCommandoCarExtinguishStartDelay;		///< The time in seconds the ground effect part is delayed to the air effect
		float mRangeHydrant;						///< Range of the hydrant
		float mRemoveSuitTime;						///< Time until ABC suit is removed again in safe areas
		float mRemoveMaskTime;						///< Time until Dust mask is removed again in safe areas

		bool mUseGroundHydrants;					///< "true" if hydrants created in ground
		bool mUseDynamicHydrants;					///< "true" if movable hydrants from "RW" are used


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
#include "em5/specs/FireSpecsGroup-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::FireSpecsGroup)
