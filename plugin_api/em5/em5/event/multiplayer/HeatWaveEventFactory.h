// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/factory/FreeplayEventFactory.h"
#include "em5/environment/weather/WeatherComponent.h"
#include "em5/event/multiplayer/HeatWaveEvent.h"

#include <qsf/base/PropertyHelper.h>
#include <qsf/time/Time.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
}


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
	*    Emergency 5 freeplay event factory for "heatwave" 11.9
	*/
	class EM5_API_EXPORT HeatWaveEventFactory : public FreeplayEventFactory
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		HeatWaveEventFactory();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~HeatWaveEventFactory();

		/**
		*  @brief
		*    Get weather
		*
		*  @return
		*    Get weather as string
		*/
		std::string getWeatherAsString() const;

		/**
		*  @brief
		*    Set weather bitmask
		*
		*  @param[in] weather
		*    Weather enum bitmask as string
		*/
		void setWeatherAsString(const std::string& weather);

		/**
		*  @brief
		*    Return weather bitmask
		*/
		WeatherComponent::WeatherBitmask getWeather() const;

		/**
		*  @brief
		*    Set weather bitmask
		*/
		void setWeather(WeatherComponent::WeatherBitmask weather);

		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(DeadPersonsToFail, uint32, mEventConfiguration.deadPersonsToFail);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(HeatwaveFadeInTime, qsf::Time, mEventConfiguration.heatwaveFadeInTime);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(HeatwaveFadeOutTime, qsf::Time, mEventConfiguration.heatwaveFadeOutTime);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(HeatwavePersistInflameObjectWaitTime, qsf::Time, mEventConfiguration.heatwavePersistInflameObjectWaitTime);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(HeatwavePersistInflameVehicleWaitTime, qsf::Time, mEventConfiguration.heatwavePersistInflameVehicleWaitTime);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(HeatwavePersistInjurePersonWaitTime, qsf::Time, mEventConfiguration.heatwavePersistInjurePersonWaitTime);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(NumberOfPersonsToInjure, uint32, mEventConfiguration.numberOfPersonsToInjure);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(NumberOfVegetationToInflame, uint32, mEventConfiguration.numberOfVegetationToInflame);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(NumberOfVehiclesToInflame, uint32, mEventConfiguration.numberOfVehiclesToInflame);


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	public:
		virtual FreeplayEvent* tryTriggerEvent() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		bool checkWeatherConditions() const;
		bool checkTime() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		WeatherComponent::WeatherBitmask mWeatherBitmask;	///< Bitmask for weather request
		uint32 mHourMin;
		uint32 mHourMax;
		HeatWaveEvent::Configuration mEventConfiguration;


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
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::HeatWaveEventFactory)
