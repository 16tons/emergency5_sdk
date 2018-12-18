// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/map/MapHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/CollisionHelper.h"
#include "em5/action/ambulance/TreatPersonAction.h"
#include "em5/action/base/DestroyParticleAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/base/GameplayAssets.h"
#include "em5/component/base/LightTimeControlComponent.h"
#include "em5/component/effect/FadeEffectComponent.h"
#include "em5/component/movement/MoveToDespawnComponent.h"
#include "em5/component/objects/ContaminationCloudComponent.h"
#include "em5/component/objects/UpgradeComponent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/component/spawnpoint/UnitSpawnPointComponent.h"
#include "em5/component/spawnpoint/civilist/PersonSpawnPointComponent.h"
#include "em5/component/vehicle/VehicleComponent.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/fire/component/FireComponent.h"
#include "em5/fire/component/ComplexFireComponent.h"
#include "em5/game/Game.h"
#include "em5/game/GameSimulation.h"
#include "em5/game/component/GameComponent.h"
#include "em5/game/cutscene/CutscenePlayer.h"
#include "em5/game/player/Player.h"
#include "em5/game/player/PlayerManager.h"
#include "em5/game/selection/SelectionManager.h"
#include "em5/game/spawn/SpawnPoolManager.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/network/multiplayer/server/Host.h"
#include "em5/network/NetworkManager.h"
#include "em5/settings/GameSettingsGroup.h"
#include "em5/EM5Helper.h"

#include <qsf_game/environment/time/TimeOfDayComponent.h>
#include <qsf_game/component/base/BoneToBoneLinkComponent.h>
#include <qsf_game/component/event/EventTagComponent.h>
#include <qsf_game/component/event/EventTagManagerComponent.h>

#include <qsf_ai/base/Line2D.h>
#include <qsf_ai/base/Math.h>
#include <qsf_ai/navigation/NavigationComponent.h>
#include <qsf_ai/worldModel/WorldModelManager.h>
#include <qsf_ai/worldModel/trafficControl/TrafficLightTimerComponent.h>
#include <qsf_ai/worldModel/trafficLanes/TrafficLaneWorld.h>

#include <qsf_compositing/component/TintableMeshComponent.h>

#include <qsf/base/ScratchBuffer.h>
#include <qsf/component/base/MetadataComponent.h>
#include <qsf/component/spatial/SpatialPartitionManagerComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/move/MovableComponent.h>
#include <qsf/component/utility/SelfDestructComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/Entity.h>
#include <qsf/map/component/MapPropertiesComponent.h>
#include <qsf/map/layer/Layer.h>
#include <qsf/map/layer/LayerManager.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/math/Random.h>
#include <qsf/math/Math.h>
#include <qsf/math/Segment.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/renderer/utility/CameraControlComponent.h>
#include <qsf/renderer/mesh/MeshComponent.h>
#include <qsf/renderer/particles/ParticlesComponent.h>
#include <qsf/renderer/utility/CameraFlightComponent.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
namespace
{
	namespace detail
	{


		//[-------------------------------------------------------]
		//[ Global definitions                                    ]
		//[-------------------------------------------------------]
		// As the game uses continuous entity IDs for dynamically created (potentially non-synced) entities already,
		// we have to start with something much higher than 1 - let's use 1 million instead.
		static const uint64 INITIAL_START_ENTITY_ID = 1000000;
		static uint64 startEntityId = INITIAL_START_ENTITY_ID;


		//[-------------------------------------------------------]
		//[ Global functions                                      ]
		//[-------------------------------------------------------]
		void createUniqueIdMapForMultiplayer(const std::vector<const qsf::Prototype*>& prototypeList, qsf::Map::UniqueIdMap& uniqueIdMap)
		{
			// For multiplayer we create continuous IDs for newly created entities after map load
			// With this, we avoid sending the id map to the client, because he can calculate it the same way
			// We need a sorted version of the ids
			boost::container::flat_set<uint64> prototypeIdSet;
			prototypeIdSet.reserve(prototypeList.size());
			for (const qsf::Prototype* prototype : prototypeList)
			{
				prototypeIdSet.emplace(prototype->getId());
			}

			for (const uint64 prototypeId : prototypeIdSet)
			{
				uniqueIdMap.emplace(prototypeId, startEntityId);
				++startEntityId;
			}
		}

		void getEntitiesWithEventTag(std::vector<qsf::Entity*>& output, const qsf::StringHash& eventTag)
		{
			// Gather all entities with the given event tag
			output.clear();

			const std::vector<qsf::game::EventTagComponent*>& eventTagComponents = qsf::game::EventTagManagerComponent::getEventTagComponentsByTag(eventTag, QSF_MAINMAP);
			if (!eventTagComponents.empty())
			{
				for (const qsf::game::EventTagComponent* eventTagComponent : eventTagComponents)
				{
					output.emplace_back(&eventTagComponent->getEntity());
				}
			}
		}

		void gatherAreaInfluencedComponents(const std::vector<qsf::Entity*>& areaBoxes, const qsf::ComponentCollection::ComponentList<qsf::Component>& candidateComponents, std::vector<qsf::Component*>& influencedComponents)
		{
			// TODO(co) This isn't the most optimized implementation, but should be sufficient for our use-case
			// Find all area entities
			for (const qsf::Entity* areaEntity : areaBoxes)
			{
				if (nullptr == areaEntity)
					continue;

				// Get current area
				const qsf::TransformComponent* areaTransformComponent = areaEntity->getComponent<qsf::TransformComponent>();
				QSF_CHECK(nullptr != areaTransformComponent, "em5::detail::gatherAreaInfluencedComponents(): Entity " << areaEntity->getId() << " has no transform component", continue);
				const glm::vec3& areaPosition = areaTransformComponent->getPosition();
				const glm::vec3& areaScale = areaTransformComponent->getScale();
				const float areaRadius = areaScale[qsf::Math::getMajorAxisOfVector(areaScale)];
				const qsf::ai::EOrientedBox areaOrientedBox(areaScale * 0.5f, areaPosition, glm::mat3_cast(areaTransformComponent->getRotation()));

				// Find all candidate entities inside the current area
				for (qsf::Component* candidateComponent : candidateComponents)
				{
					const qsf::Entity& candidateEntity = candidateComponent->getEntity();
					const qsf::TransformComponent* candidateTransformComponent = candidateEntity.getComponent<qsf::TransformComponent>();
					if (nullptr == candidateTransformComponent)
						continue;

					// Renderer component is optional, for we have e.g. entities inside lanterns that only hold a light time controller and a light child
					const qsf::RendererComponent* candidateRendererComponent = candidateEntity.getComponent<qsf::RendererComponent>();

					const glm::vec3& candidatePosition = candidateTransformComponent->getPosition();
					const float candidateRadius = (nullptr != candidateRendererComponent) ? candidateRendererComponent->getWorldRadius() : 1.0f;

					// Perform a fast distance check first
					const float squaredDistance = glm::distance2(areaPosition, candidatePosition);
					const float combinedRadius = areaRadius + candidateRadius;
					if (squaredDistance < combinedRadius * combinedRadius)
					{
						glm::vec3 candidateMinimumPoint, candidateMaximumPoint;
						if (nullptr == candidateRendererComponent || !candidateRendererComponent->getLocalAxisAlignedBoundingBox(candidateMinimumPoint, candidateMaximumPoint))
						{
							// Fallback
							candidateMinimumPoint = candidatePosition - glm::vec3(candidateRadius, candidateRadius, candidateRadius);
							candidateMaximumPoint = candidatePosition + glm::vec3(candidateRadius, candidateRadius, candidateRadius);
						}

						// Perform less efficient but more precise oriented bounding box intersection test
						// The candidate's box is actually an axis aligned box
						const qsf::ai::EOrientedBox candidateOrientedBox((candidateMaximumPoint - candidateMinimumPoint) * 0.5f, candidatePosition, qsf::Math::GLM_MAT3X3_IDENTITY);
						if (areaOrientedBox.Intersects(candidateOrientedBox))
						{
							// -> No problem if candidate is influenced by multiple areas
							influencedComponents.push_back(candidateComponent);
						}
					}
				}
			}
		}

		bool isCameraFlightAllowed(qsf::Entity& cameraEntity, const glm::vec3& targetPosition)
		{
			// Do only perform a 2D check
			const glm::vec3& sourcePosition = cameraEntity.getOrCreateComponentSafe<qsf::TransformComponent>().getPosition();
			return (glm::distance2(glm::vec3(sourcePosition.x, 0.0f, sourcePosition.z), glm::vec3(targetPosition.x, 0.0f, targetPosition.z)) > 1.0f);
		}


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
	} // detail
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Private static data                                   ]
	//[-------------------------------------------------------]
	std::pair<qsf::TransformComponent*, bool> MapHelper::mGangsterEventArea;


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	bool MapHelper::destroyEntity(qsf::Entity& entity)
	{
		Game* game = Game::getInstance();
		if (nullptr != game && game->isRunning())
		{
			// Deselect unit
			SelectionManager& selectionManager = EM5_GAME.getSelectionManager();
			if (selectionManager.isIdSelected(entity.getId()))
			{
				// Only trigger a message here, if the entity was selected...
				selectionManager.removeIdFromSelection(entity.getId());
				QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_CHANGE_SELECTION));
			}
		}

		// Unregister entity from unit pool
		EntityHelper(entity).unregisterFromUnitPool();

		// If a carried person is destroyed (cleanup by event) tell it the person how carry it
		qsf::LinkComponent* linkComponent = entity.getComponent<qsf::LinkComponent>();
		if (nullptr != linkComponent && nullptr != linkComponent->getParentPrototype())
		{
			qsf::Entity* parentEntity = static_cast<qsf::Entity*>(linkComponent->getParentPrototype());
			if (nullptr != parentEntity)
			{
				EntityHelper(*parentEntity).clearCarryTarget(EntityHelper::ANY);
			}
		}

		// Remove em5 specific entity links
		qsf::game::BoneToBoneLinkComponent* boneToBoneLinkComponent = entity.getComponent<qsf::game::BoneToBoneLinkComponent>();
		if (nullptr != boneToBoneLinkComponent)
		{
			entity.getMap().destroyObjectById(boneToBoneLinkComponent->getEntityId());
		}

		// For doctor: Delete placed medikit
		// TODO(mk) Move this function to doctor logic (in case the doctor is injured or the doctor logic is deleted)
		qsf::ActionComponent* actionComponent = entity.getComponent<qsf::ActionComponent>();
		if (nullptr != actionComponent)
		{
			qsf::Action* treatPersonAction = actionComponent->getCurrentAction();
			if (nullptr != treatPersonAction && treatPersonAction->getTypeId() == TreatPersonAction::ACTION_ID)
			{
				// Destroy the medikit via ID, can be nullptr
				entity.getMap().destroyEntityById(static_cast<TreatPersonAction*>(treatPersonAction)->getMedikitId());
			}

			// In general: Clear the action plan
			//  -> This is also done during destruction, but right here we have a much more controlled situation
			actionComponent->clearPlan();
		}

		return entity.getMap().destroyObjectById(entity.getId());
	}

	bool MapHelper::destroyEntityById(uint64 entityId)
	{
		return getMap().destroyObjectById(entityId);
	}

	void MapHelper::flyCameraToPosition(const glm::vec3& position, const Player& player, bool keepCurrentZoom)
	{
		// Get the main camera entity and check whether or not we're already pretty close to the target position
		// -> When not doing the distance check, ugly minor camera jumping might come up when e.g. trying to jump to the position multiple times in a row
		qsf::Entity* mainCamera = player.getMainCameraEntity();
		if (nullptr != mainCamera)
		{
			qsf::TransformComponent& transformComponent = mainCamera->getOrCreateComponentSafe<qsf::TransformComponent>();
			const qsf::CameraControlComponent& cameraControlComponent = mainCamera->getOrCreateComponentSafe<qsf::CameraControlComponent>();
			glm::vec3 positionOffset = transformComponent.getRotation() * glm::vec3(0.0f, 0.0f, cameraControlComponent.getTargetDistance());
			if (::detail::isCameraFlightAllowed(*mainCamera, position + positionOffset))
			{
				if (keepCurrentZoom)
				{
					// This is what the camera input handler adds to ground map level height
					glm::vec3 targetPosition = position;
					targetPosition.y += 0.5f;

					transformComponent.setPosition(targetPosition + positionOffset);
				}
				else
				{
					qsf::CameraFlightComponent* cameraFlightComponent = mainCamera->getOrCreateComponent<qsf::CameraFlightComponent>();
					if (nullptr != cameraFlightComponent)
					{
						// Move the camera to the entity
						cameraFlightComponent->flyToPosition(position, true);
					}
				}

				{ // Making unpredictable map jumps is a brutal act and will introduce visual artifacts, try to suppress at least a few of them
					// -> It's important that the following is done last in a fixed defined order, so do not use message proxies
					// Enforce player view position update at once
					for (const auto& player : EM5_PLAYERS.getAllPlayers())
					{
						player->updateViewTransform();
					}

					// No glitching vehicles in the first few frames at the new map location
					EM5_GAME.getSimulation().updateAiLod();
				}
			}
		}
	}

	void MapHelper::flyCameraToEntity(const qsf::Entity& focusEntity, const Player& player, bool keepCurrentZoom)
	{
		// Get the main camera entity and check whether or not we're already pretty close to the target position
		// -> When not doing the distance check, ugly minor camera jumping might come up when e.g. trying to jump to the position multiple times in a row
		// -> Pretty similar to "em5::MapHelper::flyCameraToPosition()" but at the end other methods are called
		qsf::Entity* mainCamera = player.getMainCameraEntity();
		if (nullptr != mainCamera)
		{
			const qsf::CameraControlComponent& cameraControlComponent = mainCamera->getOrCreateComponentSafe<qsf::CameraControlComponent>();
			qsf::TransformComponent& transformComponent = mainCamera->getOrCreateComponentSafe<qsf::TransformComponent>();
			glm::vec3 positionOffset = transformComponent.getRotation() * glm::vec3(0.0f, 0.0f, cameraControlComponent.getTargetDistance());
			glm::vec3 targetPosition = EntityHelper(const_cast<qsf::Entity&>(focusEntity)).getPosition();
			if (::detail::isCameraFlightAllowed(*mainCamera, targetPosition + positionOffset))
			{
				if (keepCurrentZoom)
				{
					// This is what the camera input handler adds to ground map level height
					targetPosition.y += 0.5f;

					transformComponent.setPosition(targetPosition + positionOffset);
				}
				else
				{
					qsf::CameraFlightComponent* cameraFlightComponent = mainCamera->getOrCreateComponent<qsf::CameraFlightComponent>();
					if (nullptr != cameraFlightComponent)
					{
						// Move the camera to the entity
						cameraFlightComponent->flyToEntity(focusEntity, true);
					}
				}

				{ // Making unpredictable map jumps is a brutal act and will introduce visual artifacts, try to suppress at least a few of them
					// -> It's important that the following is done last in a fixed defined order, so do not use message proxies
					// Enforce player view position update at once
					for (const auto& player : EM5_PLAYERS.getAllPlayers())
					{
						player->updateViewTransform();
					}

					// No glitching vehicles in the first few frames at the new map location
					EM5_GAME.getSimulation().updateAiLod();
				}
			}
		}
	}

	bool MapHelper::isUnitAutomatismActive()
	{
		if (EM5_GAME.getCutscenePlayer().isPlaying())
			return false;

		// No automatism in multiplay
		if (EM5_NETWORK.isMultiplayActive())
			return false;

		// Is automatism active via settings? Everything else than "AUTOMATISM_NONE" must be checked in the specific automatism
		if (GameSettingsGroup::getInstanceSafe().getAutomatismValue() == GameSettingsGroup::AUTOMATISM_NONE)
			return false;

		return QSF_MAINMAP.getCoreEntity().getOrCreateComponent<GameComponent>()->getUnitAutomatismActive();
	}

	bool MapHelper::isDamageSimulationActive()
	{
		return QSF_MAINMAP.getCoreEntity().getOrCreateComponent<GameComponent>()->getDamageSimulationActive();
	}

	void MapHelper::resetStoredGangsterEventAreaInformation()
	{
		MapHelper::mGangsterEventArea.first = nullptr;
	}

	void MapHelper::resetNextEntityIdForMultiplay()
	{
		detail::startEntityId = detail::INITIAL_START_ENTITY_ID;
	}

	void MapHelper::activateLayer(qsf::Layer& layer, bool activate)
	{
		if (activate)
		{
			layer.activateLayer();
		}
		else
		{
			layer.deactivateLayer(layer.getInternalBufferMap().getEntities().empty());
		}

		Game* game = Game::getInstance();
		if (nullptr != game && game->isRunning())
		{
			multiplayer::server::Host* multiplayerHost = EM5_NETWORK.getMultiplayerHost();
			if (nullptr != multiplayerHost)
			{
				multiplayerHost->onLayerChanged(layer);
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	qsf::Entity* MapHelper::createObjectByLocalPrefabAssetId(const qsf::StringHash& localAssetId, bool sendMessage) const
	{
		qsf::Map::CreateEntityIdMapCallback callback = nullptr;

		if (EM5_NETWORK.isMultiplayActive())
		{
			callback = &detail::createUniqueIdMapForMultiplayer;
		}

		qsf::Entity* entity = mMap.createObjectByLocalPrefabAssetId(localAssetId, callback);
		if (nullptr == entity)
		{
			// Error
			return nullptr;
		}

		if (sendMessage)
		{
			// Only needed for multiplay
			qsf::MessageParameters messageParameters;
			messageParameters.setParameter("CreatedEntity", entity);
			messageParameters.setParameter("PrefabHash", localAssetId.getHash());
			QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ENTITY_CREATED_FROM_PREFAB, entity->getId()), messageParameters);
		}

		return entity;
	}

	qsf::Entity* MapHelper::createEntityByPrototypeReference(const qsf::Prototype& prototype, bool sendMessage) const
	{
		//qsf::Map::CreateEntityIdMapCallback callback = nullptr;

		//if (EM5_NETWORK.isMultiplayActive())
		//{
		//	callback = &detail::createUniqueIdMapForMultiplayer;
		//}

		qsf::Entity* entity = mMap.createEntityByPrototypeReference(prototype);
		if (nullptr == entity)
		{
			// Error
			return nullptr;
		}

		// TODO(db): Make usable for multiplayer
		//if (sendMessage)
		//{
		//	// Only needed for multiplay
		//	qsf::MessageParameters messageParameters;
		//	messageParameters.setParameter("CreatedEntity", entity);
		//	messageParameters.setParameter("PrefabHash", qsf::StringHash(prototype.getId()).getHash());
		//	QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ENTITY_CREATED_FROM_PREFAB, entity->getId()), messageParameters);
		//}

		return entity;
	}

	void MapHelper::showEntityParticleIds(const std::vector<uint64>& particleEntityIds, bool show, bool repeatAnimation, bool showByHeight, float worldYOffset, bool restart) const
	{
		// Go through all particles
		for (uint64 entityId : particleEntityIds)
		{
			// Get the entity
			const qsf::Entity* entity = getMap().getEntityById(entityId);
			if (nullptr != entity)
			{
				if (showByHeight)
				{
					const qsf::TransformComponent* transformComponent = entity->getComponent<qsf::TransformComponent>();
					if (nullptr != transformComponent)
					{
						show = (transformComponent->getPosition().y < worldYOffset);
					}
				}

				// Get the particle component
				qsf::ParticlesComponent* particlesComponent = entity->getComponent<qsf::ParticlesComponent>();
				if (nullptr != particlesComponent && !particlesComponent->isHidden())
				{
					if (show)
					{
						// Don't restart on activation
						if (!particlesComponent->isActive())
						{
							particlesComponent->setActive(true);
						}

						// Restart on activation if the particle effect is already finished
						else if (restart == true && particlesComponent->isFinished())
						{
							// Make sure the particle effect is active
							particlesComponent->setActive(false);
							particlesComponent->setActive(true);
						}

						particlesComponent->setRepeatAnimation(repeatAnimation);
						particlesComponent->setEmitterEnabled(true);
					}
					else if (repeatAnimation)	// Only repeated animation are turned off
					{
						// Only disable the emitter to avoid a hard cut (it will stop itself automatically when all particles have faded out)
						particlesComponent->setEmitterEnabled(false);
					}
				}
			}
		}
	}

	void MapHelper::disableEntityParticleIds(const std::vector<uint64>& particleEntityIds) const
	{
		// Go through all particles
		for (uint64 entityId : particleEntityIds)
		{
			qsf::Entity* entity = getMap().getEntityById(entityId);
			if (nullptr != entity)
			{
				qsf::ParticlesComponent* particlesComponent = entity->getComponent<qsf::ParticlesComponent>();
				if (nullptr != particlesComponent)
				{
					particlesComponent->setActive(false);
				}
			}
		}
	}

	void MapHelper::hideParticleChildrenAtMapSave(std::vector<uint64>& particleIds) const
	{
		// Go through all particles
		for (uint64 entityId : particleIds)
		{
			// Get the entity
			const qsf::Entity* entity = getMap().getEntityById(entityId);
			if (nullptr != entity)
			{
				// Get the particle component
				qsf::ParticlesComponent* particlesComponent = entity->getComponent<qsf::ParticlesComponent>();
				if (nullptr != particlesComponent)
				{
					// Force restart of particle
					particlesComponent->setEmitterEnabled(false);
					particlesComponent->setPropertyOverrideState(qsf::ParticlesComponent::EMITTER_ENABLED, qsf::Object::PROPERTY_OVERRIDE);

					particlesComponent->setActive(false);
					particlesComponent->setPropertyOverrideState(qsf::ParticlesComponent::ACTIVE, qsf::Object::PROPERTY_OVERRIDE);
				}
			}
		}
	}

	qsf::ParticlesComponent* MapHelper::spawnParticle(qsf::LocalAssetId prefabAssetId, float timeout, const glm::vec3& position, const glm::vec3& scale, const glm::quat& rotation, bool isMultiplayerRelevant)
	{
		qsf::Entity* particleEntity = getMap().createObjectByLocalPrefabAssetId(prefabAssetId);
		if (nullptr != particleEntity)
		{
			if (isMultiplayerRelevant)
			{
				multiplayer::server::Host* multiplayerHost = EM5_NETWORK.getMultiplayerHost();
				if (nullptr != multiplayerHost)
				{
					multiplayerHost->onSpawnParticle(prefabAssetId, timeout, position, scale);
				}
			}

			{
				qsf::TransformComponent& particleTransformComponent = particleEntity->getOrCreateComponentSafe<qsf::TransformComponent>();
				particleTransformComponent.setPositionAndRotation(position, rotation);
				particleTransformComponent.setScale(scale);
			}

			qsf::ParticlesComponent * particleComponent = particleEntity->getComponent<qsf::ParticlesComponent>();
			if (nullptr != particleComponent)
			{
				particleComponent->setRepeatAnimation(false);
				particleComponent->setFixedTimeout(timeout);
				particleEntity->getOrCreateComponentSafe<qsf::SelfDestructComponent>().setTimeout(qsf::Time::fromSeconds(timeout*2.0f));
			}
			return particleComponent;
		}
		else
		{
			QSF_ERROR("Could not create prefab of type: " << prefabAssetId, QSF_REACT_NONE);
			return nullptr;
		}
	}

	void MapHelper::showEntityIds(std::vector<uint64>& entityIds, bool show, bool showByHeight, float worldYOffset) const
	{
		// TODO(mk) I think we also must deactivate the collision component

		for (uint64 entityId : entityIds)
		{
			// Get the entity
			const qsf::Entity* entity = getMap().getEntityById(entityId);
			if (nullptr != entity)
			{
				if (showByHeight)
				{
					const qsf::TransformComponent* transformComponent = entity->getComponent<qsf::TransformComponent>();
					if (nullptr != transformComponent)
					{
						show = (transformComponent->getPosition().y < worldYOffset);
					}
				}

				// Update the component
				qsf::MeshComponent* meshComponent = entity->getComponent<qsf::MeshComponent>();
				if (nullptr != meshComponent)
				{
					meshComponent->setActive(show);
				}

				// Update particles too
				qsf::ParticlesComponent* particlesComponent = entity->getComponent<qsf::ParticlesComponent>();
				if (nullptr != particlesComponent)
				{
					particlesComponent->setActive(show);
				}
			}
		}
	}

	bool MapHelper::isNightTime() const
	{
		qsf::game::TimeOfDayComponent* timeOfDayComponent = getMap().getCoreEntity().getComponent<qsf::game::TimeOfDayComponent>();
		if (nullptr != timeOfDayComponent)
		{
			return (timeOfDayComponent->getState() == qsf::game::TimeOfDayComponent::State::NIGHTTIME);
		}
		return false;
	}

	qsf::Entity* MapHelper::getHeadQuarter() const
	{
		// TODO(mk) Find the HQ via the RTW entry
		UnitSpawnPointComponent* spawnPoint = UnitSpawnPointComponent::findUnitSpawnPoint("RTW", UnitSpawnPointComponent::UNITSPAWNPOINTTYPE_ENTRY, mMap, glm::vec3());
		return (nullptr != spawnPoint) ? &spawnPoint->getEntity() : nullptr;
	}

	bool MapHelper::isContaminationArea(const glm::vec3& position) const
	{
		// we have no maximal distance here to check, so instead we have to find all contamination clouds and check the distance to each.
		const auto& contaminationCloudComponents = qsf::ComponentMapQuery(getMap()).getAllInstances<ContaminationCloudComponent>();
		if (!contaminationCloudComponents.empty())
		{
			for (ContaminationCloudComponent* cloud : contaminationCloudComponents)
			{
				if (cloud->isActive() &&
					cloud->getSimulationState() == ContaminationCloudComponent::STATE_ACTIVE &&
					cloud->isInsideContaminationRadius(position))
				{
					return true;
				}
			}
		}
		return false;
	}

	std::pair<qsf::TransformComponent*, bool> MapHelper::getGangsterEventArea() const
	{
		if (nullptr == MapHelper::mGangsterEventArea.first)
		{
			// No gangster event area known (yet)
			const std::vector<qsf::game::EventTagComponent*>& tagComponents = qsf::game::EventTagManagerComponent::getEventTagComponentsByTag(qsf::StringHash("GANGSTER_EVENT_AREA"), mMap);
			if (!tagComponents.empty())
			{
				for (qsf::game::EventTagComponent* eventTagComponent : tagComponents)
				{
					qsf::Entity& boxEntity = eventTagComponent->getEntity();

					// Box entity describes a gangster event area
					qsf::TransformComponent* transformComponent = boxEntity.getComponent<qsf::TransformComponent>();
					QSF_CHECK(nullptr != transformComponent, "DebugBox has GANGSTER_EVENT_AREA tag, but does not have a transform component!", QSF_REACT_NONE);
					if (nullptr != transformComponent)
					{
						MapHelper::mGangsterEventArea = std::make_pair(transformComponent, false);
						break;
					}
				}
			}
		}

		QSF_CHECK(nullptr != MapHelper::mGangsterEventArea.first, "This map does not have a GANGSTER_EVENT_AREA!", QSF_REACT_NONE);

		return MapHelper::mGangsterEventArea;
	}

	qsf::Entity* MapHelper::createObjectWithinGangsterEventAreaByLocalPrefabAssetId(const qsf::StringHash& localAssetId) const
	{
		// We need to query the spawn points in every call as they might have changed!
		std::vector<PersonSpawnPointComponent*> spawnPointsInGangsterEventArea;

		{
			const std::pair<qsf::TransformComponent*, bool> gangsterEventArea = getGangsterEventArea();

			// TODO(ks) Maybe more efficient way by using SpatialComponentPartition approach
			const auto& spawnPoints = qsf::ComponentMapQuery(getMap()).getAllInstances<PersonSpawnPointComponent>();
			if (!spawnPoints.empty())
			{
				for (PersonSpawnPointComponent* spawnPoint : spawnPoints)
				{
					// Use only spawnpoints that are active, so the gangster can not spawn accidentally on special event spawn points for injures or missing persons
					if (!spawnPoint->isActive())
						continue;

					// Not in gangster area?
					if (nullptr != gangsterEventArea.first && !EntityHelper(spawnPoint->getEntity()).isInsideBox(*gangsterEventArea.first, gangsterEventArea.second))
						continue;

					spawnPointsInGangsterEventArea.push_back(spawnPoint);
				}
			}
		}

		QSF_CHECK(!spawnPointsInGangsterEventArea.empty(), "No spawn points within gangster event area of this map!", return nullptr);

		// Get a random spawn point from our list of spawn points
		PersonSpawnPointComponent* randomSpawnPoint = qsf::Random::getAnyOf(spawnPointsInGangsterEventArea);

		// Create the actual entity
		qsf::Entity* spawnedEntity = createObjectByLocalPrefabAssetId(localAssetId);
		if (nullptr != spawnedEntity)
		{
			qsf::TransformComponent& spawnPointTransformComponent = randomSpawnPoint->getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();
			spawnPointTransformComponent.setScale(qsf::Math::GLM_VEC3_UNIT_XYZ);

			spawnedEntity->getOrCreateComponentSafe<qsf::TransformComponent>().setPositionAndRotation(spawnPointTransformComponent.getPosition(), spawnPointTransformComponent.getRotation());

			// Move the entity to the position of the spawnpoint
			qsf::MovableComponent* movableComponent = spawnedEntity->getComponent<qsf::MovableComponent>();
			if (nullptr != movableComponent)
			{
				movableComponent->warpToPositionAndRotation(spawnPointTransformComponent.getPosition(), spawnPointTransformComponent.getRotation());
			}

			// Fade the entity in
			spawnedEntity->getOrCreateComponentSafe<FadeEffectComponent>().fadeIn(qsf::Time::fromSeconds(1.0f));
		}

		// Can be nullptr
		return spawnedEntity;
	}

	qsf::Entity* MapHelper::createGangsterFromPoolAndSpawnItInGangsterEventArea(GangsterPool gangsterPoolType) const
	{
		// Get random prefab name from given pool
		const std::string& gangsterPoolName = getSpawnpoolNameFromGangsterPool(gangsterPoolType);
		QSF_CHECK(!gangsterPoolName.empty(), "MapHelper:createGangsterFromPoolAndSpawnItInGangsterEventArea() Empty gangster pool", return nullptr);

		SpawnPoolManager::SpawnInstanceDataPointer spawnInstanceData = nullptr;
		const std::string& entityPrefab = EM5_GAME.getSimulation().getSpawnPoolManager().getRandomNameFromSpawnPool(gangsterPoolName, &spawnInstanceData);

		// Create the actual gangster entity in the gangster event area
		return createObjectWithinGangsterEventAreaByLocalPrefabAssetId(qsf::StringHash(entityPrefab));
	}

	std::string MapHelper::getSpawnpoolNameFromGangsterPool(GangsterPool gangsterPoolType) const
	{
		// TODO(mk) The following lines are somewhat identical with some in EntityHelper::isGangsterCandidate()

		// Here we map the gangster pool type enum to fixed assets
		std::string gangsterPoolName = assets::SPANWPOOL_GANGSTER;

		// What pool to look at?
		switch (gangsterPoolType)
		{
			case GangsterPool::DEFAULT:
				// Default value already set above
				break;
			case GangsterPool::DEFAULT_EXTENDED:
				gangsterPoolName = assets::SPANWPOOL_GANGSTER_EXTENDED;
				break;
			case GangsterPool::TERRORIST:
				gangsterPoolName = assets::SPANWPOOL_GANGSTER_TERRORIST;
				break;
			case GangsterPool::CONVICT:
				gangsterPoolName = assets::SPANWPOOL_GANGSTER_CONVICT;
				break;
		}

		return gangsterPoolName;
	}

	qsf::Entity* MapHelper::getOrCreateGangsterCandidateInGangsterEventArea(GangsterPool gangsterPoolType, const std::string& gangsterTag) const
	{
		// If there is a gangster tag given, this forces us to return exactly this gangster - without further checks
		if (!gangsterTag.empty())
		{
			const auto& list = qsf::game::EventTagManagerComponent::getEventTagComponentsByTag(qsf::StringHash(gangsterTag), mMap);
			if (!list.empty())
			{
				return &qsf::Random::getAnyOf(list)->getEntity();
			}
		}
		else
		{
			// Collect all persons in the map's gangster area
			qsf::StdVectorScratchBuffer<qsf::Component*> allPersonsWithinGangsterArea;
			{
				std::pair<qsf::TransformComponent*, bool> gangsterEventArea = getGangsterEventArea();
				bool success = false;

				if (nullptr != gangsterEventArea.first)
				{
					// Getting persons in the gangster area
					qsf::SpatialPartitionManagerComponent& spatialPartitionManagerComponent = getMap().getCoreEntity().getOrCreateComponentSafe<qsf::SpatialPartitionManagerComponent>();

					const glm::vec3& position = gangsterEventArea.first->getPosition();
					const glm::vec3& halfScale = 0.5f * gangsterEventArea.first->getScale();

					const glm::vec2 boundsMin = glm::vec2(position.x, position.z) - glm::vec2(halfScale.x, halfScale.z);
					const glm::vec2 boundsMax = glm::vec2(position.x, position.z) + glm::vec2(halfScale.x, halfScale.z);

					success = spatialPartitionManagerComponent.findComponentsInBounds(PersonComponent::COMPONENT_ID, boundsMin, boundsMax, allPersonsWithinGangsterArea);
					QSF_CHECK(success, "No partition found!", QSF_REACT_NONE);
				}

				if (!success)
				{
					// If there is no gangster area, use the whole map as fallback
					// TODO(fw): Copying could certainly avoided here
					const auto& allPersons = qsf::ComponentMapQuery(mMap).getAllInstances<PersonComponent>();
					for (PersonComponent* component : allPersons)
					{
						allPersonsWithinGangsterArea.push_back(component);
					}
				}
			}

			qsf::EntityVectorScratchBuffer gangsterCandidates;
			for (qsf::Component* comp : allPersonsWithinGangsterArea)
			{
				qsf::Entity& currentCandidate = comp->getEntity();

				// Do not check for candidate being in gangster area as this is always the case here
				if (EntityHelper(currentCandidate).isGangsterCandidate(gangsterPoolType, false))
				{
					gangsterCandidates.push_back(&currentCandidate);
				}
			}

			if (!gangsterCandidates.empty())
			{
				// Found a gangster candidate
				return qsf::Random::getAnyOf(*gangsterCandidates);
			}

			// No gangster candidate found, create a new one
			return createGangsterFromPoolAndSpawnItInGangsterEventArea(gangsterPoolType);
		}

		return nullptr;
	}

	qsf::Entity* MapHelper::replaceCombinedEntityWithRealOne(qsf::Entity& combinedEntity) const
	{
		qsf::MetadataComponent* metadataComponent = combinedEntity.getComponent<qsf::MetadataComponent>();
		if (nullptr != metadataComponent)
		{
			// Check the prefab name if it contains the name "_combined"
			const qsf::AssetProxy& assetProxy = metadataComponent->getBasePrefab();
			const std::string& localAssetName = assetProxy.getLocalAssetName();

			size_t findLocation = localAssetName.find("_combined");
			if (findLocation != std::string::npos)
			{
				// Yep the given entity seems to be a combined entity no replace it with the real one
				// Get the local asset name without "_combined" -> This is then the name of the real one (as defined)
				std::string prefabName = localAssetName.substr(0, findLocation);

				// Now try to create the real one
				qsf::Entity* realEntity = createObjectByLocalPrefabAssetId(qsf::StringHash(prefabName));

				// Check if we could create it
				if (nullptr != realEntity)
				{
					// We have the new entity -> positioned on the same spot as the old one
					qsf::TransformComponent* transformComponentCombinedEntity = combinedEntity.getComponent<qsf::TransformComponent>();
					qsf::TransformComponent* transformComponentRealEntity = realEntity->getComponent<qsf::TransformComponent>();

					if (nullptr != transformComponentCombinedEntity && nullptr != transformComponentRealEntity)
					{
						transformComponentRealEntity->setTransform(transformComponentCombinedEntity->getTransform());
					}

					// Setup tint color if possible
					qsf::compositing::TintableMeshComponent* tintableMeshComponentCombined = combinedEntity.getComponent<qsf::compositing::TintableMeshComponent>();
					if (nullptr != tintableMeshComponentCombined)
					{
						const int tintPaletteIndex = tintableMeshComponentCombined->getTintPaletteIndex();

						qsf::compositing::TintableMeshComponent* tintableMeshComponentRealEntity = realEntity->getComponent<qsf::compositing::TintableMeshComponent>();
						if (nullptr != tintableMeshComponentRealEntity)
						{
							tintableMeshComponentRealEntity->setTintPaletteIndex(tintPaletteIndex);
						}
					}

					// Disable the AI/Move to despawn
					qsf::ai::NavigationComponent* navigationComponent = realEntity->getComponent<qsf::ai::NavigationComponent>();
					if (nullptr != navigationComponent)
					{
						navigationComponent->setActive(false);
					}

					MoveToDespawnComponent* moveToDespawnComponent = realEntity->getComponent<MoveToDespawnComponent>();
					if (nullptr != moveToDespawnComponent)
					{
						moveToDespawnComponent->setActive(false);
					}

					// Destroy the combined one it is no longer needed
					MapHelper::destroyEntity(combinedEntity);

					return realEntity;
				}
			}
		}

		return &combinedEntity;
	}

	bool MapHelper::isInMapBoundaries(const glm::vec3& position) const
	{
		qsf::MapPropertiesComponent* mapPropertiesComponent = mMap.getCoreEntity().getComponent<qsf::MapPropertiesComponent>();
		if (nullptr != mapPropertiesComponent)
		{
			// Get map boundaries from map properties component
			const glm::vec3& topLeft = mapPropertiesComponent->getMapBoundaryTopLeft();
			const glm::vec3& bottomRight = mapPropertiesComponent->getMapBoundaryBottomRight();

			if (topLeft.x > position.x || bottomRight.x < position.x)
				return false;

			if (topLeft.y > position.y || bottomRight.y < position.y)
				return false;

			if (topLeft.z > position.z || bottomRight.z < position.z)
				return false;

			return true;
		}

		// Error
		return false;
	}

	bool MapHelper::isInEventBoundaries(const glm::vec3& position) const
	{
		qsf::MapPropertiesComponent* mapPropertiesComponent = mMap.getCoreEntity().getComponent<qsf::MapPropertiesComponent>();
		if (nullptr != mapPropertiesComponent)
		{
			// Get event boundaries from map properties component
			const glm::vec3& topLeft = mapPropertiesComponent->getEventBoundaryTopLeft();
			const glm::vec3& bottomRight = mapPropertiesComponent->getEventBoundaryBottomRight();

			if (topLeft.x > position.x || bottomRight.x < position.x)
				return false;

			if (topLeft.y > position.y || bottomRight.y < position.y)
				return false;

			if (topLeft.z > position.z || bottomRight.z < position.z)
				return false;

			return true;
		}

		// Error
		return false;
	}

	bool MapHelper::isEntityCloseToMapBoundaries(qsf::Entity& entity, float tolerance)
	{
		qsf::MapPropertiesComponent* mapPropertiesComponent = mMap.getCoreEntity().getComponent<qsf::MapPropertiesComponent>();

		if (nullptr != mapPropertiesComponent)
		{
			// Position of entity
			qsf::TransformComponent& transformComponent = entity.getComponentSafe<qsf::TransformComponent>();
			const glm::vec3& entityPosition = transformComponent.getPosition();

			const glm::vec3& topLeft = mapPropertiesComponent->getMapBoundaryTopLeft();
			const glm::vec3& bottomRight = mapPropertiesComponent->getMapBoundaryBottomRight();

			if (entityPosition.x < (topLeft.x + tolerance) || entityPosition.x > (bottomRight.x - tolerance)
				|| entityPosition.z < (topLeft.z + tolerance) || entityPosition.z > (bottomRight.z - tolerance))
			{
				return true;
			}
		}

		return false;
	}


	bool MapHelper::isEntityCloseToEventBoundaries(qsf::Entity& entity, float tolerance)
	{
		qsf::MapPropertiesComponent* mapPropertiesComponent = mMap.getCoreEntity().getComponent<qsf::MapPropertiesComponent>();

		if (nullptr != mapPropertiesComponent)
		{
			// Position of entity
			qsf::TransformComponent& transformComponent = entity.getComponentSafe<qsf::TransformComponent>();
			const glm::vec3& entityPosition = transformComponent.getPosition();

			const glm::vec3& topLeft = mapPropertiesComponent->getEventBoundaryTopLeft();
			const glm::vec3& bottomRight = mapPropertiesComponent->getEventBoundaryBottomRight();

			if (entityPosition.x < (topLeft.x + tolerance) || entityPosition.x > (bottomRight.x - tolerance)
				|| entityPosition.z < (topLeft.z + tolerance) || entityPosition.z > (bottomRight.z - tolerance))
			{
				return true;
			}
		}

		return false;
	}

	void MapHelper::prepareMapBorderLayer(const std::string& layerName)
	{
		QSF_CHECK(!layerName.empty(), "Could not deactivate layer because name is not set", return);

		qsf::Layer* layer = mMap.getLayerManager().getLayerByName(qsf::StringHash(layerName));
		if (nullptr != layer)
		{
			// Check all entities in the layer
			for (uint64 entityId : layer->getEntityIds())
			{
				qsf::Entity* entity = mMap.getEntityById(entityId);
				if (nullptr != entity)
				{
					// Delete fire entities and components
					FireComponent* fireComponent = entity->getComponent<FireComponent>();
					if (nullptr != fireComponent)
					{
						for (uint64 entityId : fireComponent->FireParticlesArray.getVariable())
						{
							mMap.destroyEntityById(entityId);
						}
						entity->destroyComponent<FireComponent>();
					}

					// Same with complex fire
					ComplexFireComponent* complexFireComponent = entity->getComponent<ComplexFireComponent>();
					if (nullptr != complexFireComponent)
					{
						std::vector<FireComponent*> fireComponents;
						complexFireComponent->getAllFireComponents(fireComponents);
						for (FireComponent* fireComponent : fireComponents)
						{
							for (uint64 entityId : fireComponent->FireParticlesArray.getVariable())
							{
								mMap.destroyEntityById(entityId);
							}
						}
						entity->destroyComponent<ComplexFireComponent>();
					}

					// For being sure, also destroy the damage component
					entity->destroyComponent<DamageComponent>();
				}
			}
		}
	}

	void MapHelper::unlockUpgradeComponent(const std::string& upgradeName)
	{
		// Search the map for upgrade components
		for (UpgradeComponent* upgradeComponent : qsf::ComponentMapQuery(getMap()).getAllInstances<UpgradeComponent>())
		{
			// Show all upgrades with the upgrade ID
			if (upgradeComponent->getUpgradeId() == upgradeName)
			{
				upgradeComponent->showUpgrade();
			}
		}
	}

	void MapHelper::setEnergySectionEnabled(const qsf::StringHash& eventTag, bool enabled) const
	{
		qsf::EntityVectorScratchBuffer areaBoxes;
		detail::getEntitiesWithEventTag(areaBoxes, eventTag);

		setEnergySectionEnabled(areaBoxes, enabled);
	}

	void MapHelper::setEnergySectionEnabled(const std::vector<qsf::Entity*>& areaBoxes, bool enabled) const
	{
		// Get a list of all relevant energy consuming components
		const auto& lightControlComponents = qsf::ComponentMapQuery(mMap).getAllInstancesById(LightTimeControlComponent::COMPONENT_ID);
		const auto& trafficLightControlComponents = qsf::ComponentMapQuery(mMap).getAllInstancesById(qsf::ai::TrafficLightTimerComponent::COMPONENT_ID);

		// Gather area influenced components
		std::vector<qsf::Component*> influencedComponents;
		detail::gatherAreaInfluencedComponents(areaBoxes, lightControlComponents, influencedComponents);
		detail::gatherAreaInfluencedComponents(areaBoxes, trafficLightControlComponents, influencedComponents);

		// Set energy state
		for (qsf::Component* influencedComponent : influencedComponents)
		{
			influencedComponent->setActive(enabled);
		}
	}

	void MapHelper::evacuateAllUnits(const qsf::StringHash& fromEventTag, const qsf::StringHash& toEventTag, const std::vector<uint64>& ignoreEntityIds) const
	{
		const std::vector<qsf::game::EventTagComponent*>& toEventTagComponents = qsf::game::EventTagManagerComponent::getEventTagComponentsByTag(toEventTag, mMap);
		if (!toEventTagComponents.empty())
		{
			// Get a list of all relevant components
			const auto& commandableComponents = qsf::ComponentMapQuery(mMap).getAllInstancesById(CommandableComponent::COMPONENT_ID);

			// Gather area influenced components
			qsf::EntityVectorScratchBuffer areaBoxes;
			detail::getEntitiesWithEventTag(areaBoxes, fromEventTag);

			// Get a list of all units inside a from-area
			std::vector<qsf::Component*> influencedComponents;
			detail::gatherAreaInfluencedComponents(areaBoxes, commandableComponents, influencedComponents);

			// Move all units inside a from-area into one of the to-areas
			const CollisionHelper collisionHelper(mMap);
			for (qsf::Component* influencedComponent : influencedComponents)
			{
				qsf::Entity& influencedEntity = influencedComponent->getEntity();

				// Ignore entity? The linear search is fine because there are usually just a few entries.
				if (std::find(ignoreEntityIds.cbegin(), ignoreEntityIds.cend(), influencedEntity.getId()) == ignoreEntityIds.cend())
				{
					// Choose one of the to-areas by random
					// -> We could cache and reuse the following data, but on the other hand it's not really worth the effort for this use-case
					const qsf::game::EventTagComponent* eventTagComponent = qsf::Random::getAnyOf(toEventTagComponents);
					const qsf::Entity& areaEntity = eventTagComponent->getEntity();
					const qsf::TransformComponent* areaTransformComponent = areaEntity.getComponent<qsf::TransformComponent>();
					QSF_CHECK(nullptr != areaTransformComponent, "em5::detail::gatherAreaInfluencedComponents(): Entity " + std::to_string(areaEntity.getId()) + " has no transform component", continue);
					const glm::vec3& areaPosition = areaTransformComponent->getPosition();
					const glm::vec3& areaScale = areaTransformComponent->getScale();
					const float areaRadius = areaScale[qsf::Math::getMajorAxisOfVector(areaScale)];

					// Find a collision-free position which is closest to the requested position
					glm::vec3 freePosition;
					if (!collisionHelper.findFreePositionEM3(influencedEntity, qsf::Transform(areaTransformComponent->getTransform().getPosition()), areaRadius * 0.2f, 1.0f, freePosition))
					{
						collisionHelper.findFreePositionEM3(influencedEntity, qsf::Transform(areaTransformComponent->getTransform().getPosition()), areaRadius * 0.6f, 1.0f, freePosition);
					}

					// Move unit to new position
					qsf::EntityHelper(influencedEntity).setPosition(freePosition);
				}
			}
		}
	}

	void MapHelper::removeAllCiviliansInArea(const qsf::StringHash & areaEventTag)
	{
		removeCiviliansInArea(areaEventTag, false, false);
	}

	void MapHelper::removeAllCivilPersonsInArea(const qsf::StringHash & areaEventTag)
	{
		removeCiviliansInArea(areaEventTag, false, true);
	}

	void MapHelper::removeAllCivilVehiclesInArea(const qsf::StringHash & areaEventTag)
	{
		removeCiviliansInArea(areaEventTag, true, false);
	}

	void MapHelper::removeCiviliansInArea(const qsf::StringHash & areaEventTag, bool ignorePersons, bool ignoreVehicles)
	{
		const auto& moveToDespawnComponents = qsf::ComponentMapQuery(mMap).getAllInstancesById(MoveToDespawnComponent::COMPONENT_ID);

		// Gather area influenced components
		qsf::EntityVectorScratchBuffer areaBoxes;
		detail::getEntitiesWithEventTag(areaBoxes, areaEventTag);

		// Extract all civilians in area
		std::vector<qsf::Component*> influencedComponents;
		detail::gatherAreaInfluencedComponents(areaBoxes, moveToDespawnComponents, influencedComponents);

		for (qsf::Component* component : influencedComponents)
		{
			// Check if move to despawn component is active
			//  -> This is a strong indicator that this is a normal civilian not currently occupied with some event or injury
			if (component->isActive())
			{
				qsf::Entity& entity = component->getEntity();

				// Filter persons
				if (ignorePersons && nullptr != entity.getComponent<PersonComponent>())
				{
					continue;
				}

				// Filter vehicles
				if (ignoreVehicles && nullptr != entity.getComponent<VehicleComponent>())
				{
					continue;
				}

				// Filter entities that are part of an event
				if (EntityHelper(entity).checkForInitializedEventIdComponent())
				{
					continue;
				}

				MapHelper::destroyEntity(entity);
			}
		}
	}

	bool MapHelper::getPointOnClosestLane(const glm::vec3& position, uint32 aiWorldIndex, uint32 laneTypeId, qsf::Transform& outTransform, boost::optional<glm::vec3> wantedDirection)
	{
		const std::unique_ptr<qsf::ai::ManagedNavigationMapReadAccess> readAccess = qsf::ai::WorldModelManager::getInstance().acquireReadAccess(aiWorldIndex);
		if (readAccess.get() == nullptr)
			return false;

		const qsf::ai::WorldModel& requestedNavMap = readAccess.get()->get();
		const qsf::ai::TrafficLaneWorld* trafficLaneWorld = dynamic_cast<const qsf::ai::TrafficLaneWorld*>(&requestedNavMap);
		if (nullptr == trafficLaneWorld)
			return false;

		float closestDistance2 = std::numeric_limits<float>::max();

		// Cycle through all lanes
		const qsf::ai::LaneCollection& laneCollection = trafficLaneWorld->getLanes();
		for (uint32 laneIndex = 0; laneIndex < laneCollection.getNumLanes(); ++laneIndex)
		{
			const qsf::ai::Lane& lane = laneCollection.getLane(laneIndex);
			if (lane.getTypeId() == laneTypeId && lane.getNumNodes() >= 2)
			{
				// Cycle through all edges inside the lane
				for (uint32 i = 0; i+1 < lane.getNumNodes(); ++i)
				{
					const qsf::Segment edge = qsf::Segment::fromTwoPoints(lane.getNodes()[i].mPosition, lane.getNodes()[i+1].mPosition);
					const glm::vec3 closestPointOnLine = qsf::Math::getClosestPointOnLineSegment(position, edge);
					const float distance2 = glm::distance2(position, closestPointOnLine);

					if (distance2 < closestDistance2)
					{
						if (!wantedDirection.is_initialized() || glm::dot(edge.getDirection(), wantedDirection.get()) > 0.5f)
						{
							outTransform.setPosition(closestPointOnLine);
							outTransform.setRotation(qsf::Math::getLookAtQuaternion(edge.getDirection(), qsf::CoordinateSystem::getUp()));
							closestDistance2 = distance2;
						}
					}
				}
			}
		}

		return (closestDistance2 < std::numeric_limits<float>::max());
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
