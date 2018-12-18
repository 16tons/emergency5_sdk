// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/spawnpoint/SpawnPointComponent.h"
#include "em5/activity/ActivityComponent.h"
#include "em5/component/event/BlockedForEventComponent.h"
#include "em5/component/effect/FadeEffectComponent.h"
#include "em5/component/vehicle/VehicleComponent.h"
#include "em5/game/spawn/SpawnPoolManager.h"
#include "em5/game/GameSimulation.h"
#include "em5/game/Game.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/EM5Helper.h"

#include <qsf_ai/navigation/em4Router/em2012/RouteFinder/RouteFinder.h>
#include <qsf_ai/navigation/em4Router/wrapper/EM3Singletons.h>

#include <qsf_compositing/component/TintableMeshComponent.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/move/MovableComponent.h>
#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>
#include <qsf/math/Math.h>
#include <qsf/math/Random.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/physics/collision/CollisionHelper.h>
#include <qsf/physics/collision/BulletCollisionComponent.h>
#include <qsf/prototype/PrototypeManager.h>
#include <qsf/serialization/binary/StlTypeSerialization.h>
#include <qsf/QsfHelper.h>

#ifndef ENDUSER
	#include <qsf/debug/DebugDrawProxy.h>
	#include <qsf/debug/request/CircleDebugDrawRequest.h>
#endif


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 SpawnPointComponent::COMPONENT_ID = qsf::StringHash("em5::SpawnPointComponent");


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	qsf::Entity* SpawnPointComponent::spawnRandomAt(const qsf::Transform& transform, qsf::Map& map, const std::string& spawnPoolName, bool checkCollisions)
	{
		// Create a random entity. Use the game simulation, because it has knowledge about the spawn pool details
		SpawnPoolManager::SpawnInstanceDataPointer spawnInstanceData = nullptr;
		const std::string& randomName = EM5_GAME.getSimulation().getSpawnPoolManager().getRandomNameFromSpawnPool(spawnPoolName, &spawnInstanceData);
		if (!randomName.empty())
		{
			QSF_ASSERT(nullptr != spawnInstanceData, "If a random name was chosen from spawn pool, spawnInstanceData must be valid", return nullptr);

			const uint32 localAssetId = qsf::StringHash(randomName);

			// Check for collision in the area
			bool isColliding = false;
			if (checkCollisions)
			{
				// Get collision from prefab, ignore child objects
				const qsf::Prototype* prototype = QSF_MAINPROTOTYPE.getPrefabByLocalAssetId(localAssetId);
				QSF_CHECK(nullptr != prototype, "Can't spawn entity from of unknown prefab '" << randomName << "'", return nullptr);

				qsf::BulletCollisionComponent* collisionComponent = prototype->getComponent<qsf::BulletCollisionComponent>();
				if (nullptr != collisionComponent)
				{
					isColliding = !qsf::CollisionHelper(map).canBeAddedWithoutCollision(transform, *collisionComponent);

					// Let's ask the old routefinder too
					{
						glm::vec3 anchorPoint, extensions;
						glm::quat rotation;
						collisionComponent->getAsOrientedBoundingBox(anchorPoint, rotation, extensions);

						qsf::ai::CollisionList collisionRange;
						qsf::ai::EM3::Router->FindCollisions(extensions, -1, qsf::ai::ERouterObjectState(transform.getPosition(), glm::mat3_cast(transform.getRotation())), collisionRange, qsf::ai::EOTC_ANYTHING); // call with values for right now

						if (!collisionRange.empty())
						{
							isColliding = true;
						}
					}
				}
			}

			if (!isColliding)
			{
				// Create and place the new entity (person/vehicle)
				qsf::Entity* newEntity = MapHelper(map).createObjectByLocalPrefabAssetId(localAssetId);
				if (nullptr != newEntity)
				{
					newEntity->getOrCreateComponentSafe<qsf::TransformComponent>().setPositionAndRotation(transform.getPosition(), transform.getRotation());

					qsf::MovableComponent* movableComponent = newEntity->getComponent<qsf::MovableComponent>();
					if (movableComponent != nullptr)
					{
						movableComponent->warpToPositionAndRotation(transform.getPosition(), transform.getRotation());
					}

					// Mark possible vehicle as not parking, relevant for cleanup
					VehicleComponent* vehicleComponent = newEntity->getComponent<VehicleComponent>();
					if (nullptr != vehicleComponent)
					{
						vehicleComponent->setIsParkingEntity(false);
					}

					// Set random tint color
					qsf::compositing::TintableMeshComponent* tintableMeshComponent = newEntity->getComponent<qsf::compositing::TintableMeshComponent>();
					if (nullptr != tintableMeshComponent)
					{
						tintableMeshComponent->setTintPaletteIndex(qsf::Random::getRandomInt(0, 15));
					}

					// Give the new entity some second for better positioning. Avoid receiving event injuries direct after the spawn.
					newEntity->getOrCreateComponent<BlockedForEventComponent>();

					// Give activity
					if (!spawnInstanceData->activity.empty())
					{
						newEntity->getOrCreateComponentSafe<ActivityComponent>().setActivityAsset(qsf::AssetProxy(spawnInstanceData->activity));
					}

					if (!spawnInstanceData->equipment.empty())
					{
						std::string equipmentName = EM5_GAME.getSimulation().getSpawnPoolManager().getRandomEquipmentName(*spawnInstanceData);
						// Filter out the value "none" to support more variety with the equipment spawning
						if (!equipmentName.empty() && equipmentName != "none" && equipmentName != "None")
						{
							// Create equipment
							EntityHelper(*newEntity).showEquipment(equipmentName);
						}
					}

					// Emit message
					QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_SPAWN_ENTITY, newEntity->getId()));

					return newEntity;
				}
			}
#ifndef ENDUSER
			else
			{
				// Optional debug output if blocked, so we can find such spawn points at all
				qsf::DebugDrawProxy ddp;
				ddp.registerAt(QSF_DEBUGDRAW);
				ddp.addRequest(qsf::CircleDebugDrawRequest(transform.getPosition(), glm::vec3(0, 1, 0), 0.5f, qsf::Color4::PINK, true));
				ddp.setLifetimeInSeconds(15.0f);
				ddp.detachRequests();
			}
#endif
		}

		// Failed
		return nullptr;
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	qsf::Entity* SpawnPointComponent::spawnRandom(bool checkCollisions, const qsf::Time& fadeTime) const
	{
		if (!mList.empty())
		{
			const std::string& spawnPoolName = qsf::Random::getAnyOf(mList);

			qsf::TransformComponent& spawnPointTransformComponent = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();
			spawnPointTransformComponent.setScale(qsf::Math::GLM_VEC3_UNIT_XYZ);

			qsf::Entity* entity = spawnRandomAt(spawnPointTransformComponent.getTransform(), getEntity().getMap(), spawnPoolName, checkCollisions);
			if (nullptr != entity)
			{
				if (fadeTime > qsf::Time::ZERO)
				{
					// Start fading in
					entity->getOrCreateComponentSafe<FadeEffectComponent>().fadeIn(fadeTime);
				}
			}

			return entity;
		}

		return nullptr;
	}

	bool SpawnPointComponent::canSpawnWithoutCollision(qsf::Map& foreignMap)
	{
		qsf::Entity& entity = getEntity();

		if (!mList.empty())
		{
			const std::string& spawnPoolName = qsf::Random::getAnyOf(mList);

			qsf::TransformComponent& transformComponent = entity.getOrCreateComponentSafe<qsf::TransformComponent>();
			transformComponent.setScale(qsf::Math::GLM_VEC3_UNIT_XYZ);

			SpawnPoolManager::SpawnInstanceDataPointer spawnInstanceData = nullptr;
			const std::string& randomName = EM5_GAME.getSimulation().getSpawnPoolManager().getRandomNameFromSpawnPool(spawnPoolName, &spawnInstanceData);
			if (!randomName.empty())
			{
				if (nullptr == spawnInstanceData)
					return false;

				// Get collision from prefab, ignore child objects
				const qsf::Prototype* prototype = QSF_MAINPROTOTYPE.getPrefabByLocalAssetId(qsf::StringHash(randomName));
				if (nullptr == prototype)
					return false;

				qsf::BulletCollisionComponent* collisionComponent = prototype->getComponent<qsf::BulletCollisionComponent>();
				if (nullptr != collisionComponent)
				{
					if (qsf::CollisionHelper(foreignMap).canBeAddedWithoutCollision(transformComponent.getTransform(), *collisionComponent))
					{
						// Lets ask the old routefinder too
						{
							glm::vec3 anchorPoint, extensions;
							glm::quat rotation;
							collisionComponent->getAsOrientedBoundingBox(anchorPoint, rotation, extensions);

							qsf::ai::CollisionList collisionRange;
							qsf::ai::EM3::Router->FindCollisions(extensions, -1, qsf::ai::ERouterObjectState(transformComponent.getPosition(), glm::mat3_cast(transformComponent.getRotation())), collisionRange, qsf::ai::EOTC_ANYTHING); // call with values for right now

							if (!collisionRange.empty())
							{
								return false;
							}
						}
						return true;
					}
				}
			}
		}
		return false;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	void SpawnPointComponent::serialize(qsf::BinarySerializer& serializer)
	{
		Component::serialize(serializer);

		serializer.serialize(mList);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
