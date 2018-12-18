// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/network/multiplayer/datacache/datacacheitems/BoatData.h"
#include "em5/network/multiplayer/datacache/DataCacheItemHelper.h"
#include "em5/component/vehicle/BoatComponent.h"
#include "em5/map/EntityHelper.h"

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
		BoatData::BoatData(const qsf::Entity& targetEntity) :
			DataCacheBase(targetEntity),
			mRadarEnabled(false),
			mPropellerParticleEnabled(false),
			mLakeIdChanged(false),
			mLakeId(qsf::getUninitialized<uint64>())
		{
			mComponent = targetEntity.getComponent<BoatComponent>();
			if (mComponent.valid())
			{
				mRadarEnabled = mComponent->isRadarEnabled();
				mPropellerParticleEnabled = mComponent->isPropellerParticlesEnabled();
				mLakeId = mComponent->getLakeId();
			}
			else
			{
				QSF_ERROR("BoatData: The entity \"" << targetEntity.getId()<<"\" doesn't have a boat component for caching the data by this class", QSF_REACT_THROW);
			}
		}

		BoatData::~BoatData()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::DataCacheBase methods ]
		//[-------------------------------------------------------]
		bool BoatData::prepareForUpdate(const qsf::Entity& targetEntity, const qsf::Clock& clock)
		{
			if (mComponent.valid())
			{
				bool radarEnabledChanged = detail::checkForChanges(mComponent->isRadarEnabled(), mRadarEnabled);
				bool propellerParticleEnabledChanged = detail::checkForChanges(mComponent->isPropellerParticlesEnabled(), mPropellerParticleEnabled);
				mLakeIdChanged = detail::checkForChanges(mComponent->getLakeId(), mLakeId);

				return radarEnabledChanged || propellerParticleEnabledChanged || mLakeIdChanged;
			}

			return false;
		}

		void BoatData::updateData(qsf::game::BitStream& bitStream, bool force)
		{
			// When this method is called at least one tracked data by this class has changed
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getBitLength();
#endif
			bitStream.write(mRadarEnabled);
			bitStream.write(mPropellerParticleEnabled);

			detail::writeChanges(mLakeIdChanged, mLakeId, bitStream);

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getBitLength();
			QSF_LOG_PRINTS(INFO, "BoatData: written bitcount for entity \"" << mEntityId << "\": " << end-init);
#endif
		}

		void BoatData::setData(const qsf::game::BitStream& bitStream, const int32 receivedHostTickCount)
		{
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getReadPosition();
#endif
			HistoryEntry entry;
			entry.tickCount = receivedHostTickCount;
			entry.mRadarEnabled = detail::checkChanged(bitStream);
			entry.mPropellerParticleEnabled = detail::checkChanged(bitStream);
			entry.mLakeIdChanged = detail::checkChangedAndRead(bitStream, entry.mLakeId);
			mHistoryList.emplace_back(entry);

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getReadPosition();
			QSF_LOG_PRINTS(INFO, "BoatData: read bitcount for entity \"" << mEntityId << "\": " << end-init);
#endif
		}

		void BoatData::interpolate(qsf::Entity& targetEntity, const qsf::Clock& clock, const int32 receivedHostTickCount)
		{
			if (!mHistoryList.empty() && mComponent.valid())
			{
				auto iterator = mHistoryList.begin();
				while (iterator != mHistoryList.end())
				{
					const HistoryEntry& entry = *iterator;
					if (entry.tickCount == clock.getSignalCounter())
					{
						mComponent->enableRadar(entry.mRadarEnabled);
						mComponent->enablePropellerParticles(entry.mPropellerParticleEnabled);

						if (entry.mLakeIdChanged)
						{
							mComponent->setLakeId(entry.mLakeId);
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
