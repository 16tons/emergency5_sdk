// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/gangster/RiotEvent.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/base/CloseQuartersCombatAction.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/gangster/ReachVictimAction.h"
#include "em5/action/move/escape/EscapeFootShortAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/freeplay/objective/ObjectiveHelper.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/game/targetpoint/AttackPersonTargetPointProvider.h"
#include "em5/health/HealthComponent.h"
#include "em5/logic/HintHelper.h"
#include "em5/logic/ObserverHelper.h"
#include "em5/logic/local/gangsters/GangsterBaseLogic.h"
#include "em5/logic/observer/GangsterEscapedObserver.h"
#include "em5/logic/observer/PersonDiedObserver.h"
#include "em5/logic/observer/TransportedToHqObserver.h"
#include "em5/logic/observer/hint/HintShotAtObserver.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/specs/HealthSpecsGroup.h"

#include <qsf_game/base/DistanceHelper.h>

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/audio/component/DynamicMusicCompositorComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/action/WaitAction.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/map/Map.h>
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
	const uint32 RiotEvent::FREEPLAY_EVENT_ID = qsf::StringHash("em5::RiotEvent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	RiotEvent::RiotEvent() :
		mBuildingId(qsf::getUninitialized<uint64>()),
		mAttackersLeft(qsf::getUninitialized<uint32>()),
		mStartGangsters(qsf::getUninitialized<uint32>()),
		mGangstersRemainingBeforeTheyFlee(0),
		mAreGangstersFleeing(false),
		mDontShootTriggered(false),
		mVigilanteTriggered(false),
		mPreferToAttackSquad(true),
		mRemainingGangstersFleeWhenOthersArrested(false)
	{
		// Nothing here
	}

	RiotEvent::~RiotEvent()
	{
		// Nothing here
	}

	uint64 RiotEvent::getBuildingId() const
	{
		return mBuildingId;
	}

	void RiotEvent::setBuilding(const qsf::Entity& entity)
	{
		mBuildingId = entity.getId();
	}

	void RiotEvent::setPeopleToAttack(uint32 peopleToAttack)
	{
		mAttackersLeft = peopleToAttack;
	}

	void RiotEvent::setStartGangsters(uint32 startGangsters)
	{
		mStartGangsters = startGangsters;
	}

	void RiotEvent::setHintToTriggerWhenFleeing(const std::string& hintId)
	{
		mHintToTriggerWhenFleeing = hintId;
	}

	void RiotEvent::enableGangsterFleeingBehavior(uint32 remainingGangsters)
	{
		mRemainingGangstersFleeWhenOthersArrested = true;
		mGangstersRemainingBeforeTheyFlee = remainingGangsters;
	}

	void RiotEvent::setPreferToAttackSquad(bool gangstersPreferToAttackSquad)
	{
		mPreferToAttackSquad = gangstersPreferToAttackSquad;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	bool RiotEvent::onStartup()
	{
		qsf::Entity* buildingEntity = getMap().getEntityById(mBuildingId);
		QSF_CHECK(buildingEntity, "Can't find building entity; RiotEvent method: onStartup", QSF_REACT_NONE);
		if (nullptr == buildingEntity)
		{
			return false;
		}

		const uint32 initialCount = mStartGangsters;
		for (uint32 i = 0; i < initialCount; ++i)
		{
			qsf::Entity* gangsterEntity = createRiotGangster();
			if (nullptr == gangsterEntity)
			{
				return false;
			}

			mGangsterIds.push_back(gangsterEntity->getId());
			ObserverHelper(*this).createObserver<HintShotAtObserver>(gangsterEntity->getId()).initialize();

			qsf::TransformComponent& transformComponent = gangsterEntity->getOrCreateComponentSafe<qsf::TransformComponent>();
			BuildingComponent* component = buildingEntity->getComponent<BuildingComponent>();

			glm::vec3 pos = component->getDoorPosition();
			glm::vec3 offset = component->getDoorRotation() * glm::vec3(i < 2 ? -0.5f : 0.5f, 0, 2 + (i % 2));
			glm::vec3 toCenter = buildingEntity->getOrCreateComponent<qsf::TransformComponent>()->getPosition() - pos;
			if (glm::dot(offset, toCenter) > 0)
			{
				offset = -offset;
			}

			transformComponent.setPosition(pos + offset);

			qsf::ActionComponent& actionComponent = gangsterEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
			actionComponent.clearPlan();
			EventIdComponent::addToEvent(*gangsterEntity, *this, eventspreadreason::GANGSTER);
			EntityHelper(*gangsterEntity).turnIntoGangster(qsf::StringHash("em5::GangsterBaseLogic"), false);

			PersonComponent& personComponent = gangsterEntity->getOrCreateComponentSafe<PersonComponent>();
			personComponent.setPersonFlags(PersonComponent::FLAG_DRUNK, true);

			AnimationHelper animationHelper(*gangsterEntity);
			AnimationHelper::AnimationMaskData animationMaskData;
			gangsterEntity->getOrCreateComponent<qsf::MeshAnimationComponent>()->playAnimation(qsf::AssetProxy(animationHelper.getIdleAnimation(animationMaskData)), false);

			static_cast<qsf::WaitAction*>(actionComponent.pushActionById(qsf::WaitAction::ACTION_ID))->init(qsf::Time::fromSeconds(qsf::Random::getRandomFloat(0.0f, 3.0f)));
		}

		// We have to add some headroom for the initial fights
		const uint32 fights = initialCount / 2 + (initialCount % 2 != 0 ? 1 : 0);
		mAttackersLeft += fights;

		// Event is now running
		setRunning();

		// Done
		return true;
	}

	void RiotEvent::onShutdown()
	{
		// Nothing to do here
	}

	void RiotEvent::onRun()
	{
		// Show supervisor message
		showSupervisorMessage(HintHelper::getRandomStringOfIdString(HintHelper(*this).getHintParameters().mSupervisorMessageIds));
	}

	bool RiotEvent::onFailure(EventResult& eventResult)
	{
		const uint32 failObjectiveId = getFailReasonObjectiveTypeId(eventResult.mObjectives);

		if (ObjectiveHelper::OBJECTIVE_FAIL_PERSONSESCAPE == failObjectiveId)
		{
			// Show hint killer escaped
			showHint("EM5_EVENT_GANGSTER_ESCAPING_FAIL_HINT_02", false);
		}
		else if (ObjectiveHelper::OBJECTIVE_FAIL_DEADPERSONS == failObjectiveId)
		{
			// Show hint person died
			showHint(HintHelper::getRandomStringOfIdString("EM5_EVENT_TOO_MANY_PERSONS_DIED_HINT_03, EM5_EVENT_TOO_MANY_PERSONS_DIED_HINT_04"), false);
		}

		// Call base implementation
		return FreeplayEvent::onFailure(eventResult);
	}

	void RiotEvent::updateFreeplayEvent(const qsf::Time& timePassed)
	{
		// Check objectives for success or failure
		checkObjectivesState();
		if (getState() != State::STATE_RUNNING)
		{
			return;
		}

		Objective* arrestObjective = ObjectiveHelper(*this).getObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_ARRESTPERSONS);

		for (size_t i = 0; i < mGangsterIds.size(); ++i)
		{
			qsf::Entity* gangsterEntity = getMap().getEntityById(mGangsterIds[i]);
			if (nullptr == gangsterEntity)
			{
				const uint32 gangstersLeft = arrestObjective->getNeededNumber() - arrestObjective->getCurrentNumber();
				if (gangstersLeft == mGangsterIds.size())
				{
					arrestObjective->setNeededNumber(arrestObjective->getNeededNumber() - 1);
				}
				mGangsterIds.erase(mGangsterIds.begin() + i);
				--i;
				continue;
			}

			if (EntityHelper(*gangsterEntity).isArrested())
			{
				gangsterEntity->getOrCreateComponent<qsf::ActionComponent>()->clearPlan();
				mGangsterIds.erase(mGangsterIds.begin() + i);
				--i;
				continue;
			}

			// Fleeing gangsters mean we no longer have to manage the fighting, only the objectives
			if (mAreGangstersFleeing)
			{
				continue;
			}

			qsf::ActionComponent* gangsterActionComponent = gangsterEntity->getOrCreateComponent<qsf::ActionComponent>();
			if (nullptr == gangsterActionComponent->getCurrentAction() && !isAttacked(*gangsterEntity) && !isAttackingSquad(*gangsterEntity) && mAttackersLeft > 0)
			{
				if (mPreferToAttackSquad)
				{
					if (tryAttackSquad(*gangsterEntity))
					{
						continue;
					}
				}

				if (tryAttackGangster(*gangsterEntity))
				{
					continue;
				}

				qsf::Entity* vigilanteEntity = findNewTarget(*gangsterEntity);
				if (nullptr == vigilanteEntity)
				{
					if (!mPreferToAttackSquad)
					{
						tryAttackSquad(*gangsterEntity);
					}

					continue;
				}

				vigilanteAttackTarget(*vigilanteEntity, *gangsterEntity);
				ObserverHelper(*this).createObserver<HintShotAtObserver>(vigilanteEntity->getId()).initialize();
				continue;
			}
		}

		// check if we should trigger the fleeing now
		if (mRemainingGangstersFleeWhenOthersArrested && !mAreGangstersFleeing)
		{
			const uint32 gangstersLeft = static_cast<uint32>(mGangsterIds.size());

			if (gangstersLeft <= mGangstersRemainingBeforeTheyFlee)
			{
				instructGangstersToFlee();
			}
		}

		checkObjectivesState();
	}

	void RiotEvent::hintCallback(Observer& hintObserver)
	{
		if (mDontShootTriggered)
		{
			return;
		}

		mDontShootTriggered = true;

		showHint("EM5_EVENT_DONT_SHOOT_HINT_02", false);
	}

	const qsf::Entity* RiotEvent::getFocusEntity()
	{
		if (!mGangsterIds.empty())
		{
			qsf::Entity* gangster = getMap().getEntityById(mGangsterIds[0]);
			if (nullptr != gangster)
			{
				return gangster;
			}
		}

		EntityIdArray const& victims = getEntityIds();

		for (int i = 0; i < int(victims.size()); ++i)
		{
			qsf::Entity* victim = getMap().getEntityById(victims.at(i));
			if (nullptr != victim)
			{
				return victim;
			}
		}
		return nullptr;
	}

	bool RiotEvent::checkEventConfiguration()
	{
		qsf::Entity* buildingEntity = getMap().getEntityById(mBuildingId);
		return (nullptr != buildingEntity);
	}

	void RiotEvent::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base implementation
		FreeplayEvent::serialize(serializer);

		serializer.serialize(mAttackersLeft);
		serializer.serialize(mGangsterIds);
		serializer.serialize(mBuildingId);
		serializer.serialize(mPreferToAttackSquad);
		serializer.serialize(mRemainingGangstersFleeWhenOthersArrested);
		serializer.serialize(mGangstersRemainingBeforeTheyFlee);
		serializer.serialize(mAreGangstersFleeing);
		serializer.serialize(mHintToTriggerWhenFleeing);
	}

	bool RiotEvent::addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason)
	{
		switch (eventSpreadReason)
		{
			case eventspreadreason::INJURY:
			{
				for (size_t i = 0; i < mGangsterIds.size(); ++i)
				{
					if (targetEntity.getId() == mGangsterIds[i])
					{
						if (mCurrentFights.count(mGangsterIds[i]) > 0)
						{
							auto it = mCurrentFights.find(mGangsterIds[i]);
							mCurrentFights.erase(it);
						}
						else
						{
							auto it = mCurrentFights.begin();
							while (it != mCurrentFights.end())
							{
								if (it->second == mGangsterIds[i])
								{
									mCurrentFights.erase(it);
									break;
								}
								++it;
							}
						}

						Objective* arrestObjective = ObjectiveHelper(*this).getObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_ARRESTPERSONS);
						arrestObjective->setNeededNumber(arrestObjective->getNeededNumber() - 1);
						removeObserver(*mArrestObservers[mGangsterIds[i]]);
						mArrestObservers.erase(mGangsterIds[i]);
						mGangsterIds.erase(mGangsterIds.begin() + i);
						--i;
						break;
					}
				}
				break;
			}

			case eventspreadreason::GANGSTER:
			{
				ObjectiveHelper objectiveHelper(*this);
				ObserverHelper observerHelper(*this);

				// Get or create conditions
				Objective& failConditionPersonDiedObjective = objectiveHelper.getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_FAIL_DEADPERSONS);
				failConditionPersonDiedObjective.setNeededNumber(1);

				Objective& failConditionPersonEscapedObjective = objectiveHelper.getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_FAIL_PERSONSESCAPE);
				failConditionPersonEscapedObjective.setNeededNumber(1);

				Objective& needConditionArrestPersonObjective = objectiveHelper.getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_ARRESTPERSONS);
				needConditionArrestPersonObjective.increaseNeededNumber();

				// Create and connect observers
				PersonDiedObserver& observerDied = observerHelper.createObserver<PersonDiedObserver>(targetEntity.getId());
				observerDied.connectToObjective(failConditionPersonDiedObjective);

				TransportedToHqObserver& observerTransported = observerHelper.createObserver<TransportedToHqObserver>(targetEntity.getId());
				observerTransported.connectToObjective(needConditionArrestPersonObjective);
				mArrestObservers[targetEntity.getId()] = &observerTransported;

				GangsterEscapedObserver& observerEscaped = observerHelper.createObserver<GangsterEscapedObserver>(targetEntity.getId());
				observerEscaped.connectToObjective(failConditionPersonEscapedObjective);
				return true;
			}

			default:
				// Nothing to do in this case
				break;
		}

		return FreeplayEvent::addEntityToEvent(targetEntity, eventSpreadReason, newReason);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	bool RiotEvent::checkIfReachedVictim(qsf::Entity& gangster, qsf::Entity& victim)
	{
		qsf::ActionComponent& actionComponent = gangster.getOrCreateComponentSafe<qsf::ActionComponent>();
		return actionComponent.getCurrentAction()->getTypeId() != MoveAction::ACTION_ID;
	}

	bool RiotEvent::checkIfIsHealthy(qsf::Entity& personEntity)
	{
		return !personEntity.getComponent<HealthComponent>()->isInjured();
	}

	qsf::Entity* RiotEvent::findNewTarget(qsf::Entity& gangster)
	{
		// Get a listing of all person components (cannot be empty since findCandidate already checked)
		const auto& personComponentsInMap = qsf::ComponentMapQuery(getMap()).getAllInstances<PersonComponent>();

		// Try to find a victim...

		// Check nearby targets
		for (PersonComponent* personComponent : personComponentsInMap)
		{
			// Check if entity is the gangster
			qsf::Entity& entity = personComponent->getEntity();
			if (&entity != &gangster && checkVigilante(gangster, entity, true))
			{
				return &entity;
			}
		}

		return nullptr;
	}

	void RiotEvent::attackTarget(qsf::Entity& gangsterEntity, qsf::Entity& victimEntity)
	{
		AnimationHelper helperGangster(gangsterEntity);
		helperGangster.clearAnimation();
		AnimationHelper helperVictim(victimEntity);
		helperVictim.clearAnimation();

		GangsterBaseLogic* gangsterBaseLogic = gangsterEntity.getOrCreateComponent<qsf::GameLogicComponent>()->getGameLogic<GangsterBaseLogic>();
		if (nullptr != gangsterBaseLogic && GangsterBaseLogic::STATE_ARRESTED == gangsterBaseLogic->getCurrentState())
		{
			return;
		}

		EntityHelper(gangsterEntity).turnIntoGangster(qsf::StringHash("em5::GangsterBaseLogic"), false);

		qsf::ActionComponent& gangsterActionComponent = gangsterEntity.getOrCreateComponentSafe<qsf::ActionComponent>();
		gangsterActionComponent.clearPlan();

		// Move to target
		qsf::ai::ReachObjectGoal* reachObjectGoal = new qsf::ai::ReachObjectGoal(gangsterEntity, victimEntity, AttackPersonTargetPointProvider::TARGET_POINT_ID);
		gangsterActionComponent.pushAction<MoveAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(reachObjectGoal);

		// Attack
		gangsterActionComponent.pushAction<ReachVictimAction>(action::COMMAND_HIGH, qsf::action::APPEND_TO_BACK).init(victimEntity);

		const em5::HealthSpecsGroup& specsGroup = HealthSpecsGroup::getInstanceSafe();

		float lowerTargetLife = specsGroup.getCQCGangsterGangsterAttackerDamage();
		float lowerAttackerLife = specsGroup.getCQCGangsterGangsterVictimDamage();

		EntityHelper entityHelperVictim(victimEntity);

		if (entityHelperVictim.isSquadPerson())
		{
			lowerTargetLife = specsGroup.getCQCGangsterPoliceAttackerDamage();
			lowerAttackerLife = specsGroup.getCQCGangsterPoliceVictimDamage();
		}

		gangsterActionComponent.pushAction<CloseQuartersCombatAction>(action::COMMAND_HIGH, qsf::action::APPEND_TO_BACK).init(victimEntity, weapon::FIST, weapon::FIST, 0, lowerTargetLife, lowerAttackerLife);

		mCurrentFights[gangsterEntity.getId()] = victimEntity.getId();
	}

	void RiotEvent::vigilanteAttackTarget(qsf::Entity& gangster, qsf::Entity& victim)
	{
		if (!mVigilanteTriggered)
		{
			// Deactivated as requested by Game Design (should be reactivated for the patch again?)
			//showHint("EM5_EVENT_RIOT_HINT_01", false);
			mVigilanteTriggered = true;
		}

		attackTarget(gangster, victim);
		gangster.getOrCreateComponent<EventIdComponent>()->setEvent(*this, eventspreadreason::GANGSTER);
		mGangsterIds.push_back(gangster.getId());
		--mAttackersLeft;
	}

	bool RiotEvent::checkVigilante(qsf::Entity& gangster, qsf::Entity& victim, bool onlyCheckClose)
	{
		EntityHelper entityHelper(victim);

		// Make the usual victim tests
		if (!entityHelper.isValidGangsterEventTarget(gangster, true))
		{
			return false;
		}

		PersonComponent* personComponent = victim.getComponent<PersonComponent>();
		if (nullptr == personComponent)
		{
			return false; // Something absolutely went wrong here
		}

		// No seniors (We already checked for children in the call above)
		if (personComponent->getAge() == PersonComponent::AGE_SENIOR)
		{
			return false;
		}

		// Male only
		if (personComponent->getGender() != PersonComponent::GENDER_MALE)
		{
			return false;
		}

		// Check if victim is reasonably close to gangster
		if (onlyCheckClose && qsf::game::DistanceHelper::getDistance(gangster, victim) > 50.f)
		{
			return false;
		}

		// Accepted
		return true;
	}

	std::vector<uint64> const& RiotEvent::getGangsterIds() const
	{
		return mGangsterIds;
	}

	void RiotEvent::addGangster(const qsf::Entity& entity)
	{
		mGangsterIds.push_back(entity.getId());
	}

	bool RiotEvent::isAttacked(qsf::Entity& gangster)
	{
		auto it = mCurrentFights.begin();
		while (it != mCurrentFights.end())
		{
			if (it->second == gangster.getId())
			{
				return true;
			}

			++it;
		}
		return false;
	}

	bool RiotEvent::isFighting(qsf::Entity& gangster)
	{
		const uint64 gangsterId = gangster.getId();

		auto it = mCurrentFights.begin();
		while (it != mCurrentFights.end())
		{
			if (it->second == gangsterId || it->first == gangsterId)
			{
				return true;
			}

			++it;
		}
		return false;
	}

	bool RiotEvent::isAttackingSquad(qsf::Entity& gangsterEntity)
	{
		for (uint64 gangsterId : mAttackingSquad)
		{
			if (gangsterId == gangsterEntity.getId())
			{
				return true;
			}
		}
		return false;
	}

	bool RiotEvent::tryAttackSquad(qsf::Entity& gangsterEntity)
	{
		qsf::ComponentMapQuery query(gangsterEntity.getMap());
		std::vector<CommandableComponent*> components;
		query.getInstancesInCircle<CommandableComponent>(gangsterEntity.getOrCreateComponent<qsf::TransformComponent>()->getPosition(), 50.f, components);
		for (CommandableComponent* component : components)
		{
			qsf::StringHash unitType(component->getUnitType());

			if (unitType != CommandableComponent::UNITTAG_POLICE_POLICE)
			{
				continue;
			}

			qsf::Entity& squadEntity = component->getEntity();

			attackTarget(gangsterEntity, squadEntity);
			return true;
		}
		return false;
	}

	bool RiotEvent::tryAttackGangster(qsf::Entity& gangsterEntity)
	{
		qsf::ComponentMapQuery query(gangsterEntity.getMap());
		std::vector<EventIdComponent*> components;
		query.getInstancesInCircle<EventIdComponent>(gangsterEntity.getOrCreateComponent<qsf::TransformComponent>()->getPosition(), 50.f, components);
		for (EventIdComponent* component : components)
		{
			if (component->getEventId() != getId())
			{
				continue;
			}

			// Gangsters don't attack themselves
			if (component->getEntityId() == gangsterEntity.getId())
			{
				continue;
			}

			qsf::Entity& gangsterEntity2 = component->getEntity();

			if (isFighting(gangsterEntity2))
			{
				continue;
			}

			EntityHelper entityHelper(gangsterEntity2);

			// Don't attack injured gangsters
			if (entityHelper.isPersonInjured())
			{
				continue;
			}

			// Don't attack gangsters which are not idle
			if (!gangsterEntity2.getOrCreateComponentSafe<qsf::ActionComponent>().getPlan().isEmpty())
			{
				continue;
			}

			attackTarget(gangsterEntity, gangsterEntity2);
			return true;
		}
		return false;
	}

	qsf::Entity* RiotEvent::createRiotGangster()
	{
		// TODO(mk) Use the same gangster pool for all gangster events, and use in assets defined spawnpool
		// Create a pool to choose from (this is done only once!)
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

		// Create an entity from that prefab and return it
		return MapHelper(getMap()).createObjectByLocalPrefabAssetId(qsf::StringHash(randomPrefabName));
	}

	void RiotEvent::instructGangstersToFlee()
	{
		if (mAreGangstersFleeing)
		{
			return;
		}

		for (uint64 gangsterId : mGangsterIds)
		{
			qsf::Entity* const gangsterEntity = getMap().getEntityById(gangsterId);

			qsf::ActionComponent* const gangsterActionComponent = gangsterEntity->getOrCreateComponent<qsf::ActionComponent>();

			gangsterActionComponent->clearPlan();
			static_cast<EscapeFootShortAction*>(gangsterActionComponent->pushActionById(EscapeFootShortAction::ACTION_ID))->init("");
		}

		mAreGangstersFleeing = true;

		if (!mHintToTriggerWhenFleeing.empty())
		{
			showHint(mHintToTriggerWhenFleeing, false);
			getDynamicMusicCompositor().setMusicLevel(1);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
