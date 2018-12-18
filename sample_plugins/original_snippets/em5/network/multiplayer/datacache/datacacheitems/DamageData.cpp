// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/network/multiplayer/datacache/datacacheitems/DamageData.h"
#include "em5/network/multiplayer/datacache/DataCacheItemHelper.h"
#include "em5/component/damage/DamageComponent.h"

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
		DamageData::DamageData(const qsf::Entity& targetEntity) :
			DataCacheBase(targetEntity),
			mDamageState(0),
			mDamageStateChanged(false)
		{
			mComponent = targetEntity.getComponent<DamageComponent>();
			if (mComponent.valid())
			{
				mDamageState = static_cast<uint8>(mComponent->getDamageState());
			}
			else
			{
				QSF_ERROR("DamageData: The entity \""<<targetEntity.getId()<<"\" doesn't have a damage component for caching the data by this class", QSF_REACT_THROW);
			}
		}

		DamageData::~DamageData()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::DataCacheBase methods ]
		//[-------------------------------------------------------]
		bool DamageData::prepareForUpdate(const qsf::Entity& targetEntity, const qsf::Clock& clock)
		{
			if (mComponent.valid())
			{
				const uint8 damageState = static_cast<uint8>(mComponent->getDamageState());
				mDamageStateChanged = detail::checkForChanges(damageState, mDamageState);

				return mDamageStateChanged;
			}

			return false;
		}

		void DamageData::updateData(qsf::game::BitStream& bitStream, bool force)
		{
			// When this method is called at least one tracked data by this class has changed
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getBitLength();
#endif

			// We have only 5 damage states (0-4) -> can be represented with 3 bits
			detail::writeChanges(mDamageStateChanged, mDamageState, 3, bitStream);

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getBitLength();
			QSF_LOG_PRINTS(INFO, "DamageData: written bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void DamageData::setData(const qsf::game::BitStream& bitStream, const int32 receivedHostTickCount)
		{
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getReadPosition();
#endif
			// Read the data from the stream
			// We have only 5 damage states (0-4) -> can be represented with 3 bits
			if (detail::checkChangedAndRead(bitStream, 3, mDamageState))
			{
				HistoryEntry entry;
				entry.tickCount = receivedHostTickCount;
				entry.mDamageState = mDamageState;

				mHistoryList.emplace_back(entry);
			}

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getReadPosition();
			QSF_LOG_PRINTS(INFO, "DamageData: read bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void DamageData::interpolate(qsf::Entity& targetEntity, const qsf::Clock& clock, const int32 receivedHostTickCount)
		{
			if (!mHistoryList.empty() && mComponent.valid())
			{
				auto iterator = mHistoryList.begin();
				while (iterator != mHistoryList.end())
				{
					const HistoryEntry& entry = *iterator;
					if (entry.tickCount == clock.getSignalCounter())
					{
						mComponent->setDamageState(static_cast<DamageComponent::DamageState>(entry.mDamageState));

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
