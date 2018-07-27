// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/environment/weather/WeatherState.h"
#include "qsf_game/environment/weather/WeatherConfiguration.h"

#include <qsf/base/System.h>
#include <qsf/reflection/CampClass.h>
#include <qsf/base/manager/ElementManager.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Weather system
		*/
		class QSF_GAME_API_EXPORT WeatherSystem : public System
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			typedef ElementManager<uint32, WeatherState>		 WeatherStateManager;			///< Element manager containing all weather states
			typedef ElementManager<uint32, WeatherConfiguration> WeatherConfigurationManager;	///< Element manager containing all weather configurations


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			inline WeatherSystem();

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~WeatherSystem();

			//[-------------------------------------------------------]
			//[ Weather state management                              ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Get a weather state by its ID
			*
			*  @param[in] id
			*    The weather state identifier, which is the hashes weather state name
			*
			*  @return
			*    The weather state, or null pointer if none was found, do not destroy the instance
			*/
			inline const WeatherState* getWeatherStateById(const StringHash& id) const;

			const WeatherState* getDefaultWeatherState() const;

			inline WeatherStateManager& getWeatherStateManager();

			void setWeatherState(const WeatherState& weather);

			void loadWeatherStates();
			void saveWeatherStates();

			//[-------------------------------------------------------]
			//[ Weather configuration management                      ]
			//[-------------------------------------------------------]
			inline WeatherConfigurationManager& getWeatherConfigurationManager();

			void loadWeatherConfigurations();
			void saveWeatherConfigurations();

			//[-------------------------------------------------------]
			//[ Map state                                             ]
			//[-------------------------------------------------------]
			void backupMapState();
			void restoreMapState();


		//[-------------------------------------------------------]
		//[ Public virtual qsf::System methods                    ]
		//[-------------------------------------------------------]
		public:
			inline virtual const char* getName() const override;
			//[-------------------------------------------------------]
			//[ Lifecycle                                             ]
			//[-------------------------------------------------------]
			virtual bool onStartup(bool serverMode) override;
			virtual void onShutdown() override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			WeatherStateManager			mWeatherStateManager;			///< Instance of the weather state manager
			WeatherConfigurationManager mWeatherConfigurationManager;	///< Instance of the weather configuration manager


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_game/environment/weather/WeatherSystem-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::game::WeatherSystem)
