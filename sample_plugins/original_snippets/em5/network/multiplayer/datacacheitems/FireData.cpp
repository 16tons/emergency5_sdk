// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/network/multiplayer/datacache/datacacheitems/FireData.h"
#include "em5/network/multiplayer/datacache/DataCacheItemHelper.h"
#include "em5/fire/component/FireComponent.h"

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
		FireData::FireData(const qsf::Entity& targetEntity) :
			DataCacheBase(targetEntity),
			mActive(false),
			mFireState(0),
			mFireStateChanged(false),
			mFireEnergy(0.0f),
			mFireEnergyChanged(false)
		{
			mComponent = targetEntity.getComponent<FireComponent>();
			if (mComponent.valid())
			{
				mActive = mComponent->isActive();
				mFireState = static_cast<uint8>(mComponent->getFireState());
				mFireEnergy = mComponent->getEnergy();
			}
			else
			{
				QSF_ERROR("FireData: The entity \""<<targetEntity.getId()<<"\" doesn't have a fire component for caching the data by this class", QSF_REACT_THROW);
			}
		}

		FireData::~FireData()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::DataCacheBase methods ]
		//[-------------------------------------------------------]
		bool FireData::includeForPrepare(const int32 currentTickCount, bool entityIsVisible)
		{
			// Do only checks each 3. tick (The fire system updates the data in 250ms intervalls)
			return currentTickCount % 3 == 0;
		}

		bool FireData::prepareForUpdate(const qsf::Entity& targetEntity, const qsf::Clock& clock)
		{
			if (mComponent.valid())
			{
				bool activeChanged = detail::checkForChanges(mComponent->isActive(), mActive);
				const uint8 fireState = static_cast<uint8>(mComponent->getFireState());
				mFireStateChanged = detail::checkForChanges(fireState, mFireState);
				mFireEnergyChanged = detail::checkForChanges(mComponent->getEnergy(), mFireEnergy);

				return mFireStateChanged || mFireEnergyChanged || activeChanged;
			}

			return false;
		}

		void FireData::updateData(qsf::game::BitStream& bitStream, bool force)
		{
			// When this method is called at least one tracked data by this class has changed
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getBitLength();
#endif
			bitStream.write(mActive);
			// The fire state has a value range of 0-3 = can be represented by 2 bits
			detail::writeChanges(mFireStateChanged, mFireState, 2, bitStream);
			detail::writeChanges(mFireEnergyChanged, mFireEnergy, bitStream);

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getBitLength();
			QSF_LOG_PRINTS(INFO, "FireData: written bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void FireData::setData(const qsf::game::BitStream& bitStream, const int32 receivedHostTickCount)
		{
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getReadPosition();
#endif
			HistoryEntry entry;
			entry.tickCount = receivedHostTickCount;

			// Read the data from the stream
			entry.mActive = detail::checkChanged(bitStream);

			// The fire state has a value range of 0-3 = can be represented by 2 bits
			entry.mFireStateChanged = detail::checkChangedAndRead(bitStream, 2, entry.mFireState);
			entry.mFireEnergyChanged = detail::checkChangedAndRead(bitStream, entry.mFireEnergy);

			mHistoryList.emplace_back(entry);

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getReadPosition();
			QSF_LOG_PRINTS(INFO, "FireData: read bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void FireData::interpolate(qsf::Entity& targetEntity, const qsf::Clock& clock, const int32 receivedHostTickCount)
		{
			if (!mHistoryList.empty() && mComponent.valid())
			{
				auto iterator = mHistoryList.begin();
				while (iterator != mHistoryList.end())
				{
					const HistoryEntry& entry = *iterator;
					if (entry.tickCount == clock.getSignalCounter())
					{
						mComponent->setActive(entry.mActive);

						if (entry.mFireStateChanged)
						{
							mComponent->setFireStateByEvent(static_cast<FireComponent::FireState>(entry.mFireState));
						}

						if (entry.mFireEnergyChanged)
						{
							mComponent->setEnergy(entry.mFireEnergy);
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
