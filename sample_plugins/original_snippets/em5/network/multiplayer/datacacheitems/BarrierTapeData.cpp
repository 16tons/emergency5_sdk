// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/network/multiplayer/datacache/datacacheitems/BarrierTapeData.h"
#include "em5/network/multiplayer/datacache/DataCacheItemHelper.h"
#include "em5/component/objects/BarrierTapeComponent.h"

#include <qsf_game/network/BitStream.h>

#include <qsf/time/clock/Clock.h>
#include <qsf/log/LogSystem.h>
#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>
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
		BarrierTapeData::BarrierTapeData(const qsf::Entity& targetEntity) :
			DataCacheBase(targetEntity),
			mComponent(targetEntity.getComponent<BarrierTapeComponent>()),
			mNumberOfPoles(0),
			mPole{ {false, qsf::Math::GLM_VEC3_ZERO}, { false, qsf::Math::GLM_VEC3_ZERO } },
			mHasTapeChanged(false),
			mTapeExists(false),
			mTapeConnectionType(BarrierTapeComponent::POLE_TO_POLE),
			mTapeConnectionPoliceEntityId(qsf::getUninitialized<uint64>()),
			mTapeTransparency(0.0f),
			mIsFinishedBuilding(false),
			mIsBarrier(false)
		{
			if (mComponent.valid())
			{
				mNumberOfPoles = mComponent->getNumPoles();
				for (uint32 i = 0; i < mNumberOfPoles; ++i)
				{
					mPole[i].mPosition = mComponent->getPolePosition(i);
					mPole[i].mTransparency = mComponent->getPoleTransparency(i);
				}

				mTapeExists = mComponent->hasTape();
				if (mTapeExists)
				{
					mTapeConnectionType = mComponent->getTapeConnectionType();
					if (BarrierTapeComponent::POLE_TO_SQUAD == mTapeConnectionType)
					{
						mTapeConnectionPoliceEntityId = mComponent->getTapeConnectionPoliceEntityId();
					}

					mTapeTransparency = mComponent->getTapeTransparency();
				}

				mIsFinishedBuilding = mComponent->isFinishedBuilding();
				mIsBarrier = mComponent->isBarrier();
			}
			else
			{
				QSF_ERROR("BarrierTapeData: The entity \"" << targetEntity.getId() << "\" doesn't have a barrier tape component for caching the data by this class", QSF_REACT_THROW);
			}
		}

		BarrierTapeData::~BarrierTapeData()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::DataCacheBase methods ]
		//[-------------------------------------------------------]
		bool BarrierTapeData::prepareForUpdate(const qsf::Entity& targetEntity, const qsf::Clock& clock)
		{
			if (mComponent.valid())
			{
				const bool hasNumberOfPolesChanged = detail::checkForChanges(mComponent->getNumPoles(), mNumberOfPoles);

				bool hasPolePositionChanged = false;
				for (uint32 i = 0; i < mNumberOfPoles; ++i)
				{
					hasPolePositionChanged |= mPole[i].mHasPositionChanged = detail::checkForChanges(mComponent->getPolePosition(i), mPole[i].mPosition);
					hasPolePositionChanged |= detail::checkForChanges(mComponent->getPoleTransparency(i), mPole[i].mTransparency);
				}

				mHasTapeChanged = detail::checkForChanges(mComponent->hasTape(), mTapeExists);
				if (mTapeExists)
				{
					mHasTapeChanged |= detail::checkForChanges(mComponent->getTapeConnectionType(), mTapeConnectionType);
					if (BarrierTapeComponent::POLE_TO_SQUAD == mTapeConnectionType)
					{
						mHasTapeChanged |= detail::checkForChanges(mComponent->getTapeConnectionPoliceEntityId(), mTapeConnectionPoliceEntityId);
					}

					mHasTapeChanged |= detail::checkForChanges(mComponent->getTapeTransparency(), mTapeTransparency);
				}

				const bool hasIsFinishedBuildingChanged = detail::checkForChanges(mComponent->isFinishedBuilding(), mIsFinishedBuilding);
				const bool hasIsBarrierChanged = detail::checkForChanges(mComponent->isBarrier(), mIsBarrier);

				return hasNumberOfPolesChanged || hasPolePositionChanged || mHasTapeChanged || hasIsFinishedBuildingChanged || hasIsBarrierChanged;
			}

			return false;
		}

		void BarrierTapeData::updateData(qsf::game::BitStream& bitStream, bool force)
		{
			// When this method is called at least one tracked data by this class has changed
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getBitLength();
#endif

			bitStream.write(mNumberOfPoles);
			for (uint32 i = 0; i < mNumberOfPoles; ++i)
			{
				bitStream.write(mPole[i].mHasPositionChanged);
				if (mPole[i].mHasPositionChanged)
				{
					bitStream.write(mPole[i].mPosition.x);
					bitStream.write(mPole[i].mPosition.y);
					bitStream.write(mPole[i].mPosition.z);
				}

				bitStream.write(mPole[i].mTransparency);
			}

			bitStream.write(mHasTapeChanged);
			if (mHasTapeChanged)
			{
				bitStream.write(mTapeExists);
				if (mTapeExists)
				{
					bitStream.writeAs<uint32>(mTapeConnectionType, 1);
					if (BarrierTapeComponent::POLE_TO_SQUAD == mTapeConnectionType)
					{
						bitStream.write(mTapeConnectionPoliceEntityId);
					}

					bitStream.write(mTapeTransparency);
				}
			}

			bitStream.write(mIsFinishedBuilding);
			bitStream.write(mIsBarrier);

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getBitLength();
			QSF_LOG_PRINTS(INFO, "PersonMagnetData: written bitcount for entity \"" << mEntityId << "\": " << end-init);
#endif
		}

		void BarrierTapeData::setData(const qsf::game::BitStream& bitStream, const int32 receivedHostTickCount)
		{
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getReadPosition();
#endif
			HistoryEntry entry;
			entry.tickCount = receivedHostTickCount;

			bitStream.read(entry.mNumberOfPoles);
			for (uint32 i = 0; i < entry.mNumberOfPoles; ++i)
			{
				bitStream.read(entry.mPole[i].mHasPositionChanged);
				if (entry.mPole[i].mHasPositionChanged)
				{
					bitStream.read(entry.mPole[i].mPosition.x);
					bitStream.read(entry.mPole[i].mPosition.y);
					bitStream.read(entry.mPole[i].mPosition.z);
				}

				bitStream.read(entry.mPole[i].mTransparency);
			}

			bitStream.read(entry.mHasTapeChanged);
			if (entry.mHasTapeChanged)
			{
				bitStream.read(entry.mTapeExists);
				if (entry.mTapeExists)
				{
					bitStream.readAs<uint32>(entry.mTapeConnectionType, 1);
					if (BarrierTapeComponent::POLE_TO_SQUAD == entry.mTapeConnectionType)
					{
						bitStream.read(entry.mTapeConnectionPoliceEntityId);
					}

					bitStream.read(entry.mTapeTransparency);
				}
			}

			bitStream.read(entry.mIsFinishedBuilding);
			bitStream.read(entry.mIsBarrier);

			mHistoryList.emplace_back(entry);

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getReadPosition();
			QSF_LOG_PRINTS(INFO, "HelicopterData: read bitcount for entity \"" << mEntityId << "\": " << end-init);
#endif
		}

		void BarrierTapeData::interpolate(qsf::Entity& targetEntity, const qsf::Clock& clock, const int32 receivedHostTickCount)
		{
			if (!mHistoryList.empty() && mComponent.valid())
			{
				for (auto iterator = mHistoryList.begin(); iterator != mHistoryList.end(); )
				{
					HistoryEntry& entry = *iterator;
					if (entry.tickCount == clock.getSignalCounter())
					{
						// The current implementation does not allow the number of poles to decrease
						if (entry.mNumberOfPoles > mComponent->getNumPoles())
						{
							for (uint32 i = mComponent->getNumPoles(); i < entry.mNumberOfPoles; ++i)
							{
								mComponent->createPole(i);
							}
						}

						for (uint32 i = 0; i < entry.mNumberOfPoles; ++i)
						{
							if (entry.mPole[i].mHasPositionChanged)
							{
								mComponent->movePole(i, entry.mPole[i].mPosition);
								mComponent->deletePoleMarker(i);
							}
							mComponent->setPoleTransparency(i, entry.mPole[i].mTransparency);
						}

						if (entry.mHasTapeChanged)
						{
							if (entry.mTapeExists)
							{
								qsf::Entity* policeEntity = nullptr;
								if (BarrierTapeComponent::POLE_TO_SQUAD == entry.mTapeConnectionType)
								{
									policeEntity = mComponent->getEntity().getMap().getEntityById(entry.mTapeConnectionPoliceEntityId);
								}
								mComponent->createTape(entry.mTapeConnectionType, policeEntity);
								mComponent->setTapeTransparency(entry.mTapeTransparency);
							}
							else
							{
								mComponent->destroyTape();
							}
						}

						if (mComponent->isFinishedBuilding() != entry.mIsFinishedBuilding)
						{
							mComponent->setFinishedBuilding(entry.mIsFinishedBuilding);
						}

						if (mComponent->isBarrier() != entry.mIsBarrier)
						{
							// The current implementation does not allow the barrier to be destroyed explicitly
							if (entry.mIsBarrier)
							{
								mComponent->createBarrier();
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
