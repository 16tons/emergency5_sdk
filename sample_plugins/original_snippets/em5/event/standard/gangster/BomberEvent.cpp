// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/gangster/BomberEvent.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/gangster/ReachVictimAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/component/objects/BombComponent.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/freeplay/objective/ObjectiveHelper.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/game/targetpoint/GotoBombTargetPointProvider.h"
#include "em5/logic/local/gangsters/GangsterBaseLogic.h"
#include "em5/logic/ObserverHelper.h"
#include "em5/logic/observer/hint/HintShotAtObserver.h"
#include "em5/logic/HintHelper.h"
#include "em5/health/HealthComponent.h"
#include "em5/map/CollisionHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/plugin/Messages.h"

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf_game/base/DistanceHelper.h>

#include <qsf/audio/component/DynamicMusicCompositorComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/math/Random.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 BomberEvent::FREEPLAY_EVENT_ID = qsf::StringHash("em5::BomberEvent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	BomberEvent::BomberEvent() :
		mBombSpotId(qsf::getUninitialized<uint64>()),
		mBombId(qsf::getUninitialized<uint64>()),
		mBombTime(qsf::getUninitialized<float>()),
		mEscapeTime(qsf::getUninitialized<float>()),
		mState(RUNNING_TO_BOMB_SPOT),
		mBombExploded(false),
		mBombDeletionCountdown(qsf::Time::fromSeconds(10.0)) // Delete bomb prefab 10 seconds after detonation (when the explosion is 'finished')
	{
		// Nothing here
	}

	BomberEvent::~BomberEvent()
	{
		// Nothing here
	}

	uint64 BomberEvent::getBombSpotId() const
	{
		return mBombSpotId;
	}

	void BomberEvent::setBombSpot(const qsf::Entity& entity)
	{
		mBombSpotId = entity.getId();
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	bool BomberEvent::onStartup()
	{
		QSF_CHECK(mGangster.valid(), "Can't find gangster entity; BomberEvent method: onStartup", return false);

		qsf::Entity* bombSpotEntity = getMap().getEntityById(mBombSpotId);
		QSF_CHECK(bombSpotEntity, "Can't find bomb spot entity; BomberEvent method: onStartup", QSF_REACT_NONE);
		if (nullptr == bombSpotEntity)
			return false;

		EventIdComponent::addToEvent(*mGangster, *this, eventspreadreason::EVENT_SPECIFIC);

		// Place the gangster near to the victim. We have trouble with gangsters need ages to reach there goals, place them near the goals.
		qsf::TransformComponent& transformComponent = bombSpotEntity->getOrCreateComponentSafe<qsf::TransformComponent>();
		glm::vec3 newGangsterPosition = transformComponent.getPosition();

		CollisionHelper(mGangster->getMap()).findFreePositionEM3(mGangster.getSafe(), transformComponent.getPosition(), 10, 0.5, newGangsterPosition);
		mGangster->getOrCreateComponentSafe<qsf::TransformComponent>().setPosition(newGangsterPosition);

		// Order gangster to run to bomb spot
		qsf::ActionComponent& actionComponent = mGangster->getOrCreateComponentSafe<qsf::ActionComponent>();
		actionComponent.clearPlan();

		actionComponent.pushAction<MoveAction>(action::COMMAND_HIGH).init(new qsf::ai::ReachObjectGoal(mGangster.getSafe(), *bombSpotEntity, GotoBombTargetPointProvider::TARGET_POINT_ID));
		actionComponent.pushAction<ReachVictimAction>(action::COMMAND_HIGH, qsf::action::APPEND_TO_BACK).init(*bombSpotEntity);

		mState = RUNNING_TO_BOMB_SPOT;

		// Done
		return true;
	}

	void BomberEvent::onShutdown()
	{
		// Just to be sure, destroy bomb prefab if it still exists
		if (qsf::isInitialized(mBombId))
		{
			MapHelper(getMap()).destroyEntityById(mBombId);
		}

		mMessageProxy.unregister();
	}

	void BomberEvent::onRun()
	{
		// Activate the hint observers
		startHintObservers();
	}

	bool BomberEvent::onFailure(EventResult& eventResult)
	{
		const int i = qsf::Random::getRandomUint(0, 1);
		Objective* failConditionPersonEscapedObjective = eventResult.mObjectives.getObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_FAIL_PERSONSESCAPE);
		if (nullptr != failConditionPersonEscapedObjective && failConditionPersonEscapedObjective->checkAccomplished())
		{
			showHint("EM5_EVENT_GANGSTER_ESCAPING_FAIL_HINT_02", false);
		}
		else
		{
			showHint((i == 0) ? "EM5_EVENT_TOO_MANY_PERSONS_DIED_HINT_03" : "EM5_EVENT_TOO_MANY_PERSONS_DIED_HINT_04", false);
		}

		// Call base implementation
		return FreeplayEvent::onFailure(eventResult);
	}

	void BomberEvent::updateFreeplayEvent(const qsf::Time& timePassed)
	{
		// Check objectives for success or failure
		checkObjectivesState();
		if (getState() != State::STATE_RUNNING && getState() != State::STATE_HIDDEN)
			return;

		// Bomb already exploded?
		if (mBombExploded)
		{
			// Decrement timer
			mBombDeletionCountdown -= timePassed;
			if (mBombDeletionCountdown.getSeconds() <= 0.0f)
			{
				// Destroy bomb prefab
				if (qsf::isInitialized(mBombId))
				{
					MapHelper(getMap()).destroyEntityById(mBombId);
				}

				// Fail the bomb event
				setFinished(false);
			}
		}

		if (mGangster.valid() && EntityHelper(*mGangster).isArrested())
			return;

		switch (mState)
		{
			case RUNNING_TO_BOMB_SPOT:
			{
				QSF_CHECK(mGangster.valid(), "Can't find gangster entity; BomberEvent method: updateFreeplayEvent", QSF_REACT_NONE);
				if (!mGangster.valid() || !checkIfIsHealthy(*mGangster))
				{
					abortEvent();
				}
				else
				{
					qsf::Entity* bombSpotEntity = getMap().getEntityById(mBombSpotId);
					QSF_CHECK(bombSpotEntity, "Can't find bomb spot entity; BomberEvent method: updateFreeplayEvent", QSF_REACT_NONE);
					if (nullptr != bombSpotEntity && checkIfReachedBombSpot(*mGangster, *bombSpotEntity))
					{
						if (qsf::game::DistanceHelper::get2dDistance(*mGangster, *bombSpotEntity) > 5.0f)
						{
							QSF_WARN("Gangster cannot reach the bomb spot, possibly due to being stuck somewhere. Event will be aborted.", QSF_REACT_NONE);
							abortEvent();
							return;
						}

						setupGangster(*mGangster, *bombSpotEntity);

						qsf::ActionComponent& actionComponent = mGangster->getOrCreateComponentSafe<qsf::ActionComponent>();
						AnimationHelper animationHelper(*mGangster);
						actionComponent.pushAction<PlayAnimationAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(animationHelper.getAnimationManipulateObjectKneeling(), true);
						actionComponent.pushAction<TurnToAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(mBombSpotId);
						mState = GUARDING;
						setRunning();
					}
				}
				break;
			}

			case GUARDING:
			{
				qsf::Entity* bombEntity = getMap().getEntityById(mBombId);
				if (mGangster.valid() && nullptr != bombEntity)
				{
					qsf::Time timeLeft = bombEntity->getComponent<BombComponent>()->getTimeRunning();
					if (timeLeft < qsf::Time::fromSeconds(45.0f)) //TODO(mv) read value from json
					{
						showHint("EM5_GE04_HINT09", true);

						// Music
						getDynamicMusicCompositor().setMusicLevel(3);
					}
					if (timeLeft < qsf::Time::fromSeconds(mEscapeTime))
					{
						escape(*mGangster);
						mState = ESCAPING;
					}
				}
				break;
			}

			case ESCAPING:
			{
				// Fall through by design
			}

			case RESCUE:
			{
				break;
			}
		}

		if (mState != RESCUE)
		{
			if (mGangster.valid() && !checkIfIsHealthy(*mGangster))
			{
				ObjectiveHelper(*this).removeObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_ARRESTPERSONS);
				mState = RESCUE;
			}
		}
	}

	const qsf::Entity* BomberEvent::getFocusEntity()
	{
		if (mGangster.valid())
		{
			// High chance to show the gangster
			EntityHelper entityHelper(mGangster.getSafe());
			if (!entityHelper.isArrested() && !entityHelper.isPersonInjured())
			{
				if (qsf::Random::getRandomChance(0.5f))
				{
					return mGangster.get();
				}
			}
		}

		// Show random event entity
		if (!getEntityIds().empty())
		{
			uint64 entityId = qsf::Random::getAnyOf(getEntityIds());
			return getMap().getEntityById(entityId);
		}

		return getEventLocationEntity();
	}

	bool BomberEvent::addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason)
	{
		if (eventspreadreason::INJURY == eventSpreadReason && &targetEntity == mGangster.get())
		{
			ObjectiveHelper(*this).removeObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_ARRESTPERSONS);
		}

		return FreeplayEvent::addEntityToEvent(targetEntity, eventSpreadReason, newReason);
	}

	bool BomberEvent::checkEventConfiguration()
	{
		return mGangster.valid();
	}

	void BomberEvent::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base implementation
		FreeplayEvent::serialize(serializer);

		// TODO(fw): Fix this
		//serializer.serialize(mGangsterId);
		serializer.serialize(mBombSpotId);
		serializer.serialize(mBombId);
		serializer.serializeAs<uint16>(mState);
		serializer.serialize(mBombTime);
		serializer.serialize(mEscapeTime);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void BomberEvent::startHintObservers()
	{
		HintHelper(*this).createAndInitializeHintsAtLowLife(*mGangster);
		ObserverHelper(*this).createObserver<HintShotAtObserver>(mGangster->getId()).initialize();
	}

	bool BomberEvent::checkIfIsHealthy(const qsf::Entity& personEntity) const
	{
		return !personEntity.getComponent<HealthComponent>()->isInjured();
	}

	bool BomberEvent::checkIfReachedBombSpot(qsf::Entity& gangster, qsf::Entity& victim)
	{
		qsf::ActionComponent& actionComponent = gangster.getOrCreateComponentSafe<qsf::ActionComponent>();
		if (actionComponent.getPlan().isEmpty())
			return true;

		return (actionComponent.getCurrentAction()->getTypeId() != MoveAction::ACTION_ID);
	}

	void BomberEvent::setupGangster(qsf::Entity& gangster, qsf::Entity& bombSpot)
	{
		showSupervisorMessage("EM5_EVENT_SUPERV_OBJ_BOMBER_01");
		gangster.getOrCreateComponentSafe<qsf::ActionComponent>().clearPlan();
		GangsterBaseLogic& gangsterBaseLogic = EntityHelper(gangster).turnIntoGangsterByType(qsf::StringHash("BomberEventGangster"));

		qsf::Entity* bombEntity = getMap().createObjectByLocalPrefabAssetId(qsf::StringHash("em5/prefab/default/timebomb"));
		QSF_CHECK(bombEntity != nullptr, "BomberEvent: Can't create timebomb from prefab", return;)
		mBombId = bombEntity->getId();

		qsf::TransformComponent& transformComponent = bombEntity->getOrCreateComponentSafe<qsf::TransformComponent>();
		transformComponent.setPosition(bombSpot.getOrCreateComponent<qsf::TransformComponent>()->getPosition());

		// Set bombSpot position as way point for gangster
		gangsterBaseLogic.addWaypoint(*bombEntity);
		gangsterBaseLogic.setPatrolOnWaypoints(true);

		BombComponent& bombComponent = bombEntity->getOrCreateComponentSafe<BombComponent>();
		bombComponent.setTimeRunning(qsf::Time::fromSeconds(mBombTime));
		bombComponent.setTimeToDefuse(qsf::Time::fromSeconds(10.f)); //TODO(mv) read value from json?
		bombComponent.startBombTimer(*this);

		mMessageProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_ENTITY_BOMB_EXPLODED, mBombId), boost::bind(&BomberEvent::onExplode, this, _1));

		EventIdComponent::addToEvent(gangster, *this, eventspreadreason::GANGSTER);
		EventIdComponent::addToEvent(*bombEntity, *this, eventspreadreason::GANGSTER);
		qsf::ActionComponent& actionComponent = gangster.getOrCreateComponentSafe<qsf::ActionComponent>();
		actionComponent.clearPlan();

		createEventLocationEntity(*bombEntity);
	}

	void BomberEvent::escape(qsf::Entity& gangster)
	{
		showHint("EM5_EVENT_GANGSTER_ESCAPING_HINT_01", false);

		qsf::ActionComponent& actionComponent = gangster.getOrCreateComponentSafe<qsf::ActionComponent>();
		actionComponent.clearPlan();
		qsf::GameLogicComponent* gameLogicComponent = gangster.getComponent<qsf::GameLogicComponent>();
		if (nullptr != gameLogicComponent)
		{
			GangsterBaseLogic* gangsterLogic = gameLogicComponent->getGameLogic<GangsterBaseLogic>();
			if (nullptr != gangsterLogic)
			{
				gangsterLogic->setEscapeTargetTag(mEscapeTargetTag);
				gangsterLogic->escape(GangsterBaseLogic::ESCAPE_FOOT_SHORT);
			}
		}

		// Music
		getDynamicMusicCompositor().setMusicLevel(3);
	}

	void BomberEvent::onExplode(const qsf::MessageParameters& parameters)
	{
		mBombExploded = true;

		ObjectiveHelper(*this).removeObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_DISPOSEBOMB);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
