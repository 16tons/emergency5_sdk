// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/game/gamemode/FreeplayGameMode.h"
#include "em5/game/gamemode/CampaignDefinition.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class CampaignProgress;
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
	*    EMERGENCY 5 campaign game mode
	*/
	class CampaignGameMode : public FreeplayGameMode
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class FreeplaySystem;	// Sets "em5::CampaignGameMode::mCampaignIndex"


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		CampaignGameMode();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~CampaignGameMode();

		/**
		*  @brief
		*    Go to next map in campaign progress
		*/
		void initNextMap(bool triggerStartGame);

		/**
		*  @brief
		*    Are we in a main-event phase right now?
		*/
		bool isMainEvent() const;

		/**
		*  @brief
		*    Are we in a mini-event phase right now?
		*/
		bool isMiniEvent() const;

		/**
		*  @brief
		*    Received repeat signal?
		*/
		bool getReceivedRepeatSignal() const;


	//[-------------------------------------------------------]
	//[ Public virtual em5::GameMode methods                  ]
	//[-------------------------------------------------------]
	public:
		virtual std::string getGameModeAsString() const override;
		virtual void startup() override;
		virtual void shutdown() override;
		virtual void onGameLoaded() override;
		virtual void updateGameMode(qsf::Time timePassed) override;
		virtual void onSendGameModeSignal(const std::string& gameModeSignal) override;


	//[-------------------------------------------------------]
	//[ Protected virtual em5::FreeplayGameMode methods       ]
	//[-------------------------------------------------------]
	protected:
		virtual void onEventSucceeded(const qsf::MessageParameters& parameters) override;
		virtual void onEventFailed(const qsf::MessageParameters& parameters) override;
		virtual void onReceiveObjectivePoints(const qsf::MessageParameters& parameters) override;
		// Gets called when through a successful event some points should be added to players point "pool"
		virtual void onSetPointGain(const FreeplayEvent::EventResult& eventResult, int32 pointGain) override;
		virtual std::string getIntroVideo(uint32 index) override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		enum PhaseState
		{
			STATE_PENDING,		///< Initial state, before phase has started
			STATE_RUNNING,		///< Phase is running
			STATE_ENDED,		///< Phase has ended successfully, usually we would continue to the next now
			STATE_FADEOUT,		///< Phase has ended successfully, currently still fading out (optional state)
			STATE_FAILED,		///< Phase has failed and must be repeated (only used for certain campaign phase types)
			STATE_UNDEFINED		///< Undefined phase state, no updates in this state
		};

		enum HQExtendedState
		{
			HQSTATE_NONE,
			HQSTATE_EXTENDED,
			HQSTATE_NOT_EXTENDED
		};


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void startPhase();
		void updatePhase(qsf::Time timePassed);
		void phaseEnded();
		void phaseFadeout();
		void phaseFailed();

		void gotoNextPhase();

		void setUnitPoolByName(const std::string& unitPoolName);
		void applyBuyOptions(const std::vector<Player::BuyOption>& buyOptions);
		bool checkBuyOptions(const std::vector<Player::BuyOption>& buyOptions) const;
		uint32 getRemainingBuyOptionsCost(const std::vector<Player::BuyOption>& buyOptions) const;
		void clearBoughtUnits();

		void onSetPointGain(int32 pointGain);
		void showPromotionWindow();

		void onOutroFadeOutFinished(const qsf::MessageParameters&);
		void onOutroCutsceneFinished(const qsf::MessageParameters&);
		void onEventFinishFadeOutFinished(const qsf::MessageParameters&);
		void onEventFinishFadeInFinished(const qsf::MessageParameters&);

		std::string getBackgroundMusicByMapIdentifier(const std::string& mapIdentifier, uint32 campaginIndex);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32											mCampaignIndex;		///< Index of current campaign; see "Game::getCampaignDefinitionByIndex" implementation
		const CampaignDefinition*						mCurrentCampaign;	///< Currently used campaign definition
		const CampaignDefinition::CampaignPhaseArray*	mCurrentMapPhases;	///< Campaign phases of the currently played map, do not destroy the contents
		const CampaignDefinition::CampaignPhase*		mCurrentPhase;		///< Current campaign phase, do not destroy the contents
		std::shared_ptr<CampaignProgress>				mCampaignProgress;	///< The campaign progress

		uint32			mCurrentMapIndex;				///< Index of current map
		uint32			mCurrentPhaseIndex;				///< Index of the campaign phase currently active
		PhaseState		mPhaseState;					///< State inside the current campaign phase
		bool			mFirstPhaseStart;				///< "true" if phase is started for the first time inside a map
		HQExtendedState	mHQExtendedState;				///< Informs if the HQ was Extended after a main event
		uint32			mSuccessfulEvents;				///< Number of successful freeplay events in the current campaign phase
		uint32			mFailedEvents;					///< Number of failed freeplay events in the current campaign phase
		bool			mAddedBuyOptions;				///< "true" after buy options for the current phase were added (only for "freeplay" phases)
		bool			mWorldEventsAdded;				///< "true" when the world events pool is added to the players pool ()
		bool			mReceivedContinueSignal;		///< "true" if main event was won and player chose to continue to the next phase
		bool			mReceivedRepeatSignal;			///< "true" if main/single event was lost and player chose to repeat it
		bool			mHaveToShowPromotionWindow;		///< "true" if the mCurrentMapPhases.size() < mCurrentPhaseIndex;
		qsf::Time		mTimeUntilNextBuyHint;			///< Time until the next hint to the player that he should buy his vehicles now, because the game gets impatient
		qsf::Time		mDelayToNextPhaseDelayTime;		///< Time to delay next phase updates
		bool			mMadeChangesDueToDifficulty;
		float			mOriginalFireDamageConvert;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::CampaignGameMode)
