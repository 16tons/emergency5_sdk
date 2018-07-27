// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/Export.h"

#include <qsf/network/Client.h>

#include <boost/signals2/signal.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{
		class NetworkManager;
		class BitStream;
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
		*    Game client class
		*/
		class QSF_GAME_API_EXPORT Client : public qsf::Client
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
			*    Returns the UTF-8 hostname of the server this client is connected to
			*
			*  @return
			*    the UTF-8 hostname of the server this client is connected to
			*/
			std::string getServerHostname() const;

			/**
			*  @brief
			*    Returns the port of the server this client is connected to
			*
			*  @return
			*    the port of the server this client is connected to
			*/
			uint16 getServerPort() const;

			/**
			*  @brief
			*    Sends a packet to the server
			*
			*  @param[in] packet
			*    A BitStream that resembles the packet to send to the server
			*
			*  @return
			*    "true" on success, "false" otherwise
			*/
			bool sendPacket(const BitStream& packet);

			/**
			*  @brief
			*    Closes any active connection
			*
			*  @note
			*    - After this call the instance isn't usable anymore
			*/
			void disconnect();

			void setUseCompression(bool useCompression);


		//[-------------------------------------------------------]
		//[ Public boost signals                                  ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Called when the client is connected
			*
			*  @note
			*    - Recommended slot signature: void onConnected()
			*    - "qsf::game::Client::disconnected()" will be called later on in case of disconnection
			*/
			boost::signals2::signal<void()> connected;

			/**
			*  @brief
			*    Called when the client is disconnected
			*
			*  @note
			*    - Recommended slot signature: void onDisconnected()
			*    - Only called in case there was actually a connection (there was a "connected()" signal before)
			*/
			boost::signals2::signal<void()> disconnected;

			/**
			*  @brief
			*    Called when the client is disconnected unexpected
			*
			*  @note
			*    - Recommended slot signature: void onUnexpectedDisconnected()
			*    - Only called in case there was actually a connection ("qsf::game::Client::connected()" was called before)
			*    - "qsf::game::Client::disconnected()" will still be called after this method
			*/
			boost::signals2::signal<void()> unexpectedDisconnected;

			/**
			*  @brief
			*    Called if the connection attempt failed
			*
			*  @note
			*    - Recommended slot signature: void onConnectFailed()
			*    - Only called in case there was no connection ("qsf::game::Client::connected()" was *not* called before)
			*/
			boost::signals2::signal<void()> connectFailed;

			/**
			*  @brief
			*    Called when a packet is received
			*
			*  @param[in] bitStream
			*    The bit stream object containing the packet data
			*
			*  @param[in] sequenceNumber
			*    The packet sequence number
			*
			*  @note
			*    - Recommended slot signature: void onPacketReceived(const qsf::game::BitStream& bitStream, uint32 sequenceNumber)
			*    - The packet is automatically decompressed before being passed to this signal
			*/
			boost::signals2::signal<void(const BitStream&, uint32)> packetReceived;


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			/**
			*  @brief
			*    Constructor
			*/
			Client();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~Client();


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::game::Client methods           ]
		//[-------------------------------------------------------]
		protected:
			virtual void onPacketReceived(const BitStream& bitStream, uint32 sequenceNumber);


		//[-------------------------------------------------------]
		//[ Public virtual qsf::Client methods                    ]
		//[-------------------------------------------------------]
		public:
			virtual void onConnected() override;
			virtual void onDisconnected() override;
			virtual void onUnexpectedDisconnected() override;
			virtual void onConnectFailed() override;
			virtual void onPacketReceived(const std::vector<char>& packet) override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			uint32 mPacketSequenceNumber;
			bool mUseCompression;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf
