// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/network/multiplayer/datacache/datacacheitems/DoorData.h"
#include "em5/network/multiplayer/datacache/DataCacheItemHelper.h"
#include "em5/component/door/DoorComponent.h"

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
		DoorData::DoorData(const qsf::Entity& targetEntity) :
			DataCacheBase(targetEntity),
			mOpenDoor(false)
		{
			mComponent = targetEntity.getComponent<DoorComponent>();
			if (mComponent.valid())
			{
				mOpenDoor = mComponent->getDoorState() == DoorComponent::DOOR_OPEN || mComponent->getDoorState() == DoorComponent::DOOR_OPENING;
			}
			else
			{
				QSF_ERROR("DoorData: The entity \""<<targetEntity.getId()<<"\" doesn't have a door component for caching the data by this class", QSF_REACT_THROW);
			}
		}

		DoorData::~DoorData()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::DataCacheBase methods ]
		//[-------------------------------------------------------]
		bool DoorData::prepareForUpdate(const qsf::Entity& targetEntity, const qsf::Clock& clock)
		{
			if (mComponent.valid())
			{
				const DoorComponent::DoorState doorState = mComponent->getDoorState();
				return detail::checkForChanges((doorState == DoorComponent::DOOR_OPEN ||doorState == DoorComponent::DOOR_OPENING), mOpenDoor);
			}

			return false;
		}

		void DoorData::updateData(qsf::game::BitStream& bitStream, bool force)
		{
			// When this method is called at least one tracked data by this class has changed
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getBitLength();
#endif
			bitStream.write(mOpenDoor);

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getBitLength();
			QSF_LOG_PRINTS(INFO, "DoorData: written bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void DoorData::setData(const qsf::game::BitStream& bitStream, const int32 receivedHostTickCount)
		{
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getReadPosition();
#endif
			HistoryEntry entry;
			entry.tickCount = receivedHostTickCount;
			entry.mOpenDoor = detail::checkChanged(bitStream);
			mHistoryList.emplace_back(entry);

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getReadPosition();
			QSF_LOG_PRINTS(INFO, "DoorData: read bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void DoorData::interpolate(qsf::Entity& targetEntity, const qsf::Clock& clock, const int32 receivedHostTickCount)
		{
			if (!mHistoryList.empty() && mComponent.valid())
			{
				auto iterator = mHistoryList.begin();
				while (iterator != mHistoryList.end())
				{
					const HistoryEntry& entry = *iterator;
					if (entry.tickCount == clock.getSignalCounter())
					{
						if (entry.mOpenDoor)
						{
							mComponent->openDoor();
						}
						else
						{
							mComponent->closeDoor();
						}

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
