// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/event/FreeplayEvent.h"
#include "em5/game/gamemode/GameMode.h"
#include "em5/map/MapIdentifier.h"

#include <qsf/message/MessageProxy.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Map;
}
namespace em5
{
	class MapIdentifier;
	class Player;
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
	*    EMERGENCY 5 freeplay game mode
	*/
	class EM5_API_EXPORT FreeplayGameMode : public GameMode
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		FreeplayGameMode();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~FreeplayGameMode();

		/**
		*  @brief
		*    Starts or stops the world event. This is used for debug.
		*/
		void toggleWorldEventPhase();

		/**
		*  @brief
		*    Starts or stops the seasonal event. This activates and deactivate event specific layers
		*/
		void toggleSeasonalEvent(const std::string& seasonalEventIdentifier, bool show);


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
		virtual void onEventSucceeded(const qsf::MessageParameters& parameters);
		virtual void onEventFailed(const qsf::MessageParameters& parameters);
		virtual void onReceiveObjectivePoints(const qsf::MessageParameters& parameters);
		// Gets called, when through a successful event some points should be added to players point "pool"
		virtual void onSetPointGain(const FreeplayEvent::EventResult& eventResult, int32 pointGain);
		virtual void onEventCompleted(const qsf::MessageParameters& parameters);
		virtual void onFirstEventCompleted();
		virtual std::string getIntroVideo(uint32 index);


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		bool setupEventPool(const std::string& postfix = "", bool allowFallback = true);
		bool setupFixedEventPool();
		void setupUnitPool(const Player* player = nullptr);
		void setupMessageHandler();

		void updateEventGenerator(const qsf::Time& timePassed);
		void setupWorldEvents(const qsf::Map& map, bool showHint);
		void cleanupWorldEvents(const qsf::Map& map);
		void setupFreeplay();

		bool startIntroVideo(uint32 index);

		/**
		*  @brief
		*    Resets the event limit to the limit specified by the spec
		*/
		void resetEventLimit();


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void eventSucceeded(const qsf::MessageParameters& parameters);
		void eventFailed(const qsf::MessageParameters& parameters);
		void onCutsceneFinished(const qsf::MessageParameters&);
		void onEventFinished();


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		MapIdentifier	mMapIdentifier;			///< The current map
		bool			mIsPlayingIntro;
		bool			mFirstEventCompleted;
		qsf::MessageProxy mCutsceneMessageProxy;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::Time	mWorldEventWaitTime;
		qsf::Time	mWorldEventWaitAfterEndTime;
		float		mWorldEventTriggerChance;

		// Messages proxies
		qsf::MessageProxy mMessageEventSucceeded;
		qsf::MessageProxy mMessageEventFailed;
		qsf::MessageProxy mMessageReciveObjectivePoints;

		// Intro
		uint32 mIntroVideoIndex;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::FreeplayGameMode)
