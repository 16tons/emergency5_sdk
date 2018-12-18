// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/network/multiplayer/datacache/datacacheitems/HelicopterData.h"
#include "em5/network/multiplayer/datacache/DataCacheItemHelper.h"
#include "em5/component/vehicle/HelicopterComponent.h"

#include <qsf_game/network/BitStream.h>

#include <qsf/component/base/TransformComponent.h>
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
		HelicopterData::HelicopterData(const qsf::Entity& targetEntity) :
			DataCacheBase(targetEntity),
			mIsFlying(false),
			mPositionYChanged(false),
			mPositionY(0.0f)
		{
			mComponent = targetEntity.getComponent<HelicopterComponent>();
			mTransformComponent = targetEntity.getComponent<qsf::TransformComponent>();
			if (mComponent.valid() && mTransformComponent.valid())
			{
				mIsFlying = mComponent->isFlying();
				mPositionY = mTransformComponent->getPosition().y;
			}
			else
			{
				QSF_ERROR("HelicopterData: The entity \""<<targetEntity.getId()<<"\" doesn't have a helicopter component for caching the data by this class", QSF_REACT_THROW);
			}
		}

		HelicopterData::~HelicopterData()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::DataCacheBase methods ]
		//[-------------------------------------------------------]
		bool HelicopterData::prepareForUpdate(const qsf::Entity& targetEntity, const qsf::Clock& clock)
		{
			if (mComponent.valid())
			{
				bool flyingChanged = detail::checkForChanges(mComponent->isFlying(), mIsFlying);

				if (mComponent->isStartingOrLanding() && mTransformComponent.valid())
				{
					mPositionYChanged = detail::checkForChanges(mTransformComponent->getPosition().y, mPositionY);
				}
				else
				{
					mPositionYChanged = false;
				}

				return flyingChanged || mPositionYChanged;
			}

			return false;
		}

		void HelicopterData::updateData(qsf::game::BitStream& bitStream, bool force)
		{
			// When this method is called at least one tracked data by this class has changed
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getBitLength();
#endif
			bitStream.write(mIsFlying);

			detail::writeChanges(mPositionYChanged, mPositionY, bitStream);

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getBitLength();
			QSF_LOG_PRINTS(INFO, "HelicopterData: written bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void HelicopterData::setData(const qsf::game::BitStream& bitStream, const int32 receivedHostTickCount)
		{
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getReadPosition();
#endif
			HistoryEntry entry;
			entry.tickCount = receivedHostTickCount;
			entry.mIsFlying = detail::checkChanged(bitStream);
			entry.mPositionYChanged = detail::checkChangedAndRead(bitStream, entry.mPositionY);
			mHistoryList.emplace_back(entry);

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getReadPosition();
			QSF_LOG_PRINTS(INFO, "HelicopterData: read bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void HelicopterData::interpolate(qsf::Entity& targetEntity, const qsf::Clock& clock, const int32 receivedHostTickCount)
		{
			if (!mHistoryList.empty() && mComponent.valid())
			{
				auto iterator = mHistoryList.begin();
				while (iterator != mHistoryList.end())
				{
					const HistoryEntry& entry = *iterator;
					if (entry.tickCount == clock.getSignalCounter())
					{
						mComponent->setFlying(entry.mIsFlying);

						if (entry.mPositionYChanged && mTransformComponent.valid())
						{
							glm::vec3 position = mTransformComponent->getPosition();
							position.y = entry.mPositionY;
							mTransformComponent->setPosition(position);
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
