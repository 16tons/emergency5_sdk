// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/eventspread/EventSpreadReason.h"
#include "em5/freeplay/objective/ObjectiveList.h"
#include "em5/logic/EventHintParameters.h"
#include "em5/game/hint/HintMessage.h"

#include <qsf/base/NamedIdentifier.h>
#include <qsf/base/PropertyHelper.h>
#include <qsf/base/WeakPtr.h>
#include <qsf/math/EnumBitmask.h>
#include <qsf/message/MessageProxy.h>
#include <qsf/reflection/type/CampQsfTime.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
	class BinarySerializer;
	class Map;
	class MessageParameters;
	class DynamicMusicCompositorComponent;
	class SoundTrackManager;
}
namespace em5
{
	class EventIdComponent;
	class Observer;
	class MultiplayerDummyEvent;
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
	*    Abstract EMERGENCY 5 freeplay event base class
	*
	*  @remarks
	*    Sub-class implementations of this class represent single freeplay event instances.
	*    Do not mix up events with event factories:
	*      - Events are the actively running instances that affect the simulation
	*      - Event factories are used to check event preconditions and generate event instances
	*/
	class EM5_API_EXPORT FreeplayEvent : public qsf::Object, public qsf::WeakPtrTarget
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class FreeplayEventManager;	// May read and write event ID
	friend class EventSpreading;		// May change private data
	friend class Observer;				// May destroy observers
	friend class HintHelper;			// May use the hint data
	friend class ObjectiveHelper;		// May create objectives
	friend class ObserverHelper;		// May create observers
	friend class EventIdComponent;		// May access "insertEventId"
	friend class MultiplayerDummyEvent;	// Needs access to the mId member


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 GAME_TIMER_MESSAGE_ID;	///< "em5::FreeplayEvent_GameTimerMessage" unique message ID

		/** Event state */
		enum State
		{
			STATE_HIDDEN = 0,
			STATE_RUNNING,
			STATE_SUCCESS,
			STATE_FAILED
		};

		/** Event flags */
		enum class EventFlag
		{
			WORLD_EVENT,	///< World event
			MINI_EVENT		///< Mini-event for EM2016/2017 campaign
		};
		typedef qsf::EnumBitmask<uint8, EventFlag> EventFlagSet;

		// Shortcuts
		typedef ObjectiveList::ObjectiveInstanceMap ObjectiveInstanceMap;

		/** Event result structure */
		struct EventResult
		{
			std::string	mEventName;				///< Localizable name of the event (e.g. "EM5_EVENT_TITLE_04")
			bool		mSuccess = false;		///< Has the event succeeded

			uint32		mPointGain = 0;			///< All points the player gets

			uint32		mEfficiencyPoints = 0;	///< The points the player gets for his efficiency
			uint32		mObjectivePoints = 0;	///< The points the player gets for all objective he succeeded

			std::string	mFailedText;			///< The text, which should be shown on event failure

			boost::container::flat_map<std::string, uint32> mObjectiveNamesPoints;	///< Key: Name of the objective. Value: Point gain of the objective.

			ObjectiveList	mObjectives;		///< List of all objectives

			uint32			mPlayerIndex = 0;	///< To which player is the event assigned (in single player this value is always uninitialized)
			uint32			mEventId = 0;
		};
		typedef std::vector<uint64> EntityIdArray;

		struct ObjectiveResult
		{
			uint32	objectiveId;
			int		objectivePoints;	///< The total number of points for the objective
		};

		static const std::string MUSIC_LEVEL_LOW;
		static const std::string MUSIC_LEVEL_HIGH;

		typedef boost::container::flat_map<std::string, int> ObjectivePoints;

		struct RecordedHintSystemMessage
		{
			std::string				 hintId;
			HintMessage::MessageType messageType;
			inline RecordedHintSystemMessage(const std::string& _hintId, HintMessage::MessageType _messageType) :
				hintId(_hintId),
				messageType(_messageType)
			{};
		};
		typedef std::vector<RecordedHintSystemMessage> RecordedHintSystemMessages;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		FreeplayEvent();

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~FreeplayEvent();

		/**
		*  @brief
		*    Return freeplay event ID
		*/
		inline uint32 getId() const;

		/**
		*  @brief
		*    Return freeplay event's localizable name (e.g. "EM5_EVENT_TITLE_04")
		*/
		inline const std::string& getEventName() const;

		/**
		*  @brief
		*    Set freeplay event's localizable name (e.g. "EM5_EVENT_TITLE_04")
		*/
		inline void setEventName(const std::string& name);

		/**
		*  @brief
		*    Return original freeplay event factory's name
		*/
		inline const qsf::NamedIdentifier& getEventFactoryName() const;

		/**
		*  @brief
		*    Set original freeplay event factory's name
		*/
		inline void setEventFactoryName(const qsf::NamedIdentifier& factoryName);

		/**
		*  @brief
		*    Get event entities
		*/
		inline const EntityIdArray& getEntityIds() const;

		/**
		*  @brief
		*    Get a random event entity
		*/
		qsf::Entity* getRandomEventEntity() const;
		qsf::Entity* getRandomVisibleEventEntity() const;

		/**
		*  @brief
		*    Return the objectives list
		*/
		inline const ObjectiveList& getObjectiveList() const;
		const ObjectiveList& getObjectivesOrResultObjectives() const;

		/**
		*  @brief
		*    Returns the map this event takes place in, all entities of the event are inside this map
		*/
		inline qsf::Map& getMap();
		inline const qsf::Map& getMap() const;

		/**
		*  @brief
		*    Return the points the player get if he close this objective
		*/
		int getObjectivePointByText(const std::string& objectiveText) const;

		/**
		*  @brief
		*    Return the points for all objectives
		*
		*  @remarks
		*    For freeplay events the objectives are deleted after finishing, main event keep them
		*/
		int getObjectivePoints() const;

		/**
		*  @brief
		*    Return recorded hint system messages
		*
		*  @return
		*    Recorded hint system messages
		*/
		inline const RecordedHintSystemMessages& getRecordedHintSystemMessages() const;

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Startup the freeplay event
		*/
		bool startup();

		/**
		*  @brief
		*    Shutdown the freeplay event
		*/
		void shutdown();

		/**
		*  @brief
		*    Set event state to "running", i.e. show the event to the user now
		*/
		void setRunning();

		/**
		*  @brief
		*    Finish the event, and set it either to succeeded or failed
		*/
		void setFinished(bool successful);

		/**
		*  @brief
		*    Abort this event
		*/
		void abortEvent();

		/**
		*  @brief
		*    Set the flag which prevents winning the event even though all objectives succeeded
		*
		*  @remarks
		*    Make use of this flag if the event has points where all objectives can succeed but the event
		*    is not actually over yet. E.g. you can have all objectives finished, wait some time and then add
		*    new objectives to the player which then need to be achieved, too.
		*    Just DO NOT FORGET to set the flag back or the event will never be won!
		*/
		inline void setCanWinEvent(bool canWinEvent);

		/**
		*  @brief
		*    Access the flag which prevents winning the event even though all objectives succeeded
		*/
		inline bool getCanWinEvent() const;

		/**
		*  @brief
		*    Set the flag which prevents losing the event even though a fail condition is fulfilled
		*
		*  @see
		*    "setCanWinEvent"
		*/
		inline void setCanLoseEvent(bool canLoseEvent);

		/**
		*  @brief
		*    Access the flag which prevents losing the event even though a fail condition is fulfilled
		*/
		inline bool getCanLoseEvent() const;

		void finishObjective(Objective& objective);

		//[-------------------------------------------------------]
		//[ Update and time                                       ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Perform regular update once a game tick
		*/
		void performUpdateFreeplayEvent(const qsf::Time& timePassed);

		/**
		*  @brief
		*    Pause progress of event time
		*/
		void pauseEventTime();

		/**
		*  @brief
		*    Resume event time after it has been paused
		*/
		void resumeEventTime();

		/**
		*  @brief
		*    Get game time passed since this event was triggered, i.e. got its startup
		*/
		inline qsf::Time getCurrentLifeTime() const;

		/**
		*  @brief
		*    Get game time passed since this event was set to running state, i.e. was showed to the player
		*/
		inline qsf::Time getCurrentRunningTime() const;

		//[-------------------------------------------------------]
		//[ Point Gain                                            ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Set whether or not the normal or great point gain should occur
		*/
		inline void setGreatJob(bool greatJob);

		/**
		*  @brief
		*    Get point gain on normal success
		*/
		inline uint32 getNormalPointGain() const;

		/**
		*  @brief
		*    Set point gain on normal success
		*/
		inline void setNormalPointGain(uint32 pointGain);

		/**
		*  @brief
		*    Get credit gain on campaign success
		*/
		inline uint32 getNormalCreditGain() const;

		/**
		*  @brief
		*    Set credit gain on campaign success
		*/
		inline void setNormalCreditGain(uint32 creditGain);

		/**
		*  @brief
		*    Get point gain on multiplayer success
		*/
		inline uint32 getMultiplayerPointGain() const;

		/**
		*  @brief
		*    Set point gain on multiplayer success
		*/
		inline void setMultiplayerPointGain(uint32 pointGain);

		/**
		*  @brief
		*    Get or set the music level of the event (high or low)
		*/
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(MusicLevel, const std::string&, mMusicLevel)

		/**
		*  @brief
		*    Get or set event flags
		*/
		QSF_DEFINE_FLAG_GETTER_AND_SETTER(IsWorldEvent, mEventFlags, EventFlag::WORLD_EVENT)
		QSF_DEFINE_FLAG_GETTER_AND_SETTER(IsMiniEvent, mEventFlags, EventFlag::MINI_EVENT)

		/**
		*  @brief
		*    Set event timeout
		*/
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(Timeout, qsf::Time, mTimeout)

		//[-------------------------------------------------------]
		//[ Event density                                         ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Get the freeplay event's current state
		*/
		inline State getState() const;

		/**
		*  @brief
		*    Get event factor
		*/
		inline float getEventDensityIncrease() const;

		/**
		*  @brief
		*    Set event factor
		*/
		inline void setEventDensityIncrease(float eventDensityIncrease);

		/**
		*  @brief
		*    Get the event density
		*
		*  @return
		*    Just returns the event density if the event is hidden else returns 0.0f
		*/
		inline float getEventDensity() const;

		/**
		*  @brief
		*    Set event density
		*/
		inline void setEventDensity(float density);

		/**
		*  @brief
		*    Deserialize event properties
		*/
		bool deserializeEventFromBoostPTree(const boost::property_tree::ptree& pTree, qsf::Object::SerializationMethod serializationMethod = qsf::Object::SERIALIZE_IGNORE_UNKNOWN);

		/**
		*  @brief
		*    Returns the player index to which the event is assigned
		*/
		inline uint32 getPlayerIndex() const;

		/**
		*  @brief
		*    Sets the player index to which the event is assigned
		*/
		void setPlayerIndex(uint32 playerIndex);

		//[-------------------------------------------------------]
		//[ Event location                                        ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return "true" if the event location entity is set, else "false"
		*
		*  @remarks
		*    An event location entity is used only to mark the event's location for the "jump to event" function (e.g. button in objective window)
		*/
		inline bool isEventLocationEntitySet() const;

		qsf::Entity* getEventLocationEntity() const;

		//[-------------------------------------------------------]
		//[ Timeout                                               ]
		//[-------------------------------------------------------]
		bool isTimeout(const qsf::Time& timePassed);


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns "true" if the event has the right configuration and can trigger right now
		*/
		virtual bool checkEventConfiguration() = 0;

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    React to startup of the freeplay event
		*
		*  @note
		*    - Default implementation simply returns "true"
		*/
		inline virtual bool onStartup();

		/**
		*  @brief
		*    React to shutdown of the freeplay event
		*
		*  @note
		*    - Default implementation is empty
		*/
		inline virtual void onShutdown();

		/**
		*  @brief
		*    React to state change from (initial) hidden state to running state
		*
		*  @note
		*    - Default implementation is empty
		*/
		inline virtual void onRun();

		/**
		*  @brief
		*    React to state change from running state to success state
		*
		*  @return
		*    "true" when the freeplay system should be informed immediately, otherwise false (When false then the freeplay event is responsible to call informFreeplaySystemAboutBeFinished)
		*
		*  @note
		*    - Default implementation is empty
		*/
		inline virtual bool onSuccess(EventResult& eventResult);

		/**
		*  @brief
		*    React to state change from running state to failed state
		*
		*  @return
		*    "true" when the freeplay system should be informed immediately, otherwise false (When false then the freeplay event is responsible to call informFreeplaySystemAboutBeFinished)
		*/
		virtual bool onFailure(EventResult& eventResult);

		//[-------------------------------------------------------]
		//[ Points evaluation                                     ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Calculate the point gain for this event
		*/
		virtual uint32 calculatePointGain(EventResult& eventResult);

		//[-------------------------------------------------------]
		//[ Regular update                                        ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Regular update call, gets called once a game tick
		*/
		virtual void updateFreeplayEvent(const qsf::Time& timePassed) = 0;

		//[-------------------------------------------------------]
		//[ Callbacks from observers                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    React to adding to an entity to this event, i.e. setting the event ID for the entity
		*
		*  @param[in] targetEntity
		*    The entity just added; could already have been added before
		*  @param[in] eventSpreadReason
		*    The reason why this entity was added
		*  @param[in] newReason
		*    "true" if either the entity was not associated with this event before or if it was added with another event spread reason
		*
		*  @return
		*    "true" if the entity is added correctly to the event
		*/
		virtual bool addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason);

		/**
		*  @brief
		*    Handle the activation of a hint observer
		*/
		virtual void hintCallback(Observer& hintObserver);

		virtual void hintCallbackWithMessage(Observer& hintObserver, const qsf::MessageParameters& parameters);

		//[-------------------------------------------------------]
		//[ Timers                                                ]
		//[-------------------------------------------------------]
		inline virtual void onTimerTriggered(uint32 timerId) {}

		//[-------------------------------------------------------]
		//[ Miscellaneous                                         ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Get an event entity that is currently crucial for this event
		*    This is the entity which gets the focus when the player clicks on go to event
		*/
		virtual const qsf::Entity* getFocusEntity();

		//[-------------------------------------------------------]
		//[ Serialize                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Serialization using a binary serializer
		*/
		virtual void serialize(qsf::BinarySerializer& serializer);


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		//[-------------------------------------------------------]
		//[ Objectives                                            ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Add a new objective, i.e. win condition
		*/
		Objective& addObjective(uint32 objectiveId, uint32 orderPriority = 100);

		/**
		*  @brief
		*    Add a new fail condition (which is internally an objective as well)
		*/
		Objective& addFailCondition(uint32 objectiveId, uint32 neededNumber, const std::string& objectiveText);

		/**
		*  @brief
		*    Return the objectives list
		*/
		inline ObjectiveList& getObjectives();

		/**
		*  @brief
		*    Check the objectives states and finish the event if necessary
		*/
		void checkObjectivesState();

		//[-------------------------------------------------------]
		//[ Fail reason                                           ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Get the fail condition objectives type ID, of the event that is responsible for the failure of the event
		*/
		uint32 getFailReasonObjectiveTypeId(const ObjectiveList& objectives);

		//[-------------------------------------------------------]
		//[ Hints                                                 ]
		//[-------------------------------------------------------]
		bool wasHintShown(const std::string& hintName) const;
		void showHint(const std::string& hintName, bool notMoreThanOnce, bool textOnly = false);
		void showSupervisorMessage(const std::string& messageId, bool audioOnly = false);
		void markHintAsAlreadyShown(const std::string& hintName);

		//[-------------------------------------------------------]
		//[ Observers                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Create a new observer of the specified type ID, inside the given entity
		*/
		Observer& createObserverByTypeId(uint32 typeId, uint64 entityId, const std::string& name = std::string());

		/**
		*  @brief
		*    Create a new observer of the specified type, inside the given entity
		*/
		template<typename T>
		T& createObserver(uint64 entityId, const std::string& name = std::string());

		/**
		*  @brief
		*    Create a new general observer (i.e. not bound to a single entity, but observing the whole map) of the specified type ID
		*/
		inline Observer& createGeneralObserverByTypeId(uint32 typeId, const std::string& name = std::string());

		/**
		*  @brief
		*    Create a new general observer (i.e. not bound to a single entity, but observing the whole map) of the specified type
		*/
		template<typename T>
		T& createGeneralObserver(const std::string& name = std::string());

		/**
		*  @brief
		*    Find an existing observer of the specified type ID, inside the given entity
		*/
		Observer* getObserverByTypeId(uint32 typeId, uint64 entityId);

		/**
		*  @brief
		*    Removes and destroys the given observer
		*/
		void removeObserver(Observer& observer);

		//[-------------------------------------------------------]
		//[ Time                                                  ]
		//[-------------------------------------------------------]
		void informFreeplaySystemAboutBeFinished();

		//[-------------------------------------------------------]
		//[ Event location                                        ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Create the event location entity for use with getFocusEntity
		*/
		void createEventLocationEntity(qsf::Entity& entity);

		/**
		*  @brief
		*    Create the event location entity for use with getFocusEntity
		*/
		void createEventLocationEntity(const glm::vec3& position);

		//[-------------------------------------------------------]
		//[ Music                                                 ]
		//[-------------------------------------------------------]
		qsf::DynamicMusicCompositorComponent& getDynamicMusicCompositor() const;

		//[-------------------------------------------------------]
		//[ Special event types                                   ]
		//[-------------------------------------------------------]
		// EM2016/2017 campaign mini-event
		inline bool isMiniEvent() const;
		void setFailHintsForMiniEvent(EventResult& eventResult, const std::string& failedText, const std::string& miniEventSupervisorMessage, const std::string& standardEventHint = "");

		//[-------------------------------------------------------]
		//[ Timers                                                ]
		//[-------------------------------------------------------]
		void startTimer(uint32 timerId, qsf::Time waitTime);
		void setTimer(uint32 timerId, qsf::Time waitTime);
		void cancelTimer(uint32 timerId);
		void triggerTimer(uint32 timerId);

		//[-------------------------------------------------------]
		//[ Debug                                                 ]
		//[-------------------------------------------------------]
		bool checkDebugButton(const std::string& buttonName);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef boost::container::flat_map<uint32, Observer*> ObserverMap;
		typedef boost::container::flat_set<qsf::StringHash> HintIdSet;


	//[-------------------------------------------------------]
	//[ Private static methods                                ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Return the current simulation game time, i.e. the time passed since the current game session started
		*/
		static qsf::Time getCurrentGameTime();


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		// For friend EventIdComponent
		void insertEventId(EventIdComponent& eventIdComponent);
		bool removeEventIdComponent(EventIdComponent& eventIdComponent);
		void destroyEventIdComponent(EventIdComponent& eventIdComponent);

		// Internal
		void onObserverDestroyed(Observer& observer);
		void setObjectivePointsViaPTree(const boost::property_tree::ptree& pTree);

		/**
		*  @brief
		*    Does check if remaining squad entities are enough to accomplish all open goals
		*
		*  @remarks
		*    This can be very dynamic! TODO(oi) Someone with full knowledge about the game balancing needs to complete this method
		*/
		void checkForFailedConditionOfUnavailableUnitsToWinEvent(qsf::Entity& incapacitatedEntity, const eventspreadreason::Reason& eventSpreadReason);
		void handleWorldEventAchievements();

		void onGameTimerMessage(const qsf::MessageParameters& parameters);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32				 mId;					///< Unique freeplay event ID, not related to the event factory's ID
		std::string			 mEventName;			///< Name of this freeplay event; this is a localization ID for the displayed event name
		qsf::NamedIdentifier mEventFactoryName;		///< Unique ID and name of the event factory that triggered this event
		EventFlagSet		 mEventFlags;			///< Event flags
		State				 mState;				///< Event state
		bool				 mCanWinEvent;			///< If "false", event won't be won even if all objectives are fulfilled
		bool				 mCanLoseEvent;			///< If "false", event won't be lost even if a fail condition triggered

		ObjectiveList		 mObjectives;			///< Holds all objectives for this event
		ObserverMap			 mObservers;			///< Holds references to all observers for this event
		EntityIdArray		 mEntityIds;			///< Stores all entity IDs, that contain an event ID component of this event

		float				 mEventDensity;			///< Is the factor how much the event density would probably change if the event is triggered
		float				 mEventDensityIncrease;	///< The amount of points that the upper density limit is extended after the event has ended

		bool				 mGreatJob;				///< Is "true" if the player successes great

		uint32				 mNormalPointGain;		///< Score gain on success for the player (only in freeplay/multiplay)
		uint32				 mMultiplayerPointGain;	///< Score gain on success for the player (only in multiplayer mode)
		uint32				 mNormalCreditGain;		///< Credits gain on success for the player (only in campaign mode)

		ObjectivePoints		 mObjectivePoints;		///< Points for every single objective. Can be empty.

		EventHintParameters		   mEventHintParameters;	///< Contains all data for the hints
		HintIdSet				   mShownHintIdSet;			///< Holds the already shown hints
		RecordedHintSystemMessages mRecordedHintSystemMessages;

		qsf::Time			 mLifeTime;				///< Life time of this event, i.e. time since triggering (without paused times)
		qsf::Time			 mRunningTime;			///< Running time of this event, i.e. since switch to running state (without paused times)
		bool				 mTimeStopped;			///< If "true", there is no life and running time progress

		qsf::Map&			 mMap;					///< Map where the event takes place in
		EventResult			 mEventResult;			///< Event result

		// Event location
		qsf::WeakPtr<qsf::Entity> mEventLocationEntity;

		// Music
		std::string			 mMusicLevel;			///< The music level of the event

		// Timeout
		qsf::Time			 mTimeout;				///< Timeout for the event. If the event is running too long in hidden state, it aborts so other events can be triggered. Default value is 1000s

		// Timers
		qsf::MessageProxy	 mGameTimerMessageProxy;
		boost::container::flat_map<uint32, uint32> mGameTimerIdMap;	///< Maps timer IDs as used inside freeplay event to unique game timer IDs of the game timer manager


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
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/freeplay/event/FreeplayEvent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::FreeplayEvent)
