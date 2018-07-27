// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/network/multiplayer/datacache/datacacheitems/ActionData.h"
#include "em5/network/multiplayer/datacache/DataCacheItemHelper.h"
#include "em5/action/multiplayer/MultiplayerDummyAction.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/network/BitStream.h>

#include <qsf/log/LogSystem.h>
#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>
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
		ActionData::ActionData(const qsf::Entity& targetEntity) :
			DataCacheComponentExistenceTrackingBase<qsf::ActionComponent>(targetEntity),
			mCurrentPriority(0),
			mCurrentPriorityChanged(false)
		{
			const qsf::ActionComponent* actionComponent = getComponent(targetEntity);
			if (nullptr != actionComponent)
			{
				mCurrentPriority = actionComponent->getCurrentPriority();
			}
		}

		ActionData::~ActionData()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::DataCacheBase methods ]
		//[-------------------------------------------------------]
		bool ActionData::prepareForUpdate(const qsf::Entity& targetEntity, const qsf::Clock& clock)
		{
			// Call base implementation
			bool componentExistenceChanged = DataCacheComponentExistenceTrackingBase<qsf::ActionComponent>::prepareForUpdate(targetEntity, clock);

			// Check for component data changes, when component exists
			const qsf::ActionComponent* actionComponent = getComponent(targetEntity);
			if (nullptr != actionComponent)
			{
				mCurrentPriorityChanged = detail::checkForChanges(actionComponent->getCurrentPriority(), mCurrentPriority);

				return componentExistenceChanged || mCurrentPriorityChanged;
			}
			else if (componentExistenceChanged)
			{
				// Only do an reset if the component existence has changed and the component was destroyed
				// Reset cache data
				mCurrentPriority = 0;
				mCurrentPriorityChanged = false;
			}

			return componentExistenceChanged;
		}

		void ActionData::updateData(qsf::game::BitStream& bitStream, bool force)
		{
			// When this method is called at least one tracked data by this class has changed
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getBitLength();
#endif
			// Call base implementation
			DataCacheComponentExistenceTrackingBase<qsf::ActionComponent>::updateData(bitStream, force);

			// Only write component data when the entity has the component
			if (hasEntityComponent())
			{
				detail::writeChanges(mCurrentPriorityChanged, mCurrentPriority, 16, bitStream);
			}

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getBitLength();
			QSF_LOG_PRINTS(INFO, "ActionData: written bitcount for entity \"" << mEntityId << "\": " << end-init);
#endif
		}

		void ActionData::setData(const qsf::game::BitStream& bitStream, const int32 receivedHostTickCount)
		{
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getReadPosition();
#endif
			// Call base implementation
			DataCacheComponentExistenceTrackingBase<qsf::ActionComponent>::setData(bitStream, receivedHostTickCount);

			// Read the component data when the component exists
			if (hasEntityComponentFromStream())
			{
				if (detail::checkChanged(bitStream))
				{
					HistoryEntry entry;
					entry.tickCount = receivedHostTickCount;

					bitStream.read(entry.mCurrentPriority, 16);

					mHistoryList.emplace_back(entry);
				}
			}

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getReadPosition();
			QSF_LOG_PRINTS(INFO, "ActionData: read bitcount for entity \"" << mEntityId << "\": " << end-init);
#endif
		}

		void ActionData::interpolate(qsf::Entity& targetEntity, const qsf::Clock& clock, const int32 receivedHostTickCount)
		{
			// Call base implementation
			DataCacheComponentExistenceTrackingBase<qsf::ActionComponent>::interpolate(targetEntity, clock, receivedHostTickCount);

			if (!mHistoryList.empty())
			{
				qsf::ActionComponent* actionComponent = getComponent(targetEntity);
				if (nullptr != actionComponent)
				{
					auto iterator = mHistoryList.begin();
					while (iterator != mHistoryList.end())
					{
						const HistoryEntry& entry = *iterator;
						if (entry.tickCount == clock.getSignalCounter())
						{
							qsf::Action* currentAction = actionComponent->getCurrentAction();
							if (nullptr == currentAction)
							{
								actionComponent->pushAction<MultiplayerDummyAction>(entry.mCurrentPriority);
							}
							else
							{
								// On multiplayer client side we have only one action -> it is the multiplayer dummy action
								QSF_CHECK_ONCE(currentAction->getTypeId() == MultiplayerDummyAction::ACTION_ID, "Multiplayer client: entity \"" << mEntityId << "\" has an invalid action in its action plan", QSF_REACT_NONE);
								currentAction->setPriority(entry.mCurrentPriority);
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
