// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/network/multiplayer/datacache/datacacheitems/HealthData.h"
#include "em5/network/multiplayer/datacache/DataCacheItemHelper.h"
#include "em5/health/HealthComponent.h"

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
		HealthData::HealthData(const qsf::Entity& targetEntity) :
			DataCacheBase(targetEntity),
			mInjuryIdChanged(false),
			mHealthChanged(false),
			mLifeChanged(false),
			mDeathChanged(false),
			mStateChanged(false),			
			mTreatmentActiveChanged(false),
			mDiagnosisActiveChanged(false),
			mDiagnosisCompleteChanged(false)
		{
			mComponent = targetEntity.getComponent<HealthComponent>();
			if (mComponent.valid())
			{
				mInjuryId = mComponent->getInjuryId();
				mHealthEnergy = mComponent->getHealthEnergy();
				mLifeEnergy = mComponent->getLifeEnergy();
				mDeathEnergy = mComponent->getDeathEnergy();
				mIsTreatmentActive = mComponent->isTreatmentActive();
				mIsDiagnosisActive = mComponent->isDiagnosisActive();
				mIsDiagnosisComplete = mComponent->isDiagnosisComplete();
				mState = static_cast<uint8>(mComponent->mState);
			}
			else
			{
				QSF_ERROR("HealthData: The entity \""<<targetEntity.getId()<<"\" doesn't have a health component for caching the data by this class", QSF_REACT_THROW);
			}
		}

		HealthData::~HealthData()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::DataCacheBase methods ]
		//[-------------------------------------------------------]
		bool HealthData::prepareForUpdate(const qsf::Entity& targetEntity, const qsf::Clock& clock)
		{
			if (mComponent.valid())
			{
				mInjuryIdChanged = mComponent->getInjuryId() != mInjuryId;
				if (mInjuryIdChanged)
				{
					mInjuryId = mComponent->getInjuryId();
				}

				mHealthChanged = mComponent->getHealthEnergy() != mHealthEnergy;
				if (mHealthChanged)
				{
					mHealthEnergy = mComponent->getHealthEnergy();
				}

				mLifeChanged = mComponent->getLifeEnergy() != mLifeEnergy;
				if (mLifeChanged)
				{
					mLifeEnergy = mComponent->getLifeEnergy();
				}

				mDeathChanged = mComponent->getDeathEnergy() != mDeathEnergy;
				if (mDeathChanged)
				{
					mDeathEnergy = mComponent->getDeathEnergy();
				}

				const uint8 healthState = static_cast<uint8>(mComponent->mState);
				mStateChanged = detail::checkForChanges(healthState, mState);

				mTreatmentActiveChanged = mComponent->isTreatmentActive() != mIsTreatmentActive;
				if (mTreatmentActiveChanged)
				{
					mIsTreatmentActive = mComponent->isTreatmentActive();
				}

				mDiagnosisActiveChanged = mComponent->isDiagnosisActive() != mIsDiagnosisActive;
				if (mDiagnosisActiveChanged)
				{
					mIsDiagnosisActive = mComponent->isDiagnosisActive();
				}

				mDiagnosisCompleteChanged = mComponent->isDiagnosisComplete() != mIsDiagnosisComplete;
				if (mDiagnosisCompleteChanged)
				{
					mIsDiagnosisComplete = mComponent->isDiagnosisComplete();
				}

				return mInjuryIdChanged || mHealthChanged || mLifeChanged || mDeathChanged || mStateChanged || mTreatmentActiveChanged || mDiagnosisActiveChanged || mDiagnosisCompleteChanged;
			}

			return false;
		}

		void HealthData::updateData(qsf::game::BitStream& bitStream, bool force)
		{
			// When this method is called at least one tracked data by this class has changed
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getBitLength();
#endif

			detail::writeChanges(mInjuryIdChanged, mInjuryId, bitStream);

			detail::writeChanges(mHealthChanged, mHealthEnergy, bitStream);

			detail::writeChanges(mLifeChanged, mLifeEnergy, bitStream);

			detail::writeChanges(mDeathChanged, mDeathEnergy, bitStream);

			// Health state is currently in range 0-3 -> 2 bits
			detail::writeChanges(mStateChanged, mState, 2, bitStream);

			detail::writeChanges(mTreatmentActiveChanged, mIsTreatmentActive, bitStream);

			detail::writeChanges(mDiagnosisActiveChanged, mIsDiagnosisActive, bitStream);

			detail::writeChanges(mDiagnosisCompleteChanged, mIsDiagnosisComplete, bitStream);

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getBitLength();
			QSF_LOG_PRINTS(INFO, "HealthData: written bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void HealthData::setData(const qsf::game::BitStream& bitStream, const int32 receivedHostTickCount)
		{
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getReadPosition();
#endif
			HistoryEntry entry;
			entry.tickCount = receivedHostTickCount;

			entry.mInjuryIdChanged =detail::checkChangedAndRead(bitStream, entry.mInjuryId);

			entry.mHealthChanged = detail::checkChangedAndRead(bitStream, entry.mHealthEnergy);

			entry.mLifeChanged = detail::checkChangedAndRead(bitStream, entry.mLifeEnergy);

			entry.mDeathChanged = detail::checkChangedAndRead(bitStream, entry.mDeathEnergy);

			// Health state is currently in range 0-3 -> 2 bits
			entry.mStateChanged = detail::checkChangedAndRead(bitStream, 2, entry.mState);

			entry.mTreatmentActiveChanged = detail::checkChangedAndRead(bitStream, entry.mIsTreatmentActive);

			entry.mDiagnosisActiveChanged = detail::checkChangedAndRead(bitStream, entry.mIsDiagnosisActive);

			entry.mDiagnosisCompleteChanged = detail::checkChangedAndRead(bitStream, entry.mIsDiagnosisComplete);

			mHistoryList.emplace_back(entry);

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getReadPosition();
			QSF_LOG_PRINTS(INFO, "HealthData: read bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void HealthData::interpolate(qsf::Entity& targetEntity, const qsf::Clock& clock, const int32 receivedHostTickCount)
		{
			if (!mHistoryList.empty() && mComponent.valid())
			{
				auto iterator = mHistoryList.begin();
				while (iterator != mHistoryList.end())
				{
					const HistoryEntry& entry = *iterator;
					if (entry.tickCount == clock.getSignalCounter())
					{
						if (entry.mInjuryIdChanged)
						{
							// The data has changed -> apply it
							// No need to know here the freeplay event the event id is tracked separately
							// No need to play the collapse animation via this call, the animations are handled via a other cache entry
							mComponent->applyInjuryById(entry.mInjuryId, nullptr, false);
						}

						if (entry.mHealthChanged)
						{
							// The data has changed -> apply it
							mComponent->setHealthEnergy(entry.mHealthEnergy);
						}

						if (entry.mLifeChanged)
						{
							// The data has changed -> apply it
							mComponent->setLifeEnergy(entry.mLifeEnergy);
						}

						if (entry.mDeathChanged)
						{
							// The data has changed -> apply it
							mComponent->setDeathEnergy(entry.mDeathEnergy);
						}

						if (entry.mTreatmentActiveChanged)
						{
							// The data has changed -> apply it
							mComponent->setTreatmentActive(entry.mIsTreatmentActive);
						}

						if (entry.mDiagnosisActiveChanged)
						{
							// The data has changed -> apply it
							mComponent->setDiagnosisActive(entry.mIsDiagnosisActive);
						}

						if (entry.mDiagnosisCompleteChanged)
						{
							// The data has changed -> apply it
							mComponent->setDiagnosisComplete(entry.mIsDiagnosisComplete);
						}

						if (entry.mStateChanged)
						{
							mComponent->mState = static_cast<HealthComponent::State>(entry.mState);
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
