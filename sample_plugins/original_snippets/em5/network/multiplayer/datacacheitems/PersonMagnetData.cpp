// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/network/multiplayer/datacache/datacacheitems/PersonMagnetData.h"
#include "em5/network/multiplayer/datacache/DataCacheItemHelper.h"
#include "em5/component/movement/PersonMagnetComponent.h"

#include <qsf_game/network/BitStream.h>

#include <qsf/time/clock/Clock.h>
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
		PersonMagnetData::PersonMagnetData(const qsf::Entity& targetEntity) :
			DataCacheBase(targetEntity),
			mComponent(targetEntity.getComponent<PersonMagnetComponent>()),
			mCarrierEntityId(qsf::getUninitialized<uint64>()),
			mVisualizationRadius(0.0f)
		{
			if (mComponent.valid())
			{
				mCarrierEntityId = mComponent->getCarrierEntityId();
				mVisualizationRadius = mComponent->getVisualizationRadius();
			}
			else
			{
				QSF_ERROR("PersonMagnetData: The entity \""<<targetEntity.getId()<<"\" doesn't have a person magnet component for caching the data by this class", QSF_REACT_THROW);
			}
		}

		PersonMagnetData::~PersonMagnetData()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::DataCacheBase methods ]
		//[-------------------------------------------------------]
		bool PersonMagnetData::prepareForUpdate(const qsf::Entity& targetEntity, const qsf::Clock& clock)
		{
			if (mComponent.valid())
			{
				const bool hasCarrierEntityIdChanged = detail::checkForChanges(mComponent->getCarrierEntityId(), mCarrierEntityId);
				const bool hasVisualizationRadiusChanged = detail::checkForChanges(mComponent->getVisualizationRadius(), mVisualizationRadius);

				return hasCarrierEntityIdChanged || hasVisualizationRadiusChanged;
			}

			return false;
		}

		void PersonMagnetData::updateData(qsf::game::BitStream& bitStream, bool force)
		{
			// When this method is called at least one tracked data by this class has changed
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getBitLength();
#endif
			bitStream.write(mCarrierEntityId);
			bitStream.write(mVisualizationRadius);

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getBitLength();
			QSF_LOG_PRINTS(INFO, "PersonMagnetData: written bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void PersonMagnetData::setData(const qsf::game::BitStream& bitStream, const int32 receivedHostTickCount)
		{
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getReadPosition();
#endif
			HistoryEntry entry;
			entry.tickCount = receivedHostTickCount;
			bitStream.read(entry.mCarrierEntityId);
			bitStream.read(entry.mVisualizationRadius);
			mHistoryList.emplace_back(entry);

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getReadPosition();
			QSF_LOG_PRINTS(INFO, "HelicopterData: read bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void PersonMagnetData::interpolate(qsf::Entity& targetEntity, const qsf::Clock& clock, const int32 receivedHostTickCount)
		{
			if (!mHistoryList.empty() && mComponent.valid())
			{
				for (auto iterator = mHistoryList.begin(); iterator != mHistoryList.end(); )
				{
					const HistoryEntry& entry = *iterator;
					if (entry.tickCount == clock.getSignalCounter())
					{
						if (entry.mCarrierEntityId != mComponent->getCarrierEntityId())
						{
							mComponent->setCarrierEntityId(entry.mCarrierEntityId);
						}

						if (entry.mVisualizationRadius != mComponent->getVisualizationRadius())
						{
							if (entry.mVisualizationRadius > 0.0f)
							{
								mComponent->enableVisualization(entry.mVisualizationRadius);
							}
							else
							{
								mComponent->disableVisualization();
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
