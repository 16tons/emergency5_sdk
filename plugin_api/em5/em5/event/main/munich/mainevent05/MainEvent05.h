// Copyright (C) 2012-2017 Promotion Software GmbH


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
	*    EMERGENCY 5 main event 05 "Munich avalanche"
	*/
	class MainEvent05 : public MainEvent
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class MainEventMetaclasses;		// CAMP export of editable values for Game Design


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::MainEvent05" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		MainEvent05();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MainEvent05();


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
		virtual void onUpdateMainEvent(const qsf::Time& timePassed) override;
		virtual void onPrologueFinished(const qsf::MessageParameters& parameters) override;
		virtual void onUpdateMusic(const qsf::Time& timePassed) override;
		virtual uint32 getEventNumber() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void gatherEventEntities();

		void startObjectives();
		void startHintObservers();

		/**
		*  @brief
		*    Will check the condition for the gas explosion and trigger it eventually
		*/
		void checkExplosionCondition();

		/**
		*  @brief
		*    Handle the gas explosion of the hotel
		*/
		void startExplosion();

		/**
		*  @brief
		*    Handle the end of the explosion cut scene
		*/
		void onExplosionCutsceneEnded(const qsf::MessageParameters& parameters);

		/**
		*  @brief
		*    Add hint 12 after has been set down by crane
		*/
		void onCraneSetDownCar();

		/**
		*  @brief
		*    Kick hint 12 if not needed
		*/
		void onCarCutOpen();

		/**
		*  @brief
		*    Activates the intro cut scene's destructions
		*/
		void onIntroDestroyMessage(const qsf::MessageParameters& parameters);

		/**
		*  @brief
		*    Activates all tagged fire places
		*/
		void onStartFireMessage(const qsf::MessageParameters& parameters);

		/**
		*  @brief
		*    Activates the explosion cut scene's destructions
		*/
		void onExplosionDestroyMessage(const qsf::MessageParameters& parameters);

		/**
		*  @brief
		*    Create the take trucker out of truck objective
		*/
		void createExtinguishingFireObjective(qsf::Entity& trucker);

		void setSkiLiftActive(bool active);

		void triggerHint13();

		void onRescueDogFindPerson(const qsf::MessageParameters& parameters);

		void checkToStartHint07Timer();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to CAMP
		int							mFailConditionMaxFires;

		uint32						mInjuredLeftTillExplosion;  // 3

		float						mHint01LifeEnergy;	// 10%
		float						mHint02LifeEnergy;	// 20%
		float						mHint03LifeEnergy;	// 15%

		qsf::Time					mHint04WaitTime;	// 60 sec.

		qsf::Time					mHint05WaitTime;	// 20 sec.
		qsf::Time					mHint05WaitTime2;	// 40 sec.
		qsf::Time					mHint06WaitTime;	// 30 sec.
		qsf::Time					mHint07WaitTime;	// 20 sec.
		qsf::Time					mHint08WaitTime;	// 10 sec.
		qsf::Time					mHint09WaitTime;	// 40 sec.
		qsf::Time					mHint10WaitTime;	// 40 sec.

		float						mHint11LifeEnergy;	// 25%
		float						mHint12LifeEnergy;	// 15%

		bool						mOrderedTechHeli;
		bool						mRescueDogFindPerson;

		uint32						mHint13FireCount;	// 10
		bool						mHint13Triggered;
		bool						mHint13TimerStarted;
		bool						mHint13PreCondition;
		qsf::Time					mHint13WaitTime;

		uint32						mHint14FireCount;	// 15
		qsf::Time					mHint15WaitTime;	// 30 sec.
		qsf::Time					mHint16WaitTime;	// 75 sec.
		qsf::Time					mHint17WaitTime;	// 90 sec.

		bool						mHint01Triggered;
		bool						mHint02Triggered;
		bool						mHint03Triggered;
		bool						mHint12Triggered;

		// Internal
		std::vector<qsf::Entity*>	mVictim;			///< The entities of all victims
		std::vector<qsf::Entity*>	mBuriedVictims;		///< The entities of all buried victims
		std::vector<qsf::Entity*>	mGaper;				///< The entities of all gapers
		qsf::WeakPtr<qsf::Entity>	mCar;				///< The entity of all car
		qsf::Entity*				mCarVictim;			///< The entity of all car victim

		std::vector<qsf::Entity*>	mFires;				///< The entities of the fire places
		qsf::MessageProxy			mUpdateJobProxyFire;
		qsf::MessageProxy			mUpdateJobProxyDestroy;
		qsf::MessageProxy			mCraneLiftedCarMessageProxy;
		qsf::MessageProxy			mRescueDogFindPersonProxy;
		qsf::Entity*				mHotel;
		qsf::Entity*				mSkiLift01;
		qsf::Entity*				mSkiLift02;
		qsf::Entity*				mExplosionDestroyedHouse;

		uint32						mPreviousPersonsLeftForExplosion;
		bool						mWasGasExplosionTriggered;	///< Flag keeping track if the gas explosion was already triggered
		qsf::JobProxy				mGasExplosionConditionCheckJob;	///< Job which checks if the gas explosion should be triggered

		// Ending
		bool						mPlayingWinFadeOut;

		// Achievement
		qsf::Time					mAchievementTimeLimit;	///< Maximum time to unhide the buried persons, 480 sec / 8 min

		// Music
		uint32	mNumberOfBurningObjectsOnHint15;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::MainEvent05)
