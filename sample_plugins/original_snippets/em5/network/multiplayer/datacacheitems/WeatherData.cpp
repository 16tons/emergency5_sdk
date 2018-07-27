// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/network/multiplayer/datacache/datacacheitems/WeatherData.h"
#include "em5/network/multiplayer/datacache/DataCacheItemHelper.h"

#include <qsf_game/environment/weather/WeatherComponent.h>
#include <qsf_game/environment/weather/WeatherState.h>
#include <qsf_game/environment/weather/WeatherSystem.h>
#include <qsf_game/QsfGameHelper.h>
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
		WeatherData::WeatherData(const qsf::Entity& targetEntity) :
			DataCacheBase(targetEntity),
			mWeatherChanged(false)
		{
			mComponent = targetEntity.getComponent<qsf::game::WeatherComponent>();
			if (mComponent.valid())
			{
				const qsf::game::WeatherState& weatherState = mComponent->getTargetWeatherState();
				mCurrentWeatherId = qsf::StringHash(weatherState.getName()).getHash();
			}
			else
			{
				QSF_ERROR("WeatherData: The entity \""<<targetEntity.getId()<<"\" doesn't have a weather component for caching the data by this class", QSF_REACT_THROW);
			}
		}

		WeatherData::~WeatherData()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::DataCacheBase methods ]
		//[-------------------------------------------------------]
		bool WeatherData::prepareForUpdate(const qsf::Entity& targetEntity, const qsf::Clock& clock)
		{
			if (mComponent.valid())
			{
				mWeatherChanged = detail::checkForChanges(qsf::StringHash(mComponent->getTargetWeatherState().getName()).getHash(), mCurrentWeatherId);

				return mWeatherChanged;
			}

			return false;
		}

		void WeatherData::updateData(qsf::game::BitStream& bitStream, bool force)
		{
			// When this method is called at least one tracked data by this class has changed
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getBitLength();
#endif
			bitStream.write(mWeatherChanged);

			if (mWeatherChanged)
			{
				bitStream.write(mCurrentWeatherId);
			}

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getBitLength();
			QSF_LOG_PRINTS(INFO, "WeatherData: written bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void WeatherData::setData(const qsf::game::BitStream& bitStream, const int32 receivedHostTickCount)
		{
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getReadPosition();
#endif
			// Read the data from the stream and apply it to the target
			if (detail::checkChanged(bitStream))
			{
				HistoryEntry entry;
				entry.tickCount = receivedHostTickCount;

				if (!bitStream.read(entry.mCurrentWeatherId))
				{
					QSF_ERROR("WeatherData: could not read value from stream", QSF_REACT_THROW);
				}

				mHistoryList.emplace_back(entry);
			}

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getReadPosition();
			QSF_LOG_PRINTS(INFO, "WeatherData: read bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void WeatherData::interpolate(qsf::Entity& targetEntity, const qsf::Clock& clock, const int32 receivedHostTickCount)
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

						qsf::game::WeatherSystem::WeatherStateManager& weatherStateManager = QSFGAME_WEATHER.getWeatherStateManager();
						qsf::game::WeatherState* weatherState = weatherStateManager.findElement(entry.mCurrentWeatherId);
						if (nullptr != weatherState)
						{
							if (mComponent->getRestFadeTime() >= qsf::Time::fromSeconds(1.0f))
							{
								// If the weather transition is already active, force start
								mComponent->setWeatherState(*weatherState);
							}
							else
							{
								mComponent->fadeToWeatherState(*weatherState, weatherState->getChangeDuration());
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


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5
