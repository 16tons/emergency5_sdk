// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/component/base/LightTimeControlComponent.h"


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
	*    EMERGENCY 5 building light time control component class
	*/
	class EM5_API_EXPORT BuildingLightTimeControlComponent : public LightTimeControlComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::BuildingLightTimeControlComponent" unique component ID


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
		inline explicit BuildingLightTimeControlComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~BuildingLightTimeControlComponent();

		/**
		*  @brief
		*    Return the emissive material asset to use as window lights
		*
		*  @return
		*    The material asset to use (e.g. "sample/materials/spot/bat"), do no destroy the returned instance
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*    - The material asset instance is used to get material name of the emissive map of the building entity which stands for the window lights
		*/
		inline const qsf::AssetProxy& getWindowEmissiveMaterial() const;

		/**
		*  @brief
		*    Set the emissive material asset to use as window lights
		*
		*  @param[in] material
		*    The material asset to use (e.g. "sample/material/spot/bat")
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*    - The material asset instance is used to get material name of the emissive map of the building entity which stands for the window lights
		*/
		inline void setWindowEmissiveMaterial(const qsf::AssetProxy& material);

		/**
		*  @brief
		*    Return the time when the window lights should be earliest switched on
		*
		*  @return
		*    The time for the earliest switched on moment
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const qsf::Time getWindowLightEarliestOnTime() const;

		/**
		*  @brief
		*    Set the time when the window lights should be earliest switched on
		*
		*  @param[in] time
		*    The time for the earliest switch on moment
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setWindowLightEarliestOnTime(const qsf::Time& time);

		/**
		*  @brief
		*    Return the time when the window lights should be latest switched on
		*
		*  @return
		*    The time for the earliest switched on moment
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const qsf::Time getWindowLightLatestOnTime() const;

		/**
		*  @brief
		*    Set the time when the window lights should be latest switched on
		*
		*  @param[in] time
		*    The time for the earliest switch on moment
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setWindowLightLatestOnTime(const qsf::Time& time);

		/**
		*  @brief
		*    Return the time when the window lights should be earliest switched off
		*
		*  @return
		*    The time for the earliest switched off moment
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const qsf::Time getWindowLightEarliestOffTime() const;

		/**
		*  @brief
		*    Set the time when the window lights should be earliest switched off
		*
		*  @param[in] time
		*    The time for the earliest switch off moment
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setWindowLightEarliestOffTime(const qsf::Time& time);

		/**
		*  @brief
		*    Return the time when the window lights should be latest switched off
		*
		*  @return
		*    The time for the earliest switched off moment
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const qsf::Time getWindowLightLatestOffTime() const;

		/**
		*  @brief
		*    Set the time when the window lights should be latest switched off
		*
		*  @param[in] time
		*    The time for the earliest switch off moment
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setWindowLightLatestOffTime(const qsf::Time& time);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual bool onStartup() override;
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual em5::LightTimeControlComponent methods ]
	//[-------------------------------------------------------]
	protected:
		virtual void onUpdateLightState(const qsf::Time& currentTimeOfDay, bool forceSet) override;
		virtual void enableChildLights(bool enable) override;
		virtual void switchOffAllLights() override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::AssetProxy mWindowEmissiveMaterial;		///< The material asset to use (e.g. "sample/material/spot/bat") for window lights
		qsf::Time		mWindowLightEarliestOnTime;		///< The time when the window lights should be switched on (earliest time)
		qsf::Time		mWindowLightLatestOnTime;		///< The time when the window lights should be switched on (latest time)
		qsf::Time		mWindowLightEarliestOffTime;	///< The time when the window lights should be switched off (earliest time)
		qsf::Time		mWindowLightLatestOffTime;		///< The time when the window lights should be switched off (latest time)

		qsf::Time		mWindowLightOnTime;				///< The time when the window light should be switched on
		qsf::Time		mWindowLightOffTime;			///< The time when the window light should be switched off

		bool			mWindowLightIsOnStatus;			///< Indicates of the window lights are on or off


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
#include "em5/component/building/BuildingLightTimeControlComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::BuildingLightTimeControlComponent)
