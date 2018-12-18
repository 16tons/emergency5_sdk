// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/network/multiplayer/operationmessagehandler/OperationMessageHandlerManager.h"
#include "em5/network/multiplayer/operationmessagehandler/FadeEntityHandler.h"
#include "em5/network/multiplayer/operationmessagehandler/MinimapPingHandler.h"
#include "em5/network/multiplayer/server/Host.h"
#include "em5/network/multiplayer/client/Client.h"
#include "em5/network/NetworkManager.h"
#include "em5/EM5Helper.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace multiplayer
	{


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		OperationMessageHandler* OperationMessageHandlerManager::getHandlerById(OperationMessage id, bool onlyHost)
		{
			OperationMessageHandler* handler = nullptr;

			server::Host* multiplayerHost = EM5_NETWORK.getMultiplayerHost();
			if (nullptr != multiplayerHost)
			{
				handler = multiplayerHost->getOperationMessageHandlerManager().getHandlerById(id);
			}
			else if (!onlyHost)
			{
				client::Client* multiplayerClient = EM5_NETWORK.getMultiplayerClient();
				if (nullptr != multiplayerClient)
				{
					handler = multiplayerClient->getOperationMessageHandlerManager().getHandlerById(id);
				}
			}

			return handler;
		}


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		OperationMessageHandlerManager::OperationMessageHandlerManager()
		{
			mHandlerMap.emplace(FadeEntityHandler::HANDLER_ID, std::unique_ptr<FadeEntityHandler>(new FadeEntityHandler()));
			mHandlerMap.emplace(MinimapPingHandler::HANDLER_ID, std::unique_ptr<MinimapPingHandler>(new MinimapPingHandler()));
		}

		OperationMessageHandlerManager::~OperationMessageHandlerManager()
		{
			// Nothing to do here
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5
