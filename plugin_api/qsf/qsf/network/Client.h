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
	*    Network client class
	*/
	class QSF_API_EXPORT Client : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Connection state
		*/
		enum ConnectionState
		{
			STATE_DISCONNECTED = 0,	///< The client is not connected to a server
			STATE_CONNECTING,		///< The client currently tries to connect to a server
			STATE_CONNECTED			///< The client is currently connected to a server
		};

		/**
		*  @brief
		*    Connection information
		*/
		struct ConnectionInformation
		{
			std::string hostname;
			uint16 port;
			uint32 networkPing;			///< The time (im ms) it took to send the last packet and receive the successful arrival response for it
			uint32 upstreamTraffic;		///< The number of bytes per that were sent during the last second
			uint32 downstreamTraffic;	///< The number of bytes per that were received during the last second
			uint32 packetLoss;

			ConnectionInformation() :
				port(0),
				networkPing(0),
				upstreamTraffic(0),
				downstreamTraffic(0),
				packetLoss(0)
			{};
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		Client();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Client();

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
		*    Connect the client to a server
		*
		*  @param[in] hostname
		*    ASCII name of the host to connect to (e.g. "localhost")
		*  @param[in] port
		*    Port to use (e.g. "4000")
		*  @param[in] clientId
		*    Client ID (e.g. "0xdeadbabe")
		*
		*  @return
		*    "true" if all went fine, else "false"
		*
		*  @note
		*    - If the client is already connected to a server, the previous connection will be closed before opening a new one
		*/
		bool connectToServer(const std::string& hostname, unsigned short port, uint32 clientId);

		/**
		*  @brief
		*    Return the current connection state
		*
		*  @return
		*    The current connection state
		*/
		inline ConnectionState getConnectionState() const;

		/**
		*  @brief
		*    Return the client ID
		*
		*  @return
		*    The client ID, only valid in case the connection state is not "STATE_DISCONNECTED"
		*/
		inline uint32 getClientId() const;

		/**
		*  @brief
		*    Fill the given structure with up to date connection information
		*
		*  @return
		*    "true" on success, "false" otherwise (no connection established)
		*/
		bool getConnectionInformation(ConnectionInformation& connectionInformation) const;

		/**
		*  @brief
		*    Disconnect the client from the server it's currently connected to
		*/
		void disconnectFromServer();

		/**
		*  @brief
		*    Send a package to the server
		*
		*  @param[in,out] packet
		*    The packet data, that should be sent (the vector will be empty upon return of this method)
		*
		*  @return
		*    "true" if all went fine, else "false" (maybe not connected to a server?)
		*/
		bool send(std::vector<char>& packet) const;


	//[-------------------------------------------------------]
	//[ Public qsf::Client methods                            ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Called when the client is connected
		*
		*  @note
		*    - The default implementation is empty
		*    - "qsf::Client::onDisconnected()" will be called later on in case of disconnection
		*/
		virtual void onConnected();

		/**
		*  @brief
		*    Called when the client is disconnected unexpected
		*
		*  @note
		*    - The default implementation is empty
		*    - Only called in case there was actually a connection ("qsf::Client::onConnected()" was called before)
		*    - "qsf::Client::onDisconnected()" will still be called after this method
		*/
		virtual void onUnexpectedDisconnected();

		/**
		*  @brief
		*    Called when the client is disconnected
		*
		*  @note
		*    - The default implementation is empty
		*    - Only called in case there was actually a connection ("qsf::Client::onConnected()" was called before)
		*/
		virtual void onDisconnected();

		/**
		*  @brief
		*    Called if the connection attempt failed
		*
		*  @note
		*    - The default implementation is empty
		*    - Only called in case there was no connection ("qsf::Client::onConnected()" was *not* called before)
		*/
		virtual void onConnectFailed();

		/**
		*  @brief
		*    Called when the client received a package
		*
		*  @param[in,out] packet
		*    The packet data, that was received
		*
		*  @note
		*    - The default implementation is empty
		*/
		virtual void onPacketReceived(const std::vector<char>& packet);


	//[-------------------------------------------------------]
	//[ Private declarations                                  ]
	//[-------------------------------------------------------]
	private:
		class ClientThread;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Disconnect the client from the server it's currently connected to
		*
		*  @note
		*    - Use "disconnectFromServer()" (clean disconnect and flush) instead of "internalDisconnectFromServer()" whenever possible
		*/
		void internalDisconnectFromServer();

		/**
		*  @brief
		*    Main update function that is passed to the job manager
		*
		*  @param[in] jobArguments
		*    Job arguments
		*/
		void updateJob(const JobArguments& jobArguments);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool			mConnected;					///< Is the client currently properly connected to the server? Used to detect an unexpected disconnect.
		ConnectionState mConnectionState;			///< The current connection state
		uint32			mClientId;					///< Client ID, only valid in case the connection state is not "STATE_DISCONNECTED"
		JobProxy		mUpdateJobProxy;			///< Job proxy
		uint32			mTimeoutRetransmitCount;	///< The maximum retransmission count
		uint32			mTimeoutMinimum;			///< The timeout minimum in milliseconds
		uint32			mTimeoutMaximum;			///< The timeout maximum in milliseconds
		ClientThread&	mClientThread;				///< Thread that actually manages the networking


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/network/Client-inl.h"
