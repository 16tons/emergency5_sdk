// Copyright (C) 2012-2017 Promotion Software GmbH


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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    EMERGENCY 2017 main event 201 "Kreuzfahrschiff wird gekapert"
	*/
	class MainEvent201 : public MainEvent
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class MainEventMetaclasses;	// CAMP export of editable values for Game Design


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;	///< "em5::MainEvent201" unique freeplay event ID

		enum EventPhase
		{
			Phase_None,
			Phase_1,
			Phase_2_Intro,
			Phase_2
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		MainEvent201();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MainEvent201();


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
		EventPhase getEventPhase() const;
		void setEventPhase(EventPhase phase);

		void initializePhase1();
		void deinitializePhase1();

		void playPhase2IntroSequence();
		void initializePhase2();
		void deinitializePhase2();

		void updatePhase1();
		void updatePhase2();
		void updateDebugMenu();

		void gatherPhase1EventEntities();
		void gatherPhase2EventEntities();
		void startObjectives();
		void startHintObservers();

		void createObjectiveFinishedObserver();

		void onPersonDied(const qsf::MessageParameters& parameters);

		//TODO(ca) Remove this once eventspreadreason::DAMAGE_VEHICLE works as intended
		void onVehicleDamaged(const qsf::MessageParameters& parameters);

		void onPersonMagnetStopFollow(const qsf::MessageParameters& parameters);

		void onPhase2IntroSequenceFinished(const qsf::MessageParameters& parameters);

		void onTweet1TruckTrigger(const qsf::MessageParameters& parameters);
		void onTweet1Trigger(const qsf::MessageParameters& parameters);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Settings, connected to camp
		std::string mGangsterLogic;
		uint32 mMaxDeadCivilians;
		uint32 mMaxDrownedCivilians;
		uint32 mMaxInjuredUnits;
		uint32 mMaxDamagedVehicles;
		uint32 mMaxBurningObjects;
		qsf::Time mNewsFeed1WaitTime;
		qsf::Time mNewsFeed2WaitTime;
		qsf::Time mGaperHintWaitTime;

		// Internal data
		EventPhase mEventPhase;

		// - Units
		std::vector<qsf::WeakPtr<qsf::Entity>> mUnitGaper;
		std::vector<qsf::WeakPtr<qsf::Entity>> mUnitTruck;
		std::vector<qsf::WeakPtr<qsf::Entity>> mUnitBurn;
		std::vector<qsf::WeakPtr<qsf::Entity>> mUnitDrowning;
		std::vector<qsf::WeakPtr<qsf::Entity>> mUnitInjured;
		std::vector<qsf::WeakPtr<qsf::Entity>> mUnitInjuredShip;
		std::vector<qsf::WeakPtr<qsf::Entity>> mUnitTerrorist;

		// - Areas
		qsf::WeakPtr<qsf::Entity> mAreaGaperTargetArea;

		// - Marker
		qsf::WeakPtr<qsf::Entity> mMarkerGaperTargetArea;

		// - Tweets
		qsf::WeakPtr<qsf::Entity> mTweet1UnitGaper;
		qsf::WeakPtr<qsf::Entity> mTweet1UnitTruck;

		// Message proxies
		qsf::MessageProxy mPersonDiedMessageProxy;
		qsf::MessageProxy mPersonMagnetPreStartFollowMessageProxy;
		qsf::MessageProxy mPersonMagnetStopFollowMessageProxy;
		qsf::MessageProxy mSequenceFinishedMessageProxy;
		qsf::MessageProxy mTweetTriggerMessageProxy;
		//TODO(ca) Remove this once eventspreadreason::DAMAGE_VEHICLE works as intended
		qsf::MessageProxy mVehicleDamagedMessageProxy;

		// Tweet helpers
		WarblerTweetHelper mTweet1;
		WarblerTweetHelper mTweet2;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::MainEvent201)
