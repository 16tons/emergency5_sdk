// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/job/JobProxy.h"

#include <boost/noncopyable.hpp>
#include <boost/container/flat_map.hpp>

#include <vector>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Network server class
	*/
	class QSF_API_EXPORT Server : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		struct ClientInfo
		{
			std::string hostname;
			uint16 port;

			inline ClientInfo() :
				port(getUninitialized<uint16>())
			{
				// Nothing to do in here
			}

			inline ClientInfo(std::string hostname, uint16 port) :
				hostname(hostname),
				port(port)
			{
				// Nothing to do in here
			}
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		Server();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Server();

		/**
		*  @brief
		*    Set the timeout parameters for the client
		*
		*  @param[in] timeoutRetransmitCount
		*    The maximum retransmission count (0 for ENET default value of 32)
		*  @param[in] timeoutMinimum
		*    The timeout minimum in milliseconds (0 for ENET default value of 5000)
		*  @param[in] timeoutMaximum
		*    The timeout maximum in milliseconds (0 for ENET default value of 30000)
		*
		*  @remarks
		*    The timeout parameter control how and when a peer will timeout from a failure to acknowledge reliable traffic.
		*    Timeout values use an exponential backoff mechanism, where if a reliable packet is not acknowledge within some
		*    multiple of the average RTT plus a variance tolerance, the timeout will be doubled until it reaches a set limit.
		*    (timeoutRetransmitCount)
		*    If the timeout is thus at this limit and reliable packets have been sent but not acknowledged within a certain
		*    minimum time period (timeoutMinimum), the peer will be disconnected. Alternatively, if reliable packets have been
		*    sent but not acknowledged for a certain maximum time period (timeoutMaximum), the peer will be disconnected
		*    regardless of the current timeout limit value.
		*
		*  @note
		*    - The timeout values are independent from the connection state
		*/
		void setTimeout(uint32 timeoutRetransmitCount, uint32 timeoutMinimum, uint32 timeoutMaximum);

		/**
		*  @brief
		*    Start listening on the specified port and address
		*
		*  @param[in] port
		*    Port to listen on
		*  @param[in] address
		*    IP address to listen on (Hostnames are resolved, specify "0.0.0.0" to listen on all addresses on all interfaces
		*
		*  @return
		*    "true" if all went fine, else "false"
		*
		*  @note
		*    - In case the server is already started, it's stopped and restarted
		*/
		bool start(uint16 port, const std::string& address);

		/**
		*  @brief
		*    Stop the server
		*/
		void stop();

		/**
		*  @brief
		*    Close the connection to the given connected client
		*
		*  @param[in] clientId
		*    ID of the client to close the connection to
		*  @param[in] force
		*    When true the connection is forcible closed
		*
		*   @note: When force = true then no disconnected signals for that client are emitted!
		*/
		void disconnect(uint32 clientId, bool force);

		/**
		*  @brief
		*    Send a package to all connected clients.
		*
		*    Broadcasting using this method should be avoided since it ignores the individual client's states (e.g. authentification state)
		*
		*  @param[in,out] packet
		*    The packet data, that should be sent (the vector will be empty upon return of this method)
		*
		*  @return
		*    "true" if all went fine, else "false" (maybe server not started?)
		*/
		bool broadcast(std::vector<char>& packet) const;

		/**
		*  @brief
		*    Send a package to a given connected client
		*
		*  @param[in] clientId
		*    ID of the client to send the package to
		*  @param[in,out] packet
		*    The packet data, that should be sent (the vector will be empty upon return of this method)
		*
		*  @return
		*    "true" if all went fine, else "false" (maybe server not started?)
		*/
		bool sendTo(uint32 clientId, std::vector<char>& packet) const;

		const ClientInfo* getClientInfo(uint32 clientId) const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Server methods                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Called when a client is connected
		*
		*  @param[in] clientId
		*    ID of the connected client
		*
		*  @note
		*    - The default implementation is empty
		*    - "qsf::Server::onClientDisconnected()" will be called later on in case of disconnection
		*/
		virtual void onClientConnected(uint32 clientId);

		/**
		*  @brief
		*    Called when a client is disconnected
		*
		*  @param[in] clientId
		*    ID of the disconnected client
		*
		*  @note
		*    - The default implementation is empty
		*    - Only called in case there was actually a connection ("qsf::Server::onClientConnected()" was called before)
		*/
		virtual void onClientDisconnected(uint32 clientId);

		/**
		*  @brief
		*    Called when a client package has been received
		*
		*  @param[in] clientId
		*    ID of the client sending the package
		*  @param[in,out] packet
		*    The packet data, that was received
		*
		*  @note
		*    - The default implementation is empty
		*/
		virtual void onPacketReceived(uint32 clientId, const std::vector<char>& packet);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Main update function that is passed to the job manager
		*
		*  @param[in] jobArguments
		*    Job arguments
		*
		*  @note
		*    - Only called in case "mEnetHost" is valid
		*/
		void updateJob(const JobArguments& jobArguments);


	//[-------------------------------------------------------]
	//[ Private declarations                                  ]
	//[-------------------------------------------------------]
	private:
		class ServerThread;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		JobProxy									   mUpdateJobProxy;			///< Job proxy
		uint32										   mTimeoutRetransmitCount;	///< The maximum retransmission count
		uint32										   mTimeoutMinimum;			///< The timeout minimum in milliseconds
		uint32										   mTimeoutMaximum;			///< The timeout maximum in milliseconds
		ServerThread&								   mServerThread;			///< Thread that actually manages the networking
		boost::container::flat_map<uint32, ClientInfo> mClientInfoMap;			///< Stored information about all currently connected clients


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/network/Server-inl.h"
