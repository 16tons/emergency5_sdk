// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/network/multiplayer/operationmessagehandler/OperationMessageHandler.h"


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
		*    EMERGENCY 5 handler for the MINIMAP_PING operation message
		*/
		class MinimapPingHandler : public OperationMessageHandler
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const OperationMessage HANDLER_ID;	/// OperationMessage::MINIMAP_PING


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*/
			MinimapPingHandler();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~MinimapPingHandler();

			/**
			*  @brief
			*    Add data about a new minimap ping to the handler
			*/
			void onNewPing(const glm::vec3& position);


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::OperationMessageHandler methods ]
		//[-------------------------------------------------------]
		public:
			virtual void writeData(int32 currentTick, qsf::game::BitStream& bitStream) override;
			virtual bool onHandleMessage(uint32 playerId, uint64 dataSize, const qsf::game::BitStream& bitStream) override;
			virtual void applyData(int32 currentTick) override;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5
