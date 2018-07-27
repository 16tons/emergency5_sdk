// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/network/multiplayer/datacache/datacacheitems/TransformData.h"
#include "em5/network/multiplayer/datacache/DataCacheItemHelper.h"
#include "em5/component/person/PersonComponent.h"

#include <qsf_game/network/BitStream.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/link/LinkComponent.h>
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
		TransformData::TransformData(const qsf::Entity& targetEntity) :
			DataCacheBase(targetEntity),
			mInitialTransfer(true),
			mPositionChanged(false),
			mRotationChanged(false)
		{
			mComponent = targetEntity.getComponent<qsf::TransformComponent>();
			if (mComponent.valid())
			{
				mLastRotation = mComponent->getRotation();
				mLastPosition = mComponent->getPosition();
			}
			else
			{
				QSF_ERROR("TransformData: The entity \""<<targetEntity.getId()<<"\" doesn't have a TransformComponent for caching the data by this class", QSF_REACT_THROW);
			}
		}

		TransformData::~TransformData()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::DataCacheBase methods ]
		//[-------------------------------------------------------]
		bool TransformData::prepareForUpdate(const qsf::Entity& targetEntity, const qsf::Clock& clock)
		{
			if (mComponent.valid())
			{
				if (mInitialTransfer)
				{
					mPositionChanged = detail::checkForChanges(mComponent->getPosition(), mLastPosition);
					mRotationChanged = detail::checkForChanges(mComponent->getRotation(), mLastRotation);
					mInitialTransfer = false;

					// The component was created the first time -> send initial transform information
					return mPositionChanged || mRotationChanged;
				}

				if (isEntityLinked(targetEntity))
				{
					// Do nothing when entity is linked
					return false;
				}

				mPositionChanged = detail::checkForChanges(mComponent->getPosition(), mLastPosition);
				mRotationChanged = detail::checkForChanges(mComponent->getRotation(), mLastRotation);
				return mPositionChanged || mRotationChanged;
			}
			else
			{
				// The entity has no movable component anymore -> This shouldn't never happen. An instance of TransformData for such an entity should never be created
				QSF_ERROR("TransformData: The entity \""<<targetEntity.getId()<<"\" lost its transform components this instance is not longer usable for that entity", QSF_REACT_NONE);
			}

			return false;
		}

		void TransformData::updateData(qsf::game::BitStream& bitStream, bool force)
		{
			// When this method is called at least one tracked data by this class has changed
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getBitLength();
#endif

			detail::writeChanges(mPositionChanged, mLastPosition, bitStream);
			detail::writeChanges(mRotationChanged, mLastRotation, bitStream);


#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getBitLength();
			QSF_LOG_PRINTS(INFO, "TransformData: written bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void TransformData::setData(const qsf::game::BitStream& bitStream, const int32 receivedHostTickCount)
		{
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getReadPosition();
#endif
			HistoryEntry entry;
			entry.tickCount = receivedHostTickCount;

			entry.positionChanged = detail::checkChangedAndRead(bitStream, entry.position);
			entry.rotationChanged = detail::checkChangedAndRead(bitStream, entry.rotation);

			mHistoryList.emplace_back(entry);

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getReadPosition();
			QSF_LOG_PRINTS(INFO, "TransformData: read bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void TransformData::interpolate(qsf::Entity& targetEntity, const qsf::Clock& clock, const int32 receivedHostTickCount)
		{
			if (!mHistoryList.empty() && mComponent.valid())
			{
				auto iterator = mHistoryList.begin();
				while (iterator != mHistoryList.end())
				{
					if (iterator->tickCount == clock.getSignalCounter())
					{
						const HistoryEntry& historyEntry = *iterator;

						// Apply the read data
						if (historyEntry.positionChanged && !historyEntry.rotationChanged)
						{
							mComponent->setPosition(historyEntry.position);
						}
						else if (!historyEntry.positionChanged && historyEntry.rotationChanged)
						{
							mComponent->setRotation(historyEntry.rotation);
						}
						else if (historyEntry.positionChanged && historyEntry.rotationChanged)
						{
							mComponent->setPositionAndRotation(historyEntry.position, historyEntry.rotation);
						}

						iterator = mHistoryList.erase(iterator);
					}
					else
					{
						// The entry is not for the current tick -> no further processing possible
						break;
					}
				}
			}
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		bool TransformData::isEntityLinked(const qsf::Entity& entity) const
		{
			qsf::LinkComponent* linkComponent = entity.getComponent<qsf::LinkComponent>();
			return (nullptr != linkComponent && qsf::isInitialized(linkComponent->getParentId()));
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5
