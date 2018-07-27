// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/network/multiplayer/datacache/datacacheitems/DlkLadderData.h"
#include "em5/network/multiplayer/datacache/DataCacheItemHelper.h"
#include "em5/component/vehicle/parts/DLKLadderComponent.h"

#include <qsf_game/network/BitStream.h>

#include <qsf/reflection/type/CampGlmVec3.h>
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
		DlkLadderData::DlkLadderData(const qsf::Entity& targetEntity) :
			DataCacheBase(targetEntity),
			mTargetPositionChanged(false),
			mLastAnimationType(0),
			mLastAnimationTypeChanged(false)
		{
			mComponent = targetEntity.getComponent<DlkLadderComponent>();
			if (mComponent.valid())
			{
				mTargetPosition = mComponent->getTargetPosition();
				mLastAnimationType = static_cast<uint8>(mComponent->getLastAnimationState());
			}
			else
			{
				QSF_ERROR("DlkLadderData: The entity \""<<targetEntity.getId()<<"\" doesn't have a DlkLadderComponent for caching the data by this class", QSF_REACT_THROW);
			}
		}

		DlkLadderData::~DlkLadderData()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::DataCacheBase methods ]
		//[-------------------------------------------------------]
		bool DlkLadderData::prepareForUpdate(const qsf::Entity& targetEntity, const qsf::Clock& clock)
		{
			if (mComponent.valid())
			{
				mTargetPositionChanged = detail::checkForChanges(mComponent->getTargetPosition(), mTargetPosition);
				mLastAnimationTypeChanged = detail::checkForChanges(static_cast<uint8>(mComponent->getLastAnimationState()), mLastAnimationType);

				// Only for debugging
#if 0
				if (mTargetPositionChanged)
				{
					QSF_LOG_PRINTS(INFO, "DlkLadderData: New target position: " << mTargetPosition);
				}
#endif

				return mTargetPositionChanged || mLastAnimationTypeChanged;
			}
			else
			{
				// The entity has no vehicle crane component anymore -> This shouldn't never happen. An instance of DlkLadderData for such an entity should never be created
				QSF_ERROR("DlkLadderData: The entity \""<<targetEntity.getId()<<"\" lost its vehicle crane components this instance is not longer usable for that entity", QSF_REACT_NONE);
			}

			return false;
		}

		void DlkLadderData::updateData(qsf::game::BitStream& bitStream, bool force)
		{
			// When this method is called at least one tracked data by this class has changed
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getBitLength();
#endif
			detail::writeChanges(mTargetPositionChanged, mTargetPosition, bitStream);
			// The animation state has only a value range of 0-3 => can be represented by two bits
			detail::writeChanges(mLastAnimationTypeChanged, mLastAnimationType, 2, bitStream);

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getBitLength();
			QSF_LOG_PRINTS(INFO, "DlkLadderData: written bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void DlkLadderData::setData(const qsf::game::BitStream& bitStream, const int32 receivedHostTickCount)
		{
			// When this method is called at least one tracked data by this class has changed on the host side
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getReadPosition();
#endif
			HistoryEntry entry;
			entry.tickCount = receivedHostTickCount;

			entry.mTargetPositionChanged = detail::checkChangedAndRead(bitStream, mTargetPosition);
			entry.mTargetPosition = mTargetPosition;

			// The animation state has only a value range of 0-3 => can be represented by two bits
			entry.mLastAnimationTypeChanged = detail::checkChangedAndRead(bitStream, 2, mLastAnimationType);
			entry.mLastAnimationType = mLastAnimationType;

			mHistoryList.emplace_back(entry);

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getReadPosition();
			QSF_LOG_PRINTS(INFO, "DlkLadderData: read bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void DlkLadderData::interpolate(qsf::Entity& targetEntity, const qsf::Clock& clock, const int32 receivedHostTickCount)
		{
			if (!mHistoryList.empty() && mComponent.valid())
			{
				auto iterator = mHistoryList.begin();
				while (iterator != mHistoryList.end())
				{
					const HistoryEntry& entry = *iterator;
					if (entry.tickCount == clock.getSignalCounter())
					{
						if (entry.mLastAnimationTypeChanged || entry.mTargetPositionChanged)
						{
							DlkLadderComponent::AnimationState animationState = static_cast<DlkLadderComponent::AnimationState>(entry.mLastAnimationType);
							switch (animationState)
							{
								case DlkLadderComponent::AnimationState::STATE_ALIGN_LADDER:
								{
									if (entry.mTargetPositionChanged)
									{
										mComponent->alignLadder(entry.mTargetPosition);
									}
								}
								break;
								case DlkLadderComponent::AnimationState::STATE_DEPLOY_LADDER:
								{
									if (entry.mLastAnimationTypeChanged)
									{
										mComponent->deployLadder(entry.mTargetPosition);
									}
								}
								break;
								case DlkLadderComponent::AnimationState::STATE_FOLD_LADDER:
								{
									if (entry.mLastAnimationTypeChanged)
									{
										mComponent->foldLadder();
									}
								}
								break;

								case DlkLadderComponent::AnimationState::STATE_NONE:
									// Nothing to do in this case
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
