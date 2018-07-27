// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"
#include "em5/network/multiplayer_proxy/MultiplayerProxyTypes.h"

#include <qsf_game/network/Client.h>


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
			*    EMERGENCY 5 multiplayer proxy client class
			*/
			class MultiplayerProxyClient : public qsf::game::Client
			{


			//[-------------------------------------------------------]
			//[ Public boost signals                                  ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Called when data is received from the proxy
				*
				*  @param[in] messageData
				*    The data of the message itself
				*
				*  @note
				*    - The handler should not store the given message instance via pointer or reference, because when the handler method has exited it cannot be guaranteed that the instance will still be valid
				*    - Recommended slot signature: void onReceivedMessage(const em5::multiplayer::proxy::DataData& messageData)
				*    - When more then one handler is registered then the handlers should make sure that the streamposition is resetted to the position as on the function entry
				*/
				boost::signals2::signal<void(const DataData&)> receivedData;

				boost::signals2::signal<void(CreateSessionResultType, uint64)> sessionCreated;
				boost::signals2::signal<void(ConnectionResultType)> connectedToSession;
				boost::signals2::signal<void(uint8)> clientConnectedToSession;
				boost::signals2::signal<void(uint8)> clientDisconnectedFromSession;


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				MultiplayerProxyClient();

				bool connect(const std::string& hostOrIpAddress, uint16 port);
				void disconnect();

				void createSession();
				void disconnectProxyClientFromSession(uint8 proxySessionClientId);

				bool connectToSession(uint64 sessionId);
				void disconnectFromCurrentSession();

				bool sendData(uint8 proxyClientSessionId, const qsf::game::BitStream& messageData);

				bool isSessionCreated() const;
				bool isConnectedToSession() const;
				uint64 getSessionId() const;
				uint8 getProxySessionClientId() const;


			//[-------------------------------------------------------]
			//[ Public virtual qsf::Client methods                    ]
			//[-------------------------------------------------------]
			public:
				virtual void onConnected() override;
				virtual void onDisconnected() override;
				virtual void onUnexpectedDisconnected() override;
				virtual void onConnectFailed() override;


			//[-------------------------------------------------------]
			//[ Protected virtual qsf::game::Client methods           ]
			//[-------------------------------------------------------]
			protected:
				virtual void onPacketReceived(const qsf::game::BitStream& bitStream, uint32 sequenceNumber) override;


			//[-------------------------------------------------------]
			//[ Private methods                                       ]
			//[-------------------------------------------------------]
			private:
				void splitReceivedMessageIntoMessages(const qsf::game::BitStream& bitStream, uint32 messageSize);
				void handleMessage(const MessageType messageType, uint32 dataSize, const qsf::game::BitStream& bitStream);

				void resetStoredData();


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
			private:
				uint64	mSessionId;
				uint8	mProxySessionClientId;
				bool	mIsSessionCreator;


			};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // proxy
	} // multiplayer
} // em5
