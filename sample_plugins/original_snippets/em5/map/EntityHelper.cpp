// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/map/CollisionHelper.h"
#include "em5/action/base/BeStunnedAction.h"
#include "em5/action/base/GuidePersonAction.h"
#include "em5/action/police/RedirectTrafficAction.h"
#include "em5/action/police/ArrestGangsterAction.h"
#include "em5/action/move/escape/EscapeCarLongAction.h"
#include "em5/action/move/escape/EscapeCarShortAction.h"
#include "em5/action/move/escape/EscapeFootLongAction.h"
#include "em5/action/move/escape/EscapeFootShortAction.h"
#include "em5/action/move/escape/EscapeHidingFootAction.h"
#include "em5/ai/MoverType.h"
#include "em5/ai/NavigationMaps.h"
#include "em5/base/ContainerCategory.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/base/GameplayAssets.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/component/building/IndustrialPlantComponent.h"
#include "em5/component/equipment/InventoryComponent.h"
#include "em5/component/event/BlockedForEventComponent.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/component/movement/MoveToDespawnComponent.h"
#include "em5/component/movement/RandomMovementComponent.h"
#include "em5/component/movement/SpeedLimitComponent.h"
#include "em5/component/multiplayer/GhostComponent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/component/person/GaperComponent.h"
#include "em5/component/vegetation/HedgeComponent.h"
#include "em5/component/vegetation/TreeComponent.h"
#include "em5/component/vehicle/BoatComponent.h"
#include "em5/component/vehicle/HelicopterComponent.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include "em5/component/vehicle/parts/VehicleCraneComponent.h"
#include "em5/component/vehicle/parts/HelicopterRotorComponent.h"
#include "em5/component/objects/BuryComponent.h"
#include "em5/component/objects/CallForRescueComponent.h"
#include "em5/component/effect/FadeEffectComponent.h"
#include "em5/freeplay/event/FreeplayEvent.h"
#include "em5/freeplay/eventspread/EventSpreading.h"
#include "em5/freeplay/eventspread/EventSpreadHelper.h"
#include "em5/fire/component/ComplexFireComponent.h"
#include "em5/health/HealthHelper.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/game/player/Player.h"
#include "em5/game/player/PlayerManager.h"
#include "em5/game/selection/GroundMarkerComponent.h"
#include "em5/game/selection/SelectionManager.h"
#include "em5/game/selection/SelectionMarkerManager.h"
#include "em5/game/spawn/SpawnPoolManager.h"
#include "em5/game/units/OrderInfo.h"
#include "em5/game/units/OrderInfoManager.h"
#include "em5/game/units/UnitPool.h"
#include "em5/game/units/UnitHelper.h"
#include "em5/game/GameSimulation.h"
#include "em5/game/Game.h"
#include "em5/gangster/GangsterTypeManager.h"
#include "em5/logic/local/base/CarryPersonLogic.h"
#include "em5/logic/local/base/PullObjectLogic.h"
#include "em5/logic/local/gangsters/GangsterBaseLogic.h"
#include "em5/logic/local/firefighters/DiverLogic.h"
#include "em5/logic/local/firefighters/FiremanLogic.h"
#include "em5/logic/local/MiniMapIconRegistrationLogic.h"
#include "em5/network/NetworkManager.h"
#include "em5/network/multiplayer/SessionBase.h"
#include "em5/plugin/Messages.h"
#include "em5/specs/ActionRangeSpecsGroup.h"
#include "em5/specs/RoadVehicleSpecsGroup.h"
#include "em5/settings/GameSettingsGroup.h"
#include "em5/EM5Helper.h"

#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/equipment/EquipmentComponent.h>
#include <qsf_game/component/base/BoneToBoneLinkComponent.h>
#include <qsf_game/component/base/HiddenComponent.h>
#include <qsf_game/component/event/EventTagComponent.h>

#include <qsf_ai/navigation/NavigationComponent.h>
#include <qsf_ai/navigation/em4Router/RouterComponent.h>
#include <qsf_ai/navigation/em4Router/wrapper/EM3Singletons.h>
#include <qsf_ai/navigation/em4Router/wrapper/actor/EActor.h>
#include <qsf_ai/navigation/em4Router/em2012/RouteFinder/RouteFinder.h>
#include <qsf_ai/navigation/em4Router/em2012/RouteFinder/RouterEnvironment.h>

#include <qsf_compositing/component/TintableMeshComponent.h>

#include <qsf/component/move/MovableComponent.h>
#include <qsf/component/base/MetadataComponent.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/component/placement/PlacementComponent.h>
#include <qsf/physics/collision/BulletBoxCollisionComponent.h>
#include <qsf/physics/collision/BulletCollisionComponent.h>
#include <qsf/renderer/utility/CameraControlComponent.h>
#include <qsf/renderer/light/LightAnimationComponent.h>
#include <qsf/renderer/light/LightComponent.h>
#include <qsf/renderer/animation/MeshAnimationTestComponent.h>
#include <qsf/renderer/mesh/MeshComponent.h>
#include <qsf/renderer/flare/FlareComponent.h>
#include <qsf/renderer/map/GridSceneManager.h>
#include <qsf/renderer/map/GridSceneNode.h>
#include <qsf/renderer/map/QsfOgreEntity.h>
#include <qsf/renderer/utility/MeterBoxComponent.h>
#include <qsf/link/link/prototype/ContainerLink.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/map/component/MapPropertiesComponent.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>
#include <qsf/math/Random.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/message/MessageConfiguration.h>
#include <qsf/prototype/PrototypeManager.h>
#include <qsf/platform/PlatformSystem.h>
#include <qsf/base/ScratchBuffer.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace detail
	{
		// List of all civil person prefabs, used at least by EntityHelper:isValidGangsterEventTarget
		static const std::vector<std::string> civilPersonPool = {
			"em5/prefab/person/civil_business_female01",
			"em5/prefab/person/civil_business_female02",
			"em5/prefab/person/civil_business_male02",
			"em5/prefab/person/civil_casual_female01",
			"em5/prefab/person/civil_casual_female02",
			"em5/prefab/person/civil_casual_female03",
			"em5/prefab/person/civil_casual_female04",
			"em5/prefab/person/civil_casual_male02",
			"em5/prefab/person/civil_casual_male03",
			"em5/prefab/person/civil_business_male01",
			"em5/prefab/person/civil_casual_male04",
			"em5/prefab/person/civil_casual_male05",
			"em5/prefab/person/civil_casual_male06",
			"em5/prefab/person/civil_casual_male07",
			"em5/prefab/person/civil_sportive_female02"
		};
	}

	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	void EntityHelper::setupEntityAsGhost(qsf::Entity& entity, uint32 playerIndex)
	{
		Game* game = Game::getInstance();
		if (nullptr != game && game->isRunning() && EM5_NETWORK.isMultiplayActive())
		{
			// Multiplayer session instance must be valid here
			if (EM5_NETWORK.getMultiplayerSession()->isUsingPlayerSeparation())
			{
				GhostComponent* ghostComponent = entity.getOrCreateComponent<GhostComponent>();
				if (nullptr != ghostComponent)
				{
					ghostComponent->setPlayerId(playerIndex);
				}
			}
		}
	}

	void EntityHelper::setupEntityAsGhost(qsf::Entity& entity, const qsf::Entity& parentEntity)
	{
		Game* game = Game::getInstance();
		if (nullptr != game && game->isRunning() && EM5_NETWORK.isMultiplayActive())
		{
			GhostComponent* ghostComponent = parentEntity.getComponent<GhostComponent>();
			if (nullptr != ghostComponent)
			{
				setupEntityAsGhost(entity, ghostComponent->getPlayerId());
			}
		}
	}

	void EntityHelper::teardownEntityAsGhost(qsf::Entity& entity)
	{
		Game* game = Game::getInstance();
		if (nullptr != game && game->isRunning() && EM5_NETWORK.isMultiplayActive())
		{
			GhostComponent* ghostComponent = entity.getComponent<GhostComponent>();
			if (nullptr != ghostComponent)
			{
				entity.destroyComponent(*ghostComponent);

				// Also destroy ground marker
				entity.destroyComponent<GroundMarkerComponent>();

				// If the entity is a unit, reactivate it as collision
				// We can do it here because only for injured/damaged units the ghost effect is removed
				CommandableComponent* commandableComponent = entity.getComponent<CommandableComponent>();
				if (nullptr != commandableComponent)
				{
					qsf::ai::RouterComponent* routerComponent = entity.getComponent<qsf::ai::RouterComponent>();
					if (nullptr != routerComponent)
					{
						qsf::ai::EActor* actor = routerComponent->getActor();
						if (nullptr != actor && actor->GetOwnerPlayer() > -1)
						{
							qsf::ai::EM3::Router->SetOwnerPlayer(actor, -1);
						}
					}
				}
			}
		}
	}

	bool EntityHelper::isPositionVisibleForAnyPlayer(const glm::vec3& entityPosition, float additionalRadius)
	{
		// We are doing here a 2D check only
		const glm::vec2 entity2DPosition(entityPosition.x, entityPosition.z);

		const float MAX_SQUARED_DISTANCE = (70.0f + additionalRadius) * (70.0f + additionalRadius);

		// Check if the entity has a specific distance to view position of an active player
		for (const auto& player : EM5_PLAYERS.getAllPlayers())
		{
			const glm::vec3& playerViewPosition = player->getViewPosition();
			const glm::vec2 viewPosition(playerViewPosition.x, playerViewPosition.z);

			if (glm::distance2(entity2DPosition, viewPosition) < MAX_SQUARED_DISTANCE)
			{
				return true;
			}
		}

		// The entity is not "visible" to any player
		return false;
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	EntityHelper::EntityHelper(const qsf::WeakPtr<qsf::Entity>& entity) :
		qsf::EntityHelper(entity.getSafe())
	{
		// This is actually not necessary, as "qsf::WeakPtr<>::getSafe()" does such a check already
		//  -> But this way we get a more specific assert message
		QSF_ASSERT(entity.valid(), "em5::EntityHelper::EntityHelper(): The provided entity must be valid", QSF_REACT_THROW);
	}

	EntityHelper::EntityHelper(uint64 entityId, const qsf::Map& map) :
		qsf::EntityHelper(*map.getEntityById(entityId))
	{
		// This constructor is a bit dangerous and should better be avoided
		QSF_CHECK(nullptr != map.getEntityById(entityId), "em5::EntityHelper::EntityHelper(): The provided entity must be valid", QSF_REACT_THROW);
	}

	bool EntityHelper::isInsideBox(const qsf::TransformComponent& boxTransform, bool yAxisCentered) const
	{
		// Convert the world position coordinates into local coordinates of the box
		// Due this the check gets easy to do because now local position is now relative to the pivot of the box
		// The base transformation of the box (without scale transformation) is a meter box of 1 meter per edge (1.0f)
		// The box is automatically centered around the box entity pivot position on the x and z axis (the edge positions on these axises are -0.5f - 0.5f relative to the pivot position of the entity)
		// Only for the y-Axis this centering can be disabled -> the box starts on y=0.0f (relative to pivot point of the entity) and ends at 1.0f instead of -0.5f to 0.5f

		glm::vec3 localPosition = boxTransform.getTransform().vec3PositionWorldToLocal(getPosition());

		float yMin = yAxisCentered ? -0.5f : 0.0f;
		float yMax = yAxisCentered ? 0.5f : 1.0f;

		if (localPosition.x < -0.5f || localPosition.x > 0.5f)
			return false;

		if (localPosition.y < yMin || localPosition.y > yMax)
			return false;

		if (localPosition.z < -0.5f || localPosition.z > 0.5f)
			return false;

		return true;
	}

	bool EntityHelper::isInsideBox(const qsf::Entity& boxEntity) const
	{
		qsf::TransformComponent* boxTransformComponent = boxEntity.getComponent<qsf::TransformComponent>();
		if (nullptr == boxTransformComponent)
			return false;

		// TODO(fw): This does not work, as meter box components get not loaded inside the game at all
		//  -> So it's best for now to instruct level design to always set the "CenterAtPivot" flag to "false"
		const qsf::MeterBoxComponent* meterBoxComponent = boxEntity.getComponent<qsf::MeterBoxComponent>();
		return isInsideBox(*boxTransformComponent, (nullptr != meterBoxComponent) ? meterBoxComponent->getCenterAtPivot() : false);
	}

	bool EntityHelper::isInGangsterEventArea() const
	{
		MapHelper mapHelper(mEntity.getMap());
		std::pair<qsf::TransformComponent*, bool> gangsterEventArea = mapHelper.getGangsterEventArea();

		QSF_CHECK(nullptr != gangsterEventArea.first, "Could not find gangster event area!", return true);

		return isInsideBox(*gangsterEventArea.first, gangsterEventArea.second);
	}

	bool EntityHelper::isEntityDamaged() const
	{
		DamageComponent* damageComponent = mEntity.getComponent<DamageComponent>();
		if (nullptr != damageComponent)
		{
			VehicleComponent* vehicleComponent = mEntity.getComponent<VehicleComponent>();
			if (nullptr != vehicleComponent)
			{
				// For vehicles, damage level 1 is still okay
				return (vehicleComponent->getDamageState() >= DamageComponent::DAMAGESTATE_DAMAGE_LEVEL2);
			}
			else
			{
				return (damageComponent->getDamageState() != DamageComponent::DAMAGESTATE_INTACT);
			}
		}

		HealthComponent* healthComponent = mEntity.getComponent<HealthComponent>();
		if (nullptr != healthComponent)
		{
			return healthComponent->isInjured();
		}

		// Default: Not damaged
		return false;
	}

	void EntityHelper::applyPlacement(bool warpMovable) const
	{
		qsf::PlacementComponent* placementComponent = mEntity.getComponent<qsf::PlacementComponent>();
		if (nullptr != placementComponent)
		{
			qsf::TransformComponent* transformComponent = mEntity.getComponent<qsf::TransformComponent>();
			if (nullptr != transformComponent)
			{
				glm::vec3 position = transformComponent->getPosition();
				glm::quat rotation = transformComponent->getRotation();
				placementComponent->applyPlacement(position, rotation);
				transformComponent->setPositionAndRotation(position, rotation);
				if (warpMovable)
				{
					qsf::MovableComponent* movableComponent = mEntity.getComponent<qsf::MovableComponent>();
					if (movableComponent != nullptr)
					{
						movableComponent->warpToPositionAndRotation(position, rotation);
					}
				}
			}
		}
	}

	void EntityHelper::applyPlacementToPosition(glm::vec3& position) const
	{
		qsf::PlacementComponent* placementComponent = mEntity.getComponent<qsf::PlacementComponent>();
		if (nullptr != placementComponent)
		{
			glm::quat rotation;
			placementComponent->applyPlacement(position, rotation);
		}
	}

	void EntityHelper::setPositionForMultiplay(glm::vec3& position) const
	{
		glm::quat rotation = qsf::Math::GLM_QUAT_IDENTITY;

		const qsf::TransformComponent* transformComponent = mEntity.getComponent<qsf::TransformComponent>();
		if (nullptr != transformComponent)
		{
			rotation = transformComponent->getRotation();
		}

		qsf::MovableComponent* movableComponent = mEntity.getOrCreateComponent<qsf::MovableComponent>();
		if (nullptr != movableComponent)
		{
			movableComponent->warpToPositionAndRotation(position, rotation);
		}
	}

	qsf::GameLogic* EntityHelper::getGameLogicByTypeId(uint32 gameLogicTypeId) const
	{
		qsf::GameLogicComponent* gameLogicComponent = mEntity.getComponent<qsf::GameLogicComponent>();
		if (nullptr != gameLogicComponent)
		{
			return gameLogicComponent->getGameLogicByTypeId(gameLogicTypeId);
		}
		return nullptr;
	}

	bool EntityHelper::destroyGameLogicByTypeId(uint32 gameLogicTypeId) const
	{
		qsf::GameLogicComponent* gameLogicComponent = mEntity.getComponent<qsf::GameLogicComponent>();
		if (nullptr != gameLogicComponent)
		{
			qsf::GameLogic* gameLogic = gameLogicComponent->getGameLogicByTypeId(gameLogicTypeId);
			if (nullptr != gameLogic)
			{
				gameLogicComponent->deleteGameLogic(*gameLogic);
				return true;
			}
		}
		return false;
	}

	bool EntityHelper::isEntityValidEventTarget(FreeplayEvent* freeplayEvent) const
	{
		// Entity must not have an event ID already
		if (checkForEventIdComponent())
		{
			// If we have already a event id component with a freeplay event set check if it is the same as the given one -> the entity is a valid event target
			const EventIdComponent* eventIdComponent = mEntity.getComponent<EventIdComponent>();
			if (eventIdComponent->getFreeplayEvent() != freeplayEvent)
			{
				return false;
			}
		}

		// The entity is marked as not ready for events
		if (nullptr != mEntity.getComponent<BlockedForEventComponent>())
			return false;

		// Fade effect is evil, it means spawning or despawning or some specific interaction, skip them for safety
		if (nullptr != mEntity.getComponent<FadeEffectComponent>())
			return false;

		// Entity must be inside the map boundaries
		if (!isInEventBoundaries())
			return false;

		// Make sure the entity is not hidden
		if (isEntityHidden())
			return false;

		// No injured persons
		if (isPersonInjured())
			return false;

		// No buried persons
		if (isEntityBuried())
			return false;

		if (nullptr != getContainerEntity())
			return false;

		if (nullptr != mEntity.getComponent<CallForRescueComponent>())
			return false;

		if (CollisionHelper(mEntity.getMap()).isEntityUnderBridge(mEntity))
			return false;

		// Entity is a valid event target
		return true;
	}

	void EntityHelper::makeImmovable() const
	{
		mEntity.destroyComponent<RandomMovementComponent>();
		mEntity.destroyComponent<MoveToDespawnComponent>();
		qsf::ai::NavigationComponent* navigationComponent = mEntity.getComponent<qsf::ai::NavigationComponent>();
		if (nullptr != navigationComponent)
		{
			navigationComponent->setActive(false);
		}
	}

	void EntityHelper::setInvincible(bool invincible) const
	{
		HealthComponent* healthComponent = mEntity.getComponent<HealthComponent>();
		if (nullptr != healthComponent)
		{
			healthComponent->setInvincible(invincible);
		}

		FireComponent* fireComponent = mEntity.getComponent<FireComponent>();
		if (nullptr != fireComponent)
		{
			// Enable/Disable the ability to receive fire damage
			fireComponent->setActive(invincible);
		}

		VehicleComponent* vehicleComponent = mEntity.getComponent<VehicleComponent>();
		if (nullptr != vehicleComponent)
		{
			vehicleComponent->setInvincible(invincible);
		}
	}


	//[-------------------------------------------------------]
	//[ Camera & Visibility                                   ]
	//[-------------------------------------------------------]
	bool EntityHelper::isEntityHidden() const
	{
		return qsf::game::HiddenComponent::isHidden(mEntity);
	}

	bool EntityHelper::isEntityVisibleOnScreen(bool includeShadow) const
	{
		const qsf::RendererComponent* rendererComponent = mEntity.getComponent<qsf::RendererComponent>();
		if (nullptr != rendererComponent)
		{
			// We only support QSF OGRE entities, so static cast is safe in here
			qsf::QsfOgreEntity* qsfOgreEntity = static_cast<qsf::QsfOgreEntity*>(rendererComponent->getOgreEntity());
			if (nullptr != qsfOgreEntity)
			{
				const qsf::Time& lastRenderedTimestamp = includeShadow ? qsfOgreEntity->getLastRenderedTimestamp() : qsfOgreEntity->getLastNormalRenderedTimestamp();
				const qsf::Time timeDifference = static_cast<qsf::GridSceneManager*>(qsfOgreEntity->_getManager())->getLastRenderedTimestamp() - lastRenderedTimestamp;
				return (timeDifference.getMilliseconds() < 100);
			}
		}

		// Not visible
		return false;
	}

	bool EntityHelper::isEntityVisibleToCamera(qsf::CameraControlComponent& cameraControlComponent) const
	{
		// TODO(mk) Rework this function, based on "isPositionVisibleForAnyPlayer()"
		qsf::TransformComponent* transformComponent = mEntity.getComponent<qsf::TransformComponent>();
		QSF_WARN_IF(nullptr == transformComponent, "Tried to get visibility information for an entity, which has no transform component", QSF_REACT_NONE);
		if (nullptr == transformComponent)
			return false;

		static const float MAX_DISTANCE = 60.0f;
		return (glm::distance2(cameraControlComponent.getTargetPosition(), transformComponent->getPosition()) < MAX_DISTANCE * MAX_DISTANCE);
	}

	bool EntityHelper::isEntityVisibleForAnyPlayer() const
	{
		const qsf::TransformComponent* transformComponent = mEntity.getComponent<qsf::TransformComponent>();
		QSF_WARN_IF(nullptr == transformComponent, "Tried to get visibility information for an entity, which has no transform component", QSF_REACT_NONE);
		return (nullptr == transformComponent) ? false : isPositionVisibleForAnyPlayer(transformComponent->getPosition());
	}

	bool EntityHelper::isInMapBoundaries() const
	{
		// Use map helper function
		qsf::Map& map = mEntity.getMap();
		return MapHelper(map).isInMapBoundaries(getPosition());
	}

	bool EntityHelper::isInEventBoundaries() const
	{
		// Use map helper function
		qsf::Map& map = mEntity.getMap();
		return MapHelper(map).isInEventBoundaries(getPosition());
	}

	bool EntityHelper::isCloseToEventBoundaries(float tolerance) const
	{
		// Use map helper function
		qsf::Map& map = mEntity.getMap();
		return MapHelper(map).isEntityCloseToEventBoundaries(mEntity, tolerance);
	}

	bool EntityHelper::isCloseToMapBoundaries(float tolerance) const
	{
		// Use map helper function
		qsf::Map& map = mEntity.getMap();
		return MapHelper(map).isEntityCloseToMapBoundaries(mEntity, tolerance);
	}


	//[-------------------------------------------------------]
	//[ Persons                                               ]
	//[-------------------------------------------------------]
	bool EntityHelper::isEntityPerson() const
	{
		// Is it a person of any kind?
		PersonComponent* personComponent = mEntity.getComponent<PersonComponent>();
		if (nullptr != personComponent && personComponent->getGender() != PersonComponent::GENDER_ANIMAL)
		{
			return true;
		}
		return false;
	}

	bool EntityHelper::isEntityAnimal() const
	{
		// Is it a person of any kind?
		PersonComponent* personComponent = mEntity.getComponent<PersonComponent>();
		if (nullptr != personComponent && personComponent->getGender() == PersonComponent::GENDER_ANIMAL)
		{
			return true;
		}
		return false;
	}

	bool EntityHelper::isCivilPerson() const
	{
		// Check if it is person at all
		PersonComponent* personComponent = mEntity.getComponent<PersonComponent>();
		if (nullptr != personComponent && personComponent->getGender() != PersonComponent::GENDER_ANIMAL)
		{
			// Not a gangster -- double check this, as it might be a gangster without flag
			if (!personComponent->getGangsterFlag() && nullptr == getGameLogic<GangsterBaseLogic>())
			{
				// Non controllable...
				if (nullptr == mEntity.getComponent<CommandableComponent>())
				{
					return true;
				}
			}
		}
		return false;
	}

	bool EntityHelper::isGangsterPerson() const
	{
		// Check if it is person at all
		PersonComponent* personComponent = mEntity.getComponent<PersonComponent>();
		if (nullptr != personComponent && personComponent->getGender() != PersonComponent::GENDER_ANIMAL)
		{
			// Check if gangster -- double check this, as it might be a gangster without flag
			if (personComponent->getGangsterFlag() || nullptr != getGameLogic<GangsterBaseLogic>())
			{
				return true;
			}
		}
		return false;
	}

	bool EntityHelper::isGangsterCandidate(GangsterPool gangsterPoolType, bool mustBeInGangsterEventArea) const
	{
		if (!isEntityValidEventTarget())
			return false;

		// Check if entity is a correctly initialized person
		if (!isCivilPerson())
			return false;

		// Check if person is still healthy and not injured
		if (isPersonInjured())
			return false;

		// Valid civil persons have a MoveToDespawnComponent (i.e. they are walking around...)
		MoveToDespawnComponent* moveToDespawnComponent = mEntity.getComponent<MoveToDespawnComponent>();

		if (nullptr == moveToDespawnComponent)
		{
			// No DespawnComp
			return false;
		}

		// And it is active
		if (!moveToDespawnComponent->isActive())
		{
			// Inactive DespawnComp
			return false;
		}

		// Valid civil persons have a NavigationComponent
		qsf::ai::NavigationComponent* navigationComponent = mEntity.getComponent<qsf::ai::NavigationComponent>();

		if (nullptr == navigationComponent)
		{
			// No NavigationComponent
			return false;
		}

		// And it is active
		if (!navigationComponent->isActive())
		{
			// Inactive NavigationComponent
			return false;
		}

		if (CollisionHelper(mEntity.getMap()).isInWater(mEntity))
		{
			// Target is in water
			return false;
		}

		if (mustBeInGangsterEventArea && !isInGangsterEventArea())
		{
			// Since 26.08.14, gangster events shall take place within a certain gangster event area
			return false;
		}

		std::string gangsterPoolName = MapHelper(mEntity.getMap()).getSpawnpoolNameFromGangsterPool(gangsterPoolType);

		qsf::MetadataComponent* metadataComponent = mEntity.getComponent<qsf::MetadataComponent>();
		if (nullptr == metadataComponent)
		{
			return false;
		}
		const std::string& entityPrefab = metadataComponent->getBasePrefab().getLocalAssetName();

		SpawnPoolManager::SpawnInstanceDataPointer spawnInstanceData = nullptr;
		if (!EM5_GAME.getSimulation().getSpawnPoolManager().getSpawnpoolInstance(gangsterPoolName, entityPrefab, &spawnInstanceData))
		{
			// Prefab is not from gangster Pool
			return false;
		}

		return true;
	}

	bool EntityHelper::isValidGangsterEventTarget(qsf::Entity& gangsterEntity, bool needsToBeCivilTarget) const
	{
		// Store information on whether entity is civil person as it is needed later one
		bool civilPersonFlag = isCivilPerson();

		if (needsToBeCivilTarget)
		{
			// Exclude non-civil targets
			if (!civilPersonFlag && !isCivilRoadVehicle())
				return false;
		}

		// Exclude targets that are part of an event
		if (!isEntityValidEventTarget())
			return false;

		// No suicide!
		if (mEntity.getId() == gangsterEntity.getId())
			return false;

		VehicleComponent* vehicle = mEntity.getComponent<VehicleComponent>();
		if (nullptr != vehicle)
		{
			// Vehicle

			// Exclude burning vehicles
			if (vehicle->isBurning())
				return false;

			// Exclude vehicles that are damaged too much
			if (vehicle->isDamagedLevel2())
				return false;

			// Exclude destroyed vehicles
			if (vehicle->isDestroyed())
				return false;

			HelicopterComponent* heliComp = mEntity.getComponent<HelicopterComponent>();
			if (nullptr != heliComp)
			{
				// This is a helicopter
				if (heliComp->isFlying())
				{
					// Exclude flying helicopters
					return false;
				}
			}
		}
		else
		{
			PersonComponent* personComponent = mEntity.getComponent<PersonComponent>();
			if (nullptr != personComponent)
			{
				// No children
				if (personComponent->getAge() == PersonComponent::AGE_CHILD)
					return false;

				// No seniors
				if (personComponent->getAge() == PersonComponent::AGE_SENIOR)
					return false;

				// Disallow persons in vehicles or buildings (The gangster shall either attack cars or walking persons, but not persons inside containers)
				if (nullptr != getContainerEntity())
				{
					return false;
				}

				if (needsToBeCivilTarget)
				{
					qsf::MetadataComponent* metaDataComponent = mEntity.getComponent<qsf::MetadataComponent>();

					// Unlikely, but better check than be sorry
					if (metaDataComponent == nullptr)
						return false;

					// Get prefab name
					const std::string& entityPrefab = metaDataComponent->getBasePrefab().getLocalAssetName();

					// Check whether person is from a certain pool
					if (detail::civilPersonPool.end() == std::find(detail::civilPersonPool.begin(), detail::civilPersonPool.end(), entityPrefab))
					{
						// Not from pool!
						return false;
					}
				}
			}

			HealthComponent* healthComponent = mEntity.getComponent<HealthComponent>();
			if (nullptr != healthComponent)
			{
				// Exclude injured and dead persons
				if (healthComponent->isInjured() || healthComponent->isDead())
					return false;

				// Don't shoot on loaded humans
				if (isCarryingAnotherPerson())
					return false;
			}
		}

		if (needsToBeCivilTarget)
		{
			// Check whether target has a MoveToDespawnComponent
			MoveToDespawnComponent* moveToDespawnComponent = mEntity.getComponent<MoveToDespawnComponent>();

			if (nullptr == moveToDespawnComponent)
			{
				// No DespawnComp
				return false;
			}

			if (!moveToDespawnComponent->isActive())
			{
				// Inactive DespawnComp
				return false;
			}

			// Valid civil persons have a NavigationComponent
			qsf::ai::NavigationComponent* navigationComponent = mEntity.getComponent<qsf::ai::NavigationComponent>();

			if (nullptr == navigationComponent)
			{
				// No NavigationComponent
				return false;
			}

			// And it is active
			if (!navigationComponent->isActive())
			{
				// Inactive NavigationComponent
				return false;
			}
		}

		if (CollisionHelper(mEntity.getMap()).isInWater(mEntity))
		{
			// Target is in water
			return false;
		}

		// Assume target is valid
		return true;
	}

	bool EntityHelper::isStunned() const
	{
		qsf::ActionComponent* actionComponent = mEntity.getOrCreateComponent<qsf::ActionComponent>();
		return nullptr != actionComponent->getAction<BeStunnedAction>();
	}

	bool EntityHelper::isCommandable() const
	{
		return (nullptr != mEntity.getComponent<CommandableComponent>());
	}

	bool EntityHelper::isSquadPerson() const
	{
		// Squads are persons with commandable component
		//  -> Note that rescue dog and police dog count as persons here as well
		return (nullptr != mEntity.getComponent<CommandableComponent>() && nullptr != mEntity.getComponent<PersonComponent>());
	}

	bool EntityHelper::isPersonInjured() const
	{
		const HealthComponent* healthComponent = mEntity.getComponent<HealthComponent>();

		// No assert please, it's just no person so it's not an injured person
		return (healthComponent == nullptr) ? false : healthComponent->isInjured();
	}

	bool EntityHelper::isPersonContaminated() const
	{
		const HealthComponent* healthComponent = mEntity.getComponent<HealthComponent>();

		// No assert please, it's just no person so it's not an injured person
		return (healthComponent == nullptr) ? false : healthComponent->isContaminated();
	}

	bool EntityHelper::isPersonWaving() const
	{
		const PersonComponent* person = mEntity.getComponent<PersonComponent>();
		QSF_CHECK(nullptr != person, "The entity " << mEntity.getId() << " has no health component", return false);
		return person->getWavesOutOfBurningHouse();
	}

	bool EntityHelper::isPersonBuried() const
	{
		if (mEntity.getComponent<PersonComponent>() == nullptr)
			return false;

		return isEntityBuried();
	}

	bool EntityHelper::isCarrying(CarryLoad type) const
	{
		if ((type & CARRY_PERSON) && isCarryingAnotherPerson())
			return true;

		if ((type & PULL_PERSON) && isPullingPerson())
			return true;

		if ((type & PULL_OBJECT) && isPullingObject())
			return true;

		return false;
	}

	uint64 EntityHelper::getCarryTarget(CarryLoad type) const
	{
		// Some funny guys really messed this up
		const qsf::GameLogicComponent* gameLogicComponent = mEntity.getComponent<qsf::GameLogicComponent>();
		if (nullptr != gameLogicComponent)
		{
			if (nullptr != EM5_NETWORK.getMultiplayerClient())
			{
				// Special handling for multiplayer client
				// We misuse the carry person logic (which is already synchronized) to hold the information
				if (type > 0)
				{
					type = CARRY_PERSON;
				}
			}

			if (type & CARRY_PERSON)
			{
				// Get the carry person logic
				const CarryPersonLogic* carryPersonLogic = gameLogicComponent->getGameLogic<CarryPersonLogic>();
				if (nullptr != carryPersonLogic)
				{
					uint64 result = carryPersonLogic->getCarryPerson();
					if (qsf::isInitialized(result))
						return result;
				}

				DiverLogic* diverLogic = gameLogicComponent->getGameLogic<DiverLogic>();
				if (nullptr != diverLogic)
				{
					uint64 result = diverLogic->getCarryPerson();
					if (qsf::isInitialized(result))
						return result;
				}
			}

			if ((type & PULL_PERSON))
			{
				// Get the fireman logic (only one to pull persons)
				const FiremanLogic* firemanLogic = gameLogicComponent->getGameLogic<FiremanLogic>();
				if (nullptr != firemanLogic)
				{
					uint64 result = firemanLogic->getPullingPersonId();
					if (qsf::isInitialized(result))
						return result;
				}
			}

			if ((type & PULL_OBJECT))
			{
				// Get the pull object logic
				const PullObjectLogic* objectLogic = gameLogicComponent->getGameLogic<PullObjectLogic>();
				if (nullptr != objectLogic)
				{
					qsf::Entity* pulledEntity = objectLogic->getPulledObject();
					if (pulledEntity != nullptr)
						return pulledEntity ->getId();
				}
			}
		}

		return qsf::getUninitialized<uint64>();
	}

	void EntityHelper::clearCarryTarget(CarryLoad type) const
	{
		const qsf::GameLogicComponent* gameLogicComponent = mEntity.getComponent<qsf::GameLogicComponent>();
		if (nullptr != gameLogicComponent)
		{
			if ((type & CARRY_PERSON))
			{
				// Get the carry person logic
				CarryPersonLogic* carryPersonLogic = gameLogicComponent->getGameLogic<CarryPersonLogic>();
				if (nullptr != carryPersonLogic)
				{
					carryPersonLogic->setCarryPerson(qsf::getUninitialized<uint64>());
				}
			}

			if ((type & PULL_PERSON))
			{
				// Get the fireman logic (only one to pull persons)
				FiremanLogic* firemanLogic = gameLogicComponent->getGameLogic<FiremanLogic>();
				if (nullptr != firemanLogic)
				{
					firemanLogic->setPullingPersonId(qsf::getUninitialized<uint64>());
				}
			}

			if ((type & PULL_OBJECT))
			{
				// Get the pull object logic
				PullObjectLogic* objectLogic = gameLogicComponent->getGameLogic<PullObjectLogic>();
				if (nullptr != objectLogic)
				{
					objectLogic->setPulledObject(nullptr);
				}
			}
		}

		// Force animation update
		AnimationHelper(mEntity).clearAnimation();
	}

	bool EntityHelper::isCarryingAnotherPerson() const
	{
		const qsf::GameLogicComponent* gameLogicComponent = mEntity.getComponent<qsf::GameLogicComponent>();
		if (nullptr != gameLogicComponent)
		{
			// Get the carry person logic
			const CarryPersonLogic* carryPersonLogic = gameLogicComponent->getGameLogic<CarryPersonLogic>();
			if (nullptr != carryPersonLogic)
			{
				// Check if the entity is already carry a person
				return qsf::isInitialized(carryPersonLogic->getCarryPerson());
			}
		}
		return false;
	}

	bool EntityHelper::isPullingObject() const
	{
		// Get the game logic component
		const qsf::GameLogicComponent* gameLogic = mEntity.getComponent<qsf::GameLogicComponent>();
		if (nullptr != gameLogic)
		{
			// Get the pull object logic
			const PullObjectLogic* objectLogic = gameLogic->getGameLogic<PullObjectLogic>();
			if (nullptr != objectLogic)
			{
				if (objectLogic->isPullingObject())
				{
					return true;
				}
			}
		}

		return false;
	}

	bool EntityHelper::isCarriedByOtherPerson() const
	{
		// Ask link component for existing parent
		qsf::LinkComponent* linkComponent = mEntity.getComponent<qsf::LinkComponent>();
		if (nullptr != linkComponent && nullptr != linkComponent->getParentPrototype())
		{
			qsf::Entity* parentEntity = static_cast<qsf::Entity*>(linkComponent->getParentPrototype());
			// Match parents carry target with ourself?
			return mEntity.getId() == EntityHelper(*parentEntity).getCarryTarget(EntityHelper::CarryLoad::ANY);
		}

		return false;
	}

	bool EntityHelper::isPullingPerson() const
	{
		// Get the game logic component
		const qsf::GameLogicComponent* gameLogic = mEntity.getComponent<qsf::GameLogicComponent>();
		if (nullptr != gameLogic)
		{
			// Get the fireman logic (only one to pull persons)
			const FiremanLogic* firemanLogic = gameLogic->getGameLogic<FiremanLogic>();
			if (nullptr != firemanLogic)
			{
				return qsf::isInitialized(firemanLogic->getPullingPersonId());
			}
		}

		return false;
	}

	bool EntityHelper::isGuidingPerson() const
	{
		qsf::ActionComponent* actionComponent = mEntity.getComponent<qsf::ActionComponent>();
		if (nullptr != actionComponent)
		{
			// We will guide someone into a long holiday soon
			if (nullptr != actionComponent->getAction<ArrestGangsterAction>())
				return false;

			qsf::Action* action = actionComponent->getCurrentAction();
			if (nullptr != action)
			{
				return action->getTypeId() == GuidePersonAction::ACTION_ID;
			}
		}
		return false;
	}

	bool EntityHelper::isSquadPersonInValidState() const
	{
		if (!isSquadPerson() || isPersonInjured() || !isUnitInValidState())
			return false;

		return true;
	}

	bool EntityHelper::isSquadVehicleInValidState() const
	{
		if (!isSquadVehicle() || isEntityDamaged() || !isUnitInValidState())
			return false;

		return true;
	}

	bool EntityHelper::isSquadUnitInValidState() const
	{
		return (isSquadPersonInValidState() || isSquadVehicleInValidState());
	}

	bool EntityHelper::isCivilPersonInValidState() const
	{
		if (isSquadPerson())
			return false;

		if (!isEntityPerson() || isPersonInjured() || qsf::game::HiddenComponent::isHidden(mEntity) || !isInMapBoundaries() || nullptr != getContainerEntity() || isEntityHardLinked())
			return false;

		CallForRescueComponent* callForRescueComponent = mEntity.getComponent<CallForRescueComponent>();
		if (nullptr != callForRescueComponent)
			return false;

		return true;
	}

	bool EntityHelper::isUnitInValidState() const
	{
		if (qsf::game::HiddenComponent::isHidden(mEntity) || !isInMapBoundaries() || nullptr != getContainerEntity() || isEntityHardLinked())
			return false;

		CommandableComponent* commandableComponent = mEntity.getComponent<CommandableComponent>();
		if (nullptr == commandableComponent)
			return false;

		if (!commandableComponent->isActive())
			return false;

		return true;
	}

	bool EntityHelper::isUnitInValidAndHealthyState() const
	{
		return (isUnitInValidState() && !isPersonInjured() && !isEntityDamaged());
	}

	bool EntityHelper::isBuildingInValidState() const
	{
		if (!isEntityBuilding() || isBuildingDamaged() || isBuildingDestroyed())
			return false;

		return true;
	}

	void EntityHelper::disconnectFromHydrant() const
	{
		qsf::GameLogicComponent* gameLogicComponent = mEntity.getComponent<qsf::GameLogicComponent>();
		if (nullptr != gameLogicComponent)
		{
			// Get the fireman logic
			FiremanLogic* firemanLogic = gameLogicComponent->getGameLogic<FiremanLogic>();
			if (nullptr != firemanLogic)
			{
				firemanLogic->destroyFireHose();
				firemanLogic->disconnectFromHydrant();
			}
		}
	}

	bool EntityHelper::isArrested() const
	{
		qsf::GameLogicComponent* gameLogicComponent = mEntity.getComponent<qsf::GameLogicComponent>();
		if (nullptr == gameLogicComponent)
			return false;

		GangsterBaseLogic* gangsterBaseLogic = gameLogicComponent->getGameLogic<GangsterBaseLogic>();
		if (nullptr != gangsterBaseLogic && gangsterBaseLogic->getCurrentState() == GangsterBaseLogic::STATE_ARRESTED)
			return true;

		PersonComponent* personComponent = mEntity.getComponent<PersonComponent>();
		if (personComponent != nullptr && personComponent->getHandcuffedFlag())
			return true;

		return false;
	}

	bool EntityHelper::canUseUmbrella() const
	{
		PersonComponent* personComponent = mEntity.getComponent<PersonComponent>();
		if (nullptr != personComponent && personComponent->getCanUseUmbrella() && !personComponent->getGangsterFlag())	// Check here for gangster, use the person component and ask the flag for speed up the process (otherwise we would call "isGangsterPerson()")
		{
			// Ignore persons with pre-edited animations
			if (hasEnabledMeshAnimationTestComponent())
				return false;

			// Ignore persons with specific animations
			if (nullptr != mEntity.getComponent<GaperComponent>())
				return false;

			if (isCivilPerson() && !isPersonInjured() && nullptr == getContainerEntity() && !isEntityHardLinked() && nullptr == mEntity.getComponent<EventIdComponent>())
			{
				// Only entity without equipment can use an umbrella
				if (nullptr == getActiveEquipmentEntity() || checkIsActiveEquipment(equipment::UMBRELLA))
				{
					return true;
				}
			}
		}
		return false;
	}

	bool EntityHelper::isRedirectingTraffic() const
	{
		qsf::ActionComponent* actionComponent = mEntity.getComponent<qsf::ActionComponent>();
		if (nullptr != actionComponent)
		{
			const RedirectTrafficAction* redirectTrafficAction = actionComponent->getAction<RedirectTrafficAction>();
			if (nullptr != redirectTrafficAction)
			{
				return true;
			}
		}

		return false;
	}

	bool EntityHelper::canPlaySelectionFeedback() const
	{
		// Police SEK robot gives no response
		if (UnitHelper(mEntity).isPoliceSEKR())
			return false;

		// All others are okay
		return isUnitInValidAndHealthyState();
	}

	bool EntityHelper::canPlayCommandFeedback() const
	{
		// Police SEK robot gives no response
		if (UnitHelper(mEntity).isPoliceSEKR())
			return false;

		// All others are okay
		return isUnitInValidAndHealthyState();
	}

	bool EntityHelper::isGangsterFleeing() const
	{
		// The method is mostly identical with GangsterBaseLogic::isCurrentlyFleeing()

		qsf::ActionComponent* actionComponent = mEntity.getOrCreateComponent<qsf::ActionComponent>();

		if (!actionComponent->getPlan().isEmpty())
		{
			// There are actions enqueued

			uint32 currentActionTypeId = actionComponent->getCurrentAction()->getTypeId();

			if (EscapeFootLongAction::ACTION_ID == currentActionTypeId ||
				EscapeFootShortAction::ACTION_ID == currentActionTypeId ||
				EscapeCarLongAction::ACTION_ID == currentActionTypeId ||
				EscapeCarShortAction::ACTION_ID == currentActionTypeId ||
				EscapeHidingFootAction::ACTION_ID == currentActionTypeId)
			{
				// Current action is fleeing action
				return true;
			}
		}

		return false;
	}


	//[-------------------------------------------------------]
	//[ Building                                              ]
	//[-------------------------------------------------------]
	bool EntityHelper::isEntityBuilding() const
	{
		return (nullptr != mEntity.getComponent<BuildingComponent>() && nullptr == mEntity.getComponent<IndustrialPlantComponent>());
	}

	bool EntityHelper::isEntityIndustrialPlant() const
	{
		return (nullptr != mEntity.getComponent<BuildingComponent>() && nullptr != mEntity.getComponent<IndustrialPlantComponent>());
	}

	bool EntityHelper::isBuildingDamaged() const
	{
		// TODO(mk) Think about using this function is still necessary, after the damage/container/building update
		const BuildingComponent* buildingComponent = mEntity.getComponent<BuildingComponent>();
		QSF_CHECK(nullptr != buildingComponent, "There is no building component in the entity " << mEntity.getId(), return false);
		BuildingComponent::DamageState currentDamageState = buildingComponent->getDamageState();
		return BuildingComponent::DAMAGESTATE_INTACT != currentDamageState;
	}

	bool EntityHelper::isBuildingDestroyed() const
	{
		// TODO(mk) Think about using this function is still necessary, after the damage/container/building update
		const BuildingComponent* buildingComponent = mEntity.getComponent<BuildingComponent>();
		QSF_CHECK(nullptr != buildingComponent, "There is no building component in the entity " << mEntity.getId(), return false);
		return (BuildingComponent::DAMAGESTATE_DESTROYED == buildingComponent->getDamageState());
	}

	void EntityHelper::placeEntityAtBuildingDoor(qsf::Entity& buildingEntity, bool lookToDoor) const
	{
		// TODO(mk) This function is maybe not the safest. It doesn't check collision or if the building has a door.
		BuildingComponent* buildingComponent = buildingEntity.getComponent<BuildingComponent>();
		if (nullptr == buildingComponent)
			return;

		qsf::MovableComponent* movableComponent = mEntity.getComponent<qsf::MovableComponent>();
		if (nullptr == movableComponent)
			return;

		glm::quat rotation = buildingComponent->getDoorRotation();
		if (lookToDoor)
		{
			rotation *= qsf::Math::getRotationAroundAxis(qsf::CoordinateSystem::getUp(), glm::pi<float>());
		}

		// Ask for a collision free place
		glm::vec3 finalPosition = buildingComponent->getDoorPosition();
		CollisionHelper collisionHelper(mEntity.getMap());
		qsf::Transform transform(finalPosition, rotation);
		const float safetySpace = (isPersonInjured() ? 0.4f : 0.2f);
		if (collisionHelper.hasCollisionEM3(mEntity, finalPosition, safetySpace))
		{
			collisionHelper.findFreePositionEM3Safety(mEntity, transform, 1.5f, 2.5f, 4.0f, 0.4f, finalPosition);
		}

		// Place the entity to the position
		movableComponent->moveToPositionAndRotation(finalPosition, rotation);
	}


	//[-------------------------------------------------------]
	//[ Vehicle											      ]
	//[-------------------------------------------------------]
	bool EntityHelper::isVehicle() const
	{
		return (nullptr != mEntity.getComponent<VehicleComponent>());
	}

	bool EntityHelper::isHelicopter() const
	{
		return (nullptr != mEntity.getComponent<HelicopterComponent>());
	}

	bool EntityHelper::isBoat() const
	{
		const BoatComponent* boatComponent = mEntity.getComponent<BoatComponent>();
		if (nullptr == boatComponent)
			return false;

		CommandableComponent* commandableComponent = mEntity.getComponent<CommandableComponent>();
		if (nullptr == commandableComponent)
			return false;

		return commandableComponent->isFireFighterBoat();
	}

	bool EntityHelper::isShip() const
	{
		const BoatComponent* boatComponent = mEntity.getComponent<BoatComponent>();
		if (nullptr == boatComponent)
			return false;

		CommandableComponent* commandableComponent = mEntity.getComponent<CommandableComponent>();
		if (nullptr == commandableComponent)
			return false;

		return commandableComponent->isFireFighterFLB();
	}

	bool EntityHelper::isFlying() const
	{
		HelicopterComponent* helicopterComponent = mEntity.getComponent<HelicopterComponent>();
		return (helicopterComponent != nullptr) ? helicopterComponent->isFlying() : false;
	}

	bool EntityHelper::isUnit(qsf::NamedIdentifier unitName) const
	{
		const CommandableComponent* commandableComponent = mEntity.getComponent<CommandableComponent>();
		return (nullptr != commandableComponent) ? (unitName == commandableComponent->getUnitType()) : false;
	}

	bool EntityHelper::isVehicleDestroyed() const
	{
		const VehicleComponent* vehicleComponent = mEntity.getComponent<VehicleComponent>();
		return (vehicleComponent != nullptr) ? vehicleComponent->isDestroyed() : false;
	}


	//[-------------------------------------------------------]
	//[ Road vehicle                                          ]
	//[-------------------------------------------------------]
	bool EntityHelper::isRoadVehicle() const
	{
		return (nullptr != mEntity.getComponent<RoadVehicleComponent>());
	}

	bool EntityHelper::isCivilRoadVehicle() const
	{
		if (nullptr == mEntity.getComponent<RoadVehicleComponent>())
			return false;

		// Civil road vehicle has no commandable component
		return (mEntity.getComponent<CommandableComponent>() == nullptr);
	}

	bool EntityHelper::isSquadRoadVehicle() const
	{
		if (nullptr == mEntity.getComponent<RoadVehicleComponent>())
			return false;

		// Civil road vehicle has no commandable component
		return (mEntity.getComponent<CommandableComponent>() != nullptr);
	}

	bool EntityHelper::isSquadVehicle() const
	{
		if (nullptr == mEntity.getComponent<VehicleComponent>())
			return false;

		// Civil road vehicle has no commandable component
		return (mEntity.getComponent<CommandableComponent>() != nullptr);
	}

	bool EntityHelper::isGangsterRoadVehicle() const
	{
		if (!isCivilRoadVehicle())
			return false;

		RoadVehicleComponent& roadVehicleComponent = mEntity.getComponentSafe<RoadVehicleComponent>();

		// Find the gangster contained in there
		for (auto& iterator : roadVehicleComponent.getLinks())
		{
			const qsf::ContainerLink& containerLink = *iterator.second;
			qsf::Entity& linkedEntity = containerLink.getTargetEntity();

			if (EntityHelper(linkedEntity).isGangsterPerson())
			{
				return true;
			}
		}

		return false;
	}

	bool EntityHelper::isRoadVehicleDamaged() const
	{
		const RoadVehicleComponent* roadVehicleComponent = mEntity.getComponent<RoadVehicleComponent>();
		if (roadVehicleComponent == nullptr)	// No assert please, if it's no vehicle it's just not a damaged vehicle
			return false;

		return !roadVehicleComponent->isIntact() && !roadVehicleComponent->isDestroyed();
	}

	bool EntityHelper::checkVehicleHasFreeSeats(qsf::Entity& vehicleEntity) const
	{
		// Get order info
		OrderInfo* targetVehicleOrderInfo = EntityHelper(vehicleEntity).getOrderInfo();
		if (nullptr != targetVehicleOrderInfo)
		{
			// Injured unit persons are treated as civilians here
			//  -> Note that there is also the SEK robot, which is no person, but should be handled the same way
			CommandableComponent* commandableComponent = mEntity.getComponent<CommandableComponent>();
			if (nullptr != commandableComponent && !isPersonInjured())
			{
				const OrderInfo* callerOrderInfo = EM5_GAME.getOrderInfoManager().findElement(commandableComponent->getUnitType());
				if (nullptr != callerOrderInfo)
				{
					return checkVehicleHasFreeSeatsForSquadPerson(vehicleEntity, *targetVehicleOrderInfo, callerOrderInfo->getName());
				}
			}
			else
			{
				return checkVehicleHasFreeSeatsForPassengerPerson(vehicleEntity, *targetVehicleOrderInfo);
			}
		}

		// Check fails
		return false;
	}

	bool EntityHelper::checkVehicleHasFreeSeatsForSquadPerson(qsf::Entity& vehicleEntity, OrderInfo& targetVehicleOrderInfo, const std::string& callerUnitName) const
	{
		return countVehicleFreeSeatsForSquadPerson(vehicleEntity, targetVehicleOrderInfo, callerUnitName) > 0;
	}

	bool EntityHelper::checkVehicleHasFreeSeatsForPassengerPerson(qsf::Entity& vehicleEntity, OrderInfo& targetVehicleOrderInfo) const
	{
		return countVehicleFreeSeatsForPassengerPerson(vehicleEntity, targetVehicleOrderInfo) > 0;
	}

	int EntityHelper::countVehicleFreeSeatsForSquadPerson(qsf::Entity& vehicleEntity, OrderInfo& targetVehicleOrderInfo, const std::string& callerUnitName) const
	{
		// First check if person type may enter this vehicle at all
		if (targetVehicleOrderInfo.supportsPersonnel(callerUnitName))
		{
			VehicleComponent* vehicleComponent = vehicleEntity.getComponent<VehicleComponent>();
			if (nullptr != vehicleComponent)
			{
				// Count the personnel already inside vs. the personnel space available
				int availableUnitSeats = targetVehicleOrderInfo.getPersonnelSpace();
				for (auto& iterator : vehicleComponent->getLinks())
				{
					if (iterator.second->mContainerCategory == container::CONTAINERTYPE_SQUAD)
					{
						// Unit of this type is already in vehicle, remove one free seat
						--availableUnitSeats;
					}
				}
				return availableUnitSeats;
			}
		}
		return 0;
	}

	int EntityHelper::countVehicleFreeSeatsForPassengerPerson(qsf::Entity& vehicleEntity, OrderInfo& targetVehicleOrderInfo) const
	{
		VehicleComponent* vehicleComponent = vehicleEntity.getComponent<VehicleComponent>();
		if (nullptr != vehicleComponent)
		{
			// Count the passengers already inside vs. the passengers space available
			int availableSeats = targetVehicleOrderInfo.getPassengerSpace();
			for (auto& iterator : vehicleComponent->getLinks())
			{
				if (iterator.second->mContainerCategory == container::CONTAINERTYPE_PASSENGER)
				{
					// Person of this type is already in vehicle, remove one free seat
					--availableSeats;
				}
			}
			return availableSeats;
		}
		return 0;
	}

	RoadVehicleComponent* EntityHelper::getReachableRoadVehicleWithEquipment(const std::string& equipmentName) const
	{
		RoadVehicleComponent* nearestRoadVehicleComponent = nullptr;

		// Get the transform component for the position
		const qsf::TransformComponent* callerTransformComponent = mEntity.getComponent<qsf::TransformComponent>();
		if (nullptr != callerTransformComponent)
		{
			const float searchRadius = ActionRangeSpecsGroup::getInstanceSafe().getSearchRoadVehicleDistance();

			// Position of the caller
			const glm::vec3& callerPosition = callerTransformComponent->getPosition();

			// Search for road vehicle components in the radius
			std::vector<RoadVehicleComponent*> roadVehicleVector;
			qsf::ComponentMapQuery(mEntity.getMap()).getInstancesInCircle(callerPosition, searchRadius, roadVehicleVector);

			// Find the next road vehicle component with the shortest distance
			float squaredSearchRadius = searchRadius * searchRadius;
			const size_t numberOfRoadVehicles = roadVehicleVector.size();
			for (size_t i = 0; i < numberOfRoadVehicles; ++i)
			{
				RoadVehicleComponent* roadVehicle = roadVehicleVector[i];
				qsf::Entity& roadVehicleEntity = roadVehicle->getEntity();
				const float currentSquaredDistance = qsf::game::DistanceHelper::getSquaredDistance(callerPosition, roadVehicleEntity);
				EntityHelper entityHelperVehicle(roadVehicleEntity);
				if (currentSquaredDistance < squaredSearchRadius &&
					entityHelperVehicle.isSquadRoadVehicle() &&
					entityHelperVehicle.hasEquipment(equipmentName))
				{
					// Set new target
					squaredSearchRadius = currentSquaredDistance;
					nearestRoadVehicleComponent = roadVehicle;
				}
			}
		}
		return nearestRoadVehicleComponent;
	}

	bool EntityHelper::hasEquipment(const std::string& equipmentName) const
	{
		// Get the inventory component of the road vehicle and look for the right equipment
		const InventoryComponent* inventoryComponent = mEntity.getComponent<InventoryComponent>();
		if (nullptr != inventoryComponent)
		{
			// Loop over all equipments
			qsf::PrototypeManager& prototypeManager = QSF_MAINPROTOTYPE;
			for (uint64 equipmentGlobalAssetId : inventoryComponent->getEquipments())
			{
				const qsf::Prototype* equipmentPrototype = prototypeManager.getPrefabByGlobalAssetId(equipmentGlobalAssetId, true);
				if (nullptr != equipmentPrototype)
				{
					const qsf::game::EquipmentComponent* equipmentComponent = equipmentPrototype->getComponent<qsf::game::EquipmentComponent>();
					if (nullptr != equipmentComponent && equipmentComponent->getEquipmentName() == equipmentName)
					{
						return true;
					}
				}
			}
		}
		return false;
	}

	//[-------------------------------------------------------]
	//[ Vegetation                                            ]
	//[-------------------------------------------------------]
	bool EntityHelper::isEntityVegetation() const
	{
		if (nullptr != mEntity.getComponent<HedgeComponent>())
			return true;

		if (nullptr != mEntity.getComponent<TreeComponent>())
			return true;

		return false;
	}

	bool EntityHelper::isEntityTree() const
	{
		return (mEntity.getComponent<TreeComponent>() != nullptr);
	}

	//[-------------------------------------------------------]
	//[ Fire                                                  ]
	//[-------------------------------------------------------]
	bool EntityHelper::isBurning() const
	{
		// Complex fire component
		const ComplexFireComponent* complexFireComponent = mEntity.getComponent<ComplexFireComponent>();
		if (nullptr != complexFireComponent)
		{
			return complexFireComponent->isBurning();
		}

		// Fire component
		const FireComponent* fireComponent = mEntity.getComponent<FireComponent>();
		if (nullptr != fireComponent)
		{
			return fireComponent->isBurning();
		}

		return false;
	}

	bool EntityHelper::isBurned() const
	{
		// Is object burned?
		const ComplexFireComponent* complexFireComponent = mEntity.getComponent<ComplexFireComponent>();
		if (nullptr != complexFireComponent)
		{
			return complexFireComponent->isBurned();
		}

		const FireComponent* fireComponent = mEntity.getComponent<FireComponent>();
		if (nullptr != fireComponent)
		{
			return fireComponent->isBurned();
		}

		// The entity is not burnable at all so return false
		return false;
	}

	bool EntityHelper::isBurnable() const
	{
		// Is object burnable?
		const ComplexFireComponent* complexFireComponent = mEntity.getComponent<ComplexFireComponent>();
		if (nullptr != complexFireComponent)
		{
			return complexFireComponent->isBurnable();
		}

		const FireComponent* fireComponent = mEntity.getComponent<FireComponent>();
		if (nullptr != fireComponent)
		{
			return fireComponent->isBurnable();
		}

		return false;
	}

	bool EntityHelper::canBeSetOnFireNow() const
	{
		const ComplexFireComponent* complexFireComponent = mEntity.getComponent<ComplexFireComponent>();
		if (nullptr != complexFireComponent)
		{
			return (complexFireComponent->isBurnable() && !complexFireComponent->isBurned() && !complexFireComponent->isBurning());
		}

		const FireComponent* fireComponent = mEntity.getComponent<FireComponent>();
		if (nullptr != fireComponent)
		{
			return (fireComponent->isBurnable() && !fireComponent->isBurned() && !fireComponent->isBurning());
		}

		return false;
	}

	bool EntityHelper::startFire(FreeplayEvent* freeplayEvent, bool fullFire) const
	{
		qsf::Entity* entity = &mEntity;

		// Fire component
		{
			FireComponent* fireComponent = entity->getComponent<FireComponent>();
			if (nullptr != fireComponent && fireComponent->isActive())
			{
				// Start fire
				fireComponent->startFire(freeplayEvent);
				if (fullFire)
				{
					fireComponent->setEnergyPercent(1.0f);
				}
				return true;
			}
		}

		// Complex fire component
		{
			ComplexFireComponent* complexFireComponent = entity->getComponent<ComplexFireComponent>();
			if (nullptr != complexFireComponent && complexFireComponent->isActive())
			{
				// Start fire
				if (fullFire)
				{
					complexFireComponent->startFireOnAllFireComponents(freeplayEvent);
					complexFireComponent->setEnergyPercent(1.0f);
				}
				else
				{
					complexFireComponent->startFire(freeplayEvent);
				}
				return true;
			}
		}

		// Other
		{
			HealthComponent* healthComponent = entity->getComponent<HealthComponent>();
			if (nullptr != healthComponent)
			{
				// Set fire injury (Spontaneous human combustion)
				return HealthHelper(*healthComponent).injurePersonByFire(freeplayEvent);
			}
		}

		return false;
	}

	bool EntityHelper::stopFire() const
	{
		FireComponent* fireComponent = mEntity.getComponent<FireComponent>();
		if (nullptr != fireComponent)
		{
			// Stop fire
			fireComponent->stopFire();
			return true;
		}

		ComplexFireComponent* complexFireComponent = mEntity.getComponent<ComplexFireComponent>();
		if (nullptr != complexFireComponent)
		{
			// Stop fire
			complexFireComponent->stopFire();
			return true;
		}

		return false;
	}

	void EntityHelper::setCanBurn(bool canBurn) const
	{
		// Only fire components can be changed, because currently only this case is needed
		// TODO(sw) add support for complex fire components when this should also be used for e.g. buildings
		FireComponent* fireComponent = mEntity.getComponent<FireComponent>();
		if (nullptr != fireComponent)
		{
			// Start fire
			fireComponent->setActive(canBurn);
		}
	}

	//[-------------------------------------------------------]
	//[ Container                                             ]
	//[-------------------------------------------------------]
	bool EntityHelper::isContainerEmpty() const
	{
		const qsf::game::PrototypeContainer* prototypeContainer = getPrototypeContainer();
		if (nullptr != prototypeContainer)
		{
			return prototypeContainer->getLinks().empty();
		}

		// It's no container at all
		return true;
	}

	qsf::Entity* EntityHelper::getContainerEntity() const
	{
		const qsf::MovableComponent* movableComponent = mEntity.getComponent<qsf::MovableComponent>();
		return (nullptr != movableComponent) ? movableComponent->getContainerEntity() : nullptr;
	}

	qsf::Entity& EntityHelper::getContainerEntitySafe() const
	{
		qsf::Entity* containerEntity = mEntity.getComponentSafe<qsf::MovableComponent>().getContainerEntity();
		QSF_CHECK(nullptr != containerEntity, "There is no container to return in the entity " << mEntity.getId(), QSF_REACT_THROW);
		return *containerEntity;
	}

	bool EntityHelper::isEntityInBuilding() const
	{
		const qsf::Entity* containerEntity = getContainerEntity();
		return (nullptr != containerEntity && nullptr != containerEntity->getComponent<BuildingComponent>());
	}

	bool EntityHelper::isEntityInVehicle() const
	{
		const qsf::Entity* containerEntity = getContainerEntity();
		return (nullptr != containerEntity && nullptr != containerEntity->getComponent<VehicleComponent>());
	}

	bool EntityHelper::isEntityInRoadVehicle() const
	{
		const qsf::Entity* containerEntity = getContainerEntity();
		return (nullptr != containerEntity && nullptr != containerEntity->getComponent<RoadVehicleComponent>());
	}

	qsf::game::PrototypeContainer* EntityHelper::getPrototypeContainer() const
	{
		VehicleComponent* vehicleComponent = mEntity.getComponent<VehicleComponent>();
		if (nullptr != vehicleComponent)
		{
			return vehicleComponent;
		}

		BuildingComponent* buildingComponent = mEntity.getComponent<BuildingComponent>();
		if (nullptr != buildingComponent)
		{
			return buildingComponent;
		}

		return nullptr;
	}

	bool EntityHelper::isEntityHardLinked() const
	{
		qsf::LinkComponent* linkComponent = mEntity.getComponent<qsf::LinkComponent>();
		if (nullptr != linkComponent)
		{
			if (qsf::isInitialized(linkComponent->getParentId()) && linkComponent->getParentLinkType() == qsf::LinkComponent::HARD_LINK)
			{
				// Has a parent link
				return true;
			}
		}
		return false;
	}

	void EntityHelper::unlinkFromParent() const
	{
		qsf::LinkComponent* linkComponent = mEntity.getComponent<qsf::LinkComponent>();
		if (nullptr == linkComponent)
			return;

		if (qsf::isUninitialized(linkComponent->getParentId()))
			return;

		linkComponent->unlinkFromParent();
		EntityHelper::teardownEntityAsGhost(mEntity);

		// Reactivate collision
		qsf::BulletBoxCollisionComponent* targetbulletBoxCollisionComponent = mEntity.getComponent<qsf::BulletBoxCollisionComponent>();
		if (nullptr != targetbulletBoxCollisionComponent)
		{
			// Enable collision
			targetbulletBoxCollisionComponent->setActive(true);
		}
	}

	void EntityHelper::enterContainer(qsf::Entity& containerEntity, uint32 containerCategory, bool hidden) const
	{
		qsf::game::PrototypeContainer* prototypeContainer = EntityHelper(containerEntity).getPrototypeContainer();
		if (nullptr == prototypeContainer)
			return;

		if (hidden)
		{
			// Make entity invisible
			qsf::game::HiddenComponent::setHidden(mEntity, true);
		}

		// Make sure there's no bone link (or bone-to-bone link) active any more
		//  -> That's important, e.g. for the tech helicopter picking up persons
		//  -> Also, this has to be done before adding the entity to the container, otherwise e.g. the ambulance helicopter won't work
		mEntity.destroyComponent<qsf::game::BoneLinkComponent>();

		// Deactivate speed limit components to prevent invisible entities to slow down units
		SpeedLimitComponent* speedLimitComponent = mEntity.getComponent<SpeedLimitComponent>();
		if (nullptr != speedLimitComponent)
		{
			speedLimitComponent->setActive(false);
		}

		// Enter the container now
		prototypeContainer->addToContainer(mEntity, containerCategory);

		// Establish link to parent (important to not delete the children)
		qsf::LinkComponent& linkComponent = mEntity.getOrCreateComponentSafe<qsf::LinkComponent>();
		linkComponent.linkToParent(containerEntity);
		if (hidden)
		{
			linkComponent.setLocalPosition(qsf::Math::GLM_VEC3_ZERO);
			linkComponent.setLocalRotation(qsf::Math::GLM_QUAT_IDENTITY);
		}

		// Disable collision
		setCollisionEnabled(false);

		// TODO(tl): Hack to ignore reservations from this entity, it would be better to remove them though
		// With an active navigation component we are still using AI reservations from this entity, so we need to disable it
		qsf::ai::NavigationComponent* navigationComponent = mEntity.getComponent<qsf::ai::NavigationComponent>();
		if (nullptr != navigationComponent)
		{
			navigationComponent->setActive(false);
		}

		// Remove from selection
		SelectionManager& selectionManager = EM5_GAME.getSelectionManager();
		if (selectionManager.isIdSelected(mEntity.getId()))
		{
			selectionManager.removeIdFromSelection(mEntity.getId());
			QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_CHANGE_SELECTION));
		}

		GroundMarkerComponent* groundMarkerComponent = mEntity.getComponent<GroundMarkerComponent>();
		if (nullptr != groundMarkerComponent)
		{
			groundMarkerComponent->setActive(false);
		}

		// Icon logic handling
		if (isGangsterPerson())
		{
			PersonComponent& personComponent = mEntity.getComponentSafe<PersonComponent>();
			if (!personComponent.getHandcuffedFlag() && !isPersonInjured())
			{
				if (EntityHelper(containerEntity).isVehicle())
				{
					StatusOverlayComponent::showIcon(containerEntity, StatusOverlayComponent::STATUS_ICON_GANGSTER_IN_VEHICLE, true);
				}
			}
		}

		// Event spreading may react on change and e.g. update objectives
		EventSpreading::enterContainerReaction(mEntity, containerEntity, containerCategory);
	}

	void EntityHelper::leaveContainer(bool unhide) const
	{
		qsf::LinkComponent* linkComponent = mEntity.getComponent<qsf::LinkComponent>();
		if (nullptr == linkComponent || !linkComponent->hasParent())
			return;

		qsf::Entity& containerEntity = *static_cast<qsf::Entity*>(linkComponent->getParentPrototype());
		qsf::game::PrototypeContainer* prototypeContainer = EntityHelper(containerEntity).getPrototypeContainer();
		if (nullptr == prototypeContainer)
			return;

		const uint32 containerCategory = prototypeContainer->getContainerType(mEntity);

		// Remove the person from the container
		prototypeContainer->removeFromContainer(mEntity);

		// Unlink person from parent
		linkComponent->unlinkFromParent();

		if (unhide)
		{
			// Make person visible
			qsf::game::HiddenComponent::setHidden(mEntity, false);

			qsf::GameLogicComponent* gameLogicComponent = mEntity.getComponent<qsf::GameLogicComponent>();
			if (nullptr != gameLogicComponent)
			{
				gameLogicComponent->setActive(true);
			}
		}

		// Enable collision
		setCollisionEnabled(true);

		// With an inactive navigation component we are ignoring AI reservations from this entity, so we need to enable it
		qsf::ai::NavigationComponent* navigationComponent = mEntity.getComponent<qsf::ai::NavigationComponent>();
		if (nullptr != navigationComponent)
		{
			navigationComponent->setActive(true);
		}

		// Show ground marker
		if (nullptr != mEntity.getComponent<CommandableComponent>())
		{
			SelectionMarkerManager::getInstanceSafe().showGroundMarker(mEntity, false);
		}

		// Activate speed limit components again
		SpeedLimitComponent* speedLimitComponent = mEntity.getComponent<SpeedLimitComponent>();
		if (nullptr != speedLimitComponent)
		{
			speedLimitComponent->setActive(true);
		}

		// For buildings, always set him to the main door position
		// TODO(fw): How about "findFreePosition"...?
		BuildingComponent* buildingComponent = containerEntity.getComponent<BuildingComponent>();
		if (nullptr != buildingComponent)
		{
			qsf::TransformComponent& transformComponent = mEntity.getOrCreateComponentSafe<qsf::TransformComponent>();
			transformComponent.setPosition(buildingComponent->getDoorPosition());
			transformComponent.setRotation(buildingComponent->getDoorRotation());
		}

		// Gangster Icon logic handling
		if (isGangsterPerson())
		{
			PersonComponent& personComponent = mEntity.getComponentSafe<PersonComponent>();
			if (!personComponent.getHandcuffedFlag())
			{
				// Show Icon for gangster
				StatusOverlayComponent::showIcon(mEntity, StatusOverlayComponent::STATUS_ICON_GANGSTER, true);

				// Turn off gangster flag for the vehicle, refresh the vehicle minimap icon
				EntityHelper containerEntityHelper(containerEntity);
				if (containerEntityHelper.isCivilRoadVehicle() && containerEntityHelper.isContainerEmpty())
				{
					StatusOverlayComponent::showIcon(containerEntity, StatusOverlayComponent::STATUS_ICON_GANGSTER_IN_VEHICLE, false);
					RoadVehicleComponent& roadVehicleComponent = containerEntity.getComponentSafe<RoadVehicleComponent>();
					if (roadVehicleComponent.isDamagedLevel1() || roadVehicleComponent.isDamagedLevel2())
					{
						StatusOverlayComponent::showIcon(containerEntity, StatusOverlayComponent::STATUS_ICON_DEFECT_VEHICLE, true);
					}
				}
			}
		}
		else
		{
			// If necessary, create a unit icon
			EM5_GAME.getMiniMapIconRegistrationLogic().registerUnitIcon(mEntity);
		}

		// Event spreading may react on change and e.g. update objectives
		EventSpreading::leaveContainerReaction(mEntity, containerEntity, containerCategory);
	}

	bool EntityHelper::isInContainer(qsf::Entity& containerEntity, uint32 containerCategory) const
	{
		qsf::game::PrototypeContainer* prototypeContainer = EntityHelper(containerEntity).getPrototypeContainer();
		if (nullptr != prototypeContainer)
		{
			for (auto iterator : prototypeContainer->getLinks())
			{
				const qsf::ContainerLink& containerLink = *iterator.second;
				if (containerLink.mContainerCategory == containerCategory)
				{
					if (containerLink.getTargetEntity().getId() == mEntity.getId())
					{
						return true;
					}
				}
			}
		}

		return false;
	}

	bool EntityHelper::isInContainer(qsf::Entity& containerEntity) const
	{
		qsf::game::PrototypeContainer* prototypeContainer = EntityHelper(containerEntity).getPrototypeContainer();
		if (nullptr != prototypeContainer)
		{
			for (auto iterator : prototypeContainer->getLinks())
			{
				const qsf::ContainerLink& containerLink = *iterator.second;
				if (containerLink.getTargetEntity().getId() == mEntity.getId())
				{
					return true;
				}
			}
		}

		return false;
	}

	void EntityHelper::setCollisionEnabled(bool collision) const
	{
		// Enable or disable collision
		qsf::BulletCollisionComponent* bulletCollisionComponent = mEntity.getComponent<qsf::BulletCollisionComponent>();
		if (nullptr != bulletCollisionComponent)
		{
			bulletCollisionComponent->setActive(collision);
		}
	}

	bool EntityHelper::isEntityBuried() const
	{
		qsf::LinkComponent* linkComponent = mEntity.getComponent<qsf::LinkComponent>();
		if (linkComponent == nullptr)
			return false;

		return (linkComponent->getComponentFromEntityOrLinkedParent<BuryComponent>() != nullptr);
	}

	//[-------------------------------------------------------]
	//[ Cleanup                                               ]
	//[-------------------------------------------------------]
	bool EntityHelper::isFireReadyToCleanup() const
	{
		// Is the fire component or complex fire component ready for cleanup
		ComplexFireComponent* complexFireComponent = mEntity.getComponent<ComplexFireComponent>();
		if (nullptr != complexFireComponent)
		{
			return complexFireComponent->isReadyForCleanup();
		}

		FireComponent* fireComponent = mEntity.getComponent<FireComponent>();
		if (nullptr != fireComponent)
		{
			return fireComponent->isReadyForCleanup();
		}

		// There is no fire component so it does not need a cleanup
		return true;
	}

	void EntityHelper::cleanupFire() const
	{
		// Cleanup the fire component or complex fire component
		ComplexFireComponent* complexFireComponent = mEntity.getComponent<ComplexFireComponent>();
		if (nullptr != complexFireComponent)
		{
			complexFireComponent->cleanup();
		}

		FireComponent* fireComponent = mEntity.getComponent<FireComponent>();
		if (nullptr != fireComponent)
		{
			fireComponent->cleanup();
		}
	}

	//[-------------------------------------------------------]
	//[ Event tag                                             ]
	//[-------------------------------------------------------]
	bool EntityHelper::checkEntityHasEventTag(const std::string& tag) const
	{
		const qsf::game::EventTagComponent* eventTagComponent = mEntity.getComponent<qsf::game::EventTagComponent>();
		return (nullptr != eventTagComponent) ? eventTagComponent->hasTag(tag) : false;
	}

	//[-------------------------------------------------------]
	//[ Event ID component                                    ]
	//[-------------------------------------------------------]
	bool EntityHelper::checkForInitializedEventIdComponent(FreeplayEvent* freeplayEvent) const
	{
		const EventIdComponent* eventIdComponent = mEntity.getComponent<EventIdComponent>();
		if (nullptr == eventIdComponent)
			return false;

		FreeplayEvent* event = eventIdComponent->getFreeplayEvent();

		// Check if the event ID component is initialized
		return !(nullptr == event || freeplayEvent == event);
	}

	bool EntityHelper::checkForEventIdComponent() const
	{
		const EventIdComponent* eventIdComponent = mEntity.getComponent<EventIdComponent>();
		if (nullptr == eventIdComponent)
			return false;

		return (nullptr != eventIdComponent->getFreeplayEvent());
	}

	FreeplayEvent* EntityHelper::getFreeplayEvent() const
	{
		// In some cases the event ID component is in the topmost parent entity
		qsf::LinkComponent* linkComponent = mEntity.getComponent<qsf::LinkComponent>();
		EventIdComponent* eventIdComponent = (nullptr != linkComponent ? linkComponent->getComponentFromEntityOrLinkedParent<EventIdComponent>() : mEntity.getComponent<EventIdComponent>());
		return (nullptr != eventIdComponent) ? eventIdComponent->getFreeplayEvent() : nullptr;
	}

	uint32 EntityHelper::getFreeplayEventId() const
	{
		const FreeplayEvent* freeplayEvent = getFreeplayEvent();
		return (nullptr != freeplayEvent) ? freeplayEvent->getId() : qsf::getUninitialized<uint32>();
	}

	//[-------------------------------------------------------]
	//[ Equipment / Inventory                                 ]
	//[-------------------------------------------------------]
	qsf::Entity* EntityHelper::getActiveEquipmentEntity() const
	{
		// Do not use "getOrCreateComponent" here, this can get called during action component shutdown
		const InventoryComponent* inventoryComponent = mEntity.getComponent<InventoryComponent>();
		if (inventoryComponent == nullptr || !inventoryComponent->hasActiveEquipment())
			return nullptr;

		return mEntity.getMap().getEntityById(inventoryComponent->getActiveEquipmentId());
	}

	qsf::game::EquipmentComponent* EntityHelper::getActiveEquipmentComponent() const
	{
		// Get the active equipment entity
		qsf::Entity* euipmentEntity = getActiveEquipmentEntity();
		if (nullptr == euipmentEntity)
			return nullptr;	// Can't find equipment component, return null pointer

		// Search for equipment component, can return null pointer
		return euipmentEntity->getComponent<qsf::game::EquipmentComponent>();
	}

	bool EntityHelper::checkIsActiveEquipment(const std::string& equipmentName) const
	{
		qsf::game::EquipmentComponent* activeEquipmentComponent = getActiveEquipmentComponent();
		if (nullptr == activeEquipmentComponent)
			return false; // Can't found Equipment, check failed

		// Check equipment name
		return activeEquipmentComponent->getEquipmentName() == equipmentName;
	}

	void EntityHelper::deleteActiveEquipment(qsf::Time timeToDelete) const
	{
		InventoryComponent* inventoryComponent = mEntity.getComponent<InventoryComponent>();
		if (nullptr == inventoryComponent)
			return;

		qsf::Entity* equipmentEntity = getActiveEquipmentEntity();
		if (nullptr != equipmentEntity)	// Can be nullptr
		{
			if (timeToDelete == qsf::Time::ZERO)
			{
				// Destroy the active entity
				MapHelper::destroyEntity(*equipmentEntity);
			}
			else
			{
				equipmentEntity->getOrCreateComponentSafe<FadeEffectComponent>().fadeOut(timeToDelete, true);
			}
		}

		inventoryComponent->setActiveEquipmentId(qsf::getUninitialized<uint64>());

		// This is only needed for multiplay
		inventoryComponent->setActiveEquipmentName("");
	}

	void EntityHelper::storeActiveEquipmentInInventory() const
	{
		InventoryComponent* inventoryComponent = mEntity.getComponent<InventoryComponent>();
		if (nullptr == inventoryComponent || !inventoryComponent->hasActiveEquipment())
			return;	// Nothing to do

		// Put the active equipment in the inventory
		uint64 activeEquipmentId = inventoryComponent->getActiveEquipmentId();
		inventoryComponent->InventoryArray.getVariable().push_back(activeEquipmentId);

		// Remove active equipment (we put it in inventory)
		inventoryComponent->setActiveEquipmentId(qsf::getUninitialized<uint64>());

		// Hide the equipment (the links are still exist)
		qsf::Entity* equipmentEntity = mEntity.getMap().getEntityById(activeEquipmentId);
		if (nullptr != equipmentEntity)
		{
			equipmentEntity->getOrCreateComponentSafe<FadeEffectComponent>().fadeOut();
		}
	}

	void EntityHelper::storeInventoryEquipmentAsActive() const
	{
		InventoryComponent* inventoryComponent = mEntity.getComponent<InventoryComponent>();
		if (nullptr == inventoryComponent || inventoryComponent->getEquipments().empty())
			return;

		// Take the first equipment from inventory
		uint64 equipmentId = inventoryComponent->InventoryArray.getVariable().front();
		inventoryComponent->InventoryArray.getVariable().erase(inventoryComponent->InventoryArray.getVariable().begin());	// Erase the first value from the inventory

		// Take active equipment
		inventoryComponent->setActiveEquipmentId(equipmentId);

		// Show the equipment
		qsf::Entity* equipmentEntity = mEntity.getMap().getEntityById(equipmentId);
		if (nullptr != equipmentEntity)
		{
			equipmentEntity->getOrCreateComponentSafe<FadeEffectComponent>().fadeIn();
		}
	}

	void EntityHelper::linkEquipmentToEntity(qsf::Entity& parent) const
	{
		{ // Set the position
			const qsf::TransformComponent& parentTransformComponent = parent.getOrCreateComponentSafe<qsf::TransformComponent>();
			mEntity.getOrCreateComponentSafe<qsf::TransformComponent>().setPositionAndRotation(parentTransformComponent.getPosition(), parentTransformComponent.getRotation());

			// Setup ghost
			EntityHelper::setupEntityAsGhost(mEntity, parent);
		}

		// Link equipment with parent
		mEntity.getOrCreateComponentSafe<qsf::LinkComponent>().linkToParent(parent);

		// Link the equipment to the person
		qsf::game::EquipmentComponent* equipmentComponent = mEntity.getComponent<qsf::game::EquipmentComponent>();
		qsf::game::EquipmentComponent::EquipmentType equipmentType = (equipmentComponent != nullptr ? equipmentComponent->getEquipmentType() : qsf::game::EquipmentComponent::EQUIPMENTTYPE_NONE);

		std::string boneLinkName;
		if (equipmentType == qsf::game::EquipmentComponent::EQUIPMENTTYPE_TWOHANDED)
		{
			// Two hands
			qsf::game::BoneToBoneLinkComponent* boneToBoneLinkComponent = mEntity.getOrCreateComponent<qsf::game::BoneToBoneLinkComponent>(false);
			if (nullptr != boneToBoneLinkComponent)
			{
				boneToBoneLinkComponent->setBoneName(equipment::STANDARD_BONE_RIGHT_HAND);
				boneToBoneLinkComponent->setChildBoneName1(equipment::EQUIPMENT_BONE_RIGHTHAND);
				boneToBoneLinkComponent->setParentBoneName1(equipment::STANDARD_BONE_RIGHT_HAND);

				boneToBoneLinkComponent->setChildBoneName2(equipment::EQUIPMENT_BONE_LEFTHAND);
				boneToBoneLinkComponent->setParentBoneName2(equipment::STANDARD_BONE_LEFT_HAND);

				boneToBoneLinkComponent->startup(mEntity.getMap().isRunning());
			}
		}
		else if (equipmentType == qsf::game::EquipmentComponent::EQUIPMENTTYPE_RIGHTHANDED_TO_EQUIP)
		{
			qsf::game::BoneToBoneLinkComponent* boneToBoneLinkComponent = mEntity.getOrCreateComponent<qsf::game::BoneToBoneLinkComponent>(false);
			if (nullptr != boneToBoneLinkComponent)
			{
				boneToBoneLinkComponent->setBoneName(equipment::STANDARD_BONE_RIGHT_HAND);
				boneToBoneLinkComponent->setChildBoneName1(equipment::EQUIPMENT_BONE_RIGHTHAND);
				boneToBoneLinkComponent->setParentBoneName1(equipment::STANDARD_BONE_RIGHT_HAND);

				boneToBoneLinkComponent->startup(mEntity.getMap().isRunning());
			}
		}
		else if (equipmentType == qsf::game::EquipmentComponent::EQUIPMENTTYPE_LEFTHANDED_TO_EQUIP)
		{
			qsf::game::BoneToBoneLinkComponent* boneToBoneLinkComponent = mEntity.getOrCreateComponent<qsf::game::BoneToBoneLinkComponent>(false);
			if (nullptr != boneToBoneLinkComponent)
			{
				boneToBoneLinkComponent->setBoneName(equipment::STANDARD_BONE_LEFT_HAND);
				boneToBoneLinkComponent->setChildBoneName1(equipment::EQUIPMENT_BONE_LEFTHAND);
				boneToBoneLinkComponent->setParentBoneName1(equipment::STANDARD_BONE_LEFT_HAND);

				boneToBoneLinkComponent->startup(mEntity.getMap().isRunning());
			}
		}
		else if (equipmentType == qsf::game::EquipmentComponent::EQUIPMENTTYPE_RIGHTHANDED)
		{
			boneLinkName = equipment::STANDARD_BONE_RIGHT_HAND;
		}
		else if (equipmentType == qsf::game::EquipmentComponent::EQUIPMENTTYPE_LEFTHANDED)
		{
			boneLinkName = equipment::STANDARD_BONE_LEFT_HAND;
		}
		else if (equipmentType == qsf::game::EquipmentComponent::EQUIPMENTTYPE_RIGHTARM)
		{
			boneLinkName = equipment::STANDARD_BONE_RIGHT_ARM;
		}
		else if (equipmentType == qsf::game::EquipmentComponent::EQUIPMENTTYPE_LEFTARM)
		{
			boneLinkName = equipment::STANDARD_BONE_LEFT_ARM;
		}
		else if (equipmentType == qsf::game::EquipmentComponent::EQUIPMENTTYPE_RIGHTUPPERARM)
		{
			boneLinkName = equipment::STANDARD_BONE_RIGHT_UPPERARM;
		}
		else if (equipmentType == qsf::game::EquipmentComponent::EQUIPMENTTYPE_LEFTUPPERARM)
		{
			boneLinkName = equipment::STANDARD_BONE_LEFT_UPPERARM;
		}
		else if (equipmentType == qsf::game::EquipmentComponent::EQUIPMENTTYPE_RIGHTLEG)
		{
			boneLinkName = equipment::STANDARD_BONE_RIGHT_LEG;
		}
		else if (equipmentType == qsf::game::EquipmentComponent::EQUIPMENTTYPE_LEFTLEG)
		{
			boneLinkName = equipment::STANDARD_BONE_LEFT_LEG;
		}
		else if (equipmentType == qsf::game::EquipmentComponent::EQUIPMENTTYPE_BODY)
		{
			boneLinkName = equipment::STANDARD_BONE_BODY;
		}
		else if (equipmentType == qsf::game::EquipmentComponent::EQUIPMENTTYPE_HEAD)
		{
			boneLinkName = equipment::STANDARD_BONE_HEAD;
		}
		else if (equipmentType == qsf::game::EquipmentComponent::EQUIPMENTTYPE_BACK)
		{
			boneLinkName = equipment::STANDARD_BONE_BACK;
		}

		if (!boneLinkName.empty())
		{
			qsf::game::BoneLinkComponent* boneLinkComponent = mEntity.getOrCreateComponent<qsf::game::BoneLinkComponent>();
			if (nullptr != boneLinkComponent)
			{
				boneLinkComponent->setBoneName(boneLinkName);
			}
		}
	}

	void EntityHelper::showEquipment(const std::string& equipmentPrefab, int forceTintColorIndex) const
	{
		if (checkIsActiveEquipment(equipmentPrefab))
		{
			// Equipment is already equipped
			return;
		}

		// Delete the current equipment
		deleteActiveEquipment();

		// Create and attach the equipment
		if (equipmentPrefab.empty())
			return;

		uint32 localAssetId;
		if (boost::algorithm::contains(equipmentPrefab, "/prefab/"))
		{
			localAssetId = qsf::StringHash(equipmentPrefab);
		}
		else
		{
			localAssetId = qsf::StringHash("em5/prefab/equipment/" + equipmentPrefab);
		}

		// We use the map helper here to create the entity from prefab -> We have in case of multiplay the same entity ID of the equipment prefab on multiplay host and client side
		qsf::Entity* equipmentEntity = MapHelper(mEntity.getMap()).createObjectByLocalPrefabAssetId(localAssetId);
		if (nullptr == equipmentEntity)
			return;

		// Adapt equipment layer to entity's layer
		moveEntityInOwnerLayer(*equipmentEntity);

		// Set random tint color
		qsf::compositing::TintableMeshComponent* tintableMeshComponent = equipmentEntity->getComponent<qsf::compositing::TintableMeshComponent>();
		if (nullptr != tintableMeshComponent)
		{
			const int tintColorIndex = ((forceTintColorIndex == -1) ? qsf::Random::getRandomInt(0, 15) : forceTintColorIndex);
			tintableMeshComponent->setTintPaletteIndex(tintColorIndex);
		}

		// Enable shine-through effect for the created equipment, but only for squad person
		EntityHelper entityHelper(*equipmentEntity);
		if (isSquadPerson())
		{
			// Enabling the shine through effect here is necessary to avoid crashes in the renderer with the "12345703_shadow_caster" material
			entityHelper.enableShineThroughEffect();
		}

		// Link equipment to entity
		InventoryComponent* inventoryComponent = mEntity.getOrCreateComponent<InventoryComponent>();
		if (nullptr != inventoryComponent)
		{
			inventoryComponent->setActiveEquipmentId(equipmentEntity->getId());
			entityHelper.linkEquipmentToEntity(mEntity);

			// This is only needed for multiplayer
			inventoryComponent->setActiveEquipmentName(equipmentPrefab);

			setupEntityAsGhost(*equipmentEntity, mEntity);
		}
	}

	void EntityHelper::deleteAllLinkedChild()
	{
		qsf::LinkComponent* linkComponent = mEntity.getComponent<qsf::LinkComponent>();
		if (nullptr != linkComponent)
		{
			while (!linkComponent->getChildLinks().empty())
			{
				qsf::LinkComponent* childLinkComponent = *linkComponent->getChildLinks().begin();
				if (nullptr == childLinkComponent)
					continue;

				childLinkComponent->unlinkFromParent();

				mEntity.getMap().destroyEntityById(childLinkComponent->getEntityId());
			}
		}

		// In case we had ambient animations via mesh animation test component we have to reset the person animation to avoid T-pose
		disableMeshAnimationTestComponent();
	}

	void EntityHelper::moveEntityInOwnerLayer(qsf::Entity& entityToMove) const
	{
		// We can always create the meta data component
		const uint32 layerId = mEntity.getOrCreateComponentSafe<qsf::MetadataComponent>().getLayerId();

		// Update not only the "entityToMove", but its children as well
		qsf::EntityVectorScratchBuffer linkedEntities;
		qsf::LinkComponent::getConnectedLinkGroup(entityToMove, qsf::LinkComponent::TRANSFORM, true, linkedEntities);
		for (qsf::Entity* entity : linkedEntities)
		{
			entity->getOrCreateComponentSafe<qsf::MetadataComponent>().setLayerId(layerId);
		}
	}


	//[-------------------------------------------------------]
	//[ Misc                                                  ]
	//[-------------------------------------------------------]
	uint32 EntityHelper::getUnitPlayerIndex() const
	{
		const CommandableComponent* commandableComponent = mEntity.getComponent<CommandableComponent>();
		return (nullptr != commandableComponent) ? commandableComponent->getPlayerId() : qsf::getUninitialized<uint32>();
	}

	bool EntityHelper::isUnitFromLocalPlayer() const
	{
		Player* localPlayer = PlayerManager::getLocalPlayer();
		return (nullptr != localPlayer) ? (getUnitPlayerIndex() == localPlayer->getPlayerIndex()) : false;
	}

	bool EntityHelper::isUnitOfDifferentPlayer() const
	{
		Player* localPlayer = PlayerManager::getLocalPlayer();
		return (nullptr != localPlayer) ? (getUnitPlayerIndex() != localPlayer->getPlayerIndex()) : false;
	}

	OrderInfo* EntityHelper::getOrderInfo() const
	{
		const CommandableComponent* commandableComponent = mEntity.getComponent<CommandableComponent>();
		if (nullptr == commandableComponent)
			return nullptr;	// Can't find commandable component, check failed

		return EM5_GAME.getOrderInfoManager().findElement(commandableComponent->getUnitType());
	}

	void EntityHelper::unregisterFromUnitPool() const
	{
		unregisterOrBlockFromUnitPool(true);
	}

	void EntityHelper::blockFromUnitPool() const
	{
		unregisterOrBlockFromUnitPool(false);
	}

	void EntityHelper::unregisterOrBlockFromUnitPool(bool unregister) const
	{
		bool emitMessage = false;

		// Is this a player unit?
		const CommandableComponent* commandableComponent = mEntity.getComponent<CommandableComponent>();
		if (nullptr != commandableComponent)
		{
			// Tell unit pool that the unit is destroyed
			const OrderInfo* orderInfo = EM5_GAME.getOrderInfoManager().findElement(commandableComponent->getUnitType());
			if (nullptr != orderInfo)
			{
				// Unregister the unit in the player's unit pool
				Player* player = EM5_PLAYERS.getPlayerByIndex(commandableComponent->getPlayerId());
				if (nullptr != player)
				{
					UnitPool& unitPool = player->getUnitPool();
					unitPool.unregisterUnitInMap(*orderInfo, mEntity.getId());
					if (!unregister)
					{
						unitPool.registerUnitBlocking(*orderInfo);
					}
					emitMessage = true;
				}
			}
		}

		// Same for the contained entities
		const qsf::game::PrototypeContainer* prototypeContainer = getPrototypeContainer();
		if (nullptr != prototypeContainer)
		{
			// Blocking damage units feature can enabled via specs
			const bool blockedDamagedUnits = RoadVehicleSpecsGroup::getInstanceSafe().getBlockDestroyedUnits();

			for (auto iterator : prototypeContainer->getLinks())
			{
				const qsf::ContainerLink& containerLink = *iterator.second;
				const EntityHelper containerHelper(containerLink.getTargetEntity());
				if (containerHelper.isCommandable())
				{
					if (blockedDamagedUnits &&
						((containerHelper.isSquadPerson() && containerHelper.isPersonInjured()) ||
						(containerHelper.isSquadVehicle() && containerHelper.isVehicleDestroyed())))
					{
						containerHelper.blockFromUnitPool();
					}
					else
					{
						containerHelper.unregisterOrBlockFromUnitPool(unregister);
					}
					emitMessage = true;
				}
			}
		}

		// Emit message to inform the GUI?
		if (emitMessage)
		{
			qsf::MessageParameters parameters;
			parameters.setParameter("EntityId", mEntity.getId());
			QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_UNIT_UNREGISTERED), parameters);
		}
	}

	qsf::Entity* EntityHelper::getParentEntity() const
	{
		qsf::LinkComponent* linkComponent = mEntity.getComponent<qsf::LinkComponent>();
		if (nullptr != linkComponent)
		{
			if (qsf::isInitialized(linkComponent->getParentId()) && linkComponent->getParentLinkType() == qsf::LinkComponent::HARD_LINK)
			{
				return static_cast<qsf::Entity*>(linkComponent->getParentPrototype());
			}
		}

		return nullptr;
	}

	void EntityHelper::enableShineThroughEffect(bool enable) const
	{
		// Do only allow the shine through effect on units from the local player
		if (GameSettingsGroup::getInstanceSafe().getShineThroughActive() && (!isCommandable() || isUnitFromLocalPlayer()))
		{
			qsf::EntityVectorScratchBuffer linkedEntities;
			qsf::LinkComponent::getConnectedLinkGroup(mEntity, qsf::LinkComponent::TRANSFORM, true, linkedEntities);
			for (qsf::Entity* linkedEntity : linkedEntities)
			{
				// No shine through effect for helicopter rotors nor the ground marker, this looks awful
				// -> Ground marker asset: Global asset ID 317590 - Local asset name "em5/prefab/gamelogic_entity/helicopter_ground_marker" - Local asset ID 2357156651
				// -> We identify the ground marker and similar entities via disabled shadow casting
				qsf::MeshComponent* meshComponent = linkedEntity->getComponent<qsf::MeshComponent>();
				if (nullptr != meshComponent && nullptr == linkedEntity->getComponent<HelicopterRotorComponent>() && meshComponent->getCastShadows())
				{
					// Render queue 77 = shine-through effect
					meshComponent->setRenderQueueId(enable ? 77 : qsf::getUninitialized<uint32>());
				}
			}
		}
	}

	void EntityHelper::setFlareActive(bool active) const
	{
		qsf::LinkComponent* linkComponent = mEntity.getComponent<qsf::LinkComponent>();
		if (nullptr == linkComponent)
			return;

		qsf::FlareComponent* flareComponent = linkComponent->getComponentFromEntityOrLinkedChild<qsf::FlareComponent>();
		if (nullptr == flareComponent)
			return;

		flareComponent->setActive(active);
	}

	void EntityHelper::setLightActive(bool active) const
	{
		qsf::LightAnimationComponent::setLightActive(mEntity, active);
	}

	GangsterBaseLogic& EntityHelper::turnIntoGangsterByType(const qsf::StringHash& gangsterTypeId) const
	{
		PersonComponent* personComponent = mEntity.getComponent<PersonComponent>();
		QSF_CHECK(nullptr != personComponent, "Entity does not have a person component and thus cannot be turned into a gangster!", QSF_REACT_THROW);

		GangsterBaseLogic* gangsterLogic = personComponent->changeGangsterType(gangsterTypeId);
		QSF_CHECK(nullptr != gangsterLogic, "Failed to turn entity into gangster of type ID " << gangsterTypeId, QSF_REACT_THROW);

		qsf::ai::NavigationComponent* navigationComponent = mEntity.getOrCreateComponent<qsf::ai::NavigationComponent>();
		navigationComponent->setActive(true);
		navigationComponent->setNativeSecondaryNavigationMapId(NavigationMap::FREE_GROUND_AREA);
		navigationComponent->setMoverType(MoverType::GANGSTER_PERSON);

		MoveToDespawnComponent* moveToDespawnComponent = mEntity.getComponent<MoveToDespawnComponent>();
		if (nullptr != moveToDespawnComponent)
		{
			// This entity just became a gangster, so the MoveToDespawnComponent must be deactivated.
			mEntity.destroyComponent<MoveToDespawnComponent>();

			// Create gangster appeared message
			QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_GANGSTER_APPEARED, mEntity.getId()));
		}

		return *gangsterLogic;
	}

	GangsterBaseLogic& EntityHelper::turnIntoGangster(const qsf::StringHash& gangsterLogicId, bool fleeInCar) const
	{
		PersonComponent* personComponent = mEntity.getComponent<PersonComponent>();
		QSF_CHECK(nullptr != personComponent, "Entity does not have a person component and thus cannot be turned into a gangster!", QSF_REACT_THROW);

		GangsterBaseLogic* gangsterLogic = personComponent->changeGangsterLogic(gangsterLogicId);
		QSF_CHECK(nullptr != gangsterLogic, "Failed to turn entity into gangster of game logic ID " << gangsterLogicId, QSF_REACT_THROW);

		qsf::ai::NavigationComponent* navigationComponent = mEntity.getOrCreateComponent<qsf::ai::NavigationComponent>();
		navigationComponent->setNativeSecondaryNavigationMapId(NavigationMap::FREE_GROUND_AREA);
		navigationComponent->setMoverType(fleeInCar ? MoverType::GANGSTER_VEHICLE : MoverType::GANGSTER_PERSON);

		MoveToDespawnComponent* moveToDespawnComponent = mEntity.getComponent<MoveToDespawnComponent>();
		if (nullptr != moveToDespawnComponent)
		{
			// This entity just became a gangster, so the MoveToDespawnComponent must be deactivated.
			mEntity.destroyComponent<MoveToDespawnComponent>();

			// Create gangster appeared message
			QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_GANGSTER_APPEARED, mEntity.getId()));
		}

		return *gangsterLogic;
	}

	qsf::Entity* EntityHelper::getClosestEntity(const std::vector<qsf::Entity*>& entities) const
	{
		if (entities.empty())
		{
			return nullptr;
		}

		qsf::TransformComponent* transformComponent = mEntity.getComponent<qsf::TransformComponent>();
		QSF_CHECK(nullptr != transformComponent, "Entity does not have a transform component, returning nullptr!", return nullptr);

		const glm::vec3& position = transformComponent->getPosition();

		// Simple search for the entity with the closest 2d range
		qsf::Entity* closestEntity = nullptr;
		float closestDistance = std::numeric_limits<float>::max();
		for (qsf::Entity* entity : entities)
		{
			const float distance = qsf::game::DistanceHelper::get2dDistance(position, *entity);
			if (distance < closestDistance && distance > 0.001f)
			{
				closestEntity = entity;
				closestDistance = distance;
			}
		}

		return closestEntity;
	}


	//[-------------------------------------------------------]
	//[ Fade                                                  ]
	//[-------------------------------------------------------]
	bool EntityHelper::isFading() const
	{
		const FadeEffectComponent* fadeEffectComponent = mEntity.getComponent<FadeEffectComponent>();
		return (nullptr != fadeEffectComponent) ? fadeEffectComponent->isFading() : false;
	}

	void EntityHelper::fadeIn(qsf::Time fadeTime) const
	{
		mEntity.setActive(true);
		mEntity.getOrCreateComponentSafe<FadeEffectComponent>().fadeIn(fadeTime);
	}

	void EntityHelper::fadeOut(qsf::Time fadeTime, bool deleteOwner) const
	{
		mEntity.getOrCreateComponentSafe<FadeEffectComponent>().fadeOut(fadeTime, deleteOwner);
	}

	void EntityHelper::setVisibleAndFade(bool visible, qsf::Time fadeTime) const
	{
		if (visible)
		{
			fadeIn(fadeTime);
		}
		else
		{
			fadeOut(fadeTime);
		}
	}


	//[-------------------------------------------------------]
	//[ Misc                                                  ]
	//[-------------------------------------------------------]
	void EntityHelper::getComponentsFromChildrenById(uint32 componentId, std::vector<qsf::Component*>& output) const
	{
		output.clear();

		const qsf::LinkComponent* linkComponent = mEntity.getComponent<qsf::LinkComponent>();
		if (nullptr == linkComponent)
			return;

		for (qsf::LinkComponent* linkComponent : linkComponent->getChildLinks())
		{
			qsf::Entity& entity = linkComponent->getEntity();
			qsf::Component* component = entity.getComponentById(componentId);
			if (nullptr == component)
				continue;

			output.push_back(component);
		}
	}

	bool EntityHelper::hasEnabledMeshAnimationTestComponent() const
	{
		const qsf::MeshAnimationTestComponent* meshAnimationTestComponent = mEntity.getComponent<qsf::MeshAnimationTestComponent>();
		return (nullptr != meshAnimationTestComponent && meshAnimationTestComponent->isTestComponentUpdatesEnabled());
	}

	void EntityHelper::disableMeshAnimationTestComponent()
	{
		qsf::MeshAnimationTestComponent* meshAnimationTestComponent = mEntity.getComponent<qsf::MeshAnimationTestComponent>();
		if (nullptr != meshAnimationTestComponent)
		{
			// Ambient persons can have animations like, sitting on a chair, doing some strange gesture, etc
			meshAnimationTestComponent->setTestComponentUpdatesEnabled(false);
			mEntity.destroyComponent<GaperComponent>();
			mEntity.destroyComponent<MoveToDespawnComponent>();

			// In case we had ambient animations via mesh animation test component we have to reset the person animation to avoid T-pose
			PersonComponent* personComponent = mEntity.getComponent<PersonComponent>();
			if (nullptr != personComponent)
			{
				personComponent->clearAnimation();
				personComponent->forceIdleAnimationUpdate();
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
