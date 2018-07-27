// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/event/main/MainEvent.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class MainEvent01KerosinePuddle;
	class ObjectiveHelper;
}
namespace qsf
{
	class ParticlesComponent;
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
	*    EMERGENCY 5 main event 01 "Munich tram accident"
	*/
	class MainEvent01 : public MainEvent
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class MainEventMetaclasses;		// CAMP export of editable values for Game Design
	friend class MainEvent01DebugGui;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::MainEvent01" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		MainEvent01();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MainEvent01();


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
		virtual void showSupervisorMessageForFinalEvaluation(uint32 pointGain) override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		enum class WAIT_STATES
		{
			NONE,
			CAMERA_REACHES_BURNING_KEROSINE,	///< Wait mode until the camera reaches a position which shows the kerosene puddle, which starts burning
			CAMERA_REACHES_EXPLODING_TRUCK		///< Wait mode until the camera reaches a position which shows the truck, which starts exploding
		};


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void gatherEventEntities();

		void startObjectives();
		void startHintObservers();
		void startAchievementObserver();

		/**
		*  @brief
		*    Triggers the sparks when timeout reached
		*/
		void checkForShowSparks(const qsf::Time& timePassed);

		/**
		*  @brief
		*    Calculates when the sparks should be triggered next (randomly between two time values)
		*/
		void setupSparksTimeout();

		/**
		*  @brief
		*    React to the rescue of the driver
		*/
		void onDriverRescue(const qsf::MessageParameters& parameters);

		/**
		*  @brief
		*    Start the fire inside the cut scene
		*/
		void startFire(const qsf::MessageParameters& parameters);

		/**
		*  @brief
		*    Reset the player controls
		*/
		void fireCutsceneFinished(const qsf::MessageParameters& parameters);

		/**
		*  @brief
		*    Hide the kerosene entity
		*
		*  @note
		*    - Callback for stop burning message from the kerosene
		*/
		void kerosineStopBurning(const qsf::MessageParameters& parameters);

		/**
		*  @brief
		*    Handle the explosion of the truck
		*/
		void startTruckExplosion();

		/**
		*  @brief
		*    Handle the end of the explosion cut scene
		*/
		void onExplosionCutsceneEnded(const qsf::MessageParameters& parameters);

		/**
		*  @brief
		*    Create the take trucker out of truck objective
		*/
		void createSaveTruckerObjective(qsf::Entity& trucker);

		/**
		*  @brief
		*    Create the heal and bring trucker to HQ objective
		*/
		void createTreatTruckerObjective(qsf::Entity& trucker);

		/**
		*  @brief
		*    Add passenger to treat passenger objective
		*/
		void addPassengerToObjective(qsf::Entity& passenger);

		/**
		*  @brief
		*    Add other injured persons to treat other persons objective
		*/
		bool addOtherInjuredPersonsToObjective(qsf::Entity& person);

		/**
		*  @brief
		*    Handle observer message of the multi message observer for the achievement
		*/
		void onAchievementMessage(const qsf::MessageParameters& parameters);

		/**
		*  @brief
		*    Handle mouse right click message (Needed for the Hint23 check)
		*/
		void onMouseRightClickMessage(const qsf::MessageParameters& parameters);

		void checkObjectivesForSubervisorMessages(const qsf::MessageParameters& parameters);

		/**
		*  @brief
		*    Check if any RTW on the map is empty (contains no injured person)
		*/
		bool anyEmptyRTWsOnMap();

		bool hasCameraStoppedAutomaticMovement();

		/**
		*  @brief
		*    Handles the delayed Hint18 showing (in case when civilians getting fire energy)
		*/
		void onShowHint18(const qsf::MessageParameters& parameters);

		/**
		*  @brief
		*    Handles the delayed Hint18 showing (in case when civilians getting fire energy)
		*/
		void onShowHint28(const qsf::MessageParameters& parameters);

		/**
		*  @brief
		*    Checks for the fail condition of too less units to win the event
		*/
		void checkForFailedConditionOfInjuredSquad(qsf::Entity& injuredSquadEntity, const ObjectiveHelper& objectiveHelper);

		//[-------------------------------------------------------]
		//[ Music                                                 ]
		//[-------------------------------------------------------]
		void musicStart();
		void musicSuccess();
		void musicFail();
		void musicStartKerosinFire();
		void musicStartLevel2();
		void musicStartLevel3();
		void musicStartActionShort();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to CAMP
		qsf::Time								mHint01WaitTime;		// 30 sec.
		qsf::Time								mHint02WaitTime;		// 60 sec.

		qsf::Time								mHint04WaitTime;		// 30 sec.
		float									mHint05LifeEnergy;		// 35%
		float									mHint06LifeEnergy;		// 10%
		qsf::Time								mHint07WaitTime;		// 90 sec.
		qsf::Time								mHint08WaitTime;		// 20 sec.
		float									mHint09LifeEnergy;		// 25%
		qsf::Time								mHint10WaitTime;		// 30 sec.

		qsf::Time								mHint12WaitTime;		// 20 sec.
		qsf::Time								mHint13WaitTime;		// 35 sec.
		qsf::Time								mHint14WaitTime;		// 60 sec.
		qsf::Time								mHint15WaitTime;		// 90 sec.

		float									mHint17DamageEnergy;	// 25% Rest life (=> 75% damaged)

		qsf::Time								mHint18WaitTime;		// 180 sec.

		qsf::Time								mHint26WaitTime;		// 15 sec.
		qsf::Time								mHint27WaitTime;		// 20 sec.

		qsf::Time								mHint18DelayWaitTime;	// 10 sec.
		qsf::Time								mHint28DelayWaitTime;	// 25 sec.
		glm::vec2								mDayTimeAfterEnd;

		// Internal
		qsf::MessageProxy						mKeroisePuddleStopBurningMessageProxy;
		qsf::MessageProxy						mAchievementMessageProxy;				///< Message proxy to gather the information for the achievement (Rescued the 4 initial injured persons in a given time)
		qsf::MessageProxy						mHint23MessageProxy;					///< Message proxy to handle EM5_MOUSE_RIGHT_CLICK message for the hint 23 check
		qsf::MessageProxy						mObjectiveChangedMessageProxy;			///< Message proxy to handle changes in objectives by this event(Needed for the supervisor objective comments)
		qsf::Entity*							mTruck;									///< The entity of the truck
		qsf::Entity*							mTrucker;								///< The entity of the trucker
		std::vector<qsf::Entity*>				mPassengers;							///< The entities of the passengers
		qsf::Entity*							mKerosine;								///< The entity of the kerosene
		MainEvent01KerosinePuddle*				mKerosinePuddle;						///< A controller for the kerosene puddle animation
		qsf::Time								mTimeForNextSparks;						///< Timeout when the sparks should be triggered
		std::vector<qsf::ParticlesComponent*>	mSparkParticles;						///< A list of the spark particles which gets triggered randomly, The pointer should stay valid while event is running

		bool									mObjective05Succeeded;					///< Indicates the current state of the objective 06 (rescue other injured persons). This is needed because a supervisor message should be triggered when this objectives gets "reopened"

		qsf::Time								mAchievementTimeout;
		WAIT_STATES								mCurrentWaitState;						///< Holds the current wait state, which gets handled inside the periodic update method to "wait" for something has ended, which doesn't have a callback for this
		bool									mStartedTruckExplosionCutscene;
		uint32									mObserverIdForHint18_CivilianGetsHeatEnergy;

		qsf::MessageProxy						mDelayHint18ShowingTimerMessageProxy;
		qsf::MessageProxy						mDelayHint28ShowingTimerMessageProxy;

		// Music
		bool mIsHealthLowForTheMusic;			///< first time the life energy of a person is lower than 10 this is true, if all persons life energy are after this again above 40 its been set to false. This flag is used to trigger action music

		// Ending
		bool									mPlayingWinFadeOut;

		boost::container::flat_set<uint32>		mAlreadyPlayedObjectiveSupervisorMessages;	///< Contains a list of objective supervisor messages, which were already played


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::MainEvent01)
