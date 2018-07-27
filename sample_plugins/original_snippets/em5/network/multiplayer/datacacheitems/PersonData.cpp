// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/network/multiplayer/datacache/datacacheitems/PersonData.h"
#include "em5/network/multiplayer/datacache/DataCacheItemHelper.h"
#include "em5/component/person/PersonComponent.h"

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
		PersonData::PersonData(const qsf::Entity& targetEntity) :
			DataCacheBase(targetEntity),
			mIsGangster(false),
			mHasHandcuffs(false)
		{
			mComponent = targetEntity.getComponent<PersonComponent>();
			if (mComponent.valid())
			{
				mIsGangster = mComponent->getGangsterFlag();
				mHasHandcuffs = mComponent->getHandcuffedFlag();
			}
			else
			{
				QSF_ERROR("PersonData: The entity \""<<targetEntity.getId()<<"\" doesn't have a person component for caching the data by this class", QSF_REACT_THROW);
			}
		}

		PersonData::~PersonData()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::DataCacheBase methods ]
		//[-------------------------------------------------------]
		bool PersonData::includeForPrepare(const int32 currentTickCount, bool entityIsVisible)
		{
			// Only update each 500 ms
			return currentTickCount % 10 == 0;
		}

		bool PersonData::prepareForUpdate(const qsf::Entity& targetEntity, const qsf::Clock& clock)
		{
			if (mComponent.valid())
			{
				bool gangsterChanged = detail::checkForChanges(mComponent->getGangsterFlag(), mIsGangster);
				bool handcuffedChanged = detail::checkForChanges(mComponent->getHandcuffedFlag(), mHasHandcuffs);
				return gangsterChanged || handcuffedChanged;
			}

			return false;
		}

		void PersonData::updateData(qsf::game::BitStream& bitStream, bool force)
		{
			// When this method is called at least one tracked data by this class has changed
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getBitLength();
#endif
			bitStream.write(mIsGangster);
			bitStream.write(mHasHandcuffs);

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getBitLength();
			QSF_LOG_PRINTS(INFO, "PersonData: written bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void PersonData::setData(const qsf::game::BitStream& bitStream, const int32 receivedHostTickCount)
		{
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getReadPosition();
#endif
			HistoryEntry entry;
			entry.tickCount = receivedHostTickCount;
			entry.mIsGangster = detail::checkChanged(bitStream);
			entry.mHasHandcuffs = detail::checkChanged(bitStream);
			mHistoryList.emplace_back(entry);

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getReadPosition();
			QSF_LOG_PRINTS(INFO, "PersonData: read bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void PersonData::interpolate(qsf::Entity& targetEntity, const qsf::Clock& clock, const int32 receivedHostTickCount)
		{
			if (!mHistoryList.empty() && mComponent.valid())
			{
				auto iterator = mHistoryList.begin();
				while (iterator != mHistoryList.end())
				{
					const HistoryEntry& entry = *iterator;
					if (entry.tickCount == clock.getSignalCounter())
					{
						mComponent->setGangsterFlag(entry.mIsGangster);
						mComponent->setPersonFlags(PersonComponent::FLAG_HANDCUFFED, entry.mHasHandcuffs);

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
