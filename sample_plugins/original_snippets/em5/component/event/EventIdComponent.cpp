// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/freeplay/event/FreeplayEvent.h"
#include "em5/freeplay/FreeplaySystem.h"
#include "em5/game/gamemode/GameMode.h"
#include "em5/game/Game.h"
#include "em5/EM5Helper.h"

#include <qsf_ai/navigation/steering/stuckResolving/StuckResolverComponent.h>

#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>
#include <qsf/serialization/binary/BasicTypeSerialization.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 EventIdComponent::COMPONENT_ID = qsf::StringHash("em5::EventIdComponent");


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	bool EventIdComponent::addToEvent(qsf::Entity& entity, FreeplayEvent& freeplayEvent, eventspreadreason::Reason reason)
	{
		EventIdComponent& eventIdComponent = entity.getOrCreateComponentSafe<EventIdComponent>();
		return eventIdComponent.setEvent(freeplayEvent, reason);
	}

	bool EventIdComponent::tryLockForTeam(const qsf::Entity& targetEntity, const qsf::Entity& unitEntity)
	{
		CommandableComponent* commandableComponent = unitEntity.getComponent<CommandableComponent>();
		if (nullptr != commandableComponent)
		{
			EventIdComponent* eventIdComponent = targetEntity.getComponent<EventIdComponent>();
			if (nullptr != eventIdComponent)
			{
				if (eventIdComponent->isUnlockedForTeam(commandableComponent->getTeamId()))
				{
					eventIdComponent->lockForTeam(unitEntity.getId(), commandableComponent->getTeamId());
				}
				else
				{
					return false;
				}
			}
		}

		return true;
	}

	void EventIdComponent::releaseTeamLock(const qsf::Entity& targetEntity, const qsf::Entity& unitEntity)
	{
		CommandableComponent* commandableComponent = unitEntity.getComponent<CommandableComponent>();
		if (nullptr != commandableComponent)
		{
			EventIdComponent* eventIdComponent = targetEntity.getComponent<EventIdComponent>();
			if (nullptr != eventIdComponent)
			{
				eventIdComponent->releaseTeamLock(unitEntity.getId(), commandableComponent->getTeamId());
			}
		}
	}

	void EventIdComponent::releaseTeamLock(uint64 targetEntityId, qsf::Entity& unitEntity)
	{
		CommandableComponent* commandableComponent = unitEntity.getComponent<CommandableComponent>();
		if (nullptr != commandableComponent)
		{
			qsf::Entity* targetEntity = unitEntity.getMap().getEntityById(targetEntityId);
			if (nullptr != targetEntity)
			{
				EventIdComponent* eventIdComponent = targetEntity->getComponent<EventIdComponent>();
				if (nullptr != eventIdComponent)
				{
					eventIdComponent->releaseTeamLock(unitEntity.getId(), commandableComponent->getTeamId());
				}
			}
		}
	}

	bool EventIdComponent::isUnlockedForTeam(const qsf::Entity& targetEntity, const qsf::Entity& unitEntity)
	{
		CommandableComponent* commandableComponent = unitEntity.getComponent<CommandableComponent>();
		if (nullptr != commandableComponent)
		{
			EventIdComponent* eventIdComponent = targetEntity.getComponent<EventIdComponent>();
			if (nullptr != eventIdComponent)
			{
				return eventIdComponent->isUnlockedForTeam(commandableComponent->getTeamId());
			}
		}
		return true;
	}

	bool EventIdComponent::isUnlockedForTeam(const qsf::Entity& targetEntity, uint16 teamId)
	{
		EventIdComponent* eventIdComponent = targetEntity.getComponent<EventIdComponent>();
		if (nullptr != eventIdComponent)
		{
			return eventIdComponent->isUnlockedForTeam(teamId);
		}
		return true;
	}

	void EventIdComponent::registerPlayer(const qsf::Entity& targetEntity, const qsf::Entity& unitEntity)
	{
		CommandableComponent* commandableComponent = unitEntity.getComponent<CommandableComponent>();
		if (nullptr != commandableComponent)
		{
			EventIdComponent* eventIdComponent = targetEntity.getComponent<EventIdComponent>();
			if (nullptr != eventIdComponent && eventIdComponent->isUnlockedForTeam(commandableComponent->getTeamId()))
			{
				eventIdComponent->registerPlayerAsInteractedWithMe(commandableComponent->getPlayerId());
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	EventIdComponent::EventIdComponent(qsf::Prototype* prototype) :
		qsf::Component(prototype),
		mFreeplayEvent(nullptr),
		mCurrentTeamId(qsf::getUninitialized<uint16>()),
		mPlayerInteractedWithFlags(0)
	{
		// Nothing here
	}

	uint32 EventIdComponent::getEventId() const
	{
		return mFreeplayEvent.valid() ? mFreeplayEvent->getId() : qsf::getUninitialized<uint32>();
	}

	bool EventIdComponent::setEvent(FreeplayEvent& freeplayEvent, eventspreadreason::Reason eventSpreadReason)
	{
		// Only go on if there is no event set yet
		if (!mFreeplayEvent.valid())
		{
			// Assign event ID
			mFreeplayEvent = &freeplayEvent;

			// Register at the freeplay event
			mFreeplayEvent->insertEventId(*this);
		}
		else if (mFreeplayEvent != &freeplayEvent)
		{
			// Failed: Can not overwrite the freeplay event ID already set
			return false;
		}

		// Is event spread reason already set?
		if (mEventSpreadReasons.isSet(eventSpreadReason))
		{
			// Tell the freeplay event about the added entity
			mFreeplayEvent->addEntityToEvent(getEntity(), eventSpreadReason, false);
		}
		else
		{
			mEventSpreadReasons.set(eventSpreadReason);

			// Tell the freeplay event about the added entity
			mFreeplayEvent->addEntityToEvent(getEntity(), eventSpreadReason, true);
		}

		// Done
		return true;
	}

	void EventIdComponent::setEvent(FreeplayEvent& freeplayEvent)
	{
		// Only go on if there is no event set yet
		if (!mFreeplayEvent.valid())
		{
			// Assign event ID
			mFreeplayEvent = &freeplayEvent;
		}
	}

	//[-------------------------------------------------------]
	//[ Object locking                                        ]
	//[-------------------------------------------------------]
	void EventIdComponent::lockForTeam(uint64 unitEntityId, uint16 teamId)
	{
		if (qsf::isUninitialized(mCurrentTeamId) || mCurrentTeamId == teamId)
		{
			if (std::find(mCurrentPlayerRefCount.begin(), mCurrentPlayerRefCount.end(), unitEntityId) == mCurrentPlayerRefCount.end())
			{
				mCurrentTeamId = teamId;
				mCurrentPlayerRefCount.emplace_back(unitEntityId);
			}
		}
	}

	void EventIdComponent::releaseTeamLock(uint64 unitEntityId, uint16 teamId)
	{
		if (!mCurrentPlayerRefCount.empty() && mCurrentTeamId == teamId)
		{
			auto findResult = std::find(mCurrentPlayerRefCount.begin(), mCurrentPlayerRefCount.end(), unitEntityId);

			if (findResult != mCurrentPlayerRefCount.end())
			{
				mCurrentPlayerRefCount.erase(findResult);
				if (mCurrentPlayerRefCount.empty())
				{
					qsf::setUninitialized(mCurrentTeamId);
				}
			}
		}
	}

	bool EventIdComponent::isUnlockedForTeam(uint16 teamId)
	{
		return qsf::isUninitialized(mCurrentTeamId) || mCurrentTeamId == teamId;
	}

	uint16 EventIdComponent::getCurrentTeamId() const
	{
		return mCurrentTeamId;
	}

	void EventIdComponent::setCurrentTeamId(uint16 teamId)
	{
		mCurrentTeamId = teamId;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	void EventIdComponent::serialize(qsf::BinarySerializer& serializer)
	{
		if (serializer.isReading())
		{
			mFreeplayEvent = nullptr;
		}
	}

	bool EventIdComponent::onStartup()
	{
		qsf::ai::StuckResolverComponent* stuckResolverComponent = getEntity().getComponent<qsf::ai::StuckResolverComponent>();
		if (nullptr != stuckResolverComponent)
		{
			stuckResolverComponent->setObjectCanBeDeleted(false);
		}

		// Done
		return true;
	}


	void EventIdComponent::onShutdown()
	{
		if (mFreeplayEvent.valid())
		{
			// Unregister at freeplay event again
			mFreeplayEvent->removeEventIdComponent(*this);
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void EventIdComponent::registerPlayerAsInteractedWithMe(uint32 playerIndex)
	{
		// TODO(co) After the refactoring, we probably no longer need the "Game::getInstance()" check for the online editor server. But must be checked after changing it.
		if (Game::getInstance() ? (EM5_FREEPLAY.getGameMode()->isSingleInteraction()) : false)
		{
			// Only one player can interact with the event object -> Override any other player information
			mPlayerInteractedWithFlags = (1 << playerIndex);
		}
		else
		{
			mPlayerInteractedWithFlags |= (1 << playerIndex);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
