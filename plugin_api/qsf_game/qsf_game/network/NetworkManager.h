// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/Export.h"

// TODO(ca) Can't include qsf::Manager class here because of "warning C4275: non dll-interface class 'boost::noncopyable_::noncopyable' used as base for dll-interface class 'qsf::Manager'", although this seems to work in qsf::editor::base::OperationManager - what's going on?
//#include <qsf/base/manager/Manager.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{
		class Server;
		class Client;
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
		*    Game network manager class
		*/
		class QSF_GAME_API_EXPORT NetworkManager // : public Manager
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			NetworkManager();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~NetworkManager();

			//[-------------------------------------------------------]
			//[ Lifecycle                                             ]
			//[-------------------------------------------------------]
			virtual bool onStartup();
			virtual void onShutdown();

			//[-------------------------------------------------------]
			//[ Server                                                ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Creates a new server object and tries to listen on the specified port
			*
			*  @param[in] port
			*    Port number the server should listen on
			*
			*  @return
			*    The newly created server object or null pointer on error
			*/
			Server* createServer(uint16 port);

			/**
			*  @brief
			*    Returns the previously created server object or null pointer, if there is none
			*
			*  @return
			*    The previously created server object or null pointer, if there is none
			*/
			Server* getServer() const;

			/**
			*  @brief
			*    Shuts down the server, closes all connections and destroys the instance
			*/
			void closeServer();

			//[-------------------------------------------------------]
			//[ Client                                                ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Creates a new client object and tries to connect to a server listening at the specified hostname and port
			*
			*  @param[in] hostname
			*    Host name of the server to connect to (can be an IP address, a windows host name or a domain)
			*  @param[in] port
			*    Port number of the server to connect to
			*
			*  @return
			*    The newly created client object or null pointer on error
			*/
			Client* connectToServer(const std::string& hostname, uint16 port);

			/**
			*  @brief
			*    Returns the stored client object or null pointer, if there is none
			*
			*  @return
			*    The stored client object or null pointer, if there is none
			*/
			Client* getClient() const;

			/**
			*  @brief
			*    Closes the client connection and destroys the instance
			*/
			void closeClient();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			Server* mServer;
			Client* mClient;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf
