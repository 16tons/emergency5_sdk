// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/network/multiplayer/datacache/datacacheitems/VehicleData.h"
#include "em5/network/multiplayer/datacache/DataCacheItemHelper.h"
#include "em5/component/vehicle/VehicleComponent.h"

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
		VehicleData::VehicleData(const qsf::Entity& targetEntity) :
			DataCacheBase(targetEntity),
			mSpecialEntityId(qsf::getUninitialized<uint64>()),
			mCanBeTowedFlag(false)
		{
			mComponent = targetEntity.getComponent<VehicleComponent>();
			if (mComponent.valid())
			{
				mSpecialEntityId = mComponent->getSpecialEntity();
				mCanBeTowedFlag = mComponent->getCanBeTowed();
			}
			else
			{
				QSF_ERROR("VehicleData: The entity \""<<targetEntity.getId()<<"\" doesn't have a vehicle component for caching the data by this class", QSF_REACT_THROW);
			}
		}

		VehicleData::~VehicleData()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::DataCacheBase methods ]
		//[-------------------------------------------------------]
		bool VehicleData::includeForPrepare(const int32 currentTickCount, bool entityIsVisible)
		{
			// Only update each 1s
			return currentTickCount % 20 == 0;
		}

		bool VehicleData::prepareForUpdate(const qsf::Entity& targetEntity, const qsf::Clock& clock)
		{
			if (mComponent.valid())
			{
				bool anyChange = false;
				anyChange |= detail::checkForChanges(mComponent->getSpecialEntity(), mSpecialEntityId);
				anyChange |= detail::checkForChanges(mComponent->getCanBeTowedFlag(), mCanBeTowedFlag);
				return anyChange;
			}

			return false;
		}

		void VehicleData::updateData(qsf::game::BitStream& bitStream, bool force)
		{
			// When this method is called at least one tracked data by this class has changed
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getBitLength();
#endif
			bitStream.write(mSpecialEntityId);
			bitStream.write(mCanBeTowedFlag);

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getBitLength();
			QSF_LOG_PRINTS(INFO, "VehicleData: written bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void VehicleData::setData(const qsf::game::BitStream& bitStream, const int32 receivedHostTickCount)
		{
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getReadPosition();
#endif
			HistoryEntry entry;
			entry.tickCount = receivedHostTickCount;
			bitStream.read(entry.mSpecialEntityId);
			bitStream.read(entry.mCanBeTowedFlag);
			mHistoryList.emplace_back(entry);

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getReadPosition();
			QSF_LOG_PRINTS(INFO, "VehicleData: read bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void VehicleData::interpolate(qsf::Entity& targetEntity, const qsf::Clock& clock, const int32 receivedHostTickCount)
		{
			if (!mHistoryList.empty() && mComponent.valid())
			{
				auto iterator = mHistoryList.begin();
				while (iterator != mHistoryList.end())
				{
					const HistoryEntry& entry = *iterator;
					if (entry.tickCount == clock.getSignalCounter())
					{
						mComponent->setSpecialEntity(entry.mSpecialEntityId);

						if (mComponent->getCanBeTowedFlag() != entry.mCanBeTowedFlag)
							mComponent->setCanBeTowed(entry.mCanBeTowedFlag);

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
