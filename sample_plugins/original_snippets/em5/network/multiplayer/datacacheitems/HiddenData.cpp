// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/network/multiplayer/datacache/datacacheitems/HiddenData.h"
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
		HiddenData::HiddenData(const qsf::Entity& targetEntity) :
			DataCacheComponentExistenceTrackingBase<qsf::game::HiddenComponent>(targetEntity),
			mHiddenType(0),
			mHiddenTypeChanged(false)
		{
			const qsf::game::HiddenComponent* hiddenComponent = getComponent(targetEntity);
			if (nullptr != hiddenComponent)
			{
				mHiddenType = static_cast<uint8>(hiddenComponent->getHiddenType());
			}
		}

		HiddenData::~HiddenData()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::DataCacheBase methods ]
		//[-------------------------------------------------------]
		bool HiddenData::prepareForUpdate(const qsf::Entity& targetEntity, const qsf::Clock& clock)
		{
			// Call base implementation
			const bool componentExistenceChanged = DataCacheComponentExistenceTrackingBase<qsf::game::HiddenComponent>::prepareForUpdate(targetEntity, clock);

			// Check for component data changes, when component exists
			const qsf::game::HiddenComponent* hiddenComponent = getComponent(targetEntity);
			if (nullptr != hiddenComponent )
			{
				const uint8 newHiddtenType = static_cast<uint8>(hiddenComponent->getHiddenType());

				mHiddenTypeChanged = componentExistenceChanged || newHiddtenType != mHiddenType;
				if (mHiddenTypeChanged)
				{
					mHiddenType = newHiddtenType;
				}

				return mHiddenTypeChanged || componentExistenceChanged;
			}
			else if (componentExistenceChanged)
			{
				// Only do an reset if the component existence has changed and the component was destroyed
				// Reset cache data
				mHiddenTypeChanged = false;
				mHiddenType = 0;
			}

			return componentExistenceChanged;
		}

		void HiddenData::updateData(qsf::game::BitStream& bitStream, bool force)
		{
			// When this method is called at least one tracked data by this class has changed
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getBitLength();
#endif

			// Call base implementation
			DataCacheComponentExistenceTrackingBase<qsf::game::HiddenComponent>::updateData(bitStream, force);

			// Only write component data when the entity has the component
			if (hasEntityComponent())
			{
				bitStream.write(mHiddenTypeChanged);
				if (mHiddenTypeChanged)
				{
					// The hidden type has a value range of 0-3 => can be represented by 2 bits
					bitStream.write(mHiddenType, 2);
				}
			}

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getBitLength();
			QSF_LOG_PRINTS(INFO, "HiddenData: written bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void HiddenData::setData(const qsf::game::BitStream& bitStream, const int32 receivedHostTickCount)
		{
			// When this method is called at least one tracked data by this class has changed on the host side
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getReadPosition();
#endif
			// Call base implementation
			DataCacheComponentExistenceTrackingBase<qsf::game::HiddenComponent>::setData(bitStream, receivedHostTickCount);

			// Read the component data when the component exists
			if (hasEntityComponentFromStream())
			{
				HistoryEntry entry;
				entry.tickCount = receivedHostTickCount;

				entry.mHiddenTypeChanged = detail::checkChangedAndRead(bitStream, 2, entry.mHiddenType);

				mHistoryList.emplace_back(entry);
			}

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getReadPosition();
			QSF_LOG_PRINTS(INFO, "HiddenData: read bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void HiddenData::interpolate(qsf::Entity& targetEntity, const qsf::Clock& clock, const int32 receivedHostTickCount)
		{
			// Call base implementation
			DataCacheComponentExistenceTrackingBase<qsf::game::HiddenComponent>::interpolate(targetEntity, clock, receivedHostTickCount);

			if (!mHistoryList.empty())
			{
				qsf::game::HiddenComponent* hiddenComponent = getComponent(targetEntity);
				if (nullptr != hiddenComponent )
				{
					auto iterator = mHistoryList.begin();
					while (iterator != mHistoryList.end())
					{
						const HistoryEntry& entry = *iterator;
						if (entry.tickCount == clock.getSignalCounter())
						{
							if (entry.mHiddenTypeChanged)
							{
								// The data has changed -> apply it
								hiddenComponent->setHiddenType(static_cast<qsf::game::HiddenComponent::HiddenType>(entry.mHiddenType));
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
