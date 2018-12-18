// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/network/multiplayer/operationmessagehandler/OperationMessageHandler.h"

#include <vector>


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
		*    EMERGENCY 5 handler for the FADE_ENTITY operation message
		*/
		class FadeEntityHandler : public OperationMessageHandler
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const OperationMessage HANDLER_ID;	/// OperationMessage::FADE_ENTITY


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*/
			FadeEntityHandler();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~FadeEntityHandler();

			/**
			*  @brief
			*    Add data about a new fade effect to the handler
			*/
			void onNewFadeEntity(uint64 entityId, const qsf::Time& fadeTime, bool fadeIn);


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::OperationMessageHandler methods ]
		//[-------------------------------------------------------]
		public:
			virtual void writeData(int32 currentTick, qsf::game::BitStream& bitStream) override;
			virtual bool onHandleMessage(uint32 playerId, uint64 dataSize, const qsf::game::BitStream& bitStream) override;
			virtual void applyData(int32 currentTick) override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			std::vector<std::pair<int32, std::vector<FadeEntityData>>>	mClientDataList;	///< List of received data
			std::unordered_map<int32, std::vector<FadeEntityData>>		mHostDataList;		///< List of data created on the host side, which are not yet transferred to the clients


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5
