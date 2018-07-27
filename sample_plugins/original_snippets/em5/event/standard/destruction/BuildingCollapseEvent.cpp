// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/destruction/BuildingCollapseEvent.h"
#include "em5/base/GameplayAssets.h"
#include "em5/command/tech/LiftUpByCraneCommand.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/component/objects/BuryComponent.h"
#include "em5/fire/component/FireComponent.h"
#include "em5/freeplay/objective/ObjectiveHelper.h"
#include "em5/game/Game.h"
#include "em5/game/GameSimulation.h"
#include "em5/game/spawn/SpawnPoolManager.h"
#include "em5/health/HealthComponent.h"
#include "em5/logic/ObserverHelper.h"
#include "em5/logic/HintHelper.h"
#include "em5/logic/observer/hint/HintMessageGeneralObserver.h"
#include "em5/logic/observer/hint/HintLowEnergyObserver.h"
#include "em5/logic/observer/generic/MessageObserver.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/EM5Helper.h"

#include <qsf_game/component/event/EventTagComponent.h>
#include <qsf_game/component/base/HiddenComponent.h>
#include <qsf_game/timer/GameTimerManager.h>

#include <qsf/audio/component/DynamicMusicCompositorComponent.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/math/Random.h>
#include <qsf/physics/collision/CollisionHelper.h>
#include <qsf/physics/collision/BulletBoxCollisionComponent.h>
#include <qsf/renderer/particles/ParticlesComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 BuildingCollapseEvent::FREEPLAY_EVENT_ID = qsf::StringHash("em5::BuildingCollapseEvent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	BuildingCollapseEvent::BuildingCollapseEvent() :
		mDamageTimerId(qsf::getUninitialized<uint32>()),
		mDebrisTimerId(qsf::getUninitialized<uint32>()),
		mTargetBuildingId(qsf::getUninitialized<uint64>()),
		mExplosionId(qsf::getUninitialized<uint64>()),
		mExplosionRadius(10.0f),
		mDamageBuildingTime(qsf::Time::fromSeconds(5.0f)),
		mDebrisSpawnTime(qsf::Time::fromSeconds(4.0f)),
		mHint01WaitTime(qsf::Time::fromSeconds(15.0f)),
		mHint2LifeEnergy(15.0f),
		mMinDebris(3),
		mMaxDebris(4),
		mSpawnPersonsUnderDebris(false),
		mExplosion(nullptr),
		mPlayerActivityObserverId(0)
	{
		// Nothing here
	}

	BuildingCollapseEvent::~BuildingCollapseEvent()
	{
		// Nothing here
	}

	void BuildingCollapseEvent::init(uint64 buildingId, const std::string& eventTagDebris, const std::string& eventTagExplosion, bool spawnPersonsUnderDebris)
	{
		mTargetBuildingId = buildingId;
		mEventTagDebris = eventTagDebris;
		mEventTagExplosion = eventTagExplosion;
		mSpawnPersonsUnderDebris = spawnPersonsUnderDebris;
	}

	float BuildingCollapseEvent::getDebrisSpawnTime() const
	{
		return mDebrisSpawnTime.getSeconds();
	}

	void BuildingCollapseEvent::setDebrisSpawnTime(float seconds)
	{
		mDebrisSpawnTime = qsf::Time::fromSeconds(seconds);
	}

	float BuildingCollapseEvent::getDamageBuildingTime() const
	{
		return mDamageBuildingTime.getSeconds();
	}

	void BuildingCollapseEvent::setDamageBuildingTime(float seconds)
	{
		mDamageBuildingTime = qsf::Time::fromSeconds(seconds);
	}

	float BuildingCollapseEvent::getHint01WaitTime() const
	{
		return mHint01WaitTime.getSeconds();
	}

	void BuildingCollapseEvent::setHint01WaitTime(float seconds)
	{
		mHint01WaitTime = qsf::Time::fromSeconds(seconds);
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	bool BuildingCollapseEvent::onStartup()
	{
		qsf::Entity* buildingEntity = getMap().getEntityById(mTargetBuildingId);
		if (buildingEntity == nullptr)
			return false;

		if (!collectEventTagEntities(*buildingEntity))
			return false;

		if (!explodeBuilding(*buildingEntity))
			return false;

		if (!EventIdComponent::addToEvent(*buildingEntity, *this, eventspreadreason::EVENT_SPECIFIC))
			return false;

		// Buildings have to be damaged before start - so injured persons and debris spawns inside the dust of the crumbling building
		qsf::MessageConfiguration messageDamage(Messages::EM5_EVENT_TIMER_SIGNAL, getId());
		mTimerMessageProxy.registerAt(messageDamage, boost::bind(&BuildingCollapseEvent::onDamageBuilding, this, _1));
		mDamageTimerId = EM5_GAMETIMERS.addTimer(messageDamage, mDamageBuildingTime);

		return true;
	}

	void BuildingCollapseEvent::onShutdown()
	{
		// Disable particles
		if (mExplosion != nullptr)
		{
			qsf::ParticlesComponent * particleComponent = mExplosion->getComponent<qsf::ParticlesComponent>();
			if (particleComponent != nullptr)
			{
				particleComponent->setEmitterEnabled(false);
				particleComponent->setActive(false);
			}
		}

		mCraneLiftMessageProxy.unregister();
		mTimerMessageProxy.unregister();
	}

	void BuildingCollapseEvent::onRun()
	{
		// TODO(mz): Start Action-music

		qsf::Entity* buildingEntity = getMap().getEntityById(mTargetBuildingId);

		spawnDebrisAndInjured(*buildingEntity);
		spawnRubble(*buildingEntity);
		startFires(*buildingEntity);

		// Show supervisor message
		showSupervisorMessage(HintHelper::getRandomStringOfIdString(HintHelper(*this).getHintParameters().mSupervisorMessageIds));

		if (mSpawnPersonsUnderDebris)
		{
			// Play the hint if the player does nothing of those:
			// - Ordered a to lift some debris
			// - Ordered tech_crane
			MessageObserver& observePlayerActivity = createGeneralObserver<MessageObserver>("PLAYER_ACTIVE");
			observePlayerActivity.initialize(qsf::MessageConfiguration(Messages::EM5_ORDER_UNIT, CommandableComponent::UNITTAG_TECH_CRANE),
				qsf::MessageConfiguration(Messages::EM5_POST_COMMAND_EXECUTE, LiftUpByCraneCommand::PLUGINABLE_ID),
				MessageObserver::OR_LOGIC);
			mPlayerActivityObserverId = observePlayerActivity.getId();

			// Sends hint if message was not created after mHint01WaitTime
			createGeneralObserver<HintMessageGeneralObserver>("EM5_EVENT_BUILDING_COLLAPSED_HINT_01").initialize(
				qsf::MessageConfiguration(Messages::EM5_OBSERVER_MESSAGE, mPlayerActivityObserverId), mHint01WaitTime, false);
		}

		// Check for all original victims if their health energy drops too low
		for (uint64 victimId : mVictims)
		{
			HintLowEnergyObserver& hint2Observer = createObserver<HintLowEnergyObserver>(victimId, "EM5_EVENT_BUILDING_COLLAPSED_HINT_02");
			hint2Observer.initialize(mHint2LifeEnergy*0.01f, HintLowEnergyObserver::HINTENERGYTYPE_LIFE);
		}

		// Objectives
		if (mSpawnPersonsUnderDebris)
		{
			Objective& needConditionFindPersonObjective = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_PERSONSBURIEDHEAVYDEBRIS);
			needConditionFindPersonObjective.setNeededNumber((uint32)mDebris.size());
			qsf::MessageConfiguration msgUnbury(Messages::EM5_UNHIDE_BURIED);
			mCraneLiftMessageProxy.registerAt(msgUnbury, boost::bind(&BuildingCollapseEvent::onUnbury, this, _1));
		}

		// Fail condition, no one may die.
		ObjectiveHelper(*this).addDeadCivilPersonsFailCondition(1);
		ObjectiveHelper(*this).addDeadUnitPersonsFailCondition(1);
	}

	void BuildingCollapseEvent::updateFreeplayEvent(const qsf::Time& timePassed)
	{
		// Check objectives for success or failure
		checkObjectivesState();
	}

	bool BuildingCollapseEvent::addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason)
	{
		ObserverHelper observerHelper(*this);

		// Call base implementation
		return FreeplayEvent::addEntityToEvent(targetEntity, eventSpreadReason, newReason);
	}

	void BuildingCollapseEvent::hintCallback(Observer& hintObserver)
	{
		const std::string& observerName = hintObserver.getName();

		if (!wasHintShown(observerName))
		{
			// TODO(mz):

			showHint(observerName, true);

			// Short-Action-Music on EM5_EVENT_BUILDING_COLLAPSED_HINT_02
			getDynamicMusicCompositor().setMusicLevel(3);
		}
	}

	bool BuildingCollapseEvent::checkEventConfiguration()
	{
		// Can't think of additional checks to those of the factory
		return true;
	}

	const qsf::Entity* BuildingCollapseEvent::getFocusEntity()
	{
		const qsf::Map& map = getMap();
		const ObjectiveHelper objectiveHelper(*this);

		// Stay at the ruin of the house as long as there are persons to transport
		// (per design, but probably would be better to switch thos those if they are too far away from the ruin)
		if (objectiveHelper.getObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_TRANSPORTINJURED) != nullptr)
		{
			return map.getEntityById(mTargetBuildingId);
		}

		{ // Any other fire left?
			const Objective* extingushFireObjective = objectiveHelper.getObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_EXTINGUISHFIRES);
			if (nullptr != extingushFireObjective)
			{
				const qsf::Entity* candidateEntity = map.getEntityById(extingushFireObjective->getRandomNeededEntityId());
				if (nullptr != candidateEntity)
				{
					return candidateEntity;
				}
			}
		}

		{ // Any contaminated persons?
			const Objective* treatPersonsObjective = objectiveHelper.getObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_DECONTAMINATEPERSONS);
			if (nullptr != treatPersonsObjective)
			{
				const qsf::Entity* candidateEntity = map.getEntityById(treatPersonsObjective->getRandomNeededEntityId());
				if (nullptr != candidateEntity)
				{
					return candidateEntity;
				}
			}
		}

		{ // Any damaged vehicles left?
			const Objective* cleanupVehiclesObjective = objectiveHelper.getObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_CLEANUPVEHICLES);
			if (nullptr != cleanupVehiclesObjective)
			{
				// Use the target building instead of the car, to avoid wrong position when its transported to the hq
				if (cleanupVehiclesObjective->checkAccomplished() == false)
				{
					return map.getEntityById(mTargetBuildingId);
				}
			}
		}

		// Stay at the ruin
		return map.getEntityById(mTargetBuildingId);
	}

	void BuildingCollapseEvent::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base implementation
		FreeplayEvent::serialize(serializer);

		serializer.serialize(mTargetBuildingId);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	bool BuildingCollapseEvent::collectEventTagEntities(qsf::Entity& buildingEntity)
	{
		qsf::LinkComponent* linkComponent = buildingEntity.getComponent<qsf::LinkComponent>();
		if (linkComponent == nullptr)
			return false;

		// Get all entities belonging to the building which are tagged for this event
		mDebrisPlaceholders.clear();
		mExplosion = nullptr;
		for (qsf::LinkComponent* childLinkComponent : linkComponent->getChildLinks())
		{
			qsf::Entity& childEntity = childLinkComponent->getEntity();
			qsf::game::EventTagComponent* tagComponent = childEntity.getComponent<qsf::game::EventTagComponent>();
			if (tagComponent)
			{
				// Debris
				if (tagComponent->hasTag(mEventTagDebris))
				{
					// Must have a person placeholder attached as child
					if (getPersonPlaceholderLinked(tagComponent->getEntity()))
					{
						mDebrisPlaceholders.push_back(&tagComponent->getEntity());
					}
				}
				// Explosion (no longer used)
				else if (tagComponent->hasTag(mEventTagExplosion))
				{
					mExplosion = &tagComponent->getEntity();
				}
			}
		}

		QSF_ASSERT(!mDebrisPlaceholders.empty(), "building tagged for collapse event but has no debris attached", QSF_REACT_NONE);

		return !mDebrisPlaceholders.empty();
	}

	qsf::Entity* BuildingCollapseEvent::getPersonPlaceholderLinked(qsf::Entity& entity) const
	{
		const qsf::LinkComponent* linkComponent = entity.getComponent<qsf::LinkComponent>();
		if (nullptr == linkComponent)
			return nullptr;

		const boost::container::flat_set<qsf::LinkComponent*>& childLinks = linkComponent->getChildLinks();
		if (childLinks.empty())
			return nullptr;

		// There's only 1 child edited for those, so take it
		return &(*childLinks.begin())->getEntity();
	}

	std::string BuildingCollapseEvent::getAdultCivilianPrefabName() const
	{
		return EM5_GAME.getSimulation().getSpawnPoolManager().getRandomNameFromSpawnPool(assets::SPAWNPOOL_DEFAULT_PERSON);
	}

	bool BuildingCollapseEvent::explodeBuilding(qsf::Entity& buildingEntity)
	{
		// Let the building explode
		if (mExplosion != nullptr)
		{
			// Custom particles (no longer needed for now) as there's a general explosion slot which can be used

			qsf::ParticlesComponent * particleComponent = mExplosion->getComponent<qsf::ParticlesComponent>();
			if (particleComponent != nullptr)
			{
				particleComponent->setActive(true);
				particleComponent->setEmitterEnabled(true);
				particleComponent->setRepeatAnimation(false);

				const qsf::TransformComponent& transformComponent = particleComponent->getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();
				mExplosionCenter = transformComponent.getPosition();
			}
		}
		else
		{
			BuildingComponent* buildingComponent = buildingEntity.getComponent<BuildingComponent>();
			if (buildingComponent != nullptr)
				buildingComponent->showExplosionFireParticles();

			const qsf::TransformComponent& transformComponent = buildingEntity.getOrCreateComponentSafe<qsf::TransformComponent>();
			mExplosionCenter = transformComponent.getPosition();
		}

		// TODO(mz): Play boom sound;

		return true;
	}

	void BuildingCollapseEvent::startFires(const qsf::Entity& buildingEntity)
	{
		// Start careful fires that don't affect other buildings or vehicles
		qsf::Map& map = buildingEntity.getMap();
		std::vector<FireComponent*> fireComponentVector;
		qsf::ComponentMapQuery(map).getInstancesInCircle(mExplosionCenter, mExplosionRadius, fireComponentVector);
		for (FireComponent* fireComponent : fireComponentVector)
		{
			// Direct check for entity
			const EntityHelper entityHelper(fireComponent->getEntity());
			if (entityHelper.isEntityBuilding() || entityHelper.isVehicle())
				continue;

			// Same check for top-most parent
			qsf::Prototype& prototype = qsf::LinkComponent::getTopmostAncestorPrototype(fireComponent->getEntity(), qsf::LinkComponent::SELECT_PARENT);
			if (prototype.isEntity())
			{
				const EntityHelper entityAncestorHelper(static_cast<qsf::Entity&>(prototype));
				if (entityAncestorHelper.isEntityBuilding() || entityAncestorHelper.isVehicle())
					continue;
			}

			// Burn it baby
			fireComponent->startFire(this);
		}
	}

	void BuildingCollapseEvent::onDamageBuilding(const qsf::MessageParameters& parameters)
	{
		qsf::Entity* buildingEntity = getMap().getEntityById(mTargetBuildingId);
		if (buildingEntity != nullptr)
		{
			BuildingComponent* buildingComponent = buildingEntity->getComponent<BuildingComponent>();
			if (buildingComponent != nullptr)
			{
				// Building now destroyed
				buildingComponent->setDamageState(DamageComponent::DAMAGESTATE_DESTROYED);
			}
		}

		// Once debris spawns the event really starts
		qsf::MessageConfiguration messageStart(Messages::EM5_EVENT_TIMER_SIGNAL, getId());
		mTimerMessageProxy.registerAt(messageStart, boost::bind(&BuildingCollapseEvent::onStartRunning, this, _1));
		mDebrisTimerId = EM5_GAMETIMERS.addTimer(messageStart, mDebrisSpawnTime);
	}

	void BuildingCollapseEvent::onStartRunning(const qsf::MessageParameters&)
	{
		// Event is now running
		setRunning();
	}

	void BuildingCollapseEvent::onUnbury(const qsf::MessageParameters& parameters)
	{
		uint64 unburiedId = 0;
		parameters.getParameter("DebrisId", unburiedId);

		// Check if one of our debris parts was removed
		for (auto it = mDebris.begin(); it != mDebris.end(); ++it)
		{
			// Is this the debris which had hidden the person?
			if (*it == unburiedId)
			{
				// Remove the debris and update the objective
				ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_PERSONSBURIEDHEAVYDEBRIS).increaseCurrentNumber();
				mDebris.erase(it);
				return;
			}
		}
	}

	void BuildingCollapseEvent::spawnDebrisAndInjured(qsf::Entity& buildingEntity)
	{
		qsf::Map& map = buildingEntity.getMap();

		// Injured persons get one of those randomly
		std::vector<uint32> potentialInjuries;
		potentialInjuries.push_back(injury::BURN_3_GRADE);
		potentialInjuries.push_back(injury::BURN_4_GRADE);
		potentialInjuries.push_back(injury::BROKEN_SKULL);
		potentialInjuries.push_back(injury::HEAD_BODY_LACERATION);

		// How much debris do we want?
		uint32 numDebris = qsf::Random::getRandomUint(mMinDebris, mMaxDebris);
		QSF_ASSERT((size_t)numDebris <= mDebrisPlaceholders.size(), "Either too many debris requested or building doesn't have enough debris added", QSF_REACT_NONE);
		if (numDebris > mDebrisPlaceholders.size())
			numDebris = (uint32)mDebrisPlaceholders.size();

		// Prepare for it
		mVictims.clear();
		mVictims.reserve((size_t)numDebris);
		mDebris.clear();
		mDebris.reserve((size_t)numDebris);
		qsf::CollisionHelper collisionHelper(buildingEntity.getMap());

		// Go through all potential debris positions
		for (uint32 i = 0; i < numDebris && !mDebrisPlaceholders.empty(); ++i)
		{
			// Take one of those randomly
			const size_t randIndex = qsf::Random::getRandomSize(0, mDebrisPlaceholders.size() - 1);
			qsf::Entity& placeholderEntity = *mDebrisPlaceholders[randIndex];

			// Is that debris still free? (TODO(mz): when we get a findFreePosition we can use that)
			if (collisionHelper.canBeAddedWithoutCollision(placeholderEntity))
			{
				qsf::Entity* debrisEntity = nullptr;
				if (mSpawnPersonsUnderDebris)
				{
					// Create a debris object
					debrisEntity = MapHelper(map).createEntityByPrototypeReference(placeholderEntity);	// Only copy one entity without childs
					if (debrisEntity != nullptr)
					{
						// Make entity visible
						debrisEntity->destroyComponent<qsf::game::HiddenComponent>();
						debrisEntity->destroyComponent<qsf::LinkComponent>();	// The entity we copy is hard linked to the building, we don't want this

						// Activate collision
						qsf::BulletBoxCollisionComponent* bulletBoxCollisionComponent = debrisEntity->getOrCreateComponent<qsf::BulletBoxCollisionComponent>();
						bulletBoxCollisionComponent->setActive(true);

						EventIdComponent::addToEvent(*debrisEntity, *this, eventspreadreason::EVENT_SPECIFIC);
						mDebris.push_back(debrisEntity->getId());

						// Put on the placeholder position
						usePlaceholderTransform(*debrisEntity, placeholderEntity);
					}
					else
					{
						QSF_ERROR("Could not copy debris entity for building collapse", QSF_REACT_NONE);
					}
				}

				// Spawn persons under the debris (on person placeholder position).
				// Use a random adult person (to avoid size-differences).
				qsf::Entity* personPlaceholderEntity = getPersonPlaceholderLinked(placeholderEntity);
				if (personPlaceholderEntity != nullptr)
				{
					// Create a person
					std::string personId = getAdultCivilianPrefabName();
					qsf::Entity* personEntity = map.createObjectByLocalPrefabAssetId(qsf::StringHash(personId));
					if (personEntity != nullptr)
					{
						EventIdComponent::addToEvent(*personEntity, *this, eventspreadreason::EVENT_SPECIFIC);

						mVictims.push_back(personEntity->getId());

						// Put it on placeholder position
						usePlaceholderTransform(*personEntity, *personPlaceholderEntity);

						// Injure person
						personEntity->getComponentSafe<HealthComponent>().applyInjuryById(qsf::Random::getAnyOf(potentialInjuries), this, false);

						if (mSpawnPersonsUnderDebris && debrisEntity != nullptr)
						{
							// Bury it under the debris
							BuryComponent& buryComponent = debrisEntity->getOrCreateComponentSafe<BuryComponent>();
							buryComponent.buryEntity(*personEntity);
							buryComponent.setDraggable(false);
						}
					}
					else
					{
						QSF_ERROR("Got an invalid person id from the spawnpool" << personId, QSF_REACT_NONE);
					}
				}
				else
				{
					QSF_ERROR("Lost the person under the placeholder debris - should be impossible", QSF_REACT_NONE);
				}
			}

			// Don't use it again
			mDebrisPlaceholders.erase(mDebrisPlaceholders.begin() + randIndex);
		}
	}

	void BuildingCollapseEvent::spawnRubble(qsf::Entity& buildingEntity)
	{
		BuildingComponent * buildingComponent = buildingEntity.getComponent<BuildingComponent>();
		if (buildingComponent != nullptr)
		{
			buildingComponent->setCollapseRubbleVisible(true);
		}
	}

	void BuildingCollapseEvent::usePlaceholderTransform(qsf::Entity& entity, const qsf::Entity& placeholder) const
	{
		qsf::TransformComponent& transformTarget = entity.getOrCreateComponentSafe<qsf::TransformComponent>();
		const qsf::TransformComponent& transformPlaceholder = placeholder.getComponentSafe<qsf::TransformComponent>();
		transformTarget.setTransform(transformPlaceholder.getTransform());
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
