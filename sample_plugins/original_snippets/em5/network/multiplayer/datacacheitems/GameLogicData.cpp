// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/network/multiplayer/datacache/datacacheitems/GameLogicData.h"
#include "em5/network/multiplayer/datacache/DataCacheItemHelper.h"
#include "em5/logic/local/base/CarryPersonLogic.h"
#include "em5/logic/local/base/PullObjectLogic.h"
#include "em5/logic/local/firefighters/FiremanLogic.h"
#include "em5/logic/local/firefighters/CommandoCarLogic.h"
#include "em5/logic/local/police/GangsterInBuildingLogic.h"

#include <qsf_game/network/BitStream.h>

#include <qsf/logic/gamelogic/GameLogicComponent.h>
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
		GameLogicData::GameLogicData(const qsf::Entity& targetEntity) :
			DataCacheComponentExistenceTrackingBase<qsf::GameLogicComponent>(targetEntity),
			mCarriedPersonId(qsf::getUninitialized<uint64>()),
			mCarriedPersonIdChanged(false),
			mActiveClothing(qsf::getUninitialized<uint8>()),
			mActiveClothingChanged(false),
			mGangsterInBuildingFlagsChanged(false),
			mIsAreaExtinguishSkillReady(false),
			mIsAreaExtinguishSkillReadyChanged(false)
		{
			const qsf::GameLogicComponent* gamelogicComponent = getComponent(targetEntity);
			if (nullptr != gamelogicComponent)
			{
				CarryPersonLogic* carryPersonLogic = gamelogicComponent->getGameLogic<CarryPersonLogic>();
				if (nullptr != carryPersonLogic)
				{
					mCarriedPersonId = carryPersonLogic->getCarryPerson();
				}

				FiremanLogic* firemanLogic = gamelogicComponent->getGameLogic<FiremanLogic>();
				if (nullptr != firemanLogic)
				{
					mActiveClothing = static_cast<uint8>(firemanLogic->getActiveClothing());
				}

				GangsterInBuildingLogic* gangsterInBuildingLogic = gamelogicComponent->getGameLogic<GangsterInBuildingLogic>();
				if (nullptr != gangsterInBuildingLogic)
				{
					mGangsterInBuildingFlags.set(GangsterInBuildingLogicExists);
					if (gangsterInBuildingLogic->getAllowNormalPolice())
					{
						mGangsterInBuildingFlags.set(AllowNormalPolice);
					}
					if (gangsterInBuildingLogic->getCanSpeakWithHostageTaker())
					{
						mGangsterInBuildingFlags.set(CanSpeakWithHostageTaker);
					}
					if (gangsterInBuildingLogic->getInUseFromSpeaker())
					{
						mGangsterInBuildingFlags.set(InUseFromSpeaker);
					}
				}

				CommandoCarLogic* commandoCarLogic = gamelogicComponent->getGameLogic<CommandoCarLogic>();
				if (nullptr != commandoCarLogic)
				{
					mIsAreaExtinguishSkillReady = commandoCarLogic->isAreaExtinguishSkillReady();
				}
			}
		}

		GameLogicData::~GameLogicData()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::DataCacheBase methods ]
		//[-------------------------------------------------------]
		bool GameLogicData::prepareForUpdate(const qsf::Entity& targetEntity, const qsf::Clock& clock)
		{
			// Call base implementation
			bool componentExistenceChanged = DataCacheComponentExistenceTrackingBase<qsf::GameLogicComponent>::prepareForUpdate(targetEntity, clock);

			// Check for component data changes, when component exists
			const qsf::GameLogicComponent* gamelogicComponent = getComponent(targetEntity);
			if (nullptr != gamelogicComponent)
			{
				bool hasValue = false;
				uint64 carriedPersonId = qsf::getUninitialized<uint64>();

				const CarryPersonLogic* carryPersonLogic = gamelogicComponent->getGameLogic<CarryPersonLogic>();
				if (nullptr != carryPersonLogic)
				{
					hasValue = true;
					carriedPersonId = carryPersonLogic->getCarryPerson();
				}

				if (qsf::isUninitialized(carriedPersonId))
				{
					const FiremanLogic* firemanLogic = gamelogicComponent->getGameLogic<FiremanLogic>();
					if (nullptr != firemanLogic)
					{
						hasValue = true;
						carriedPersonId = firemanLogic->getPullingPersonId();
					}
				}

				if (qsf::isUninitialized(carriedPersonId))
				{
					const PullObjectLogic* pullobjectLogic = gamelogicComponent->getGameLogic<PullObjectLogic>();
					if (nullptr != pullobjectLogic)
					{
						qsf::Entity* pulledEntity = pullobjectLogic->getPulledObject();
						if (pulledEntity != nullptr)
						{
							carriedPersonId = pulledEntity ->getId();
							hasValue = true;
						}
					}
				}

				// Check for active clothing (only needed for firefighters)
				const FiremanLogic* firemanLogic = gamelogicComponent->getGameLogic<FiremanLogic>();
				if (nullptr != firemanLogic)
				{
					mActiveClothingChanged = detail::checkForChanges(static_cast<uint8>(firemanLogic->getActiveClothing()), mActiveClothing);
				}
				else
				{
					mActiveClothingChanged = false;
				}

				if (hasValue)
				{
					mCarriedPersonIdChanged = detail::checkForChanges(carriedPersonId, mCarriedPersonId);
				}
				else
				{
					mCarriedPersonIdChanged = false;
				}

				mGangsterInBuildingFlagsChanged = false;
				GangsterInBuildingLogic* gangsterInBuildingLogic = gamelogicComponent->getGameLogic<GangsterInBuildingLogic>();
				if (nullptr != gangsterInBuildingLogic)
				{
					if (!mGangsterInBuildingFlags.isSet(GangsterInBuildingLogicExists))
					{
						mGangsterInBuildingFlags.set(GangsterInBuildingLogicExists);
						mGangsterInBuildingFlagsChanged = true;
					}
					if (gangsterInBuildingLogic->getAllowNormalPolice() != mGangsterInBuildingFlags.isSet(AllowNormalPolice))
					{
						mGangsterInBuildingFlags.invert(AllowNormalPolice);
						mGangsterInBuildingFlagsChanged = true;
					}
					if (gangsterInBuildingLogic->getCanSpeakWithHostageTaker() != mGangsterInBuildingFlags.isSet(CanSpeakWithHostageTaker))
					{
						mGangsterInBuildingFlags.invert(CanSpeakWithHostageTaker);
						mGangsterInBuildingFlagsChanged = true;
					}
					if (gangsterInBuildingLogic->getInUseFromSpeaker() != mGangsterInBuildingFlags.isSet(InUseFromSpeaker))
					{
						mGangsterInBuildingFlags.invert(InUseFromSpeaker);
						mGangsterInBuildingFlagsChanged = true;
					}
				}
				else if (mGangsterInBuildingFlags.isSet(GangsterInBuildingLogicExists))
				{
					mGangsterInBuildingFlags.clearAll();
					mGangsterInBuildingFlagsChanged = true;
				}

				CommandoCarLogic* commandoCarLogic = gamelogicComponent->getGameLogic<CommandoCarLogic>();
				if (nullptr != commandoCarLogic)
				{
					mIsAreaExtinguishSkillReadyChanged = detail::checkForChanges(commandoCarLogic->isAreaExtinguishSkillReady(), mIsAreaExtinguishSkillReady);
				}
				else
				{
					mIsAreaExtinguishSkillReadyChanged = false;
				}

				return componentExistenceChanged || mCarriedPersonIdChanged || mActiveClothingChanged | mGangsterInBuildingFlagsChanged || mIsAreaExtinguishSkillReadyChanged;
			}

			return componentExistenceChanged;
		}

		void GameLogicData::updateData(qsf::game::BitStream& bitStream, bool force)
		{
			// When this method is called at least one tracked data by this class has changed
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getBitLength();
#endif

			// Call base implementation
			DataCacheComponentExistenceTrackingBase<qsf::GameLogicComponent>::updateData(bitStream, force);

			// Only write component data when the entity has the component
			if (hasEntityComponent())
			{
				detail::writeChanges(mCarriedPersonIdChanged, mCarriedPersonId, bitStream);
				detail::writeChanges(mActiveClothingChanged, mActiveClothing, bitStream);
				detail::writeChanges(mGangsterInBuildingFlagsChanged, mGangsterInBuildingFlags.getStorage(), 4, bitStream);
				detail::writeChanges(mIsAreaExtinguishSkillReadyChanged, mIsAreaExtinguishSkillReady, bitStream);
			}

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getBitLength();
			QSF_LOG_PRINTS(INFO, "GameLogicData: written bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void GameLogicData::setData(const qsf::game::BitStream& bitStream, const int32 receivedHostTickCount)
		{
			// When this method is called at least one tracked data by this class has changed on the host side
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getReadPosition();
#endif

			// Call base implementation
			DataCacheComponentExistenceTrackingBase<qsf::GameLogicComponent>::setData(bitStream, receivedHostTickCount);

			// Read the component data when the component exists
			if (hasEntityComponentFromStream())
			{
				HistoryEntry entry;
				entry.tickCount = receivedHostTickCount;
				entry.mCarriedPersonIdChanged = detail::checkChangedAndRead(bitStream, entry.mCarriedPersonId);
				entry.mActiveClothingChanged = detail::checkChangedAndRead(bitStream, entry.mActiveClothing);
				entry.mGangsterInBuildingFlagsChanged = detail::checkChangedAndRead(bitStream, 4, entry.mGangsterInBuildingFlags.getStorageRef());
				entry.mIsAreaExtinguishSkillReadyChanged = detail::checkChangedAndRead(bitStream, entry.mIsAreaExtinguishSkillReady);

				mHistoryList.emplace_back(entry);
			}

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getReadPosition();
			QSF_LOG_PRINTS(INFO, "GameLogicData: read bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void GameLogicData::interpolate(qsf::Entity& targetEntity, const qsf::Clock& clock, const int32 receivedHostTickCount)
		{
			// Call base implementation
			DataCacheComponentExistenceTrackingBase<qsf::GameLogicComponent>::interpolate(targetEntity, clock, receivedHostTickCount);

			if (!mHistoryList.empty())
			{
				qsf::GameLogicComponent* gamelogicComponent = getComponent(targetEntity);
				if (nullptr != gamelogicComponent )
				{
					auto iterator = mHistoryList.begin();
					while (iterator != mHistoryList.end())
					{
						const HistoryEntry& entry = *iterator;
						if (entry.tickCount == clock.getSignalCounter())
						{
							if (entry.mCarriedPersonIdChanged)
							{
								CarryPersonLogic* carryPersonLogic = gamelogicComponent->getGameLogic<CarryPersonLogic>();
								if (nullptr == carryPersonLogic)
								{
									carryPersonLogic = static_cast<CarryPersonLogic*>(gamelogicComponent->createGameLogicByTypeId(CarryPersonLogic::GAMELOGIC_TYPE_ID));
								}

								carryPersonLogic->setCarryPerson(entry.mCarriedPersonId);
							}

							if (entry.mActiveClothingChanged)
							{
								FiremanLogic* firemanLogic = gamelogicComponent->getOrCreateGameLogic<FiremanLogic>();
								firemanLogic->setActiveClothing(static_cast<FiremanLogic::Clothing>(entry.mActiveClothing), false);
							}

							if (entry.mGangsterInBuildingFlagsChanged)
							{
								if (entry.mGangsterInBuildingFlags.isSet(GangsterInBuildingLogicExists))
								{
									GangsterInBuildingLogic& gangsterInBuildingLogic = gamelogicComponent->getOrCreateGameLogicSafe<GangsterInBuildingLogic>();
									gangsterInBuildingLogic.setAllowNormalPolice(entry.mGangsterInBuildingFlags.isSet(AllowNormalPolice));
									gangsterInBuildingLogic.setCanSpeakWithHostageTaker(entry.mGangsterInBuildingFlags.isSet(CanSpeakWithHostageTaker));
									gangsterInBuildingLogic.setInUseFromSpeaker(entry.mGangsterInBuildingFlags.isSet(InUseFromSpeaker));
								}
								else
								{
									GangsterInBuildingLogic* gangsterInBuildingLogic = gamelogicComponent->getGameLogic<GangsterInBuildingLogic>();
									if (nullptr != gangsterInBuildingLogic)
									{
										gamelogicComponent->deleteGameLogic(*gangsterInBuildingLogic);
									}
								}
							}

							if (entry.mIsAreaExtinguishSkillReadyChanged)
							{
								CommandoCarLogic* commandoCarLogic = gamelogicComponent->getGameLogic<CommandoCarLogic>();
								if (nullptr != commandoCarLogic)
								{
									commandoCarLogic->mAreaExtinguishCooldownTimer = entry.mIsAreaExtinguishSkillReady ? qsf::Time::ZERO : qsf::Time::fromSeconds(1.0f);
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
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5
