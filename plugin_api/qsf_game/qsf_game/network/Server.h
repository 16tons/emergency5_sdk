// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/Export.h"

#include <qsf/network/Server.h>

#include <boost/signals2/signal.hpp>
#include <boost/container/flat_map.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{
		class NetworkManager;
		class ServerConnection;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Game server class (just a wrapper for linnet server)
		*/
		class QSF_GAME_API_EXPORT Server : public qsf::Server
		{


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
			friend class NetworkManager;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Returns the port this server is listening on
			*
			*  @return
			*    the port this server is listening on
			*/
			uint16 getPort() const;

			/**
			*  @brief
			*    Returns the maximum number of simultaneously connected clients configured in this server (0 = unlimited)
			*
			*  @return
			*    The maximum number of simultaneously connected clients configured in this server (0 = unlimited)
			*/
			uint32 getMaxClients() const;

			/**
			*  @brief
			*    Retrieves the ServerConnection object associated with the given serverConnectionId
			*
			*  @param[in] serverConnectionId
			*    Identifier of the ServerConnection object to retrieve (returned by "qsf::game::ServerConnection::getId()")
			*
			*  @return
			*    a pointer to the ServerConnection object associated with the given serverConnectionId, nullptr on error, do not destroy the instance
			*/
			ServerConnection* getServerConnectionById(uint32 serverConnectionId);


		//[-------------------------------------------------------]
		//[ Public boost signals                                  ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Called when a client is connected
			*
			*  @param[in] serverConnection
			*    The ServerConnection object for the newly connected client
			*
			*  @note
			*    - Recommended slot signature: void onClientConnected(qsf::game::ServerConnection& serverConnection)
			*    - "qsf::game::Server::onClientDisconnected()" will be called later on in case of disconnection
			*/
			boost::signals2::signal<void(ServerConnection&)> clientConnected;

			/**
			*  @brief
			*    Called when a client is disconnected
			*
			*  @param[in] serverConnection
			*    The ServerConnection object for the disconnecting client, do not store the reference, the object will be destroyed after all slots connected to this signal have returned
			*
			*  @note
			*    - Recommended slot signature: void onClientDisconnected(qsf::game::ServerConnection& serverConnection)
			*    - Only called in case there was actually a connection ("qsf::game::Server::onClientConnected()" was called before)
			*/
			boost::signals2::signal<void(ServerConnection&)> clientDisconnected;

			/**
			*  @brief
			*    Called when the server is shut down
			*
			*  @note
			*    - Recommended slot signature: void onServerShutdown()
			*/
			boost::signals2::signal<void()> serverShutdown;


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			/**
			*  @brief
			*    Constructor
			*/
			Server();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~Server();

			bool start(uint16 port);
			bool start(uint16 port, const std::string& address);

			void stop();


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::Server methods                 ]
		//[-------------------------------------------------------]
		protected:
			virtual void onClientConnected(uint32 clientId) override;
			virtual void onClientDisconnected(uint32 clientId) override;
			virtual void onPacketReceived(uint32 clientId, const std::vector<char>& packet) override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			uint16													mPort;
			boost::container::flat_map<uint32, ServerConnection*>	mServerConnectionMap;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf
