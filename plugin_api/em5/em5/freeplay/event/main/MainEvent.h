// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/event/FreeplayEvent.h"

#include <qsf/base/WeakPtr.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/reflection/type/CampGlmVec2.h>
#include <qsf/debug/DebugDrawProxy.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class GameComponent;
	class CutscenePlayer;
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
	*    EMERGENCY 5 main event base class
	*
	*  @remarks
	*    Sub-class implementations of this class represent single main event instances.
	*/
	class EM5_API_EXPORT MainEvent : public FreeplayEvent
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class MainEventMetaclasses;	// CAMP export of editable values for Game Design


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const std::string SEQUENCE_FADEOUT;	// "sequence_fadeout"


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		MainEvent(const std::string& eraseLayerName, const std::string& settingLayerName, const std::string& eventLayerName, const std::string& introCutsceneLayerName, const std::string& sceneTag,
			const std::string& introSequence, const std::string& prologueSequence);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MainEvent();

		/**
		*  @brief
		*    Gets called when either the win or loose screen has been closed
		*/
		void onFinishScreenClosed();


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	public:
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual bool onSuccess(EventResult& eventResult) override;
		virtual bool onFailure(EventResult& eventResult) override;
		virtual uint32 calculatePointGain(EventResult& eventResult) override;
		virtual void updateFreeplayEvent(const qsf::Time& timePassed) override;
		virtual bool checkEventConfiguration() override;
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual em5::MainEvent methods              ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    React to the intro start request; do only overwrite the default implementation if you really have to
		*/
		virtual void onStartIntroRequest();

		/**
		*  @brief
		*    React to the activation of the black screen, which is the start of the intro cutscene
		*/
		virtual inline void onIntroStart(const qsf::MessageParameters& parameters);

		/**
		*  @brief
		*    React to the end of the intro; note that the prologue cutscene is played next
		*/
		virtual inline void onIntroFinished(const qsf::MessageParameters& parameters);

		/**
		*  @brief
		*    React to the end of the prologue
		*/
		virtual inline void onPrologueFinished(const qsf::MessageParameters& parameters);

		/**
		*  @brief
		*    Gets called when the main event gets an update call
		*    We use here a separate update method, which the main events should be implement to support the feature "Show entity which caused the failure".
		*    This feature needs that the base class gets update calls from the freeplay system
		*/
		virtual void onUpdateMainEvent(const qsf::Time& timePassed);

		/**
		*  @brief
		*    Gets called when the main event gets an update call
		*    We use here a separate update method, which the main events should be implement to support triggering of specific music tracks.
		*/
		virtual void onUpdateMusic(const qsf::Time& timePassed);

		/**
		*  @brief
		*    Gets number of the main event (Is needed for the highscore management)
		*/
		virtual uint32 getEventNumber() = 0;

		/**
		*  @brief
		*    Return event tag of a start camera for the event; only necessary for main events without prologue cutscene
		*/
		virtual std::string getCamStartEventTag();

		/**
		*  @brief
		*    Shows the briefing supervisor message for the event
		*/
		virtual void showBriefingSupervisorMessage();

		/**
		*  @brief
		*    Shows a supervisor message when the event ends
		*/
		virtual void showSupervisorMessageForFinalEvaluation(uint32 pointGain);


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		//[-------------------------------------------------------]
		//[ Layers                                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Activate a layer
		*/
		void activateLayer(const std::string& activate, bool showError = true);

		/**
		*  @brief
		*    Deactivate a layer
		*/
		void deactivateLayer(const std::string& deactivate, bool showError = true, bool saveEntities = false);

		/**
		*  @brief
		*    Get the event layer name
		*/
		inline const std::string& getEventLayerName() const;

		/**
		*  @brief
		*    Get the erase layer name
		*/
		inline const std::string& getEraseLayerName() const;

		/**
		*  @brief
		*    Get the cut scene layer name
		*/
		inline const std::string& getCutsceneLayerName() const;

		//[-------------------------------------------------------]
		//[ Game simulation                                       ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Set player control active / inactive
		*/
		void setPlayerControl(bool playerControl) const;

		/**
		*  @brief
		*    Set damage, fire and health simulation active / inactive
		*/
		void setGameSimulation(bool gameSimulation) const;

		/**
		*  @brief
		*    Set civil cars and persons spawning active / inactive
		*/
		void setCivilistSpawn(bool spawnEnabled) const;

		/**
		*  @brief
		*    Set AI simulation
		*/
		void setAiSimulation(bool aiSimulation) const;

		//[-------------------------------------------------------]
		//[ Scene                                                 ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Get the scene tag entity
		*/
		inline qsf::Entity& getScene() const;

		/**
		*  @brief
		*    Set the current day time and weather of the event
		*/
		void setStartDayTimeAndWeather();

		//[-------------------------------------------------------]
		//[ Point gain                                            ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Add objective text / point value pair to event result
		*
		*  @return
		*    Returns zero if the objective is not existing or accomplished
		*/
		uint32 addObjectiveValue(const qsf::StringHash& objectiveId, EventResult& eventResult, uint32 pointValue) const;

		/**
		*  @brief
		*    Add objective text / point value pair to event result
		*
		*  @return
		*    Returns zero if the objective is not existing (null pointer) or not accomplished
		*/
		uint32 addObjectiveValue(const Objective* objective, EventResult& eventResult, uint32 pointValue) const;

		/**
		*  @brief
		*   Get the efficiency point gain
		*/
		uint32 getEfficiencyPointValue() const;

		//[-------------------------------------------------------]
		//[ Cutscene                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Start the intro
		*/
		void startIntro(bool forceNoFadeOut = false);

		/**
		*  @brief
		*    Get the current cutscene player
		*/
		CutscenePlayer& getCutscenePlayer() const;

		/**
		*  @brief
		*    Is moving the camera to the target entity to highlight a specific event, like a very small in-game cutscene.
		*/
		void flyCameraTo(qsf::Entity& targetCameraEntity, float maximumAcceleration = 100.0f, float maximumSpeed = 25.0f);

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Extended version of the setFinished. This method supports showing the entity which caused the failure of the event
		*
		*  @param[in] successful
		*    "true" if event was successfully finished otherwise "false"
		*  @param[in] showFailedReason
		*    "true" when on failed state the entity, which caused the failure should be shown otherwise "false" (e.g. when the event code is doing this already)
		*/
		void setMainEventFinished(bool successful, bool showFailedReason = true);

		//[-------------------------------------------------------]
		//[ Music                                                 ]
		//[-------------------------------------------------------]
		bool isNumberOfBurningObjectsBelowX(uint32 number);					///< Is the number of burning objects in the event below x
		bool personsWithHealthBelowXPercent(float health);					///< Is the health of all persons in the event below x
		bool isEntityGettingHeatDamage(const qsf::Entity& entity) const;	///< Is the specific entity getting some damage by heat or fire (not burning)
		bool isObjectiveAccomplished(uint32 typeId);						///< Check a given Objective if it is accompplished
		void setCurrentTime(const glm::vec2& currentTime);

		//[-------------------------------------------------------]
		//[ Achievement                                           ]
		//[-------------------------------------------------------]
		void triggerCampaignHeroAchievement() const;						///< The Campaign Hero achievement hase to be triggered on success of the last event in the campign


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		//[-------------------------------------------------------]
		//[ Life cycle                                            ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Is called after the intro black screen fully activated
		*/
		void introStarted(const qsf::MessageParameters& parameters);

		/**
		*  @brief
		*    Is called after the intro was finished
		*/
		void introFinished(const qsf::MessageParameters& parameters);

		/**
		*  @brief
		*    Is called after the prologue of the event finished
		*/
		void prologueFinished(const qsf::MessageParameters& parameters);

		/**
		*  @brief
		*    Do all needed cleanup for the main event (damaged person/vehicle cleanup, remove all units from map and unblock any blocked units)
		*/
		void cleanup();


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		// Connected to CAMP
		uint32				mMinEfficiencyPoints;
		uint32				mMaxEfficiencyPoints;
		qsf::Time			mMinEfficiencyTime;
		qsf::Time			mMaxEfficiencyTime;
		uint32				mNeededPointsForGold;
		uint32				mNeededPointsForSilver;

		// Internal
		qsf::MessageProxy	mLifeCycleMessageProxy;		///< Listens to messages that affect the lifecycle


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to CAMP
		glm::vec2			mStartDayTime;
		std::string			mStartWeather;
		float				mTimeOfDaySpeed;

		// Internal
		const std::string	mEventLayerName;				///< The layer the event entities are in so that they can be shown
		const std::string	mEraseLayerName;				///< The layer that must be erased
		const std::string	mSceneryLayerName;				///< The layer of the setting that is to activate
		const std::string	mIntroCutsceneLayerName;		///< The layer where the intro cut scene entities are in
		const std::string	mIntroSequence;					///< The layer where the intro cut scene entities are in
		const std::string	mPrologueSequence;				///< The layer where the intro cut scene entities are in

		qsf::Entity*		mScene;							///< The entity defining the scene area (as a "virtual object")
		GameComponent*		mGameComponent;					///< The current game component
		bool				mShowFailedReason;				///< Indicates if the failed reason should be shown
		qsf::Time			mShowFailedWindowWaitTime;		///< Wait time before the failed window is shown, is only used, when show failed reason is true

		float				mTimeOfDayOriginalDaySpeedFactor;
		float				mTimeOfDayOriginalNightSpeedFactor;


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
#include "em5/freeplay/event/main/MainEvent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::MainEvent)
