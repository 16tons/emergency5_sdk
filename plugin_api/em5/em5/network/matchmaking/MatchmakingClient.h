// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"
#include "em5/network/matchmaking/MatchmakingTypes.h"

#include <qsf/network/layered/client/QsfClient.h>

#include <boost/signals2/signal.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class MatchmakingProtocol;
	namespace packet
	{
		class ProxyServerPropertiesResponse;
		class TestOpenPortResponse;
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
	*    EM5 matchmaking network client class
	*/
	class EM5_API_EXPORT MatchmakingClient : public qsf::QsfClient
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class MatchmakingProtocol;	// The matchmaking protocol needs to tell the matchmaking client about itself


	//[-------------------------------------------------------]
	//[ Public boost signals                                  ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Signal emitted after the connection has been established
		*/
		//void connected();
		boost::signals2::signal<void()> connected;

		/**
		*  @brief
		*    Signal emitted after the connection has been closed
		*/
		//void disconnected();
		boost::signals2::signal<void()> disconnected;

		/**
		*  @brief
		*    Signal emitted after the connection has been lost
		*/
		//void connectionLost();
		boost::signals2::signal<void()> connectionLost;

		/**
		*  @brief
		*    Signal emitted after the connection attempt failed
		*/
		//void connectFailed();
		boost::signals2::signal<void()> connectFailed;

		/**
		*  @brief
		*    Signal emitted after the register host response is available
		*/
		//void registerHostResponse(RegisterHostStatus status);
		boost::signals2::signal<void(multiplayer::RegisterHostStatus)> registerHostResponse;

		/**
		*  @brief
		*    Signal emitted after the host list is available
		*/
		//void hostList(const std::vector<HostEntry>& hosts);
		boost::signals2::signal<void(const std::vector<multiplayer::HostEntry>&)> hostList;

		/**
		*  @brief
		*    Signal emitted after the test open port response is available
		*/
		//void testOpenPortResponse(const em5::packet::TestOpenPortResponse& response);
		boost::signals2::signal<void(const em5::packet::TestOpenPortResponse&)> testOpenPortResponse;

		/**
		*  @brief
		*    Signal emitted after the proxy server properties response is available
		*/
		//void proxyServerPropertiesResponse(const em5::packet::ProxyServerPropertiesResponse& response);
		boost::signals2::signal<void(const em5::packet::ProxyServerPropertiesResponse&)> proxyServerPropertiesResponse;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		MatchmakingClient();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MatchmakingClient();

		/**
		*  @brief
		*    Attempt to register this host to the matchmaking server
		*
		*  @return
		*    "true" if all went fine, else "false"
		*/
		bool requestRegisterHost(const multiplayer::HostEntry& hostEntry, const std::string& gameModificationString) const;

		/**
		*  @brief
		*    Update this hosts entry at the matchmaking server
		*
		*  @return
		*    "true" if all went fine, else "false"
		*/
		bool updateHost(const multiplayer::HostEntry& hostEntry) const;

		bool unregisterHost() const;

		/**
		*  @brief
		*    Ask the matchmaking server to check if the previously registered local host is reachable
		*
		*  @return
		*    "true" if all went fine, else "false"
		*/
		bool requestTestOpenPort() const;

		/**
		*  @brief
		*    Request the list of hosts from the matchmaking server
		*
		*  @return
		*    "true" if all went fine, else "false"
		*/
		bool requestHostList(const std::string& gameModificationString) const;

		/**
		*  @brief
		*    Request the proxy server properties information from the matchmaking server
		*
		*  @return
		*    "true" if all went fine, else "false"
		*/
		bool requestProxyServerProperties() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Client methods                    ]
	//[-------------------------------------------------------]
	public:
		virtual void onConnected() override;
		virtual void onUnexpectedDisconnected() override;
		virtual void onConnectFailed() override;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::QsfClient methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual const qsf::QsfProtocolList& getProtocols() const override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void setMatchmakingProtocol(MatchmakingProtocol* matchmakingProtocol);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::QsfProtocolList mProtocols;			///< List of protocols the editor client supports
		MatchmakingProtocol* mMatchmakingProtocol;	///< Matchmaking protocol instance, can be a null pointer, do not destroy the instance, pointer is set by "em5::MatchmakingProtocol"


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
