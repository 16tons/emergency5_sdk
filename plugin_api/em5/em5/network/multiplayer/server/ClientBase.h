// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"
#include "em5/network/multiplayer/OperationMessages.h"

#include <boost/noncopyable.hpp>
#include <boost/signals2.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{
		class BitStream;
	}

	class Clock;
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
			*    EMERGENCY 5 multiplayer client base class (represents a client on the server side)
			*/
			class EM5_API_EXPORT ClientBase : public boost::noncopyable
			{


			//[-------------------------------------------------------]
			//[ Public boost signals                                  ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Called when a operation message is received from the client
				*
				*  @param[in] client
				*    The server side client object which received the operation message
				*  @param[in] operationMessageId
				*    The id of the operation message
				*  @param[in] dataSize
				*    The payload size of the message (excluded the size needed for the Id and the payload size)
				*  @param[in] bitStream
				*    The bit stream object which contains the whole operation message data. The stream is positioned after the information about the operation message id and the payload size
				*
				*  @note
				*    - The handler should not store the given bit stream instance, because when the handler method has exited it cannot be guaranteed that the instance will still be valid
				*    - Recommended slot signature: void onReceivedOperationMessage(const em5::multiplayer::server::ClientBase& client, em5::multiplayer::OperationMessage operationMessageId, uint64 dataSize, const qsf::game::BitStream& bitStream)
				*    - When more then one handler is registered then the handlers should make sure that the stream position is resetted to the position as on the function entry
				*/
				boost::signals2::signal<void(ClientBase&, const OperationMessage, uint64, const qsf::game::BitStream&)> receivedOperationMessage;


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Default constructor
				*/
				inline ClientBase();

				/**
				*  @brief
				*    Destructor
				*/
				inline virtual ~ClientBase();

				inline uint32 getPlayerIndex() const;
				inline void setPlayerIndex(uint32 playerIndex);


			//[-------------------------------------------------------]
			//[ Public virtual em5::multiplayer::server::ClientBase methods ]
			//[-------------------------------------------------------]
			public:
				virtual void disconnect() = 0;
				virtual void disconnect(bool force) = 0;
				virtual uint64 getId() const = 0;

				/**
				*  @brief
				*    Send data
				*
				*  @note
				*    This method is used to send ingame data (e.g. DataUpdate messages) to the clients, this special method is only needed to make the lokal client easier to implement because the local client does not need to always parse the message data to find out if it should record the data or not
				*/
				virtual bool sendData(const qsf::Clock& simulationClock, const qsf::game::BitStream& bitStream) = 0;
				virtual bool sendData(const qsf::game::BitStream& bitStream) = 0;


			//[-------------------------------------------------------]
			//[ Protected methods                                     ]
			//[-------------------------------------------------------]
			protected:
				/**
				*  @brief
				*    Called, when data is received
				*/
				void onDataReceived(const qsf::game::BitStream& bitStream);


			//[-------------------------------------------------------]
			//[ Private methods                                       ]
			//[-------------------------------------------------------]
			private:
				void splitReceivedMessageIntoOperationMessages(const qsf::game::BitStream& bitStream, uint32 messageSize);


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
			private:
				uint32	mPlayerIndex;


			};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // server
	} // multiplayer
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/network/multiplayer/server/ClientBase-inl.h"
