// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/event/main/MainEvent.h"
#include "em5/event/helper/WarblerTweetHelper.h"

#include <qsf/job/JobProxy.h>
#include <qsf/base/WeakPtr.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class GangsterWithWaypointReachedCallback;
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
	*    EMERGENCY 2017 main event 204 "Schmutzige Bombe in Straﬂenbahn"
	*/
	class MainEvent204 : public MainEvent
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class MainEventMetaclasses;	// CAMP export of editable values for Game Design


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;	///< "em5::MainEvent204" unique freeplay event ID

		enum EventPhase
		{
			Phase_None,
			Phase_1,
			Phase_2,
			Phase_3_Intro,
			Phase_3
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		MainEvent204();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MainEvent204();


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
		virtual void hintCallbackWithMessage(Observer& hintObserver, const qsf::MessageParameters& parameters) override;

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

		virtual uint32 getEventNumber() override;
		virtual std::string getCamStartEventTag() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void setEventPhase(EventPhase phase);

		void initializePhase1();
		void deinitializePhase1();
		void gatherPhase1EventEntities();
		void updatePhase1();

		void initializePhase2();
		void deinitializePhase2();
		void gatherPhase2EventEntities();
		void updatePhase2();

		void playPhase3IntroSequence();
		void initializePhase3();
		void deinitializePhase3();
		void gatherPhase3EventEntities();
		void updatePhase3();

		void updateDebugMenu();

		void startObjectives();
		void startHintObservers();

		void makePersonsBuriedUnderDebris(const std::vector<qsf::WeakPtr<qsf::Entity>>& personEntities, const std::vector<qsf::WeakPtr<qsf::Entity>>& debrisEntities, uint32 rescueObjectiveTypeId);
		void makeVehicleWreck(qsf::Entity& entity);
		void makeVehicleWrecksWithPeopleInside(const std::vector<qsf::WeakPtr<qsf::Entity>>& vehicleEntities, const std::vector<qsf::WeakPtr<qsf::Entity>>& personEntities, uint32 rescueObjectiveTypeId, uint32 cutObjectiveTypeId = qsf::getUninitialized<uint32>(), bool canBeTowed = true);

		void enableBrokenOverheadLineEffect(bool state);

		void checkTriggerPhase3IntroSequence();

		void onEboxRepaired(const qsf::MessageParameters& parameters);
		void onGaperVictimInjured(const qsf::MessageParameters& parameters, int32 terroristIndex);
		void onGangsterReachedWaypoint(GangsterWithWaypointReachedCallback* gangsterLogic);

		void onPhase2TriggerGameTimerFinished(const qsf::MessageParameters& parameters);
		void onPhase2TweetTriggerGameTimerFinished(const qsf::MessageParameters& parameters);
		void onPhase3IntroSequenceFinished(const qsf::MessageParameters& parameters);
		void onPhase3DissolveCloudGameTimerFinished(const qsf::MessageParameters& parameters);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Settings, connected to camp
		std::string mGangsterLogic;
		qsf::Time mPhase2TriggerWaitTime;
		qsf::Time mPhase3BombCloudDissolveWaitTime;
		uint32 mMaxDeadPersons;
		uint32 mMaxDeadUnits;
		uint32 mMaxBurningObjects;
		uint32 mMaxNewlyContaminatedPersons;
		qsf::Time mNewsFeed3WaitTime;


		// Internal data
		EventPhase mEventPhase;

		// - Units
		std::vector<qsf::WeakPtr<qsf::Entity>> mUnitP1Burn;
		std::vector<qsf::WeakPtr<qsf::Entity>> mUnitP1Injure;
		std::vector<qsf::WeakPtr<qsf::Entity>> mUnitP1Car;
		std::vector<qsf::WeakPtr<qsf::Entity>> mUnitP1CarCut;
		std::vector<qsf::WeakPtr<qsf::Entity>> mUnitP1CarCutVictim;
		std::vector<qsf::WeakPtr<qsf::Entity>> mUnitP2Terrorist;
		std::vector<qsf::WeakPtr<qsf::Entity>> mUnitP2Victim;
		std::vector<qsf::WeakPtr<qsf::Entity>> mUnitP2Car;
		std::vector<qsf::WeakPtr<qsf::Entity>> mUnitP3Debris;
		std::vector<qsf::WeakPtr<qsf::Entity>> mUnitP3DebrisVictim;
		std::vector<qsf::WeakPtr<qsf::Entity>> mUnitP3ElectricShocker;
		std::vector<qsf::WeakPtr<qsf::Entity>> mUnitP3ElectricEbox;
		std::vector<qsf::WeakPtr<qsf::Entity>> mUnitP3DebrisSmall;
		std::vector<qsf::WeakPtr<qsf::Entity>> mUnitP3DebrisSmallVictim;
		std::vector<qsf::WeakPtr<qsf::Entity>> mUnitP3Burn;
		std::vector<qsf::WeakPtr<qsf::Entity>> mUnitP3Injure;
		std::vector<qsf::WeakPtr<qsf::Entity>> mUnitP3Car;
		std::vector<qsf::WeakPtr<qsf::Entity>> mUnitP3CarCut;
		std::vector<qsf::WeakPtr<qsf::Entity>> mUnitP3CarCutVictim;
		std::vector<qsf::WeakPtr<qsf::Entity>> mUnitP3Train;
		std::vector<qsf::WeakPtr<qsf::Entity>> mUnitP3TrainVictim;
		std::vector<qsf::WeakPtr<qsf::Entity>> mUnitP3BuryingTree;
		qsf::WeakPtr<qsf::Entity> mUnitP3BombCloud;

		// - Positions
		std::vector<qsf::WeakPtr<qsf::Entity>> mPositionP2TerroristDestination;
		std::vector<qsf::WeakPtr<qsf::Entity>> mPositionP2GaperDestination;

		// - Sounds
		qsf::WeakPtr<qsf::Entity> mSoundElectricShocker;

		// - Tweets
		qsf::WeakPtr<qsf::Entity> mTweet1UnitGaper;

		// Message proxies
		qsf::MessageProxy mEboxRepairedMessageProxy;
		qsf::MessageProxy mGameTimerMessageProxy;
		qsf::MessageProxy mSequenceFinishedMessageProxy;
		std::vector<qsf::MessageProxy> mGaperVictimInjuredMessageProxy;

		// Tweet helpers
		WarblerTweetHelper mTweet1;
		WarblerTweetHelper mTweet2;
		WarblerTweetHelper mTweet3;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::MainEvent204)
