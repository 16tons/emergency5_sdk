// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/network/multiplayer/datacache/datacacheitems/BuildingData.h"
#include "em5/network/multiplayer/datacache/DataCacheItemHelper.h"
#include "em5/component/building/BuildingComponent.h"

#include <qsf_game/network/BitStream.h>

#include <qsf/log/LogSystem.h>
#include <qsf/map/Entity.h>


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
		BuildingData::BuildingData(const qsf::Entity& targetEntity) :
			DataCacheBase(targetEntity),
			mPersonAtWindowEntityId(qsf::getUninitialized<uint64>()),
			mPersonAtWindowEntityIdChanged(false),
			mDoorBlocked(false)
		{
			mComponent = targetEntity.getComponent<BuildingComponent>();
			if (mComponent.valid())
			{
				mPersonAtWindowEntityId = mComponent->mPersonInWindowEntityId;
			}
			else
			{
				QSF_ERROR("BuildingData: The entity \""<<targetEntity.getId()<<"\" doesn't have a building component for caching the data by this class", QSF_REACT_THROW);
			}
		}

		BuildingData::~BuildingData()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::DataCacheBase methods ]
		//[-------------------------------------------------------]
		bool BuildingData::includeForPrepare(const int32 currentTickCount, bool entityIsVisible)
		{
			if (mComponent.valid())
			{
				// Update only each 150 ms
				return currentTickCount % 3 == 0;
			}
			return false;
		}

		bool BuildingData::prepareForUpdate(const qsf::Entity& targetEntity, const qsf::Clock& clock)
		{
			if (mComponent.valid())
			{
				mPersonAtWindowEntityIdChanged = detail::checkForChanges(mComponent->mPersonInWindowEntityId, mPersonAtWindowEntityId);
				const bool doorBlockedChanged = detail::checkForChanges(mComponent->mIsDoorBlocked, mDoorBlocked);
				return mPersonAtWindowEntityIdChanged || doorBlockedChanged;
			}

			return false;
		}

		void BuildingData::updateData(qsf::game::BitStream& bitStream, bool force)
		{
			// When this method is called at least one tracked data by this class has changed
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getBitLength();
#endif
			detail::writeChanges(mPersonAtWindowEntityIdChanged, mPersonAtWindowEntityId, bitStream);
			bitStream.write(mDoorBlocked);

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getBitLength();
			QSF_LOG_PRINTS(INFO, "BuildingData: written bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void BuildingData::setData(const qsf::game::BitStream& bitStream, const int32 receivedHostTickCount)
		{
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getReadPosition();
#endif
			HistoryEntry entry;
			entry.tickCount = receivedHostTickCount;
			entry.mPersonAtWindowEntityIdChanged = detail::checkChangedAndRead(bitStream, entry.mPersonAtWindowEntityId);
			bitStream.read(entry.mDoorBlocked);
			mHistoryList.emplace_back(entry);

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getReadPosition();
			QSF_LOG_PRINTS(INFO, "BuildingData: read bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void BuildingData::interpolate(qsf::Entity& targetEntity, const qsf::Clock& clock, const int32 receivedHostTickCount)
		{
			if (!mHistoryList.empty() && mComponent.valid())
			{
				auto iterator = mHistoryList.begin();
				while (iterator != mHistoryList.end())
				{
					const HistoryEntry& entry = *iterator;
					if (entry.tickCount == clock.getSignalCounter())
					{
						if (entry.mPersonAtWindowEntityIdChanged)
						{
							mComponent->mPersonInWindowEntityId = entry.mPersonAtWindowEntityId;
						}

						mComponent->mIsDoorBlocked = entry.mDoorBlocked;

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
