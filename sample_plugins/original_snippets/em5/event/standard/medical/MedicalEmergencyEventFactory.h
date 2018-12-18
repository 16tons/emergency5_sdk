// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/factory/FreeplayEventFactory.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/environment/weather/WeatherComponent.h"


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
	*    EMERGENCY 5 freeplay event factory for "medical emergency"
	*/
	class EM5_API_EXPORT MedicalEmergencyEventFactory : public FreeplayEventFactory
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef qsf::EnumBitmask<uint8, PersonComponent::Age> AgeBitmask;


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static qsf::Entity* getRandomCandidatePerson(const AgeBitmask* ageBitmask = nullptr, bool insideBuilding = false);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		MedicalEmergencyEventFactory();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MedicalEmergencyEventFactory();

		/**
		*  @brief
		*    Get age as string
		*
		*  @return
		*    Age as string
		*/
		std::string getAgeAsString() const;

		/**
		*  @brief
		*    Set age bitmask
		*
		*  @param[in] jsonAge
		*    Age enum bitmask as string
		*/
		void setAgeAsString(const std::string& jsonAge);

		/**
		*  @brief
		*    Return age bitmask
		*/
		inline AgeBitmask getAge() const;

		/**
		*  @brief
		*    Set age bitmask
		*/
		inline void setAge(AgeBitmask age);

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
		inline WeatherComponent::WeatherBitmask getWeather() const;

		/**
		*  @brief
		*    Set weather bitmask
		*/
		inline void setWeather(WeatherComponent::WeatherBitmask weather);

		/**
		*  @brief
		*    Get inside building
		*/
		inline bool getInsideBuilding() const;

		/**
		*  @brief
		*   Set inside building
		*/
		inline void setInsideBuilding(bool insideBuilding);

		/**
		*  @brief
		*   Get event layer
		*/
		inline const std::string& getEventLayer() const;

		/**
		*  @brief
		*   Set event layer
		*/
		inline void setEventLayer(const std::string& eventLayer);


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	public:
		virtual FreeplayEvent* tryTriggerEvent() override;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		bool checkWeatherConditions() const;
		qsf::Entity* findCandidate() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		AgeBitmask						 mAgeBitmask;		///< Bitmask for age request
		WeatherComponent::WeatherBitmask mWeatherBitmask;	///< Bitmask for weather request
		bool							 mInsideBuilding;	///< Whether target must be inside a building or not -- TODO(fw): This is completely out-dated!
		std::string						 mEventLayer;


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
#include "em5/event/standard/medical/MedicalEmergencyEventFactory-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::MedicalEmergencyEventFactory)
