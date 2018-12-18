// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/gangster/RowdyEvent.h"
#include "em5/action/base/CloseQuartersCombatAction.h"
#include "em5/action/gangster/ReachVictimAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/ai/MovementModes.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/freeplay/objective/ObjectiveHelper.h"
#include "em5/game/targetpoint/AttackPersonTargetPointProvider.h"
#include "em5/health/HealthComponent.h"
#include "em5/logic/ObserverHelper.h"
#include "em5/logic/local/gangsters/GangsterBaseLogic.h"
#include "em5/logic/observer/PersonDiedObserver.h"
#include "em5/logic/observer/hint/HintShotAtObserver.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf_game/base/DistanceHelper.h>

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/action/WaitAction.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/math/Random.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 RowdyEvent::FREEPLAY_EVENT_ID = qsf::StringHash("em5::RowdyEvent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	RowdyEvent::RowdyEvent() :
		mBuildingId(qsf::getUninitialized<uint64>()),
		mGangsterId(qsf::getUninitialized<uint64>()),
		mCurrentVictimId(qsf::getUninitialized<uint64>()),
		mAttacksLeft(0),
		mWaiting(true),
		mRowdyHurt(false),
		mVisible(false),
		mDontShootTriggered(false)
	{
		// Nothing here
	}

	RowdyEvent::~RowdyEvent()
	{
		// Nothing here
	}

	uint64 RowdyEvent::getGangsterId() const
	{
		return mGangsterId;
	}

	void RowdyEvent::setGangster(const qsf::Entity& entity)
	{
		mGangsterId = entity.getId();
	}

	uint64 RowdyEvent::getBuildingId() const
	{
		return mBuildingId;
	}

	void RowdyEvent::setBuilding(const qsf::Entity& entity)
	{
		mBuildingId = entity.getId();
	}

	void RowdyEvent::setPeopleToAttack(uint64 peopleToAttack)
	{
		mAttacksLeft = peopleToAttack;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	bool RowdyEvent::onStartup()
	{
		qsf::Entity* buildingEntity = getMap().getEntityById(mBuildingId);
		QSF_CHECK(buildingEntity, "Can't find building entity; RowdyEvent method: onStartup", QSF_REACT_NONE);
		if (nullptr == buildingEntity)
			return false;

		// TODO(mk) Use the same gangster pool for all gangster events, and use in assets defined spawnpool
		static std::vector<std::string> riotGangsterPool;
		if (riotGangsterPool.empty())
		{
			riotGangsterPool.emplace_back("em5/prefab/person/civil_gangster_male01");
			riotGangsterPool.emplace_back("em5/prefab/person/civil_hooligan_male01");
			riotGangsterPool.emplace_back("em5/prefab/person/civil_hooligan_male02");
			riotGangsterPool.emplace_back("em5/prefab/person/civil_rocker_male01");
			riotGangsterPool.emplace_back("em5/prefab/person/civil_rocker_male02");
		}

		// Pick a random one
		std::string randomPrefabName = qsf::Random::getAnyOf(riotGangsterPool);

		qsf::Entity* gangsterEntity = MapHelper(getMap()).createObjectByLocalPrefabAssetId(qsf::StringHash(randomPrefabName));
		if (nullptr == gangsterEntity)
			return false;

		mGangsterId = gangsterEntity->getId();
		GangsterEvent::setGangster(*gangsterEntity, false);

		// Setup gangster logic
		GangsterBaseLogic* gangsterLogic = getGangsterLogic();
		if (nullptr != gangsterLogic)
		{
			gangsterLogic->setEscapeTargetTag(mEscapeTargetTag);
		}

		// TODO(fw): mGangster weak ptr from base class could be used much more in this class

		const glm::vec3& doorPosition = buildingEntity->getComponent<BuildingComponent>()->getDoorPosition();
		gangsterEntity->getOrCreateComponentSafe<qsf::TransformComponent>().setPosition(doorPosition);

		qsf::MovableComponent* movableComponent = gangsterEntity->getComponent<qsf::MovableComponent>();
		if (movableComponent != nullptr)
		{
			movableComponent->warpToPositionAndRotation(doorPosition, qsf::Math::GLM_QUAT_IDENTITY);
		}
		gangsterEntity->getOrCreateComponentSafe<qsf::ActionComponent>().clearPlan();
		gangsterEntity->getOrCreateComponentSafe<PersonComponent>().setPersonFlags(PersonComponent::FLAG_DRUNK, true);

		EventIdComponent::addToEvent(*gangsterEntity, *this, eventspreadreason::EVENT_SPECIFIC);

		// Done
		return true;
	}

	void RowdyEvent::onShutdown()
	{
		// Nothing to do here
	}

	void RowdyEvent::onRun()
	{
		// Activate the hint observers
		startHintObservers();
	}

	bool RowdyEvent::onFailure(EventResult& eventResult)
	{
		if (qsf::Random::getRandomUint(0, 1) == 0)
		{
			showHint("EM5_EVENT_TOO_MANY_PERSONS_DIED_HINT_03", false);
		}
		else
		{
			showHint("EM5_EVENT_TOO_MANY_PERSONS_DIED_HINT_04", false);
		}

		// Call base implementation
		return FreeplayEvent::onFailure(eventResult);
	}

	void RowdyEvent::updateFreeplayEvent(const qsf::Time& timePassed)
	{
		// Check objectives for success or failure
		checkObjectivesState();
		if (getState() != State::STATE_RUNNING && getState() != State::STATE_HIDDEN)
			return;

		qsf::Entity* gangsterEntity = getMap().getEntityById(mGangsterId);
		if (nullptr != gangsterEntity && EntityHelper(*gangsterEntity).isArrested())
			return;

		if (!mVisible)
		{
			QSF_CHECK(nullptr != gangsterEntity, "No Gangster Entity found!", abortEvent(); return);
			qsf::Entity* victimEntity = getMap().getEntityById(mCurrentVictimId);
			if (nullptr != victimEntity && checkIfReachedVictim(*gangsterEntity, *victimEntity))
			{
				mVisible = true;
				setRunning();
				showSupervisorMessage("EM5_EVENT_SUPERV_OBJ_ROWDY_01");
				EntityHelper(*gangsterEntity).turnIntoGangster(qsf::StringHash("em5::GangsterBaseLogic"), false);
				EventIdComponent::addToEvent(*gangsterEntity, *this, eventspreadreason::GANGSTER);
				startObjectives(*gangsterEntity);
			}
		}

		if (nullptr == gangsterEntity)
		{
			checkObjectivesState();
			return;
		}

		if (!mRowdyHurt && !checkIfIsHealthy(*gangsterEntity))
		{
			ObjectiveHelper(*this).removeObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_ARRESTPERSONS);
			mCurrentVictimId = qsf::getUninitialized<uint64>();
			mRowdyHurt = true;
		}

		// Case the rowdy is hurt
		if (mRowdyHurt)
		{
			checkObjectivesState();
			return;
		}

		// Case the rowdy attacked enough persons
		if (mAttacksLeft <= 0)
		{
			// Now the rowdy attacks only police units
			if (mWaiting)
			{
				qsf::Entity* target = findNewPoliceTarget(*gangsterEntity);
				if (nullptr != target)
				{
					attackTarget(*gangsterEntity, *target);
				}
			}

			checkObjectivesState();
			return;
		}

		if (mWaiting)
		{
			qsf::Entity* target = findNewTarget(*gangsterEntity);
			if (nullptr != target)
			{
				attackTarget(*gangsterEntity, *target);
			}

			checkObjectivesState();
			return;
		}

		qsf::Entity* victimEntity = getMap().getEntityById(mCurrentVictimId);
		if (nullptr != victimEntity && gangsterEntity->getComponent<qsf::ActionComponent>()->getPlan().isEmpty())
		{
			// Reaches this code if the gangster is finished with this closeCombatAction
			ObserverHelper observerHelper(*this);

			Objective& failConditionPersonDiedObjective = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_FAIL_DEADPERSONS);
			observerHelper.createObserver<PersonDiedObserver>(mCurrentVictimId).connectToObjective(failConditionPersonDiedObjective);

			mCurrentVictimId = qsf::getUninitialized<uint64>();
			mWaiting = true;
			--mAttacksLeft;
		}

		// Victim is despawned or inside building or vehicles
		if (nullptr == victimEntity || nullptr != EntityHelper(*victimEntity).getContainerEntity())
		{
			mCurrentVictimId = qsf::getUninitialized<uint64>();
			mWaiting = true;
		}

		checkObjectivesState();
	}

	void RowdyEvent::hintCallback(Observer& hintObserver)
	{
		if (!mDontShootTriggered)
		{
			mDontShootTriggered = true;

			showHint("EM5_EVENT_DONT_SHOOT_HINT_02", false);
		}
	}

	const qsf::Entity* RowdyEvent::getFocusEntity()
	{
		qsf::Entity* gangster = getMap().getEntityById(mGangsterId);
		if (nullptr != gangster && !EntityHelper(*gangster).isArrested())
		{
			// Gangster valid and not (yet) arrested. Return it.
			return gangster;
		}

		const EntityIdArray& victims = getEntityIds();
		for (int i = 0; i < static_cast<int>(victims.size()); ++i)
		{
			const qsf::Entity* victim = getMap().getEntityById(victims.at(i));
			if (nullptr != victim)
			{
				const HealthComponent* healthComponent = victim->getComponent<HealthComponent>();
				if (nullptr != healthComponent && healthComponent->isInjured())
				{
					// Event-related entity is injured, probably an injured victim. Return it.
					return victim;
				}
			}
		}

		// Nothing to do here
		return nullptr;
	}

	bool RowdyEvent::checkEventConfiguration()
	{
		return (nullptr != getMap().getEntityById(mBuildingId));
	}

	void RowdyEvent::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base implementation
		FreeplayEvent::serialize(serializer);

		serializer.serialize(mGangsterId);
		serializer.serialize(mBuildingId);
		serializer.serialize(mCurrentVictimId);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void RowdyEvent::startObjectives(qsf::Entity& targetEntity)
	{
		// Create the fail condition
		Objective& failConditionPersonDiedObjective = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_FAIL_DEADPERSONS);
		failConditionPersonDiedObjective.setNeededNumber(1); // Hard scripted value
	}

	void RowdyEvent::startHintObservers()
	{
		ObserverHelper(*this).createObserver<HintShotAtObserver>(mGangsterId).initialize();
	}

	bool RowdyEvent::checkIfReachedVictim(qsf::Entity& gangster, qsf::Entity& victim) const
	{
		qsf::ActionComponent& actionComponent = gangster.getOrCreateComponentSafe<qsf::ActionComponent>();
		if (actionComponent.getPlan().isEmpty())
			return true;

		return (actionComponent.getCurrentAction()->getTypeId() != MoveAction::ACTION_ID);
	}

	bool RowdyEvent::checkIfIsHealthy(const qsf::Entity& personEntity) const
	{
		return !personEntity.getComponent<HealthComponent>()->isInjured();
	}

	qsf::Entity* RowdyEvent::findNewTarget(qsf::Entity& gangster) const
	{
		static const float searchRange = 50.0f;

		// Get a listing of all person components in radius
		std::vector<PersonComponent*> personComponentsList;
		glm::vec3 position = EntityHelper(gangster).getPosition();
		qsf::ComponentMapQuery(getMap()).getInstancesInCircle(position, searchRange, personComponentsList);

		// Check nearby targets
		for (PersonComponent* personComponent : personComponentsList)
		{
			// Check if entity is the gangster
			qsf::Entity& entity = personComponent->getEntity();
			if (&entity == &gangster)
				continue;

			// Some checks
			if (!checkVictim(gangster, entity))
				continue;

			return &entity;
		}

		// If no victim in reach search nearest on map
		const auto& personComponentsInMap = qsf::ComponentMapQuery(getMap()).getAllInstances<PersonComponent>();

		float closestDistance = std::numeric_limits<float>::max();
		qsf::Entity* closestEntity = nullptr;

		// Check all targets, take closest
		for (PersonComponent* personComponent : personComponentsInMap)
		{
			// Check if entity is not the gangster
			qsf::Entity& entity = personComponent->getEntity();
			if (&entity == &gangster)
				continue;

			// Some checks
			if (!checkVictim(gangster, entity))
				continue;

			// Get closest
			const float currentDistance = qsf::game::DistanceHelper::getDistance(gangster, entity);
			if (currentDistance < closestDistance)
			{
				closestDistance = currentDistance;
				closestEntity = &entity;
			}
		}

		return closestEntity;
	}

	void RowdyEvent::attackTarget(qsf::Entity& gangsterEntity, qsf::Entity& targetEntity)
	{
		qsf::ActionComponent& actionComponent = gangsterEntity.getOrCreateComponentSafe<qsf::ActionComponent>();
		actionComponent.clearPlan();

		// Move to target
		qsf::ai::ReachObjectGoal* reachObjectGoal = new qsf::ai::ReachObjectGoal(gangsterEntity, targetEntity, AttackPersonTargetPointProvider::TARGET_POINT_ID);
		actionComponent.pushAction<MoveAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(reachObjectGoal, MovementModes::MOVEMENT_MODE_RUN);

		// Start fight
		actionComponent.pushAction<ReachVictimAction>(action::COMMAND_HIGH, qsf::action::APPEND_TO_BACK).init(targetEntity);
		actionComponent.pushAction<CloseQuartersCombatAction>(action::COMMAND_HIGH, qsf::action::APPEND_TO_BACK).init(targetEntity, weapon::FIST, weapon::FIST, 0, 0.2f, 0.02f);

		// Let the victim wait to better reach it
		if (!EntityHelper(targetEntity).isSquadPerson())
		{
			qsf::ActionComponent& victimActionComponent = targetEntity.getOrCreateComponentSafe<qsf::ActionComponent>();
			victimActionComponent.pushAction<qsf::WaitAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(qsf::Time::fromSeconds(15.0f));
		}

		// Set event ID
		targetEntity.getOrCreateComponent<EventIdComponent>()->setEvent(*this, eventspreadreason::NO_REASON);

		mWaiting = false;
		mCurrentVictimId = targetEntity.getId();
	}

	bool RowdyEvent::checkVictim(qsf::Entity& gangster, qsf::Entity& victim) const
	{
		if (!EntityHelper(victim).isValidGangsterEventTarget(gangster, false))
			return false;

		// Only victims in gangster event area!
		if (!EntityHelper(victim).isInGangsterEventArea())
			return false;

		if (nullptr != victim.getComponent<CommandableComponent>())
		{
			// No gender check for police
		}
		else
		{
			// Only male persons
			if (victim.getComponent<PersonComponent>()->getGender() != PersonComponent::GENDER_MALE)
				return false;
		}

		// Accepted
		return true;
	}

	qsf::Entity* RowdyEvent::findNewPoliceTarget(qsf::Entity& gangster) const
	{
		float closestDistance = std::numeric_limits<float>::max();
		qsf::Entity* closestEntity = nullptr;

		const auto& commandableComponentsInMap = qsf::ComponentMapQuery(getMap()).getAllInstances<CommandableComponent>();

		// Check all commandable entities
		for (CommandableComponent* commandableComponent : commandableComponentsInMap)
		{
			// Safety check
			if (nullptr == commandableComponent)
				continue;

			// Only attack police persons
			if (!commandableComponent->isPolicePerson() && !commandableComponent->isPoliceSEKPerson())
				continue;

			// Get entity
			qsf::Entity& entity = commandableComponent->getEntity();

			// Check if unit is inside vehicle or building
			if (nullptr != EntityHelper(entity).getContainerEntity())
				continue;

			// Check if unit is injured
			if (EntityHelper(entity).isPersonInjured())
				continue;

			// Get the distance
			const float currentDistance = qsf::game::DistanceHelper::getDistance(gangster, entity);
			if (currentDistance < closestDistance)
			{
				closestDistance = currentDistance;
				closestEntity = &entity;
			}
		}

		return closestEntity;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
