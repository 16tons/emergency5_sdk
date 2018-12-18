// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf_game/environment/weather/WeatherComponent.h>

#include <qsf/math/EnumBitmask.h>


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
	*    EMERGENCY 5 weather core component class
	*
	*  @note
	*    - Core component, see "qsf::Map::getCoreEntity()" documentation for details
	*    - High level controller
	*
	*  @todo
	*    - TODO(co) Connect properties to the CAMP reflection system
	*/
	class EM5_API_EXPORT WeatherComponent : public qsf::game::WeatherComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::WeatherComponent" unique component ID

		// Event weather flag definitions
		enum EventWeather
		{
			WEATHER_UNDEFINED = 0,
			WEATHER_HOT,		///< TODO(fw): Everyone likes enum value descriptions ;)
			WEATHER_COLD,
			WEATHER_CLOUDY,
			WEATHER_FOGGY,
			WEATHER_RAINY,
			WEATHER_HEAVY_RAIN,
			WEATHER_STORM,
			WEATHER_HEAVY_STORM,
			WEATHER_THUNDER,
			WEATHER_HAIL,
			WEATHER_NORMAL
		};

		// Bitmask of all of the above weather flags; allows for arbitrary combinations of the flags
		typedef qsf::EnumBitmask<uint32, EventWeather> WeatherBitmask;


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
		inline explicit WeatherComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~WeatherComponent();

		/**
		*  @brief
		*    Check whether people should use umbrellas because of rain right now
		*/
		inline bool getUseUmbrellas() const;

		/**
		*  @brief
		*    Calculate weather used in events
		*
		*  @return
		*    Calculated event weather
		*
		*  @todo
		*    TODO(jm) Set the values of the event weather
		*/
		WeatherBitmask getEventWeather() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::WeatherComponent methods       ]
	//[-------------------------------------------------------]
	protected:
		virtual void onWeatherChange() override;
		virtual void updateSimulation(const qsf::JobArguments& jobArguments) override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool mUseUmbrellas;	///< If "true", persons without further equipment should switch to umbrellas because of the rain
		float mLastUseUmbrellaValue;
		qsf::Time	mUmbrellaUpdateTimer;
		bool	mCheckUmbrellaChanges;


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
#include "em5/environment/weather/WeatherComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::WeatherComponent)
QSF_CAMP_TYPE_NONCOPYABLE(em5::WeatherComponent::EventWeather)
