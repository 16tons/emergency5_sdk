// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace user
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline MedicalCheerEventFactory::AgeBitmask MedicalCheerEventFactory::getAge() const
	{
		return mAgeBitmask;
	}

	inline void MedicalCheerEventFactory::setAge(MedicalCheerEventFactory::AgeBitmask age)
	{
		mAgeBitmask = age;
	}

	inline em5::WeatherComponent::WeatherBitmask MedicalCheerEventFactory::getWeather() const
	{
		return mWeatherBitmask;
	}

	inline void MedicalCheerEventFactory::setWeather(em5::WeatherComponent::WeatherBitmask weather)
	{
		mWeatherBitmask = weather;
	}

	inline bool MedicalCheerEventFactory::getInsideBuilding() const
	{
		return mInsideBuilding;
	}

	inline void MedicalCheerEventFactory::setInsideBuilding(bool insideBuilding)
	{
		mInsideBuilding = insideBuilding;
	}

	inline const std::string& MedicalCheerEventFactory::getEventLayer() const
	{
		return mEventLayer;
	}

	inline void MedicalCheerEventFactory::setEventLayer(const std::string& eventLayer)
	{
		mEventLayer = eventLayer;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // user
