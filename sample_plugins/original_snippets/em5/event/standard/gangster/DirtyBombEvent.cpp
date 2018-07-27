// Copyright (C) 2012-2014 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/gangster/DirtyBombEvent.h"
#include "em5/event/EventHelper.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/command/drone/SEKDroneSearchCommand.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/component/objects/ContaminationComponent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/fire/component/FireComponent.h"
#include "em5/freeplay/objective/ObjectiveHelper.h"
#include "em5/health/HealthComponent.h"
#include "em5/health/HealthHelper.h"
#include "em5/logic/local/gangsters/GangsterCivilArmedLogic.h"
#include "em5/logic/ObserverHelper.h"
#include "em5/logic/observer/hint/HintMessageGeneralObserver.h"
#include "em5/logic/observer/PersonDiedObserver.h"
#include "em5/logic/observer/GangsterEscapedObserver.h"
#include "em5/logic/observer/TransportedToHqObserver.h"
#include "em5/logic/HintHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/plugin/Messages.h"

#include <qsf_game/component/base/HiddenComponent.h>

#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/map/Map.h>
#include <qsf/math/Random.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/plugin/QsfJobs.h>
#include <qsf/renderer/component/RendererComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Local definitions                                     ]
	//[-------------------------------------------------------]
	// Event hints
	const std::string TERRORISTS_HINT_01 = "EM5_EVENT_TERRORISTS_HINT_01";
	const std::string TERRORISTS_HINT_02 = "EM5_EVENT_TERRORISTS_HINT_02";
	const std::string TERRORISTS_HINT_03 = "EM5_EVENT_TERRORISTS_HINT_03";
	const std::string TERRORISTS_HINT_04 = "EM5_EVENT_TERRORISTS_HINT_04";


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 DirtyBombEvent::FREEPLAY_EVENT_ID = qsf::StringHash("em5::DirtyBombEvent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	DirtyBombEvent::DirtyBombEvent() :
		mExplosionRadius(120.0f),
		mContaminationRadius(180.0f),
		mBombTargetId(qsf::getUninitialized<uint64>()),
		mTerroristSeekTime(qsf::Time::fromSeconds(180.0f)),
		mHint01WaitTime(qsf::Time::fromSeconds(120.0f)),
		mHint03WaitTime(qsf::Time::fromSeconds(80.0f)),
		mHint04LifeEnergy(0.1f),
		mHint02Triggered(false),
		mHint04Triggered(false)
	{
		// Nothing here
	}

	DirtyBombEvent::~DirtyBombEvent()
	{
		// Nothing here
	}

	std::vector<uint64>& DirtyBombEvent::getTerroristIds()
	{
		return mTerroristIds;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	bool DirtyBombEvent::onStartup()
	{
		// Add bomb target
		qsf::Entity* bombTarget = getMap().getEntityById(mBombTargetId);
		QSF_CHECK(nullptr != bombTarget, "Can't find bomb target entity; DirtyBombEvent method: onStartup", return false);

		// Make sure the bomb is visible and enable its fire component
		qsf::RendererComponent* rendererComponent = bombTarget->getComponent<qsf::RendererComponent>();
		if (nullptr != rendererComponent)
			rendererComponent->setActive(true);

		FireComponent* fireComponent = bombTarget->getComponent<FireComponent>();
		if (nullptr != fireComponent)
			fireComponent->setActive(true);

		EventIdComponent::addToEvent(*bombTarget, *this, eventspreadreason::NO_REASON);

		explodeDirtyBomb(*bombTarget);

		// Create message proxies
		mTerroristFoundMessageProxies.resize(mTerroristIds.size());

		// Add terrorists
		for (size_t index = 0; index < mTerroristIds.size(); ++index)
		{
			qsf::Entity* terroristEntity = getMap().getEntityById(mTerroristIds[index]);
			QSF_CHECK(terroristEntity, "Can't find terrorist entity; DirtyBombEvent method: onStartup", QSF_REACT_NONE);
			if (nullptr == terroristEntity)
				return false;

			// Hide the gangster
			qsf::game::HiddenComponent::setHidden(*terroristEntity, true, qsf::game::HiddenComponent::HIDDENTYPE_DEFAULT);

			// Turn into gangster, use civil armed because gangster should flee from police
			EntityHelper(*terroristEntity).turnIntoGangster("em5::GangsterCivilArmedLogic", false);

			// Set terrorists immune
			HealthComponent* healthComponent = terroristEntity->getComponent<HealthComponent>();
			if (healthComponent != nullptr)
			{
				HealthHelper(*healthComponent).setImmuneToInjurySources(false, true);
			}

			// Set weapons
			GangsterBaseLogic* gangsterBaseLogic = terroristEntity->getOrCreateComponent<qsf::GameLogicComponent>()->getGameLogic<GangsterBaseLogic>();
			gangsterBaseLogic->showWeapon(equipment::GANGSTER_PISTOL);
			GangsterBaseLogic::WeaponFlagSet weapons;
			weapons.set(weapon::FIST);
			weapons.set(weapon::PISTOL);
			gangsterBaseLogic->setEquippedWeaponTypes(weapons);

			// Set the escape target tag
			gangsterBaseLogic->setEscapeTargetTag(mEscapeTargetTag);

			// Add new objectives
			Objective& needFindKillerObjective = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_FINDKILLERWITHSEKDRONE);
			needFindKillerObjective.increaseNeededNumber(terroristEntity);

			Objective& failGangsterNotFoundObjective = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_FAIL_GANGSTERNOTFOUND);
			failGangsterNotFoundObjective.setNeededNumber(1);

			// Register find gangster message proxy (use find hidden person for this)
			qsf::MessageConfiguration message(Messages::EM5_ACTION_FIND_HIDDEN_PERSON_WITH_SEK_DRONE, terroristEntity->getId());
			mTerroristFoundMessageProxies[index].registerAt(message, boost::bind(&DirtyBombEvent::onTerroristFound, this, _1));

			EventIdComponent::addToEvent(*terroristEntity, *this, eventspreadreason::EVENT_SPECIFIC);

			// Add timer
			qsf::jobs::JobConfiguration jobConfiguration;
			jobConfiguration.mTimeBetweenCalls = mTerroristSeekTime;
			mTerroristSeekTimer.registerAt(qsf::QsfJobs::SIMULATION, boost::bind(&DirtyBombEvent::onTerroristSeekFail, this, _1), jobConfiguration);
		}

		setRunning();

		// Done
		return true;
	}

	void DirtyBombEvent::onShutdown()
	{
		// If gangsters are hidden delete them
		for (uint64 entityId : mTerroristIds)
		{
			qsf::Entity* terrorist = getMap().getEntityById(entityId);
			if (nullptr != terrorist && EntityHelper(*terrorist).isEntityHidden())
			{
				MapHelper(getMap()).destroyEntityById(entityId);
			}
		}

		// Unregister message proxies
		for (qsf::MessageProxy& messageProxy : mTerroristFoundMessageProxies)
		{
			messageProxy.unregister();
		}

		// Unregister timer
		mTerroristSeekTimer.unregister();

		// Deactivate contamination component
		qsf::Entity* bombTarget = getMap().getEntityById(mBombTargetId);
		if (nullptr != bombTarget)
		{
			EntityHelper(*bombTarget).cleanupFire();

			ContaminationComponent* contaminationComponent = bombTarget->getComponent<ContaminationComponent>();
			if (nullptr != contaminationComponent)
			{
				contaminationComponent->setActive(false);
			}

			// Hide the bomb entity again and disable its fire component
			qsf::RendererComponent* rendererComponent = bombTarget->getComponent<qsf::RendererComponent>();
			if (nullptr != rendererComponent)
				rendererComponent->setActive(false);

			FireComponent* fireComponent = bombTarget->getComponent<FireComponent>();
			if (nullptr != fireComponent)
				fireComponent->setActive(false);
		}
	}

	void DirtyBombEvent::onRun()
	{
		// Play supervisor message
		showSupervisorMessage(HintHelper::getRandomStringOfIdString(HintHelper(*this).getHintParameters().mSupervisorMessageIds));

		// Activate the hint observers
		startHintObservers();
	}

	bool DirtyBombEvent::onFailure(EventResult& eventResult)
	{
		// Call base implementation
		return FreeplayEvent::onFailure(eventResult);
	}

	void DirtyBombEvent::updateFreeplayEvent(const qsf::Time& timePassed)
	{
		if (mHint04Triggered == false)
		{
			playHint04();
		}

		if (mHint02Triggered == false)
		{
			playHint02();
		}

		checkObjectivesState();
	}

	void DirtyBombEvent::hintCallback(Observer& hintObserver)
	{
		showHint(hintObserver.getName(), false);
	}

	const qsf::Entity* DirtyBombEvent::getFocusEntity()
	{
		qsf::Entity* bombTarget = getMap().getEntityById(mBombTargetId);
		if (nullptr != bombTarget && EntityHelper(*bombTarget).isBurning())
		{
			return bombTarget;
		}

		// Show random event entity
		return EventHelper(*this).getRandomEventEntity();
	}

	bool DirtyBombEvent::checkEventConfiguration()
	{
		// No bomb target
		qsf::Entity* bombTargetEntity = getMap().getEntityById(mBombTargetId);
		if (nullptr == bombTargetEntity)
			return false;

		// No contamination
		ContaminationComponent* contaminationComponent = bombTargetEntity->getComponent<ContaminationComponent>();
		if (nullptr == contaminationComponent)
			return false;

		// No terrorists
		if (mTerroristIds.empty())
			return false;

		for (uint64 entityId : mTerroristIds)
		{
			qsf::Entity* entity = getMap().getEntityById(entityId);
			if (nullptr == entity)
				return false;
		}

		return true;
	}

	void DirtyBombEvent::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base implementation
		FreeplayEvent::serialize(serializer);

		serializer.serialize(mTerroristIds);
		serializer.serialize(mBombTargetId);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void DirtyBombEvent::startHintObservers()
	{
		// hint 01: no sek drone search action started after x sec.
		createGeneralObserver<HintMessageGeneralObserver>(TERRORISTS_HINT_01).initialize(
			qsf::MessageConfiguration(Messages::EM5_POST_COMMAND_EXECUTE, SEKDroneSearchCommand::PLUGINABLE_ID), mHint01WaitTime, false);

		// Show hint 03 only if persons are contaminated
		Objective* objective = getObjectives().getObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_DECONTAMINATEPERSONS);
		if (nullptr != objective && objective->getNeededNumber() > 0)
		{
			// hint 03: no decontamination vehicle after X sec.
			createGeneralObserver<HintMessageGeneralObserver>(TERRORISTS_HINT_03).initialize(
				qsf::MessageConfiguration(Messages::EM5_ORDER_UNIT, CommandableComponent::UNITTAG_FIREFIGHTERS_DEKONP), mHint03WaitTime, false);
		}
	}

	bool DirtyBombEvent::addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason)
	{
		if (eventSpreadReason == eventspreadreason::INJURY)
		{
			Objective& failConditionPersonDiedObjective = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_FAIL_DEADPERSONS);
			failConditionPersonDiedObjective.setNeededNumber(1);
			createObserver<PersonDiedObserver>(targetEntity.getId()).connectToObjective(failConditionPersonDiedObjective);
		}
		return FreeplayEvent::addEntityToEvent(targetEntity, eventSpreadReason, newReason);
	}

	void DirtyBombEvent::explodeDirtyBomb(qsf::Entity& entity)
	{
		// Let the bomb explode
		std::vector<FireComponent*> fireComponentVector;
		std::vector<PersonComponent*> injuredPersonComponentVector;
		std::vector<PersonComponent*> contaminatedPersonComponentVector;
		qsf::Map& map = entity.getMap();

		// Get bomb position
		glm::vec3 position = entity.getComponentSafe<qsf::TransformComponent>().getPosition();

		// Get fire targets and persons in the explosion radius
		qsf::ComponentMapQuery(map).getInstancesInCircle(position, mExplosionRadius, fireComponentVector);
		qsf::ComponentMapQuery(map).getInstancesInCircle(position, mExplosionRadius, injuredPersonComponentVector);
		qsf::ComponentMapQuery(map).getInstancesInCircle(position, mContaminationRadius, contaminatedPersonComponentVector);

		// Start fire
		for (FireComponent* fireComponent : fireComponentVector)
		{
			fireComponent->startFire(this);
		}

		// Contaminate and injure persons
		for (PersonComponent* personComponent : injuredPersonComponentVector)
		{
			if (qsf::Random::getRandomChance(0.5f))
			{
				personComponent->getEntity().getOrCreateComponentSafe<HealthComponent>().injurePersonByEventById(injury::RADIOACTIVE_CONTAMINATION_ORGAN_FAILURE, this);
			}

			else
			{
				personComponent->getEntity().getOrCreateComponentSafe<HealthComponent>().injurePersonByEventById(injury::RADIOACTIVE_CONTAMINATION_ORGAN_BLEEDING, this);
			}
		}

		// Contaminate persons
		for (PersonComponent* personComponent : contaminatedPersonComponentVector)
		{
			// They only get contaminated if they are not injured by now
			HealthHelper(personComponent->getEntity()).injurePersonByContamination(this, Injury::CONTAMINATION_RADIOACTIVE);
		}

		// Start fire and contaimation cloud
		entity.getOrCreateComponentSafe<ContaminationComponent>().setActive(true);
		EntityHelper(entity).startFire(this);

		// Start explosion
		DamageComponent* damageComponent = entity.getComponent<DamageComponent>();
		if (nullptr != damageComponent)
		{
			// Create particle effect. The explosion particle effect must be started after the fire is started, otherwise it will be deactivated by the startFire process.
			damageComponent->showExplosionFireParticles();
		}
	}

	void DirtyBombEvent::onTerroristSeekFail(const qsf::JobArguments& jobArguments)
	{
		ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_FAIL_GANGSTERNOTFOUND).increaseCurrentNumber();

		// Unregister message proxies
		for (qsf::MessageProxy& messageProxy : mTerroristFoundMessageProxies)
		{
			messageProxy.unregister();
		}

		// Unregister timer
		mTerroristSeekTimer.unregister();
	}

	void DirtyBombEvent::onTerroristFound(const qsf::MessageParameters& parameters)
	{
		// Get terrorist ID
		uint64 entityId = parameters.getFilter(1);
		qsf::Entity* terrorist = getMap().getEntityById(entityId);
		QSF_WARN_IF(nullptr == terrorist, "Could not find terrorist for dirty bomb event!", QSF_REACT_NONE);

		if (nullptr == terrorist)
			return;

		// Increase terrorist seek objective
		ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_FINDKILLERWITHSEKDRONE).increaseCurrentNumber(terrorist);

		// Add new objectives
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
			PersonDiedObserver& observerDied = observerHelper.createObserver<PersonDiedObserver>(entityId);
			observerDied.connectToObjective(failConditionPersonDiedObjective);

			TransportedToHqObserver& observerTransported = observerHelper.createObserver<TransportedToHqObserver>(entityId);
			observerTransported.connectToObjective(needConditionArrestPersonObjective);

			GangsterEscapedObserver& observerEscaped = observerHelper.createObserver<GangsterEscapedObserver>(entityId);
			observerEscaped.connectToObjective(failConditionPersonEscapedObjective);
		}

		// Find and unregister the message proxy
		for (size_t index = 0; index < mTerroristIds.size(); ++index)
		{
			if (mTerroristIds[index] == entityId)
			{
				mTerroristFoundMessageProxies[index].unregister();
				break;
			}
		}

		// Unregister the timer if all proxies are unregistered (this means all terrorists are found)
		bool allTerroristsFound = true;
		for (qsf::MessageProxy& messageProxy : mTerroristFoundMessageProxies)
		{
			if (messageProxy.isValid())
			{
				allTerroristsFound = false;
			}
		}

		if (allTerroristsFound)
		{
			mTerroristSeekTimer.unregister();
		}
	}

	void DirtyBombEvent::playHint02()
	{
		for (uint64 entityId : mTerroristIds)
		{
			qsf::Entity* terroristEntity = getMap().getEntityById(entityId);
			if (nullptr == terroristEntity)
				continue;

			if (!EntityHelper(*terroristEntity).isGangsterFleeing())
				continue;

			// Show hint 02
			showHint(TERRORISTS_HINT_02, false);
			mHint02Triggered = true;
			return;
		}
	}

	void DirtyBombEvent::playHint04()
	{
		// Get objective entities for decontamination objective and check if there is one with low energy
		Objective* decontamionateObjective = ObjectiveHelper(*this).getObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_DECONTAMINATEPERSONS);
		if (nullptr != decontamionateObjective)
		{
			for (uint64 entityId : decontamionateObjective->getNeededEntityIdSet())
			{
				qsf::Entity* entity = getMap().getEntityById(entityId);
				if (nullptr != entity)
				{
					if (!EntityHelper(*entity).isPersonContaminated())
						continue;

					if (HealthHelper(*entity).getLifeEnergyPercentage() > mHint04LifeEnergy)
						continue;

					showHint(TERRORISTS_HINT_04, false);
					mHint04Triggered = true;
					break;
				}
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
