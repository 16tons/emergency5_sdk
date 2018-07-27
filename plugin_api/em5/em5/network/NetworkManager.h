// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/network/matchmaking/MatchmakingTypes.h"

#include <qsf_game/network/NetworkManager.h>

#include <qsf/base/UniqueInstance.h>

#include <boost/signals2/signal.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class UpdateCheck;
	class MatchmakingClient;
	class MapIdentifier;
	namespace multiplayer
	{
		class SessionBase;
		namespace server
		{
			class Host;
		}
		namespace client
		{
			class Client;
		}
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


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    EMERGENCY 5 network manager class
	*/
	class EM5_API_EXPORT NetworkManager : public qsf::game::NetworkManager, public qsf::UniqueInstance<NetworkManager>
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint16 MATCHMAKING_SERVER_PORT;
		static const uint16 MULTIPLAYER_PROXY_SERVER_PORT;


	//[-------------------------------------------------------]
	//[ Public boost signals                                  ]
	//[-------------------------------------------------------]
	public:
		boost::signals2::signal<void(multiplayer::SessionBase&)> sessionBaseInstanceCreated;


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns a string identifying the currently running combination of game modifications
		*
		*  @return
		*    A string identifying the currently running combination of game modifications
		*/
		static std::string generateCurrentGameModificationString();


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
		virtual bool onStartup() override;
		virtual void onShutdown() override;

		//[-------------------------------------------------------]
		//[ Matchmaking                                           ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Override the matchmaking url and port
		*
		*  @param[in] matchmakingUrl
		*    The new url for the matchmaking server
		*  @param[in] matchmakingPort
		*    The new port for the matchmaking server
		*
		*  @note
		*    - When this method is called changes from an update check regarding matchmaking server is ignored and always the set values are used!
		*/
		void setMatchmakingUrlAndPort(const std::string& matchmakingUrl, uint16 matchmakingPort);

		/**
		*  @brief
		*    Creates a new MatchmakingClient object and tries to connect to the master matchmaking server listening at the predefined hostname MATCHMAKING_SERVER_HOSTNAME and port MATCHMAKING_SERVER_PORT
		*
		*  @return
		*    The newly created MatchmakingClient object or nullptr on error
		*
		*  @note
		*    - A weak pointer to the returned instance is cached and reused instead of creating a fresh connection as long as it stays valid
		*/
		std::shared_ptr<MatchmakingClient> connectToMatchmakingServer();

		/**
		*  @brief
		*    Returns the stored MatchmakingClient object or nullptr, if there is none
		*
		*  @return
		*    The stored MatchmakingClient object or nullptr, if there is none
		*/
		std::shared_ptr<MatchmakingClient> getMatchmakingClient() const;

		//[-------------------------------------------------------]
		//[ Multiplayer Client                                    ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Creates a new MultiplayerClient object
		*
		*  @return
		*    The newly created MultiplayerClient object or nullptr on error
		*/
		multiplayer::client::Client* createMultiplayerClient();

		/**
		*  @brief
		*    Creates a new MultiplayerClient object and tries to connect to a multiplayer host listening at the specified hostname and port
		*
		*  @param[in] hostname
		*    Host name of the multiplayer host to connect to (can be an IP address, a windows host name or a domain)
		*  @param[in] port
		*    Port number of the multiplayer host to connect to
		*
		*  @return
		*    The newly created MultiplayerClient object or nullptr on error
		*/
		multiplayer::client::Client* connectToMultiplayerHost(const multiplayer::HostEntry& hostEntry, const std::string& playerName, const std::string& sessionPassword);

		/**
		*  @brief
		*    Creates a new MultiplayerClient object with a local host instance (This is only for testing)
		*
		*  @param[in] dataPath
		*    The path where the local client will find the files for the game player/command recorder.
		*
		*  @return
		*    The newly created MultiplayerClient object or nullptr on error
		*/
		multiplayer::client::Client* connectToLocalMultiplayerHost(const std::string& dataPath, const MapIdentifier& mapIdentifier, const std::string& playerName, const std::string& sessionPassword);

		/**
		*  @brief
		*    Returns the stored MultiplayerClient object or nullptr, if there is none
		*
		*  @return
		*    The stored MultiplayerClient object or nullptr, if there is none
		*/
		multiplayer::client::Client* getMultiplayerClient() const;

		/**
		*  @brief
		*    Closes the multiplayer client connection and destroys the instance
		*/
		void closeMultiplayerClient();

		//[-------------------------------------------------------]
		//[ Multiplayer Host                                      ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Creates a new MultiplayerHost object
		*
		*  @return
		*    The newly created MultiplayerHost object or nullptr on error
		*/
		multiplayer::server::Host* createMultiplayerHost();

		/**
		*  @brief
		*    Returns the stored MultiplayerHost object or nullptr, if there is none
		*
		*  @return
		*    The stored MultiplayerHost object or nullptr, if there is none
		*/
		multiplayer::server::Host* getMultiplayerHost() const;

		/**
		*  @brief
		*    Closes the multiplayer host and destroys the instance
		*/
		void closeMultiplayerHost();

		//[-------------------------------------------------------]
		//[ Multiplayer General                                   ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns if a multiplay session is active (a multiplayer host or client exists)
		*/
		bool isMultiplayActive() const;

		/**
		*  @brief
		*    Returns the stored multiplayer session object or nullptr, if there is none (Can be either host or client)
		*
		*  @return
		*    The stored multiplayer session object or nullptr, if there is none
		*/
		multiplayer::SessionBase* getMultiplayerSession() const;

		/**
		*  @brief
		*    Return the update check instance, and creates it if needed
		*/
		UpdateCheck& getUpdateCheck();

		//[-------------------------------------------------------]
		//[ Multiplayer Proxy Client                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Override the proxy url and port
		*
		*  @param[in] proxyUrl
		*    The new url for the proxy server
		*  @param[in] proxyPort
		*    The new port for the proxy server
		*/
		void setProxyUrlAndPort(const std::string& proxyUrl, uint16 proxyPort);

		/**
		*  @brief
		*    Creates a new MultiplayerProxyClient object (when not already created) and tries to connect to the proxy server (with default values for hostname and port
		*
		*  @return
		*    The newly created MultiplayerProxyClient object or nullptr on error
		*
		*  @note
		*    - A pointer to the returned instance is cached and reused instead of creating a fresh connection as long as it stays valid
		*/
		multiplayer::proxy::MultiplayerProxyClient* connectToMultiplayerProxyServer();

		/**
		*  @brief
		*    Creates a new MultiplayerProxyClient object(when not already created) and tries to connect to the proxy server
		*
		*  @param[in] hostname
		*    The hostname of the proxy server
		*  @param[in] port
		*    The port of the proxy server
		*
		*  @return
		*    The newly created MultiplayerProxyClient object or nullptr on error
		*
		*  @note
		*    - A pointer to the returned instance is cached and reused instead of creating a fresh connection as long as it stays valid
		*/
		multiplayer::proxy::MultiplayerProxyClient* connectToMultiplayerProxyServer(const std::string& hostname, uint16 port);

		/**
		*  @brief
		*    Returns the stored MultiplayerProxyClient object or nullptr, if there is none
		*
		*  @return
		*    The stored MultiplayerProxyClient object or nullptr, if there is none
		*/
		multiplayer::proxy::MultiplayerProxyClient* getMultiplayerProxyClient() const;

		/**
		*  @brief
		*    Closes the multiplayer proxy client and destroys the instance
		*/
		void closeMultiplayerProxyClient();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::weak_ptr<MatchmakingClient>			mMatchmakingClient;
		multiplayer::client::Client*				mMultiplayerClient;
		multiplayer::server::Host*					mMultiplayerHost;
		multiplayer::proxy::MultiplayerProxyClient* mMultiplayerProxyClient;

		UpdateCheck*						mUpdateCheck;

		boost::optional<std::string> mMatchmakingUrl;
		boost::optional<uint16> mMatchmakingPort;
		boost::optional<std::string> mProxyUrl;
		boost::optional<uint16> mProxyPort;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
