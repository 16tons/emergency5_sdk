// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "game_plugin/PrecompiledHeader.h"
#include "game_plugin/event/MedicalCheerEvent.h"
#include "game_plugin/logic/observer/CheeringObserver.h"

#include <em5/event/EventHelper.h>
#include <em5/component/event/EventIdComponent.h>
#include <em5/component/spawnpoint/SpawnPointComponent.h>
#include <em5/freeplay/objective/ObjectiveHelper.h>
#include <em5/map/MapHelper.h>
#include <em5/map/EntityHelper.h>
#include <em5/game/Game.h>
#include <em5/health/HealthSystem.h>
#include <em5/health/HealthComponent.h>
#include <em5/logic/ObserverHelper.h>
#include <em5/logic/HintHelper.h>
#include <em5/logic/observer/PersonDiedObserver.h>
#include <em5/logic/observer/TreatPersonObserver.h>
#include <em5/logic/observer/hint/HintLowEnergyObserver.h>
#include <em5/plugin/Messages.h>
#include <em5/EM5Helper.h>

#include <qsf_game/component/event/EventTagManagerComponent.h>
#include <qsf_game/timer/GameTimerManager.h>

#include <qsf_compositing/component/TintableMeshComponent.h>

#include <qsf/math/Random.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/action/WaitAction.h>
#include <qsf/log/LogSystem.h>
#include <qsf/audio/SoundTrackManager.h>
#include <qsf/audio/component/DynamicMusicCompositorComponent.h>
#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>
#include <qsf/serialization/binary/BasicTypeSerialization.h>
#include <qsf/serialization/binary/StlTypeSerialization.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace user
{

	//[-------------------------------------------------------]
	//[ Local helper functions                                ]
	//[-------------------------------------------------------]
	namespace detail
	{


		inline qsf::Entity* getOrSpawnEntity(qsf::Entity& entity)
		{
			// Given entity is possibly a spawn point
			em5::SpawnPointComponent* spawnPointComponent = entity.getComponent<em5::SpawnPointComponent>();
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
	const uint32 MedicalCheerEvent::FREEPLAY_EVENT_ID = qsf::StringHash("user::MedicalCheerEvent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	MedicalCheerEvent::MedicalCheerEvent() :
		mInjuredEntityId(qsf::getUninitialized<uint64>()),
		mRescueWithHelicopter(false),
		mSpawnPointEntityId(qsf::getUninitialized<uint64>())
	{
		// Nothing here
	}

	MedicalCheerEvent::~MedicalCheerEvent()
	{
		// Nothing here
	}

	uint64 MedicalCheerEvent::getTargetPersonId() const
	{
		return mInjuredEntityId;
	}

	void MedicalCheerEvent::setTargetPerson(qsf::Entity& entity)
	{
		mInjuredEntityId = entity.getId();
	}

	const std::string& MedicalCheerEvent::getInjuryName() const
	{
		return mInjuryName;
	}

	void MedicalCheerEvent::setInjuryName(const std::string& injuryName)
	{
		mInjuryName = injuryName;
	}

	float MedicalCheerEvent::getRunningDelay() const
	{
		return mRunningDelay.getSeconds();
	}

	void MedicalCheerEvent::setRunningDelay(float runningDelay)
	{
		mRunningDelay = qsf::Time::fromSeconds(runningDelay);
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	bool MedicalCheerEvent::onStartup()
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
			em5::EventIdComponent* eventIdComponent = personEntity->getOrCreateComponent<em5::EventIdComponent>();
			eventIdComponent->setEvent(*this, em5::eventspreadreason::NO_REASON);

			// Start timer for delayed startup
			qsf::MessageConfiguration message(em5::Messages::EM5_EVENT_TIMER_SIGNAL, getId());
			mMessageProxy.registerAt(message, boost::bind(&MedicalCheerEvent::delayedStartup, this, _1));
			EM5_GAMETIMERS.addTimer(message, mRunningDelay);
		}

		// Done
		return true;
	}

	void MedicalCheerEvent::onShutdown()
	{
		// Remove event ID component from spawnpoint
		qsf::Entity* entity = getMap().getEntityById(mSpawnPointEntityId);
		if (nullptr != entity)
		{
			entity->destroyComponent<em5::EventIdComponent>();
		}

		if (!mEventLayer.empty())
		{
			deactivateLayer(mEventLayer);
		}
	}

	void MedicalCheerEvent::onRun()
	{
		// No null pointer check in here. The factory already checked these pointers.
		// Get the person to injure
		qsf::Entity* personEntity = getMap().getEntityById(mInjuredEntityId);

		// Injure the person
		personEntity->getComponent<em5::HealthComponent>()->injurePersonByEventById(mInjuryName, this);

		// Add additional objectives if person must be rescued
		if (mRescueWithHelicopter)
		{
			em5::EventHelper(*this).makeRescuablePerson(*personEntity);
		}

		// Set all observers and objectives for the first target
		startObjectives(*personEntity);

		// Set start hint observers after the event entity was added
		startHintObservers(*personEntity);

		// Show supervisor message
		em5::HintHelper::showSupervisorMessage(em5::HintHelper::getRandomStringOfIdString(em5::HintHelper(*this).getHintParameters().mSupervisorMessageIds));

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
					entity->getOrCreateComponentSafe<em5::HealthComponent>().injurePersonByEventById(mInjuryName, this);
				}
			}
		}

		// Add cheering objective when person is treated
		mTreatedPersonMessageProxy.registerAt(qsf::MessageConfiguration(em5::Messages::EM5_HEALTH_TREATED_PERSON, personEntity->getId()), boost::bind(&MedicalCheerEvent::onTreatedPerson, this, _1));
	}

	bool MedicalCheerEvent::onFailure(EventResult& eventResult)
	{
		// Don't despawn the dead person. Let Cleanup them remove.

		// Call base implementation
		return FreeplayEvent::onFailure(eventResult);
	}

	void MedicalCheerEvent::updateFreeplayEvent(const qsf::Time& timePassed)
	{
		// Check objectives for success or failure
		checkObjectivesState();
	}

	bool MedicalCheerEvent::addEntityToEvent(qsf::Entity& targetEntity, em5::eventspreadreason::Reason eventSpreadReason, bool newReason)
	{
		em5::ObserverHelper observerHelper(*this);

		if (em5::eventspreadreason::INJURY == eventSpreadReason)
		{
			// Get or create fail conditions
			em5::Objective& failConditionPersonDiedObjective = em5::ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(em5::ObjectiveHelper::OBJECTIVE_FAIL_DEADPERSONS);

			// Create and connect observers
			em5::PersonDiedObserver& observerDied = observerHelper.createObserver<em5::PersonDiedObserver>(targetEntity.getId());
			observerDied.connectToObjective(failConditionPersonDiedObjective);
		}

		// Call base implementation
		return FreeplayEvent::addEntityToEvent(targetEntity, eventSpreadReason, newReason);
	}

	void MedicalCheerEvent::hintCallback(em5::Observer& hintObserver)
	{
		em5::HintHelper::showHint(hintObserver.getName());

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

	const qsf::Entity* MedicalCheerEvent::getFocusEntity()
	{
		{ // Is the first person still injured?
			qsf::Entity* candidateEntity = getMap().getEntityById(mInjuredEntityId);
			if (nullptr != candidateEntity && em5::EntityHelper(*candidateEntity).isPersonInjured())
			{
				return candidateEntity;
			}
		}

		em5::ObjectiveHelper objectiveHelper(*this);

		{ // Is there a person to treat?
			const em5::Objective* treatPersons = objectiveHelper.getObjectiveByTypeId(em5::ObjectiveHelper::OBJECTIVE_NEED_TREATPERSONS);
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
			const em5::Objective* healPersons = objectiveHelper.getObjectiveByTypeId(em5::ObjectiveHelper::OBJECTIVE_NEED_HEALPERSONS);
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

	bool MedicalCheerEvent::checkEventConfiguration()
	{
		// Check for valid injury
		if (nullptr == EM5_HEALTH.getInjuryById(mInjuryName))
		{
			// Injury not found, error
			return false;
		}

		return true;
	}

	void MedicalCheerEvent::serialize(qsf::BinarySerializer& serializer)
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
	void MedicalCheerEvent::startObjectives(const qsf::Entity& targetEntity)
	{
		// Create the fail condition
		em5::Objective& failConditionPersonDiedObjective = em5::ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(em5::ObjectiveHelper::OBJECTIVE_FAIL_DEADPERSONS);
		failConditionPersonDiedObjective.setNeededNumber(1); // Hard scripted value
	}

	void MedicalCheerEvent::startHintObservers(const qsf::Entity& targetEntity)
	{
		const em5::HintHelper hintHelper(*this);
		hintHelper.createAndInitializeGeneralHintsAtInjured();

		if (hintHelper.getHintParameters().mHintsAtLowLifeEnergy != "off")
		{
			// Create hint on low life energy: Injured person has only X% lifeenergy
			float hintLowLifeThreshold = hintHelper.getHintParameters().mHintsAtLowLifeEnergy_Threshold;
			mHintLowLifeEnergyName = em5::HintHelper::getRandomStringOfIdString(hintHelper.getHintParameters().mHintsAtLowLifeEnergy);
			createObserver<em5::HintLowEnergyObserver>(mInjuredEntityId, mHintLowLifeEnergyName).initialize(hintLowLifeThreshold, em5::HintLowEnergyObserver::HINTENERGYTYPE_LIFE);
		}

		if (hintHelper.getHintParameters().mHintsAtLowHealthEnergy != "off")
		{
			// Create hint on low health energy: Injured person has only X% healthenergy
			float hintLowHealthThreshold = hintHelper.getHintParameters().mHintsAtLowHealthEnergy_Threshold;
			mHintLowHealthEnergyName = em5::HintHelper::getRandomStringOfIdString(hintHelper.getHintParameters().mHintsAtLowHealthEnergy);
			createObserver<em5::HintLowEnergyObserver>(mInjuredEntityId, mHintLowHealthEnergyName).initialize(hintLowHealthThreshold, em5::HintLowEnergyObserver::HINTENERGYTYPE_HEALTH);
		}
	}

	bool MedicalCheerEvent::checkCandidate(qsf::Entity* targetEntity)
	{
		if (nullptr == targetEntity)
			return false;

		em5::EntityHelper entityHelper(*targetEntity);

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

	void MedicalCheerEvent::delayedStartup(const qsf::MessageParameters& parameters)
	{
		qsf::Entity* personEntity = getMap().getEntityById(getTargetPersonId());
		if (checkCandidate(personEntity))
		{
			// Destroy an event component if there is one
			personEntity->destroyComponent<em5::EventIdComponent>();
			setRunning();
		}
		else
		{
			abortEvent();
		}
	}

	void MedicalCheerEvent::activateLayer(const std::string& layerName)
	{
		QSF_LOG_PRINTS(DEBUG, "Medical event activates layer '" << layerName << "'");

		// Deactivate the layer first to be sure that the layer is loaded new
		em5::MapHelper(getMap()).activateLayerByName(layerName);
	}

	void MedicalCheerEvent::deactivateLayer(const std::string& layerName)
	{
		QSF_LOG_PRINTS(DEBUG, "Medical event deactivates layer '" << layerName << "'");
		em5::MapHelper(getMap()).deactivateLayerByName(layerName);
	}

	void MedicalCheerEvent::onTreatedPerson(const qsf::MessageParameters& parameters)
	{
		// Add the cheering observer
		em5::Objective& cheeringObjective = getObjectives().getOrCreateObjective(qsf::StringHash("Cheering"), em5::Objective::OBJECTIVETYPE_REQUIRED, getId());

		cheeringObjective.setText("Let the doctor cheer!!!");
		cheeringObjective.setNeededNumber(1);

		CheeringObserver& cheeringObserver = em5::ObserverHelper(*this).createGeneralObserver<CheeringObserver>();
		cheeringObserver.connectToObjective(cheeringObjective);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // user
