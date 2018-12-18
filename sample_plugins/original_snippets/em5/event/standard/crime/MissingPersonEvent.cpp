// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/crime/MissingPersonEvent.h"
#include "em5/event/EventHelper.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/component/objects/DivingSpotComponent.h"
#include "em5/freeplay/objective/ObjectiveHelper.h"
#include "em5/freeplay/eventspread/EventSpreadHelper.h"
#include "em5/game/Game.h"
#include "em5/game/minimap/MinimapHelper.h"
#include "em5/health/HealthComponent.h"
#include "em5/health/HealthSystem.h"
#include "em5/logic/local/gangsters/GangsterBaseLogic.h"
#include "em5/logic/ObserverHelper.h"
#include "em5/logic/observer/PersonDiedObserver.h"
#include "em5/logic/observer/hint/HintMessageGeneralObserver.h"
#include "em5/logic/observer/GangsterEscapedObserver.h"
#include "em5/logic/observer/TransportedToHqObserver.h"
#include "em5/logic/observer/TreatPersonObserver.h"
#include "em5/logic/HintHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/specs/ActionRangeSpecsGroup.h"
#include "em5/plugin/Messages.h"
#include "em5/EM5Helper.h"

#include <qsf_game/component/base/HiddenComponent.h>
#include <qsf_game/timer/GameTimerManager.h>

#include <qsf/base/StringParser.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/math/Random.h>
#include <qsf/plugin/QsfJobs.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Local definitions                                     ]
	//[-------------------------------------------------------]
	namespace
	{
		// Event hints
		const std::string SUPERV_MISSINGPERSON_01	 = "EM5_EVENT_SUPERV_OBJ_MISSINGPERSON_01";
		const std::string SUPERV_MURDER_01			 = "EM5_EVENT_SUPERV_OBJ_MURDER_01";
		const std::string MISSINGPERSON_HINT_01		 = "EM5_EVENT_MISSINGPERSON_HINT_01";
		const std::string MISSINGPERSON_HINT_02		 = "EM5_EVENT_MISSINGPERSON_HINT_02";
		const std::string MISSINGPERSON_HINT_03		 = "EM5X_MISSINGPERSON_DOG_DIVER_01";
		const std::string MISSINGPERSON_FAIL_HINT_01 = "EM5_EVENT_MISSINGPERSON_FAIL_HINT_01";

		// Local messages
		const qsf::StringHash MESSAGE_ID_TIMER("em5::MissingPersonEvent_Timer");
	}


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 MissingPersonEvent::FREEPLAY_EVENT_ID = qsf::StringHash("em5::MissingPersonEvent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	MissingPersonEvent::MissingPersonEvent() :
		mIsPersonFound(false),
		mIsVictimDead(false),
		mIsPersonUnderwater(false),
		mIsKillerDetectedByDrone(false),
		mKillerEntityId(qsf::getUninitialized<uint64>()),
		mDogSearchSpotId(qsf::getUninitialized<uint64>()),
		mHint01WaitTime(qsf::Time::fromSeconds(30.0f)),
		mKillerSeekHint01WaitTime(qsf::Time::fromSeconds(20.0f)),
		mKillerSeekTime(qsf::Time::fromSeconds(180.0f)),
		mIconEntity(nullptr)
	{
		// Nothing here
	}

	MissingPersonEvent::~MissingPersonEvent()
	{
		// Nothing here
	}

	void MissingPersonEvent::init(qsf::Entity& entity, uint64 dogSearchSpotId)
	{
		mMissingPerson = entity;
		mMissingPersonPosition = EntityHelper(entity).getPosition();
		mDogSearchSpotId = dogSearchSpotId;
	}

	const std::string& MissingPersonEvent::getInjuryName() const
	{
		return mInjuryName;
	}

	void MissingPersonEvent::setInjuryName(const std::string& injuryName)
	{
		std::vector<std::string> splittedIds;
		qsf::StringParser::splitString(injuryName, splittedIds);

		if (splittedIds.empty())
		{
			mInjuryName = injuryName;
		}

		mInjuryName = qsf::Random::getAnyOf(splittedIds);
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	bool MissingPersonEvent::onStartup()
	{
		// Get the missing person entity
		QSF_CHECK(mMissingPerson.valid(), "MissingPersonEvent::onStartup(): Can't find person entity", return false);

		// Hide the person
		qsf::game::HiddenComponent::setHidden(*mMissingPerson, true, qsf::game::HiddenComponent::HIDDENTYPE_BLOCKED);

		HealthComponent* healthComponent = mMissingPerson->getComponent<HealthComponent>();
		QSF_CHECK(healthComponent, "MissingPersonEvent::onStartup(): Can't find health component", QSF_REACT_NONE);
		if (nullptr == healthComponent)
			return false;

		// Calculate if the person is dead or injured
		if (mIsVictimDead)
		{
			// Configure the visual injury
			VisualVitalSigns visualVitalSigns;
			visualVitalSigns.setVisualVitalSignBloodPuddle(true);
			visualVitalSigns.setVisualVitalSignBloodCovered(true);

			// Kill the person
			healthComponent->killPerson(this, &visualVitalSigns, injury::GUN_SHOT_WOUND_BODY_HARD);
		}
		else
		{
			// Injure the person. Add additional objectives later, when the person is found
			healthComponent->applyInjuryById(qsf::StringHash(mInjuryName), nullptr);

			// Get or create fail conditions
			Objective& failConditionPersonDiedObjective = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_FAIL_DEADPERSONS);

			// Create and connect observers
			PersonDiedObserver& observerDied = ObserverHelper(*this).createObserver<PersonDiedObserver>(mMissingPerson->getId());
			observerDied.connectToObjective(failConditionPersonDiedObjective);
		}

		// In case we have a dog spot, move the missing person to the position. So the dog will move to this position instead
		mIsPersonUnderwater = false;	// Init
		qsf::Entity* dogSearchSpot = getMap().getEntityById(mDogSearchSpotId);
		if (nullptr != dogSearchSpot)
		{
			glm::vec3 dogPosition = EntityHelper(*dogSearchSpot).getPosition();
			EntityHelper(*mMissingPerson).setPositionForMultiplay(dogPosition);
			mIsPersonUnderwater = true;
		}

		// Event is now running
		setRunning();

		// Done
		return true;
	}

	void MissingPersonEvent::onShutdown()
	{
		mDeadPersonDignosisProxy.unregister();
		mKillerFoundMessageProxy.unregister();
		mRescueDogFindPersonProxy.unregister();
		mFindEntityFromDivingSpotProxy.unregister();
		mGameTimerMessageProxy.unregister();

		// Remove event icon
		removeIconEntity();
	}

	void MissingPersonEvent::onRun()
	{
		// Set all observers and objectives for the target
		startObjectives();

		// Activate the hint observers
		startHintObservers();

		// Play supervisor message
		showSupervisorMessage(SUPERV_MISSINGPERSON_01);

		{ // Create icon for minimap
			// Use the dog search radius as reference for the event radius
			const float searchRange = ActionRangeSpecsGroup::getInstanceSafe().getRescueDogSearchDistance() * 0.5f;

			// Show the area around the missing person, except we have a extra dog search point
			qsf::Entity* iconIndicatorEntity = mMissingPerson.get();
			qsf::Entity* dogSearchSpot = getMap().getEntityById(mDogSearchSpotId);
			if (nullptr != dogSearchSpot)
			{
				iconIndicatorEntity = dogSearchSpot;
			}

			// Create icon entity
			mIconEntity = EventHelper(*this).createEventIconCircleArea(*iconIndicatorEntity, searchRange, searchRange);
			MinimapHelper::registerIcon(*mIconEntity, MiniMapIconType::MISSING_PERSON);
		}

		// Register for game timer messages
		mGameTimerMessageProxy.registerAt(qsf::MessageConfiguration(MESSAGE_ID_TIMER, getId()), boost::bind(&MissingPersonEvent::onGameTimerMessage, this, _1));
	}

	bool MissingPersonEvent::onSuccess(EventResult& eventResult)
	{
		// Call base implementation
		return FreeplayEvent::onSuccess(eventResult);
	}

	bool MissingPersonEvent::onFailure(EventResult& eventResult)
	{
		qsf::Entity* missingPerson = mMissingPerson.get();

		const uint32 failObjectiveId = getFailReasonObjectiveTypeId(eventResult.mObjectives);

		if (nullptr != missingPerson && !mIsVictimDead && missingPerson->getOrCreateComponentSafe<HealthComponent>().isDead())
		{
			// Show Hint FAIL 01 (person is deceased before found)
			showHint(MISSINGPERSON_FAIL_HINT_01, false);
		}
		else if (ObjectiveHelper::OBJECTIVE_FAIL_GANGSTERNOTFOUND == failObjectiveId)
		{
			// Show hint could not find killer
			showHint("EM5_EVENT_MURDER_FAIL_HINT_01", false);
		}
		else if (ObjectiveHelper::OBJECTIVE_FAIL_PERSONSESCAPE == failObjectiveId)
		{
			// Show hint killer escaped
			showHint("EM5_EVENT_GANGSTER_ESCAPING_FAIL_HINT_02", false);
		}
		else if (ObjectiveHelper::OBJECTIVE_FAIL_DEADPERSONS == failObjectiveId)
		{
			showHint(HintHelper::getRandomStringOfIdString("EM5_EVENT_TOO_MANY_PERSONS_DIED_HINT_03, EM5_EVENT_TOO_MANY_PERSONS_DIED_HINT_04"), false);
		}

		// Call base implementation
		return FreeplayEvent::onFailure(eventResult);
	}

	void MissingPersonEvent::updateFreeplayEvent(const qsf::Time&)
	{
		// Early out if person doesn't exist anymore
		if (mMissingPerson.valid())
		{
			// Check if the person is still hidden
			if (!mIsPersonFound)
			{
				if (checkIfMissingPersonIsFound())
				{
					foundMissingPerson();
				}
			}
		}

		// This part should not be necessary, but should fix Hansoft #7821
		//  -> The "treatment" objective was not achieved although the "transported" objective was
		{
			Objective* objectiveTreatPerson = getObjectives().getObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_TREATPERSONS);
			Objective* objectiveTransportPerson = getObjectives().getObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_TRANSPORTINJURED);
			if (nullptr != objectiveTreatPerson && nullptr != objectiveTransportPerson)
			{
				// Transported implies treated
				if (objectiveTransportPerson->checkAccomplished() && !objectiveTreatPerson->checkAccomplished())
				{
					objectiveTreatPerson->setAccomplished();
				}
			}
		}

		// Check objectives for success or failure
		checkObjectivesState();
	}

	void MissingPersonEvent::hintCallback(Observer& hintObserver)
	{
		showHint(hintObserver.getName(), false);
	}

	const qsf::Entity* MissingPersonEvent::getFocusEntity()
	{
		if (!mIsPersonFound)
			return mIconEntity.get();

		qsf::Entity* killerEntity = getMap().getEntityById(mKillerEntityId);
		if (nullptr != killerEntity)
		{
			EntityHelper entityHelper(*killerEntity);
			if (!entityHelper.isEntityInVehicle() && !entityHelper.isEntityHidden())
			{
				return killerEntity;
			}
		}

		if (mMissingPerson.valid())
		{
			EntityHelper entityHelper(*mMissingPerson);
			if (!entityHelper.isEntityHidden() && entityHelper.isEntityInVehicle())
			{
				return mMissingPerson.get();
			}
		}

		return getEventLocationEntity();
	}

	bool MissingPersonEvent::checkEventConfiguration()
	{
		// Check for valid injury
		if (nullptr == EM5_HEALTH.getInjuryById(qsf::StringHash(mInjuryName)))
		{
			// Injury not found, error
			return false;
		}

		return true;
	}

	void MissingPersonEvent::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base implementation
		FreeplayEvent::serialize(serializer);

		serializer.serialize(mIsPersonFound);
		serializer.serialize(mIsVictimDead);
		serializer.serialize(mHint01WaitTime);
		serializer.serialize(mInjuryName);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void MissingPersonEvent::startObjectives()
	{
		// Create the need condition
		Objective& needConditionFindPersonObjective = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_FINDPERSON);
		needConditionFindPersonObjective.setNeededNumber(1); // Hard scripted value

		// Create the fail condition
		Objective& failConditionPersonDiedObjective = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_FAIL_DEADPERSONS);
		failConditionPersonDiedObjective.setNeededNumber((mIsVictimDead ? 2 : 1)); // Hard scripted value
	}

	void MissingPersonEvent::startHintObservers()
	{
		// hint 01: no rescue dog car unit after X sec.
		createGeneralObserver<HintMessageGeneralObserver>(MISSINGPERSON_HINT_01).initialize(
			qsf::MessageConfiguration(Messages::EM5_ORDER_UNIT, CommandableComponent::UNITTAG_AMBULANCE_RESCUEDOGCAR), mHint01WaitTime, false);

		mRescueDogFindPersonProxy.registerAt(qsf::MessageConfiguration(
			Messages::EM5_RESCUE_DOG_FOUND_PERSON), boost::bind(&MissingPersonEvent::onRescueDogFindPerson, this, _1));
	}

	void MissingPersonEvent::onGameTimerMessage(const qsf::MessageParameters& parameters)
	{
		const uint32 timerId = parameters.getFilter<uint32>(2);
		switch (timerId)
		{
			case 0:
			{
				if (!mIsKillerDetectedByDrone)
				{
					ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_FAIL_GANGSTERNOTFOUND).increaseCurrentNumber();
					mKillerFoundMessageProxy.unregister();
				}
				break;
			}
		}
	}

	void MissingPersonEvent::foundMissingPerson()
	{
		mIsPersonFound = true;

		// Update objectives
		const ObjectiveHelper objectiveHelper(*this);
		objectiveHelper.getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_FINDPERSON).increaseCurrentNumber();
		if (mIsVictimDead)
		{
			// Register finish diagnosis message proxy
			qsf::MessageConfiguration message(Messages::EM5_HEALTH_TREATED_PERSON, mMissingPerson->getId());
			mDeadPersonDignosisProxy.registerAt(message, boost::bind(&MissingPersonEvent::onDiagnosisDead, this, _1));

			// Add manually treat and transport objectives and observer for dead person
			if (mMissingPerson.valid())
			{
				// Get or create objective
				Objective& objectiveTreatPerson = objectiveHelper.getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_TREATPERSONS);
				objectiveTreatPerson.increaseNeededNumber(mMissingPerson.get());

				// Get or create objective
				Objective& objectiveTransportPerson = objectiveHelper.getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_TRANSPORTINJURED);
				objectiveTransportPerson.increaseNeededNumber(mMissingPerson.get());

				// Create observer
				const ObserverHelper observerHelper(*this);
				TransportedToHqObserver& observerTansported = observerHelper.createObserver<TransportedToHqObserver>(mMissingPerson->getId());
				observerTansported.connectToObjective(objectiveTransportPerson);

				TreatPersonObserver& observerTreat = observerHelper.createObserver<TreatPersonObserver>(mMissingPerson->getId());
				observerTreat.connectToObjective(objectiveTreatPerson);
			}
		}
		else
		{
			// Add injury objectives
			eventspreadreason::Reason reason = eventspreadreason::INJURY;
			EventSpreadHelper::setEventIdComponent(this, *mMissingPerson, reason);

			// Show hint 02 (person is found and injured)
			showHint(MISSINGPERSON_HINT_02, false);
		}

		createEventLocationEntity(*mMissingPerson);

		// Remove event icon
		removeIconEntity();
	}

	bool MissingPersonEvent::checkIfMissingPersonIsFound()
	{
		if (!mMissingPerson.valid())
			return false;

		return !mIsPersonUnderwater && !qsf::game::HiddenComponent::isHidden(*mMissingPerson);
	}

	void MissingPersonEvent::removeIconEntity()
	{
		// Destroy the icon
		if (mIconEntity.valid())
		{
			MinimapHelper::unregisterIcon(*mIconEntity);
			EventHelper(*this).removeEventIcon(*mIconEntity);
			mIconEntity = nullptr;
		}
	}

	void MissingPersonEvent::startKillerSeeking()
	{
		// Find killer
		qsf::Entity* killerEntity = MapHelper(getMap()).getOrCreateGangsterCandidateInGangsterEventArea(GangsterPool::DEFAULT, mMurderTag);
		QSF_WARN_IF(nullptr == killerEntity, "MissingPersonEvent::startKillerSeeking(): Could not find killer for killer seeking event!", QSF_REACT_NONE);
		if (nullptr == killerEntity)
			return;

		// Save the killer entity ID
		mKillerEntityId = killerEntity->getId();

		// Turn into gangster, use civil armed because gangster should flee from police
		EntityHelper(*killerEntity).turnIntoGangsterByType("MissingPersonMurder");

		GangsterBaseLogic* gangsterBaseLogic = killerEntity->getOrCreateComponent<qsf::GameLogicComponent>()->getGameLogic<GangsterBaseLogic>();
		if (nullptr != gangsterBaseLogic)
		{
			// Set the escape target tag
			gangsterBaseLogic->setEscapeTargetTag(mEscapeTargetTag);
		}

		// Set the escape target tag
		gangsterBaseLogic->setEscapeTargetTag(mEscapeTargetTag);

		// Hide the gangster
		qsf::game::HiddenComponent::setHidden(*killerEntity, true, qsf::game::HiddenComponent::HIDDENTYPE_DEFAULT);

		// Change the current event title
		setEventName("EM5_EVENT_TITLE_48");

		// Play supervisor message
		showSupervisorMessage(SUPERV_MURDER_01);

		// Add new Objectives
		Objective& needFindKillerObjective = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_FINDKILLERWITHSEKDRONE);
		needFindKillerObjective.increaseNeededNumber(killerEntity);

		Objective& failGangsterNotFoundObjective = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_FAIL_GANGSTERNOTFOUND);
		failGangsterNotFoundObjective.setNeededNumber(1);

		// Register find gangster message proxy (use find hidden person for this)
		qsf::MessageConfiguration message(Messages::EM5_ACTION_FIND_HIDDEN_PERSON_WITH_SEK_DRONE, mKillerEntityId);
		mKillerFoundMessageProxy.registerAt(message, boost::bind(&MissingPersonEvent::onKillerDetected, this, _1));


		// Add killer entity to event
		EventIdComponent::addToEvent(*killerEntity, *this, eventspreadreason::EVENT_SPECIFIC);

		// Start timer for fail condition
		EM5_GAMETIMERS.addTimer(qsf::MessageConfiguration(MESSAGE_ID_TIMER, getId(), 0), mKillerSeekTime);
	}

	void MissingPersonEvent::onDiagnosisDead(const qsf::MessageParameters& parameters)
	{
		startKillerSeeking();
		mDeadPersonDignosisProxy.unregister();
	}

	void MissingPersonEvent::onKillerDetected(const qsf::MessageParameters& parameters)
	{
		// Get the gangster entity
		qsf::Entity* killerEntity = getMap().getEntityById(mKillerEntityId);
		QSF_WARN_IF(nullptr == killerEntity, "Could not find gangster for killer seeking event!", QSF_REACT_NONE);

		if (nullptr == killerEntity)
			return;

		// Complete the killer seek objective
		ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_FINDKILLERWITHSEKDRONE).setAccomplished();

		mIsKillerDetectedByDrone = true;

		// Add new objectives
		{
			// Get or create conditions
			ObjectiveHelper objectiveHelper(*this);
			Objective& failConditionPersonDiedObjective = objectiveHelper.getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_FAIL_DEADPERSONS);
			failConditionPersonDiedObjective.setNeededNumber(1);

			Objective& failConditionPersonEscapedObjective = objectiveHelper.getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_FAIL_PERSONSESCAPE);
			failConditionPersonEscapedObjective.setNeededNumber(1);

			Objective& needConditionArrestPersonObjective = objectiveHelper.getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_ARRESTPERSONS);
			needConditionArrestPersonObjective.increaseNeededNumber();

			// Create and connect observers
			ObserverHelper observerHelper(*this);
			PersonDiedObserver& observerDied = observerHelper.createObserver<PersonDiedObserver>(mKillerEntityId);
			observerDied.connectToObjective(failConditionPersonDiedObjective);

			TransportedToHqObserver& observerTransported = observerHelper.createObserver<TransportedToHqObserver>(mKillerEntityId);
			observerTransported.connectToObjective(needConditionArrestPersonObjective);

			GangsterEscapedObserver& observerEscaped = observerHelper.createObserver<GangsterEscapedObserver>(mKillerEntityId);
			observerEscaped.connectToObjective(failConditionPersonEscapedObjective);
		}

		// Unregister message proxy
		mKillerFoundMessageProxy.unregister();
	}

	void MissingPersonEvent::onRescueDogFindPerson(const qsf::MessageParameters& parameters)
	{
		qsf::Entity* dogSearchSpot = getMap().getEntityById(mDogSearchSpotId);
		if (nullptr != dogSearchSpot)
		{
			// In case the victim is in water, create the diving spot and add objectives
			qsf::Entity* divingSpotEntity = MapHelper(getMap()).createObjectByLocalPrefabAssetId(equipment::PREFAB_FIRE_DIVINGSPOT);
			if (nullptr == divingSpotEntity)
			{
				return;	// Error
			}

			// Set the diving spot to the position in water (the initial position of the missing person)
			EntityHelper(*divingSpotEntity).setPositionForMultiplay(mMissingPersonPosition);

			DivingSpotComponent* divingSpotComponent = divingSpotEntity->getComponent<DivingSpotComponent>();
			if (nullptr != divingSpotComponent)
			{
				// Transform the missing entity to the diving spot
				divingSpotComponent->setAlternativeTargetId(mMissingPerson->getId());
			}

			if (mMissingPerson.valid())
			{
				// Hide the person at the Diving spot
				EntityHelper(*mMissingPerson).setPositionForMultiplay(mMissingPersonPosition);
				qsf::game::HiddenComponent::setHidden(*mMissingPerson, true);
			}

			// TODO(mk) Maybe recreate the personplacement

			// Add update objectives
			Objective& needConditionRescueDrowningObjective = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_RESCUEDROWNING);
			needConditionRescueDrowningObjective.setNeededNumber(1); // Hard scripted value

			// TODO(mk) Currently this is not working, maybe because testmap is broken
			//Objective& needConditionBringPersonToLandObjective = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_BRINGPERSONTOLAND);
			//needConditionBringPersonToLandObjective.setNeededNumber(1); // Hard scripted value

			// Register new message for the next event step
			mRescueDogFindPersonProxy.unregister();
			qsf::MessageConfiguration message(Messages::EM5_ACTION_FIND_ENTITY_FROM_DIVINGSPOT, divingSpotEntity->getId(), mMissingPerson->getId());
			mFindEntityFromDivingSpotProxy.registerAt(message, boost::bind(&MissingPersonEvent::findEntityInDivingSpot, this, _1));

			// Show hint
			showHint(MISSINGPERSON_HINT_03, false);
		}
	}

	void MissingPersonEvent::findEntityInDivingSpot(const qsf::MessageParameters& parameters)
	{
		mIsPersonUnderwater = false;

		// Update objectives
		ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_RESCUEDROWNING).increaseCurrentNumber();

		if (mMissingPerson.valid())
		{
			// Injure the person
			mMissingPerson->getComponent<HealthComponent>()->injurePersonByEventById(injury::DROWNING_NORMAL, this);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
