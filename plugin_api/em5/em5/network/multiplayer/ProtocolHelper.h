// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/network/multiplayer/OperationMessages.h"
#include "em5/network/multiplayer/SessionBase.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
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
		*    EMERGENCY 5 multiplayer protocol helper
		*/
		class EM5_API_EXPORT ProtocolHelper
		{


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		public:
			static uint16 getProtocolVersion();

			/**
			*  @brief
			*    Writes the multiplayer protocol header to the given bit stream
			*    The header consist of following parts:
			*    - An Identifier string of 5 chars (EM5MP)
			*    - A protocol version, uint16 (The current version knows the helper)
			*    - The size of the payload (in amount of bits), uint32
			*
			*  @param[in] bitStream
			*    The bit stream to which the data is written to
			*  @param[in] payloadSize
			*    The payload size
			*/
			static void writeHeader(qsf::game::BitStream& bitStream, uint32 payloadSize);

			/**
			*  @brief
			*    Reads the multiplayer protocol header from the given stream and does some validation checks
			*    The header consist of following parts:
			*    - An Identifier string of 5 chars (EM5MP)
			*    - A protocol version, uint16 (The current version knows the helper)
			*    - The size of the payload (in amount of bits), uint32
			*
			*  @param[in] bitStream
			*    The bit stream from which the data is read from
			*  @param[out] protocolVersion
			*    The protocol version
			*  @param[out] payloadSize
			*    The payload size
			*
			*  @return
			*    "true" when all checks went fine, otherwise "false"
			*/
			static bool readAndCheckHeader(const qsf::game::BitStream& bitStream, uint16& protocolVersion, uint32& payloadSize);

			/**
			*  @brief
			*    Checks if the bit stream contains enough data for the payload
			*
			*  @param[in] bitStream
			*    The source bit stream to check
			*  @param[in] payloadSize
			*    The payload size which should the bit stream contain
			*
			*  @return
			*    "true" when the check went fine, otherwise "false"
			*/
			static bool checkPayloadDataSize(const qsf::game::BitStream& bitStream, uint32 payloadSize);

			/**
			*  @brief
			*    Writes the payload data to the bit stream
			*
			*  @param[in] bitStream
			*    The bit stream to which the data is written to
			*  @param[in] payloadData
			*    The bit stream containing the payload data
			*
			*  @return
			*    "true" when all went fine, otherwise "false"
			*/
			static bool writePayloadData(qsf::game::BitStream& bitStream, const qsf::game::BitStream& payloadData);

			/**
			*  @brief
			*    Reads the payload data from the bit stream
			*
			*  @param[in] bitStream
			*    The source bit stream from which the data is read from
			*  @param[out] payloadData
			*    The bit stream to which the read payload data is written to
			*  @param[in] payloadSize
			*    The payload size which should be read from the source bit stream
			*
			*  @return
			*    "true" when reading went fine, otherwise "false"
			*/
			static bool readPayloadData(const qsf::game::BitStream& bitStream, qsf::game::BitStream& payloadData, uint32 payloadSize);

			/**
			*  @brief
			*    Write an operation data to the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream to which the data is written to
			*  @param[in] operationMessage
			*    The operation message type which was read from the stream
			*  @param[in] operationMessageData
			*    The payload data of the operation message (can be empty)
			*
			*  @return
			*    "true" when reading went fine, otherwise "false"
			*/
			static bool writeOperationMessage(qsf::game::BitStream& bitStream, const OperationMessage& operationMessage, const qsf::game::BitStream& operationMessageData);

			/**
			*  @brief
			*    Reads an operation data from the bit stream
			*
			*  @param[in] bitStream
			*    The source bit stream from which the data is read from
			*  @param[out] operationMessage
			*    The operation message type which was read from the stream
			*
			*  @return
			*    "true" when reading went fine, otherwise "false"
			*/
			static bool readOperationMessageType(const qsf::game::BitStream& bitStream, OperationMessage& operationMessage);

			/**
			*  @brief
			*    Reads an operation data from the bit stream
			*
			*  @param[in] bitStream
			*    The source bit stream from which the data is read from
			*  @param[out] operationMessage
			*    The operation message type which was read from the stream
			*  @param[out] operationMessageData
			*    The payload data of the operation message (can be empty)
			*
			*  @return
			*    "true" when reading went fine, otherwise "false"
			*/
			static bool readOperationMessage(const qsf::game::BitStream& bitStream, OperationMessage& operationMessage, qsf::game::BitStream& operationMessageData);

			/**
			*  @brief
			*    Writes the data of the CLIENT_JOIN_REQUEST operation message to the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream to which the data is written to
			*  @param[in] joinRequestData
			*     The join request data
			*
			*  @return
			*    "true" when writing went fine, otherwise "false"
			*/
			static bool writeClientJoinRequestOperationMessageData(qsf::game::BitStream& bitStream, const JoinRequestData& joinRequestData);

			/**
			*  @brief
			*    Reads the data of the CLIENT_JOIN_REQUEST operation message from the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream from which the data is read from
			*  @param[out] joinRequestData
			*     The join request data
			*
			*  @return
			*    "true" when reading went fine, otherwise "false"
			*/
			static bool readClientJoinRequestOperationMessageData(const qsf::game::BitStream& bitStream, JoinRequestData& joinRequestData);

			/**
			*  @brief
			*    Writes the data of the SERVER_JOIN_RESPONSE operation message to the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream to which the data is written to
			*  @param[in] joinResponseData
			*    The join response data
			*
			*  @return
			*    "true" when writing went fine, otherwise "false"
			*/
			// TODO(sw) Add additional data for the join response, as e.g. which map
			static bool writeServerJoinResponseOperationMessageData(qsf::game::BitStream& bitStream, const JoinResponseData& joinResponseData);

			/**
			*  @brief
			*    Writes the data of the SERVER_JOIN_RESPONSE operation message to the bit stream.
			*    This is a convenience method for join response which deny the join
			*
			*  @param[in] bitStream
			*    The target bit stream to which the data is written to
			*  @param[in] joinResponse
			*    The join response value
			*
			*  @return
			*    "true" when writing went fine, otherwise "false"
			*/
			static bool writeServerJoinResponseWithoutAdditionalDataOperationMessageData(qsf::game::BitStream& bitStream, const JoinResponse& joinResponse);

			/**
			*  @brief
			*    Reads the data of the SERVER_JOIN_RESPONSE operation message from the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream from which the data is read from
			*  @param[out] joinResponseData
			*    The join response data
			*
			*  @return
			*    "true" when reading went fine, otherwise "false"
			*/
			// TODO(sw) Add additional data for the join request, as game mod string and the content checksums
			static bool readServerJoinResponseOperationMessageData(const qsf::game::BitStream& bitStream, JoinResponseData& joinResponseData);

			/**
			*  @brief
			*    Writes the data of the CLIENT_CHANGE_READY operation message to the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream to which the data is written to
			*  @param[in] readyState
			*    The data for the "ready for game" operation message
			*/
			static bool writeReadyForGameOperationMessageData(qsf::game::BitStream& bitStream, const bool readyState);

			/**
			*  @brief
			*    Reads the data of the CLIENT_CHANGE_READY operation message from the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream to which the data is written to
			*  @param[out] readyState
			*    The data for the CLIENT_CHANGE_READY operation message
			*/
			static bool readReadyForGameOperationMessageData(const qsf::game::BitStream& bitStream, bool& readyState);

			/**
			*  @brief
			*    Writes the data of the CLIENT_PLAYER_CHANGED_USERDATA operation message to the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream to which the data is written to
			*  @param[in] userData
			*    The data for the "userData changed" operation message
			*/
			static bool writeUserDataChangedOperationMessageData(qsf::game::BitStream& bitStream, const uint32 userData);

			/**
			*  @brief
			*    Reads the data of the CLIENT_PLAYER_CHANGED_USERDATA operation message from the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream to which the data is written to
			*  @param[out] userData
			*    The data for the CLIENT_PLAYER_CHANGED_USERDATA operation message
			*/
			static bool readUserDataChangedOperationMessageData(const qsf::game::BitStream& bitStream, uint32& userData);

			/**
			*  @brief
			*    Writes the data of the SERVER_LOAD_MAP operation message to the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream to which the data is written to
			*/
			static bool writeServerLoadMapOperationMessageData(qsf::game::BitStream& bitStream);

			/**
			*  @brief
			*    Writes the data of the CLIENT_MAP_LOADED operation message to the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream to which the data is written to
			*/
			static bool writeClientMapLoadedOperationMessageData(qsf::game::BitStream& bitStream);

			/**
			*  @brief
			*    Writes the data of the SERVER_GAME_START operation message to the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream to which the data is written to
			*/
			static bool writeServerGameStartOperationMessageData(qsf::game::BitStream& bitStream);

			/**
			*  @brief
			*    Writes the data of the SERVER_PLAYER_JOINED operation message to the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream to which the data is written to
			*  @param[in] playerList
			*    The List of player information to be send by the operation message as data
			*
			*  @return
			*    "true" when writing went fine, otherwise "false"
			*/
			static bool writeServerPlayerJoinedOperationMessageData(qsf::game::BitStream& bitStream, const std::vector<SessionBase::PlayerInfo>& playerList);

			/**
			*  @brief
			*    Reads the data of the SERVER_PLAYER_JOINED operation message from the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream from which the data is read from
			*  @param[out] playerList
			*    Will contain the list of player information which were part of the message
			*
			*  @return
			*    "true" when reading went fine, otherwise "false"
			*/
			static bool readServerPlayerJoinedOperationMessageData(const qsf::game::BitStream& bitStream, std::vector<SessionBase::PlayerInfo>& playerList);

			/**
			*  @brief
			*    Writes the data of the SERVER_PLAYER_LEFT operation message to the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream to which the data is written to
			*  @param[in] playerIndex
			*    The index of the player which has left the game
			*
			*  @return
			*    "true" when writing went fine, otherwise "false"
			*/
			static bool writeServerPlayerLeftOperationMessageData(qsf::game::BitStream& bitStream, uint32 playerIndex);

			/**
			*  @brief
			*    Reads the data of the SERVER_PLAYER_LEFT operation message from the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream from which the data is read from
			*  @param[out] playerIndex
			*    The index of the player which has left the game
			*
			*  @return
			*    "true" when reading went fine, otherwise "false"
			*/
			static bool readServerPlayerLeftOperationMessageData(const qsf::game::BitStream& bitStream, uint32& playerIndex);

			/**
			*  @brief
			*    Writes the data of the SERVER_PLAYER_CHANGED_READY operation message to the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream to which the data is written to
			*  @param[in] playerIndex
			*    The player index of the player which changed his ready state
			*  @param[in] isReady
			*    The ready state
			*
			*  @return
			*    "true" when writing went fine, otherwise "false"
			*/
			static bool writeServerPlayerChangedReadyStateOperationMessageData(qsf::game::BitStream& bitStream, uint32 playerIndex, bool isReady);

			/**
			*  @brief
			*    Reads the data of the SERVER_PLAYER_CHANGED_READY operation message from the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream from which the data is read from
			*  @param[out] playerIndex
			*    The player index of the player which changed his ready state
			*  @param[out] isReady
			*    The ready state
			*
			*  @return
			*    "true" when reading went fine, otherwise "false"
			*/
			static bool readServerPlayerChangedReadyStateOperationMessageData(const qsf::game::BitStream& bitStream, uint32& playerIndex, bool& isReady);

			/**
			*  @brief
			*    Writes the data of the SERVER_PLAYER_CHANGED_USERDATA operation message to the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream to which the data is written to
			*  @param[in] playerIndex
			*    The player index of the player which changed his ready state
			*  @param[in] userData
			*    The userData value
			*
			*  @return
			*    "true" when writing went fine, otherwise "false"
			*/
			static bool writeServerPlayerChangedUserDataOperationMessageData(qsf::game::BitStream& bitStream, uint32 playerIndex, uint32 userData);

			/**
			*  @brief
			*    Reads the data of the SERVER_PLAYER_CHANGED_USERDATA operation message from the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream from which the data is read from
			*  @param[out] playerIndex
			*    The player index of the player which changed his ready state
			*  @param[out] userData
			*    The userData value
			*
			*  @return
			*    "true" when reading went fine, otherwise "false"
			*/
			static bool readServerPlayerChangedUserDataOperationMessageData(const qsf::game::BitStream& bitStream, uint32& playerIndex, uint32& userData);

			/**
			*  @brief
			*    Writes the data of the CREATE_ENTITY_FROM_PREFAB operation message to the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream to which the data is written to
			*  @param[in] createdEntitiesList
			*    The list of create entity from prefab data items
			*
			*  @return
			*    "true" when writing went fine, otherwise "false"
			*/
			static bool writeCreateEntityFromPrefabOperationMessageData(qsf::game::BitStream& bitStream, const std::vector<EntityCreatedFromPrefabData>& createdEntitiesList, int32 currentTickCount);

			/**
			*  @brief
			*    Reads the data of the CREATE_ENTITY_FROM_PREFAB operation message from the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream from which the data is read from
			*  @param[out] createdEntitiesList
			*    The list of create entity from prefab data items
			*
			*  @return
			*    "true" when reading went fine, otherwise "false"
			*/
			static bool readCreateEntityFromPrefabOperationMessageData(const qsf::game::BitStream& bitStream, std::vector<EntityCreatedFromPrefabData>& createdEntitiesList, int32& currentTickCount);

			/**
			*  @brief
			*    Writes the data of the DESTROY_ENTITY operation message to the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream to which the data is written to
			*  @param[in] destroyedEntityIdsList
			*    The list of destroyed entity Ids
			*
			*  @return
			*    "true" when writing went fine, otherwise "false"
			*/
			static bool writeDestroyEntityOperationMessageData(qsf::game::BitStream& bitStream, const std::vector<uint64>& destroyedEntityIdsList, int32 currentTickCount);

			/**
			*  @brief
			*    Reads the data of the DESTROY_ENTITY operation message from the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream from which the data is read from
			*  @param[in] destroyedEntityIdsList
			*    The list of destroyed entity Ids
			*
			*  @return
			*    "true" when reading went fine, otherwise "false"
			*/
			static bool readDestroyEntityOperationMessageData(const qsf::game::BitStream& bitStream, std::vector<uint64>& destroyedEntityIdsList, int32& currentTickCount);

			/**
			*  @brief
			*    Writes the data of the UNIT_SPAWNED operation message to the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream to which the data is written to
			*  @param[in] spawnedUnitDataList
			*    The list of data about spawned units (Currently only the data from the commandable component)
			*
			*  @return
			*    "true" when writing went fine, otherwise "false"
			*/
			static bool writeUnitSpawnedOperationMessageData(qsf::game::BitStream& bitStream, const std::vector<UnitSpawnedData>& spawnedUnitDataList, int32 currentTickCount);

			/**
			*  @brief
			*    Reads the data of the UNIT_SPAWNED operation message from the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream from which the data is read from
			*  @param[in] spawnedUnitDataList
			*     The list of data about spawned units (Currently only the data from the commandable component)
			*
			*  @return
			*    "true" when reading went fine, otherwise "false"
			*/
			static bool readUnitSpawnedOperationMessageData(const qsf::game::BitStream& bitStream, std::vector<UnitSpawnedData>& spawnedUnitDataList, int32& currentTickCount);

			/**
			*  @brief
			*    Writes the data of the UNIT_SPAWNED operation message to the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream to which the data is written to
			*  @param[in] spawnedUnitDataList
			*    The list of data about spawned units (Currently only the data from the commandable component)
			*
			*  @return
			*    "true" when writing went fine, otherwise "false"
			*/
			static bool writeUnitSpawnedPlayerUpdateSelectionOperationMessageData(qsf::game::BitStream& bitStream, const std::vector<UnitSpawnedPlayerUpdateSelectionData>& spawnedUnitPlayerUpdateSelectionDataList, int32 currentTickCount);

			/**
			*  @brief
			*    Reads the data of the UNIT_SPAWNED operation message from the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream from which the data is read from
			*  @param[in] spawnedUnitDataList
			*     The list of data about spawned units (Currently only the data from the commandable component)
			*
			*  @return
			*    "true" when reading went fine, otherwise "false"
			*/
			static bool readUnitSpawnedPlayerUpdateSelectionOperationMessageData(const qsf::game::BitStream& bitStream, std::vector<UnitSpawnedPlayerUpdateSelectionData>& spawnedUnitPlayerUpdateSelectionDataList, int32& currentTickCount);

			/**
			*  @brief
			*    Writes the data of the CLIENT_ORDERED_UNIT operation message to the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream to which the data is written to
			*  @param[in] orderedUnitData
			*    The ordered unit data
			*
			*  @return
			*    "true" when writing went fine, otherwise "false"
			*/
			static bool writeClientOrderedUnitOperationMessageData(qsf::game::BitStream& bitStream, const OrderUnitData& orderedUnitData);

			/**
			*  @brief
			*    Reads the data of the CLIENT_ORDERED_UNIT operation message from the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream from which the data is read from
			*  @param[out] orderedUnitData
			*    The ordered unit data
			*
			*  @return
			*    "true" when reading went fine, otherwise "false"
			*/
			static bool readClientOrderedUnitOperationMessageData(const qsf::game::BitStream& bitStream, OrderUnitData& orderedUnitData);

			/**
			*  @brief
			*    Writes the data of the CLIENT_COMMAND_EXECUTION operation message to the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream to which the data is written to
			*  @param[in] commandData
			*    The command data
			*
			*  @return
			*    "true" when writing went fine, otherwise "false"
			*/
			static bool writeClientCommandExecutionOperationMessageData(qsf::game::BitStream& bitStream, const CommandExecutionData& commandData);

			/**
			*  @brief
			*    Reads the data of the CLIENT_COMMAND_EXECUTION operation message from the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream from which the data is read from
			*  @param[out] commandData
			*    The command data
			*
			*  @return
			*    "true" when reading went fine, otherwise "false"
			*/
			static bool readClientCommandExecutionOperationMessageData(const qsf::game::BitStream& bitStream, CommandExecutionData& commandData);

			/**
			*  @brief
			*    Writes the data of the SERVER_COMMAND_EXECUTION_RESPONSE operation message to the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream to which the data is written to
			*  @param[in] commandData
			*    The command result data
			*
			*  @return
			*    "true" when writing went fine, otherwise "false"
			*/
			static bool writeServerCommandExecutionResponseOperationMessageData(qsf::game::BitStream& bitStream, const CommandExecutionResult& commandData);

			/**
			*  @brief
			*    Reads the data of the SERVER_COMMAND_EXECUTION_RESPONSE operation message from the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream from which the data is read from
			*  @param[out] commandData
			*    The command result data
			*
			*  @return
			*    "true" when reading went fine, otherwise "false"
			*/
			static bool readServerCommandExecutionResponseOperationMessageData(const qsf::game::BitStream& bitStream, CommandExecutionResult& commandData);

			/**
			*  @brief
			*    Writes the data of the FREEPLAY_EVENT_UPDATE operation message to the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream to which the data is written to
			*  @param[in] updatedFreeplayEventList
			*    The list of data about changed freeplay events
			*
			*  @return
			*    "true" when writing went fine, otherwise "false"
			*/
			static bool writeFreeplayEventUpdateOperationMessageData(qsf::game::BitStream& bitStream, const std::unordered_map<uint32, FreeplayEventData>& updatedFreeplayEventList, int32 currentTickCount);

			/**
			*  @brief
			*    Reads the data of the FREEPLAY_EVENT_UPDATE operation message from the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream from which the data is read from
			*  @param[in] updatedFreeplayEventList
			*    The list of data about changed freeplay events
			*
			*  @return
			*    "true" when reading went fine, otherwise "false"
			*/
			static bool readFreeplayEventUpdateOperationMessageData(const qsf::game::BitStream& bitStream, std::vector<FreeplayEventData>& updatedFreeplayEventList, int32& currentTickCount);

			/**
			*  @brief
			*    Writes the data of the CHAT_MESSAGE operation message to the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream to which the data is written to
			*  @param[in] chatMessage
			*    The chat message data
			*
			*  @return
			*    "true" when writing went fine, otherwise "false"
			*/
			static bool writeChatMessageOperationMessageData(qsf::game::BitStream& bitStream, const SessionBase::ChatMessage& chatMessage);

			/**
			*  @brief
			*    Reads the data of the CHAT_MESSAGE operation message from the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream from which the data is read from
			*  @param[out] chatMessage
			*    The chat message data
			*
			*  @return
			*    "true" when reading went fine, otherwise "false"
			*/
			static bool readChatMessageOperationMessageData(const qsf::game::BitStream& bitStream, SessionBase::ChatMessage& chatMessage);

			/**
			*  @brief
			*    Writes the data of the PLAYER_SCORE_UPDATE operation message to the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream to which the data is written to
			*  @param[in] playerScoreList
			*    The list of changed player scores (key = player index, value = score)
			*
			*  @return
			*    "true" when writing went fine, otherwise "false"
			*/
			static bool writePlayerScoreChangedOperationMessageData(qsf::game::BitStream& bitStream, const boost::container::flat_map<uint32, uint32>& playerScoreList, int32 currentTickCount);

			/**
			*  @brief
			*    Reads the data of the PLAYER_SCORE_UPDATE operation message from the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream from which the data is read from
			*  @param[out] playerScoreList
			*    The list of changed player scores (key = player index, value = score)
			*
			*  @return
			*    "true" when reading went fine, otherwise "false"
			*/
			static bool readPlayerScoreChangedOperationMessageData(const qsf::game::BitStream& bitStream, boost::container::flat_map<uint32, uint32>& playerScoreList, int32& currentTickCount);

			/**
			*  @brief
			*    Writes the data of the GAME_MODE_UPDATE operation message to the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream to which the data is written to
			*  @param[in] gameModeUpdateData
			*    The game mode update data
			*
			*  @return
			*    "true" when writing went fine, otherwise "false"
			*/
			static bool writeGameModeUpdateOperationMessageData(qsf::game::BitStream& bitStream, const GameModeUpdateData& gameModeUpdateData, int32 currentTickCount);

			/**
			*  @brief
			*    Reads the data of the GAME_MODE_UPDATE operation message from the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream from which the data is read from
			*  @param[out] gameModeUpdateData
			*    The game mode update data
			*
			*  @return
			*    "true" when reading went fine, otherwise "false"
			*/
			static bool readGameModeUpdateOperationMessageData(const qsf::game::BitStream& bitStream, GameModeUpdateData& gameModeUpdateData, int32& currentTickCount);

			/**
			*  @brief
			*    Writes the data of the NEW_HINTS operation message to the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream to which the data is written to
			*  @param[in] newHintList
			*    The list of new hints
			*
			*  @return
			*    "true" when writing went fine, otherwise "false"
			*/
			static bool writeNewHintsOperationMessageData(qsf::game::BitStream& bitStream, const std::vector<NewHintData>& newHintList, int32 currentTickCount);

			/**
			*  @brief
			*    Reads the data of the NEW_HINTS operation message from the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream from which the data is read from
			*  @param[out] newHintList
			*    The list of new hints
			*
			*  @return
			*    "true" when reading went fine, otherwise "false"
			*/
			static bool readNewHintsOperationMessageData(const qsf::game::BitStream& bitStream, std::vector<NewHintData>& newHintList, int32& currentTickCount);

			/**
			*  @brief
			*    Writes the data of the MINIMAP_MESSAGES operation message to the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream to which the data is written to
			*  @param[in] minimapMessageList
			*    The list of minimap related messages
			*
			*  @return
			*    "true" when writing went fine, otherwise "false"
			*/
			static bool writeMinimapMessagesOperationMessageData(qsf::game::BitStream& bitStream, const std::vector<MinimapMessageData>& minimapMessageList, int32 currentTickCount);

			/**
			*  @brief
			*    Reads the data of the MINIMAP_MESSAGES operation message from the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream from which the data is read from
			*  @param[out] minimapMessageList
			*    The list of minimap related messages
			*
			*  @return
			*    "true" when reading went fine, otherwise "false"
			*/
			static bool readMinimapMessagesOperationMessageData(const qsf::game::BitStream& bitStream, std::vector<MinimapMessageData>& minimapMessageList, int32& currentTickCount);

			/**
			*  @brief
			*    Writes the data of the MINIMAP_ICON_MESSAGES operation message to the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream to which the data is written to
			*  @param[in] minimapMessageList
			*    The list of minimap icon messages
			*
			*  @return
			*    "true" when writing went fine, otherwise "false"
			*/
			static bool writeMinimapIconMessagesOperationMessageData(qsf::game::BitStream& bitStream, const std::vector<MinimapIconMessageData>& minimapMessageList, int32 currentTickCount);

			/**
			*  @brief
			*    Reads the data of the MINIMAP_ICON_MESSAGES operation message from the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream from which the data is read from
			*  @param[out] minimapMessageList
			*    The list of minimap icon messages
			*
			*  @return
			*    "true" when reading went fine, otherwise "false"
			*/
			static bool readMinimapIconMessagesOperationMessageData(const qsf::game::BitStream& bitStream, std::vector<MinimapIconMessageData>& minimapMessageList, int32& currentTickCount);

			/**
			*  @brief
			*    Writes the data of the AUDIO_UPDATE operation message to the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream to which the data is written to
			*  @param[in] changedAudioList
			*    The list of audio related messages
			*
			*  @return
			*    "true" when writing went fine, otherwise "false"
			*/
			static bool writeAudioUpdatedMessagesOperationMessageData(qsf::game::BitStream& bitStream, const std::vector<AudioUpdateData>& changedAudioList, int32 currentTickCount);

			/**
			*  @brief
			*    Reads the data of the AUDIO_UPDATE operation message from the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream from which the data is read from
			*  @param[out] changedAudioList
			*    The list of audio related messages
			*
			*  @return
			*    "true" when reading went fine, otherwise "false"
			*/
			static bool readAudioUpdatedOperationMessageData(const qsf::game::BitStream& bitStream, std::vector<AudioUpdateData>& changedAudioList, int32& currentTickCount);

			/**
			*  @brief
			*    Writes the data of the LAYER_CHANGED operation message to the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream to which the data is written to
			*  @param[in] changedLayerList
			*    The list of changed layers
			*
			*  @return
			*    "true" when writing went fine, otherwise "false"
			*/
			static bool writeLayerChangedOperationMessageData(qsf::game::BitStream& bitStream, const std::vector<LayerChangedData>& changedLayerList, int32 currentTickCount);

			/**
			*  @brief
			*    Reads the data of the LAYER_CHANGED operation message from the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream from which the data is read from
			*  @param[out] changedAudioList
			*    The list of audio related messages
			*
			*  @return
			*    "true" when reading went fine, otherwise "false"
			*/
			static bool readLayerChangedOperationMessageData(const qsf::game::BitStream& bitStream, std::vector<LayerChangedData>& changedLayerList, int32& currentTickCount);

			/**
			*  @brief
			*    Writes the data of the FIREHOSE_UPDATE operation message to the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream to which the data is written to
			*  @param[in] newFirehosesList
			*    The list of new fire hoses
			*
			*  @return
			*    "true" when writing went fine, otherwise "false"
			*/
			static bool writeNewFireHoseOperationMessageData(qsf::game::BitStream& bitStream, const std::vector<CreateFireHoseData>& newFirehosesList, int32 currentTickCount);

			/**
			*  @brief
			*    Reads the data of the FIREHOSE_UPDATE operation message from the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream from which the data is read from
			*  @param[in] newFirehosesList
			*    The list of new fire hoses
			*
			*  @return
			*    "true" when reading went fine, otherwise "false"
			*/
			static bool readNewFireHoseOperationMessageData(const qsf::game::BitStream& bitStream, std::vector<CreateFireHoseData>& newFirehosesList, int32& currentTickCount);

			/**
			*  @brief
			*    Writes the data of the SPAWN_PARTICLE operation message to the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream to which the data is written to
			*  @param[in] spawnParticleList
			*    The list of newly spawned particles
			*
			*  @return
			*    "true" when writing went fine, otherwise "false"
			*/
			static bool writeSpawnParticleOperationMessageData(qsf::game::BitStream& bitStream, const std::vector<SpawnParticleData>& spawnParticleList, int32 currentTickCount);

			/**
			*  @brief
			*    Reads the data of the SPAWN_PARTICLE operation message from the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream from which the data is read from
			*  @param[in] spawnParticleList
			*    The list of newly spawned particles
			*
			*  @return
			*    "true" when reading went fine, otherwise "false"
			*/
			static bool readSpawnParticleOperationMessageData(const qsf::game::BitStream& bitStream, std::vector<SpawnParticleData>& spawnParticleList, int32& currentTickCount);

			/**
			*  @brief
			*    Writes the data of the CLIENT_CAMERA_POSITION operation message to the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream to which the data is written to
			*  @param[in] position
			*    The position of the players camera
			*
			*  @return
			*    "true" when writing went fine, otherwise "false"
			*/
			static bool writeClientCameraPositionOperationMessageData(qsf::game::BitStream& bitStream, const glm::vec3& position);

			/**
			*  @brief
			*    Reads the data of the CLIENT_CAMERA_POSITION operation message from the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream from which the data is read from
			*  @param[in] position
			*    The position of the players camera
			*
			*  @return
			*    "true" when reading went fine, otherwise "false"
			*/
			static bool readClientCameraPositionOperationMessageData(const qsf::game::BitStream& bitStream, glm::vec3& position);

			/**
			*  @brief
			*    Writes the data of the SERVER_PING operation message to the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream to which the data is written to
			*  @param[in] pingData
			*    The server ping data
			*
			*  @return
			*    "true" when writing went fine, otherwise "false"
			*/
			static bool writeServerPingOperationMessageData(qsf::game::BitStream& bitStream, const ServerPingData& pingData);

			/**
			*  @brief
			*    Reads the data of the SERVER_PING operation message from the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream from which the data is read from
			*  @param[in] pingData
			*    The server ping data
			*
			*  @return
			*    "true" when reading went fine, otherwise "false"
			*/
			static bool readServerPingOperationMessageData(const qsf::game::BitStream& bitStream, ServerPingData& pingData);

			/**
			*  @brief
			*    Writes the data of the CLIENT_PING_ACKNOWLEDGE operation message to the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream to which the data is written to
			*  @param[in] pingAcknowledgeData
			*    The client ping acknowledge data
			*
			*  @return
			*    "true" when writing went fine, otherwise "false"
			*/
			static bool writeClientPingAcknowledgeOperationMessageData(qsf::game::BitStream& bitStream, const ClientPingAcknowledgeData& pingAcknowledgeData);

			/**
			*  @brief
			*    Reads the data of the CLIENT_PING_ACKNOWLEDGE operation message from the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream from which the data is read from
			*  @param[in] pingAcknowledgeData
			*    The client ping acknowledge data
			*
			*  @return
			*    "true" when reading went fine, otherwise "false"
			*/
			static bool readClientPingAcknowledgeOperationMessageData(const qsf::game::BitStream& bitStream, ClientPingAcknowledgeData& pingAcknowledgeData);

			/**
			*  @brief
			*    Writes the data of the FADE_ENTITY operation message to the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream to which the data is written to
			*  @param[in] fadeEntityDataList
			*    A list of fade entity data
			*
			*  @return
			*    "true" when writing went fine, otherwise "false"
			*/
			static bool writeFadeEntityOperationMessageData(qsf::game::BitStream& bitStream, const std::vector<FadeEntityData>& fadeEntityDataList, int32 currentTickCount);

			/**
			*  @brief
			*    Reads the data of the FADE_ENTITY operation message from the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream from which the data is read from
			*  @param[in] fadeEntityDataList
			*    A list of fade entity data
			*
			*  @return
			*    "true" when reading went fine, otherwise "false"
			*/
			static bool readFadeEntityOperationMessageData(const qsf::game::BitStream& bitStream, std::vector<FadeEntityData>& fadeEntityDataList, int32& currentTickCount);

			/**
			*  @brief
			*    Writes the data of the MINIMAP_PING operation message to the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream to which the data is written to
			*  @param[in] minimapPingData
			*    Minimap ping data
			*
			*  @return
			*    "true" when writing went fine, otherwise "false"
			*/
			static bool writeMinimapPingOperationMessageData(qsf::game::BitStream& bitStream, const MinimapPingData& minimapPingData, int32 currentTickCount);

			/**
			*  @brief
			*    Reads the data of the MINIMAP_PING operation message from the bit stream
			*
			*  @param[in] bitStream
			*    The target bit stream from which the data is read from
			*  @param[in] minimapPingData
			*    Minimap ping data
			*
			*  @return
			*    "true" when reading went fine, otherwise "false"
			*
			*  @note
			*    This method writes only the payload data (without the operation message id)
			*/
			static bool readMinimapPingOperationMessageData(const qsf::game::BitStream& bitStream, MinimapPingData& minimapPingData, int32& currentTickCount);

			/**
			*  @brief
			*    Write a single operation message data to the bit stream (This writes also the header for a multiplayer "message")
			*
			*  @param[in] bitStream
			*    The target bit stream to which the data is written to
			*  @param[in] operationMessage
			*    The operation message type which was read from the stream
			*  @param[in] operationMessagePayload
			*    The payload data of the operation message (can be empty)
			*
			*  @return
			*    "true" when reading went fine, otherwise "false"
			*/
			static bool writeOperationMessageAsSingleMessage(qsf::game::BitStream& bitStream, const OperationMessage& operationMessage, const qsf::game::BitStream& operationMessagePayload);

			/**
			*  @brief
			*    Write a single operation message data to the bit stream (This writes also the header for a multiplayer "message")
			*
			*  @param[in] bitStream
			*    The target bit stream to which the data is written to
			*  @param[in] operationMessage
			*    The operation message type which was read from the stream
			*
			*  @return
			*    "true" when reading went fine, otherwise "false"
			*/
			static bool writeOperationMessageWithoutAdditionalPayloadAsSingleMessage(qsf::game::BitStream& bitStream, const OperationMessage& operationMessage);


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5
