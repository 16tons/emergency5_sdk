// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/network/multiplayer/datacache/datacacheitems/SupportLegData.h"
#include "em5/network/multiplayer/datacache/DataCacheItemHelper.h"
#include "em5/component/vehicle/parts/SupportLegsComponent.h"

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
		SupportLegData::SupportLegData(const qsf::Entity& targetEntity) :
			DataCacheBase(targetEntity),
			mLastAnimationType(0),
			mLastAnimationTypeChanged(false)
		{
			mComponent = targetEntity.getComponent<SupportLegsComponent>();
			if (mComponent.valid())
			{
				mLastAnimationType = static_cast<uint8>(mComponent->mSupportLegsState);
			}
			else
			{
				QSF_ERROR("SupportLegData: The entity \""<<targetEntity.getId()<<"\" doesn't have a SupportLegsComponent for caching the data by this class", QSF_REACT_THROW);
			}
		}

		SupportLegData::~SupportLegData()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::DataCacheBase methods ]
		//[-------------------------------------------------------]
		bool SupportLegData::prepareForUpdate(const qsf::Entity& targetEntity, const qsf::Clock& clock)
		{
			if (mComponent.valid())
			{
				mLastAnimationTypeChanged = detail::checkForChanges(static_cast<uint8>(mComponent->mSupportLegsState), mLastAnimationType);

				return mLastAnimationTypeChanged;
			}
			else
			{
				// The entity has no vehicle crane component anymore -> This shouldn't never happen. An instance of SupportLegData for such an entity should never be created
				QSF_ERROR("SupportLegData: The entity \""<<targetEntity.getId()<<"\" lost its support legs components this instance is not longer usable for that entity", QSF_REACT_NONE);
			}

			return false;
		}

		void SupportLegData::updateData(qsf::game::BitStream& bitStream, bool force)
		{
			// When this method is called at least one tracked data by this class has changed
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getBitLength();
#endif
			// The animation state has only a value range of 0-3 => can be represented by two bits
			detail::writeChanges(mLastAnimationTypeChanged, mLastAnimationType, 2, bitStream);

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getBitLength();
			QSF_LOG_PRINTS(INFO, "SupportLegData: written bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void SupportLegData::setData(const qsf::game::BitStream& bitStream, const int32 receivedHostTickCount)
		{
			// When this method is called at least one tracked data by this class has changed on the host side
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getReadPosition();
#endif
			HistoryEntry entry;
			entry.tickCount = receivedHostTickCount;

			// The animation state has only a value range of 0-3 => can be represented by two bits
			entry.mLastAnimationTypeChanged = detail::checkChangedAndRead(bitStream, 2, mLastAnimationType);
			entry.mLastAnimationType = mLastAnimationType;

			mHistoryList.emplace_back(entry);

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getReadPosition();
			QSF_LOG_PRINTS(INFO, "SupportLegData: read bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void SupportLegData::interpolate(qsf::Entity& targetEntity, const qsf::Clock& clock, const int32 receivedHostTickCount)
		{
			if (!mHistoryList.empty() && mComponent.valid())
			{
				auto iterator = mHistoryList.begin();
				while (iterator != mHistoryList.end())
				{
					const HistoryEntry& entry = *iterator;
					if (entry.tickCount == clock.getSignalCounter())
					{
						if (entry.mLastAnimationTypeChanged)
						{
							SupportLegsComponent::SupportLegsState animationState = static_cast<SupportLegsComponent::SupportLegsState>(entry.mLastAnimationType);
							switch (animationState)
							{
								case SupportLegsComponent::SupportLegsState::SUPPORTLEGSSTATE_EXTENDING_ANIMATION:
								{
									mComponent->startExtending();
								}
								break;
								case SupportLegsComponent::SupportLegsState::SUPPORTLEGSSTATE_HIDDING_ANIMATION:
								{
									mComponent->hideExtending();
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
