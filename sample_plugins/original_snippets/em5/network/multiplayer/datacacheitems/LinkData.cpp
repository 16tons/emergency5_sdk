// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/network/multiplayer/datacache/datacacheitems/LinkData.h"
#include "em5/network/multiplayer/datacache/DataCacheItemHelper.h"

#include <qsf/component/link/LinkComponent.h>
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
		LinkData::LinkData(const qsf::Entity& targetEntity) :
			DataCacheComponentExistenceTrackingBase<qsf::LinkComponent>(targetEntity),
			mLinkParentId(qsf::getUninitialized<uint64>()),
			mLinkParentIdChanged(false),
			mLinkType(static_cast<int8>(qsf::LinkComponent::INVALID_LINK_TYPE)),
			mLinkTypeChanged(false),
			mLocalPosition(qsf::Math::GLM_VEC3_ZERO),
			mLocalPositionChanged(false),
			mLocalRotation(qsf::Math::GLM_QUAT_IDENTITY),
			mLocalRotationChanged(false)
		{
			const qsf::LinkComponent* linkComponent = getComponent(targetEntity);
			if (nullptr != linkComponent)
			{
				mLinkParentId = linkComponent->getParentId();
				mLinkType = static_cast<int8>(linkComponent->getParentLinkType());
				mLocalPosition = linkComponent->getLocalPosition();
				mLocalRotation = linkComponent->getLocalRotation();
			}
		}

		LinkData::~LinkData()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::DataCacheBase methods ]
		//[-------------------------------------------------------]
		bool LinkData::prepareForUpdate(const qsf::Entity& targetEntity, const qsf::Clock& clock)
		{
			// Call base implementation
			bool componentExistenceChanged = DataCacheComponentExistenceTrackingBase<qsf::LinkComponent>::prepareForUpdate(targetEntity, clock);

			// Check for component data changes, when component exists
			const qsf::LinkComponent* linkComponent = getComponent(targetEntity);
			if (nullptr != linkComponent )
			{
				mLinkParentIdChanged = detail::checkForChanges(linkComponent->getParentId(), mLinkParentId);
				mLinkTypeChanged = detail::checkForChanges(static_cast<int8>(linkComponent->getParentLinkType()), mLinkType);

				if (mLinkParentIdChanged)
				{
					// When we have a link parent id change -> we must also send the link type -> because the link type might change on client side due EntityHelper::leaveContainer
					// Which doesn't get noticed on host side because we see only the end result after each tick
					mLinkTypeChanged = true;
				}

				// When the link parent id changed also send local transform changes even when they didn't changed
				mLocalPositionChanged = detail::checkForChanges(linkComponent->getLocalPosition(), mLocalPosition) || mLinkParentIdChanged;
				mLocalRotationChanged = detail::checkForChanges(linkComponent->getLocalRotation(), mLocalRotation) || mLinkParentIdChanged;

				return mLocalRotationChanged || mLocalPositionChanged || mLinkTypeChanged || mLinkParentIdChanged || componentExistenceChanged;
			}
			else if (componentExistenceChanged)
			{
				// Only do an reset if the component existence has changed and the component was destroyed
				// Reset cache data
				mLinkParentIdChanged = false;
				mLinkTypeChanged = false;
				mLocalPositionChanged = false;
				mLocalRotationChanged = false;
				mLocalRotation = qsf::Math::GLM_QUAT_IDENTITY;
				mLocalPosition = qsf::Math::GLM_VEC3_ZERO;
				qsf::setUninitialized(mLinkParentId);
				mLinkType = static_cast<int8>(qsf::LinkComponent::INVALID_LINK_TYPE);
			}

			return componentExistenceChanged;
		}

		void LinkData::updateData(qsf::game::BitStream& bitStream, bool force)
		{
			// When this method is called at least one tracked data by this class has changed
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getBitLength();
#endif
			// Call base implementation
			DataCacheComponentExistenceTrackingBase<qsf::LinkComponent>::updateData(bitStream, force);

			// Only write component data when the entity has the component
			if (hasEntityComponent())
			{
				detail::writeChanges(mLinkParentIdChanged, mLinkParentId, bitStream);
				detail::writeChanges(mLinkTypeChanged, mLinkType, bitStream);
				detail::writeChanges(mLocalPositionChanged, mLocalPosition, bitStream);
				detail::writeChanges(mLocalRotationChanged, mLocalRotation, bitStream);
			}

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getBitLength();
			QSF_LOG_PRINTS(INFO, "LinkData: written bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void LinkData::setData(const qsf::game::BitStream& bitStream, const int32 receivedHostTickCount)
		{
			// When this method is called at least one tracked data by this class has changed on the host side
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getReadPosition();
#endif
			// Call base implementation
			DataCacheComponentExistenceTrackingBase<qsf::LinkComponent>::setData(bitStream, receivedHostTickCount);

			// Read the component data when the component exists
			if (hasEntityComponentFromStream())
			{
				HistoryEntry entry;
				entry.tickCount = receivedHostTickCount;

				entry.mLinkParentIdChanged = detail::checkChangedAndRead(bitStream, entry.mLinkParentId);
				entry.mLinkTypeChanged = detail::checkChangedAndRead(bitStream, entry.mLinkType);
				entry.mLocalPositionChanged = detail::checkChangedAndRead(bitStream, entry.mLocalPosition);
				entry.mLocalRotationChanged = detail::checkChangedAndRead(bitStream, entry.mLocalRotation);

				mHistoryList.emplace_back(entry);
			}

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getReadPosition();
			QSF_LOG_PRINTS(INFO, "LinkData: read bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void LinkData::interpolate(qsf::Entity& targetEntity, const qsf::Clock& clock, const int32 receivedHostTickCount)
		{
			// Call base implementation
			DataCacheComponentExistenceTrackingBase<qsf::LinkComponent>::interpolate(targetEntity, clock, receivedHostTickCount);

			if (!mHistoryList.empty())
			{
				qsf::LinkComponent* linkComponent = getComponent(targetEntity);
				if (nullptr != linkComponent)
				{
					auto iterator = mHistoryList.begin();
					while (iterator != mHistoryList.end())
					{
						const HistoryEntry& entry = *iterator;
						if (entry.tickCount == clock.getSignalCounter())
						{
							if (entry.mLinkParentIdChanged)
							{
								// The data has changed -> apply it
								linkComponent->setParentId(entry.mLinkParentId);
							}

							if (entry.mLinkTypeChanged)
							{
								// The data has changed -> apply it
								linkComponent->setParentLinkType(static_cast<qsf::LinkComponent::LinkType>(entry.mLinkType));
							}

							if (entry.mLocalRotationChanged)
							{
								// The data has changed -> apply it
								linkComponent->setLocalRotation(entry.mLocalRotation);
							}

							if (entry.mLocalPositionChanged)
							{
								// The data has changed -> apply it
								linkComponent->setLocalPosition(entry.mLocalPosition);
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
