// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/network/multiplayer/operationmessagehandler/MinimapPingHandler.h"
#include "em5/network/multiplayer/client/Client.h"
#include "em5/network/multiplayer/server/Host.h"
#include "em5/network/multiplayer/ProtocolHelper.h"
#include "em5/network/NetworkManager.h"
#include "em5/game/Game.h"
#include "em5/game/GameSimulation.h"
#include "em5/game/minimap/MinimapHelper.h"
#include "em5/EM5Helper.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace multiplayer
	{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		const OperationMessage MinimapPingHandler::HANDLER_ID = OperationMessage::MINIMAP_PING;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		MinimapPingHandler::MinimapPingHandler()
		{
			// Nothing to do here
		}

		MinimapPingHandler::~MinimapPingHandler()
		{
			// Nothing to do here
		}

		void MinimapPingHandler::onNewPing(const glm::vec3& position)
		{
			if (EM5_NETWORK.isMultiplayActive())
			{
				MinimapHelper::registerPingByWorldPosition(position);

				int32 tickCount = EM5_GAME.getSimulation().getClock().getSignalCounter();

				qsf::game::BitStream bitStream;
				ProtocolHelper::writeMinimapPingOperationMessageData(bitStream, MinimapPingData(position), tickCount);

				multiplayer::server::Host* multiplayerHost = EM5_NETWORK.getMultiplayerHost();
				if (nullptr != multiplayerHost)
				{
					multiplayerHost->sendOperationMessageToAllClients(HANDLER_ID, bitStream);
				}
				else
				{
					multiplayer::client::Client* multiplayerClient = EM5_NETWORK.getMultiplayerClient();
					if (nullptr != multiplayerClient)
					{
						multiplayerClient->sendOperationMessageToHost(HANDLER_ID, bitStream);
					}
				}
			}
		}


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::OperationMessageHandler methods ]
		//[-------------------------------------------------------]
		void MinimapPingHandler::writeData(int32 currentTick, qsf::game::BitStream& bitStream)
		{
			// Nothing to do in here
		}

		bool MinimapPingHandler::onHandleMessage(uint32 playerId, uint64 dataSize, const qsf::game::BitStream& bitStream)
		{
			int32 tickCount;
			MinimapPingData data;
			if (ProtocolHelper::readMinimapPingOperationMessageData(bitStream, data, tickCount))
			{
				MinimapHelper::registerPingByWorldPosition(data.mPingPosition);

				multiplayer::server::Host* multiplayerHost = EM5_NETWORK.getMultiplayerHost();
				if (nullptr != multiplayerHost)
				{
					// We are the host send the message to all other clients
					qsf::game::BitStream bitStream;
					ProtocolHelper::writeMinimapPingOperationMessageData(bitStream, data, tickCount);
					multiplayerHost->sendOperationMessageToAllClients(HANDLER_ID, bitStream, playerId);
				}

				return true;
			}

			return false;
		}

		void MinimapPingHandler::applyData(int32 currentTick)
		{
			// Nothing to do in here
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5
