// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"
#include "em5/network/multiplayer/server/ClientBase.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	namespace multiplayer
	{
		namespace proxy
		{
			class MultiplayerProxyClient;
			struct DataData;
		}
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
			*    EMERGENCY 5 multiplayer network client via proxy (server side multiplayer client instance)
			*/
			// TODO(sw) This causes that the same data is send multiple times via the proxy, but it makes the proxy usage for sending/receiving data via proxy transparent to the host
			class EM5_API_EXPORT ProxyClient : public ClientBase
			{


			//[-------------------------------------------------------]
			//[ Public static methods                                 ]
			//[-------------------------------------------------------]
			public:
				static uint64 getIdFromProxySessionClientId(uint8 proxySessionClientId);
				static bool isIdFromAProxyClient(uint64 clientId);


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Constructor
				*
				*  @param[in] serverConnection
				*    The server connection to be used for the communication
				*/
				explicit ProxyClient(proxy::MultiplayerProxyClient& serverConnection, uint8 proxySessionClientId);

				/**
				*  @brief
				*    Destructor
				*/
				virtual ~ProxyClient();


			//[-------------------------------------------------------]
			//[ Public virtual em5::multiplayer::client::ClientBase methods ]
			//[-------------------------------------------------------]
			public:
				virtual void disconnect() override;
				virtual void disconnect(bool force) override;
				virtual uint64 getId() const override;
				virtual bool sendData(const qsf::Clock& simulationClock, const qsf::game::BitStream& bitStream) override;
				virtual bool sendData(const qsf::game::BitStream& bitStream) override;


			//[-------------------------------------------------------]
			//[ Private methods                                       ]
			//[-------------------------------------------------------]
			private:
				void onServerClientPacketReceived(const proxy::DataData& data);


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
			private:
				proxy::MultiplayerProxyClient&	mServerConnection;
				uint8 mProxySessionClientId;
				uint64 mId;

				boost::signals2::connection	mReceivedDataConnection;


			};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // server
	} // multiplayer
} // em5
