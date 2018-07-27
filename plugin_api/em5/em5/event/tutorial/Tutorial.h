// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/event/FreeplayEvent.h"
#include "em5/settings/GameSettingsGroup.h"

#include <qsf/job/JobProxy.h>
#include <qsf/message/MessageProxy.h>
#include <qsf/reflection/type/CampGlmVec2.h>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class GameComponent;
	class TutorialStep;
}
namespace qsf
{
	class Entity;
	namespace game
	{
		class EventTagManagerComponent;
	}
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
	*    EMERGENCY 5 tutorial event
	*/
	class Tutorial : public FreeplayEvent
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class FreeplayEventMetaclasses2;		// CAMP export of editable values for Game Design
	friend class TutorialStepIntro;				// To access the protected methods
	friend class TutorialStep;					// To access the protected methods
	friend class TutorialStepStation1;			// To access the protected methods
	friend class TutorialStepStation2;			// To access the protected methods
	friend class TutorialStepStation3;			// To access the protected methods
	friend class TutorialStepOutro;				// To access the protected methods


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::Tutorial" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		Tutorial();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Tutorial();


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
		virtual bool checkEventConfiguration() override;

		//[-------------------------------------------------------]
		//[ Points evaluation                                     ]
		//[-------------------------------------------------------]
		virtual uint32 calculatePointGain(EventResult& eventResult) override;

		//[-------------------------------------------------------]
		//[ Regular update                                        ]
		//[-------------------------------------------------------]
		virtual void updateFreeplayEvent(const qsf::Time& timePassed) override;

		//[-------------------------------------------------------]
		//[ Callbacks from observers                              ]
		//[-------------------------------------------------------]
		virtual bool addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason) override;

		//[-------------------------------------------------------]
		//[ Miscellaneous                                         ]
		//[-------------------------------------------------------]
		virtual const qsf::Entity* getFocusEntity() override;

		//[-------------------------------------------------------]
		//[ Serialize                                             ]
		//[-------------------------------------------------------]
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
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
		*    Set AI simulation
		*/
		void setAiSimulation(bool aiSimulation) const;

		/**
		*  @brief
		*    Set civilian spawn active / inactive
		*/
		void setCivilianSpawn(bool civilianSpawn) const;

		//[-------------------------------------------------------]
		//[ Layers                                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Activate the scenery layer
		*/
		void activateLayer(const std::string& activate) const;

		/**
		*  @brief
		*    Deactivate the scenery layer
		*/
		void deactivateLayer(const std::string& deactivate, bool saveEntities = false) const;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::vector<std::unique_ptr<TutorialStep>> TutorialStepContainer;

		enum class HUD_BUTTON_ID
		{
			UNITTYPE_FIREFIGHTER,
			UNITTYPE_AMBULANCE,
			UNITTYPE_POLICE,
			UNITTYPE_TEC,
			UNIT_FIREFIGHTER_RW,
			UNIT_AMBULANCE_NEF,
			UNIT_AMBULANCE_RTW,
			UNIT_POLICE_STW,
			UNIT_TEC_ASF,
			ORDER_QUESTIONMARK,
			ORDER_SEND_UNIT,
			ORDER_BACK_TO_HQ,
			UNIT_PERSON_EXIT,
			EQUIPPED_FIREHOSE,
			EQUIPPED_RESCUECUTTER,
			EQUIPPED_PEPPERSPRAY,
			EQUIPPED_HANDCUFFS,
			OBJECTIVE_LIST_GOTO_EVENT,
		};


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		// Handler for the EM5_PRE_COMMAND_EXECUTE to deny for specific entities the move command
		void onPreCommandExecuted(const qsf::MessageParameters& parameters);

		void setStartDayTimeAndWeather();
		void enableCommandsInput(bool enable) const;
		void enableGlobalInput(bool enable) const;
		void enableZoomInput(bool enable) const;
		void enableRotateInput(bool enable) const;
		void enableSelectionInput(bool enable) const;
		void enableMultiSelectionInput(bool enable) const;
		void enableHighlightInteractionObjects(bool enable) const;
		void enableMinimapInput(bool enable) const;
		void enableLeftClickInput(bool enable) const;

		void enableUnitAutomatism(bool enable);

		void showMouseCursor(bool show) const;

		void showAllHudElements(bool show) const;
		void showUnitTypeButtons(bool show) const;
		void showMinimap(bool show) const;
		void enableAllHudButtons(bool enable) const;
		void disableAllBlinkingButtons() const;
		bool isVehicleListHudWindowOpen(const qsf::Time& timePassed) const;
		bool isVehicleOrderHudWindowOpen(const qsf::Time& timePassed) const;
		void enableButton(const HUD_BUTTON_ID buttonId, bool enable) const;
		void letHudButtonBlink(const HUD_BUTTON_ID buttonId, const bool blink) const;
		void letObjectiveFlash(const qsf::StringHash& objectiveId) const;

		void letObjectiveFlashWhenNotCompleted(uint32 objectiveId) const;

		qsf::Entity* acquireTaggedEntity(const std::string& tag);
		/**
		*  @brief
		*    Get multiple event-tagged entities needed for the main event
		*
		*  @param[out] outEntities
		*    The entities found
		*  @param[in] tag
		*    The event tag to search for
		*  @param[in] minimumNumberNeeded
		*    The minimum number of entities needed
		*
		*  @return
		*    Will return true if there have been found enough entities or false if there were some missing (a log message is provided then)
		*/
		bool tryAcquireTaggedEntities(std::vector<qsf::Entity*>& outEntities, const std::string& tag, uint32 minimumNumberNeeded);

		void changeToNextStep();

		void addEntityToDeniedMoveList(const qsf::Entity& entity);
		void removeEntityFromDeniedMoveList(const qsf::Entity& entity);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to CAMP
		glm::vec2								mStartDayTime;
		std::string								mStartWeather;

		// Internal
		qsf::game::EventTagManagerComponent*	mEventTagManager;				///< The current event tag manager
		GameComponent*							mGameComponent;					///< The current game component

		TutorialStepContainer					mTutorialSteps;
		TutorialStepContainer::iterator			mCurrentTutorialStep;
		glm::vec3								mOriginalMapBoundaryTopLeft;
		glm::vec3								mOriginalMapBoundaryBottomRight;
		std::vector<uint64>						mEntitiesToDenyMoveCommand;		///< A list of entities for which the move command execution is denied
		qsf::MessageProxy						mDenyMoveCommandMessageProxy;

		// Spec settings for GD connected to CAMP
		// General
		// Intro
		qsf::Time mIntroWaitTimeForSupervisor;
		qsf::Time mIntroWaitTimeForIntroEnd;
		// Station one
		qsf::Time mHint01WaitTime;
		qsf::Time mHint02WaitTime;
		qsf::Time mHint03WaitTime;
		qsf::Time mHint04WaitTime;
		qsf::Time mHint05WaitTime;
		qsf::Time mHint08WaitTime;
		qsf::Time mSupervisor15WaitTime;
		// Station two
		qsf::Time mScrollSlowTime;
		qsf::Time mScrollFastTime;
		qsf::Time mHint12WaitTime;
		qsf::Time mHint14WaitTime;
		qsf::Time mHint15WaitTime;
		qsf::Time mHint16WaitTime;
		qsf::Time mHint17WaitTime;
		qsf::Time mHint18WaitTime;
		qsf::Time mHint20WaitTime;
		qsf::Time mHint21WaitTime;
		// Station three
		qsf::Time mHint22WaitTime;
		qsf::Time mHint23WaitTime;
		qsf::Time mHint25WaitTime;
		qsf::Time mHint27WaitTime;

		// Settings backup
		GameSettingsGroup::AutomatismValue	mAutomatismBackup;
		float								mOriginalPolicemanAutomatismWaitTime;
		float								mOriginalParamedicAutomatismWaitTime;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::Tutorial)
