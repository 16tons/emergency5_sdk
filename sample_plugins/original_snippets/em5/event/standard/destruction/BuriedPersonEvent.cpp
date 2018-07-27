// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/destruction/BuriedPersonEvent.h"
#include "em5/event/EventHelper.h"
#include "em5/freeplay/objective/ObjectiveHelper.h"
#include "em5/freeplay/eventspread/EventSpreadHelper.h"
#include "em5/component/objects/BuryComponent.h"
#include "em5/map/MapHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/health/HealthSystem.h"
#include "em5/logic/ObserverHelper.h"
#include "em5/logic/HintHelper.h"
#include "em5/logic/observer/PersonDiedObserver.h"
#include "em5/logic/observer/hint/HintLowEnergyObserver.h"
#include "em5/logic/observer/UnhideBuriedObserver.h"
#include "em5/specs/ActionRangeSpecsGroup.h"
#include "em5/plugin/Messages.h"
#include "em5/EM5Helper.h"

#include <qsf/log/LogSystem.h>
#include <qsf/map/layer/Layer.h>
#include <qsf/map/layer/LayerManager.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 BuriedPersonEvent::FREEPLAY_EVENT_ID = qsf::StringHash("em5::BuriedPersonEvent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	BuriedPersonEvent::BuriedPersonEvent() :
		mInjuredEntityId(qsf::getUninitialized<uint64>()),
		mBuryObjectEntityId(qsf::getUninitialized<uint64>()),
		mIsHidden(false),
		mIconEntity(nullptr)
	{
		// Nothing here
	}

	BuriedPersonEvent::~BuriedPersonEvent()
	{
		// Nothing here
	}

	uint64 BuriedPersonEvent::getTargetPersonId() const
	{
		return mInjuredEntityId;
	}

	void BuriedPersonEvent::setTargetPerson(const qsf::Entity& entity)
	{
		mInjuredEntityId = entity.getId();
	}

	const std::string& BuriedPersonEvent::getInjuryName() const
	{
		return mInjuryName;
	}

	void BuriedPersonEvent::setInjuryName(const std::string& injuryName)
	{
		mInjuryName = injuryName;
	}

	float BuriedPersonEvent::getRunningDelay() const
	{
		return mRunningDelay.getSeconds();
	}

	void BuriedPersonEvent::setRunningDelay(float runningDelay)
	{
		mRunningDelay = qsf::Time::fromSeconds(runningDelay);
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	bool BuriedPersonEvent::onStartup()
	{
		qsf::Layer* layer = getMap().getLayerManager().getLayerByName(qsf::StringHash(mEventLayer));
		if (nullptr == layer)
			return false;

		// Deactivate the layer first to be sure that the layer is loaded new
		layer->deactivateLayer();
		layer->activateLayer();

		qsf::Map& layerBufferMap = layer->getInternalBufferMap();
		auto entities = layerBufferMap.getEntities();
		for (qsf::Entity* targetEntity : entities)
		{
			if (nullptr != targetEntity->getComponent<BuryComponent>())
			{
				mBuryObjectEntityId = targetEntity->getId();
				break;
			}
		}

		qsf::Entity* buryObject = getMap().getEntityById(mBuryObjectEntityId);
		if (nullptr == buryObject)
			return false;

		qsf::Entity* personEntity = getMap().getEntityById(mInjuredEntityId);
		if (nullptr == personEntity)
			return false;

		// No need for null pointer check here
		BuryComponent* buryComponent = buryObject->getComponent<BuryComponent>();
		buryComponent->buryEntity(*personEntity);

		if (mIsHidden)
		{
			buryComponent->setHidden(true);
			EventHelper(*this).makeMissingPerson(*personEntity, qsf::StringHash(mInjuryName), false);

			{ // Create icon for minimap
				// Use the dog search radius as reference for the event radius
				const float searchRange = ActionRangeSpecsGroup::getInstanceSafe().getRescueDogSearchDistance() * 0.5f;

				// Create icon entity
				mIconEntity = EventHelper(*this).createEventIconCircleArea(*personEntity, searchRange, searchRange);
			}

			// Register find buried message proxy
			qsf::MessageConfiguration message(Messages::EM5_RESCUE_DOG_FOUND_PERSON, personEntity->getId());
			mBuriedFoundMessageProxy.registerAt(message, boost::bind(&BuriedPersonEvent::onBuriedFound, this, _1));
		}
		else
		{
			buryComponent->setHidden(false);

			// Injure the person
			EventHelper(*this).makeInjuredPerson(*personEntity, mInjuryName);

			// Add objectives
			Objective& rescuePeople = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_PERSONSBURIEDHEAVYDEBRIS2);
			rescuePeople.increaseNeededNumber(personEntity);

			UnhideBuriedObserver& unhideBuriedObserver = ObserverHelper(*this).createObserver<UnhideBuriedObserver>(personEntity->getId());
			unhideBuriedObserver.connectToObjective(rescuePeople);
		}

		setRunning();

		// Done
		return true;
	}

	void BuriedPersonEvent::onShutdown()
	{
		if (!mEventLayer.empty())
		{
			deactivateLayer(mEventLayer);
		}

		if (nullptr != mIconEntity)
		{
			EventHelper(*this).removeEventIcon(*mIconEntity);
		}

		mBuriedFoundMessageProxy.unregister();
	}

	void BuriedPersonEvent::onRun()
	{
		// No null pointer check in here. The factory already checked these pointers.
		qsf::Entity* personEntity = getMap().getEntityById(mInjuredEntityId);

		// Set all observers and objectives for the target
		startObjectives(*personEntity);

		// Play supervisor message
		showSupervisorMessage(HintHelper::getRandomStringOfIdString(HintHelper(*this).getHintParameters().mSupervisorMessageIds));
	}

	bool BuriedPersonEvent::onFailure(EventResult& eventResult)
	{
		// Call base implementation
		return FreeplayEvent::onFailure(eventResult);
	}

	void BuriedPersonEvent::updateFreeplayEvent(const qsf::Time& timePassed)
	{
		// Check objectives for success or failure
		checkObjectivesState();
	}

	bool BuriedPersonEvent::addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason)
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

	void BuriedPersonEvent::hintCallback(Observer& hintObserver)
	{
		showHint(hintObserver.getName(), false);
	}

	const qsf::Entity* BuriedPersonEvent::getFocusEntity()
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

	bool BuriedPersonEvent::checkEventConfiguration()
	{
		// Check for valid injury
		if (nullptr == EM5_HEALTH.getInjuryById(qsf::StringHash(mInjuryName)))
		{
			// Injury not found, error
			return false;
		}

		return true;
	}

	void BuriedPersonEvent::serialize(qsf::BinarySerializer& serializer)
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
	void BuriedPersonEvent::startObjectives(const qsf::Entity& targetEntity)
	{
		// Create the fail condition
		Objective& failConditionPersonDiedObjective = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_FAIL_DEADPERSONS);
		failConditionPersonDiedObjective.setNeededNumber(1); // Hard scripted value
	}

	void BuriedPersonEvent::startHintObservers(const qsf::Entity& targetEntity)
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

	bool BuriedPersonEvent::checkCandidate(qsf::Entity* targetEntity)
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

	void BuriedPersonEvent::activateLayer(const std::string& layerName)
	{
		QSF_LOG_PRINTS(DEBUG, "Buried person event activates layer '" << layerName << "'");
		MapHelper(getMap()).activateLayerByName(layerName);
	}

	void BuriedPersonEvent::deactivateLayer(const std::string& layerName)
	{
		QSF_LOG_PRINTS(DEBUG, "Buried person event deactivates layer '" << layerName << "'");
		MapHelper(getMap()).deactivateLayerByName(layerName);
	}

	void BuriedPersonEvent::onBuriedFound(const qsf::MessageParameters& parameters)
	{
		qsf::Entity* personEntity = getMap().getEntityById(mInjuredEntityId);
		if (nullptr == personEntity)
			return;

		if (nullptr != mIconEntity)
		{
			EventHelper(*this).removeEventIcon(*mIconEntity);
		}

		// Injure the person
		// Add injury objectives
		eventspreadreason::Reason reason = eventspreadreason::INJURY;
		EventSpreadHelper::setEventIdComponent(this, *personEntity, reason);

		// Add objectives
		Objective& rescuePeople = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_PERSONSBURIEDHEAVYDEBRIS2);
		rescuePeople.increaseNeededNumber(personEntity);

		UnhideBuriedObserver& unhideBuriedObserver = ObserverHelper(*this).createObserver<UnhideBuriedObserver>(personEntity->getId());
		unhideBuriedObserver.connectToObjective(rescuePeople);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
