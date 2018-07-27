// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "game_plugin/PrecompiledHeader.h"
#include "game_plugin/multiplayer/IndicatorData.h"
#include "game_plugin/component/IndicatorComponent.h"

#include <em5/network/multiplayer/datacache/DataCacheItemHelper.h>

#include <qsf_game/network/BitStream.h>

#include <qsf/log/LogSystem.h>
#include <qsf/map/Entity.h>
#include <qsf/time/clock/Clock.h>


// You might want to enable this for debugging
//#define ENABLE_MULTIPLAYER_CACHE_DEBUG


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace user
{
	namespace multiplayer
	{


		// Easy access to "em5::multiplayer::detail" and others
		using namespace em5::multiplayer;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		IndicatorData::IndicatorData(const qsf::Entity& targetEntity) :
			DataCacheComponentExistenceTrackingBase<IndicatorComponent>(targetEntity),
			mColorChanged(false),
			mColor(0),
			mIsAnimated(false)
		{
			IndicatorComponent* component = targetEntity.getComponent<IndicatorComponent>();
			if (nullptr != component)
			{
				mColor = component->getColor();
				mIsAnimated = component->isAnimated();
			}
		}

		IndicatorData::~IndicatorData()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::DataCacheBase methods ]
		//[-------------------------------------------------------]
		bool IndicatorData::includeForPrepare(const int32 currentTickCount, bool entityIsVisible)
		{
			// Only update every 500 ms
			return currentTickCount % 5 == 0;
		}

		bool IndicatorData::prepareForUpdate(const qsf::Entity& targetEntity, const qsf::Clock& clock)
		{
			// Call base implementation
			bool componentExistenceChanged = DataCacheComponentExistenceTrackingBase<IndicatorComponent>::prepareForUpdate(targetEntity, clock);

			// Check for component data changes, when component exists
			const IndicatorComponent* indicatorComponent = getComponent(targetEntity);
			if (nullptr != indicatorComponent)
			{
				// Sync everything whenever one of the two properties have changed
				mColorChanged = detail::checkForChanges(static_cast<uint32>(indicatorComponent->getColor()), mColor);
				bool animatedChanged = detail::checkForChanges(indicatorComponent->isAnimated(), mIsAnimated);

				return (mColorChanged || animatedChanged);
			}
			else if (componentExistenceChanged)
			{
				// Only do an reset if the component existence has changed and the component was destroyed
				mColorChanged = false;
				mColor = 0;
				mIsAnimated = false;
				return true;
			}

			return false;
		}

		void IndicatorData::updateData(qsf::game::BitStream& bitStream, bool force)
		{
			// When this method is called at least one tracked data by this class has changed
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getBitLength();
#endif
			// Call base implementation
			DataCacheComponentExistenceTrackingBase<IndicatorComponent>::updateData(bitStream, force);

			// Only write component data when the entity has the component
			if (hasEntityComponent())
			{
				// Write color only when it has actually changed
				detail::writeChanges(mColorChanged, mColor, bitStream);

				// Write animated flag in any case
				//  -> It makes no real sense to first write a bool to tell us whether it changed, because the animated flag itself is only a single bit
				bitStream.write(mIsAnimated);
			}

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getBitLength();
			QSF_LOG_PRINTS(INFO, "IndicatorData: Written bitcount for entity \"" << mEntityId << "\": " << end-init);
#endif
		}

		void IndicatorData::setData(const qsf::game::BitStream& bitStream, const int32 receivedHostTickCount)
		{
			// When this method is called at least one tracked data by this class has changed on the host side
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getReadPosition();
#endif
			// Call base implementation
			DataCacheComponentExistenceTrackingBase<IndicatorComponent>::setData(bitStream, receivedHostTickCount);

			// Read the component data when the component exists
			if (hasEntityComponentFromStream())
			{
				HistoryEntry entry;
				entry.mTickCount = receivedHostTickCount;

				entry.mColorChanged = detail::checkChangedAndRead(bitStream, entry.mColor);
				entry.mIsAnimated = detail::checkChanged(bitStream);

				mHistoryList.emplace_back(entry);
			}

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getReadPosition();
			QSF_LOG_PRINTS(INFO, "IndicatorData: read bitcount for entity \"" << mEntityId << "\": " << end-init);
#endif
		}

		void IndicatorData::interpolate(qsf::Entity& targetEntity, const qsf::Clock& clock, const int32 receivedHostTickCount)
		{
			// Call base implementation
			DataCacheComponentExistenceTrackingBase<IndicatorComponent>::interpolate(targetEntity, clock, receivedHostTickCount);

			if (!mHistoryList.empty())
			{
				IndicatorComponent* indicatorComponent = getComponent(targetEntity);
				if (nullptr != indicatorComponent)
				{
					auto iterator = mHistoryList.begin();
					while (iterator != mHistoryList.end())
					{
						const HistoryEntry& entry = *iterator;
						if (entry.mTickCount == clock.getSignalCounter())
						{
							if (entry.mColorChanged)
							{
								indicatorComponent->setColor(static_cast<IndicatorComponent::Color>(entry.mColor));
							}
							indicatorComponent->setIsAnimated(entry.mIsAnimated);

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
} // user
