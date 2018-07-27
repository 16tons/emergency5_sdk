// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/network/multiplayer/datacache/datacacheitems/TimeOfDayData.h"
#include "em5/network/multiplayer/datacache/DataCacheItemHelper.h"

#include <qsf_game/environment/time/TimeOfDayComponent.h>
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
		TimeOfDayData::TimeOfDayData(const qsf::Entity& targetEntity) :
			DataCacheBase(targetEntity),
			mTimeOfDayChanged(false)
		{
			mComponent = targetEntity.getComponent<qsf::game::TimeOfDayComponent>();
			if (mComponent.valid())
			{
				mCurrentTimeOfDay = packTimeOfDay(mComponent->getTimeAsGlmVec2());
			}
			else
			{
				QSF_ERROR("TimeOfDayData: The entity \""<<targetEntity.getId()<<"\" doesn't have a time of day component for caching the data by this class", QSF_REACT_THROW);
			}
		}

		TimeOfDayData::~TimeOfDayData()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::DataCacheBase methods ]
		//[-------------------------------------------------------]
		bool TimeOfDayData::prepareForUpdate(const qsf::Entity& targetEntity, const qsf::Clock& clock)
		{
			if (mComponent.valid())
			{
				mTimeOfDayChanged = detail::checkForChanges(packTimeOfDay(mComponent->getTimeAsGlmVec2()), mCurrentTimeOfDay);

				return mTimeOfDayChanged;
			}

			return false;
		}

		void TimeOfDayData::updateData(qsf::game::BitStream& bitStream, bool force)
		{
			// When this method is called at least one tracked data by this class has changed
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getBitLength();
#endif
			bitStream.write(mTimeOfDayChanged);

			if (mTimeOfDayChanged)
			{
				bitStream.write(mCurrentTimeOfDay, 17);
			}

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getBitLength();
			QSF_LOG_PRINTS(INFO, "TimeOfDayData: written bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void TimeOfDayData::setData(const qsf::game::BitStream& bitStream, const int32 receivedHostTickCount)
		{
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getReadPosition();
#endif
			// Read the data from the stream and apply it to the target
			if (detail::checkChanged(bitStream))
			{
				HistoryEntry entry;
				entry.tickCount = receivedHostTickCount;

				uint32 timeValue;
				if (!bitStream.read(timeValue, 17))
				{
					QSF_ERROR("TimeOfDayData: could not read value from stream", QSF_REACT_THROW);
				}

				entry.mCurrentTimeOfDay = unpackTimeOfDay(timeValue);

				mHistoryList.emplace_back(entry);
			}

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getReadPosition();
			QSF_LOG_PRINTS(INFO, "TimeOfDayData: read bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void TimeOfDayData::interpolate(qsf::Entity& targetEntity, const qsf::Clock& clock, const int32 receivedHostTickCount)
		{
			if (!mHistoryList.empty() && mComponent.valid())
			{
				auto iterator = mHistoryList.begin();
				while (iterator != mHistoryList.end())
				{
					const HistoryEntry& entry = *iterator;
					if (entry.tickCount == clock.getSignalCounter())
					{
						// The data has changed -> apply it
						mComponent->setTimeAsGlmVec2(entry.mCurrentTimeOfDay);

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

		uint32 TimeOfDayData::packTimeOfDay(glm::vec2 time)
		{
			// The values of time of day is limited to 0-23 for hours, 0-59 for minutes and 0-59 for seconds
			// These values can be packt into 17 bits (5 Bits for the hour value, 6 Bits for the minute value and 6 Bits for the second value)
			const uint8 hourValue = static_cast<uint8>(time.x);
			const uint8 minuteValue = static_cast<uint8>(time.y);
			const uint8 secondsValue = static_cast<uint8>((time.y-minuteValue)*60);

			return (hourValue<<12) + (minuteValue<<6) + secondsValue;
		}

		glm::vec2 TimeOfDayData::unpackTimeOfDay(uint32 packedTime)
		{
			// The values of time of day is limited to 0-23 for hours, 0-59 for minutes and 0-59 for seconds
			// These values can be packt into 17 bits (5 Bits for the hour value, 6 Bits for the minute value and 6 Bits for the second value)
			// The value 0x3f is used to unset the additional upper two bits for the minute/second value (because these bits are part of the hour/minute value)
			const uint8 hourValue = static_cast<uint8>(packedTime>>12);
			const uint8 minuteValue = static_cast<uint8>(packedTime>>6 & 0x3f);
			const uint8 secondsValue = static_cast<uint8>(packedTime & 0x3f);

			return glm::vec2(hourValue, minuteValue+secondsValue/60.0f);
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5
