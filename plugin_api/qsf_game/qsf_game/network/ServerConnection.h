// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/Export.h"

#include <boost/noncopyable.hpp>
#include <boost/signals2/signal.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{
		class BitStream;
		class Server;
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
		*    Game server connection class
		*/
		class QSF_GAME_API_EXPORT ServerConnection : public boost::noncopyable
		{
			friend class Server;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Returns the unique identifier of this ServerConnection object
			*
			*  @return
			*    The unique identifier of this ServerConnection object
			*/
			uint32 getId() const;

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
			*    Closes this ServerConnection
			*
			*  @param[in] force
			*    Force the disconnect -> do it imediately
			*/
			void disconnect(bool force = false);

			void setUseCompression(bool useCompression);


		//[-------------------------------------------------------]
		//[ Public boost signals                                  ]
		//[-------------------------------------------------------]
		public:
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
			ServerConnection(Server& server, uint32 clientId);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~ServerConnection();

			void onPacketReceived(const std::vector<char>& packet);


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::game::ServerConnection methods ]
		//[-------------------------------------------------------]
		protected:
			virtual void onPacketReceived(const BitStream& bitStream, uint32 sequenceNumber);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			Server& mServer;
			uint32	mClientId;
			uint32	mPacketSequenceNumber;
			bool	mUseCompression;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf
