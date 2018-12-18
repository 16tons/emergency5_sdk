// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/network/multiplayer/datacache/datacacheitems/BoneToBoneLinkData.h"
#include "em5/network/multiplayer/datacache/DataCacheItemHelper.h"

#include <qsf_game/component/base/BoneToBoneLinkComponent.h>
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
		BoneToBoneLinkData::BoneToBoneLinkData(const qsf::Entity& targetEntity) :
			DataCacheComponentExistenceTrackingBase<qsf::game::BoneToBoneLinkComponent>(targetEntity),
			mLocalRotation1Changed(false),
			mLocalPosition1Changed(false),
			mBoneNameChanged(false),
			mParentBoneNameChanged(false),
			mChildBoneName1Changed(false),
			mLocalRotation2Changed(false),
			mLocalPosition2Changed(false),
			mParentBoneName2Changed(false),
			mChildBoneName2Changed(false),
			mParentEntity2(qsf::getUninitialized<uint64>()),
			mParentEntity2Changed(false)
		{
			const qsf::game::BoneToBoneLinkComponent* boneToBoneLinkComponent = getComponent(targetEntity);
			if (nullptr != boneToBoneLinkComponent)
			{
				mLocalRotation1 = boneToBoneLinkComponent->getLocalRotation1();
				mLocalPosition1 = boneToBoneLinkComponent->getLocalPosition1();
				mBoneName = boneToBoneLinkComponent->getBoneName();
				mParentBoneName = boneToBoneLinkComponent->getParentBoneName1();
				mChildBoneName1 = boneToBoneLinkComponent->getChildBoneName1();

				mLocalRotation2 = boneToBoneLinkComponent->getLocalRotation2();
				mLocalPosition2 = boneToBoneLinkComponent->getLocalPosition2();
				mParentBoneName2 = boneToBoneLinkComponent->getParentBoneName2();
				mChildBoneName2 = boneToBoneLinkComponent->getChildBoneName2();
				mParentEntity2 = boneToBoneLinkComponent->getParentEntity2();
			}
		}

		BoneToBoneLinkData::~BoneToBoneLinkData()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::DataCacheBase methods ]
		//[-------------------------------------------------------]
		bool BoneToBoneLinkData::prepareForUpdate(const qsf::Entity& targetEntity, const qsf::Clock& clock)
		{
			// Call base implementation
			bool componentExistenceChanged = DataCacheComponentExistenceTrackingBase<qsf::game::BoneToBoneLinkComponent>::prepareForUpdate(targetEntity, clock);

			// Check for component data changes, when component exists
			const qsf::game::BoneToBoneLinkComponent* boneToBoneLinkComponent = getComponent(targetEntity);
			if (nullptr != boneToBoneLinkComponent )
			{
				mLocalRotation1Changed = detail::checkForChanges(boneToBoneLinkComponent->getLocalRotation1(), mLocalRotation1);
				mLocalPosition1Changed = detail::checkForChanges(boneToBoneLinkComponent->getLocalPosition1(), mLocalPosition1);
				mBoneNameChanged = detail::checkForChanges(boneToBoneLinkComponent->getBoneName(), mBoneName);
				mParentBoneNameChanged = detail::checkForChanges(boneToBoneLinkComponent->getParentBoneName1(), mParentBoneName);
				mChildBoneName1Changed = detail::checkForChanges(boneToBoneLinkComponent->getChildBoneName1(), mChildBoneName1);

				mLocalRotation2Changed = detail::checkForChanges(boneToBoneLinkComponent->getLocalRotation2(), mLocalRotation2);
				mLocalPosition2Changed = detail::checkForChanges(boneToBoneLinkComponent->getLocalPosition2(), mLocalPosition2);
				mParentBoneName2Changed = detail::checkForChanges(boneToBoneLinkComponent->getParentBoneName2(), mParentBoneName2);
				mChildBoneName2Changed = detail::checkForChanges(boneToBoneLinkComponent->getChildBoneName2(), mChildBoneName2);
				mParentEntity2Changed = detail::checkForChanges(boneToBoneLinkComponent->getParentEntity2(), mParentEntity2);

				return componentExistenceChanged || mLocalRotation1Changed
						|| mLocalPosition1Changed || mBoneNameChanged
						|| mParentBoneNameChanged || mChildBoneName1Changed
						|| mLocalRotation2Changed || mLocalPosition2Changed
						|| mParentBoneName2Changed || mChildBoneName2Changed
						|| mParentEntity2Changed;
			}
			else if (componentExistenceChanged)
			{
				// Only do an reset if the component existence has changed and the component was destroyed
				// Reset cache data
				mLocalRotation1 = qsf::Math::GLM_QUAT_IDENTITY;
				mLocalPosition1 = qsf::Math::GLM_VEC3_ZERO;
				mBoneName.clear();
				mParentBoneName.clear();
				mChildBoneName1.clear();

				mLocalRotation1Changed = false;
				mLocalPosition1Changed = false;
				mBoneNameChanged = false;
				mParentBoneNameChanged = false;
				mChildBoneName1Changed = false;

				mLocalRotation2 = qsf::Math::GLM_QUAT_IDENTITY;
				mLocalRotation2Changed = false;
				mLocalPosition2 = qsf::Math::GLM_VEC3_ZERO;
				mLocalPosition2Changed = false;
				mParentBoneName2.clear();
				mParentBoneName2Changed = false;
				mChildBoneName2.clear();
				mChildBoneName2Changed = false;
				qsf::setUninitialized(mParentEntity2);
				mParentEntity2Changed = false;
			}

			return componentExistenceChanged;
		}

		void BoneToBoneLinkData::updateData(qsf::game::BitStream& bitStream, bool force)
		{
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getBitLength();
#endif
			// When this method is called at least one tracked data by this class has changed

			// Call base implementation
			DataCacheComponentExistenceTrackingBase<qsf::game::BoneToBoneLinkComponent>::updateData(bitStream, force);

			// Only write component data when the entity has the component
			if (hasEntityComponent())
			{
				detail::writeChanges(mLocalRotation1Changed, mLocalRotation1, bitStream);
				detail::writeChanges(mLocalPosition1Changed, mLocalPosition1, bitStream);
				detail::writeChanges(mBoneNameChanged, mBoneName, bitStream);
				detail::writeChanges(mParentBoneNameChanged, mParentBoneName, bitStream);
				detail::writeChanges(mChildBoneName1Changed, mChildBoneName1, bitStream);

				detail::writeChanges(mLocalRotation2Changed, mLocalRotation2, bitStream);
				detail::writeChanges(mLocalPosition2Changed, mLocalPosition2, bitStream);
				detail::writeChanges(mParentBoneName2Changed, mParentBoneName2, bitStream);
				detail::writeChanges(mChildBoneName2Changed, mChildBoneName2, bitStream);
				detail::writeChanges(mParentEntity2Changed, mParentEntity2, bitStream);
			}
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getBitLength();
			QSF_LOG_PRINTS(INFO, "BoneToBoneLinkData: written bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void BoneToBoneLinkData::setData(const qsf::game::BitStream& bitStream, const int32 receivedHostTickCount)
		{
			// When this method is called at least one tracked data by this class has changed on the host side
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getReadPosition();
#endif

			// Call base implementation
			DataCacheComponentExistenceTrackingBase<qsf::game::BoneToBoneLinkComponent>::setData(bitStream, receivedHostTickCount);

			// Read the component data when the component exists
			if (hasEntityComponentFromStream())
			{
				HistoryEntry entry;
				entry.tickCount = receivedHostTickCount;

				entry.mLocalRotation1Changed = detail::checkChangedAndRead(bitStream, entry.mLocalRotation1);
				entry.mLocalPosition1Changed = detail::checkChangedAndRead(bitStream, entry.mLocalPosition1);
				entry.mBoneNameChanged = detail::checkChangedAndRead(bitStream, entry.mBoneName);
				entry.mParentBoneNameChanged = detail::checkChangedAndRead(bitStream, entry.mParentBoneName);
				entry.mChildBoneName1Changed = detail::checkChangedAndRead(bitStream, entry.mChildBoneName1);

				entry.mLocalRotation2Changed = detail::checkChangedAndRead(bitStream, entry.mLocalRotation2);
				entry.mLocalPosition2Changed = detail::checkChangedAndRead(bitStream, entry.mLocalPosition2);
				entry.mParentBoneName2Changed = detail::checkChangedAndRead(bitStream, entry.mParentBoneName2);
				entry.mChildBoneName2Changed = detail::checkChangedAndRead(bitStream, entry.mChildBoneName2);
				entry.mParentEntity2Changed = detail::checkChangedAndRead(bitStream, entry.mParentEntity2);

				mHistoryList.emplace_back(entry);
			}

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getReadPosition();
			QSF_LOG_PRINTS(INFO, "BoneToBoneLinkData: read bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void BoneToBoneLinkData::interpolate(qsf::Entity& targetEntity, const qsf::Clock& clock, const int32 receivedHostTickCount)
		{
			// Call base implementation
			DataCacheComponentExistenceTrackingBase<qsf::game::BoneToBoneLinkComponent>::interpolate(targetEntity, clock, receivedHostTickCount);

			if (!mHistoryList.empty())
			{
				qsf::game::BoneToBoneLinkComponent* boneToBoneLinkComponent = getComponent(targetEntity);
				if (nullptr != boneToBoneLinkComponent)
				{
					auto iterator = mHistoryList.begin();
					while (iterator != mHistoryList.end())
					{
						const HistoryEntry& entry = *iterator;
						if (entry.tickCount == clock.getSignalCounter())
						{
							if (entry.mLocalRotation1Changed)
							{
								boneToBoneLinkComponent->setLocalRotation1(entry.mLocalRotation1);
							}

							if (entry.mLocalPosition1Changed)
							{
								boneToBoneLinkComponent->setLocalPosition1(entry.mLocalPosition1);
							}

							if (entry.mBoneNameChanged)
							{
								boneToBoneLinkComponent->setBoneName(entry.mBoneName);
							}

							if (entry.mParentBoneNameChanged)
							{
								boneToBoneLinkComponent->setParentBoneName1(entry.mParentBoneName);
							}

							if (entry.mChildBoneName1Changed)
							{
								boneToBoneLinkComponent->setChildBoneName1(entry.mChildBoneName1);
							}

							if (entry.mParentEntity2Changed)
							{
								boneToBoneLinkComponent->setParentEntity2(entry.mParentEntity2);
							}

							if (entry.mLocalRotation2Changed)
							{
								boneToBoneLinkComponent->setLocalRotation2(entry.mLocalRotation2);
							}

							if (entry.mLocalPosition2Changed)
							{
								boneToBoneLinkComponent->setLocalPosition2(entry.mLocalPosition2);
							}

							if (entry.mParentBoneName2Changed)
							{
								boneToBoneLinkComponent->setParentBoneName2(entry.mParentBoneName2);
							}

							if (entry.mChildBoneName2Changed)
							{
								boneToBoneLinkComponent->setChildBoneName2(entry.mChildBoneName2);
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
