// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/network/multiplayer/client/HostBase.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{
		class Client;
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
			*    EMERGENCY 5 multiplayer network host (client side multiplayer host instance)
			*/
			class EM5_API_EXPORT NetworkHost : public HostBase
			{


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Constructor
				*/
				explicit NetworkHost();

				/**
				*  @brief
				*    Destructor
				*/
				virtual ~NetworkHost();


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
				void onGameClientConnected();
				void onGameClientDisconnected();
				void onGameClientUnexpectedDisconnected();
				void onGameClientConnectFailed();
				void onGameClientPacketReceived(const qsf::game::BitStream& bitStream, uint32 sequenceNumber);


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
			private:
				qsf::game::Client*	mClientConnection;


			};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // client
	} // multiplayer
} // em5
