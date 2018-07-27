// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline MedicalEmergencyEventFactory::AgeBitmask MedicalEmergencyEventFactory::getAge() const
	{
		return mAgeBitmask;
	}

	inline void MedicalEmergencyEventFactory::setAge(MedicalEmergencyEventFactory::AgeBitmask age)
	{
		mAgeBitmask = age;
	}

	inline WeatherComponent::WeatherBitmask MedicalEmergencyEventFactory::getWeather() const
	{
		return mWeatherBitmask;
	}

	inline void MedicalEmergencyEventFactory::setWeather(WeatherComponent::WeatherBitmask weather)
	{
		mWeatherBitmask = weather;
	}

	inline bool MedicalEmergencyEventFactory::getInsideBuilding() const
	{
		return mInsideBuilding;
	}

	inline void MedicalEmergencyEventFactory::setInsideBuilding(bool insideBuilding)
	{
		mInsideBuilding = insideBuilding;
	}

	inline const std::string& MedicalEmergencyEventFactory::getEventLayer() const
	{
		return mEventLayer;
	}

	inline void MedicalEmergencyEventFactory::setEventLayer (const std::string& eventLayer)
	{
		mEventLayer = eventLayer;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
