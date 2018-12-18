// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"
#include "em5/network/multiplayer_proxy/MultiplayerProxyTypes.h"


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
		namespace proxy
		{


			//[-------------------------------------------------------]
			//[ Classes                                               ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    EMERGENCY 5 multiplayer proxy protocol helper
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
				*    Writes the multiplayer proxy protocol header to the given bit stream
				*    The header consist of following parts:
				*    - An Identifier string of 6 chars (EM5MPP)
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
				*    Reads the multiplayer proxy protocol header from the given stream and does some validation checks
				*    The header consist of following parts:
				*    - An Identifier string of 6 chars (EM5MPP)
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
				*    Write an message data to the bit stream
				*
				*  @param[in] bitStream
				*    The target bit stream to which the data is written to
				*  @param[in] messageType
				*    The message type to be written to the stream
				*  @param[in] messageData
				*    The payload data of the message (can be empty)
				*
				*  @return
				*    "true" when reading went fine, otherwise "false"
				*/
				static bool writeMessage(qsf::game::BitStream& bitStream, const MessageType& messageType, const qsf::game::BitStream& messageData);

				/**
				*  @brief
				*    Reads message type and data size from the bit stream
				*
				*  @param[in] bitStream
				*    The source bit stream from which the data is read from
				*  @param[out] messageType
				*    The message type which was read from the stream
				*  @param[out] dataSize
				*    The size of the message data
				*
				*  @return
				*    "true" when reading went fine, otherwise "false"
				*/
				static bool readMessageTypeAndDataSize(const qsf::game::BitStream& bitStream, MessageType& messageType, uint32& dataSize);

				/**
				*  @brief
				*    Reads an message data from the bit stream
				*
				*  @param[in] bitStream
				*    The source bit stream from which the data is read from
				*  @param[out] operationMessage
				*    The operation message type which was read from the stream
				*  @param[out] messageData
				*    The payload data of the message (can be empty)
				*
				*  @return
				*    "true" when reading went fine, otherwise "false"
				*/
				static bool readMessage(const qsf::game::BitStream& bitStream, MessageType& messageType, qsf::game::BitStream& messageData);

				/**
				*  @brief
				*    Write a single message data to the bit stream (This writes also the header for a multiplayer proxy "message")
				*
				*  @param[in] bitStream
				*    The target bit stream to which the data is written to
				*  @param[in] messageType
				*    The message type which was read from the stream
				*  @param[in] messagePayload
				*    The payload data of the message (can be empty)
				*
				*  @return
				*    "true" when reading went fine, otherwise "false"
				*/
				static bool writeMessageAsSingleMessage(qsf::game::BitStream& bitStream, const MessageType& messageType, const qsf::game::BitStream& messagePayload);

				/**
				*  @brief
				*    Write a single message data to the bit stream (This writes also the header for a multiplayer proxy "message")
				*
				*  @param[in] bitStream
				*    The target bit stream to which the data is written to
				*  @param[in] messageType
				*    The message type which was read from the stream
				*
				*  @return
				*    "true" when reading went fine, otherwise "false"
				*/
				static bool writeMessageAsSingleMessageWithoutData(qsf::game::BitStream& bitStream, const MessageType& messageType);

				/**
				*  @brief
				*    Writes the data of the CREATESESSION message to the bit stream
				*
				*  @param[in] bitStream
				*    The target bit stream to which the data is written to
				*  @param[in] createSessionData
				*     The create session data
				*
				*  @return
				*    "true" when writing went fine, otherwise "false"
				*/
				static bool writeCreateSessionMessageData(qsf::game::BitStream& bitStream, const CreateSessionData& createSessionData);

				/**
				*  @brief
				*    Reads the data of the CREATESESSION message from the bit stream
				*
				*  @param[in] bitStream
				*    The target bit stream from which the data is read from
				*  @param[out] createSessionData
				*     The create session data
				*
				*  @return
				*    "true" when reading went fine, otherwise "false"
				*/
				static bool readCreateSessionMessageData(const qsf::game::BitStream& bitStream, CreateSessionData& createSessionData);

				/**
				*  @brief
				*    Writes the data of the CREATESESSION_RESPONSE message to the bit stream
				*
				*  @param[in] bitStream
				*    The target bit stream to which the data is written to
				*  @param[in] createSessionData
				*     The create session response data
				*
				*  @return
				*    "true" when writing went fine, otherwise "false"
				*/
				static bool writeCreateSessionResponseMessageData(qsf::game::BitStream& bitStream, const CreateSessionResponseData& createSessionResponseData);

				/**
				*  @brief
				*    Reads the data of the CREATESESSION_RESPONSE message from the bit stream
				*
				*  @param[in] bitStream
				*    The target bit stream from which the data is read from
				*  @param[out] createSessionResponseData
				*     The create session response data
				*
				*  @return
				*    "true" when reading went fine, otherwise "false"
				*/
				static bool readCreateSessionResponseMessageData(const qsf::game::BitStream& bitStream, CreateSessionResponseData& createSessionResponseData);

				/**
				*  @brief
				*    Writes the data of the CONNECTTOSESSION message to the bit stream
				*
				*  @param[in] bitStream
				*    The target bit stream to which the data is written to
				*  @param[in] connectToSessionData
				*     The connect to session data
				*
				*  @return
				*    "true" when writing went fine, otherwise "false"
				*/
				static bool writeConnectToSessionMessageData(qsf::game::BitStream& bitStream, const ConnectToSessionData& connectToSessionData);

				/**
				*  @brief
				*    Reads the data of the CONNECTTOSESSION message from the bit stream
				*
				*  @param[in] bitStream
				*    The target bit stream from which the data is read from
				*  @param[out] connectToSessionData
				*     The connect to session data
				*
				*  @return
				*    "true" when reading went fine, otherwise "false"
				*/
				static bool readConnectToSessionMessageData(const qsf::game::BitStream& bitStream, ConnectToSessionData& connectToSessionData);

				/**
				*  @brief
				*    Writes the data of the CONNECTTOSESSION_RESPONCSE message to the bit stream
				*
				*  @param[in] bitStream
				*    The target bit stream to which the data is written to
				*  @param[in] connectToSessionResponseData
				*     The connect to session response data
				*
				*  @return
				*    "true" when writing went fine, otherwise "false"
				*/
				static bool writeConnectToSessionResponseMessageData(qsf::game::BitStream& bitStream, const ConnectToSessionResponseData& connectToSessionResponseData);

				/**
				*  @brief
				*    Reads the data of the CONNECTTOSESSION_RESPONSE message from the bit stream
				*
				*  @param[in] bitStream
				*    The target bit stream from which the data is read from
				*  @param[out] connectToSessionResponseData
				*     The connect to session response data
				*
				*  @return
				*    "true" when reading went fine, otherwise "false"
				*/
				static bool readConnectToSessionResponseMessageData(const qsf::game::BitStream& bitStream, ConnectToSessionResponseData& connectToSessionResponseData);

				/**
				*  @brief
				*    Writes the data of the CLIENT_CONNECTED_TO_SESSION message to the bit stream
				*
				*  @param[in] bitStream
				*    The target bit stream to which the data is written to
				*  @param[in] clientConnectedToSessionData
				*     The client connected to session response data
				*
				*  @return
				*    "true" when writing went fine, otherwise "false"
				*/
				static bool writeClientConnectedToSessionMessageData(qsf::game::BitStream& bitStream, const ClientConnectedToSessionData& clientConnectedToSessionData);

				/**
				*  @brief
				*    Reads the data of the CLIENT_CONNECTED_TO_SESSION message from the bit stream
				*
				*  @param[in] bitStream
				*    The target bit stream from which the data is read from
				*  @param[out] clientConnectedToSessionData
				*     The client connected to session response data
				*
				*  @return
				*    "true" when reading went fine, otherwise "false"
				*/
				static bool readClientConnectedToSessionMessageData(const qsf::game::BitStream& bitStream, ClientConnectedToSessionData& clientConnectedToSessionData);

				/**
				*  @brief
				*    Writes the data of the CLIENT_DISCONNECT_FROM_SESSSION message to the bit stream
				*
				*  @param[in] bitStream
				*    The target bit stream to which the data is written to
				*  @param[in] clientDisconnectedFromSessionData
				*     The client disconnected from session response data
				*
				*  @return
				*    "true" when writing went fine, otherwise "false"
				*/
				static bool writeClientDisconnectFromSessionMessageData(qsf::game::BitStream& bitStream, const ClientDisconnectedFromSessionData& clientDisconnectedFromSessionData);

				/**
				*  @brief
				*    Reads the data of the CLIENT_DISCONNECT_FROM_SESSSION message from the bit stream
				*
				*  @param[in] bitStream
				*    The target bit stream from which the data is read from
				*  @param[out] clientDisconnectedFromSessionData
				*     The client disconnected from session response data
				*
				*  @return
				*    "true" when reading went fine, otherwise "false"
				*/
				static bool readClientDisconnectFromSessionMessageData(const qsf::game::BitStream& bitStream, ClientDisconnectedFromSessionData& clientDisconnectedFromSessionData);

				/**
				*  @brief
				*    Writes the data of the DATA message to the bit stream
				*
				*  @param[in] bitStream
				*    The target bit stream to which the data is written to
				*  @param[in] data
				*     The data
				*
				*  @return
				*    "true" when writing went fine, otherwise "false"
				*/
				static bool writeDataMessageData(qsf::game::BitStream& bitStream, const DataData& data);

				/**
				*  @brief
				*    Writes the data of the DATA message to the bit stream
				*
				*  @param[in] bitStream
				*    The target bit stream to which the data is written to
				*  @param[in] proxySessionClientId
				*     The per session id of the proxy client to which the data should be send
				*  @param[in] data
				*     The data to be send
				*
				*  @return
				*    "true" when writing went fine, otherwise "false"
				*/
				static bool writeDataMessageData(qsf::game::BitStream& bitStream, const uint8 proxySessionClientId, const qsf::game::BitStream& data);

				/**
				*  @brief
				*    Reads the data of the DATA message from the bit stream
				*
				*  @param[in] bitStream
				*    The target bit stream from which the data is read from
				*  @param[out] data
				*     The data
				*
				*  @return
				*    "true" when reading went fine, otherwise "false"
				*/
				static bool readDataMessageData(const qsf::game::BitStream& bitStream, DataData& data);

				/**
				*  @brief
				*    Reads only the metadata of the DATA message from the bit stream
				*
				*  @param[in] bitStream
				*    The target bit stream from which the data is read from
				*  @param[out] data
				*     The data
				*
				*  @return
				*    "true" when reading went fine, otherwise "false"
				*/
				static bool readMetaDataOfDataMessageData(const qsf::game::BitStream& bitStream, DataData& data);

				static bool updateProxySessionClientIdInDataMessageData(qsf::game::BitStream& bitStream);


			};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // proxy
	} // multiplayer
} // em5
