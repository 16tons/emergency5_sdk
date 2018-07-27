// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"
#include "em5/network/multiplayer/client/HostBase.h"
#include "em5/network/multiplayer_proxy/MultiplayerProxyTypes.h"


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
		namespace client
		{


			//[-------------------------------------------------------]
			//[ Classes                                               ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    EMERGENCY 5 multiplayer network host via proxy (client side multiplayer host instance)
			*/
			class EM5_API_EXPORT ProxyHost : public HostBase
			{


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Constructor
				*/
				explicit ProxyHost(proxy::MultiplayerProxyClient& connection, uint64 sessionId);

				/**
				*  @brief
				*    Destructor
				*/
				virtual ~ProxyHost();


			//[-------------------------------------------------------]
			//[ Public virtual em5::multiplayer::client::HostBase methods ]
			//[-------------------------------------------------------]
			public:
				virtual bool connect(const std::string& hostOrIpAddress, uint16 port) override;
				virtual bool sendData(const qsf::game::BitStream& bitStream) override;
				virtual void shutdown() override;


			//[-------------------------------------------------------]
			//[ Private methods                                       ]
			//[-------------------------------------------------------]
			private:
				void onGameClientPacketReceived(const proxy::DataData& data);
				void onConnectedToSession(proxy::ConnectionResultType result);
				void onDisconnected();


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
			private:
				proxy::MultiplayerProxyClient&	mClientConnection;
				uint64							mProxySessionId; //< Holds the session id to which the proxy host should connect to

				boost::signals2::connection	mReceivedDataConnection;
				boost::signals2::connection	mConnectedConnection;
				boost::signals2::connection	mDisconnectedConnection;


			};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // client
	} // multiplayer
} // em5
