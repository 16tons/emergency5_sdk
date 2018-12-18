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
#include "em5/network/multiplayer/SessionBase.h"
#include "em5/network/multiplayer/datacache/MapCache.h"
#include "em5/network/multiplayer_proxy/MultiplayerProxyTypes.h"
#include "em5/map/MapIdentifier.h"

#include <qsf/job/JobProxy.h>
#include <qsf/message/MessageProxy.h>
#include <qsf/math/Transform.h>

#include <vector>
#include <unordered_map>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Layer;
	class Map;
	class Clock;
	namespace game
	{
		class BitStream;
		class ServerConnection;
	}
}
namespace em5
{
	class MapIdentifier;
	class MatchmakingClient;
	namespace multiplayer
	{
		namespace server
		{
			class ClientBase;
		}
		namespace proxy
		{
			class MultiplayerProxyClient;
		}
	}
	namespace packet
	{
		class ProxyServerPropertiesResponse;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace multiplayer
	{
		namespace server
		{


			//[-------------------------------------------------------]
			//[ Classes                                               ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    EMERGENCY 5 multiplayer host class
			*/
			class EM5_API_EXPORT Host : public SessionBase
			{


			//[-------------------------------------------------------]
			//[ Public static methods                                 ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Set a list of colors to use for the players
				*
				*  @param[in] playerColorsToUse
				*    A list of colors to use for players. No change is done when empty
				*
				*  @note
				*    - The order in the give list equals to the player slot index (the "slot" shown in the lobby window)
				*    - When less colors then maximum player count is given then the slot index is clamped to max color index
				*/
				static void setPlayerColorsToUse(const std::vector<qsf::Color3>& playerColorsToUse);


			//[-------------------------------------------------------]
			//[ Public boost signals                                  ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Called when all clients and the host have loaded the map
				*
				*  @note
				*    - Recommended slot signature: void onReadyForGameStart()
				*/
				boost::signals2::signal<void()> readyForGameStart;

				/**
				*  @brief
				*    Called when the host lost its connection to the proxy
				*
				*  @note
				*    - Recommended slot signature: void onReadyForGameStart()
				*/
				boost::signals2::signal<void()> lostConnectionToProxy;


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Constructor
				*/
				Host();

				/**
				*  @brief
				*    Destructor
				*/
				virtual ~Host();

				bool createSession(const std::string& localUserName, uint16 port, const std::string& sessionPassword, const HostEntry& hostEntry);

				/**
				*  @brief
				*    Creates a local session. This session is only used for testing the multiplayer serialization part
				*
				*  @param[in] dataPath
				*    The path where the local host will find the files for the game recorder/command player.
				*  @param[in] mapIdentifier
				*    The identifier of the map to be used
				*/
				void createLocalSession(const std::string& dataPath, const MapIdentifier& mapIdentifier);

				/**
				*  @brief
				*    Closes the currently running session and closes all related network connections
				*/
				void closeSession();

				/**
				*  @brief
				*    Initiates the map loading -> starting the game
				*/
				bool loadMap();

				/**
				*  @brief
				*    Is called when the game itself should run now (e.g. the hosting player clicked on the "start game" button in the loading screen)
				*/
				void onGameStart();

				/**
				*  @brief
				*    Returns if all players are ready
				*/
				bool areAllPlayersReady();

				/**
				*  @brief
				*    Kicks the given player, if exists
				*/
				void kickPlayer(uint32 playerIndex);

				/**
				*  @brief
				*    Changes userDate for given player, if exists
				*/
				void changeUserDataForPlayer(uint32 playerIndex, uint32 newUserData);

				/**
				*  @brief
				*    Changes game mode update data(This data is send via normal update)
				*/
				void changeGameModeUpdate(bool finished, uint16 timeleftInSeconds);

				/**
				*  @brief
				*    Add a new hint which should be send to the clients
				*/
				void addNewHint(const HintMessage::MessageType messageType, const std::string& message, uint32 playerIndex);

				void onNewAudio(AudioUpdateData& data);
				void onAudioStopped(const uint32 audioId);
				void onAudioUpdated(AudioUpdateData& data);

				void onLayerChanged(const qsf::Layer& layer);

				/**
				*  @brief
				*    Sends data to a given client (identified by player index)
				*/
				void sendOperationMessageToClient(uint32 playerIndex, const OperationMessage operationMessageId, const qsf::game::BitStream& operationMessageData);

				/**
				*  @brief
				*    Sends data to all clients
				*/
				void sendOperationMessageToAllClients(const OperationMessage operationMessageId, const qsf::game::BitStream& operationMessageData, uint32 excludedPlayerIndex = qsf::getUninitialized<uint32>(0));

				/**
				*  @brief
				*    Sends the highscore submit initiated message to all clients.
				*/
				void onSendHighscoreSubmitInitiated();

				/**
				*  @brief
				*    Sends the highscore submit declined message to all clients.
				*/
				void onSendHighscoreSubmitDeclined();

				void onNewFireHose(CreateFireHoseData& data);

				void onSpawnParticle(const uint32 prefabId, const float timeout, const glm::vec3& position, const glm::vec3& scale);


			//[-------------------------------------------------------]
			//[ Public virtual em5::multiplayer::SessionBase methods  ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Is called when the map was successfully loaded but before game is initialized
				*/
				virtual bool startup(qsf::Map& map) override;

				/**
				*  @brief
				*    Is called when the map was successfully loaded and any further game initializations where done
				*/
				virtual void loadingFinished() override;

				/**
				*  @brief
				*    Is called when host want to publish the highscore
				*/
				void acceptHighsocrePublising();


			//[-------------------------------------------------------]
			//[ Protected virtual em5::multiplayer::SessionBase methods ]
			//[-------------------------------------------------------]
			protected:
				virtual void onSendChatMessage(const ChatMessage& chatMessage) override;
				virtual void onChatMessageReceived(const ChatMessage& chatMessage);


			//[-------------------------------------------------------]
			//[ Private methods                                       ]
			//[-------------------------------------------------------]
			private:
				// qsf::game::server part
				void onGameServerClientConnected(qsf::game::ServerConnection& serverConnection);
				void onGameServerClientDisconnected(qsf::game::ServerConnection& serverConnection);
				void onGameServerShutdown();

				// Matchmaking part
				bool connectToMatchmaking(const std::string& localUserName);
				void sendMatchmakingHostUpdate();
				void onMatchmakingClientConnected();
				void onMatchmakingClientDisconnected();
				void onMatchmakingClientConnectionLost();
				void onMatchmakingClientConnectFailed();
				void onMatchmakingClientRegisterHostResponse(multiplayer::RegisterHostStatus status);
				void onMatchmakingClientProxyServerPropertiesResponse(const packet::ProxyServerPropertiesResponse& response);

				void closeMatchmakingConnection();

				// Proxy part
				void connectToProxy();
				void onProxyClientConnected();
				void onProxyClientDisconnected();
				void onProxyClientConnectFailed();
				void onProxyClientSessionCreated(proxy::CreateSessionResultType result, uint64 sessionId);
				void onProxyClientClientConnectedToSession(uint8 proxySessionClientId);
				void onProxyClientClientDisconnectedFromSession(uint8 proxySessionClientId);
				void closeProxyConnection();

				// Local message handling
				void onEntityCreatedFromPrefab(const qsf::MessageParameters& parameters);
				void onEntityDestroyed(uint64 destroyedEntityId);
				void onUnitSpawned(const qsf::MessageParameters& parameters);
				void onUnitSpawnedPlayerUpdateSelection(const qsf::MessageParameters& parameters);
				void onFreeplayEventSucceeded(const qsf::MessageParameters& parameters);
				void onFreeplayEventFailed(const qsf::MessageParameters& parameters);
				void onFreeplayEventUpdated(const qsf::MessageParameters& parameters);
				void onFreeplayEventTriggered(const qsf::MessageParameters& parameters);
				void onPlayerScoreChanged(const qsf::MessageParameters& parameters);

				void onMinimapRelatedMessage(const qsf::MessageParameters& parameters);
				void onMinimapAddIconMessage(const qsf::MessageParameters& parameters);
				void onMinimapAddCirclePolygonMessage(const qsf::MessageParameters& parameters);
				void onMinimapRemoveCirclePolygonMessage(const qsf::MessageParameters& parameters);
				void onMinimapRemoveIconMessage(const qsf::MessageParameters& parameters);

				// Data Update helper
				void gatherDataForSend(const qsf::JobArguments& jobArguments);
				void sendDataToAllClients(const qsf::Clock& clock, const qsf::game::BitStream& bitStream);
				void sendDataToAllClients(const qsf::game::BitStream& bitStream);
				void sendDataToAllClients(const qsf::game::BitStream& bitStream, uint32 excludedPlayerIndex);
				void sendDataToProxyForAllClients(const qsf::game::BitStream& bitStream);
				void getDataForUpdateDataOperationMessage(qsf::game::BitStream& bitStream, const qsf::Clock& clock);
				void getDataForOtherOperationMessages(qsf::game::BitStream& bitStream, const qsf::Clock& clock);

				// highscorePublishing
				void publishHighscore();

				/**
				*  @brief
				*    Gets called, when a client has connected to this host
				*/
				void onClientConnected(std::unique_ptr<ClientBase> client);

				/**
				*  @brief
				*    Gets called, when a client disconnects from this host
				*/
				void onClientDisconnected(uint64 clientId);

				void onReceivedOperationMessage(ClientBase& client, const OperationMessage operationMessageId, uint64 dataSize, const qsf::game::BitStream& bitStream);

				/**
				*  @brief
				*    Handler for the OperationMessage::COMMAND_EXECUTION
				*/
				void onCommandExecutionReceived(ClientBase& client, uint64 dataSize, const qsf::game::BitStream& bitStream);

				/**
				*  @brief
				*    Handler for the OperationMessage::CLIENT_JOIN_REQUEST
				*/
				void handleClientJoinRequest(ClientBase& client, uint64 dataSize, const qsf::game::BitStream& bitStream);

				/**
				*  @brief
				*    Handler for the OperationMessage::CLIENT_CHANGE_READY
				*/
				void handleClientChangeReady(ClientBase& client, uint64 dataSize, const qsf::game::BitStream& bitStream);

				/**
				*  @brief
				*    Handler for the OperationMessage::CLIENT_MAP_LOADED
				*/
				void handleClientLoadedMap(uint32 playerIndex);

				/**
				*  @brief
				*    Handler for the OperationMessage::CLIENT_ORDERED_UNIT
				*/
				void handleClientOrderedUnit(ClientBase& client, uint64 dataSize, const qsf::game::BitStream& bitStream);

				/**
				*  @brief
				*    Handler for the OperationMessage::HIGHSCORE_SUBMIT_INITIATED
				*/
				void handleHighscoreSubmitInitiated();

				/**
				*  @brief
				*    Handler for the OperationMessage::HIGHSCORE_SUBMIT_ACCEPT
				*/
				void handleHighscoreSubmitAccept(uint32 playerIndex);

				/**
				*  @brief
				*    Handler for the OperationMessage::HIGHSCORE_SUBMIT_DECLINED
				*/
				void handleHighscoreSubmitDeclined();

				/**
				*  @brief
				*    Handler for the OperationMessage::CLIENT_PLAYER_CHANGED_USERDATA
				*/
				void handleClientPlayerChangedUserData(ClientBase& client, uint64 dataSize, const qsf::game::BitStream& bitStream);

				// Helper methods
				void addUser(uint32 playerIndex, const std::string& userName, uint8 edition, uint8 mapQuality, bool isReady = false);
				void removeUser(uint32 playerIndex);
				void removePlayerUnitsFromMap(uint32 playerIndex);
				void setupHostEntry(HostEntry& hostEntry);
				std::string getUniquePlayerName(const std::string& playerName);

				PlayerInfo* getPlayerInfo(uint32 playerIndex);
				ClientBase* getClient(uint32 playerIndex);
				void removeClientFormActiveList(uint32 playerIndex);
				void addClientToActiveList(ClientBase* client);

				void updatePing();
				void handleClientPingAcknowledge(ClientBase& client, uint64 dataSize, const qsf::game::BitStream& bitStream);


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
			private:
				std::string												mDataPath;
				qsf::Map*												mCurrentMap;
				qsf::GlobalAssetId										mMapGlobalAssetId;		///< The global asset id of the map which should be loaded for the multiplayer
				std::unordered_map<uint64, std::unique_ptr<ClientBase>>	mClients;				///< A list of all connected clients
				std::vector<std::pair<uint32, ClientBase*>>				mActiveClients;			///< A list of "active" (have a player index assigned) clients. We use a pair here so it doesn't matter if the pointer is still valid when trying to remove an element via its player index.
				qsf::JobProxy											mUpdateDataJobProxy;
				MapIdentifier											mMapIdentifier;

				std::shared_ptr<MatchmakingClient>				mMatchmakingClient;
				std::vector<boost::signals2::connection>		mMatchmakingSignalConnections;
				uint16											mLocalPort;
				uint16											mExternalPort;
				bool											mIsPortForwarded;
				ContentAuthenticationChecksums					mContentAuthenticationChecksums;

				proxy::MultiplayerProxyClient*					mMultiplayerProxyClient;
				std::vector<boost::signals2::connection>		mMultiplayerProxyClientSignalConnections;
				bool											mReceivedProxyServerProperties;

				MapCache										mMapCache;				///< The data cache for the current map

				std::vector<EntityCreatedFromPrefabData>		mEntityCreatedFromPrefabList;	///< Contains a list of all entities created from prefab between two data update calls
				std::vector<uint64>								mEntityDestroyedList;			///< Contains a list of all entities destroyed between two data update calls
				std::vector<UnitSpawnedData>					mUnitSpawnedList;				///< Contains a list of all unit spawned between two data update calls
				std::vector<UnitSpawnedPlayerUpdateSelectionData>	mUnitSpawnedPlayerUpdateSelectionList;	///< Contains a list of all unit spawns that qualify for a selection update between two data update calls
				std::unordered_map<uint32, FreeplayEventData>	mChangedEventList;				///< Contains a list of all changed freeplay events between two data update calls
				boost::container::flat_map<uint32, uint64>		mFocusEntityOfActiveEventsList;	///< Contains a list of freeplay event ids for which the focus entity should be checked for changes
				boost::container::flat_map<uint32, uint32>		mChangedPlayerScoreList;		///< Contains a list of all player score changes between two data update calls
				GameModeUpdateData								mGameModeUpdateData;
				std::vector<NewHintData>						mNewHintsList;					///< Contains a list of all new hints which were started between two data update calls
				std::vector<MinimapMessageData>					mMinimapReleatedMessageList;	///< Contains a list of all minimap related messages which were sent between two data update calls
				std::vector<MinimapIconMessageData>				mMinimapIconMessageList;				///< Contains a list of all minimap icon messages which were sent between two data update calls
				std::vector<AudioUpdateData>					mAudioUpdatedList;				///< Contains a list of changed audio between two data update calls
				std::vector<LayerChangedData>					mLayerChangedList;				///< Contains a list of changed layers between two data update calls
				std::vector<CreateFireHoseData>					mNewFireHosesList;				///< Contains a list of new fire hoses between two data update calls
				std::vector<SpawnParticleData>					mSpawnParticlesList;			///< Contains a list of new spawned particles between two data update calls

				qsf::MessageProxy								mEntityCreatedFromPrefabMessageProxy;	///< Message proxy for the "Created entity from prefab" message
				qsf::MessageProxy								mEntityDestroyedMessageProxy;			///< Message proxy for the "entity destroyed" message
				qsf::MessageProxy								mUnitSpawnedMessageProxy;				///< Message proxy for the "unit spawned" message
				qsf::MessageProxy								mUnitSpawnedPlayerUpdateSelectionMessageProxy;	///< Message proxy for the "unit spawned player update selection" message
				qsf::MessageProxy								mEventSuccessMessageProxy;				///< Message proxy for the "freeplay event succeeded" message
				qsf::MessageProxy								mEventFailedMessageProxy;				///< Message proxy for the "freeplay event failed" message
				qsf::MessageProxy								mEventChangedMessageProxy;				///< Message proxy for the "freeplay event changed" message
				qsf::MessageProxy								mEventTriggeredMessageProxy;			///< Message proxy for the "freeplay event triggered" message
				qsf::MessageProxy								mPlayerScoreChangedMessageProxy;		///< Message proxy for the "player score changed" message

				// Minimap related messages, which not already implicit handled on client side via other data
				// TODO(sw) We could avoid sending these information, when we could push certain actions on client side
				qsf::MessageProxy								mEntityEnteredVehicleMessageProxy;
				qsf::MessageProxy								mEntityLeftVehicleMessageProxy;
				qsf::MessageProxy								mPersonPlacedInPoliceMessageProxy;
				qsf::MessageProxy								mPersonPlacedInAmbulanceMessageProxy;
				qsf::MessageProxy								mGangsterAppearedMessageProxy;
				qsf::MessageProxy								mGangsterEscapedMessageProxy;
				qsf::MessageProxy								mPersonGetsHealedMessageProxy;
				qsf::MessageProxy								mCarPickedUpMessageProxy;
				qsf::MessageProxy								mPickupPersonByParamedicMessageProxy;
				qsf::MessageProxy								mRescueDogFoundPersonMessageProxy;
				qsf::MessageProxy								mUnloadVehicleFromTransporterMessageProxy;
				qsf::MessageProxy								mLoadVehicleToTransporterMessageProxy;

				// Minimap icon messages that directly add or remove icons
				qsf::MessageProxy								mMinimapAddIconMessageProxy;
				qsf::MessageProxy								mMinimapAddCirclePolygonMessageProxy;
				qsf::MessageProxy								mMinimapRemoveCirclePolygonMessageProxy;
				qsf::MessageProxy								mMinimapRemoveIconMessageProxy;

				qsf::Time										mLastSendPingTime;

				// Statistics
				uint64											mDataPerSecond;
				uint64											mPeekDataPerSecond;
				uint64											mPeekDataPerUpdate;
				uint64											mAverageDataPerSecond;
				uint64											mAverageDataPerUpdate;
				uint64											mLowestDataPerSecond;
				uint64											mLowestDataPerUpdate;
				MultiValueHistory								mDataValueHistory;


			};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // server
	} // multiplayer
} // em5
