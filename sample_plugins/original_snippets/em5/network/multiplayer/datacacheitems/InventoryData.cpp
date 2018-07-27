// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/network/multiplayer/datacache/datacacheitems/InventoryData.h"
#include "em5/network/multiplayer/datacache/DataCacheItemHelper.h"
#include "em5/component/equipment/InventoryComponent.h"

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
		InventoryData::InventoryData(const qsf::Entity& targetEntity) :
			DataCacheComponentExistenceTrackingBase<InventoryComponent>(targetEntity),
			mActiveEquipmentId(qsf::getUninitialized<uint64>()),
			mActiveEquipmentIdChanged(false)
		{
			const InventoryComponent* inventoryComponent = getComponent(targetEntity);
			if (nullptr != inventoryComponent)
			{
				mActiveEquipmentId = inventoryComponent->getActiveEquipmentId();
			}
		}

		InventoryData::~InventoryData()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::DataCacheBase methods ]
		//[-------------------------------------------------------]
		bool InventoryData::prepareForUpdate(const qsf::Entity& targetEntity, const qsf::Clock& clock)
		{
			// Call base implementation
			bool componentExistenceChanged = DataCacheComponentExistenceTrackingBase<InventoryComponent>::prepareForUpdate(targetEntity, clock);

			// Check for component data changes, when component exists
			const InventoryComponent* inventoryComponent = getComponent(targetEntity);
			if (nullptr != inventoryComponent )
			{
				mActiveEquipmentIdChanged = detail::checkForChanges(inventoryComponent->getActiveEquipmentId(), mActiveEquipmentId);
				return mActiveEquipmentIdChanged || componentExistenceChanged;
			}
			else
			{
				// Reset cached data
				mActiveEquipmentIdChanged = false;
				qsf::setUninitialized(mActiveEquipmentId);
			}

			return componentExistenceChanged;
		}

		void InventoryData::updateData(qsf::game::BitStream& bitStream, bool force)
		{
			// When this method is called at least one tracked data by this class has changed
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getBitLength();
#endif

			// Call base implementation
			DataCacheComponentExistenceTrackingBase<InventoryComponent>::updateData(bitStream, force);

			// Only write component data when the entity has the component
			if (hasEntityComponent())
			{
				detail::writeChanges(mActiveEquipmentIdChanged, mActiveEquipmentId, bitStream);
			}

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getBitLength();
			QSF_LOG_PRINTS(INFO, "Iventory: written bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void InventoryData::setData(const qsf::game::BitStream& bitStream, const int32 receivedHostTickCount)
		{
			// When this method is called at least one tracked data by this class has changed on the host side
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getReadPosition();
#endif

			// Call base implementation
			DataCacheComponentExistenceTrackingBase<InventoryComponent>::setData(bitStream, receivedHostTickCount);

			// Read the component data when the component exists
			if (hasEntityComponentFromStream())
			{
				HistoryEntry entry;
				entry.tickCount = receivedHostTickCount;

				entry.mActiveEquipmentIdChanged = detail::checkChangedAndRead(bitStream, entry.mActiveEquipmentId);

				mHistoryList.emplace_back(entry);
			}

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getReadPosition();
			QSF_LOG_PRINTS(INFO, "InventoryData: read bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void InventoryData::interpolate(qsf::Entity& targetEntity, const qsf::Clock& clock, const int32 receivedHostTickCount)
		{
			// Call base implementation
			DataCacheComponentExistenceTrackingBase<InventoryComponent>::interpolate(targetEntity, clock, receivedHostTickCount);

			if (!mHistoryList.empty())
			{
				InventoryComponent* inventoryComponent = getComponent(targetEntity);
				if (nullptr != inventoryComponent )
				{
					auto iterator = mHistoryList.begin();
					while (iterator != mHistoryList.end())
					{
						const HistoryEntry& entry = *iterator;
						if (entry.tickCount == clock.getSignalCounter())
						{
							if (entry.mActiveEquipmentIdChanged)
							{
								// The data has changed -> apply it
								// We set here only the id (needed e.g. for the ui), the linking is done via other transferred data
								inventoryComponent->setActiveEquipmentId(entry.mActiveEquipmentId);
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
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5
