// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/Export.h"

#include <qsf/base/PropertyHelper.h>
#include <qsf/component/Component.h>
#include <qsf/debug/DebugDrawProxy.h>
#include <qsf/job/JobProxy.h>
#include <qsf/reflection/type/CampGlmVec2.h>
#include <qsf/reflection/type/CampQsfAssetProxy.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace logic
	{
		class Sequence;
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
		*    Time of day component class
		*
		*  @note
		*    - Core component, see "qsf::Map::getCoreEntity()" documentation for details
		*    - High level controller
		*/
		class QSF_GAME_API_EXPORT TimeOfDayComponent : public Component
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 COMPONENT_ID;		///< "qsf::game::TimeOfDayComponent" unique component ID
			static const uint32 ACTIVE;				///< "Active" unique class property ID inside the class
			static const uint32 TIME;				///< "Time" unique class property ID inside the class
			static const uint32 SEQUENCE;			///< "Sequence" unique class property ID inside the class
			static const uint32 DAY_SPEED_FACTOR;	///< "DaySpeedFactor" unique class property ID inside the class
			static const uint32 NIGHT_SPEED_FACTOR;	///< "NightSpeedFactor" unique class property ID inside the class

			enum State
			{
				DAYTIME,
				NIGHTTIME
			};


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		public:
			static Time getCurrentTimeOfDay();


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
			inline explicit TimeOfDayComponent(Prototype* prototype);

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~TimeOfDayComponent();

			//[-------------------------------------------------------]
			//[ Current time of day                                   ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Return the current time of day
			*
			*  @return
			*    The current time, between Time::ZERO and Time::fromHours(24) for 0:00 to 24:00
			*/
			inline Time getTime() const;

			/**
			*  @brief
			*    Set the current time of day
			*
			*  @param[in] time
			*    The current time, values outside Time::ZERO and Time::fromHours(24) will be clamped
			*/
			void setTime(Time time);

			/**
			*  @brief
			*    Set the current time of day and force its immediate application
			*
			*  @param[in] time
			*    The current time, values outside Time::ZERO and Time::fromHours(24) will be clamped
			*
			*  @note
			*    - Connected to the CAMP reflection system
			*/
			void setTimeAndForceApplication(Time time);

			/**
			*  @brief
			*    Return the current time of day
			*
			*  @return
			*    The current time as glm::vec2, x = hour in [0, 23]h range, y = minute in [0, 59]m range
			*
			*  @note
			*    - Connected to the CAMP reflection system
			*/
			glm::vec2 getTimeAsGlmVec2() const;

			/**
			*  @brief
			*    Set the current time of day
			*
			*  @param[in] time
			*    The current time, x = hour in [0, 23]h range, y = minute in [0, 59]m range
			*
			*  @note
			*    - Connected to the CAMP reflection system
			*/
			void setTimeAsGlmVec2(const glm::vec2& time);

			/**
			*  @brief
			*    Get the day time hour
			*
			*  @return
			*    Hour value of the current day time, in [0, 23] range
			*/
			inline uint32 getHour() const;

			/**
			*  @brief
			*    Get the day time minute
			*
			*  @return
			*    Minute value of the current day time, in [0, 59] range
			*/
			inline uint32 getMinute() const;

			bool isTimeInInterval(Time startTime, Time endTime) const;

			//[-------------------------------------------------------]
			//[ Day / night state                                     ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Return the current state, day or night
			*
			*  @note
			*    - Connected to the CAMP reflection system
			*/
			inline State getState() const;

			inline Time getDayBegin() const;
			inline Time getDayEnd() const;

			bool isDayTime() const;

			QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(DaySpeedFactor, float, mDaySpeedFactor, DAY_SPEED_FACTOR)
			QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(NightSpeedFactor, float, mNightSpeedFactor, NIGHT_SPEED_FACTOR)

			//[-------------------------------------------------------]
			//[ Sequence                                              ]
			//[-------------------------------------------------------]
			void applyTimeOfDayWithSequence(Time time);

			inline const AssetProxy& getSequenceAsset() const;
			void setSequenceAsset(const AssetProxy& sequenceAsset);

			bool loadSequence();
			void unloadSequence();
			void evaluateSequenceAtCurrentTime();

			const logic::Sequence* getSequence() const;

			inline void setIsInSequence(bool isInSequenceUpdate);


		//[-------------------------------------------------------]
		//[ Public virtual qsf::Component methods                 ]
		//[-------------------------------------------------------]
		public:
			virtual void serialize(qsf::BinarySerializer& serializer) override;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::Component methods              ]
		//[-------------------------------------------------------]
		protected:
			virtual void onSetActive(bool active) override;
			virtual void onSetDebug(bool debug) override;

			//[-------------------------------------------------------]
			//[ Lifecycle                                             ]
			//[-------------------------------------------------------]
			virtual bool onStartup() override;
			virtual void onShutdown() override;
			virtual void onSetSimulating(bool simulating) override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Main update function that is passed to the job manager
			*
			*  @param[in] jobArguments
			*    Job arguments
			*/
			void updateJob(const JobArguments& jobArguments);

			/**
			*  @brief
			*    Apply current time
			*/
			void applyCurrentTime();

			/**
			*  @brief
			*    Internal on set active handler, which setups the update job proxy
			*/
			void internalOnSetActive(bool active);

			/**
			*  @brief
			*    Search all AmbientPolygons in the map and give them the new time (in case its changed). They react with sound changes or start particles
			*/
			void updateAmbientPolygons(Time newTime);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			// Connected to the CAMP reflection system
			Time			 mTime;						///< The current time; always stays between Time::ZERO and Time::fromHours(24) for 0:00 to 24:00
			State			 mState;					///< State of the current state
			AssetProxy		 mSequenceAsset;			///< Sequence asset proxy of the sequence defining property changes over the day

			// Internal only
			Time			 mDayBegin;					///< Begin of the day
			Time			 mDayEnd;					///< End of the day
			float			 mDaySpeedFactor;			///< Speed factor of the day time in relation to the real time
			float			 mNightSpeedFactor;			///< Speed factor of the night time in relation to the real time

			logic::Sequence* mSequence;					///< Sequence instance for evaluation
			float			 mSequenceEvaluationTime;	///< Time of last sequence evaluation, measured in the sequence's own time
			bool			 mIsInSequenceUpdate;

			JobProxy		 mUpdateJobProxy;			///< Job proxy
			DebugDrawProxy	 mDebugDrawProxy;			///< Only for debug


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
#include "qsf_game/environment/time/TimeOfDayComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::game::TimeOfDayComponent)
QSF_CAMP_TYPE_NONCOPYABLE(qsf::game::TimeOfDayComponent::State)
