// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/destruction/StormDamageEvent.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/component/movement/MoveToDespawnComponent.h"
#include "em5/component/objects/BuryComponent.h"
#include "em5/component/objects/EboxComponent.h"
#include "em5/component/objects/UsableByEngineerComponent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/component/vehicle/VehicleComponent.h"
#include "em5/event/standard/destruction/helper/CarAccidentHelper.h"
#include "em5/event/standard/fire/BuildingFireEventFactory.h"
#include "em5/event/standard/fire/TreeAndBushFireEventFactory.h"
#include "em5/event/standard/medical/MedicalEmergencyEventFactory.h"
#include "em5/freeplay/objective/ObjectiveHelper.h"
#include "em5/health/HealthHelper.h"
#include "em5/logic/HintHelper.h"
#include "em5/logic/ObserverHelper.h"
#include "em5/logic/cleanup/LayerCleanup.h"
#include "em5/logic/observer/UnhideBuriedObserver.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/plugin/Messages.h"

#include <qsf_game/component/event/EventTagHelper.h>
#include <qsf_game/environment/weather/WeatherComponent.h>
#include <qsf_game/environment/weather/WeatherSystem.h>
#include <qsf_game/QsfGameHelper.h>

#include <qsf/base/ScratchBuffer.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/log/LogSystem.h>
#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>
#include <qsf/map/layer/Layer.h>
#include <qsf/map/layer/LayerManager.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/math/Random.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Anonymous namespace                                   ]
//[-------------------------------------------------------]
namespace
{
	const qsf::StringHash EVENT_LAYER_NAME("FP_01_STURMSCHADEN");
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 StormDamageEvent::FREEPLAY_EVENT_ID = qsf::StringHash("em5::StormDamageEvent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	StormDamageEvent::StormDamageEvent() :
		mTimeBeforeFirstTask(qsf::Time::fromSeconds(8.0f)),
		mTimeBetweenTasks(qsf::Time::fromSeconds(15.0f)),
		mEboxRepairTime(qsf::Time::fromSeconds(10.0f)),
		mTrappedPersonsInBuildings(2),
		mDeadPersonsToFail(2),
		mBurningObjectsToFail(50),
		mEventLayer(nullptr),
		mIsTriggeringNewTasks(true)
	{
		// Nothing here
	}

	StormDamageEvent::~StormDamageEvent()
	{
		// Nothing here
	}

	void StormDamageEvent::setNumberOfTreeOnRoadTasks(uint32 number)
	{
		mTaskPool[TASK_TREE_ON_ROAD] = number;
	}

	void StormDamageEvent::setNumberOfTreeOnCarTasks(uint32 number)
	{
		mTaskPool[TASK_TREE_ON_CAR] = number;
	}

	void StormDamageEvent::setNumberOfCarAccidentTasks(uint32 number)
	{
		mTaskPool[TASK_CAR_ACCIDENT] = number;
	}

	void StormDamageEvent::setNumberOfLightningFireTasks(uint32 number)
	{
		mTaskPool[TASK_LIGHTNING_FIRE] = number;
	}

	void StormDamageEvent::setNumberOfTornadoTasks(uint32 number)
	{
		mTaskPool[TASK_TORNADO] = number;
	}

	void StormDamageEvent::setNumberOfBrokenElectricPylonTasks(uint32 number)
	{
		mTaskPool[TASK_BROKEN_ELECTRIC_PYLON] = number;
	}

	void StormDamageEvent::setNumberOfInjuriesTasks(uint32 number)
	{
		mTaskPool[TASK_INJURIES] = number;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	bool StormDamageEvent::onStartup()
	{
		// Search for event layer; it has a fixed name
		mEventLayer = QSF_MAINMAP.getLayerManager().getLayerByName(EVENT_LAYER_NAME);
		if (nullptr == mEventLayer)
			return false;

		// Setup scene for event
		setupScene();

		// Event is now running
		setRunning();

		return true;
	}

	void StormDamageEvent::onShutdown()
	{
		// Cleanup layers again
		for (const auto& pair : mLayersToCleanup)
		{
			LayerCleanup::createLayerCleanup(*pair.first, pair.second);
		}
	}

	void StormDamageEvent::onRun()
	{
		// Show supervisor message
		showSupervisorMessage(HintHelper::getRandomStringOfIdString(HintHelper(*this).getHintParameters().mSupervisorMessageIds));

		startObjectives();

		// Create message listener for damaging and repairing
		mRepairEboxMessageProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_ACTION_REPAIRED_OR_USED), boost::bind(&StormDamageEvent::onEboxRepaired, this, _1));

		mTimeUntilNextTask = mTimeBeforeFirstTask;
	}

	bool StormDamageEvent::onFailure(EventResult& eventResult)
	{
		// Call base implementation
		return FreeplayEvent::onFailure(eventResult);
	}

	bool StormDamageEvent::onSuccess(EventResult& eventResult)
	{
		// Nothing here
		return true;
	}

	void StormDamageEvent::updateFreeplayEvent(const qsf::Time& timePassed)
	{
		if (mIsTriggeringNewTasks)
		{
			// If weather does not fit, correct it -- we want a storm all the time
			qsf::game::WeatherComponent* weatherComponent = getMap().getCoreEntity().getComponent<qsf::game::WeatherComponent>();
			if (nullptr != weatherComponent)
			{
				if (weatherComponent->getLastSetWeatherStateName() != "Storm")
				{
					qsf::game::WeatherSystem::WeatherStateManager& weatherStateManager = QSFGAME_WEATHER.getWeatherStateManager();
					qsf::game::WeatherState* weatherState = weatherStateManager.findElement(qsf::StringHash("Storm"));
					if (nullptr != weatherState)
					{
						weatherComponent->fadeToWeatherState(*weatherState, qsf::Time::fromSeconds(10.0f));
					}
				}
			}

			// Trigger the next task
			mTimeUntilNextTask -= timePassed;
			if (mTimeUntilNextTask <= qsf::Time::ZERO)
			{
				const bool result = tryTriggerNewTask();
				if (result)
				{
					// Triggering was a success, now restart timer
					mTimeUntilNextTask = mTimeBetweenTasks;
				}
			}

			// Check for failure only
			if (getState() == STATE_RUNNING)
			{
				if (getObjectives().checkFailure())
				{
					// Output fail reason in log
					getObjectives().debugLogFailure();

					// Event failed...
					setFinished(false);
				}
			}
		}
		else
		{
			// Check objectives for success or failure
			//  -> We do this only if "mIsTriggeringNewTasks" is not set any more
			checkObjectivesState();
		}
	}

	bool StormDamageEvent::addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason)
	{
		// Call base implementation
		return FreeplayEvent::addEntityToEvent(targetEntity, eventSpreadReason, newReason);
	}

	const qsf::Entity* StormDamageEvent::getFocusEntity()
	{
		return getRandomEventEntity();
	}

	bool StormDamageEvent::checkEventConfiguration()
	{
		qsf::Layer* eventLayer = QSF_MAINMAP.getLayerManager().getLayerByName(EVENT_LAYER_NAME);
		if (nullptr == eventLayer)
			return false;

		return true;
	}

	void StormDamageEvent::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base implementation
		FreeplayEvent::serialize(serializer);
	}

	void StormDamageEvent::hintCallback(Observer& hintObserver)
	{
		// No implementation needed
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void StormDamageEvent::startObjectives()
	{
		ObjectiveHelper objectiveHelper(*this);

		// Objectives to win the event are added elsewhere (e.g. when an ebox gets damaged)

		// Fail conditions
		if (mDeadPersonsToFail > 0)
		{
			objectiveHelper.addDeadCivilPersonsFailCondition(mDeadPersonsToFail);
			objectiveHelper.addDeadUnitPersonsFailCondition(mDeadPersonsToFail);
		}

		objectiveHelper.addBurningObjectsAndBuildingsFailCondition(mBurningObjectsToFail);
	}

	void StormDamageEvent::setupScene()
	{
		// Nothing to do here, as the tasks will setup their scenes
	}

	bool StormDamageEvent::tryTriggerNewTask()
	{
		uint32 numTasksInPool = 0;
		for (auto& pair : mTaskPool)
		{
			numTasksInPool += pair.second;
		}

		if (numTasksInPool == 0)
		{
			// Nothing left to do
			mIsTriggeringNewTasks = false;
			return false;
		}

		bool result = false;
		for (int tries = 0; tries < 5; ++tries)
		{
			// Choose a random task type from the task pool
			TaskType taskType = TASK_TREE_ON_ROAD;	// Fallback value only
			uint32 randomIndex = qsf::Random::getRandomUint(0, numTasksInPool-1);
			for (auto& pair : mTaskPool)
			{
				if (randomIndex < pair.second)
				{
					taskType = pair.first;
					break;
				}
				else
				{
					randomIndex -= pair.second;
				}
			}

			// Don't allow the same task type twice in a row
			if (!mTasksStarted.empty() && mTasksStarted.back().mType == taskType)
			{
				// We make an exception in case there is only this task type left
				if (numTasksInPool != mTaskPool[taskType])
				{
					continue;
				}
			}

			// Go and give it a try
			result = startTask(taskType);
			if (result)
			{
				QSF_CHECK(mTaskPool[taskType] >= 1, "Something went wrong in task pool", return true);
				--mTaskPool[taskType];
				return true;
			}
		}

		return false;
	}

	bool StormDamageEvent::startTask(TaskType type)
	{
		switch (type)
		{
			case TASK_TREE_ON_ROAD:
			{
				// Choose a random layer
				qsf::Layer* layer = chooseSubLayer("STORMTREE", false);
				if (nullptr == layer)
					return false;

				// Show the layer
				activateLayer(*layer);

				// Initialize fallen trees there
				initBuryingObjectsInLayer(*layer, "EM2017_FP01_STORMTREE", true);

				showHint("EM2017_FP01_STORMTREE_HINT01", false);

				QSF_LOG_PRINTS(DEBUG, "Storm damage event triggered task \"Tree on road\" (STORMTREE layer)");
				return true;
			}

			case TASK_TREE_ON_CAR:
			{
				// Choose a random layer
				qsf::Layer* layer = chooseSubLayer("STORMCAR", false);
				if (nullptr == layer)
					return false;

				// Show the layer
				activateLayer(*layer);

				// Initialize fallen trees and damaged cars there (in this order!)
				initBuryingObjectsInLayer(*layer, "EM2017_FP01_STORMTREE", true);
				initAccidentCarsInLayer(*layer);

				showHint("EM2017_FP01_STORMCAR_HINT01", false);

				QSF_LOG_PRINTS(DEBUG, "Storm damage event triggered task \"Tree on car\" (STORMCAR layer)");
				return true;
			}

			case TASK_CAR_ACCIDENT:
			{
				// Choose a random layer
				qsf::Layer* layer = chooseSubLayer("CARACCIDENT", false);
				if (nullptr == layer)
					return false;

				// Show the layer
				activateLayer(*layer);

				// Initialize accidents there
				initAccidentCarsInLayer(*layer);

				showHint("EM2017_FP01_CARACCIDENT_HINT01", false);

				QSF_LOG_PRINTS(DEBUG, "Storm damage event triggered task \"Car accident\" (CARACCIDENT layer)");
				return true;
			}

			case TASK_LIGHTNING_FIRE:
			{
				// Choose a random building, tree or hedge
				qsf::Entity* entity = findLightningFireTarget();
				if (nullptr == entity)
					return false;

				// Pyromaniac's fun time!
				EntityHelper(*entity).startFire(this);

				showHint("EM2017_FP01_THUNDERSTORM_HINT01", false);

				QSF_LOG_PRINTS(DEBUG, "Storm damage event triggered task \"Fire by lightning\"");
				return true;
			}

			case TASK_TORNADO:
			{
				// Choose a random layer
				qsf::Layer* layer = chooseSubLayer("TORNADO", true);
				if (nullptr == layer)
					return false;

				// Prepare all buildings
				if (!initDamagedBuildingsAssociatedWithLayer(*layer))
					return false;

				// Activate/deactivate sub-layers accordingly
				toggleAddEraseSubLayers(*layer);

				// Debris
				initBuryingObjectsInLayer(*layer, "EM2017_FP01_DEBRIS", false);

				showHint("EM2017_FP01_TORNADO_HINT01", false);

				QSF_LOG_PRINTS(DEBUG, "Storm damage event triggered task \"Tornado\" (TORNADO layer)");
				return true;
			}

			case TASK_BROKEN_ELECTRIC_PYLON:
			{
				// Choose a random layer
				qsf::Layer* layer = chooseSubLayer("ELECTRIC", true);
				if (nullptr == layer)
					return false;

				// Activate/deactivate sub-layers accordingly
				toggleAddEraseSubLayers(*layer);

				// Initialize pylons and eboxes
				initBuryingObjectsInLayer(*layer, "EM2017_FP_01_PYLON", false);
				initEboxInLayer(*layer);

				showHint("EM2017_FP01_ELECTRIC_HINT01", false);

				QSF_LOG_PRINTS(DEBUG, "Storm damage event triggered task \"Broken power pole\" (ELECTRIC layer)");
				return true;
			}

			case TASK_INJURIES:
			{
				// Injure a random civilist
				qsf::Entity* entity = MedicalEmergencyEventFactory::getRandomCandidatePerson();
				if (nullptr == entity)
					return false;

				// Stone injuries are a somewhat reasonable choice here
				HealthHelper(*entity).injurePersonByStone(this);

				showHint("EM2017_FP01_THUNDERINJURY_HINT01", false);

				QSF_LOG_PRINTS(DEBUG, "Storm damage event triggered task \"Injuries\"");
				return true;
			}
		}

		// Failed
		return false;
	}

	qsf::Layer* StormDamageEvent::chooseSubLayer(const std::string& layerNaming, bool usesAddEraseLayers)
	{
		std::vector<qsf::Layer*> candidates;

		for (qsf::Layer* layer : mEventLayer->getLayers())
		{
			// Check layer name
			if (layer->getName().find(layerNaming) != std::string::npos)
			{
				// Check if the layer (or the right sub layer) is already active
				qsf::Layer* layerToCheck = layer;
				if (usesAddEraseLayers)
				{
					// Find "add" sub layer
					for (qsf::Layer* subLayer : layer->getLayers())
					{
						if (subLayer->getName().find("add") != std::string::npos)
						{
							layerToCheck = subLayer;
							break;
						}
					}
				}

				if (!layerToCheck->isActive())
				{
					candidates.push_back(layer);
				}
			}
		}

		return candidates.empty() ? nullptr : qsf::Random::getAnyOf(candidates);
	}

	void StormDamageEvent::activateLayer(qsf::Layer& layer)
	{
		MapHelper::activateLayer(layer, true);
		mLayersToCleanup.emplace_back(&layer, nullptr);
	}

	void StormDamageEvent::toggleAddEraseSubLayers(qsf::Layer& layer)
	{
		qsf::Layer* addLayer = nullptr;
		qsf::Layer* eraseLayer = nullptr;
		for (qsf::Layer* subLayer : layer.getLayers())
		{
			if (subLayer->getName().find("add") != std::string::npos)
			{
				// "add" sub layer
				MapHelper::activateLayer(*subLayer, true);

				QSF_WARN_IF(nullptr != addLayer, "Multiple add layers inside " << layer.getName(), QSF_REACT_NONE);
				addLayer = subLayer;
			}
			else if (subLayer->getName().find("erase") != std::string::npos)
			{
				// "erase" sub layer
				MapHelper::activateLayer(*subLayer, false);

				QSF_WARN_IF(nullptr != eraseLayer, "Multiple erase layers inside " << layer.getName(), QSF_REACT_NONE);
				eraseLayer = subLayer;
			}
		}

		if (nullptr != addLayer)
		{
			mLayersToCleanup.emplace_back(addLayer, eraseLayer);
		}
		else
		{
			QSF_WARN_IF(nullptr != eraseLayer, "Layer " << layer.getName() << " has erase layer, but no add layer", QSF_REACT_NONE);
		}
	}

	void StormDamageEvent::initBuryingObjectsInLayer(qsf::Layer& layer, const std::string& eventTag, bool makeCuttable)
	{
		// Find all objects the player has to remove
		qsf::EntityVectorScratchBuffer objects;
		qsf::EntityVectorScratchBuffer buriedEntities;

		qsf::game::EventTagHelper("StormDamageEvent").acquireTaggedEntitiesInLayer(objects, eventTag, layer);

		for (qsf::Entity* object : objects)
		{
			BuryComponent* buryComponent = object->getComponent<BuryComponent>();
			QSF_CHECK(nullptr != buryComponent, "Object " << object->getId() << " has no bury component", continue);

			if (makeCuttable)
			{
				buryComponent->setCutableBySaw(true);	// In case this is not edited correctly
			}

			getMap().getEntitiesByIds(buryComponent->BuriedEntitiesArray.getVariable(), buriedEntities, true);
			for (qsf::Entity* buriedEntity : buriedEntities)
			{
				EntityHelper entityHelper(*buriedEntity);
				if (entityHelper.isCivilPerson())
				{
					// Update placement (e.g. the pylons are rotated, so the persons' transforms have to be corrected)
					entityHelper.applyPlacement();

					// Injure person
					HealthHelper(*buriedEntity).injurePersonByInjuryAtStartup(this);

					// Add to objective
					Objective& rescuePeople = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_PERSONSBURIEDHEAVYDEBRIS);
					rescuePeople.increaseNeededNumber(buriedEntity);

					UnhideBuriedObserver& unhideBuriedObserver = ObserverHelper(*this).createObserver<UnhideBuriedObserver>(buriedEntity->getId());
					unhideBuriedObserver.connectToObjective(rescuePeople);
				}
			}

			// Add to event, so we can handle the objective correctly
			EventIdComponent::addToEvent(*object, *this, eventspreadreason::BROKEN_TREE);
		}
	}

	void StormDamageEvent::initAccidentCarsInLayer(qsf::Layer& layer)
	{
		// Check all entities in the accident layer
		qsf::Layer::EntityIdSet entities = layer.getEntityIds();	// Careful, don't use a reference as layer is modified in the loop
		for (uint64 entityId : entities)
		{
			qsf::Entity* targetEntity = getMap().getEntityById(entityId);
			if (nullptr != targetEntity)
			{
				EntityHelper targetEntityHelper(*targetEntity);
				if (targetEntityHelper.isCivilRoadVehicle())
				{
					// First remove all civilists around
					{
						qsf::StdVectorScratchBuffer<MoveToDespawnComponent*> components;
						qsf::ComponentMapQuery(getMap()).getInstancesInCircle<MoveToDespawnComponent>(targetEntityHelper.getPosition(), 5.0f, components);
						for (MoveToDespawnComponent* component : components)
						{
							// Check if move to despawn component is active
							//  -> This is a strong indicator that this is a normal civilian not currently occupied with some event or injury
							if (component->isActive())
							{
								qsf::Entity& entity = component->getEntity();

								// Filter entities that are part of an event
								if (EntityHelper(entity).checkForInitializedEventIdComponent())
								{
									continue;
								}

								MapHelper::destroyEntity(entity);
							}
						}
					}

					// Now prepare the accident
					CarAccidentHelper::prepareAccidentCar(*targetEntity, layer, *this, false, false);
				}
			}
		}
	}

	void StormDamageEvent::initEboxInLayer(qsf::Layer& layer)
	{
		qsf::game::EventTagHelper eventTagHelper("StormDamageEvent");

		qsf::Entity* ebox = nullptr;
		qsf::Entity* pylon = nullptr;
		eventTagHelper.acquireTaggedEntityInLayer(ebox, "EM2017_FP01_EBOX", layer);
		eventTagHelper.acquireTaggedEntityInLayer(pylon, "EM2017_FP_01_PYLON", layer);
		QSF_CHECK(nullptr != ebox, "No ebox found", return);
		QSF_CHECK(nullptr != pylon, "No pylon found", return);

		EboxComponent& eboxComponent = ebox->getOrCreateComponentSafe<EboxComponent>();
		eboxComponent.setEboxDefective();

		// Add component for using with engineer
		UsableByEngineerComponent& usableByEngineerComponent = ebox->getOrCreateComponentSafe<UsableByEngineerComponent>();
		usableByEngineerComponent.initWithUseTime(mEboxRepairTime);

		// Activate lightning particle effect
		qsf::Entity* eboxEffect = nullptr;
		eventTagHelper.acquireTaggedEntityInLayer(eboxEffect, "EM2017_FP01_EBOX_LIGHTNING", layer);
		if (nullptr != eboxEffect)
		{
			setParticleEffectEnabled(*eboxEffect, true);
		}

		// Pylon can't be lifted before ebox is switched off
		pylon->getComponentSafe<BuryComponent>().setBlocked(true);

		// Add to event
		EventIdComponent::addToEvent(*ebox, *this, eventspreadreason::EVENT_SPECIFIC);

		// Add to objective
		Objective& objectiveRepairEboxes = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_REPAIREBOXES);
		objectiveRepairEboxes.increaseNeededNumber(ebox);

		mEboxInfos.emplace_back();
		EboxInfo& info = mEboxInfos.back();
		info.mEbox = ebox;
		info.mEffect = eboxEffect;
		info.mPylon = pylon;
	}

	bool StormDamageEvent::initDamagedBuildingsAssociatedWithLayer(qsf::Layer& layer)
	{
		const std::string buildingEventTag = "EM2017_FP01_TORNADO_" + layer.getName().substr(layer.getName().length() - 2);

		qsf::EntityVectorScratchBuffer buildings;
		qsf::game::EventTagHelper("StormDamageEvent").acquireTaggedEntities(buildings, buildingEventTag);

		// Check if player looks there right now -- if at least one is visible, abort this task
		for (qsf::Entity* building : buildings)
		{
			if (EntityHelper(building).isEntityVisibleForAnyPlayer())
				return false;
		}

		for (qsf::Entity* building : buildings)
		{
			BuildingComponent& buildingComponent = building->getComponentSafe<BuildingComponent>();

			// Trap persons inside the building
			buildingComponent.setupTrappedInjuredPersons(mTrappedPersonsInBuildings, mTrappedPersonsInBuildings, 0, 0, true, true);
			buildingComponent.createTrappedPerson(*this);

			// Damage building
			buildingComponent.setDestroyedWithoutAnimation(this);
		}

		return true;
	}

	qsf::Entity* StormDamageEvent::findLightningFireTarget()
	{
		const float randomFloat = qsf::Random::getRandomFloat(0.0f, 1.0f);
		if (randomFloat < 0.5f)
		{
			// Find a tree
			return TreeAndBushFireEventFactory::findTreeCandidate("", "");
		}
		else
		{
			// Find a building, no special properties needed
			return BuildingFireEventFactory::findCandidate("", "", BuildingFireEventFactory::SearchFlags());
		}
	}

	void StormDamageEvent::setParticleEffectEnabled(qsf::Entity& entity, bool enabled)
	{
		entity.setActive(enabled);

		// New editing: Multiple particles as children (just as in TrainAccidentEvent)
		qsf::LinkComponent* linkComponent = entity.getComponent<qsf::LinkComponent>();
		if (linkComponent != nullptr)
		{
			for (qsf::LinkComponent* linkedEntityComponent : linkComponent->getChildLinks())
			{
				linkedEntityComponent->getEntity().setActive(enabled);
			}
		}
	}

	void StormDamageEvent::onEboxRepaired(const qsf::MessageParameters& parameters)
	{
		// Find out which ebox was repaired
		const uint64 eboxEntityId = parameters.getFilter(1);
		for (EboxInfo& eboxInfo : mEboxInfos)
		{
			if (eboxInfo.mEbox.valid() && eboxInfo.mEbox->getId() == eboxEntityId)
			{
				// Disable particle effect
				if (eboxInfo.mEffect.valid())
				{
					setParticleEffectEnabled(*eboxInfo.mEffect, false);
				}

				// Update objective
				Objective& objectiveRepairEboxes = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_REPAIREBOXES);
				objectiveRepairEboxes.increaseCurrentNumber(eboxInfo.mEbox.get());

				// Pylon can now be lifted
				if (eboxInfo.mPylon.valid())
				{
					eboxInfo.mPylon->getComponentSafe<BuryComponent>().setBlocked(false);
				}
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
