// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/event/FreeplayEvent.h"

#include <qsf/message/MessageProxy.h>
#include <qsf/base/PropertyHelper.h>
#include <qsf/time/Time.h>


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
	*    Emergency 5 freeplay event "heat wave" 11.9
	*/
	class EM5_API_EXPORT HeatWaveEvent : public FreeplayEvent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::HeatWaveEvent" unique freeplay event ID

		struct Configuration
		{
			Configuration() :
				numberOfPersonsToInjure(3),
				numberOfVehiclesToInflame(4),
				numberOfVegetationToInflame(7),
				heatwaveFadeInTime(qsf::Time::fromSeconds(5.0f)),
				heatwaveFadeOutTime(qsf::Time::fromSeconds(5.0f)),
				heatwavePersistInjurePersonWaitTime(qsf::Time::fromSeconds(5.0f)),
				heatwavePersistInflameObjectWaitTime(qsf::Time::fromSeconds(5.0f)),
				heatwavePersistInflameVehicleWaitTime(qsf::Time::fromSeconds(5.0f)),
				deadPersonsToFail(2)
			{}

			void serialize(qsf::BinarySerializer& serializer)
			{
				serializer.serialize(numberOfPersonsToInjure);
				serializer.serialize(numberOfVehiclesToInflame);
				serializer.serialize(numberOfVegetationToInflame);
				serializer.serialize(heatwaveFadeInTime);
				serializer.serialize(heatwaveFadeOutTime);
				serializer.serialize(heatwavePersistInjurePersonWaitTime);
				serializer.serialize(heatwavePersistInflameObjectWaitTime);
				serializer.serialize(heatwavePersistInflameVehicleWaitTime);
				serializer.serialize(deadPersonsToFail);
			}

			uint32 numberOfPersonsToInjure;
			uint32 numberOfVehiclesToInflame;
			uint32 numberOfVegetationToInflame;

			qsf::Time heatwaveFadeInTime;
			qsf::Time heatwaveFadeOutTime;

			qsf::Time heatwavePersistInjurePersonWaitTime;
			qsf::Time heatwavePersistInflameObjectWaitTime;
			qsf::Time heatwavePersistInflameVehicleWaitTime;

			uint32 deadPersonsToFail;
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		HeatWaveEvent();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~HeatWaveEvent();

		/**
		*  @brief
		*    Get number persons that have to die so that the event fails
		*/
		Configuration& getConfiguration() { return mConfiguration; }


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	public:
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual void onRun() override;
		virtual bool onFailure(EventResult& eventResult) override;
		virtual void updateFreeplayEvent(const qsf::Time& timePassed) override;
		virtual bool addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason) override;
		virtual void hintCallback(Observer& hintObserver) override;
		virtual const qsf::Entity* getFocusEntity() override;
		virtual bool checkEventConfiguration() override;
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void startObjectives();
		void startHintObservers();

		bool injureARandomPerson();

		bool inflameARandomVehicle();
		qsf::Entity* findVehicleCandidate() const;
		bool checkVehicleCandidate(qsf::Entity& entity) const;

		void chooseNextPersistingState(bool fastRetry);

		bool inflameARandomObject();
		qsf::Entity* findTreeOrHedgeCandidate() const;
		void toggleCustomGlobalAmbientAIBehavior(bool enable);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		enum HeatwaveState
		{
			HEATWAVE_TURN_UP,						///< Heatwave is starting, fading in
			HEATWAVE_PERSISTING_INJURE_PERSON,		///< Heatwave running, changed AI behavior, waiting to injure a person
			HEATWAVE_PERSISTING_INFLAME_OBJECT,		///< Heatwave running, changed AI behavior, waiting to enkindle an object
			HEATWAVE_PERSISTING_INFLAME_VEHICLE,	///< Heatwave running, changed AI behavior, waiting to enkindle a vehicle
			HEATWAVE_TURN_DOWN,						///< Heatwave is fading out
			HEATWAVE_AFTERMATH						///< Waiting till the aftermath is cleaned up by the player
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Configuration mConfiguration;
		HeatwaveState mState;
		qsf::Time	  mCurrentStateTime;
		uint32		  mNumberOfPersonsInjured;
		uint32		  mNumberOfVehiclesInflamed;
		uint32		  mNumberOfObjectsInflamed;
		bool		  mTriggerForrestFire;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::HeatWaveEvent)
