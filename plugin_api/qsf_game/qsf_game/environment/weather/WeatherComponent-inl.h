// Copyright (C) 2012-2018 Promotion Software GmbH


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
		inline const WeatherState& WeatherComponent::getCurrentWeatherState() const
		{
			return *mCurrentWeather;
		}

		inline const WeatherState& WeatherComponent::getTargetWeatherState() const
		{
			return *mTargetWeather;
		}

		inline bool WeatherComponent::getAutomaticTransition() const
		{
			return mInternalFlags.isSet(AUTOMATIC_TRANSITION);
		}

		inline void WeatherComponent::setAutomaticTransition(bool transition)
		{
			mInternalFlags.set(AUTOMATIC_TRANSITION, transition);
		}

		inline const std::string& WeatherComponent::getWeatherConfigurationName() const
		{
			return mWeatherConfigurationName;
		}

		inline void WeatherComponent::onWeatherChange()
		{
			// Nothing to do in here
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf
