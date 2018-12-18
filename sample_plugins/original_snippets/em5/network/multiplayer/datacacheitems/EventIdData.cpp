// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/network/multiplayer/datacache/datacacheitems/EventIdData.h"
#include "em5/network/multiplayer/datacache/DataCacheItemHelper.h"
#include "em5/freeplay/FreeplaySystem.h"
#include "em5/freeplay/event/FreeplayEventManager.h"
#include "em5/EM5Helper.h"

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
		EventIdData::EventIdData(const qsf::Entity& targetEntity) :
			DataCacheComponentExistenceTrackingBase<EventIdComponent>(targetEntity),
			mEventIdChanged(false),
			mEventId(qsf::getUninitialized<uint32>()),
			mCurrentTeamIdChanged(false),
			mCurrentTeamId(qsf::getUninitialized<uint8>())
		{
			const EventIdComponent* eventIdComponent = getComponent(targetEntity);
			if (nullptr != eventIdComponent)
			{
				mEventId = eventIdComponent->getEventId();
				mCurrentTeamId = eventIdComponent->getCurrentTeamId();
			}
		}

		EventIdData::~EventIdData()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::DataCacheBase methods ]
		//[-------------------------------------------------------]
		bool EventIdData::prepareForUpdate(const qsf::Entity& targetEntity, const qsf::Clock& clock)
		{
			// Call base implementation
			bool componentExistenceChanged = DataCacheComponentExistenceTrackingBase<EventIdComponent>::prepareForUpdate(targetEntity, clock);

			// Check for component data changes, when component exists
			const EventIdComponent* eventIdComponent = getComponent(targetEntity);
			if (nullptr != eventIdComponent)
			{
				mEventIdChanged = detail::checkForChanges(eventIdComponent->getEventId(), mEventId);
				mCurrentTeamIdChanged = detail::checkForChanges(eventIdComponent->getCurrentTeamId(), mCurrentTeamId);
				return mCurrentTeamIdChanged || mEventIdChanged || componentExistenceChanged;
			}
			else if (componentExistenceChanged)
			{
				// Only do an reset if the component existence has changed and the component was destroyed
				// Reset cached data
				qsf::setUninitialized(mEventId);
				qsf::setUninitialized(mCurrentTeamId);
			}

			return componentExistenceChanged;
		}

		void EventIdData::updateData(qsf::game::BitStream& bitStream, bool force)
		{
			// When this method is called at least one tracked data by this class has changed
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getBitLength();
#endif

			// Call base implementation
			DataCacheComponentExistenceTrackingBase<EventIdComponent>::updateData(bitStream, force);

			// Only write component data when the entity has the component
			if (hasEntityComponent())
			{
				detail::writeChanges(mEventIdChanged, mEventId, bitStream);
				detail::writeChanges(mCurrentTeamIdChanged, mCurrentTeamId, bitStream);
			}

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getBitLength();
			QSF_LOG_PRINTS(INFO, "EventIdData: written bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void EventIdData::setData(const qsf::game::BitStream& bitStream, const int32 receivedHostTickCount)
		{
			// When this method is called at least one tracked data by this class has changed on the host side
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getReadPosition();
#endif

			// Call base implementation
			DataCacheComponentExistenceTrackingBase<EventIdComponent>::setData(bitStream, receivedHostTickCount);

			// Read the component data when the component exists
			if (hasEntityComponentFromStream())
			{
				HistoryEntry entry;
				entry.tickCount = receivedHostTickCount;
				entry.mEventIdChanged = detail::checkChangedAndRead(bitStream, entry.mEventId);
				entry.mCurrentTeamIdChanged = detail::checkChangedAndRead(bitStream, entry.mCurrentTeamId);
				mHistoryList.emplace_back(entry);
			}

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getReadPosition();
			QSF_LOG_PRINTS(INFO, "EventIdData: read bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void EventIdData::interpolate(qsf::Entity& targetEntity, const qsf::Clock& clock, const int32 receivedHostTickCount)
		{
			// Call base implementation
			DataCacheComponentExistenceTrackingBase<EventIdComponent>::interpolate(targetEntity, clock, receivedHostTickCount);

			if (!mHistoryList.empty())
			{
				EventIdComponent* eventIdComponent = getComponent(targetEntity);
				if (nullptr != eventIdComponent)
				{
					auto iterator = mHistoryList.begin();
					while (iterator != mHistoryList.end())
					{
						const HistoryEntry& entry = *iterator;
						if (entry.tickCount == clock.getSignalCounter())
						{
							if (entry.mEventIdChanged)
							{
								FreeplayEvent* freeplayEvent = EM5_FREEPLAY.getEventManager().getEventById(entry.mEventId);
								if (nullptr != freeplayEvent)
								{
									eventIdComponent->setEvent(*freeplayEvent);
								}
							}

							if (entry.mCurrentTeamIdChanged)
							{
								eventIdComponent->setCurrentTeamId(entry.mCurrentTeamId);
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
