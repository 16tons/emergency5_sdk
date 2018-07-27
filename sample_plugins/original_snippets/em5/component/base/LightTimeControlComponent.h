// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/component/Component.h>
#include <qsf/reflection/type/CampQsfAssetProxy.h>
#include <qsf/time/Time.h>


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
	*    EMERGENCY 5 light time control component class
	*/
	class EM5_API_EXPORT LightTimeControlComponent : public qsf::Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::LightTimeControlComponent" unique component ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] prototype
		*    The prototype this component is in, no null pointer allowed
		*/
		inline explicit LightTimeControlComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~LightTimeControlComponent();

		/**
		*  @brief
		*    Return the emissive material asset to use as lights
		*
		*  @return
		*    The material asset to use (e.g. "sample/materials/spot/bat"), do no destroy the returned instance
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*    - The material asset instance is used to get material name of the emissive map of the entity which stands for the lights
		*/
		inline const qsf::AssetProxy& getLightsEmissiveMaterial() const;

		/**
		*  @brief
		*    Set the emissive material asset to use as lights
		*
		*  @param[in] material
		*    The material asset to use (e.g. "sample/material/spot/bat")
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*    - The material asset instance is used to get material name of the emissive map of the entity which stands for the sign lights
		*/
		inline void setLightsEmissiveMaterial(const qsf::AssetProxy& material);

		/**
		*  @brief
		*    Return the time when the sign lights and linked lights should be earliest switched on
		*
		*  @return
		*    The time for the earliest switched on moment
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const qsf::Time getLightsEarliestOnTime() const;

		/**
		*  @brief
		*    Set the time when the sign lights and linked lights should be earliest switched on
		*
		*  @param[in] time
		*    The time for the earliest switch on moment
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setLightsEarliestOnTime(const qsf::Time& time);

		/**
		*  @brief
		*    Return the time when the sign lights and linked lights should be latest switched on
		*
		*  @return
		*    The time for the earliest switched on moment
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const qsf::Time getLightsLatestOnTime() const;

		/**
		*  @brief
		*    Set the time when the sign lights and linked lights should be latest switched on
		*
		*  @param[in] time
		*    The time for the earliest switch on moment
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setLightsLatestOnTime(const qsf::Time& time);

		/**
		*  @brief
		*    Return the time when the sign lights and linked lights should be earliest switched off
		*
		*  @return
		*    The time for the earliest switched off moment
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const qsf::Time getLightsEarliestOffTime() const;

		/**
		*  @brief
		*    Set the time when the sign lights and linked lights should be earliest switched off
		*
		*  @param[in] time
		*    The time for the earliest switch off moment
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setLightsEarliestOffTime(const qsf::Time& time);

		/**
		*  @brief
		*    Return the time when the sign lights and linked lights should be latest switched off
		*
		*  @return
		*    The time for the earliest switched off moment
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const qsf::Time getLightsLatestOffTime() const;

		/**
		*  @brief
		*    Set the time when the sign lights and linked lights should be latest switched off
		*
		*  @param[in] time
		*    The time for the earliest switch off moment
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setLightsLatestOffTime(const qsf::Time& time);

		/**
		*  @brief
		*    Return the entity ID of the linked child light to be controlled by this instance
		*
		*  @return
		*    The entity ID of the linked child light or qsf::getUninitialized<uint64>() when not set
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline uint64 getLightEntityId() const;

		/**
		*  @brief
		*    Set the entity ID of the linked child light to be controlled by this instance
		*
		*  @param[in] entityId
		*    The entity ID of the linked child light, can be qsf::getUninitialized<uint64>()
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setLightEntityId(uint64 entityId);

		/**
		*  @brief
		*    Update current light state depending on time-of-day
		*
		*  @param[in] currentTimeOfDay
		*    The current time-of-day
		*  @param[in] forceSet
		*    If "true", all changes have to be applied immediately
		*/
		inline void updateLightState(const qsf::Time& currentTimeOfDay, bool forceSet);

		/**
		*  @brief
		*    Return "true" if this component should receive regular state update calls from the simulation
		*/
		inline bool shouldBeSimulated() const;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual bool onStartup() override;
		virtual void onSetSimulating(bool simulating) override;
		virtual void onSetActive(bool active) override;
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual em5::LightTimeControlComponent methods ]
	//[-------------------------------------------------------]
	protected:
		virtual void onUpdateLightState(const qsf::Time& currentTimeOfDay, bool forceSet);
		virtual void enableChildLights(bool enable);
		virtual void switchOffAllLights();


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		void enableEmissiveMap(const qsf::AssetProxy& material, bool enable);
		qsf::Time getTimeFromTimeRange(const qsf::Time& minimumTime, const qsf::Time& maximumTime);
		bool isTimeInOnInterval(const qsf::Time& timeToCheck, const qsf::Time& lightsOnTime, const qsf::Time& lightsOffTime);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool			mShouldBeSimulated;				///< "true" in case this component should receive regular state update calls from the simulation

		qsf::AssetProxy mLightsEmissiveMaterial;		///< The material asset to use (e.g. "sample/material/spot/bat") for lights
		qsf::Time		mLightsEarliestOnTime;			///< The time when the sign lights and linked lights should be switched on (earliest time)
		qsf::Time		mLightsLatestOnTime;			///< The time when the sign lights and linked lights should be switched on (latest time)
		qsf::Time		mLightsEarliestOffTime;			///< The time when the sign lights and linked lights should be switched off (earliest time)
		qsf::Time		mLightsLatestOffTime;			///< The time when the sign lights and linked lights should be switched off (latest time)

		qsf::Time		mLightsOnTime;					///< The time when the sign lights and linked lights should be switched on
		qsf::Time		mLightsOffTime;					///< The time when the sign lights and linked lights should be switched off

		bool			mLightsAreOnStatus;				///< Indicates if the lights and linked lights are on or off
		uint64			mLightEntityId;					///< Entity id of the light which should be controlled


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
#include "em5/component/base/LightTimeControlComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::LightTimeControlComponent)
