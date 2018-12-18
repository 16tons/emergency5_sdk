// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/network/multiplayer/datacache/datacacheitems/ParticleData.h"
#include "em5/network/multiplayer/datacache/DataCacheItemHelper.h"

#include <qsf_game/network/BitStream.h>

#include <qsf/log/LogSystem.h>
#include <qsf/map/Entity.h>
#include <qsf/renderer/particles/ParticlesComponent.h>
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
		ParticleData::ParticleData(const qsf::Entity& targetEntity) :
			DataCacheBase(targetEntity),
			mActive(false),
			mEmitterEnabled(false)
		{
			mComponent = targetEntity.getComponent<qsf::ParticlesComponent>();
			if (mComponent.valid())
			{
				mActive = mComponent->isActive();
				mEmitterEnabled = mComponent->getEmitterEnabled();
			}
			else
			{
				QSF_ERROR("ParticleData: The entity \""<<targetEntity.getId()<<"\" doesn't have a particles component for caching the data by this class", QSF_REACT_THROW);
			}
		}

		ParticleData::~ParticleData()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::DataCacheBase methods ]
		//[-------------------------------------------------------]
		bool ParticleData::includeForPrepare(const int32 currentTickCount, bool entityIsVisible)
		{
			if (mComponent.valid())
			{
				// Update only each 150 ms
				return currentTickCount % 3 == 0;
			}
			return false;
		}

		bool ParticleData::prepareForUpdate(const qsf::Entity& targetEntity, const qsf::Clock& clock)
		{
			if (mComponent.valid())
			{
				const bool activeChanged = detail::checkForChanges(mComponent->isActive(), mActive);
				const bool emitterEnabledChanged = detail::checkForChanges(mComponent->getEmitterEnabled(), mEmitterEnabled);
				return activeChanged || emitterEnabledChanged;
			}

			return false;
		}

		void ParticleData::updateData(qsf::game::BitStream& bitStream, bool force)
		{
			// When this method is called at least one tracked data by this class has changed
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getBitLength();
#endif
			bitStream.write(mActive);
			bitStream.write(mEmitterEnabled);

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getBitLength();
			QSF_LOG_PRINTS(INFO, "ParticleData: written bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void ParticleData::setData(const qsf::game::BitStream& bitStream, const int32 receivedHostTickCount)
		{
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getReadPosition();
#endif
			HistoryEntry entry;
			entry.tickCount = receivedHostTickCount;
			entry.mActive = detail::checkChanged(bitStream);
			entry.mEmitterEnabled = detail::checkChanged(bitStream);
			mHistoryList.emplace_back(entry);

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getReadPosition();
			QSF_LOG_PRINTS(INFO, "ParticleData: read bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void ParticleData::interpolate(qsf::Entity& targetEntity, const qsf::Clock& clock, const int32 receivedHostTickCount)
		{
			if (!mHistoryList.empty() && mComponent.valid())
			{
				auto iterator = mHistoryList.begin();
				while (iterator != mHistoryList.end())
				{
					const HistoryEntry& entry = *iterator;
					if (entry.tickCount == clock.getSignalCounter())
					{
						if (mComponent->isActive() != entry.mActive)
						{
							mComponent->setActive(entry.mActive);
						}

						if (mComponent->getEmitterEnabled() != entry.mEmitterEnabled)
						{
							mComponent->setEmitterEnabled(entry.mEmitterEnabled);
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
