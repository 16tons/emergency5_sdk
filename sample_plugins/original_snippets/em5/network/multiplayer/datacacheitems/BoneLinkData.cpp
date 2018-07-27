// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/network/multiplayer/datacache/datacacheitems/BoneLinkData.h"
#include "em5/network/multiplayer/datacache/DataCacheItemHelper.h"

#include <qsf_game/component/base/BoneLinkComponent.h>
#include <qsf_game/network/BitStream.h>

#include <qsf/log/LogSystem.h>
#include <qsf/map/Entity.h>
#include <qsf/math/Math.h>


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
		BoneLinkData::BoneLinkData(const qsf::Entity& targetEntity) :
			DataCacheComponentExistenceTrackingBase<qsf::game::BoneLinkComponent>(targetEntity),
			mLocalRotationChanged(false),
			mLocalPositionChanged(false),
			mBoneNameChanged(false)
		{
			const qsf::game::BoneLinkComponent* boneLinkComponent = getComponent(targetEntity);
			if (nullptr != boneLinkComponent)
			{
				mLocalRotation = boneLinkComponent->getLocalRotation();
				mLocalPosition = boneLinkComponent->getLocalPosition();
				mBoneName = boneLinkComponent->getBoneName();
			}
		}

		BoneLinkData::~BoneLinkData()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::DataCacheBase methods ]
		//[-------------------------------------------------------]
		bool BoneLinkData::prepareForUpdate(const qsf::Entity& targetEntity, const qsf::Clock& clock)
		{
			// Call base implementation
			bool componentExistenceChanged = DataCacheComponentExistenceTrackingBase<qsf::game::BoneLinkComponent>::prepareForUpdate(targetEntity, clock);

			// Check for component data changes, when component exists
			const qsf::game::BoneLinkComponent* boneLinkComponent = getComponent(targetEntity);
			if (nullptr != boneLinkComponent)
			{
				mLocalRotationChanged = detail::checkForChanges(boneLinkComponent->getLocalRotation(), mLocalRotation);
				mLocalPositionChanged = detail::checkForChanges(boneLinkComponent->getLocalPosition(), mLocalPosition);
				mBoneNameChanged = detail::checkForChanges(boneLinkComponent->getBoneName(), mBoneName);

				return componentExistenceChanged || mLocalRotationChanged || mLocalPositionChanged || mBoneNameChanged;
			}
			else if (componentExistenceChanged)
			{
				// Only do an reset if the component existence has changed and the component was destroyed
				// Reset cache data
				mLocalRotationChanged = false;
				mLocalPositionChanged = false;
				mBoneNameChanged = false;
				mLocalRotation = qsf::Math::GLM_QUAT_IDENTITY;
				mLocalPosition = qsf::Math::GLM_VEC3_ZERO;
				mBoneName.clear();
			}

			return componentExistenceChanged;
		}

		void BoneLinkData::updateData(qsf::game::BitStream& bitStream, bool force)
		{
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getBitLength();
#endif
			// When this method is called at least one tracked data by this class has changed

			// Call base implementation
			DataCacheComponentExistenceTrackingBase<qsf::game::BoneLinkComponent>::updateData(bitStream, force);

			// Only write component data when the entity has the component
			if (hasEntityComponent())
			{
				detail::writeChanges(mLocalRotationChanged, mLocalRotation, bitStream);
				detail::writeChanges(mLocalPositionChanged, mLocalPosition, bitStream);
				detail::writeChanges(mBoneNameChanged, mBoneName, bitStream);
			}
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getBitLength();
			QSF_LOG_PRINTS(INFO, "BoneLinkData: written bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void BoneLinkData::setData(const qsf::game::BitStream& bitStream, const int32 receivedHostTickCount)
		{
			// When this method is called at least one tracked data by this class has changed on the host side
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getReadPosition();
#endif

			// Call base implementation
			DataCacheComponentExistenceTrackingBase<qsf::game::BoneLinkComponent>::setData(bitStream, receivedHostTickCount);

			// Read the component data when the component exists
			if (hasEntityComponentFromStream())
			{
				HistoryEntry entry;
				entry.tickCount = receivedHostTickCount;

				entry.mLocalRotationChanged = detail::checkChangedAndRead(bitStream, entry.mLocalRotation);
				entry.mLocalPositionChanged = detail::checkChangedAndRead(bitStream, entry.mLocalPosition);
				entry.mBoneNameChanged = detail::checkChangedAndRead(bitStream, entry.mBoneName);

				mHistoryList.emplace_back(entry);
			}

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getReadPosition();
			QSF_LOG_PRINTS(INFO, "BoneLinkData: read bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void BoneLinkData::interpolate(qsf::Entity& targetEntity, const qsf::Clock& clock, const int32 receivedHostTickCount)
		{
			// Call base implementation
			DataCacheComponentExistenceTrackingBase<qsf::game::BoneLinkComponent>::interpolate(targetEntity, clock, receivedHostTickCount);

			if (!mHistoryList.empty())
			{
				qsf::game::BoneLinkComponent* boneLinkComponent = getComponent(targetEntity);
				if (nullptr != boneLinkComponent)
				{
					auto iterator = mHistoryList.begin();
					while (iterator != mHistoryList.end())
					{
						const HistoryEntry& entry = *iterator;
						if (entry.tickCount == clock.getSignalCounter())
						{
							if (entry.mLocalRotationChanged)
							{
								boneLinkComponent->setLocalRotation(entry.mLocalRotation);
							}

							if (entry.mLocalPositionChanged)
							{
								boneLinkComponent->setLocalPosition(entry.mLocalPosition);
							}

							if (entry.mBoneNameChanged)
							{
								boneLinkComponent->setBoneName(entry.mBoneName);
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
