// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/network/matchmaking/MatchmakingTypes.h"
#include "em5/network/multiplayer/OperationMessages.h"
#include "em5/network/multiplayer/operationmessagehandler/OperationMessageHandlerManager.h"

#include <qsf/math/Color3.h>

#include <boost/noncopyable.hpp>
#include <boost/signals2.hpp>

#include <unordered_map>
#include <list>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Map;
	namespace game
	{
		class BitStream;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace multiplayer
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    EMERGENCY 5 multiplayer session base class
		*/
		class EM5_API_EXPORT SessionBase : public boost::noncopyable
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 CHAT_HISTORY_SIZE;

			// Combined states for a multiplayer host and client (Contains also states which are only used by one part)
			/**
			State changes definition
			- Host
			  NONE -> LOBBY
				- When create session is called and anything went fine
				-> Now connections are accepted
			  LOBBY -> LOADING_SCREEN
				- When loadMap is called and every client is ready (e.g. the hosting player presses the "start game" button in the lobby view)
				-> Any further connections are denied
			  LOADING_SCREEN -> IN_GAME
				- When onGameStart is called (e.g. the player presses the "start game" button on the loading screen)
				-> Any further connections are denied
			  IN_GAME -> POST_GAME

			- Client
			  NONE -> CONNECTING_TO_HOST
				- When joinSession is called
			  CONNECTING_TO_HOST -> CONNECTED_TO_HOST
			    - When connection to host was successfully
			  CONNECTED_TO_HOST-> LOBBY
				- When connection to the host succeeded and the join request was accepted
			  LOBBY -> LOADING_SCREEN
				- When the host sends the "load map" operation message
			  LOADING_SCREEN -> IN_GAME
				- When the host sends the "start game" operation message
			  IN_GAME -> POST_GAME
			*/
			enum class State : uint8
			{
				NONE,							///< Used by both: Initial state after an instance was created
				CONNECTING_TO_HOST,				///< Only used by an client to signal that the client tries to connect to the host
				CONNECTED_TO_HOST,				///< Only used by an client to signal that the client is connected to the host -> next step join request
				LOBBY,							///< Used by both to signal that they are currently in the "Lobby" state (Host accepts join request)
				LOADING_SCREEN,					///< Used by both to signal that the map is currently loading (At this state the host denies any further join requests)
				IN_GAME,						///< Used by both to signal that the game is currently running (At this state the host denies any further join requests)
				POST_GAME,
				POST_GAME_FINISH				///< Gets set after highscore publish is accepted/declined
			};

			struct PlayerInfo
			{
				uint16		teamId;
				uint32		playerIndex;
				std::string	playerName;
				qsf::Color3	playerColor;
				bool		isReady;
				bool		hasMapLoaded;	// Only internal for the host
				bool		isLocal;		// Only internal for the local player
				uint8		edition;		// Only internal for the host
				bool		wantHighscoreSubmit; // Only internal for the host
				uint16		ping; // Only internal for the host
				bool		pingAcknowleged; // Only internal for the host
				qsf::Time   lastPingRequestTime; // Only internal for the host
				uint8		mapQuality; // Contains the map quality which the player has set via graphic settings
				uint32		userData; // A user/game defined value, currently unused in EM5

				inline PlayerInfo() :
					teamId(qsf::getUninitialized<uint16>()),
					playerIndex(qsf::getUninitialized<uint32>()),
					isReady(false),
					hasMapLoaded(false),
					isLocal(false),
					edition(0),
					wantHighscoreSubmit(false),
					ping(0),
					pingAcknowleged(true),
					mapQuality(0),
					userData(qsf::getUninitialized<uint32>())
				{
					// Nothing to do in here
				}
			};

			struct ChatMessage
			{
				uint32 playerIndex;		///< A value of "qsf::getUninitialized<uint32>()" refers to "SYSTEM" and may be used for game event / lobby event / etc in chat
				std::string text;

				inline ChatMessage() :
					playerIndex(qsf::getUninitialized<uint32>())
				{
					// Nothing to do in here
				}

				inline ChatMessage(uint32 playerIndex, const std::string& text) :
					playerIndex(playerIndex),
					text(text)
				{
					// Nothing to do in here
				}
			};


		//[-------------------------------------------------------]
		//[ Public boost signals                                  ]
		//[-------------------------------------------------------]
		public:
			/** void onCurrentStateChanged(State prevState, State newState) */
			boost::signals2::signal<void(State, State)> currentStateChanged;

			/** void onChatMessage(const ChatMessage& message) */
			boost::signals2::signal<void(const ChatMessage&)> chatMessage;

			/** void onPlayerJoined(uint32 playerIndex, const PlayerInfo& playerInfo) */
			boost::signals2::signal<void(uint32, const PlayerInfo&)> playerJoined;

			/** void onPlayerLeft(uint32 playerIndex, const PlayerInfo& playerInfo) */
			boost::signals2::signal<void(uint32, const PlayerInfo&)> playerLeft;

			/** void onPlayerChangeReady(uint32 playerIndex, bool isReady) */
			boost::signals2::signal<void(uint32, bool)> playerChangeReady;

			/** void onPlayerUserDataChanged(uint32 playerIndex, uint32 userData) */
			boost::signals2::signal<void(uint32, uint32)> playerUserDataChanged;

			/** void onGameModeDataChanged(const GameModeUpdateData& gameModeData) */
			boost::signals2::signal<void(const GameModeUpdateData&)> gameModeDataChanged;


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		public:
			static void generateContentAuthenticationChecksums(ContentAuthenticationChecksums& checksums);


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			SessionBase();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~SessionBase();

			//[-------------------------------------------------------]
			//[ Query configuration                                   ]
			//[-------------------------------------------------------]
			uint32 getLocalPlayerIndex() const;
			const std::string& getSessionName() const;
			const std::string& getSessionPassword() const;
			uint32 getMaximumPlayerCount() const;
			qsf::GlobalAssetId getGlobalMapAssetId() const;
			multiplayer::GameMode getGameMode() const;
			uint32 getDuration() const;	// Duration of the game session in minutes, 0 for endless mode
			const std::string& getGameModificationString() const;
			bool isUsingPlayerSeparation() const;

			uint8 getMapQualityToUse() const;

			//[-------------------------------------------------------]
			//[ State                                                 ]
			//[-------------------------------------------------------]
			State getCurrentState() const;
			const std::list<ChatMessage>& getChatHistory() const;
			const std::unordered_map<uint32, PlayerInfo>& getPlayerList() const;
			const PlayerInfo* getPlayerByIndex(uint32 playerIndex) const;
			void sendChatMessage(const std::string& message);
			OperationMessageHandlerManager& getOperationMessageHandlerManager();


		//[-------------------------------------------------------]
		//[ Public virtual methods                                ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Is called when the map was successfully loaded but before game is initialized
			*/
			virtual bool startup(qsf::Map& map) = 0;

			/**
			*  @brief
			*    Is called when the map was successfully loaded and any further game initializations where done
			*/
			virtual void loadingFinished() = 0;


		//[-------------------------------------------------------]
		//[ Protected virtual methods                             ]
		//[-------------------------------------------------------]
		protected:
			virtual void onSendChatMessage(const ChatMessage& chatMessage) = 0;
			virtual void onChatMessageReceived(const ChatMessage& chatMessage);


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			void setLocalPlayerIndex(uint32 localPlayerIndex);
			void setSessionName(const std::string& name);
			void setSessionPassword(const std::string& password);
			void setMaximumPlayerCount(uint32 count);
			void setGlobalMapAssetId(qsf::GlobalAssetId globalAssetId);
			void setGameMode(multiplayer::GameMode mode);
			void setDuration(uint32 duration);
			void setGameModificationString(const std::string& string);
			void setUsingPlayerSeparation(bool isUsingPlayerSeparation);
			void setCurrentState(State state);
			void addChatMessage(const ChatMessage& message);
			void clearChatHistory();
			void handleChatMessageData(uint64 dataSize, const qsf::game::BitStream& bitStream);
			void setupGameSession();

			void onPlayerUserDataChanged(uint32 playerIndex, uint32 userData);

			/**
			*  @brief
			*    Common modifications of the map for host and client
			*/
			void setupMap(qsf::Map& map);


		//[-------------------------------------------------------]
		//[ Protected data                                        ]
		//[-------------------------------------------------------]
		protected:
			std::unordered_map<uint32, PlayerInfo>	mPlayerInfoMap;	///< key=playerIndex value=PlayerInfo


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			// Configuration
			std::string						mSessionName;			///< The name of the game session
			std::string						mSessionPassword;		///< The password required to join the session
			uint32							mMaximumPlayerCount;	///< Maximum number of players in this game session
			qsf::GlobalAssetId				mGlobalMapAssetId;		///< Global asset ID of the map the game session will be played on
			multiplayer::GameMode			mGameMode;				///< Game mode that will be played in this game session
			uint32							mDuration;				///< Duration of the game session in minutes
			std::string						mGameModificationString;
			bool							mUsingPlayerSeparation;	///< If set, players can't influence each other (referring to AI)

			// State
			std::list<ChatMessage>			mChatHistory;
			State							mCurrentState;
			uint32							mLocalPlayerIndex;

			OperationMessageHandlerManager	mOperationMessageHandlerManager;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5
