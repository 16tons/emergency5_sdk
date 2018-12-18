// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/gangster/TerrorAttackEvent.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/base/ExitBuildingAction.h"
#include "em5/action/gangster/ReachVictimAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/component/objects/BombComponent.h"
#include "em5/component/vehicle/VehicleComponent.h"
#include "em5/freeplay/objective/ObjectiveHelper.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/health/HealthComponent.h"
#include "em5/logic/local/gangsters/GangsterBaseLogic.h"
#include "em5/logic/ObserverHelper.h"
#include "em5/logic/observer/PersonDiedObserver.h"
#include "em5/logic/observer/hint/HintShotAtObserver.h"
#include "em5/logic/observer/hint/HintMessageGeneralObserver.h"
#include "em5/logic/HintHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/plugin/Messages.h"

#include <qsf_game/base/DistanceHelper.h>

#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>

#include <qsf/audio/component/DynamicMusicCompositorComponent.h>
#include <qsf/link/link/prototype/ContainerLink.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/log/LogSystem.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/math/Random.h>
#include <qsf/renderer/mesh/MeshComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 TerrorAttackEvent::FREEPLAY_EVENT_ID = qsf::StringHash("em5::TerrorAttackEvent");

	const std::string HINT01 = "EM5_EVENT_TERRORATTACK_HINT_01";
	const std::string HINT02 = "EM5_EVENT_TERRORATTACK_HINT_02";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	TerrorAttackEvent::TerrorAttackEvent() :
		mBombTime(qsf::Time::fromSeconds(30.0f)),
		mBombsPlaced(0),
		mBombDeletionCountdown(qsf::Time::fromSeconds(10.0)) // Delete bomb prefab 10 seconds after detonation (when the explosion is 'finished')
	{
		// Nothing here
	}

	TerrorAttackEvent::~TerrorAttackEvent()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	bool TerrorAttackEvent::onStartup()
	{
		for (Terrorist& terrorist: mTerrorists)
		{
			qsf::Entity* gangsterEntity = getMap().getEntityById(terrorist.mGangsterId);
			QSF_CHECK(gangsterEntity, "Can't find gangster entity; TerrorAttackEvent method: onStartup", QSF_REACT_NONE);
			if (nullptr == gangsterEntity)
				return false;

			qsf::Entity* bombSpotEntity = getMap().getEntityById(terrorist.mBombSpotId);
			QSF_CHECK(bombSpotEntity, "Can't find bomb spot entity; TerrorAttackEvent method: onStartup", QSF_REACT_NONE);
			if (nullptr == bombSpotEntity)
				return false;

			QSF_LOG_PRINTS(DEBUG, "Terrorist " << EntityHelper(*gangsterEntity).getName() << " is running to " << EntityHelper(*bombSpotEntity).getName());
			QSF_LOG_PRINTS(DEBUG, "Terrorist at " << EntityHelper(*gangsterEntity).getPosition() << " is running to " << EntityHelper(*bombSpotEntity).getPosition());

			EventIdComponent::addToEvent(*gangsterEntity, *this, eventspreadreason::GANGSTER);

			// Order gangster to run to bomb spot
			qsf::ActionComponent& actionComponent = gangsterEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
			actionComponent.clearPlan();
			MoveAction* moveAction = &actionComponent.pushAction<MoveAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT);
			qsf::TransformComponent* transformComponent = bombSpotEntity->getComponent<qsf::TransformComponent>();
			QSF_CHECK(transformComponent, "Bomb spot has no transform component; TerrorAttackEvent method: onStartup", return false);
			const glm::vec3& targetPosition = transformComponent->getPosition();
			qsf::ai::ReachSinglePointGoal* goal = new qsf::ai::ReachSinglePointGoal(qsf::logic::TargetPoint(targetPosition, false, 1.0f, 5.0f));
			moveAction->init(goal);
			ReachVictimAction* reachVictimAction = &actionComponent.pushAction<ReachVictimAction>(action::COMMAND_HIGH, qsf::action::APPEND_TO_BACK);
			reachVictimAction->init(*bombSpotEntity);

			terrorist.mState = RUNNING_TO_BOMB_SPOT;
		}
		setRunning();

		// Done
		return true;
	}

	void TerrorAttackEvent::onShutdown()
	{
		for (Terrorist terrorist : mTerrorists)
		{
			// Just to be sure, destroy bomb prefab if it still exists
			if (qsf::isInitialized(terrorist.mBombId))
			{
				MapHelper(getMap()).destroyEntityById(terrorist.mBombId);
			}
		}

		mMessageProxy.unregister();
	}

	void TerrorAttackEvent::onRun()
	{
		// Activate the hint observers
		startHintObservers();
		showSupervisorMessage("EM5_EVENT_SUPERV_OBJ_TERRORATTACK_01");
		for (Terrorist & terrorist : mTerrorists)
		{
			qsf::Entity* gangsterEntity = getMap().getEntityById(terrorist.mGangsterId);
			PersonComponent* personComponent = gangsterEntity->getOrCreateComponent<PersonComponent>();
			personComponent->setGangsterFlag(true);
		}
	}

	bool TerrorAttackEvent::onFailure(EventResult& eventResult)
	{
		const int i = qsf::Random::getRandomUint(0, 1);
		showHint((i == 0) ? "EM5_EVENT_TOO_MANY_PERSONS_DIED_HINT_01" : "EM5_EVENT_TOO_MANY_PERSONS_DIED_HINT_02", false);

		// Call base implementation
		return FreeplayEvent::onFailure(eventResult);
	}

	void TerrorAttackEvent::updateFreeplayEvent(const qsf::Time& timePassed)
	{
		// Check objectives for success or failure
		checkObjectivesState();
		if (getState() != State::STATE_RUNNING && getState() != State::STATE_HIDDEN)
			return;
		for (Terrorist& terrorist : mTerrorists)
		{
			qsf::Entity* gangsterEntity = getMap().getEntityById(terrorist.mGangsterId);
			qsf::Entity* bombSpotEntity = getMap().getEntityById(terrorist.mBombSpotId);

			// Bomb already exploded?
			if (terrorist.mBombExploded)
			{
				// Decrement timer
				mBombDeletionCountdown -= timePassed;

				// Destroy bomb entity
				if (mBombDeletionCountdown.getSeconds() <= 0.0f && qsf::isInitialized(terrorist.mBombId))
				{
					MapHelper(getMap()).destroyEntityById(terrorist.mBombId);
				}
			}

			if (nullptr != gangsterEntity && EntityHelper(*gangsterEntity).isArrested())
				continue;

			switch (terrorist.mState)
			{
				case RUNNING_TO_BOMB_SPOT:
					QSF_CHECK(gangsterEntity, "Can't find gangster entity; TerrorAttackEvent method: updateFreeplayEvent", removeTerrorist(terrorist); continue);
					QSF_CHECK(bombSpotEntity, "Can't find bomb spot entity; TerrorAttackEvent method: updateFreeplayEvent", removeTerrorist(terrorist); continue);
					if (!checkIfIsHealthy(*gangsterEntity))
					{
						removeTerrorist(terrorist);
						continue;
					}

					if (checkIfReachedBombSpot(*gangsterEntity, *bombSpotEntity))
					{
						if (qsf::game::DistanceHelper::get2dDistance(*gangsterEntity, *bombSpotEntity) > 5.0f)
						{
							QSF_WARN("Gangster cannot reach the bomb spot, possibly due to being stuck somewhere. Event will be aborted.", QSF_REACT_NONE);
							removeTerrorist(terrorist);
							continue;
						}

						qsf::ActionComponent& actionComponent = gangsterEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
						AnimationHelper animationHelper(*gangsterEntity);
						actionComponent.pushAction<PlayAnimationAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(animationHelper.getAnimationManipulateObjectKneeling(), true);
						actionComponent.pushAction<TurnToAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(terrorist.mBombSpotId);
						terrorist.mState = PERPETRATING;
					}
					break;

				case PERPETRATING:
				{
					QSF_CHECK(gangsterEntity, "Can't find gangster entity; TerrorAttackEvent method: updateFreeplayEvent", removeTerrorist(terrorist); continue);
					qsf::ActionComponent& actionComponent = gangsterEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
					if (nullptr == actionComponent.getAction<PlayAnimationAction>())
					{
						setupGangster(terrorist);
						++mBombsPlaced;
						if (mBombsPlaced == mTerrorists.size())
						{
							showHint(HINT01, true);
						}
						terrorist.mState = RUNNING_AMOK;
					}
					break;
				}

				case RUNNING_AMOK:
					break;

				case RESCUE:
					break;
			}

			if (terrorist.mState != RESCUE && nullptr != gangsterEntity && !checkIfIsHealthy(*gangsterEntity))
			{
				ObjectiveHelper(*this).removeObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_ARRESTPERSONS);
				terrorist.mState = RESCUE;
			}
		}
	}

	const qsf::Entity* TerrorAttackEvent::getFocusEntity()
	{
		std::vector<qsf::Entity*> terroristNotInAmokState;
		for (Terrorist& terrorist : mTerrorists)
		{
			if (terrorist.mState == RUNNING_TO_BOMB_SPOT || terrorist.mState == PERPETRATING)
			{
				qsf::Entity* gangster = getMap().getEntityById(terrorist.mGangsterId);
				if (gangster != nullptr)
				{
					terroristNotInAmokState.push_back(gangster);
				}
			}

		}
		if (!terroristNotInAmokState.empty())
		{
			return qsf::Random::getAnyOf(terroristNotInAmokState);
		}
		std::vector<qsf::Entity*> bombs;
		for (Terrorist& terrorist : mTerrorists)
		{
			if (terrorist.mState != RUNNING_TO_BOMB_SPOT && terrorist.mState != PERPETRATING && !terrorist.mBombExploded)
			{
				qsf::Entity* bombEntity = getMap().getEntityById(terrorist.mBombId);
				if (nullptr != bombEntity && bombEntity->getComponent<BombComponent>()->canBeDefused())
				{
					bombs.push_back(bombEntity);
				}
			}
		}

		return bombs.empty() ? getMap().getEntityById(qsf::Random::getAnyOf(getEntityIds())) : qsf::Random::getAnyOf(bombs);
	}

	bool TerrorAttackEvent::checkEventConfiguration()
	{
		for (Terrorist bomber : mTerrorists)
		{
			qsf::Entity* gangsterEntity = getMap().getEntityById(bomber.mGangsterId);
			if (nullptr == gangsterEntity)
			{
				return false;
			}
		}
		return true;
	}

	void TerrorAttackEvent::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base implementation
		FreeplayEvent::serialize(serializer);

		serializer.serialize(mBombTime);
		serializer.serialize(mBombsPlaced);
		auto terroristCount = mTerrorists.size();
		serializer.serialize(terroristCount);
		if (serializer.isReading())
		{
			for (uint32 i = 0; i < terroristCount; ++i)
			{
				Terrorist terrorist;
				serializer.serialize(terrorist.mBombExploded);
				serializer.serialize(terrorist.mBombId);
				serializer.serialize(terrorist.mBombSpotId);
				serializer.serialize(terrorist.mGangsterId);
				serializer.serializeAs<uint16>(terrorist.mState);
				mTerrorists.push_back(terrorist);
			}
		}
		else
		{
			for (Terrorist & terrorist : mTerrorists)
			{
				serializer.serialize(terrorist.mBombExploded);
				serializer.serialize(terrorist.mBombId);
				serializer.serialize(terrorist.mBombSpotId);
				serializer.serialize(terrorist.mGangsterId);
				serializer.serializeAs<uint16>(terrorist.mState);
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void TerrorAttackEvent::startHintObservers()
	{
		for (Terrorist terrorist : mTerrorists)
		{
			qsf::Entity* gangsterEntity = getMap().getEntityById(terrorist.mGangsterId);
			HintHelper(*this).createAndInitializeHintsAtLowLife(*gangsterEntity);
			ObserverHelper(*this).createObserver<HintShotAtObserver>(terrorist.mGangsterId).initialize();
		}
		createGeneralObserver<HintMessageGeneralObserver>(HINT02).initialize(
			qsf::MessageConfiguration(Messages::EM5_COMPONENT_GETSINJURED_PERSON));
	}

	void TerrorAttackEvent::setupGangster(Terrorist& terrorist)
	{
		qsf::Entity* gangster = getMap().getEntityById(terrorist.mGangsterId);
		qsf::Entity* bombSpot = getMap().getEntityById(terrorist.mBombSpotId);

		gangster->getOrCreateComponentSafe<qsf::ActionComponent>().clearPlan();
		EntityHelper(*gangster).turnIntoGangster(qsf::StringHash("em5::GangsterKillerPsychoLogic"), false);
		qsf::Entity* bombEntity = getMap().createObjectByLocalPrefabAssetId(qsf::StringHash("em5/prefab/default/timebomb"));
		terrorist.mBombId = bombEntity->getId();

		qsf::TransformComponent& transformComponent = bombEntity->getOrCreateComponentSafe<qsf::TransformComponent>();
		transformComponent.setPosition(bombSpot->getOrCreateComponent<qsf::TransformComponent>()->getPosition());

		BombComponent& bombComponent = bombEntity->getOrCreateComponentSafe<BombComponent>();
		bombComponent.setTimeRunning(mBombTime);
		bombComponent.setTimeToDefuse(qsf::Time::fromSeconds(10.0f)); //TODO(mv) json?
		bombComponent.startBombTimer(*this);

		mMessageProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_ENTITY_BOMB_EXPLODED, terrorist.mBombId), boost::bind(&TerrorAttackEvent::onExplode, this, _1, terrorist));

		EventIdComponent::addToEvent(*bombEntity, *this, eventspreadreason::GANGSTER);
		gangster->getOrCreateComponentSafe<qsf::ActionComponent>().clearPlan();
	}

	bool TerrorAttackEvent::checkIfIsHealthy(const qsf::Entity& personEntity) const
	{
		return !personEntity.getComponent<HealthComponent>()->isInjured();
	}

	bool TerrorAttackEvent::checkIfReachedBombSpot(qsf::Entity& gangster, qsf::Entity& victim) const
	{
		qsf::ActionComponent& actionComponent = gangster.getOrCreateComponentSafe<qsf::ActionComponent>();
		if (actionComponent.getPlan().isEmpty())
			return true;

		return (actionComponent.getCurrentAction()->getTypeId() != MoveAction::ACTION_ID);
	}

	void TerrorAttackEvent::onExplode(const qsf::MessageParameters& parameters, Terrorist& terrorist)
	{
		qsf::Entity* bombEntity = getMap().getEntityById(terrorist.mBombId);
		qsf::ComponentMapQuery componentMapQuery(bombEntity->getMap());
		glm::vec3 bombPosition = bombEntity->getOrCreateComponent<qsf::TransformComponent>()->getPosition();

		{ // Injure all people in range
			std::vector<PersonComponent*> personComponents;
			componentMapQuery.getInstancesInCircle<PersonComponent>(bombPosition, 20.0f, personComponents);

			for (PersonComponent* personComponent : personComponents)
			{
				qsf::Entity& personEntity = personComponent->getEntity();
				EntityHelper(personEntity).startFire(this);
				personEntity.getOrCreateComponentSafe<EventIdComponent>().setEvent(*this, eventspreadreason::INJURY);
			}
		}

		{ // Damage all vehicles in range
			std::vector<VehicleComponent*> vehicleComponents;
			componentMapQuery.getInstancesInCircle<VehicleComponent>(bombPosition, 20.0f, vehicleComponents);

			for (VehicleComponent* vehicleComponent : vehicleComponents)
			{
				qsf::Entity& vehicleEntity = vehicleComponent->getEntity();
				EntityHelper(vehicleEntity).startFire(this);
				vehicleEntity.getOrCreateComponentSafe<EventIdComponent>().setEvent(*this, eventspreadreason::FIRE);
			}
		}

		{ // Damage all buildings in range
			std::vector<BuildingComponent*> buildingComponents;
			componentMapQuery.getInstancesInCircle<BuildingComponent>(bombPosition, 20.0f, buildingComponents);

			for (BuildingComponent* buildingComponent : buildingComponents)
			{
				qsf::Entity& buildingEntity = buildingComponent->getEntity();
				EntityHelper(buildingEntity).startFire(this);
				buildingEntity.getOrCreateComponentSafe<EventIdComponent>().setEvent(*this, eventspreadreason::FIRE);
				for (auto& iterator : buildingComponent->getLinks())
				{
					const qsf::ContainerLink& containerLink = *iterator.second;
					qsf::Entity& linkedEntity = containerLink.getTargetEntity();

					if (!EntityHelper(linkedEntity).isEntityPerson())
					{
						// An other child entity, maybe light or something else
						continue;
					}
					linkedEntity.getOrCreateComponent<qsf::ActionComponent>()->pushAction<ExitBuildingAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT);
					break;
				}
			}
		}

		terrorist.mBombExploded = true;

		// TODO(co) Dead code?
		/*
		// Hide the bomb (do not yet remove it)
		qsf::MeshComponent* meshComp = bombEntity->getComponent<qsf::MeshComponent>();
		if (nullptr != meshComp)
		{
			// TODO(ks) Hide the bomb mesh! (Setting it to hidden or inactive
			// hides the explosion particle effects as well which is an unwanted side-effect!)
		}
		*/

		ObjectiveHelper(*this).removeObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_DISPOSEBOMB);
	}

	void TerrorAttackEvent::addTerrorist(qsf::Entity& gangster, qsf::Entity& bombSpot)
	{
		Terrorist terrorist;
		terrorist.mState = RUNNING_TO_BOMB_SPOT;
		terrorist.mGangsterId = gangster.getId();
		terrorist.mBombSpotId = bombSpot.getId();
		terrorist.mBombExploded = false;
		mTerrorists.push_back(terrorist);
	}

	bool TerrorAttackEvent::addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason)
	{
		switch (eventSpreadReason)
		{
			case eventspreadreason::INJURY:
			{
				const ObjectiveHelper objectiveHelper(*this);

				// Get the fail condition
				Objective& failConditionPersonDiedObjective = objectiveHelper.getObjectiveByTypeIdSafe(ObjectiveHelper::OBJECTIVE_FAIL_DEADPERSONS);
				failConditionPersonDiedObjective.setNeededNumber(2);

				// Create and connect observer
				const ObserverHelper observerHelper(*this);
				PersonDiedObserver& personDiedObserver = observerHelper.createObserver<PersonDiedObserver>(targetEntity.getId());
				personDiedObserver.connectToObjective(failConditionPersonDiedObjective);
				for (Terrorist& terrorist : mTerrorists)
				{
					if (targetEntity.getId() == terrorist.mGangsterId)
					{
						ObjectiveHelper(*this).removeObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_ARRESTPERSONS);
					}
				}
				break;
			}

			default:
				// Nothing to do in this case
				break;
		}

		return FreeplayEvent::addEntityToEvent(targetEntity, eventSpreadReason, newReason);
	}

	void TerrorAttackEvent::removeTerrorist(Terrorist& terrorist)
	{
		for (auto iter = mTerrorists.begin(); iter != mTerrorists.end(); ++iter)
		{
			if ((*iter).mGangsterId == terrorist.mGangsterId)
			{
				mTerrorists.erase(iter);
				break;
			}
		}
		if (mTerrorists.empty())
		{
			abortEvent();
		}
	}

	void TerrorAttackEvent::hintCallback(Observer& hintObserver)
	{
		if (hintObserver.getName() == HINT02)
		{
			getDynamicMusicCompositor().setMusicLevel(1);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
