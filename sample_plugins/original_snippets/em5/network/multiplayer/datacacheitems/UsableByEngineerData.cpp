// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/network/multiplayer/datacache/datacacheitems/UsableByEngineerData.h"
#include "em5/network/multiplayer/datacache/DataCacheItemHelper.h"

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
		UsableByEngineerData::UsableByEngineerData(const qsf::Entity& targetEntity) :
			DataCacheComponentExistenceTrackingBase<UsableByEngineerComponent>(targetEntity),
			mUsed(false),
			mIsInUse(false)
		{
			const UsableByEngineerComponent* usableByEngineerComponent = getComponent(targetEntity);
			if (nullptr != usableByEngineerComponent)
			{
				mUsed = usableByEngineerComponent->getUsed();
				mIsInUse = usableByEngineerComponent->getIsInUse();
			}
		}

		UsableByEngineerData::~UsableByEngineerData()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::DataCacheBase methods ]
		//[-------------------------------------------------------]
		bool UsableByEngineerData::prepareForUpdate(const qsf::Entity& targetEntity, const qsf::Clock& clock)
		{
			// Call base implementation
			bool componentExistenceChanged = DataCacheComponentExistenceTrackingBase<UsableByEngineerComponent>::prepareForUpdate(targetEntity, clock);

			// Check for component data changes, when component exists
			const UsableByEngineerComponent* usableByEngineerComponent = getComponent(targetEntity);
			if (nullptr != usableByEngineerComponent)
			{
				bool usedChanged = detail::checkForChanges(usableByEngineerComponent->getUsed(), mUsed);
				bool isInUseChanged = detail::checkForChanges(usableByEngineerComponent->getIsInUse(), mIsInUse);
				return componentExistenceChanged || usedChanged || isInUseChanged;
			}

			return componentExistenceChanged;
		}

		void UsableByEngineerData::updateData(qsf::game::BitStream& bitStream, bool force)
		{
			// When this method is called at least one tracked data by this class has changed
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getBitLength();
#endif
			// Call base implementation
			DataCacheComponentExistenceTrackingBase<UsableByEngineerComponent>::updateData(bitStream, force);

			// Only write component data when the entity has the component
			if (hasEntityComponent())
			{
				bitStream.write(mUsed);
				bitStream.write(mIsInUse);
			}

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getBitLength();
			QSF_LOG_PRINTS(INFO, "UsableByEngineerData: written bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void UsableByEngineerData::setData(const qsf::game::BitStream& bitStream, const int32 receivedHostTickCount)
		{
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getReadPosition();
#endif
			// Call base implementation
			DataCacheComponentExistenceTrackingBase<UsableByEngineerComponent>::setData(bitStream, receivedHostTickCount);

			// Read the component data when the component exists
			if (hasEntityComponentFromStream())
			{
				HistoryEntry entry;
				entry.tickCount = receivedHostTickCount;
				entry.mUsed = detail::checkChanged(bitStream);
				entry.mIsInUse = detail::checkChanged(bitStream);
				mHistoryList.emplace_back(entry);
			}

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getReadPosition();
			QSF_LOG_PRINTS(INFO, "UsableByEngineerData: read bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void UsableByEngineerData::interpolate(qsf::Entity& targetEntity, const qsf::Clock& clock, const int32 receivedHostTickCount)
		{
			// Call base implementation
			DataCacheComponentExistenceTrackingBase<UsableByEngineerComponent>::interpolate(targetEntity, clock, receivedHostTickCount);

			UsableByEngineerComponent* usableByEngineerComponent = getComponent(targetEntity);
			if (nullptr != usableByEngineerComponent)
			{
				if (!mHistoryList.empty())
				{
					auto iterator = mHistoryList.begin();
					while (iterator != mHistoryList.end())
					{
						const HistoryEntry& entry = *iterator;
						if (entry.tickCount == clock.getSignalCounter())
						{

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
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5
