// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/game/gamemode/GameModeType.h"
#include "em5/map/MapIdentifier.h"

#include <qsf/math/Color4.h>

#include <vector>


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
	*    EMERGENCY 5 game session configuration
	*/
	class EM5_API_EXPORT GameSessionConfiguration
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		enum CampaignIndex
		{
			EMERGENCY_5_CAMPAIGN	   = 0,
			EMERGENCY_2016_CAMPAIGN	   = 1,
			EMERGENCY_2017_CAMPAIGN	   = 2,
			EMERGENCY_20YEARS_MISSIONS = 3
		};

		struct PlayerMetadata
		{
			std::string mName;
			bool		mIsLocal;
			uint8		mEdition;
			uint32		mPlayerIndex;
			uint16		mTeamId;
			qsf::Color4	mPlayerColor;
			uint32		mUserData;

			PlayerMetadata() : mIsLocal(false), mEdition(0), mPlayerIndex(0), mTeamId(qsf::getUninitialized<uint16>()), mUserData(qsf::getUninitialized<uint32>()){}
		};
		typedef std::vector<PlayerMetadata> PlayerArray;

		static const uint32 MAXIMUM_NUMBER_OF_CAMPAIGNS = 4;	///< Maximum number of possible campaigns, use the dynamic "em5::VersionSwitch::getNumberOfCampaigns()" whenever possible instead


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return the game mode type by using a given CAMP class name; multiple game mode class implementations can use the same game mode type
		*/
		static gamemode::Type getGameModeTypeByClassName(const std::string& className);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		GameSessionConfiguration();

		/**
		*  @brief
		*    Destructor
		*/
		~GameSessionConfiguration();

		/**
		*  @brief
		*    Return the game mode CAMP class name
		*/
		const std::string& getGameModeClassName() const;

		/**
		*  @brief
		*    Return the game mode type; multiple game mode class implementations can use the same game mode type
		*/
		gamemode::Type getGameModeType() const;

		/**
		*  @brief
		*    Returns "true" if the game is running in campaign mode
		*/
		bool isGameModeCampaign() const;

		/**
		*  @brief
		*    Returns "true" if the game is running in freeplay mode
		*/
		bool isGameModeFreeplay() const;

		/**
		*  @brief
		*    Returns "true" if the game is running in multiplayer mode
		*/
		bool isGameModeMultiplay() const;

		/**
		*  @brief
		*    Returns "true" if tutorial should be active (ignored depending on game mode and map)
		*/
		bool isTutorialActive() const;

		/**
		*  @brief
		*    Return the map identifier
		*/
		const MapIdentifier& getMapIdentifier() const;

		/**
		*  @brief
		*    Return the campaign phase index inside the map
		*/
		uint32 getCampaignPhaseIndex() const;

		/**
		*  @brief
		*    Return the list of players
		*/
		const PlayerArray& getPlayers() const;

		/**
		*  @brief
		*    Return whether or not a single main event is played exclusively
		*/
		bool isSingleMainEvent() const;

		/**
		*  @brief
		*    Return the number of the single main event (0 for tutorial, 1..15 for EM5 main events, 100..104 for EM2016, etc.)
		*/
		uint32 getSingleMainEventNumber() const;

		/**
		*  @brief
		*    Return the index of the single main event campaign phase
		*/
		uint32 getSingleMainEventPhaseIndex() const;

		/**
		*  @brief
		*    Reset to defaults
		*/
		void reset();

		/**
		*  @brief
		*    Setup for campaign mode
		*
		*  @param[in] campaignProgress
		*    The campaign progress instance to be used. When a nullptr is given then a new campaign is started (on map Munich with first phase)
		*     Note: The game session configuration takes ownership of this instance
		*  @param[in] campaignIndex
		*    Index of the campaign to play
		*  @param[in] value
		*    Indicates of the tutorial is active
		*/
		void setupForCampaign(CampaignProgress* campaignProgress, uint32 campaignIndex, bool tutorialActive, const std::string& gameModeClassName = "");

		/**
		*  @brief
		*    Setup for single main event
		*/
		void setupForSingleMainEvent(uint32 mainEventNumber, const MapIdentifier& mapIdentifier, uint32 campaignIndex, uint32 campaignPhaseIndex, bool tutorialActive, const std::string& gameModeClassName = "");

		/**
		*  @brief
		*    Setup for freeplay mode
		*/
		void setupForFreeplay(const MapIdentifier& mapIdentifier, const std::string& gameModeClassName = "");

		/**
		*  @brief
		*    Setup for challenge freeplay mode
		*/
		void setupForChallengeFreeplay(const MapIdentifier& mapIdentifier, const std::string& gameModeClassName = "");

		/**
		*  @brief
		*    Setup for multiplay coop mode
		*
		*  @param[in] durationInMinutes
		*    Duration of the game session in minutes, 0 for endless mode
		*
		*  @remarks
		*    There are no players added, you have to do this afterwards.
		*/
		void setupForMultiplayCoop(const MapIdentifier& mapIdentifier, uint32 durationInMinutes, const std::string& gameModeClassName = "");

		/**
		*  @brief
		*    Setup for multiplay pvp mode
		*
		*  @param[in] durationInMinutes
		*    Duration of the game session in minutes, 0 for endless mode
		*
		*  @remarks
		*    There are no players added, you have to do this afterwards.
		*/
		void setupForMultiplayPvp(const MapIdentifier& mapIdentifier, uint32 durationInMinutes, const std::string& gameModeClassName = "");

		/**
		*  @brief
		*    Setup for benchmark mode
		*/
		void setupForBenchmark(const std::string& gameModeClassName = "");

		/**
		*  @brief
		*    Add a new player
		*/
		PlayerMetadata& addPlayer(bool local = true);


	//[-------------------------------------------------------]
	//[ Public data                                           ]
	//[-------------------------------------------------------]
	public:
		std::string							mGameModeClassName;			///< Game mode CAMP class name: one of campaign, freeplay, multiplay
		bool								mTutorialActive;			///< "true" if tutorial should be active (ignored depending on game mode and map)
		uint32								mCampaignIndex;				///< Index of campaign to play (only campaign game mode)
		MapIdentifier						mMapIdentifier;				///< Map identifier for the selected map
		PlayerArray							mPlayers;					///< Players
		std::shared_ptr<CampaignProgress>	mCampaignProgress;			///< The campaign progress to be used, only valid when game mode is campaign
		uint32								mSingleMainEventNumber;		///< Single main event only: Number of main event (e.g. 302 for EM20 mission "Street Race")
		uint32								mSingleMainEventPhaseIndex;	///< Single main event only: Index of the campaign phase to start
		uint32								mGameDurationInMinutes;		///< Duration of the game session in minutes, 0 for endless mode


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
