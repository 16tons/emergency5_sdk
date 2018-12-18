// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/fire/SimpleFireEvent.h"
#include "em5/event/EventHelper.h"
#include "em5/base/GameplayAssets.h"
#include "em5/base/ContainerCategory.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/component/building/IndustrialPlantComponent.h"
#include "em5/component/spawnpoint/civilist/PersonSpawnPointComponent.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/command/fire/DecontaminatePersonCommand.h"
#include "em5/freeplay/objective/ObjectiveHelper.h"
#include "em5/fire/component/FireComponent.h"
#include "em5/game/Game.h"
#include "em5/health/HealthHelper.h"
#include "em5/logic/ObserverHelper.h"
#include "em5/logic/HintHelper.h"
#include "em5/logic/observer/hint/HintMessageGeneralObserver.h"
#include "em5/logic/observer/generic/MessageObserver.h"
#include "em5/logic/observer/PersonDiedObserver.h"
#include "em5/logic/cleanup/LayerCleanup.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/EM5Helper.h"

#include <qsf_game/timer/GameTimerManager.h>

#include <qsf/audio/component/DynamicMusicCompositorComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/log/LogSystem.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/map/layer/LayerManager.h>
#include <qsf/map/query/ComponentMapQuery.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Local definitions                                     ]
	//[-------------------------------------------------------]
	// Hints
	const std::string PERSON_BALCONY_HINT_01 = "EM5_EVENT_PERSON_BALCONY_HINT_01";
	const qsf::Time PERSON_BALCONY_HINT_01_WAIT_TIME(qsf::Time::fromSeconds(60.0f));


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 SimpleFireEvent::FREEPLAY_EVENT_ID = qsf::StringHash("em5::SimpleFireEvent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	SimpleFireEvent::SimpleFireEvent() :
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mDeadPersonsToFail(0),
		mInjuredSquadToFail(0),
		mBurningObjectsToFail(0),
		mExplosion(false),
		mIndustrialPlant(false),
		mDefectEbox(false),
		mExplosionRadius(0.0f),
		mExplosionId(qsf::getUninitialized<uint64>()),
		mPersonsInside(false),
		mPersonsAtWindow(false),
		mTrappedPersonsRescueTime(qsf::Time::fromSeconds(300.0f)),
		mTrappedHealthyPersonsMin(1),
		mTrappedHealthyPersonsMax(3),
		mTrappedInjuredPersonsMin(0),
		mTrappedInjuredPersonsMax(0),
		mTrappedPersonWithPlague(false),
		mBlockedDoor(true),
		mSpawnFromConnectedSpawnpools(false),
		mTrappedPersons(0),
		mTrappedInjuredPersons(0),
		mHint01WaitTime(qsf::Time::fromSeconds(60.0f))
	{
		// Nothing here
	}

	SimpleFireEvent::~SimpleFireEvent()
	{
		// Nothing here
	}

	uint64 SimpleFireEvent::getTargetEntityId() const
	{
		return mTargetEntityId;
	}

	void SimpleFireEvent::setTargetEntity(const qsf::Entity& entity)
	{
		mTargetEntityId = entity.getId();
	}

	uint32 SimpleFireEvent::getDeadPersonsToFail() const
	{
		return mDeadPersonsToFail;
	}

	void SimpleFireEvent::setDeadPersonsToFail(uint32 personsToFail)
	{
		mDeadPersonsToFail = personsToFail;
	}

	void SimpleFireEvent::setAmbientLayerName(const std::string& ambientLayerName)
	{
		mAmbientLayerName = ambientLayerName;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	bool SimpleFireEvent::onStartup()
	{
		// What should be done with the entity?
		if (mIndustrialPlant)
		{
			qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
			executeIndustrialPlantFire(*targetEntity);

			for (uint64 entityId : mVictimSpawnPoints)
			{
				qsf::Entity* spawnEntity = getMap().getEntityById(entityId);
				EventHelper::spawnEntity(*spawnEntity);
			}
		}
		else
		{
			// Event is now running
			setRunning();
		}

		// Done
		return true;
	}

	void SimpleFireEvent::onShutdown()
	{
		// Destroy the created explosion
		if (qsf::isInitialized(mExplosionId))
		{
			getMap().destroyEntityById(mExplosionId);
		}

		// Ensure everything from the event is cleaned up.
		if (!mAmbientLayerName.empty())
		{
			qsf::Layer* layer = getMap().getLayerManager().getLayerByName(qsf::StringHash(mAmbientLayerName));
			if (nullptr != layer)
			{
				LayerCleanup::createLayerCleanup(*layer);
			}
		}

		mMessageProxyStoppedBurning.unregister();
		mMessageProxyContaminationStart.unregister();
		mMessageProxyNeedContaminatedTime.unregister();
	}

	void SimpleFireEvent::onRun()
	{
		// No null pointer check in here. The factory already checked these pointers.
		qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);

		// What should be done with the entity?
		if (mIndustrialPlant)
		{
			// In this case we already have a burning building and contaminated person here

			ObjectiveHelper(*this).addDeadPersonsFailCondition(2);
		}
		else if (mExplosion)
		{
			executeExplosion(*targetEntity);
		}
		else if (mPersonsInside)
		{
			executeFireWithPersonsInside(*targetEntity);
		}
		else if (mPersonsAtWindow)
		{
			executeFireWithPersonAtWindow(*targetEntity);
		}
		else
		{
			executeNormalFire(*targetEntity);
		}

		spawnPersonsFromConnectedSpawnPoints(*targetEntity);

		// Activate ambient layer
		if (!mAmbientLayerName.empty())
		{
			MapHelper(getMap()).activateLayerByName(mAmbientLayerName, true);
		}

		// Set all observers and objectives for the first target
		startObjectives(*targetEntity);

		// Set start hint observers after the event entity was added
		startHintObservers(*targetEntity);

		// Show supervisor message
		showSupervisorMessage(HintHelper::getRandomStringOfIdString(HintHelper(*this).getHintParameters().mSupervisorMessageIds));

		createEventLocationEntity(*targetEntity);
	}

	void SimpleFireEvent::updateFreeplayEvent(const qsf::Time& timePassed)
	{
		// Check objectives for success or failure
		checkObjectivesState();
	}

	bool SimpleFireEvent::addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason)
	{
		if (eventspreadreason::INJURY == eventSpreadReason)
		{
			// Get or create fail conditions
			Objective& failConditionPersonDiedObjective = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_FAIL_DEADPERSONS);

			// Create and connect observers
			PersonDiedObserver& observerDied = ObserverHelper(*this).createObserver<PersonDiedObserver>(targetEntity.getId());
			observerDied.connectToObjective(failConditionPersonDiedObjective);

			// Call base implementation
			FreeplayEvent::addEntityToEvent(targetEntity, eventSpreadReason, newReason);

			// Done
			return true;
		}

		// Call base implementation
		return FreeplayEvent::addEntityToEvent(targetEntity, eventSpreadReason, newReason);
	}

	void SimpleFireEvent::hintCallback(Observer& hintObserver)
	{
		showHint(hintObserver.getName(), false);

		// Play short action music
		getDynamicMusicCompositor().setMusicLevel(3);
	}

	const qsf::Entity* SimpleFireEvent::getFocusEntity()
	{
		const qsf::Map& map = getMap();

		{ // Is the first building still burning?
			qsf::Entity* candidateEntity = map.getEntityById(mTargetEntityId);
			if (nullptr != candidateEntity && EntityHelper(*candidateEntity).isBurning())
			{
				return candidateEntity;
			}
		}

		const ObjectiveHelper objectiveHelper(*this);

		{ // Is there any other fire left?
			Objective* extingushFireObjective = objectiveHelper.getObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_EXTINGUISHFIRES);
			if (nullptr != extingushFireObjective)
			{
				qsf::Entity* candidateEntity = map.getEntityById(extingushFireObjective->getRandomNeededEntityId());
				if (nullptr != candidateEntity)
				{
					return candidateEntity;
				}
			}
		}

		{ // Are there any persons injured?
			Objective* treatPersonsObjective = objectiveHelper.getObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_TREATPERSONS);
			if (nullptr != treatPersonsObjective)
			{
				qsf::Entity* candidateEntity = map.getEntityById(treatPersonsObjective->getRandomNeededEntityId());
				if (nullptr != candidateEntity)
				{
					return candidateEntity;
				}
			}
		}

		{ // Is there an entity to untrap?
			const Objective* freePersonsObjective = objectiveHelper.getObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_FREEPERSONSFROMBUILDING);
			if (nullptr != freePersonsObjective)
			{
				const qsf::Entity* candidateEntity = map.getEntityById(freePersonsObjective->getRandomNeededEntityId());
				if (nullptr != candidateEntity)
				{
					return candidateEntity;
				}
			}
		}

		{ // Are there any persons contaminated?
			Objective* objective = objectiveHelper.getObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_DECONTAMINATEPERSONS);
			if (nullptr != objective)
			{
				qsf::Entity* candidateEntity = map.getEntityById(objective->getRandomNeededEntityId());
				if (nullptr != candidateEntity)
				{
					return candidateEntity;
				}
			}
		}

		return getEventLocationEntity();
	}

	bool SimpleFireEvent::checkEventConfiguration()
	{
		// TODO(mk) Add Check if event has right configuration an can start now

		return true;
	}

	void SimpleFireEvent::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base implementation
		FreeplayEvent::serialize(serializer);

		serializer.serialize(mTargetEntityId);
		serializer.serialize(mDeadPersonsToFail);
		serializer.serialize(mExplosionId);
		serializer.serialize(mExplosion);
		serializer.serialize(mExplosionRadius);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void SimpleFireEvent::startObjectives(const qsf::Entity& targetEntity)
	{
		ObjectiveHelper objectiveHelper(*this);
		if (mDeadPersonsToFail > 0)
		{
			// Create the fail condition
			Objective& failConditionPersonDiedObjective = objectiveHelper.getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_FAIL_DEADPERSONS);

			int numberDeadPersons = (mDeadPersonsToFail >= mTrappedInjuredPersons ? mDeadPersonsToFail : mTrappedInjuredPersons);
			failConditionPersonDiedObjective.setNeededNumber(numberDeadPersons);
		}

		if (mInjuredSquadToFail > 0)
		{
			objectiveHelper.addInjuredUnitPersonsFailCondition(mInjuredSquadToFail);
		}
		if (mBurningObjectsToFail > 0)
		{
			objectiveHelper.addBurningObjectsFailCondition(mBurningObjectsToFail);
		}
	}

	void SimpleFireEvent::startHintObservers(const qsf::Entity& targetEntity)
	{
		const HintHelper hintHelper(*this);
		hintHelper.createAndInitializeGeneralHintsAtInjured();
		hintHelper.createAndInitializeGeneralHintAtDamagedVehicle();

		if (mPersonsInside)
		{
			if (mBlockedDoor)
			{
				// Hint 01   Firefighter has not started to break the door after X sec
				createGeneralObserver<HintMessageGeneralObserver>("EM5_EVENT_PERSON_INJURED_IN_BUILDING_HINT_07").initialize(
					qsf::MessageConfiguration(Messages::EM5_ACTION_BREAK_BUILDING_DOOR_OPEN_START, mTargetEntityId), mHint01WaitTime, false);
			}
		}
		else if (mIndustrialPlant)
		{
			// Play the hint if the player does nothing of those:
			// - ordered to decontaminate someone
			// - ordered a decontaminatin vehicle
			MessageObserver& observePlayerActivity = createGeneralObserver<MessageObserver>("PLAYER_ACTIVE");
			observePlayerActivity.initialize(qsf::MessageConfiguration(Messages::EM5_ORDER_UNIT, CommandableComponent::UNITTAG_FIREFIGHTERS_DEKONP),
				qsf::MessageConfiguration(Messages::EM5_POST_COMMAND_EXECUTE, DecontaminatePersonCommand::PLUGINABLE_ID),
				MessageObserver::OR_LOGIC);

			// Sends hint if message was not created after mHint01WaitTime
			const std::string& hintMsg("EM5_EVENT_PERSON_CHEMICAL_CONTAMINATION_HINT_01");
			createGeneralObserver<HintMessageGeneralObserver>(hintMsg).initialize(
				qsf::MessageConfiguration(Messages::EM5_OBSERVER_MESSAGE, observePlayerActivity.getId()), mHint01WaitTime, false);
		}
	}

	bool SimpleFireEvent::onFailure(EventResult& eventResult)
	{
		if (isMiniEvent())
		{
			// Extra variant for the minievent08
			const uint32 failObjective = getFailReasonObjectiveTypeId(eventResult.mObjectives);
			if (ObjectiveHelper::OBJECTIVE_FAIL_DEADPERSONS == failObjective)
			{
				setFailHintsForMiniEvent(eventResult, "EM5_GE08_FAIL01", "EM5_GE01_SUPERV_FAIL01", "");
				return true;
			}
			else if (ObjectiveHelper::OBJECTIVE_FAIL_INJUREDSQUAD == failObjective)
			{
				setFailHintsForMiniEvent(eventResult, "EM5_GE07_FAIL05", "EM5_GE01_SUPERV_FAIL02", "");
				return true;
			}
			else if (ObjectiveHelper::OBJECTIVE_FAIL_BURNING_OBJECTS == failObjective)
			{
				setFailHintsForMiniEvent(eventResult, "EM5_GE02_FAIL06", "EM5_GE05_SUPERV_FAIL04 ", "");
				return true;
			}
		}

		// Call base implementation
		return FreeplayEvent::onFailure(eventResult);
	}

	void SimpleFireEvent::executeNormalFire(qsf::Entity& entity)
	{
		// Start fire
		EntityHelper(entity).startFire(this);
	}

	void SimpleFireEvent::executeFireWithPersonAtWindow(qsf::Entity& entity)
	{
		BuildingComponent* buildingComponent = entity.getComponent<BuildingComponent>();
		if (nullptr == buildingComponent)
			return;

		qsf::Entity* personEntity = SpawnPointComponent::spawnRandomAt(qsf::Transform(), getMap(), assets::SPAWNPOOL_DEFAULT_PERSON, false);
		if (nullptr == personEntity)
			return;

		EntityHelper(*personEntity).enterContainer(entity, container::CONTAINERTYPE_PASSENGER);

		// Let person wave out of window
		buildingComponent->letPersonWaveOutOfWindow(*personEntity, this);

		// Start fire
		EntityHelper(entity).startFire(this);

		// Specific Hint if no dlk ordered after X seconds
		createGeneralObserver<HintMessageGeneralObserver>(PERSON_BALCONY_HINT_01).initialize(
			qsf::MessageConfiguration(Messages::EM5_ACTION_LOAD_PERSON_ON_DLK, personEntity->getId()), PERSON_BALCONY_HINT_01_WAIT_TIME, false);
	}

	void SimpleFireEvent::executeFireWithPersonsInside(qsf::Entity& entity)
	{
		BuildingComponent* buildingComponent = entity.getComponent<BuildingComponent>();
		if (nullptr == buildingComponent)
			return;

		// Trap persons inside the building
		if (mTrappedPersonWithPlague)
		{
			buildingComponent->setTrappedPersonsInjuryId(injury::PLAGUE_CONTAMINATION);
		}
		buildingComponent->setupTrappedInjuredPersons(mTrappedHealthyPersonsMin, mTrappedHealthyPersonsMax, mTrappedInjuredPersonsMin, mTrappedInjuredPersonsMax, mBlockedDoor, true, mTrappedPersonsRescueTime);

		mTrappedPersons = buildingComponent->getSetupNumberOfTrappedPersons();
		mTrappedInjuredPersons = buildingComponent->getSetupNumberOfTrappedInjuredPersons();

		// Start fire
		EntityHelper(entity).startFire(this);
	}

	void SimpleFireEvent::executeExplosion(qsf::Entity& entity)
	{
		BuildingComponent* buildingComponent = entity.getComponent<BuildingComponent>();
		if (nullptr == buildingComponent)
			return;

		// Let the building explode
		std::vector<FireComponent*> fireComponentVector;
		std::vector<PersonComponent*> personComponentVector;
		qsf::Map& map = entity.getMap();

		glm::vec3 position = buildingComponent->getExplosionCenterWorldSpacePosition();

		// Get fire targets and persons in the explosion radius
		qsf::ComponentMapQuery(map).getInstancesInCircle(position, mExplosionRadius, fireComponentVector);
		qsf::ComponentMapQuery(map).getInstancesInCircle(position, mExplosionRadius, personComponentVector);

		// Start fire
		uint32 firesStarted = 0;
		for (FireComponent* fireComponent : fireComponentVector)
		{
			const EntityHelper entityHelper(fireComponent->getEntity());
			if (!entityHelper.isVehicle())
			{
				if (fireComponent->isBurnable() && !fireComponent->isBurning())
				{
					fireComponent->startFire(this);
					++firesStarted;
				}
			}
		}

		// Fallback: If no fires were started yet, incinerate the building
		if (firesStarted == 0)
		{
			EntityHelper(buildingComponent->getEntity()).startFire(this);
		}

		// Let persons flee
		for (PersonComponent* personComponent : personComponentVector)
		{
			if (EntityHelper(personComponent->getEntity()).isCivilPersonInValidState())
			{
				personComponent->getEntity().getOrCreateComponentSafe<qsf::GameLogicComponent>().createGameLogicByTypeId(qsf::StringHash("em5::FleeFromFireLogic"));
			}
		}

		// Create particle effect. The explosion particle effect must be started after the fire is started, otherwise it will be deactivated by the startFire process.
		buildingComponent->showExplosionFireParticles();
	}

	void SimpleFireEvent::executeIndustrialPlantFire(qsf::Entity& entity)
	{
		QSF_LOG_PRINTS(DEBUG, "executeIndustrialPlantFire");

		IndustrialPlantComponent* industrialPlantComponent = entity.getComponent<IndustrialPlantComponent>();
		QSF_ASSERT(nullptr != industrialPlantComponent, "SimpleFireEvent::executeIndustrialPlantFire does miss the IndustrialPlantComponent", return);

		industrialPlantComponent->startSimulation();

		mMessageProxyContaminationStart.registerAt(qsf::MessageConfiguration(Messages::EM5_COMPONENT_CONTAMINATION_START_SPREADING, entity.getId()), boost::bind(&SimpleFireEvent::onContaminationStartSpreading, this, _1));

		executeNormalFire(entity);
	}

	void SimpleFireEvent::onContaminationStartSpreading(const qsf::MessageParameters& parameters)
	{
		// Contamination started. If we have no contaminated person in a short time we have to spawn one.
		if (!mMessageProxyNeedContaminatedTime.isValid())
		{
			qsf::MessageConfiguration messageStart(Messages::EM5_EVENT_TIMER_SIGNAL, getId());
			mMessageProxyNeedContaminatedTime.registerAt(messageStart, boost::bind(&SimpleFireEvent::onNeedContaminated, this, _1));
			EM5_GAMETIMERS.addTimer(messageStart, qsf::Time::fromSeconds(0.5f));
		}
	}

	void SimpleFireEvent::onNeedContaminated(const qsf::MessageParameters& parameters)
	{
		qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
		QSF_ASSERT(nullptr != targetEntity, "SimpleFireEvent::onNeedContaminated does miss the targetEntity", return);

		IndustrialPlantComponent* industrialPlantComponent = targetEntity->getComponent<IndustrialPlantComponent>();
		QSF_ASSERT(nullptr != industrialPlantComponent, "SimpleFireEvent::onNeedContaminated does miss the IndustrialPlantComponent", return);

		if (0 == industrialPlantComponent->getContaminatedPersons())
		{
			// Have to spawn one
			spawnInContaminationArea(*targetEntity);
		}

		setRunning();
	}

	void SimpleFireEvent::spawnInContaminationArea(qsf::Entity& buildingEntity)
	{
		BuildingComponent* buildingComponent = buildingEntity.getComponent<BuildingComponent>();
		QSF_ASSERT(nullptr != buildingComponent, "SimpleFireEvent::spawnInContaminationArea does miss the targetEntity", return);
		if (buildingComponent != nullptr)
		{
			QSF_ASSERT(buildingComponent->hasDoor(), "SimpleFireEvent::spawnInContaminationArea has a building without door", return);

			glm::vec3 position;
			glm::quat rotation;
			float distance = 2.0f;
			if (buildingComponent->getWorldPositionInFrontOfDoor(distance, position, rotation))
			{
				QSF_LOG_PRINTS(DEBUG, "spawnInContaminationArea");
				qsf::Transform transform;
				transform.setPositionAndRotation(position, rotation);

				SpawnPointComponent::spawnRandomAt(transform, getMap(), assets::SPAWNPOOL_DEFAULT_PERSON, false);
			}
		}
	}

	void SimpleFireEvent::spawnPersonsFromConnectedSpawnPoints(qsf::Entity& buildingEntity)
	{
		if (mSpawnFromConnectedSpawnpools)
		{
			BuildingComponent* buildingComponent = buildingEntity.getComponent<BuildingComponent>();
			if (nullptr == buildingComponent)
				return;

			qsf::LinkComponent* linkComponent = buildingEntity.getComponent<qsf::LinkComponent>();
			if (linkComponent != nullptr)
			{
				// We test here for tags but it is also possible to use the array position to find the right entity
				for (const qsf::LinkComponent* childComponent : linkComponent->getChildLinks())
				{
					qsf::Entity& childEntity = childComponent->getEntity();

					PersonSpawnPointComponent* personSpawnPointComponent = childEntity.getComponent<PersonSpawnPointComponent>();
					if (nullptr != personSpawnPointComponent)
					{
						qsf::Entity* spawnedEntity = personSpawnPointComponent->spawnRandom();
						if (spawnedEntity != nullptr)
						{
							HealthHelper healthHelper(*spawnedEntity);
							if (mTrappedPersonWithPlague)
							{
								healthHelper.injurePersonByPlague(this);
							}
							else
							{
								healthHelper.injurePersonBySmoke(this);
							}
						}
					}
				}
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
