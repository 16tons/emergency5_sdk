// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/event/main/MainEvent.h"

#include <qsf/job/JobProxy.h>
#include <qsf/base/WeakPtr.h>


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
	*    EMERGENCY 5 main event 04 "Munich aircraft bomb"
	*/
	class MainEvent04 : public MainEvent
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class MainEventMetaclasses;	// CAMP export of editable values for game design


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;	///< "em5::MainEvent01" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		MainEvent04();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MainEvent04();


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	public:
		//[-------------------------------------------------------]
		//[ Life cycle                                            ]
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
		virtual void onIntroStart(const qsf::MessageParameters& parameters) override;
		virtual void onIntroFinished(const qsf::MessageParameters& parameters) override;
		virtual void onPrologueFinished(const qsf::MessageParameters& parameters) override;

		//[-------------------------------------------------------]
		//[ Regular update                                        ]
		//[-------------------------------------------------------]
		virtual void onUpdateMainEvent(const qsf::Time& timePassed) override;
		virtual void onUpdateMusic(const qsf::Time& timePassed) override;

		virtual uint32 getEventNumber() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void gatherEventEntities();

		void startObjectives();
		void startHintObservers();

		void startIntroFire();
		void startIntroDestruction();

		/**
		*  @brief
		*    React to the gasbottle explosion
		*/
		void gasBottleExplosion();

		/**
		*  @brief
		*    React to the end of the cutscene
		*/
		void onGasBottleExlosionCutsceneEnded(const qsf::MessageParameters& parameters);

		/**
		*  @brief
		*    Starts fires on the objects which should be set alight by the gas bottle explosion
		*/
		void startGasbottleFires();

		/**
		*  @brief
		*    Handle the explosion of the second bomb
		*/
		void startBombExplosion();

		/**
		*  @brief
		*    Handle the end of the explosion cut scene
		*/
		void onExplosionCutsceneEnded(const qsf::MessageParameters& parameters);

		/**
		*  @brief
		*    Create the disarm second bomb objective
		*/
		void createDisarmBombObjective(qsf::Entity& bomb);

		/**
		*  @brief
		*    Checking the condition for the gasbottle explosion sequence
		*/
		void checkGasbottleExplosionCondition(const qsf::JobArguments& jobArguments);

		/**
		*  @brief
		*    Counting down the time till the two cars explode, if not extinguished before
		*/
		void carExplosionTrigger(const qsf::JobArguments& jobArguments);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to CAMP
		qsf::Time					mBombCountDownTime;	// 240 sec.
		qsf::Time					mBombDefuseTime;	// 10 sec.
		qsf::Time					mCarExplosionTime;	// 120 sec.
		uint32						mGasExplosionFireCount; // 5
		qsf::Time					mHint01WaitTime;	// 30 sec.
		qsf::Time					mHint02WaitTime;	// 30 sec.
		qsf::Time					mHint03WaitTime;	// 100 sec.
		float						mHint04LifeEnergy;	// 10%
		int							mHint05Injuries;	// 10
		qsf::Time					mHint06WaitTime;	// 45 sec.
		qsf::Time					mHint06WaitTime2;	// 90 sec.
		qsf::Time					mHint06WaitTime3;	// 150 sec.
		qsf::Time					mHint07WaitTime;	// 10 sec.
		qsf::Time					mHint08WaitTime;	// 30 sec.
		qsf::Time					mHint09WaitTime;	// 90 sec.
		qsf::Time					mHint11WaitTime;	// 190 sec.
		qsf::Time					mHint12WaitTime;	// 90 sec.
		float						mHint13LifeEnergy;	// 20%
		qsf::Time					mHint14WaitTime;	// 20 sec.
		qsf::Time					mHint14WaitTime2;	// 105 sec.
		qsf::Time					mHint15WaitTime;	// 20 sec.
		qsf::Time					mHint16WaitTime;	// 150 sec
		int							mHint17MaxFire;		// 5
		int							mHint18MaxFire;		// 12
		int							mHint19MaxFire;		// 4
		qsf::Time					mHint19aWaitTime;	// 180s
		qsf::Time					mHint20WaitTime;	// 80 sec.

		// Internal
		std::vector<qsf::Entity*>	mWorkers;			///< The entities of the workers
		std::vector<qsf::WeakPtr<qsf::Entity>>	mCrashCars;	///< The entities of the crashed cars
		std::vector<qsf::Entity*>	mCrashCarVictims;	///< The entities of the victims inside the crashed cars
		std::vector<qsf::Entity*>	mBlocks;			///< The entities of the debris blocks
		std::vector<qsf::Entity*>	mBlockVictims;		///< The entities of the victims under the debris blocks
		qsf::Entity*				mBomb2;				///< The entity of the second bomb

		std::vector<qsf::Entity*>	mFires;				///< The entities of the fire places
		std::vector<qsf::Entity*>	mDestroyIntro;		///< The entities of the objects that can be destroyed
		std::vector<qsf::Entity*>	mFireGasBottle;		///< The entities of the fire places

		qsf::JobProxy				mUpdateJobProxyGasBottle;
		qsf::JobProxy				mUpdateJobProxyCivilCars;
		qsf::MessageProxy			mLivecycleProxyGasbottleStartFire;
		qsf::MessageProxy			mLivecycleProxyIntroDestroyObjects;
		qsf::MessageProxy			mCarStopBurningMessageProxy;
		qsf::Time					mUnitHint01TriggerCountdownLeftTime;	/// The timespan before the bomb explodes when this hint should be triggered
		qsf::Time					mTimePassedCars;

		// Ending
		bool						mPlayingWinFadeOut;
		bool						mStartedBombExplosionSequence;

		// Hint accessory
		bool						mHint01Triggered;
		bool						mHint02Triggered;
		bool						mHint04Triggered;
		bool						mHint07Triggered;
		bool						mHint14Triggered;
		bool						mHint17Triggered;
		bool						mHint18Triggered;
		bool						mHint20Triggered;
		int							mHint18Amount;
		bool						mHint19Triggered;
		int							mHint19Amount;

		int							mFailLimitDestroyedSquadVehicles;
		int							mFailLimitInjuredPersons;
		int							mFailLimitExplodedCivilCars;
		int							mFailLimitBurningBuildings;
		int							mFailLimitBurningObjects;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::MainEvent04)
