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
namespace qsf
{
	namespace game
	{
		class ServerConnection;
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
			*    EMERGENCY 5 multiplayer network client (server side multiplayer client instance)
			*/
			class EM5_API_EXPORT NetworkClient : public ClientBase
			{


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
				explicit NetworkClient(qsf::game::ServerConnection& serverConnection);

				/**
				*  @brief
				*    Destructor
				*/
				virtual ~NetworkClient();


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
				void onServerClientPacketReceived(const qsf::game::BitStream& bitStream, uint32 sequenceNumber);


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
			private:
				qsf::game::ServerConnection&	mServerConnection;
				boost::signals2::connection	mPacketReceivedConnection;


			};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // server
	} // multiplayer
} // em5
