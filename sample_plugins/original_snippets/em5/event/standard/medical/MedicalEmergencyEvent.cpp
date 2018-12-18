// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/medical/MedicalEmergencyEvent.h"
#include "em5/event/EventHelper.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/component/spawnpoint/UnitSpawnPointComponent.h"
#include "em5/freeplay/objective/ObjectiveHelper.h"
#include "em5/game/Game.h"
#include "em5/health/HealthSystem.h"
#include "em5/health/HealthComponent.h"
#include "em5/logic/ObserverHelper.h"
#include "em5/logic/HintHelper.h"
#include "em5/logic/observer/PersonDiedObserver.h"
#include "em5/logic/observer/hint/HintLowEnergyObserver.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/EM5Helper.h"

#include <qsf_game/component/event/EventTagManagerComponent.h>
#include <qsf_game/timer/GameTimerManager.h>

#include <qsf_compositing/component/TintableMeshComponent.h>

#include <qsf/audio/component/DynamicMusicCompositorComponent.h>
#include <qsf/math/Random.h>
#include <qsf/log/LogSystem.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Local helper functions                                ]
	//[-------------------------------------------------------]
	namespace detail
	{

		// TODO(fw): This is just copied from "GangsterEvent" base class, should be refactored
		inline qsf::Entity* getOrSpawnEntity(qsf::Entity& entity)
		{
			// Given entity is possibly a spawn point
			SpawnPointComponent* spawnPointComponent = entity.getComponent<SpawnPointComponent>();
			if (nullptr != spawnPointComponent)
			{
				qsf::Entity* spawnedEntity = spawnPointComponent->spawnRandom();
				if (nullptr != spawnedEntity)
				{
					// Set random tint color
					qsf::compositing::TintableMeshComponent* tintableMeshComponent = spawnedEntity->getComponent<qsf::compositing::TintableMeshComponent>();
					if (nullptr != tintableMeshComponent)
					{
						tintableMeshComponent->setTintPaletteIndex(qsf::Random::getRandomInt(1, 15));	// Not 0, that's what the first entity probably has already
					}
				}
				return spawnedEntity;
			}
			else
			{
				return &entity;
			}
		}

	}


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 MedicalEmergencyEvent::FREEPLAY_EVENT_ID = qsf::StringHash("em5::MedicalEmergencyEvent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	MedicalEmergencyEvent::MedicalEmergencyEvent() :
		mInjuredEntityId(qsf::getUninitialized<uint64>()),
		mSpawnPointEntityId(qsf::getUninitialized<uint64>()),
		mRescueWithHelicopter(false)
	{
		// Nothing here
	}

	MedicalEmergencyEvent::~MedicalEmergencyEvent()
	{
		// Nothing here
	}

	uint64 MedicalEmergencyEvent::getTargetPersonId() const
	{
		return mInjuredEntityId;
	}

	void MedicalEmergencyEvent::setTargetPerson(const qsf::Entity& entity)
	{
		mInjuredEntityId = entity.getId();
	}

	const std::string& MedicalEmergencyEvent::getInjuryName() const
	{
		return mInjuryName;
	}

	void MedicalEmergencyEvent::setInjuryName(const std::string& injuryName)
	{
		mInjuryName = injuryName;
	}

	float MedicalEmergencyEvent::getRunningDelay() const
	{
		return mRunningDelay.getSeconds();
	}

	void MedicalEmergencyEvent::setRunningDelay(float runningDelay)
	{
		mRunningDelay = qsf::Time::fromSeconds(runningDelay);
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	bool MedicalEmergencyEvent::onStartup()
	{
		if (0.0f == mRunningDelay.getSeconds())
		{
			// Event is now running
			setRunning();
		}
		else
		{
			// Set an empty event ID component
			qsf::Entity* personEntity = getMap().getEntityById(getTargetPersonId());
			EventIdComponent* eventIdComponent = personEntity->getOrCreateComponent<EventIdComponent>();
			eventIdComponent->setEvent(*this, eventspreadreason::NO_REASON);

			// Start timer for delayed startup
			qsf::MessageConfiguration message(Messages::EM5_EVENT_TIMER_SIGNAL, getId());
			mMessageProxy.registerAt(message, boost::bind(&MedicalEmergencyEvent::delayedStartup, this, _1));
			EM5_GAMETIMERS.addTimer(message, mRunningDelay);
		}

		// Done
		return true;
	}

	void MedicalEmergencyEvent::onShutdown()
	{
		// Remove event ID component from spawnpoint
		qsf::Entity* entity = getMap().getEntityById(mSpawnPointEntityId);
		if (nullptr != entity)
		{
			entity->destroyComponent<EventIdComponent>();
		}

		if (!mEventLayer.empty())
		{
			deactivateLayer(mEventLayer);
		}

		mMessageProxy.unregister();
	}

	void MedicalEmergencyEvent::onRun()
	{
		// No null pointer check in here. The factory already checked these pointers.
		// Get the person to injure
		qsf::Entity* personEntity = getMap().getEntityById(mInjuredEntityId);

		// Injure the person
		personEntity->getComponent<HealthComponent>()->injurePersonByEventById(mInjuryName, this);

		// Add additional objectives if person must be rescued
		if (mRescueWithHelicopter)
		{
			EventHelper(*this).makeRescuablePerson(*personEntity);
		}

		// Set all observers and objectives for the first target
		startObjectives(*personEntity);

		// Set start hint observers after the event entity was added
		startHintObservers(*personEntity);

		// Show supervisor message
		showSupervisorMessage(HintHelper::getRandomStringOfIdString(HintHelper(*this).getHintParameters().mSupervisorMessageIds));

		// Set location entity
		createEventLocationEntity(*personEntity);

		// Activate event layer
		if (!mEventLayer.empty())
		{
			activateLayer(mEventLayer);
		}

		// More persons to be injured?
		if (!mSecondEventTag.empty())
		{
			const auto& list = qsf::game::EventTagManagerComponent::getEventTagComponentsByTag(qsf::StringHash(mSecondEventTag), QSF_MAINMAP);
			for (qsf::game::EventTagComponent* eventTagComponent : list)
			{
				qsf::Entity* entity = detail::getOrSpawnEntity(eventTagComponent->getEntity());
				if (nullptr != entity)
				{
					// Injure the poor guy
					entity->getOrCreateComponentSafe<HealthComponent>().injurePersonByEventById(mInjuryName, this);
				}
			}
		}
	}

	bool MedicalEmergencyEvent::onFailure(EventResult& eventResult)
	{
		// Don't despawn the dead person. Let Cleanup them remove.

		// Call base implementation
		return FreeplayEvent::onFailure(eventResult);
	}

	void MedicalEmergencyEvent::updateFreeplayEvent(const qsf::Time& timePassed)
	{
		// Check objectives for success or failure
		checkObjectivesState();
	}

	bool MedicalEmergencyEvent::addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason)
	{
		ObserverHelper observerHelper(*this);

		if (eventspreadreason::INJURY == eventSpreadReason)
		{
			// Get or create fail conditions
			Objective& failConditionPersonDiedObjective = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_FAIL_DEADPERSONS);

			// Create and connect observers
			PersonDiedObserver& observerDied = observerHelper.createObserver<PersonDiedObserver>(targetEntity.getId());
			observerDied.connectToObjective(failConditionPersonDiedObjective);
		}

		// Call base implementation
		return FreeplayEvent::addEntityToEvent(targetEntity, eventSpreadReason, newReason);
	}

	void MedicalEmergencyEvent::hintCallback(Observer& hintObserver)
	{
		showHint(hintObserver.getName(), false);

		// Music
		if (mHintLowLifeEnergyName == hintObserver.getName() && getDynamicMusicCompositor().getMusicLevel() != 3)
		{
			// Play short action music
			getDynamicMusicCompositor().changeMusicTrack();
			getDynamicMusicCompositor().setMusicLevel(3);
		}

		if (mHintLowHealthEnergyName == hintObserver.getName() && getDynamicMusicCompositor().getMusicLevel() != 3)
		{
			// Play short action music
			getDynamicMusicCompositor().changeMusicTrack();
			getDynamicMusicCompositor().setMusicLevel(3);
		}
	}

	const qsf::Entity* MedicalEmergencyEvent::getFocusEntity()
	{
		{ // Is the first person still injured?
			qsf::Entity* candidateEntity = getMap().getEntityById(mInjuredEntityId);
			if (nullptr != candidateEntity && EntityHelper(*candidateEntity).isPersonInjured())
			{
				return candidateEntity;
			}
		}

		ObjectiveHelper objectiveHelper(*this);

		{ // Is there a person to treat?
			const Objective* treatPersons = objectiveHelper.getObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_TREATPERSONS);
			if (nullptr != treatPersons)
			{
				const qsf::Entity* candidateEntity = getMap().getEntityById(treatPersons->getRandomNeededEntityId());
				if (nullptr != candidateEntity)
				{
					return candidateEntity;
				}
			}
		}

		{ // Is there an entity to heal?
			const Objective* healPersons = objectiveHelper.getObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_HEALPERSONS);
			if (nullptr != healPersons)
			{
				const qsf::Entity* candidateEntity = getMap().getEntityById(healPersons->getRandomNeededEntityId());
				if (nullptr != candidateEntity)
				{
					return candidateEntity;
				}
			}
		}

		return getEventLocationEntity();
	}

	bool MedicalEmergencyEvent::checkEventConfiguration()
	{
		// Check for valid injury
		if (nullptr == EM5_HEALTH.getInjuryById(mInjuryName))
		{
			// Injury not found, error
			return false;
		}

		return true;
	}

	void MedicalEmergencyEvent::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base implementation
		FreeplayEvent::serialize(serializer);

		serializer.serialize(mInjuredEntityId);
		serializer.serialize(mInjuryName);
		serializer.serialize(mEventLayer);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void MedicalEmergencyEvent::startObjectives(const qsf::Entity& targetEntity)
	{
		// Create the fail condition
		Objective& failConditionPersonDiedObjective = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_FAIL_DEADPERSONS);
		failConditionPersonDiedObjective.setNeededNumber(1); // Hard scripted value
	}

	void MedicalEmergencyEvent::startHintObservers(const qsf::Entity& targetEntity)
	{
		const HintHelper hintHelper(*this);
		hintHelper.createAndInitializeGeneralHintsAtInjured();

		if (hintHelper.getHintParameters().mHintsAtLowLifeEnergy != "off")
		{
			// Create hint on low life energy: Injured person has only X% lifeenergy
			float hintLowLifeThreshold = hintHelper.getHintParameters().mHintsAtLowLifeEnergy_Threshold;
			mHintLowLifeEnergyName = HintHelper::getRandomStringOfIdString(hintHelper.getHintParameters().mHintsAtLowLifeEnergy);
			createObserver<HintLowEnergyObserver>(mInjuredEntityId, mHintLowLifeEnergyName).initialize(hintLowLifeThreshold, HintLowEnergyObserver::HINTENERGYTYPE_LIFE);
		}

		if (hintHelper.getHintParameters().mHintsAtLowHealthEnergy != "off")
		{
			// Create hint on low health energy: Injured person has only X% healthenergy
			float hintLowHealthThreshold = hintHelper.getHintParameters().mHintsAtLowHealthEnergy_Threshold;
			mHintLowHealthEnergyName = HintHelper::getRandomStringOfIdString(hintHelper.getHintParameters().mHintsAtLowHealthEnergy);
			createObserver<HintLowEnergyObserver>(mInjuredEntityId, mHintLowHealthEnergyName).initialize(hintLowHealthThreshold, HintLowEnergyObserver::HINTENERGYTYPE_HEALTH);
		}
	}

	bool MedicalEmergencyEvent::checkCandidate(qsf::Entity* targetEntity)
	{
		if (nullptr == targetEntity)
			return false;

		EntityHelper entityHelper(*targetEntity);

		if (!entityHelper.isCivilPerson())
			return false;

		if (entityHelper.isEntityValidEventTarget())
			return false;

		// Is target hardlinked (e.g picked up by paramedic)
		if (entityHelper.isEntityHardLinked())
			return false;

		// Accepted
		return true;
	}

	void MedicalEmergencyEvent::delayedStartup(const qsf::MessageParameters& parameters)
	{
		qsf::Entity* personEntity = getMap().getEntityById(getTargetPersonId());
		if (checkCandidate(personEntity))
		{
			// Destroy an event component if there is one
			personEntity->destroyComponent<EventIdComponent>();
			setRunning();
		}
		else
		{
			abortEvent();
		}
	}

	void MedicalEmergencyEvent::activateLayer(const std::string& layerName)
	{
		QSF_LOG_PRINTS(DEBUG, "Medical event activates layer '" << layerName << "'");

		// Deactivate the layer first to be sure that the layer is loaded new
		MapHelper(getMap()).activateLayerByName(layerName);
	}

	void MedicalEmergencyEvent::deactivateLayer(const std::string& layerName)
	{
		QSF_LOG_PRINTS(DEBUG, "Medical event deactivates layer '" << layerName << "'");
		MapHelper(getMap()).deactivateLayerByName(layerName);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
