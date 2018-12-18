// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/network/multiplayer/datacache/datacacheitems/VehicleCraneData.h"
#include "em5/network/multiplayer/datacache/DataCacheItemHelper.h"
#include "em5/component/vehicle/parts/VehicleCraneComponent.h"

#include <qsf_game/network/BitStream.h>

#include <qsf/log/LogSystem.h>
#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>


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
		VehicleCraneData::VehicleCraneData(const qsf::Entity& targetEntity) :
			DataCacheBase(targetEntity),
			mTargetPositionChanged(false),
			mLastAnimationType(0),
			mLastAnimationTypeChanged(false),
			mStrapVisible(false),
			mSequenceType(0),
			mSequenceTypeChanged(false),
			mMoveHookDistancePercentage(0),
			mMoveHookDistancePercentageChanged(false),
			mEntityIdLinkedToHook(qsf::getUninitialized<uint64>()),
			mEntityIdLinkedToHookChanged(false)
		{
			mComponent = targetEntity.getComponent<VehicleCraneComponent>();
			if (mComponent.valid())
			{
				mTargetPosition = mComponent->getTargetPosition();
				mLastAnimationType = static_cast<uint8>(mComponent->getLastAnimationState());
				mStrapVisible = mComponent->isStrapVisible();
				mSequenceType = static_cast<uint8>(mComponent->getSequenceType());
				// It is a percentage value and currently only full percentaqge is used
				mMoveHookDistancePercentage = static_cast<uint8>(mComponent->getHookMoveDistancePercentage()*100);
				mEntityIdLinkedToHook = mComponent->getEntityIdLinkedToHook();
			}
			else
			{
				QSF_ERROR("VehicleCraneData: The entity \""<<targetEntity.getId()<<"\" doesn't have a VehicleCraneComponent for caching the data by this class", QSF_REACT_THROW);
			}
		}

		VehicleCraneData::~VehicleCraneData()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::DataCacheBase methods ]
		//[-------------------------------------------------------]
		bool VehicleCraneData::prepareForUpdate(const qsf::Entity& targetEntity, const qsf::Clock& clock)
		{
			if (mComponent.valid())
			{
				mTargetPositionChanged = detail::checkForChanges(mComponent->getTargetPosition(), mTargetPosition);
				mLastAnimationTypeChanged = detail::checkForChanges(static_cast<uint8>(mComponent->getLastAnimationState()), mLastAnimationType);
				mSequenceTypeChanged = detail::checkForChanges(static_cast<uint8>(mComponent->getSequenceType()), mSequenceType);
				mMoveHookDistancePercentageChanged = detail::checkForChanges(static_cast<uint8>(mComponent->getHookMoveDistancePercentage()*100), mMoveHookDistancePercentage);
				mEntityIdLinkedToHookChanged = detail::checkForChanges(mComponent->getEntityIdLinkedToHook(), mEntityIdLinkedToHook);
				// const bool strapVisibleChanged = detail::checkForChanges(mComponent->isStrapVisible(), mStrapVisible);	// TODO(co) Variable is not used?

				return (mTargetPositionChanged || mLastAnimationTypeChanged || mSequenceTypeChanged || mMoveHookDistancePercentageChanged || mEntityIdLinkedToHookChanged);
			}
			else
			{
				// The entity has no vehicle crane component anymore -> This shouldn't never happen. An instance of VehicleCraneData for such an entity should never be created
				QSF_ERROR("VehicleCraneData: The entity \""<<targetEntity.getId()<<"\" lost its vehicle crane components this instance is not longer usable for that entity", QSF_REACT_NONE);
			}

			return false;
		}

		void VehicleCraneData::updateData(qsf::game::BitStream& bitStream, bool force)
		{
			// When this method is called at least one tracked data by this class has changed
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getBitLength();
#endif
			bitStream.write(mStrapVisible);
			detail::writeChanges(mTargetPositionChanged, mTargetPosition, bitStream);
			detail::writeChanges(mSequenceTypeChanged, mSequenceType, bitStream);
			detail::writeChanges(mMoveHookDistancePercentageChanged, mMoveHookDistancePercentage, bitStream);
			detail::writeChanges(mEntityIdLinkedToHookChanged, mEntityIdLinkedToHook, bitStream);
			detail::writeChanges(mLastAnimationTypeChanged, mLastAnimationType, bitStream);

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getBitLength();
			QSF_LOG_PRINTS(INFO, "VehicleCraneData: written bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void VehicleCraneData::setData(const qsf::game::BitStream& bitStream, const int32 receivedHostTickCount)
		{
			// When this method is called at least one tracked data by this class has changed on the host side
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getReadPosition();
#endif
			HistoryEntry entry;
			entry.tickCount = receivedHostTickCount;

			bitStream.read(entry.mStrapVisible);

			entry.mTargetPositionChanged = detail::checkChangedAndRead(bitStream, mTargetPosition);
			entry.mTargetPosition = mTargetPosition;

			entry.mSequenceTypeChanged = detail::checkChangedAndRead(bitStream, mSequenceType);
			entry.mSequenceType = mSequenceType;

			entry.mMoveHookDistancePercentageChanged = detail::checkChangedAndRead(bitStream, mMoveHookDistancePercentage);
			entry.mMoveHookDistancePercentage = mMoveHookDistancePercentage;

			entry.mEntityIdLinkedToHookChanged = detail::checkChangedAndRead(bitStream, entry.mEntityIdLinkedToHook);

			entry.mLastAnimationTypeChanged = detail::checkChangedAndRead(bitStream, mLastAnimationType);
			entry.mLastAnimationType = mLastAnimationType;

			mHistoryList.emplace_back(entry);

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getReadPosition();
			QSF_LOG_PRINTS(INFO, "VehicleCraneData: read bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void VehicleCraneData::interpolate(qsf::Entity& targetEntity, const qsf::Clock& clock, const int32 receivedHostTickCount)
		{
			if (!mHistoryList.empty() && mComponent.valid())
			{
				auto iterator = mHistoryList.begin();
				while (iterator != mHistoryList.end())
				{
					const HistoryEntry& entry = *iterator;
					if (entry.tickCount == clock.getSignalCounter())
					{
						if (mComponent->isStrapVisible() != entry.mStrapVisible)
						{
							mComponent->setVisibleStrap(entry.mStrapVisible);
						}

						if (entry.mSequenceTypeChanged)
						{
							mComponent->setSequenceType(static_cast<VehicleCraneComponent::SequenceType>(mSequenceType));
						}

						if (entry.mEntityIdLinkedToHookChanged)
						{
							if (qsf::isInitialized(entry.mEntityIdLinkedToHook))
							{
								qsf::Entity* linkedEntity = targetEntity.getMap().getEntityById(entry.mEntityIdLinkedToHook);
								if (nullptr != linkedEntity)
								{
									mComponent->linkTargetToHook(*linkedEntity);
								}
							}
							else
							{
								mComponent->unlinkTargetFromHook();
							}
						}

						if (entry.mLastAnimationTypeChanged || entry.mMoveHookDistancePercentageChanged || entry.mTargetPositionChanged)
						{
							VehicleCraneComponent::AnimationState animationState = static_cast<VehicleCraneComponent::AnimationState>(entry.mLastAnimationType);
							switch (animationState)
							{
								case VehicleCraneComponent::AnimationState::STATE_NONE:
								case VehicleCraneComponent::AnimationState::STATE_STOP_ALL_ANIMATION:
								{
									if (entry.mLastAnimationTypeChanged)
									{
										mComponent->stopAllAnimation();
									}
								}
								break;
								case VehicleCraneComponent::AnimationState::STATE_ALIGN_ARM:
								{
									if (entry.mTargetPositionChanged)
									{
										mComponent->alignArm(entry.mTargetPosition);
									}
								}
								break;
								case VehicleCraneComponent::AnimationState::STATE_LOAD_CARGO_ARM:
								{
									if (entry.mLastAnimationTypeChanged)
									{
										mComponent->loadCargoArm();
									}
								}
								break;
								case VehicleCraneComponent::AnimationState::STATE_HOOK_MOVE:
								{
									if (entry.mTargetPositionChanged || entry.mMoveHookDistancePercentageChanged)
									{
										// It is a percentage value and currently only full percentaqge is used
										mComponent->hookMove(entry.mTargetPosition, entry.mMoveHookDistancePercentage/100.f);
									}
								}
								break;
								case VehicleCraneComponent::AnimationState::STATE_HOOK_MOVE_BACK:
								{
									if (entry.mLastAnimationTypeChanged)
									{
										mComponent->hookMoveBack();
									}
								}
								break;
								case VehicleCraneComponent::AnimationState::STATE_FOLD_ARM:
								{
									if (entry.mLastAnimationTypeChanged)
									{
										mComponent->foldArm();
									}
								}
								break;
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
