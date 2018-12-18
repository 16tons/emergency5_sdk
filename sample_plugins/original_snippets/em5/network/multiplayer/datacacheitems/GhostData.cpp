// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/network/multiplayer/datacache/datacacheitems/GhostData.h"
#include "em5/network/multiplayer/datacache/DataCacheItemHelper.h"
#include "em5/game/selection/GroundMarkerComponent.h"

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
		GhostData::GhostData(const qsf::Entity& targetEntity) :
			DataCacheComponentExistenceTrackingBase<GhostComponent>(targetEntity),
			mPlayerId(qsf::getUninitialized<uint32>()),
			mPlayerIdChanged(false)
		{
			const GhostComponent* ghostComponent = getComponent(targetEntity);
			if (nullptr != ghostComponent)
			{
				mPlayerId = ghostComponent->getPlayerId();
			}
		}

		GhostData::~GhostData()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::DataCacheBase methods ]
		//[-------------------------------------------------------]
		bool GhostData::prepareForUpdate(const qsf::Entity& targetEntity, const qsf::Clock& clock)
		{
			// Call base implementation
			bool componentExistenceChanged = DataCacheComponentExistenceTrackingBase<GhostComponent>::prepareForUpdate(targetEntity, clock);

			// Check for component data changes, when component exists
			const GhostComponent* ghostComponent = getComponent(targetEntity);
			if (nullptr != ghostComponent)
			{
				mPlayerIdChanged = detail::checkForChanges(ghostComponent->getPlayerId(), mPlayerId);

				return componentExistenceChanged || mPlayerIdChanged;
			}
			else if (componentExistenceChanged)
			{
				// Only do an reset if the component existence has changed and the component was destroyed
				// Reset cache data
				qsf::setUninitialized(mPlayerId);
				mPlayerIdChanged = false;
			}

			return componentExistenceChanged;
		}

		void GhostData::updateData(qsf::game::BitStream& bitStream, bool force)
		{
			// When this method is called at least one tracked data by this class has changed
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getBitLength();
#endif
			// Call base implementation
			DataCacheComponentExistenceTrackingBase<GhostComponent>::updateData(bitStream, force);

			// Only write component data when the entity has the component
			if (hasEntityComponent())
			{
				detail::writeChanges(mPlayerIdChanged, mPlayerId, bitStream);
			}

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getBitLength();
			QSF_LOG_PRINTS(INFO, "GhostData: written bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void GhostData::setData(const qsf::game::BitStream& bitStream, const int32 receivedHostTickCount)
		{
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getReadPosition();
#endif
			// Call base implementation
			DataCacheComponentExistenceTrackingBase<GhostComponent>::setData(bitStream, receivedHostTickCount);

			// Read the component data when the component exists
			if (hasEntityComponentFromStream())
			{
				if (detail::checkChanged(bitStream))
				{
					HistoryEntry entry;
					entry.tickCount = receivedHostTickCount;

					bitStream.read(entry.mPlayerId);

					mHistoryList.emplace_back(entry);
				}
			}

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getReadPosition();
			QSF_LOG_PRINTS(INFO, "GhostData: read bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void GhostData::interpolate(qsf::Entity& targetEntity, const qsf::Clock& clock, const int32 receivedHostTickCount)
		{
			bool hadComponent = hasEntityComponent();
			// Call base implementation
			DataCacheComponentExistenceTrackingBase<GhostComponent>::interpolate(targetEntity, clock, receivedHostTickCount);

			GhostComponent* ghostComponent = getComponent(targetEntity);
			if (nullptr != ghostComponent)
			{
				if (!mHistoryList.empty())
				{
					auto iterator = mHistoryList.begin();
					while (iterator != mHistoryList.end())
					{
						const HistoryEntry& entry = *iterator;
						if (entry.tickCount == clock.getSignalCounter())
						{
							ghostComponent->setPlayerId(entry.mPlayerId);

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
			else if (hadComponent)
			{
				// Also destroy ground marker
				targetEntity.destroyComponent<GroundMarkerComponent>();
			}
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5
