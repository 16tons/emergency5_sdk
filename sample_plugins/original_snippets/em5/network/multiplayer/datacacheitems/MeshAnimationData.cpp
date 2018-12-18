// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/network/multiplayer/datacache/datacacheitems/MeshAnimationData.h"
#include "em5/network/multiplayer/datacache/DataCacheItemHelper.h"

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
		MeshAnimationData::MeshAnimationData(const qsf::Entity& targetEntity) :
			DataCacheComponentExistenceTrackingBase<qsf::MeshAnimationComponent>(targetEntity),
			mDataChanged(false),
			mAnimationTypeChanged(false),
			mBlendSyncChanged(false),
			mBlendTimeChanged(false),
			mBoneRetargeting1Changed(false),
			mBoneRetargeting2Changed(false),
			mLocalAssetId1Changed(false),
			mLocalAssetId2Changed(false),
			mBoneMaskChanged(false)
		{
			qsf::MeshAnimationComponent* meshAnimationComponent = getComponent(targetEntity);
			if (nullptr != meshAnimationComponent)
			{
				mLastAnimationData = meshAnimationComponent->getLastAnimationData();
			}
		}

		MeshAnimationData::~MeshAnimationData()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::DataCacheBase methods ]
		//[-------------------------------------------------------]
		bool MeshAnimationData::includeForPrepare(const int32 currentTickCount, bool entityIsVisible)
		{
			return true;
		}

		bool MeshAnimationData::prepareForUpdate(const qsf::Entity& targetEntity, const qsf::Clock& clock)
		{
			// Call base implementation
			bool componentExistenceChanged = DataCacheComponentExistenceTrackingBase<qsf::MeshAnimationComponent>::prepareForUpdate(targetEntity, clock);

			// Check for component data changes, when component exists
			qsf::MeshAnimationComponent* meshAnimationComponent = getComponent(targetEntity);
			if (nullptr != meshAnimationComponent )
			{
				mDataChanged = checkIfDataHasChanged(meshAnimationComponent->getLastAnimationData());
			}
			else
			{
				mDataChanged = false;
			}

			return componentExistenceChanged || mDataChanged;
		}

		void MeshAnimationData::updateData(qsf::game::BitStream& bitStream, bool force)
		{
			// When this method is called at least one tracked data by this class has changed
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getBitLength();
#endif
			// Call base implementation
			DataCacheComponentExistenceTrackingBase<qsf::MeshAnimationComponent>::updateData(bitStream, force);

			// Only write component data when the entity has the component
			if (hasEntityComponent())
			{
				bitStream.write(mDataChanged);
				if (mDataChanged)
				{
					bitStream.write(mAnimationTypeChanged);
					if (mAnimationTypeChanged)
					{
						bitStream.writeAs<uint8>(mLastAnimationData.mAnimationType, 3);
					}

					if (mLastAnimationData.mAnimationType != qsf::MeshAnimationComponent::ANIMATION_TYPE_STOP_ALL)
					{
						// Common for all animation types except stop all
						detail::writeChanges(mLocalAssetId1Changed, mLastAnimationData.mLocalAnimationAssetId1, bitStream);
						bitStream.write(mLastAnimationData.mLoop);
						bitStream.write(mLastAnimationData.mReverse1);

						bitStream.write(mBoneRetargeting1Changed);
						if (mBoneRetargeting1Changed)
						{
							bitStream.writeAs<uint8>(mLastAnimationData.mBoneRetargeting1, 2);
						}

						// Write animation type specific values
						if (mLastAnimationData.mAnimationType == qsf::MeshAnimationComponent::ANIMATION_TYPE_BLEND_MASK
								|| mLastAnimationData.mAnimationType == qsf::MeshAnimationComponent::ANIMATION_TYPE_BLEND_SINGLE)
						{
							// Blend related values
							bitStream.write(mBlendSyncChanged);
							if (mBlendSyncChanged)
							{
								bitStream.writeAs<uint8>(mLastAnimationData.mBlendSync, 3);
							}

							bitStream.write(mBlendTimeChanged);
							if (mBlendTimeChanged)
							{
								// Use an bit to indicate if the blend time is zero or non zero -> if zero no blend time is transferred
								bool blendTimeIsZero = mLastAnimationData.mBlendTime == qsf::Time::ZERO;
								bitStream.write(blendTimeIsZero);
								if (!blendTimeIsZero)
								{
									// TODO(sw) After a quick look on every call to start an animation with blend time following values where used:
									//          - 0
									//          - 0.1s till 2s
									//          => We can send the blend time as a tenth of seconds which would be then a value between 1 and 200 * 10 ms
									//          ==> We can use here an uint8 value
									uint8 tenthMiliseconds = static_cast<uint8>(mLastAnimationData.mBlendTime.getMilliseconds()/10);
									bitStream.write(tenthMiliseconds);
								}
							}
						}

						if (mLastAnimationData.mAnimationType == qsf::MeshAnimationComponent::ANIMATION_TYPE_BLEND_MASK
							|| mLastAnimationData.mAnimationType == qsf::MeshAnimationComponent::ANIMATION_TYPE_PLAY_MASK)
						{
							// Mask related values
							detail::writeChanges(mLocalAssetId2Changed, mLastAnimationData.mLocalAnimationAssetId2, bitStream);
							bitStream.write(mLastAnimationData.mReverse2);
							bitStream.write(mBoneRetargeting2Changed);
							if (mBoneRetargeting2Changed)
							{
								bitStream.writeAs<uint8>(mLastAnimationData.mBoneRetargeting2, 2);
							}
						}

						if (mLastAnimationData.mAnimationType == qsf::MeshAnimationComponent::ANIMATION_TYPE_BLEND_MASK)
						{
							bitStream.write(mBoneMaskChanged);
							if (mBoneMaskChanged)
							{
								// Blend mask related values. We can send the amount of bone mask as uint8 value, because currently there is only one place where a bone mask is set and then only for one bone
								bitStream.writeAs<uint8>(mLastAnimationData.mBoneMask.size());
								for (const std::string& boneName : mLastAnimationData.mBoneMask)
								{
									bitStream.write(boneName);
								}
							}
						}
					}
				}
			}

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getBitLength();
			QSF_LOG_PRINTS(INFO, "MeshAnimationData: written bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void MeshAnimationData::setData(const qsf::game::BitStream& bitStream, const int32 receivedHostTickCount)
		{
			// When this method is called at least one tracked data by this class has changed on the host side
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getReadPosition();
#endif
			// Call base implementation
			DataCacheComponentExistenceTrackingBase<qsf::MeshAnimationComponent>::setData(bitStream, receivedHostTickCount);

			// Read the component data when the component exists
			if (hasEntityComponentFromStream())
			{
				if (detail::checkChanged(bitStream))
				{
					// When we get here, we have at least one value changed
					// Read animation type
					mAnimationTypeChanged = detail::checkChanged(bitStream);
					if (mAnimationTypeChanged)
					{
						// Animation type has changed
						if (!bitStream.readAs<uint8>(mLastAnimationData.mAnimationType, 3))
						{
							QSF_ERROR("MeshAnimationData: could not read value from stream", QSF_REACT_THROW);
						}
					}

					// Check animation type to determine for which values additional data was send
					if (mLastAnimationData.mAnimationType != qsf::MeshAnimationComponent::ANIMATION_TYPE_STOP_ALL)
					{
						// Read common for all animation types except stop all
						mLocalAssetId1Changed = detail::checkChangedAndRead(bitStream, mLastAnimationData.mLocalAnimationAssetId1);

						if (!bitStream.read(mLastAnimationData.mLoop))
						{
							QSF_ERROR("MeshAnimationData: could not read value from stream", QSF_REACT_THROW);
						}

						if (!bitStream.read(mLastAnimationData.mReverse1))
						{
							QSF_ERROR("MeshAnimationData: could not read value from stream", QSF_REACT_THROW);
						}

						mBoneRetargeting1Changed = detail::checkChanged(bitStream);
						if (mBoneRetargeting1Changed)
						{
							if (!bitStream.readAs<uint8>(mLastAnimationData.mBoneRetargeting1, 2))
							{
								QSF_ERROR("MeshAnimationData: could not read value from stream", QSF_REACT_THROW);
							}
						}

						if (mLastAnimationData.mAnimationType == qsf::MeshAnimationComponent::ANIMATION_TYPE_BLEND_MASK
							|| mLastAnimationData.mAnimationType == qsf::MeshAnimationComponent::ANIMATION_TYPE_BLEND_SINGLE)
						{
							// Blend related values
							mBlendSyncChanged = detail::checkChanged(bitStream);
							if (mBlendSyncChanged)
							{
								if (!bitStream.readAs<uint8>(mLastAnimationData.mBlendSync, 3))
								{
									QSF_ERROR("MeshAnimationData: could not read value from stream", QSF_REACT_THROW);
								}
							}

							mBlendTimeChanged = detail::checkChanged(bitStream);
							if (mBlendTimeChanged)
							{
								int64 time = 0;
								bool isZero;
								if (!bitStream.read(isZero))
								{
									QSF_ERROR("MeshAnimationData: could not read value from stream", QSF_REACT_THROW);
								}

								if (!isZero)
								{
									// TODO(sw) After a quick look on every call to start an animation with blend time following values where used:
									//          - 0
									//          - 0.1s till 2s
									//          => We can send the blend time as a tenth of seconds which would be then a value between 1 and 200 * 10 ms
									//          ==> We can use here an uint8 value
									if (!bitStream.readAs<uint8>(time))
									{
										QSF_ERROR("MeshAnimationData: could not read value from stream", QSF_REACT_THROW);
									}
									// To get full miliseconds values back we have to multiply the value with 10
									time *= 10;
								}
								mLastAnimationData.mBlendTime = qsf::Time::fromMilliseconds(time);
							}
						}

						if (mLastAnimationData.mAnimationType == qsf::MeshAnimationComponent::ANIMATION_TYPE_BLEND_MASK
							|| mLastAnimationData.mAnimationType == qsf::MeshAnimationComponent::ANIMATION_TYPE_PLAY_MASK)
						{
							mLocalAssetId2Changed = detail::checkChangedAndRead(bitStream, mLastAnimationData.mLocalAnimationAssetId2);

							if (!bitStream.read(mLastAnimationData.mReverse2))
							{
								QSF_ERROR("MeshAnimationData: could not read value from stream", QSF_REACT_THROW);
							}

							mBoneRetargeting2Changed = detail::checkChanged(bitStream);
							if (mBoneRetargeting2Changed)
							{
								if (!bitStream.readAs<uint8>(mLastAnimationData.mBoneRetargeting2, 2))
								{
									QSF_ERROR("MeshAnimationData: could not read value from stream", QSF_REACT_THROW);
								}
							}
						}

						if (mLastAnimationData.mAnimationType == qsf::MeshAnimationComponent::ANIMATION_TYPE_BLEND_MASK)
						{
							mBoneMaskChanged = detail::checkChanged(bitStream);
							if (mBoneMaskChanged)
							{
								size_t boneMaskCount;
								if (!bitStream.readAs<uint8>(boneMaskCount))
								{
									QSF_ERROR("MeshAnimationData: could not read value from stream", QSF_REACT_THROW);
								}

								mLastAnimationData.mBoneMask.reserve(boneMaskCount);
								// Read bone masks
								for (size_t i = 0; i < boneMaskCount; ++i)
								{
									std::string value;
									if (!bitStream.read(value))
									{
										QSF_ERROR("MeshAnimationData: could not read value from stream", QSF_REACT_THROW);
									}
									mLastAnimationData.mBoneMask.emplace_back(value);
								}
							}
						}
					}

					HistoryEntry entry;
					entry.tickCount = receivedHostTickCount;
					entry.AnimationData = mLastAnimationData;
					mHistoryList.emplace_back(entry);
				}
			}

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getReadPosition();
			QSF_LOG_PRINTS(INFO, "MeshAnimationData: read bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void MeshAnimationData::interpolate(qsf::Entity& targetEntity, const qsf::Clock& clock, const int32 receivedHostTickCount)
		{
			// Call base implementation
			DataCacheComponentExistenceTrackingBase<qsf::MeshAnimationComponent>::interpolate(targetEntity, clock, receivedHostTickCount);

			if (!mHistoryList.empty())
			{
				qsf::MeshAnimationComponent* meshAnimationComponent = getComponent(targetEntity);
				if (nullptr != meshAnimationComponent)
				{
					auto iterator = mHistoryList.begin();
					while (iterator != mHistoryList.end())
					{
						if (iterator->tickCount == clock.getSignalCounter())
						{
							const qsf::MeshAnimationComponent::LastAnimatonData& lastAnimationData = iterator->AnimationData;
							// Apply the read data
							if (lastAnimationData.mAnimationType == qsf::MeshAnimationComponent::ANIMATION_TYPE_STOP_ALL)
							{
								meshAnimationComponent->stopAllAnimations();
							}
							else if (lastAnimationData.mAnimationType == qsf::MeshAnimationComponent::ANIMATION_TYPE_PLAY_SINGLE)
							{
								meshAnimationComponent->playAnimation(qsf::AssetProxy(lastAnimationData.mLocalAnimationAssetId1), lastAnimationData.mLoop, lastAnimationData.mReverse1, lastAnimationData.mBoneRetargeting1);
							}
							else if (lastAnimationData.mAnimationType == qsf::MeshAnimationComponent::ANIMATION_TYPE_PLAY_MASK)
							{
								meshAnimationComponent->playMaskedAnimation(qsf::AssetProxy(lastAnimationData.mLocalAnimationAssetId1), qsf::AssetProxy(lastAnimationData.mLocalAnimationAssetId2), lastAnimationData.mBoneMask, lastAnimationData.mLoop, lastAnimationData.mReverse1, lastAnimationData.mReverse2, lastAnimationData.mBoneRetargeting1, lastAnimationData.mBoneRetargeting2);
							}
							else if (lastAnimationData.mAnimationType == qsf::MeshAnimationComponent::ANIMATION_TYPE_BLEND_SINGLE)
							{
								meshAnimationComponent->blendToAnimation(qsf::AssetProxy(lastAnimationData.mLocalAnimationAssetId1), lastAnimationData.mLoop, lastAnimationData.mBlendTime, lastAnimationData.mReverse1, lastAnimationData.mBlendSync, lastAnimationData.mBoneRetargeting1);
							}
							else if (lastAnimationData.mAnimationType == qsf::MeshAnimationComponent::ANIMATION_TYPE_BLEND_MASK)
							{
								meshAnimationComponent->blendToMaskedAnimation(qsf::AssetProxy(lastAnimationData.mLocalAnimationAssetId1), qsf::AssetProxy(lastAnimationData.mLocalAnimationAssetId2), lastAnimationData.mBoneMask, lastAnimationData.mLoop, lastAnimationData.mBlendTime, lastAnimationData.mReverse1, lastAnimationData.mReverse2, lastAnimationData.mBlendSync, lastAnimationData.mBoneRetargeting1, lastAnimationData.mBoneRetargeting2);
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
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		bool MeshAnimationData::checkIfDataHasChanged(const qsf::MeshAnimationComponent::LastAnimatonData& changedAnimationData)
		{
			mAnimationTypeChanged = detail::checkForChanges(changedAnimationData.mAnimationType, mLastAnimationData.mAnimationType);
			mLocalAssetId1Changed = detail::checkForChanges(changedAnimationData.mLocalAnimationAssetId1, mLastAnimationData.mLocalAnimationAssetId1);
			mLocalAssetId2Changed = detail::checkForChanges(changedAnimationData.mLocalAnimationAssetId2, mLastAnimationData.mLocalAnimationAssetId2);
			mLastAnimationData.mLoop = changedAnimationData.mLoop;
			mLastAnimationData.mReverse1 = changedAnimationData.mReverse1;
			mLastAnimationData.mReverse2 = changedAnimationData.mReverse2;
			mBoneRetargeting1Changed = detail::checkForChanges(changedAnimationData.mBoneRetargeting1, mLastAnimationData.mBoneRetargeting1);
			mBoneRetargeting2Changed = detail::checkForChanges(changedAnimationData.mBoneRetargeting2, mLastAnimationData.mBoneRetargeting2);

			mBoneMaskChanged = false;
			if (changedAnimationData.mBoneMask.size() != mLastAnimationData.mBoneMask.size())
			{
				mBoneMaskChanged = true;
			}
			else
			{
				// Bone mask list size are same, compare each value
				for (size_t i = 0; i < changedAnimationData.mBoneMask.size(); ++i)
				{
					if (changedAnimationData.mBoneMask[i] != mLastAnimationData.mBoneMask[i])
					{
						mBoneMaskChanged = true;
						break;
					}
				}
			}

			if (mBoneMaskChanged)
			{
				// Save the new bone mask list
				mLastAnimationData.mBoneMask = changedAnimationData.mBoneMask;
			}

			mBlendTimeChanged = detail::checkForChanges(changedAnimationData.mBlendTime, mLastAnimationData.mBlendTime);
			mBlendSyncChanged = detail::checkForChanges(changedAnimationData.mBlendSync, mLastAnimationData.mBlendSync);

			return mAnimationTypeChanged || mBlendSyncChanged
				   || mBlendTimeChanged || mBoneRetargeting1Changed
				   || mBoneRetargeting2Changed || mLocalAssetId1Changed
				   || mLocalAssetId2Changed || mBoneMaskChanged;


		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5
