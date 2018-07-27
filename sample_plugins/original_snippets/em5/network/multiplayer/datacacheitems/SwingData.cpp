// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/network/multiplayer/datacache/datacacheitems/SwingData.h"
#include "em5/network/multiplayer/datacache/DataCacheItemHelper.h"
#include "em5/component/movement/SwingComponent.h"

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
		SwingData::SwingData(const qsf::Entity& targetEntity) :
			DataCacheBase(targetEntity),
			mIsActive(false),
			mAnimationType(0),
			mAnimationTypeChanged(false),
			mAnimationCylce(0),
			mAnimationCylceChanged(false),
			mAnimationIntensity(0),
			mAnimationIntensityChanged(false),
			mVelocitySkew(0),
			mVelocitySkewChanged(false)
		{
			mComponent = targetEntity.getComponent<SwingComponent>();
			if (mComponent.valid())
			{
				mIsActive = mComponent->isActive();
				mAnimationType = static_cast<uint8>(mComponent->getAnimationType());
				mAnimationCylce = static_cast<uint8>(mComponent->getAnimationCycle());
				mAnimationIntensity = static_cast<uint8>(mComponent->getAnimationIntensity());
				mVelocitySkew = static_cast<int8>(mComponent->getVelocitySkew()*100);
			}
			else
			{
				QSF_ERROR("SwingData: The entity \""<<targetEntity.getId()<<"\" doesn't have a swing component for caching the data by this class", QSF_REACT_THROW);
			}
		}

		SwingData::~SwingData()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::DataCacheBase methods ]
		//[-------------------------------------------------------]
		bool SwingData::prepareForUpdate(const qsf::Entity& targetEntity, const qsf::Clock& clock)
		{
			if (mComponent.valid())
			{
				bool activeChange = detail::checkForChanges(mComponent->isActive(), mIsActive);

				mAnimationTypeChanged = detail::checkForChanges(static_cast<uint8>(mComponent->getAnimationType()), mAnimationType);
				mAnimationCylceChanged = detail::checkForChanges(static_cast<uint8>(mComponent->getAnimationCycle()), mAnimationCylce);
				mAnimationIntensityChanged = detail::checkForChanges(static_cast<uint8>(mComponent->getAnimationIntensity()), mAnimationIntensity);
				mVelocitySkewChanged = detail::checkForChanges(static_cast<int8>(mComponent->getVelocitySkew()*100), mVelocitySkew);

				return activeChange || mAnimationTypeChanged || mAnimationCylceChanged || mAnimationIntensityChanged || mVelocitySkewChanged;
			}

			return false;
		}

		void SwingData::updateData(qsf::game::BitStream& bitStream, bool force)
		{
			// When this method is called at least one tracked data by this class has changed
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getBitLength();
#endif
			bitStream.write(mIsActive);

			detail::writeChanges(mAnimationTypeChanged, 2, mAnimationType, bitStream);
			detail::writeChanges(mAnimationCylceChanged, mAnimationCylce, bitStream);
			detail::writeChanges(mAnimationIntensityChanged, mAnimationIntensity, bitStream);
			detail::writeChanges(mVelocitySkewChanged, mVelocitySkew, bitStream);

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getBitLength();
			QSF_LOG_PRINTS(INFO, "SwingData: written bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void SwingData::setData(const qsf::game::BitStream& bitStream, const int32 receivedHostTickCount)
		{
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getReadPosition();
#endif
			HistoryEntry entry;
			entry.tickCount = receivedHostTickCount;
			entry.mIsActive = detail::checkChanged(bitStream);
			entry.mAnimationTypeChanged = detail::checkChangedAndRead(bitStream, entry.mAnimationType);
			entry.mAnimationCylceChanged = detail::checkChangedAndRead(bitStream, entry.mAnimationCylce);
			entry.mAnimationIntensityChanged = detail::checkChangedAndRead(bitStream, entry.mAnimationIntensity);
			entry.mVelocitySkewChanged = detail::checkChangedAndRead(bitStream, entry.mVelocitySkew);
			mHistoryList.emplace_back(entry);

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getReadPosition();
			QSF_LOG_PRINTS(INFO, "SwingData: read bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void SwingData::interpolate(qsf::Entity& targetEntity, const qsf::Clock& clock, const int32 receivedHostTickCount)
		{
			if (!mHistoryList.empty() && mComponent.valid())
			{
				auto iterator = mHistoryList.begin();
				while (iterator != mHistoryList.end())
				{
					const HistoryEntry& entry = *iterator;
					if (entry.tickCount == clock.getSignalCounter())
					{
						if (entry.mAnimationTypeChanged)
						{
							mComponent->setAnimationType(static_cast<SwingComponent::AnimationType>(entry.mAnimationType));
						}

						if (entry.mAnimationCylceChanged)
						{
							mComponent->setAnimationCycle(static_cast<float>(entry.mAnimationCylce));
						}

						if (entry.mAnimationIntensityChanged)
						{
							mComponent->setAnimationIntensity(static_cast<float>(entry.mAnimationIntensity));
						}

						if (entry.mVelocitySkewChanged)
						{
							mComponent->setVelocitySkew(entry.mVelocitySkew/100.0f);
						}

						mComponent->setActive(entry.mIsActive);

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
