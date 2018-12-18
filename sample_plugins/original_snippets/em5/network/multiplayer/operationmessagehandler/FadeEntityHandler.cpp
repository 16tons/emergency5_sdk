// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/network/multiplayer/operationmessagehandler/FadeEntityHandler.h"
#include "em5/network/multiplayer/ProtocolHelper.h"
#include "em5/component/effect/FadeEffectComponent.h"
#include "em5/game/Game.h"
#include "em5/game/GameSimulation.h"
#include "em5/EM5Helper.h"

#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>
#include <qsf/time/clock/Clock.h>
#include <qsf/QsfHelper.h>


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
		const OperationMessage FadeEntityHandler::HANDLER_ID = OperationMessage::FADE_ENTITY;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		FadeEntityHandler::FadeEntityHandler()
		{
			// Nothing to do here
		}

		FadeEntityHandler::~FadeEntityHandler()
		{
			// Nothing to do here
		}

		void FadeEntityHandler::onNewFadeEntity(uint64 entityId, const qsf::Time& fadeTime, bool fadeIn)
		{
			int32 tickCount = EM5_GAME.getSimulation().getClock().getSignalCounter();
			std::vector<FadeEntityData>& list = mHostDataList[tickCount];
			list.emplace_back(entityId, fadeTime, fadeIn);
		}


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::OperationMessageHandler methods ]
		//[-------------------------------------------------------]
		void FadeEntityHandler::writeData(int32 currentTick, qsf::game::BitStream& bitStream)
		{
			auto iterator = mHostDataList.find(currentTick);
			if (iterator != mHostDataList.end())
			{
				ProtocolHelper::writeFadeEntityOperationMessageData(bitStream, iterator->second, currentTick);

				mHostDataList.erase(iterator);
			}
		}

		bool FadeEntityHandler::onHandleMessage(uint32 playerId, uint64 dataSize, const qsf::game::BitStream& bitStream)
		{
			int32 tickCount;
			std::vector<FadeEntityData> dataList;
			if (ProtocolHelper::readFadeEntityOperationMessageData(bitStream, dataList, tickCount))
			{
				mClientDataList.emplace_back(tickCount, std::move(dataList));
				return true;
			}

			return false;
		}

		void FadeEntityHandler::applyData(int32 currentTick)
		{
			qsf::Map& mainMap = QSF_MAINMAP;

			auto iterator = mClientDataList.begin();
			while (iterator != mClientDataList.end())
			{
				if (iterator->first == currentTick)
				{
					// Apply the data
					for (FadeEntityData& data : iterator->second)
					{
						qsf::Entity* entity = mainMap.getEntityById(data.mEntityId);
						if (nullptr != entity)
						{
							FadeEffectComponent& fadeEffectComponent = entity->getOrCreateComponentSafe<FadeEffectComponent>();
							fadeEffectComponent.init(data.mFadeIn, data.mFadeTime);
						}
					}

					iterator = mClientDataList.erase(iterator);
				}
				else
				{
					// The data is not for the current tick
					break;
				}
			}
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5
