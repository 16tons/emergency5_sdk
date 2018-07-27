// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf_game/network/BitStream.h>


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
			//[ Global definitions                                    ]
			//[-------------------------------------------------------]
			enum class MessageType : uint8
			{
				INVALID,
				CREATESESSION,						// Send by a connected client when it wants to create a session
				CREATESESSION_RESPONSE,				// Send to the connected client as response to its create a session request
				CONNECTTOSESSION,					// Send by a connected client when it wants to connect to an existing session
				CONNECTTOSESSION_RESPONSE,			// Send to the connected client as response to its connect to session request
				DISCONNECT_FROM_CURRENT_SESSION,	// Send by a connected client when it wants to disconnect from the current connected session
													// Also send by the proxy when the current session gets terminated
				CLIENT_CONNECTED_TO_SESSION,		// Send to the session creator when an client connects to the session
				CLIENT_DISCONNECT_FROM_SESSSION,	// Send by the session creator when an client should be disconnected from the session.
													// This message is also send by the proxy when the clients disconnects from the proxy while connected to a session or client sends disconnect from current session message
				DATA,								// Send by connected clients which are part of a session
													// When the message comes from the session creator then this data is send to all session clients except the session creator
													// When the message is send from a client which is not the session creator then the data is only send to the session creator
				_NUM_MESSAGES
			};

			enum class ConnectionResultType : uint8
			{
				SUCCESS,
				UNKNOWN_SESSION,
				ALREADY_ASSIGNED_TO_A_SESSION,
				SESSION_FULL
			};

			enum class CreateSessionResultType : uint8
			{
				SUCCESS,
				ALREADY_ASSIGNED_TO_A_SESSION
			};

			/**
			*  @brief
			*    Base struct for proxy messages
			*
			*  @note
			*    This is only used as a common base to have only one method to pass message data around via reference or pointer
			*    DON'T try to delete a message instance via a pointer of this type! The base class has no virtual destructor defined!
			*/
			struct DataBase
			{
				// TODO(sw) Currently empty
			};

			struct CreateSessionData : public DataBase
			{
				// TODO(sw) Currently empty
			};

			struct CreateSessionResponseData : public DataBase
			{
				CreateSessionResultType mResult;
				uint64 mSessionId;
			};

			struct ConnectToSessionData : public DataBase
			{
				uint64 mSessionId;
			};

			struct ConnectToSessionResponseData : public DataBase
			{
				ConnectionResultType mResult;
				uint64 mSessionId;
				uint8 mProxySessionClientId;
			};

			struct ClientConnectedToSessionData : public DataBase
			{
				uint8 mProxySessionClientId;	// The id of the client known by the proxy which connected to a session
			};

			struct ClientDisconnectedFromSessionData : public DataBase
			{
				uint8 mProxySessionClientId;	// The id of the client known by the proxy which disconnected from a session
			};

			struct DataData : public DataBase
			{
				uint8 mProxySessionClientId;	// The id of the client known by the proxy to which the data should be send (qsf::getUninitialized value when all clients should get the data)
												// When the data is send from a client then this value is the id of the sending client
				qsf::game::BitStream dataBitStream;
			};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // proxy
	} // multiplayer
} // em5
