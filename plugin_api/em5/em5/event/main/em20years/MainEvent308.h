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
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class GangsterME308FactoryBoss;
}


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
	*    EMERGENCY 20 years edition mission 08 "Fireworks Factory"
	*/
	class MainEvent308 : public Em20MainEventBase
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class MainEventMetaclasses;		// CAMP export of editable values for Game Design


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::MainEvent308" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		MainEvent308();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MainEvent308();


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

		//[-------------------------------------------------------]
		//[ Regular update                                        ]
		//[-------------------------------------------------------]
		virtual void onUpdateMainEvent(const qsf::Time& timePassed) override;

		virtual uint32 getEventNumber() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void setupScene();
		void startObjectives();

		void onCameraFlightStopped(const qsf::MessageParameters& parameters);
		void onCutsceneEventKill(const qsf::MessageParameters& parameters);
		void onCutsceneEventErase(const qsf::MessageParameters& parameters);
		void onCutsceneEnd(const qsf::MessageParameters& parameters);

		void onPersonSavedPlacedFromDLK(const qsf::MessageParameters& parameters);
		void onBuriedFound(const qsf::MessageParameters& parameters);

		void triggerHouseBreakdown();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to CAMP
		qsf::Time mTimeUntilHouseBreakdown;
		qsf::Time mGangsterWaitTime;
		qsf::Time mHint01WaitTime;
		qsf::Time mHint02WaitTime;

		// Internal
		qsf::WeakPtr<qsf::Entity> mCollapsingHouse;
		qsf::WeakPtr<qsf::Entity> mCollapsingHouseCamera;
		qsf::WeakPtr<qsf::Entity> mHouseDeathZone;
		qsf::WeakPtr<qsf::Entity> mGangster;
		qsf::WeakPtr<qsf::Entity> mGangsterTarget;
		qsf::WeakPtr<GangsterME308FactoryBoss> mGangsterLogic;
		std::vector<qsf::WeakPtr<qsf::Entity>> mPersonsToFindByDog;

		qsf::MessageProxy mCameraFlightStoppedMessageProxy;
		qsf::MessageProxy mCutsceneEventKillMessageProxy;
		qsf::MessageProxy mCutsceneEventEraseMessageProxy;
		qsf::MessageProxy mCutsceneEndMessageProxy;
		qsf::MessageProxy mBalconyPersonSaveProxy;
		qsf::MessageProxy mBuriedFoundMessageProxy;

		bool mDeathZoneEvacuated;
		bool mHouseBreakdownTriggered;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::MainEvent308)