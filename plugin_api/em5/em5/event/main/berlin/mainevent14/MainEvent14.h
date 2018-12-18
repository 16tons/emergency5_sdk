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
	*    EMERGENCY 5 main event 14 "Earthquake"
	*/
	class MainEvent14 : public MainEvent
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class MainEventMetaclasses;		// CAMP export of editable values for Game Design


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::MainEvent14" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		MainEvent14();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MainEvent14();


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	public:
		//[-------------------------------------------------------]
		//[ Life cycle                                            ]
		//[-------------------------------------------------------]
		virtual bool onStartup() override;
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
	//[ Protected virtual em5::MainEvent methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual void onIntroFinished(const qsf::MessageParameters& parameters) override;
		virtual void onPrologueFinished(const qsf::MessageParameters& parameters) override;
		virtual void onUpdateMusic(const qsf::Time& timePassed) override;
		virtual uint32 getEventNumber() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void gatherEventEntities();

		void updateBridgeWomanCountdown(const qsf::JobArguments& jobArguments);
		void onBridgeWomanSaved();
		void onBridgeWomanSavedPlacedFromDLK();
		void onBarrelExtinguishStarted(const qsf::MessageParameters& parameters);
		void onBarrelExtinguishFinished(const qsf::MessageParameters& parameters);
		void onCivilianDied(const qsf::MessageParameters& parameters);

		void startObjectives();
		void defineObjectives();
		void startHintObservers();

		void startFinalCutscene(const std::string* cutscene);
		void endFinalCutscene();

		bool isBarrelEntity(uint64 barrelEntityId) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::JobProxy				mBridgeWomanProxy;
		qsf::MessageProxy			mBridgeWomanSaveProxy;
		qsf::Entity*				mBridgeWoman;
		qsf::Entity*				mBridgeWomanJumpTarget;
		bool						mBridgeWomanSaved;
		std::vector<qsf::Entity*>	mCrashcarVictims;
		std::vector<qsf::Entity*>	mVictims;
		std::vector<qsf::Entity*>	mBurningAtStart;
		std::vector<qsf::Entity*>	mCrashcars;
		std::vector<qsf::Entity*>	mBarrels;

		qsf::MessageProxy			mDecontaminatedProxy;
		qsf::MessageProxy			mBarrelExtinguishObserver;
		qsf::MessageProxy			mBarrelExtinguishFinishedObserver;
		qsf::MessageProxy			mCivilianDiedObserver;

		// Connected to CAMP

		uint32						mMaxDeadCivilians;
		uint32						mMaxContaminatedCivils;
		uint32						mMaxInjuredSquadUnits;
		uint32						mMaxDeadSquadUnits;
		qsf::Time					mWomanFallCounter;
		qsf::Time					mWomanFallTime;

		float						mHint01LifeEnergy;
		float						mHint02LifeEnergy;
		float						mHint17LifeEnergy;
		float						mHint04LifeEnergy;
		qsf::Time					mHint08WaitTime;

		bool						mHint01Triggered;
		bool						mHint02Triggered;
		bool						mHint17Triggered;
		bool						mHint04Triggered;
		bool						mHint08Triggered;
		bool						mHint09Triggered;
		bool						mHint14Triggered;
		bool						mHint15Triggered;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::MainEvent14)
