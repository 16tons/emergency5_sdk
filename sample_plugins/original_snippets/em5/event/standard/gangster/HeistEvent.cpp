// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/gangster/HeistEvent.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/base/EnterBuildingAction.h"
#include "em5/action/base/ExitBuildingAction.h"
#include "em5/action/gangster/ReachVictimAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/ai/MovementModes.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/freeplay/objective/ObjectiveHelper.h"
#include "em5/game/Game.h"
#include "em5/game/GameSimulation.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/game/spawn/SpawnPoolManager.h"
#include "em5/health/injury/Injuries.h"
#include "em5/health/HealthComponent.h"
#include "em5/logic/local/gangsters/GangsterCivilArmedLogic.h"
#include "em5/logic/ObserverHelper.h"
#include "em5/logic/observer/hint/HintShotAtObserver.h"
#include "em5/logic/observer/PersonDiedObserver.h"
#include "em5/logic/HintHelper.h"
#include "em5/map/CollisionHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/EM5Helper.h"

#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>

#include <qsf/audio/component/DynamicMusicCompositorComponent.h>
#include <qsf/component/move/MovableComponent.h>
#include <qsf/link/link/prototype/ContainerLink.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/action/WaitAction.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/math/Random.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 HeistEvent::FREEPLAY_EVENT_ID = qsf::StringHash("em5::HeistEvent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	HeistEvent::HeistEvent() :
		mBuildingId(qsf::getUninitialized<uint64>()),
		mVictimId(qsf::getUninitialized<uint64>()),
		mPerpetrating(false),
		mPerpetrated(false),
		mRescue(false),
		mHeistDuration(0.0f),
		mDontShootTriggered(false)
	{
		// Nothing here
	}

	HeistEvent::~HeistEvent()
	{
		// Nothing here
	}

	uint64 HeistEvent::getBuildingId() const
	{
		return mBuildingId;
	}

	void HeistEvent::setBuilding(qsf::Entity& entity)
	{
		mBuildingId = entity.getId();
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	bool HeistEvent::onStartup()
	{
		QSF_CHECK(mGangster.valid(), "Can't find gangster entity; HeistEvent method: onStartup", return false);

		qsf::Entity* buildingEntity = getMap().getEntityById(mBuildingId);
		QSF_CHECK(buildingEntity, "Can't find building entity; HeistEvent method: onStartup", QSF_REACT_NONE);
		if (nullptr == buildingEntity)
			return false;

		EventIdComponent::addToEvent(*mGangster, *this, eventspreadreason::EVENT_SPECIFIC);

		// Order gangster to run to building
		{
			// Place the gangster near to the building. We have trouble with gangsters need ages to reach there goals, place them near the goals.
			BuildingComponent& buildingComponent = buildingEntity->getComponentSafe<BuildingComponent>();
			glm::vec3 targetPosition = buildingComponent.getDoorPosition();
			glm::vec3 newGangsterPosition = targetPosition;
			CollisionHelper(mGangster->getMap()).findFreePositionEM3(mGangster.getSafe(), targetPosition, 10, 0.5, newGangsterPosition);
			mGangster->getOrCreateComponentSafe<qsf::TransformComponent>().setPosition(newGangsterPosition);

			// The gangster entity might be just created. To avoid t-pose until the move action takes place play the idle animation
			AnimationHelper animationHelper(*mGangster);
			animationHelper.clearAnimation();

			qsf::ActionComponent& actionComponent = mGangster->getOrCreateComponentSafe<qsf::ActionComponent>();
			actionComponent.clearPlan();

			qsf::ai::ReachSinglePointGoal* goal = new qsf::ai::ReachSinglePointGoal(qsf::logic::TargetPoint(targetPosition, false, 0.0f, 5.0f));
			actionComponent.pushAction<MoveAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(goal);
			actionComponent.pushAction<ReachVictimAction>(action::COMMAND_HIGH, qsf::action::APPEND_TO_BACK).init(*buildingEntity);
		}

		// Done
		return true;
	}

	void HeistEvent::onShutdown()
	{
		//Nothing to do here
	}

	void HeistEvent::onRun()
	{
		// Activate the hint observers
		startHintObservers();
	}

	bool HeistEvent::onFailure(EventResult& eventResult)
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

	void HeistEvent::updateFreeplayEvent(const qsf::Time& timePassed)
	{
		// Check objectives for success or failure
		checkObjectivesState();
		if (getState() != State::STATE_RUNNING && getState() != State::STATE_HIDDEN)
			return;

		qsf::Entity* buildingEntity = getMap().getEntityById(mBuildingId);

		if (mGangster.valid() && EntityHelper(*mGangster).isArrested())
			return;

		// Check if the gangster reached the victim
		if (!mPerpetrated && !mPerpetrating)
		{
			if (!mGangster.valid() || nullptr == buildingEntity || !checkIfIsHealthy(*mGangster))
			{
				// Error, don't start the event
				abortEvent();
				return;
			}

			if (checkIfReachedBuilding(*mGangster, *buildingEntity))
			{
				qsf::ActionComponent& actionComponent = mGangster->getOrCreateComponentSafe<qsf::ActionComponent>();
				actionComponent.pushAction<EnterBuildingAction>().init(*buildingEntity);
				actionComponent.pushAction<ExitBuildingAction>();
				setRunning();
				mPerpetrating = true;
			}
		}
		else if (mPerpetrating)
		{
			if (!mGangster.valid() || nullptr == buildingEntity || !checkIfIsHealthy(*mGangster))
			{
				// Error, don't start the event
				abortEvent();
				return;
			}

			qsf::ActionComponent& actionComponent = mGangster->getOrCreateComponentSafe<qsf::ActionComponent>();
			if (nullptr == actionComponent.getAction<ExitBuildingAction>())
			{
				setupGangster(*mGangster, *buildingEntity);
				actionComponent.pushAction<qsf::WaitAction>().init(qsf::Time::fromSeconds(mHeistDuration));

				mPerpetrating = false;
				mPerpetrated = true;
			}
		}
		else
		{
			if (!mRescue && mGangster.valid())
			{
				if (!checkIfIsHealthy(*mGangster))
				{
					ObjectiveHelper(*this).removeObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_ARRESTPERSONS);
					mRescue = true;
				}
			}
		}
	}

	void HeistEvent::hintCallback(Observer& hintObserver)
	{
		if (mDontShootTriggered)
		{
			return;
		}

		mDontShootTriggered = true;

		if (hintObserver.getTypeId() == HintShotAtObserver::GAMELOGIC_TYPE_ID)
		{
			showHint("EM5_EVENT_DONT_SHOOT_HINT_02", false);
		}
		else
		{
			showHint("EM5_EVENT_NEW_PERSON_INJURED_HINT_01", false);
		}
	}

	const qsf::Entity* HeistEvent::getFocusEntity()
	{
		// Show random event entity
		return getRandomEventEntity();
	}

	bool HeistEvent::checkEventConfiguration()
	{
		qsf::Entity* buildingEntity = getMap().getEntityById(mBuildingId);
		return (mGangster.valid()) && (nullptr != buildingEntity);
	}

	void HeistEvent::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base implementation
		FreeplayEvent::serialize(serializer);

		serializer.serialize(mBuildingId);
		serializer.serialize(mVictimId);
		serializer.serialize(mHeistDuration);
		serializer.serialize(mPerpetrating);
		serializer.serialize(mPerpetrated);
		serializer.serialize(mVictimPool);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void HeistEvent::startHintObservers()
	{
		HintHelper(*this).createAndInitializeHintsAtLowLife(*mGangster);
		ObserverHelper(*this).createObserver<HintShotAtObserver>(mGangster->getId()).initialize();
	}

	bool HeistEvent::checkIfReachedBuilding(qsf::Entity& gangster, qsf::Entity& victim)
	{
		qsf::ActionComponent& actionComponent = gangster.getOrCreateComponentSafe<qsf::ActionComponent>();
		if (actionComponent.getPlan().isEmpty())
			return true;

		return actionComponent.getCurrentAction()->getTypeId() != MoveAction::ACTION_ID;
	}

	void HeistEvent::setupGangster(qsf::Entity& gangster, qsf::Entity& building)
	{
		showSupervisorMessage("EM5_EVENT_SUPERV_OBJ_HEIST_01_01");
		EntityHelper(gangster).turnIntoGangster(qsf::StringHash("em5::GangsterCivilArmedLogic"), false);

		// Setup gangster logic
		GangsterBaseLogic* gangsterLogic = getGangsterLogic();
		if (nullptr != gangsterLogic)
		{
			gangsterLogic->setEscapeTargetTag(mEscapeTargetTag);
		}

		// Get Victim from building child links
		BuildingComponent& buildingComponent = building.getComponentSafe<BuildingComponent>();
		qsf::Entity* victim = nullptr;
		for (auto &p : buildingComponent.getLinks())
		{
			qsf::ContainerLink* containerLink = p.second;
			qsf::Entity& target = containerLink->getTargetEntity();

			EntityHelper entityHelper(target);
			if (!entityHelper.isValidGangsterEventTarget(gangster, true))
				continue;

			// Found victim, place it outside the building
			qsf::ActionComponent& actionComponent = target.getOrCreateComponentSafe<qsf::ActionComponent>();
			actionComponent.clearPlan();
			actionComponent.pushAction<ExitBuildingAction>();
			victim = &target;
			break;
		}

		if (nullptr == victim)
		{
			// Spawn victim and place it outside the building
			const std::string& randomName = EM5_GAME.getSimulation().getSpawnPoolManager().getRandomNameFromSpawnPool(mVictimPool);
			victim = MapHelper(getMap()).createObjectByLocalPrefabAssetId(qsf::StringHash(randomName));
			if (nullptr != victim)
			{
				qsf::TransformComponent& transformComponent = victim->getOrCreateComponentSafe<qsf::TransformComponent>();
				glm::vec3 position = buildingComponent.getDoorPosition();
				transformComponent.setPosition(position);

				qsf::MovableComponent* movableComponent = victim->getComponent<qsf::MovableComponent>();
				if (movableComponent != nullptr)
					movableComponent->warpToPositionAndRotation(position, qsf::Math::GLM_QUAT_IDENTITY);

				qsf::ActionComponent& actionComponent = victim->getOrCreateComponentSafe<qsf::ActionComponent>();
				actionComponent.clearPlan();
			}
		}

		if (nullptr == victim)
		{
			// Error, don't start the event
			abortEvent();
			return;
		}

		// Get injury for the victim
		uint32 injuryId = injury::GUN_SHOT_WOUND_LUNG;
		int j = qsf::Random::getRandomUint(0, 2);
		switch (j)
		{
			case 0:
				injuryId = injury::GUN_SHOT_WOUND_LUNG;
				break;

			case 1:
				injuryId = injury::GUN_SHOT_WOUND_BODY;
				break;

			case 2:
				injuryId = injury::GUN_SHOT_WOUND_BODY_HARD;
				break;
		}

		HealthComponent* healthComponent = victim->getComponent<HealthComponent>();
		healthComponent->injurePersonByEventById(injuryId, this);
		mVictimId = victim->getId();
		EventIdComponent::addToEvent(gangster, *this, eventspreadreason::GANGSTER);
		escape(gangster);
	}

	void HeistEvent::escape(qsf::Entity& gangster)
	{
		showHint("EM5_EVENT_GANGSTER_ESCAPING_HINT_01", false);

		qsf::ActionComponent& actionComponent = gangster.getOrCreateComponentSafe<qsf::ActionComponent>();
		actionComponent.clearPlan();

		qsf::GameLogicComponent* gameLogicComponent = gangster.getComponent<qsf::GameLogicComponent>();
		if (nullptr != gameLogicComponent)
		{
			GangsterBaseLogic* gameLogic = gameLogicComponent->getGameLogic<GangsterCivilArmedLogic>();
			if (nullptr != gameLogic)
			{
				gameLogic->escape(GangsterBaseLogic::ESCAPE_FOOT_LONG);
			}
		}

		// Music
		getDynamicMusicCompositor().setMusicLevel(3);
	}

	bool HeistEvent::checkIfIsHealthy(qsf::Entity& personEntity)
	{
		return !personEntity.getComponent<HealthComponent>()->isInjured();
	}

	bool HeistEvent::addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason)
	{
		if (eventSpreadReason == eventspreadreason::INJURY)
		{
			Objective& failConditionPersonDiedObjective = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_FAIL_DEADPERSONS);
			failConditionPersonDiedObjective.setNeededNumber(1);
			createObserver<PersonDiedObserver>(targetEntity.getId()).connectToObjective(failConditionPersonDiedObjective);
		}
		return FreeplayEvent::addEntityToEvent(targetEntity, eventSpreadReason, newReason);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
