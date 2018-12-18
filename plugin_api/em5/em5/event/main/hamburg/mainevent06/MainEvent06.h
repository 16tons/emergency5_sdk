// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/event/main/MainEvent.h"
#include "em5/event/main/hamburg/mainevent06/ME06SparkIgniter.h"


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
	*    EMERGENCY 5 main event 06 "Loading accident"
	*/
	class MainEvent06 : public MainEvent
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class MainEventMetaclasses;		// CAMP export of editable values for Game Design


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::MainEvent06" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		MainEvent06();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MainEvent06();


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

		void createSaveCraneOperatorObjective();
		void onCraneOpSaved();
		void onCraneOpPlacedFromDlk();

		void onTechHeliCalled();

		void startFinalCutscene(const std::string& cutscene);
		void endFinalCutscene();
		void checkAchievementState();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to CAMP

		float						mHint01LifeEnergy;				// 10%
		float						mHint02LifeEnergy;				// 20%
		float						mHint03LifeEnergy;				// 15%

		qsf::Time					mHint04WaitTime;				// 75 sec.
		int							mHint04Triggered;

		qsf::Time					mHint06WaitTime;				// 30 sec.

		int							mHint07FireAmount;				// 10
		float						mHint09ShipLifeEnergy;			// 20%

		qsf::Time					mHint11WaitTime;				// 30 sec.
		qsf::Time					mHint11WaitTime2;				// 60 sec.
		qsf::Time					mHint12WaitTime;				// 90 sec.
		qsf::Time					mHint12WaitTime2;				// 60 sec.
		qsf::Time					mHint13WaitTime;				// 45 sec.
		qsf::Time					mHint13WaitTime2;				// 60 sec.
		qsf::Time					mHint14WaitTime;				// 30 sec.
		qsf::Time					mHint14WaitTime2;				// 60 sec.

		qsf::Time					mHint15WaitTime;				// 20 sec.
		qsf::Time					mHint16WaitTime;				// 3 min.
		qsf::Time					mCraneOperatorDropsWaitTime;	// 6 min.
		bool						mCraneOperatorDropTimerRunning;

		// TODO(sw) Remove me, not needed anymore
		float						mHint16CraneOpLifeEnergy;	// 35%

		int							mFailConMaxObjDestByFire;	// 15

		qsf::Time					mSparkIgnitionTime01;
		qsf::Time					mSparkIgnitionTime02;
		qsf::Time					mSparkIgnitionTime03;
		ME06SparkIgniter			mIgniter01;
		ME06SparkIgniter			mIgniter02;
		ME06SparkIgniter			mIgniter03;

		bool						mHint02Triggered;

		// Internal
		std::vector<qsf::Entity*>	mDrowning;					// People who are drowning at the beginning
		std::vector<qsf::Entity*>	mFires;						// The entities of the fire places
		std::vector<qsf::Entity*>	mBurner;					// People burning ship at a certain point
		qsf::WeakPtr<qsf::Entity>	mCraneOperator;				// Part of the ship that makes people on it go over board when it starts burning
		std::vector<qsf::Entity*>	mShip1Parts;				// Parts from ship 1. If one of these is destroyed, the mission is over.
		qsf::Entity*				mShip2Tower;				// Tower of ship 2. If this catches fire, mission is over.

		qsf::JobProxy				mUpdateJobProxy;
		qsf::Time					mTimePassed;
		qsf::Time					mTimeToFall;

		qsf::MessageProxy			mTechHeliCallProxy;
		qsf::MessageProxy			mCraneOpSavedMessageProxy;

		// Ending
		bool						mPlayingWinFadeOut;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::MainEvent06)
