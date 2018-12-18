// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/network/multiplayer/datacache/datacacheitems/WaterCannonData.h"
#include "em5/network/multiplayer/datacache/DataCacheItemHelper.h"
#include "em5/component/vehicle/parts/WaterCannonComponent.h"

#include <qsf_game/network/BitStream.h>

#include <qsf/log/LogSystem.h>
#include <qsf/map/Entity.h>
#include <qsf/time/clock/Clock.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace multiplayer
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		WaterCannonData::WaterCannonData(const qsf::Entity& targetEntity) :
			DataCacheBase(targetEntity),
			mTargetEntityId(qsf::getUninitialized<uint64>()),
			mTargetEntityIdChanged(false)
		{
			mComponent = targetEntity.getComponent<WaterCannonComponent>();
			if (mComponent.valid())
			{
				mTargetEntityId = mComponent->getTargetEntityId();
			}
			else
			{
				QSF_ERROR("WaterCannonData: The entity \""<<targetEntity.getId()<<"\" doesn't have a WaterjetComponent for caching the data by this class", QSF_REACT_THROW);
			}
		}

		WaterCannonData::~WaterCannonData()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::DataCacheBase methods ]
		//[-------------------------------------------------------]
		bool WaterCannonData::prepareForUpdate(const qsf::Entity& targetEntity, const qsf::Clock& clock)
		{
			if (mComponent.valid())
			{
				mTargetEntityIdChanged = detail::checkForChanges(mComponent->getTargetEntityId(), mTargetEntityId);

				return mTargetEntityIdChanged;
			}
			else
			{
				// The entity has no vehicle crane component anymore -> This shouldn't never happen. An instance of WaterCannonData for such an entity should never be created
				QSF_ERROR("WaterCannonData: The entity \""<<targetEntity.getId()<<"\" lost its water cannon component this instance is not longer usable for that entity", QSF_REACT_NONE);
			}

			return false;
		}

		void WaterCannonData::updateData(qsf::game::BitStream& bitStream, bool force)
		{
			// When this method is called at least one tracked data by this class has changed
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getBitLength();
#endif
			detail::writeChanges(mTargetEntityIdChanged, mTargetEntityId, bitStream);

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getBitLength();
			QSF_LOG_PRINTS(INFO, "WaterCannonData: written bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void WaterCannonData::setData(const qsf::game::BitStream& bitStream, const int32 receivedHostTickCount)
		{
			// When this method is called at least one tracked data by this class has changed on the host side
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getReadPosition();
#endif
			if (detail::checkChangedAndRead(bitStream, mTargetEntityId))
			{
				HistoryEntry entry;
				entry.tickCount = receivedHostTickCount;
				entry.mTargetEntityId = mTargetEntityId;

				mHistoryList.emplace_back(entry);
			}

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getReadPosition();
			QSF_LOG_PRINTS(INFO, "WaterCannonData: read bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void WaterCannonData::interpolate(qsf::Entity& targetEntity, const qsf::Clock& clock, const int32 receivedHostTickCount)
		{
			if (!mHistoryList.empty() && mComponent.valid())
			{
				auto iterator = mHistoryList.begin();
				while (iterator != mHistoryList.end())
				{
					const HistoryEntry& entry = *iterator;
					if (entry.tickCount == clock.getSignalCounter())
					{
						mComponent->setTargetEntityId(entry.mTargetEntityId);

						iterator = mHistoryList.erase(iterator);
					}
					else
					{
						// The entry is not for the current tick -> no further processing possible
						return;
					}
				}
			}
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5
