// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/EventHelper.h"
#include "em5/base/ComponentsRandomEnumerator.h"
#include "em5/base/GameplayAssets.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/component/movement/MoveToDespawnComponent.h"
#include "em5/component/objects/CallForRescueComponent.h"
#include "em5/component/objects/BuryComponent.h"
#include "em5/component/overlay/StatusOverlayComponent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/component/person/GaperComponent.h"
#include "em5/component/spawnpoint/civilist/PersonSpawnPointComponent.h"
#include "em5/freeplay/event/FreeplayEvent.h"
#include "em5/freeplay/objective/ObjectiveHelper.h"
#include "em5/game/Game.h"
#include "em5/game/player/PlayerManager.h"
#include "em5/health/HealthComponent.h"
#include "em5/health/HealthHelper.h"
#include "em5/logic/local/MiniMapIconRegistrationLogic.h"
#include "em5/logic/observer/FlyingHeliPlacedPersonObserver.h"
#include "em5/logic/observer/PersonDiedObserver.h"
#include "em5/logic/observer/RescueDogFoundPersonObserver.h"
#include "em5/logic/observer/RescueDrowningPersonObserver.h"
#include "em5/logic/observer/UnhideBuriedObserver.h"
#include "em5/logic/ObserverHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/EM5Helper.h"

#include <qsf_game/component/base/HiddenComponent.h>
#include <qsf_game/component/event/EventTagManagerComponent.h>

#include <qsf_ai/navigation/NavigationComponent.h>

#include <qsf/base/ScratchBuffer.h>
#include <qsf/base/StringParser.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/Map.h>
#include <qsf/map/Entity.h>
#include <qsf/map/layer/Layer.h>
#include <qsf/map/layer/LayerManager.h>
#include <qsf/math/Color3.h>
#include <qsf/math/Color4.h>
#include <qsf/physics/collision/BulletBoxCollisionComponent.h>
#include <qsf/physics/collision/CollisionHelper.h>
#include <qsf/renderer/utility/CameraControlComponent.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	qsf::Entity* EventHelper::findSpawnPoint(const qsf::StringHash& tag)
	{
		// Get a listing of all spawnpoints for the given tag
		const std::vector<qsf::game::EventTagComponent*>& tagComponentsInMap = qsf::game::EventTagManagerComponent::getEventTagComponentsByTag(tag, QSF_MAINMAP);
		ComponentsRandomEnumerator<qsf::game::EventTagComponent> randomEnumerator(tagComponentsInMap);

		while (qsf::Entity* spawnPointEntity = randomEnumerator.getNextEntity())
		{
			// Check if the candidate is appropriate
			if (checkSpawnPoint(*spawnPointEntity))
			{
				return spawnPointEntity;
			}
		}

		return nullptr;
	}

	qsf::Entity* EventHelper::spawnEntity(qsf::Entity& spawnpoint, const qsf::Time& fadeTime)
	{
		// Check if entity has a spawn component
		PersonSpawnPointComponent* spawnPointEntity = spawnpoint.getComponent<PersonSpawnPointComponent>();
		return (nullptr == spawnPointEntity) ? nullptr : spawnPointEntity->spawnRandom(true, fadeTime);
	}

	void EventHelper::copyPosition(const qsf::Entity& reference, qsf::Entity& target)
	{
		const qsf::TransformComponent& referenceTransformComponent = reference.getComponentSafe<qsf::TransformComponent>();
		qsf::TransformComponent& targetTransformComponent = target.getComponentSafe<qsf::TransformComponent>();
		targetTransformComponent.setPositionAndRotation(referenceTransformComponent.getPosition(), referenceTransformComponent.getRotation());
	}

	bool EventHelper::activateEventLayer(const std::string& layerName)
	{
		qsf::CollisionHelper collisionHelper(QSF_MAINMAP);

		qsf::Layer* layer = QSF_MAINMAP.getLayerManager().getLayerByName(qsf::StringHash(layerName));
		if (nullptr == layer)
			return false;

		qsf::Map& layerBufferMap = layer->getInternalBufferMap();
		auto entities = layerBufferMap.getEntities();
		for (qsf::Entity* targetEntity : entities)
		{
			// Check collision
			qsf::BulletCollisionComponent* collisionComponent = targetEntity->getComponent<qsf::BulletCollisionComponent>();
			if (nullptr != collisionComponent)
			{
				const qsf::TransformComponent& transformComponent = targetEntity->getOrCreateComponentSafe<qsf::TransformComponent>();
				if (!collisionHelper.canBeAddedWithoutCollision(transformComponent.getTransform(), *collisionComponent))
				{
					return false;
				}
			}
		}

		return true;
	}

	bool EventHelper::calculateMapBoundingBox(const qsf::Map& map, glm::vec3& resultMin, glm::vec3& resultMax)
	{
		bool first = true;

		auto entities = map.getEntities();
		for (qsf::Entity* targetEntity : entities)
		{
			const qsf::TransformComponent& transformComponent = targetEntity->getOrCreateComponentSafe<qsf::TransformComponent>();
			glm::vec3 pos(transformComponent.getPosition());
			if (first)
			{
				first = false;
				resultMin = pos;
				resultMax = pos;
			}
			else
			{
				if (pos.x < resultMin.x)
					resultMin.x = pos.x;
				if (pos.y < resultMin.y)
					resultMin.y = pos.y;
				if (pos.z < resultMin.z)
					resultMin.z = pos.z;
				if (pos.x > resultMax.x)
					resultMax.x = pos.x;
				if (pos.y > resultMax.y)
					resultMax.y = pos.y;
				if (pos.z > resultMax.z)
					resultMax.z = pos.z;
			}
		}

		return !first;
	}

	bool EventHelper::isLayerVisible(const qsf::Layer& layer)
	{
		glm::vec3 borderMin, borderMax;
		if (!calculateMapBoundingBox(layer.getInternalBufferMap(), borderMin, borderMax))
		{
			// No bounding box, so we can't see it
			return false;
		}

		// TODO(fw): We could check all individual entities for visibility instead
		const glm::vec3 center((borderMax + borderMin) * 0.5f);
		const float radius = glm::distance(borderMin, borderMax) * 0.5f;
		return EntityHelper::isPositionVisibleForAnyPlayer(center, radius);
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	EventHelper::EventHelper(FreeplayEvent& freeplayEvent) :
		mFreeplayEvent(freeplayEvent)
	{
		// Nothing here
	}

	void EventHelper::makeMissingPerson(qsf::Entity& entity, const qsf::StringHash& injuryHash, bool isDead)
	{
		// Hide the person
		qsf::game::HiddenComponent::setHidden(entity, true, qsf::game::HiddenComponent::HIDDENTYPE_BLOCKED);

		// Get HealthComponent
		HealthComponent* healthComponent = entity.getComponent<HealthComponent>();
		QSF_CHECK(nullptr != healthComponent, "Can't find health component; EventHelper method: makeMissingPerson", return);

		// Calculate if the person is dead or injured
		if (isDead)
		{
			// Configure the visual injury
			VisualVitalSigns visualVitalSigns;
			visualVitalSigns.setVisualVitalSignBloodPuddle(true);
			visualVitalSigns.setVisualVitalSignBloodCovered(true);

			// Kill the person
			healthComponent->killPerson(&mFreeplayEvent, &visualVitalSigns);
		}
		else
		{
			// Injure the person
			//  -> Add additional objectives later, when the person is found (i.e. no freeplay event is passed)
			//  -> Use InjuryAtStartupComponent's data if available, otherwise the given injury
			HealthHelper(*healthComponent).injurePersonByInjuryAtStartup(nullptr, injuryHash, false);

			// Get or create fail conditions
			Objective& failConditionPersonDiedObjective = ObjectiveHelper(mFreeplayEvent).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_FAIL_DEADPERSONS);

			// Create and connect observers
			PersonDiedObserver& observerDied = ObserverHelper(mFreeplayEvent).createObserver<PersonDiedObserver>(entity.getId());
			observerDied.connectToObjective(failConditionPersonDiedObjective);
		}

		// Create objectives
		Objective& needConditionFindPersonObjective = ObjectiveHelper(mFreeplayEvent).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_FINDPERSON);
		needConditionFindPersonObjective.increaseNeededNumber(&entity);

		// Create and connect observers
		RescueDogFoundPersonObserver& observerFoundPerson = ObserverHelper(mFreeplayEvent).createObserver<RescueDogFoundPersonObserver>(entity.getId());
		observerFoundPerson.connectToObjective(needConditionFindPersonObjective);
	}

	void EventHelper::makeInjuredPerson(qsf::Entity& entity, const std::string& injury, bool addInjuryObjectives)
	{
		HealthComponent* healthComponent = entity.getComponent<HealthComponent>();
		QSF_CHECK(nullptr != healthComponent, "Can't find health component; EventHelper method: makeInjuredPerson", return);

		// Injure the person
		if (addInjuryObjectives)
		{
			healthComponent->injurePersonByEventById(qsf::StringHash(injury), &mFreeplayEvent);
		}
		else
		{
			healthComponent->injurePersonByEventById(qsf::StringHash(injury), nullptr);
		}
	}

	void EventHelper::makeDrownedPerson(qsf::Entity& entity, bool addInjuryObjectives)
	{
		if (!EntityHelper(entity).isEntityPerson())
			return;

		// Injure the person
		if (addInjuryObjectives)
		{
			HealthHelper(entity).drownPerson(&mFreeplayEvent);
		}
		else
		{
			HealthHelper(entity).drownPerson(nullptr);
		}

		// Add to rescue objective.
		Objective& rescueDrowningObjective = ObjectiveHelper(mFreeplayEvent).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_BRINGPERSONTOLAND);
		rescueDrowningObjective.increaseNeededNumber(&entity);

		// Clear objective slot when drowning person is rescued
		RescueDrowningPersonObserver& rescueDrowningObserver = ObserverHelper(mFreeplayEvent).createObserver<RescueDrowningPersonObserver>(entity.getId());
		rescueDrowningObserver.connectToObjective(rescueDrowningObjective);
	}

	void EventHelper::makeRescuablePerson(qsf::Entity& entity)
	{
		EventIdComponent::addToEvent(entity, mFreeplayEvent, eventspreadreason::Reason::EVENT_SPECIFIC);

		// Add call for rescue component
		CallForRescueComponent& callForRescueComponent = entity.getOrCreateComponentSafe<CallForRescueComponent>();
		callForRescueComponent.setCanRescuedByHelicopter(true);

		// Add objectives
		Objective& rescuePeople = ObjectiveHelper(mFreeplayEvent).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_RESCUEPERSONWITHHELI);
		rescuePeople.increaseNeededNumber(&entity);

		FlyingHeliPlacedPersonObserver& placedDownByHeliObserver = ObserverHelper(mFreeplayEvent).createObserver<FlyingHeliPlacedPersonObserver>(entity.getId());
		placedDownByHeliObserver.connectToObjective(rescuePeople);

		// Deactivate movement logic
		MoveToDespawnComponent* moveToDespawnComponent = entity.getComponent<MoveToDespawnComponent>();
		if (nullptr != moveToDespawnComponent)
		{
			moveToDespawnComponent->setActive(false);
		}

		// Deactivate ai navigation
		qsf::ai::NavigationComponent* navigationComponent = entity.getComponent<qsf::ai::NavigationComponent>();
		if (nullptr != navigationComponent)
		{
			navigationComponent->setActive(false);
		}
	}

	void EventHelper::makeTweetPerson(qsf::Entity& entity, bool active)
	{
		if (!EntityHelper(entity).isEntityPerson())
			return;

		entity.getOrCreateComponentSafe<HealthComponent>().setInvincible(active);

		if (active)
		{
			entity.getOrCreateComponentSafe<GaperComponent>().setUseMobilephone(true);
		}
		else
		{
			entity.destroyComponent<GaperComponent>();
		}

		StatusOverlayComponent::showIcon(entity, StatusOverlayComponent::STATUS_ICON_WARBLERTWEET, active);
	}

	void EventHelper::buryPersonsUnderLightDebris(qsf::Entity& debrisEntity, bool makeMissing)
	{
		buryPersonsUnderDebris(debrisEntity, ObjectiveHelper::OBJECTIVE_NEED_PERSONSBURIEDLIGHTDEBRIS, makeMissing);
	}

	void EventHelper::buryPersonsUnderHeavyDebris(qsf::Entity& debrisEntity, bool makeMissing)
	{
		buryPersonsUnderDebris(debrisEntity, ObjectiveHelper::OBJECTIVE_NEED_PERSONSBURIEDHEAVYDEBRIS2, makeMissing);
	}

	void EventHelper::buryPersonsUnderDebris(qsf::Entity& debrisEntity, uint32 objectiveId, bool makeMissing)
	{
		// Setup persons under debris
		BuryComponent* buryComponent = debrisEntity.getComponent<BuryComponent>();
		QSF_CHECK(nullptr != buryComponent, "No bury component found in entity " << debrisEntity.getId(), return);

		for (uint64 buriedEntityId : buryComponent->BuriedEntitiesArray.getVariable())
		{
			qsf::Entity* buriedEntity = debrisEntity.getMap().getEntityById(buriedEntityId);
			if (buriedEntity != nullptr)
			{
				buryComponent->buryEntity(*buriedEntity);

				if (makeMissing)
				{
					makeMissingPerson(*buriedEntity, injury::BROKEN_SKULL, false);
				}
				else
				{
					// Add objectives
					Objective& rescuePeople = ObjectiveHelper(mFreeplayEvent).getOrCreateObjectiveByTypeId(objectiveId);
					rescuePeople.increaseNeededNumber(buriedEntity);

					UnhideBuriedObserver& unhideBuriedObserver = ObserverHelper(mFreeplayEvent).createObserver<UnhideBuriedObserver>(buriedEntity->getId());
					unhideBuriedObserver.connectToObjective(rescuePeople);
				}
			}
		}

		// Show the debris (just in case it was hidden before)
		debrisEntity.setActive(true);
	}

	qsf::Entity* EventHelper::createEventIconCircleArea(const qsf::Entity& entity, float radius, float maxOffset)
	{
		// Create color
		static const qsf::Color4 iconColor(qsf::Color3::ORANGE, 0.5f);

		// Get position
		qsf::TransformComponent& transformComponent = entity.getComponentSafe<qsf::TransformComponent>();
		glm::vec3 entityPosition = transformComponent.getPosition();

		const float randomOffsetX = qsf::Random::getRandomFloat(-maxOffset, maxOffset);
		const float randomOffsetY = qsf::Random::getRandomFloat(-maxOffset, maxOffset);

		entityPosition.x += randomOffsetX;
		entityPosition.y += randomOffsetY;

		// Create icon entity
		qsf::Entity* iconEntity = MapHelper(mFreeplayEvent.getMap()).createObjectByLocalPrefabAssetId(assets::PREFAB_EVENT_POSITION_HELPER_ENTITY);
		QSF_CHECK(nullptr != iconEntity, "There is no entity created; EventHelper method: createEventIconCircleArea", QSF_REACT_THROW);

		// Set icon position
		EntityHelper(*iconEntity).setPositionForMultiplay(entityPosition);

		// Add minimap icon
		EM5_GAME.getMiniMapIconRegistrationLogic().registerCirclePolygon(iconEntity->getId(), radius, 40, iconColor);

		return iconEntity;
	}

	void EventHelper::removeEventIcon(qsf::Entity& entity)
	{
		// Unregister icon
		EM5_GAME.getMiniMapIconRegistrationLogic().unregisterPolygon(entity.getId());

		// Destroy entity
		mFreeplayEvent.getMap().destroyEntityById(entity.getId());
	}

	qsf::Entity* EventHelper::getRandomEventEntity(bool showHidden)
	{
		// Show random event entity
		if (mFreeplayEvent.getEntityIds().empty())
			return nullptr;

		qsf::StdVectorScratchBuffer<uint64> openEntityIds(mFreeplayEvent.getEntityIds());
		while (!openEntityIds.empty())
		{
			// Pick out a random entity ID; but don't do a simple erase, but move the last entity ID there (to reduce copying overhead)
			const size_t index = qsf::Random::getRandomSize(0, openEntityIds.size() - 1);
			const uint64 entityId = openEntityIds[index];
			openEntityIds.replaceWithBack(index);

			qsf::Entity* entity = mFreeplayEvent.getMap().getEntityById(entityId);
			if (nullptr == entity)
				continue;

			if (EntityHelper(*entity).isEntityHidden() && !showHidden)
				continue;

			return entity;
		}

		return nullptr;
	}


	//[-------------------------------------------------------]
	//[ Private static methods                                ]
	//[-------------------------------------------------------]
	bool EventHelper::checkSpawnPoint(qsf::Entity& targetEntity)
	{
		EntityHelper entityHelper(targetEntity);

		// Check if spawnpoint has event ID component
		if (entityHelper.checkForEventIdComponent())
			return false;

		if (!entityHelper.isInEventBoundaries())
			return false;

		// Check if entity has a spawn component
		if (nullptr == targetEntity.getComponent<PersonSpawnPointComponent>())
			return false;

		// Accepted
		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
