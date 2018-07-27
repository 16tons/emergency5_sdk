// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/network/multiplayer/OperationMessages.h"
#include "em5/network/multiplayer/SessionBase.h"
#include "em5/network/multiplayer/datacache/MapCache.h"
#include "em5/network/multiplayer/client/AudioManager.h"

#include <qsf/job/JobProxy.h>
#include <qsf/message/MessageProxy.h>

#include <memory> // for std::unique_ptr


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class CommandContext;
	class MapIdentifier;
	namespace multiplayer
	{
		namespace client
		{
			class HostBase;
		}
	}
}
namespace qsf
{
	class Map;
	namespace game
	{
		class BitStream;
		class Command;
		class CommandContext;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace multiplayer
	{
		namespace client
		{


			//[-------------------------------------------------------]
			//[ Classes                                               ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    EMERGENCY 5 multiplayer client class
			*/
			class EM5_API_EXPORT Client : public SessionBase
			{


			//[-------------------------------------------------------]
			//[ Public boost signals                                  ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Called when the client is disconnected
				*
				*  @note
				*    - Recommended slot signature: void onDisconnected()
				*    - Only called in case there was actually a connection
				*/
				boost::signals2::signal<void()> disconnected;


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Constructor
				*/
				Client();

				/**
				*  @brief
				*    Destructor
				*/
				virtual ~Client();

				bool joinSession(const multiplayer::HostEntry& hostEntry, const std::string& playerName, const std::string& sessionPassword);

				/**
				*  @brief
				*    "Joins" a local session.(Which effectively creates a local host instance) This session is only used for testing the multiplayer serialization part
				*
				*  @param[in] dataPath
				*    The path where the local client will find the files for the game player/command recorder.
				*/
				void joinLocalSession(const std::string& dataPath, const MapIdentifier& mapIdentifier, const std::string& playerName, const std::string& sessionPassword);

				/**
				*  @brief
				*    Closes the currently running session and closes all related network connections
				*/
				void closeSession();

				bool getReadyState() const;
				void setReadyState(bool isReady);
				void changeUserData(uint32 newUserData);

				JoinResponse getJoinResponse() const;

				/**
				*  @brief
				*    Sends data to the host
				*/
				void sendOperationMessageToHost(const OperationMessage operationMessageId, const qsf::game::BitStream& operationMessageData);

				void onCommandExecuted(const qsf::game::Command& command, const std::vector<qsf::Entity*>& callers, qsf::game::CommandContext& context);

				void onSendHighscoreSubmitInitiated();

				void onSendHighscoreSubmitDeclined();

				void onSendHighscoreSubmitAccept();


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


			//[-------------------------------------------------------]
			//[ Protected virtual em5::multiplayer::SessionBase methods ]
			//[-------------------------------------------------------]
			protected:
				virtual void onSendChatMessage(const ChatMessage& chatMessage) override;


			//[-------------------------------------------------------]
			//[ Private methods                                       ]
			//[-------------------------------------------------------]
			private:
				bool connectToHost();
				void shutdownConnection();

				void onConnectedToHost();
				void onDisconnectedFromHost();
				void onUnexpectedDisconnectFromHost();
				void onReceivedOperationMessage(const OperationMessage operationMessageId, uint64 dataSize, const qsf::game::BitStream& bitStream);
				void onInterpolation(const qsf::JobArguments& jobArguments);

				// Message handlers
				void onPreOrderUnitCommandExecute(const qsf::MessageParameters& parameters);

				void handleCreateEntityFromPrefabData(uint64 dataSize, const qsf::game::BitStream& bitStream);
				void handleDestroyEntityData(uint64 dataSize, const qsf::game::BitStream& bitStream);
				void handleUnitSpawnedData(uint64 dataSize, const qsf::game::BitStream& bitStream);
				void handleUnitSpawnedPlayerUpdateSelectionData(uint64 dataSize, const qsf::game::BitStream& bitStream);
				void handleFreeplayEventUpdatedData(uint64 dataSize, const qsf::game::BitStream& bitStream);
				void handlePlayerScoreChangedData(uint64 dataSize, const qsf::game::BitStream& bitStream);
				void handleGameModeUpdateData(uint64 dataSize, const qsf::game::BitStream& bitStream);
				void handleMinimapRelatedMessageData(uint64 dataSize, const qsf::game::BitStream& bitStream);
				void handleMinimapIconMessageData(uint64 dataSize, const qsf::game::BitStream& bitStream);
				void handleCommandResponseData(uint64 dataSize, const qsf::game::BitStream& bitStream);
				void handleLayerChangedData(uint64 dataSize, const qsf::game::BitStream& bitStream);
				void handleHighscoreSubmitInitiated();
				void handleHighscoreSubmitDeclined();
				void handleHighscoreSubmitSucceeded(uint64 dataSize, const qsf::game::BitStream& bitStream);
				void handleServerPing(uint64 dataSize, const qsf::game::BitStream& bitStream);
				void handleFireHoseUpdateData(uint64 dataSize, const qsf::game::BitStream& bitStream);
				void handleSpawnParticleData(uint64 dataSize, const qsf::game::BitStream& bitStream);

				void disableJob(uint32 jobId);

				void applyLayerChanges(int32 currentTick);
				void applyDestroyEntityChanges(int32 currentTick);

				// Proxy part
				bool connectToProxy();
				void onProxyClientConnected();
				void onProxyClientDisconnected();
				void onProxyClientConnectFailed();
				void closeProxyConnection();


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
			private:
				multiplayer::HostEntry		mHostEntry;
				std::string					mDataPath;
				qsf::Map*					mCurrentMap;
				std::unique_ptr<HostBase>	mHost;						///< The host to which the client has connected
				MapCache					mMapCache;

				std::string					mPlayerName;
				bool						mIsReady;
				bool						mFirstDataUpdateHandled;	///< Indicates if we got the first DATA_UPDATE message from the host
				JoinResponse				mJoinResponse;

				glm::vec3					mLastViewPosition;

				qsf::JobProxy				mInterpolateJobProxy;
				qsf::MessageProxy			mOrderUnitMessageProxy;	///< Message proxy for recording order unit commands and send them to the host
				qsf::JobProxy				mReconnectOneShotJobProxy;

				boost::container::flat_map<uint32, bool> mJobStatus;

				std::vector<std::pair<int32, std::vector<EntityCreatedFromPrefabData>>> mCreateEntityHistory;
				std::vector<std::pair<int32, std::vector<uint64>>> mDestroyEntityHistory;
				std::vector<std::pair<int32, std::vector<UnitSpawnedData>>> mUnitSpawnedHistory;
				std::vector<std::pair<int32, std::vector<UnitSpawnedPlayerUpdateSelectionData>>> mUnitSpawnedPlayerUpdateSelectionHistory;
				std::vector<std::pair<int32, std::vector<FreeplayEventData>>> mChangedEventList;
				std::vector<std::pair<int32, boost::container::flat_map<uint32, uint32>>> mChangedPlayerScoreHistory;
				std::vector<std::pair<int32, GameModeUpdateData>> mGameModeUpdateHistory;
				std::vector<std::pair<int32, std::vector<NewHintData>>> mNewHintHistory;
				std::vector<std::pair<int32, std::vector<MinimapMessageData>>> mMinimapMessagesHistory;
				std::vector<std::pair<int32, std::vector<MinimapIconMessageData>>> mMinimapIconMessagesHistory;
				std::vector<std::pair<int32, std::vector<LayerChangedData>>> mLayerChangedHistory;
				std::vector<std::pair<int32, std::vector<CreateFireHoseData>>> mCreateFireHoseHistory;
				std::vector<std::pair<int32, std::vector<SpawnParticleData>>> mSpawnParticleHistory;

				AudioManager				mAudioManager; ///< Handles the AUDIO_UPDATE message

				std::vector<boost::signals2::connection>		mMultiplayerProxyClientSignalConnections;


			};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // client
	} // multiplayer
} // em5
