// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/network/multiplayer/OperationMessages.h"

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{
		class BitStream;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace multiplayer
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    EMERGENCY 5 base class for operation message handler
		*/
		class EM5_API_EXPORT OperationMessageHandler : public boost::noncopyable
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*/
			OperationMessageHandler();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~OperationMessageHandler();


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::OperationMessageHandler methods ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Writes the stored data for the given tick into the bit stream
			*
			*  @param[in] currentTick
			*    The tick count for which data should be written to the bit stream
			*  @param[in] bitStream
			*    The stream to which any changed data will be written to
			*/
			virtual void writeData(int32 currentTick, qsf::game::BitStream& bitStream) = 0;

			/**
			*  @brief
			*    Gets called when a operation message (handled by this handler) is received
			*
			*  @param[in] playerId
			*    The ID of the player which has send the message
			*  @param[in] dataSize
			*    The size (bitcount) of the message payload (content)
			*  @param[in] bitStream
			*    The stream containing the data (the current read position of the stream points to the start of the message payload)
			*
			*  @return
			*    "true" if all went fine, else "false"
			*/
			virtual bool onHandleMessage(uint32 playerId, uint64 dataSize, const qsf::game::BitStream& bitStream) = 0;

			/**
			*  @brief
			*    Gets called when the received data should by applied
			*
			*  @param[in] currentTick
			*    The tick count for which any received data should be applied
			*/
			virtual void applyData(int32 currentTick) = 0;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5
