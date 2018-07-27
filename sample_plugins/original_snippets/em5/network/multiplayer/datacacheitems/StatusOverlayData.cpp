// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/network/multiplayer/datacache/datacacheitems/StatusOverlayData.h"
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
		StatusOverlayData::StatusOverlayData(const qsf::Entity& targetEntity) :
			DataCacheComponentExistenceTrackingBase<StatusOverlayComponent>(targetEntity),
			mIconFlagsChanged(false),
			mIconFlags(0),
			mBarChanged(false),
			mVisibleBars(0)
		{
			const StatusOverlayComponent* statusOverlayComponent = getComponent(targetEntity);
			if (nullptr != statusOverlayComponent)
			{
				mIconFlags = statusOverlayComponent->getIconFlags();
			}
		}

		StatusOverlayData::~StatusOverlayData()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::DataCacheBase methods ]
		//[-------------------------------------------------------]
		bool StatusOverlayData::prepareForUpdate(const qsf::Entity& targetEntity, const qsf::Clock& clock)
		{
			// Call base implementation
			bool componentExistenceChanged = DataCacheComponentExistenceTrackingBase<StatusOverlayComponent>::prepareForUpdate(targetEntity, clock);

			// Check for component data changes, when component exists
			const StatusOverlayComponent* statusOverlayComponent = getComponent(targetEntity);
			if (nullptr != statusOverlayComponent)
			{
				mIconFlagsChanged = detail::checkForChanges(statusOverlayComponent->getIconFlags(), mIconFlags);
				const StatusOverlayComponent::BarMap& barMap = statusOverlayComponent->getBarValues();

				uint8 visibleBars = 0;
				mChangedBarValues.clear();
				if (!barMap.empty())
				{
					for (auto& barPair : barMap)
					{
						uint8 barType = static_cast<uint8>(barPair.first);
						uint8 barTypeIndex = 1<<barPair.first;
						uint8 barValue = static_cast<uint8>(barPair.second.mProgress * 100);
						visibleBars |= barTypeIndex;

						if (mVisibleBars & barTypeIndex)
						{
							uint8& currentValue = mCurrentBarValues[barType];

							// We know about the bar -> check if value has changed
							if (currentValue != barValue )
							{
								// Encode the bar type and the value (percentage) into 11 bits
								// First 8 bits are the value, the following 3 bits are the type
								mChangedBarValues.emplace_back(barValue+(barType<<8));
								currentValue = barValue;
							}
						}
						else
						{
							// We don't know nothing about the bar
							mChangedBarValues.emplace_back(barValue+(barType<<8));
							mCurrentBarValues.emplace(barType, barValue);
						}

					}
				}

				if (visibleBars != mVisibleBars)
				{
					mVisibleBars = visibleBars;
					mBarChanged = true;
				}

				mBarChanged = mBarChanged || !mChangedBarValues.empty();
				return mIconFlagsChanged || mBarChanged;
			}
			else if (componentExistenceChanged)
			{
				// Only do an reset if the component existence has changed and the component was destroyed
				// Reset cached data
				mIconFlagsChanged = false;
				mBarChanged = false;
				mVisibleBars = 0;
				mCurrentBarValues.clear();
				mChangedBarValues.clear();
			}

			return componentExistenceChanged;
		}

		void StatusOverlayData::updateData(qsf::game::BitStream& bitStream, bool force)
		{
			// When this method is called at least one tracked data by this class has changed
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getBitLength();
#endif
			// Call base implementation
			DataCacheComponentExistenceTrackingBase<StatusOverlayComponent>::updateData(bitStream, force);

			// Only write component data when the entity has the component
			if (hasEntityComponent())
			{
				detail::writeChanges(mIconFlagsChanged, mIconFlags, bitStream);
				bitStream.write(mBarChanged);
				if (mBarChanged)
				{
					bitStream.write(mVisibleBars);

					// Write if we have changed bar values
					bitStream.write(!mChangedBarValues.empty());

					if (!mChangedBarValues.empty())
					{
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
						size_t count = mChangedBarValues.size();
						QSF_LOG_PRINTS(INFO, "OverlayData: changed bar values size: "<<count);
#endif

						// Write count
						// We have currently max 5 bars => can be represented by 3 bits
						bitStream.writeAs<uint8>(mChangedBarValues.size(), 3);
						for (const uint16& value : mChangedBarValues)
						{
							bitStream.write(value, 11);
						}
					}
				}
			}

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getBitLength();
			QSF_LOG_PRINTS(INFO, "OverlayData: written bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void StatusOverlayData::setData(const qsf::game::BitStream& bitStream, const int32 receivedHostTickCount)
		{
			// When this method is called at least one tracked data by this class has changed on the host side
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getReadPosition();
#endif

			// Call base implementation
			DataCacheComponentExistenceTrackingBase<StatusOverlayComponent>::setData(bitStream, receivedHostTickCount);

			// Read the component data when the component exists
			if (hasEntityComponentFromStream())
			{
				HistoryEntry entry;
				entry.tickCount = receivedHostTickCount;
				entry.mIconFlagsChanged = detail::checkChangedAndRead(bitStream, entry.mIconFlags);
				entry.mBarChanged = detail::checkChanged(bitStream);
				if (entry.mBarChanged)
				{
					//size_t init3 = bitStream.getReadPosition();	// TODO(co) Variable is not used?
					if (!bitStream.read(entry.mVisibleBars))
					{
						QSF_ERROR("OverlayData: could not read data from stream", QSF_REACT_THROW);
					}

					if (detail::checkChanged(bitStream))
					{
						// Read counts
						size_t count;
						if (!bitStream.readAs<uint8>(count, 3))
						{
							QSF_ERROR("OverlayData: could not read data from stream", QSF_REACT_THROW);
						}

						entry.mChangedBarValues.reserve(count);

						for (size_t index = 0; index < count; ++index)
						{
							uint16 value;
							if (!bitStream.read(value, 11))
							{
								QSF_ERROR("OverlayData: could not read data from stream", QSF_REACT_THROW);
							}
							entry.mChangedBarValues.emplace_back(value);
						}
					}
				}
				mHistoryList.emplace_back(entry);
			}

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getReadPosition();
			QSF_LOG_PRINTS(INFO, "OverlayData: read bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void StatusOverlayData::interpolate(qsf::Entity& targetEntity, const qsf::Clock& clock, const int32 receivedHostTickCount)
		{
			// Call base implementation
			DataCacheComponentExistenceTrackingBase<StatusOverlayComponent>::interpolate(targetEntity, clock, receivedHostTickCount);

			if (!mHistoryList.empty())
			{
				StatusOverlayComponent* statusOverlayComponent = getComponent(targetEntity);
				if (nullptr != statusOverlayComponent)
				{
					auto iterator = mHistoryList.begin();
					while (iterator != mHistoryList.end())
					{
						const HistoryEntry& entry = *iterator;
						if (entry.tickCount == clock.getSignalCounter())
						{
							if (entry.mBarChanged)
							{
								// Remove hidden bars
								for (uint8 barType = 0; barType < StatusOverlayComponent::BarType::_BAR_COUNT; ++barType)
								{
									if (0 == (entry.mVisibleBars & (1<<barType)))
									{
										statusOverlayComponent->hideBarOverlay(static_cast<StatusOverlayComponent::BarType>(barType));
									}
								}

								// Update bar values
								for (uint16 barValueAndType : entry.mChangedBarValues)
								{
									// The bar type and the value (percentage) are encoded into 11 bits
									// First 8 bits are the value, the following 3 bits are the type
									uint8 value = static_cast<uint8>(barValueAndType);
									uint8 barType = static_cast<uint8>((barValueAndType>>8) & 0x07);
									statusOverlayComponent->showBarOverlay(static_cast<StatusOverlayComponent::BarType>(barType), value / 100.0f);
								}
							}

							if (entry.mIconFlagsChanged)
							{
								for (uint16 iconType = 0; iconType < StatusOverlayComponent::_STATUS_ICON_COUNT; ++iconType)
								{
									const bool show = (entry.mIconFlags & (static_cast<uint64>(1) << iconType)) != 0;
									statusOverlayComponent->showIcon(static_cast<StatusOverlayComponent::StatusIconType>(iconType), show);
								}
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
