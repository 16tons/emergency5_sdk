// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/Export.h"

#include <qsf/audio/AudioProxy.h>
#include <qsf/component/Component.h>
#include <qsf/job/JobProxy.h>
#include <qsf/debug/DebugDrawProxy.h>
#include <qsf/math/EnumBitmask.h>
#include <qsf/time/Time.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class CameraComponent;
	namespace game
	{
		class WeatherState;
		class WeatherConfiguration;
	}
}
namespace
{
	namespace detail
	{
		struct MapState;
	}
}


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
		*    Weather core component class
		*
		*  @note
		*    - Core component, see "qsf::Map::getCoreEntity()" documentation for details
		*    - High level controller
		*/
		class QSF_GAME_API_EXPORT WeatherComponent : public Component
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 COMPONENT_ID;	///< "qsf::game::WeatherComponent" unique component ID
			static const uint32 ACTIVE;			///< "Active" unique class property ID inside the class


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
			explicit WeatherComponent(Prototype* prototype);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~WeatherComponent();

			/**
			*  @brief
			*    Return current weather configuration
			*
			*  @return
			*    The current weather configuration
			*/
			const WeatherConfiguration* getCurrentWeatherConfiguration();

			/**
			*  @brief
			*    Return current weather state
			*
			*  @return
			*    The current weather state
			*/
			inline const WeatherState& getCurrentWeatherState() const;

			/**
			*  @brief
			*    Return target weather state currently fading to
			*/
			inline const WeatherState& getTargetWeatherState() const;

			/**
			*  @brief
			*    Reset to the start weather state defined in weather configuration
			*
			*  @param[in] fullRestDuration
			*    If "true", show the start weather state for its full duration, otherwise immediately start fading to the next weather state
			*  @return
			*    "true" if everything went fine, "false" if there is no weather configuation or the start weather state is invalid
			*
			*  @note
			*    - Does nothing if no weather configuration is set
			*/
			bool resetToStartWeatherState(bool fullRestDuration = true);

			/**
			*  @brief
			*    Set a new weather state
			*
			*  @param[in] weather
			*    Weather with the new weather state
			*/
			void setWeatherState(const WeatherState& weather);

			/**
			*  @brief
			*    Will fade to the next weather
			*
			*  @param[in] weather
			*    The next weather were fade to
			*  @param[in] changeDuration
			*    Time how long it will take to fade to the next weather
			*/
			void fadeToWeatherState(const WeatherState& weather, const Time& changeDuration);

			//[-------------------------------------------------------]
			//[ Properties                                            ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Return the status of the automatic transition
			*
			*  @return
			*    Returns "true" if the automatic transition is active, otherwise "false"
			*/
			inline bool getAutomaticTransition() const;

			/**
			*  @brief
			*    Set the status of the automatic transition
			*
			*  @param[in] transition
			*    If "true", activate the automatic transition, otherwise deactivate it
			*/
			inline void setAutomaticTransition(bool transition);

			/**
			*  @brief
			*    Return the current weather configuration name
			*
			*  @return
			*    Name of the weather configuration currently set
			*/
			inline const std::string& getWeatherConfigurationName() const;

			/**
			*  @brief
			*    Set the current weather configuration
			*
			*  @param[in] name
			*    Name of the weather configuration to set
			*/
			void setWeatherConfigurationByName(const std::string& name);

			/**
			*  @brief
			*    Return the name of last set weather state
			*
			*  @return
			*    Name of the weather state
			*/
			const std::string& getLastSetWeatherStateName() const;

			/**
			*  @brief
			*    Set weather state for cutscene
			*
			*  @param[in] name
			*    Name of the weather state to set
			*
			*  @note
			*    - This method should only be used as a property setter for the sequencer
			*/
			void setWeatherStateByNameForCutscenes(const std::string& name);

			//[-------------------------------------------------------]
			//[ Misc                                                  ]
			//[-------------------------------------------------------]
			void backupMapState();
			void restoreMapState();

			void updateAfterMapChanges();

			void setCameraComponent(CameraComponent* cameraComponent);

			// Debug
			void jumpToNextState();
			Time getRestFadeTime();


		//[-------------------------------------------------------]
		//[ Public virtual qsf::Component methods                 ]
		//[-------------------------------------------------------]
		public:
			virtual void serialize(qsf::BinarySerializer& serializer) override;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::Component methods              ]
		//[-------------------------------------------------------]
		protected:
			virtual void onSetSimulating(bool simulating) override;
			virtual void onSetActive(bool active) override;
			virtual void onSetDebug(bool debug) override;
			virtual bool onStartup() override;
			virtual void onShutdown() override;


		//[-------------------------------------------------------]
		//[ Protected virtual methods                             ]
		//[-------------------------------------------------------]
		protected:
			// React to weather change
			inline virtual void onWeatherChange();
			virtual void updateSimulation(const JobArguments& jobArguments);
			virtual void updateAnimation(const JobArguments& jobArguments);


		//[-------------------------------------------------------]
		//[ Private definitions                                   ]
		//[-------------------------------------------------------]
		private:
			enum InternalFlags
			{
				AUTOMATIC_TRANSITION,	///< When it's set, the weather will change automatically
				IS_SERIALIZED			///< Flag for the serialization
			};
			typedef EnumBitmask<uint8, InternalFlags> InternalFlagSet;

			typedef ::detail::MapState MapState;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void updateWeatherParticleAnimation(WeakPtr<Entity>& effectEntity, float skewAngle = 0.0f);
			void updateAnimationJobRegistration();
			void updateLightingAnimation(qsf::Time timeElapsed);

			void applyCurrentWeather();

			void backupCurrentMapState(MapState& mapState);
			void applyMapState(const MapState& mapState);
			void applyWeatherParticleEffect(const MapState& mapState, WeakPtr<Entity>& effectEntity, float effectIntensity, const StringHash& prefabLocalAssetId);
			void applyWeatherAudioEffect(float rainIntensity, float stormIntensity);
			void applyWeatherAudioEffectIntensity(AudioProxy& audioProxy, float intensity);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			// Connected to CAMP
			std::string mWeatherConfigurationName;

			// Internal
			const WeatherConfiguration* mWeatherConfiguration;	///< Weather configuration instance; can be a null pointer, do not destroy the instance
			WeatherState*				mCurrentWeather;		///< Weather that is currently set, can be a null pointer, we're responsible for destroying the instance
			WeatherState*				mTargetWeather;			///< Target weather state; can be a null pointer, we're responsible for destroying the instance
			Time						mRestFadeTime;			///< Time until the fading to the next weather has stopped
			Time						mFadeInterval;			///< Fading step on each update simulation
			Time						mRestDuration;			///< Time for the duration to fade to the next weather state
			uint32						mCurrentWeatherId;		///< ID of the current weather state
			InternalFlagSet				mInternalFlags;			///< Set of internal flags, see enum "InternalFlags"

			// Map state
			bool	  mOriginalMapStateInitialized;
			MapState& mOriginalMapState;	///< We're responsible for destroying the instance in case we no longer need it
			MapState& mCurrentMapState;		///< We're responsible for destroying the instance in case we no longer need it

			// Job proxies for regular update
			JobProxy mSimulationJobProxy;	///< Regular simulation job proxy; for updates once a game tick
			JobProxy mAnimationJobProxy;	///< Regular animation job proxy; for updates once a frame

			// Debug
			DebugDrawProxy mDebugDrawProxy;

			// Rain and storm particle effects
			WeakPtr<Entity>				 mRainEntity;
			WeakPtr<Entity>				 mStormEntity;
			WeakPtr<Entity>				 mSnowEntity;
			WeakPtr<Entity>				 mAshFallEntity;
			WeakPtr<CameraComponent>	 mCameraComponent;
			std::vector<WeakPtr<Entity>> mLightingEntities;
			Time						 mTimeUntilNextFlash;

			// Rain and storm audio effects
			AudioProxy mRainLightAudioProxy;
			AudioProxy mRainHeavyAudioProxy;
			AudioProxy mStormAudioProxy;


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
#include "qsf_game/environment/weather/WeatherComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::game::WeatherComponent)
