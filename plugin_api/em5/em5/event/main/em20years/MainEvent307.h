// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/event/main/em20years/Em20MainEventBase.h"

#include <qsf/audio/AudioProxy.h>


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
	*    EMERGENCY 20 years edition mission 07 "Train Accident"
	*/
	class MainEvent307 : public Em20MainEventBase
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class MainEventMetaclasses;		// CAMP export of editable values for Game Design


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::MainEvent307" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		MainEvent307();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MainEvent307();


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	public:
		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual void onRun() override;
		virtual bool onSuccess(EventResult& eventResult) override;
		virtual bool onFailure(EventResult& eventResult) override;

		//[-------------------------------------------------------]
		//[ Points evaluation                                     ]
		//[-------------------------------------------------------]
		virtual uint32 calculatePointGain(EventResult& eventResult) override;

		//[-------------------------------------------------------]
		//[ Callbacks from observers                              ]
		//[-------------------------------------------------------]
		virtual bool addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason) override;
		virtual void hintCallback(Observer& hintObserver) override;

		//[-------------------------------------------------------]
		//[ Timers                                                ]
		//[-------------------------------------------------------]
		virtual void onTimerTriggered(uint32 timerId) override;

		//[-------------------------------------------------------]
		//[ Miscellaneous                                         ]
		//[-------------------------------------------------------]
		virtual const qsf::Entity* getFocusEntity() override;

		//[-------------------------------------------------------]
		//[ Serialize                                             ]
		//[-------------------------------------------------------]
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual em5::MainEvent methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual void onUpdateMainEvent(const qsf::Time& timePassed) override;

		virtual uint32 getEventNumber() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void setupScene();
		void startObjectives();
		void startTrainFall();

		void setDivingSpotActive(qsf::Entity& entity, bool active);

		void onCameraFlightStopped(const qsf::MessageParameters& parameters);
		void onCutsceneEnd(const qsf::MessageParameters& parameters);
		void onHangingPersonSavedPlacedFromDLK(const qsf::MessageParameters& parameters);
		void onDivingSpotEntitySpawned(const qsf::MessageParameters& parameters);
		void onCutVehicle(const qsf::MessageParameters& parameters);
		void onEboxRepaired(const qsf::MessageParameters& parameters);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to CAMP
		qsf::Time mTimeToRescueWater01;
		qsf::Time mTimeToRescueWater02;
		qsf::Time mTimeToRescueWater03;
		qsf::Time mTimeToRescueWater04;
		qsf::Time mTimeUntilTrainFalls;
		qsf::Time mTimeUntilHint01;
		qsf::Time mEboxUseTime;

		// Internal
		struct PersonInWater
		{
			qsf::WeakPtr<qsf::Entity> mPerson;
			glm::vec3 mStartPosition;
			glm::vec3 mEndPosition;
			qsf::Time mTime;
			qsf::Time mTotalTime;
			bool mRescued = false;
			bool mFadingOut = false;
		};
		struct DivingSpot
		{
			qsf::WeakPtr<qsf::Entity> mDivingSpotEntity;
			bool mWasCleared = false;
		};

		std::vector<PersonInWater> mPersonsInWater;
		std::vector<DivingSpot> mDivingSpots;
		qsf::WeakPtr<qsf::Entity> mHangingPerson;
		qsf::WeakPtr<qsf::Entity> mTrainFall;
		qsf::WeakPtr<qsf::Entity> mTrainFallPerson;
		qsf::WeakPtr<qsf::Entity> mTrainFallDivingSpot;
		qsf::WeakPtr<qsf::Entity> mTrainFallStartCamera;
		qsf::WeakPtr<qsf::Entity> mTrainMoveSequence;
		qsf::WeakPtr<qsf::Entity> mEbox;
		std::vector<qsf::WeakPtr<qsf::Entity>> mDeathZone;
		qsf::WeakPtr<qsf::Entity> mDeathZoneSparks;
		std::vector<qsf::WeakPtr<qsf::Entity>> mDeathZonePersonParticles;

		qsf::MessageProxy mCameraFlightStoppedMessageProxy;
		qsf::MessageProxy mCutsceneEndMessageProxy;
		qsf::MessageProxy mHangingPersonSaveProxy;
		qsf::MessageProxy mDivingSpotEntitySpawnProxy;
		qsf::MessageProxy mCutRoadVehicleProxy;
		qsf::MessageProxy mRepairEboxMessageProxy;

		bool mTrainFallPersonRescued;
		bool mDeathZoneActive;
		qsf::AudioProxy mDeathZoneAudioProxy;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::MainEvent307)
