// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
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
		namespace client
		{


			//[-------------------------------------------------------]
			//[ Classes                                               ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    EMERGENCY 5 multiplayer host base class (represents an host on the client side)
			*/
			class EM5_API_EXPORT HostBase : public boost::noncopyable
			{


			//[-------------------------------------------------------]
			//[ Public boost signals                                  ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Called when the connection to the host is established
				*
				*  @note
				*    - Recommended slot signature: void onConnected()
				*    - "em5::multiplayer::client::HostBase::disconnected()" will be called later on in case of disconnection
				*/
				boost::signals2::signal<void()> connected;

				/**
				*  @brief
				*    Called when the client is disconnected
				*
				*  @note
				*    - Recommended slot signature: void onDisconnected()
				*    - Only called in case there was actually a connection
				*/
				boost::signals2::signal<void()> disconnected;

				/**
				*  @brief
				*    Called when the client is disconnected unexpected
				*
				*  @note
				*    - Recommended slot signature: void onUnexpectedDisconnected()
				*    - Only called in case there was actually a connection
				*/
				boost::signals2::signal<void()> unexpectedDisconnected;

				/**
				*  @brief
				*    Called when a operation message is received from the host server
				*
				*  @param[in] operationMessageId
				*    The id of the operation message
				*  @param[in] dataSize
				*    The payload size of the message
				*  @param[in] bitStream
				*    The bit stream object which contains the whole operation message data.
				*
				*  @note
				*    - The handler should not store the given bit stream instance, because when the handler method has exited it cannot be guaranteed that the instance will still be valid
				*    - Recommended slot signature: void onReceivedOperationMessage(const int32, tickCount, const em5::multiplayer::OperationMessage operationMessageId, uint32 dataSize, const qsf::game::BitStream& bitStream)
				*    - When more then one handler is registered then the handlers should make sure that the stream position is resetted to the position as on the function entry
				*/
				boost::signals2::signal<void(const OperationMessage, uint32, const qsf::game::BitStream&)> receivedOperationMessage;


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Default constructor
				*/
				inline HostBase();

				/**
				*  @brief
				*    Destructor
				*/
				inline virtual ~HostBase();


			//[-------------------------------------------------------]
			//[ Public virtual em5::multiplayer::client::HostBase methods ]
			//[-------------------------------------------------------]
			public:
				virtual bool connect(const std::string& hostOrIpAddress, uint16 port) = 0;

				/**
				*  @brief
				*    Sends data to the host
				*/
				virtual bool sendData(const qsf::game::BitStream& bitStream) = 0;

				inline virtual void shutdown();


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
#include "em5/network/multiplayer/client/HostBase-inl.h"
