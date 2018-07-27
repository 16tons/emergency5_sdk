// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/StringHash.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline WeatherSystem::WeatherSystem()
		{
			// Nothing to do in here
		}

		inline WeatherSystem::~WeatherSystem()
		{
			// Nothing to do in here
		}

		inline const WeatherState* WeatherSystem::getWeatherStateById(const StringHash& id) const
		{
			// Interpret "Default" as start weather
			return (id == qsf::StringHash("Default")) ? getDefaultWeatherState() : mWeatherStateManager.findElement(id.getHash());
		}

		inline WeatherSystem::WeatherStateManager& WeatherSystem::getWeatherStateManager()
		{
			return mWeatherStateManager;
		}

		inline WeatherSystem::WeatherConfigurationManager& WeatherSystem::getWeatherConfigurationManager()
		{
			return mWeatherConfigurationManager;
		}


		//[-------------------------------------------------------]
		//[ Public virtual qsf::System methods                    ]
		//[-------------------------------------------------------]
		inline const char* WeatherSystem::getName() const
		{
			return "Weather system";
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf
