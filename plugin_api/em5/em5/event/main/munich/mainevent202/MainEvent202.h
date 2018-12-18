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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    EMERGENCY 2017 main event 202 "Bombenanschlag auf dem Oktoberfest"
	*/
	class MainEvent202 : public MainEvent
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class MainEventMetaclasses;	// CAMP export of editable values for Game Design


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;	///< "em5::MainEvent202" unique freeplay event ID

		enum EventPhase
		{
			Phase_None,
			Phase_1,
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
		MainEvent202();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MainEvent202();


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

		void initializePhase2();
		void deinitializePhase2();

		void updatePhase1(const qsf::Time& timePassed);
		void updatePhase2(const qsf::Time& timePassed);
		void updateDebugMenu();

		void gatherPhase1EventEntities();
		void gatherPhase2EventEntities();
		void startObjectives();
		void startHintObservers();
		void createObjectiveFinishedObserver();

		void onPhase1EndSequenceFinished(const qsf::MessageParameters& parameters);
		void onPoliceDogFoundObject(const qsf::MessageParameters& parameters);
		void onRescueDogFoundPerson(const qsf::MessageParameters& parameters);
		void onBombExploded(const qsf::MessageParameters& parameters);

		void createDisarmBombObjective(qsf::Entity& bombEntity);
		void showBomb(uint32 bombIndex, bool show, bool fade = true);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Settings, connected to camp
		qsf::Time mPhaseOneTime;
		qsf::Time mBombTimeToDefuse;
		uint32 mNumberOfBombs;
		uint32 mMaxDeadCivilians;
		uint32 mMaxInjuredUnits;
		uint32 mMaxBurningObjects;
		qsf::Time mNewsFeed1WaitTime;

		// Internal data
		EventPhase mEventPhase;
		qsf::Time mPhaseOneTimeRemaining;
		bool mExplosionCutscenePlayed;
		bool mCanTriggerMusicLevel1;
		std::set<uint32> mUnitActiveBomb;

		// Areas
		qsf::WeakPtr<qsf::Entity> mAreaGaperTargetArea;

		// Marker
		qsf::WeakPtr<qsf::Entity> mMarkerGaperTargetArea;

		// Tweets
		qsf::WeakPtr<qsf::Entity> mTweet1Camera;

		// Units
		std::vector<qsf::WeakPtr<qsf::Entity>> mUnitBomb;
		std::vector<qsf::WeakPtr<qsf::Entity>> mUnitBombObject;
		std::vector<qsf::WeakPtr<qsf::Entity>> mUnitBombDecal;
		std::vector<qsf::WeakPtr<qsf::Entity>> mUnitPoliceDogTarget;
		std::vector<qsf::WeakPtr<qsf::Entity>> mUnitBurn;
		std::vector<qsf::WeakPtr<qsf::Entity>> mUnitInjured;
		std::vector<qsf::WeakPtr<qsf::Entity>> mUnitDebris;
		std::vector<qsf::WeakPtr<qsf::Entity>> mUnitDebrisVictim;
		std::vector<qsf::WeakPtr<qsf::Entity>> mUnitEscape;
		std::vector<qsf::WeakPtr<qsf::Entity>> mUnitTalkToPerson;
		std::vector<qsf::WeakPtr<qsf::Entity>> mUnitPersonSpawner;

		// Positions
		std::vector<qsf::WeakPtr<qsf::Entity>> mPositionEscapeTarget;

		// Messages
		qsf::MessageProxy mPoliceDogFoundObjectMessageProxy;
		qsf::MessageProxy mPoliceTalkedToPersonMessageProxy;
		qsf::MessageProxy mRescueDogFoundPersonMessageProxy;
		qsf::MessageProxy mSequenceFinishedMessageProxy;
		qsf::MessageProxy mBombExplodedMessageProxy;

		// Tweet helpers
		WarblerTweetHelper mTweet1;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::MainEvent202)
