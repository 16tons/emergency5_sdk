// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/event/main/em20years/Em20MainEventBase.h"


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
	*    EMERGENCY 20 years edition mission 03 "Hurricane"
	*/
	class MainEvent303 : public Em20MainEventBase
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class MainEventMetaclasses;		// CAMP export of editable values for Game Design


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::MainEvent303" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		MainEvent303();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MainEvent303();


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
		virtual void onIntroFinished(const qsf::MessageParameters& parameters) override;
		virtual void onUpdateMainEvent(const qsf::Time& timePassed) override;
		virtual uint32 getEventNumber() override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		enum class Cutscene
		{
			NONE,
			CAR_DROP,
			FIRE_02,	// There is no FIRE_01 any more, BOOM is the replacement
			BOOM
		};


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void setupScene();
		void startObjectives();

		void startCameraFlight(Cutscene cutscene);
		void startSequence(Cutscene cutscene);

		void onCameraFlightStopped(const qsf::MessageParameters& parameters);
		void onCutsceneEventDivingSpot(const qsf::MessageParameters& parameters);
		void onCutsceneEventBurn(const qsf::MessageParameters& parameters);
		void onCutsceneEventDestroy(const qsf::MessageParameters& parameters);
		void onCutsceneEnd(const qsf::MessageParameters& parameters);

		void onCutVehicle(const qsf::MessageParameters& parameters);
		void onEntityLiftedByCrane(const qsf::MessageParameters& parameters);
		void onEntityPlacedByCrane(const qsf::MessageParameters& parameters);
		void onBuriedFound(const qsf::MessageParameters& parameters);
		void onDivingSpotEntitySpawned(const qsf::MessageParameters& parameters);

		void setDivingSpotActive(qsf::Entity& entity, bool active);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to CAMP
		qsf::Time mTimeUntilCarDrop;
		qsf::Time mTimeUntilFire02;
		qsf::Time mTimeUntilBoom;
		qsf::Time mTimeForHint05;

		// Internal
		qsf::WeakPtr<qsf::Entity> mStartCameraCarDrop;
		qsf::WeakPtr<qsf::Entity> mStartCameraFire02;
		qsf::WeakPtr<qsf::Entity> mStartCameraBoom;
		qsf::WeakPtr<qsf::Entity> mCraneBlock;
		qsf::WeakPtr<qsf::Entity> mDivingSpot;
		qsf::WeakPtr<qsf::Entity> mSpawnedSunkenCar;
		qsf::WeakPtr<qsf::Entity> mAreaEvacuate;
		std::vector<qsf::WeakPtr<qsf::Entity>> mPersonsToFindByDog;

		qsf::MessageProxy mCameraFlightStoppedMessageProxy;
		qsf::MessageProxy mCutsceneEventDivingSpotMessageProxy;
		qsf::MessageProxy mCutsceneEventBurnMessageProxy;
		qsf::MessageProxy mCutsceneEventDestroyMessageProxy;
		qsf::MessageProxy mCutsceneEndMessageProxy;
		qsf::MessageProxy mCutRoadVehicleProxy;
		qsf::MessageProxy mLiftWithCraneProxy;
		qsf::MessageProxy mPlaceByCraneProxy;
		qsf::MessageProxy mBuriedFoundMessageProxy;
		qsf::MessageProxy mDivingSpotEntitySpawnProxy;

		Cutscene mPlayingCutscene;
		std::deque<Cutscene> mCutsceneQueue;
		uint32 mCutscenesPlayed;

		struct Evacuate
		{
			qsf::WeakPtr<qsf::Entity> mEntity;
			bool mIsInsideArea = false;
		};
		std::vector<Evacuate> mPersonsToEvacuate;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::MainEvent303)
