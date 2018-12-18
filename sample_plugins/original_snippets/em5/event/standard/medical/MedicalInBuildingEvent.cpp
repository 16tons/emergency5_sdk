// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/medical/MedicalInBuildingEvent.h"
#include "em5/base/GameplayAssets.h"
#include "em5/base/ContainerCategory.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/component/spawnpoint/civilist/PersonSpawnPointComponent.h"
#include "em5/freeplay/objective/ObjectiveHelper.h"
#include "em5/game/Game.h"
#include "em5/health/HealthHelper.h"
#include "em5/logic/cleanup/LayerCleanup.h"
#include "em5/logic/ObserverHelper.h"
#include "em5/logic/HintHelper.h"
#include "em5/logic/observer/hint/HintMessageGeneralObserver.h"
#include "em5/logic/observer/generic/MessageObserver.h"
#include "em5/logic/observer/PersonDiedObserver.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/EM5Helper.h"

#include <qsf/audio/component/DynamicMusicCompositorComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/log/LogSystem.h>
#include <qsf/map/layer/LayerManager.h>


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
	const qsf::Time PERSON_BALCONY_HINT_01_WAIT_TIME = qsf::Time::fromSeconds(60.0f);


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 MedicalInBuildingEvent::FREEPLAY_EVENT_ID = qsf::StringHash("em5::MedicalInBuildingEvent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	MedicalInBuildingEvent::MedicalInBuildingEvent() :
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mDeadPersonsToFail(0),
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
		mFailAmountInfectedPersons(2),
		mHint01WaitTime(qsf::Time::fromSeconds(60.0f))
	{
		// Nothing here
	}

	MedicalInBuildingEvent::~MedicalInBuildingEvent()
	{
		// Nothing here
	}

	uint64 MedicalInBuildingEvent::getTargetEntityId() const
	{
		return mTargetEntityId;
	}

	void MedicalInBuildingEvent::setTargetEntity(const qsf::Entity& entity)
	{
		mTargetEntityId = entity.getId();
	}

	uint32 MedicalInBuildingEvent::getDeadPersonsToFail() const
	{
		return mDeadPersonsToFail;
	}

	void MedicalInBuildingEvent::setDeadPersonsToFail(uint32 personsToFail)
	{
		mDeadPersonsToFail = personsToFail;
	}

	void MedicalInBuildingEvent::setAmbientLayerName(const std::string& layerName)
	{
		mAmbientLayerName = layerName;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	bool MedicalInBuildingEvent::onStartup()
	{
		// Event is now running
		setRunning();

		// Done
		return true;
	}

	void MedicalInBuildingEvent::onShutdown()
	{
		// Ensure everything from the event is cleaned up
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

	void MedicalInBuildingEvent::onRun()
	{
		// No null pointer check in here. The factory already checked these pointers.
		qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);

		if (mPersonsInside)
		{
			executeFireWithPersonsInside(*targetEntity);
		}
		else if (mPersonsAtWindow)
		{
			executeFireWithPersonAtWindow(*targetEntity);
		}

		spawnPersonsFromConnectedSpawnPoints(*targetEntity);

		// Activate ambient layer
		if (!mAmbientLayerName.empty())
		{
			MapHelper(getMap()).activateLayerByName(mAmbientLayerName, true);
		}

		// Set all observers and objectives for the first target
		if (mDeadPersonsToFail > 0)
		{
			ObjectiveHelper(*this).addDeadCivilPersonsFailCondition(mDeadPersonsToFail);
		}

		// Set start hint observers after the event entity was added
		startHintObservers(*targetEntity);

		// Show supervisor message
		showSupervisorMessage(HintHelper::getRandomStringOfIdString(HintHelper(*this).getHintParameters().mSupervisorMessageIds));

		createEventLocationEntity(*targetEntity);

		// Added the number of infected persons at start. So we can easier check against this value;
		mFailAmountInfectedPersons += getNumberInfectedPersons();
	}

	void MedicalInBuildingEvent::updateFreeplayEvent(const qsf::Time& timePassed)
	{
		// Check objectives for success or failure
		checkObjectivesState();

		if (getNumberInfectedPersons() > mFailAmountInfectedPersons)
		{
			setFinished(false);
		}
	}

	bool MedicalInBuildingEvent::addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason)
	{
		// Call base implementation
		return FreeplayEvent::addEntityToEvent(targetEntity, eventSpreadReason, newReason);
	}

	void MedicalInBuildingEvent::hintCallback(Observer& hintObserver)
	{
		showHint(hintObserver.getName(), false);

		// Play short action music
		getDynamicMusicCompositor().setMusicLevel(3);
	}

	const qsf::Entity* MedicalInBuildingEvent::getFocusEntity()
	{
		const qsf::Map& map = getMap();
		const ObjectiveHelper objectiveHelper(*this);

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

		{ // Are there any persons to test?
			Objective* objective = objectiveHelper.getObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_QUICKTESTPERSONS);
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

	bool MedicalInBuildingEvent::checkEventConfiguration()
	{
		// TODO(mk) Add Check if event has right configuration an can start now

		return true;
	}

	void MedicalInBuildingEvent::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base implementation
		FreeplayEvent::serialize(serializer);

		serializer.serialize(mTargetEntityId);
		serializer.serialize(mDeadPersonsToFail);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void MedicalInBuildingEvent::startHintObservers(const qsf::Entity& targetEntity)
	{
		const HintHelper hintHelper(*this);
		hintHelper.createAndInitializeGeneralHintsAtInjured();
		hintHelper.createAndInitializeGeneralHintAtDamagedVehicle();

		if (mPersonsInside)
		{
			if (mBlockedDoor)
			{
				// Firefighter has not started to break the door after X sec
				createGeneralObserver<HintMessageGeneralObserver>("EM5X_HOSTEL_UNITHINT01").initialize(
					qsf::MessageConfiguration(Messages::EM5_ACTION_BREAK_BUILDING_DOOR_OPEN_START, mTargetEntityId), mHint01WaitTime, false);
			}
		}
	}

	bool MedicalInBuildingEvent::onFailure(EventResult& eventResult)
	{
		const uint32 failObjective = getFailReasonObjectiveTypeId(eventResult.mObjectives);
		if (ObjectiveHelper::OBJECTIVE_FAIL_DEADPERSONS == failObjective)
		{
			setFailHintsForMiniEvent(eventResult, "EM5_GE14_FAIL01", "EM5_GE01_SUPERV_FAIL01", "");
			return true;
		}

		if (mFailAmountInfectedPersons > getNumberInfectedPersons())
		{
			setFailHintsForMiniEvent(eventResult, "EM5X_ME100_FAIL02", "EM5X_ME101_SUPERV_FAIL02", "");
			return true;
		}

		// Call base implementation
		return FreeplayEvent::onFailure(eventResult);
	}

	void MedicalInBuildingEvent::executeFireWithPersonAtWindow(qsf::Entity& entity)
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

		// Specific hint if no DLK ordered after X seconds
		createGeneralObserver<HintMessageGeneralObserver>(PERSON_BALCONY_HINT_01).initialize(
			qsf::MessageConfiguration(Messages::EM5_ACTION_LOAD_PERSON_ON_DLK, personEntity->getId()), PERSON_BALCONY_HINT_01_WAIT_TIME, false);
	}

	void MedicalInBuildingEvent::executeFireWithPersonsInside(qsf::Entity& entity)
	{
		BuildingComponent* buildingComponent = entity.getComponent<BuildingComponent>();
		if (nullptr == buildingComponent)
			return;

		// Trap persons inside the building
		buildingComponent->setTrappedPersonsInjuryId(injury::PLAGUE_CONTAMINATION);
		buildingComponent->setupTrappedInjuredPersons(mTrappedHealthyPersonsMin, mTrappedHealthyPersonsMax, mTrappedInjuredPersonsMin, mTrappedInjuredPersonsMax, mBlockedDoor, true, mTrappedPersonsRescueTime);

		mTrappedPersons = buildingComponent->getSetupNumberOfTrappedPersons();
		mTrappedInjuredPersons = buildingComponent->getSetupNumberOfTrappedInjuredPersons();

		buildingComponent->createTrappedPerson(*this);
	}

	void MedicalInBuildingEvent::spawnPersonsFromConnectedSpawnPoints(qsf::Entity& buildingEntity)
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
					if (nullptr != personSpawnPointComponent && !personSpawnPointComponent->isActive())	// Use only inactive spawnpoints, not the building doors, only the extra edited ones!
					{
						qsf::Entity* spawnedEntity = personSpawnPointComponent->spawnRandom();
						if (spawnedEntity != nullptr)
						{
							HealthHelper healthHelper(*spawnedEntity);
							if (mTrappedPersonWithPlague)
							{
								healthHelper.injurePersonByPlague(this, false);
							}
							else
							{
								healthHelper.injurePersonBySmoke(this, false);
							}
						}
					}
				}
			}
		}
	}

	int MedicalInBuildingEvent::getNumberInfectedPersons()
	{
		// Check objectives with the number of protester
		ObjectiveHelper objectiveHelper(*this);
		Objective* quicktestObjective = objectiveHelper.getObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_QUICKTESTPERSONS);
		if (nullptr != quicktestObjective)
		{
			return quicktestObjective->getNeededNumber();
		}

		// Error
		return 0;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
