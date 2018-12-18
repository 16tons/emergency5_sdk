// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/vehicle/HelicopterComponent.h"
#include "em5/component/vehicle/parts/HelicopterHatchComponent.h"
#include "em5/component/vehicle/parts/HelicopterRotorComponent.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/component/overlay/StatusOverlayComponent.h"
#include "em5/component/movement/SwingComponent.h"
#include "em5/fire/component/FireComponent.h"
#include "em5/game/groundmap/GroundMaps.h"
#include "em5/audio/AudioHelper.h"
#include "em5/ai/em4Router/actor/EObject.h"
#include "em5/ai/em4Router/actor/EVehicle.h"
#include "em5/map/MapHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Jobs.h"

#include <qsf_game/component/base/LightControllerComponent.h>

#include <qsf_ai/navigation/em4Router/RouterComponent.h>

#include <qsf/component/link/LinkComponent.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/physics/collision/BulletBoxCollisionComponent.h>
#include <qsf/renderer/mesh/MeshComponent.h>
#include <qsf/renderer/light/LightComponent.h>
#include <qsf/renderer/particles/ParticlesComponent.h>
#include <qsf/math/CoordinateSystem.h>
#include <qsf/math/Math.h>
#include <qsf/map/Map.h>
#include <qsf/map/Entity.h>
#include <qsf/map/query/GroundMapQuery.h>
#include <qsf/job/JobArguments.h>
#include <qsf/serialization/binary/StlTypeSerialization.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 HelicopterComponent::COMPONENT_ID = qsf::StringHash("em5::HelicopterComponent");


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	void HelicopterComponent::ignoreMarkerSelectionEntityId(const qsf::Entity& entity, boost::container::flat_set<uint64>& ignoreEntityIds)
	{
		const HelicopterComponent* helicopterComponent = entity.getComponent<HelicopterComponent>();
		if (nullptr != helicopterComponent)
		{
			const qsf::Entity* markerSelectionEntity = helicopterComponent->getMarkerSelectionEntity();
			if (nullptr != markerSelectionEntity)
			{
				ignoreEntityIds.insert(markerSelectionEntity->getId());
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	HelicopterComponent::HelicopterComponent(qsf::Prototype* prototype) :
		VehicleComponent(prototype),
		mGroundLightEntityId(qsf::getUninitialized<uint64>()),
		mCollisionExtentsOnGround(qsf::Math::GLM_VEC3_UNIT_XYZ),
		mCollisionExtentsInAir(qsf::Math::GLM_VEC3_UNIT_XYZ),
		mLightEnabled(false),
		mIsFlying(false),
		mCanLandOnGround(true),
		mIsStartingOrLanding(false),
		mMarkerLightEntity(nullptr),
		mMarkerSelectionEntity(nullptr),
		mMarkerLightEnabled(true),
		mMarkerSelectorEnabled(true),
		VehicleRotorArray(*this, mVehicleRotorIds),
		VehicleLightsArray(*this, mVehicleLightIds)
	{
		setInvincible(true);	// set all helicopter invincible by default
	}

	HelicopterComponent::~HelicopterComponent()
	{
		stopDustEffects();
	}

	void HelicopterComponent::setGroundMarkerEnabled(bool light, bool selector)
	{
		if (mMarkerSelectionEntity.valid())
		{
			qsf::MeshComponent* markerSelectionMeshComponent = mMarkerSelectionEntity->getComponent<qsf::MeshComponent>();
			if (nullptr != markerSelectionMeshComponent)
			{
				markerSelectionMeshComponent->setActive(selector);
			}
		}

		if (mMarkerLightEntity.valid())
		{
			qsf::LightComponent* markerLightComponent = mMarkerLightEntity->getComponent<qsf::LightComponent>();
			if (nullptr != markerLightComponent)
			{
				markerLightComponent->setActive(light);
			}
		}
	}

	qsf::Entity* HelicopterComponent::getMarkerSelectionEntity() const
	{
		return mMarkerSelectionEntity.get();
	}

	bool HelicopterComponent::getLightEnabled() const
	{
		return mLightEnabled;
	}

	void HelicopterComponent::setLightEnabled(bool light)
	{
		if (mLightEnabled != light)
		{
			enableLights(light);
		}
	}

	const qsf::AssetProxy& HelicopterComponent::getDustEffect() const
	{
		return mDustEffectAsset;
	}

	void HelicopterComponent::setDustEffect(const qsf::AssetProxy& dustEffect)
	{
		mDustEffectAsset = dustEffect;
	}

	const qsf::AssetProxy& HelicopterComponent::getGroundMarkerAsset() const
	{
		return mGroundMarkerAsset;
	}

	void HelicopterComponent::setGroundMarkerAsset(const qsf::AssetProxy& groundMarkerAsset)
	{
		mGroundMarkerAsset = groundMarkerAsset;
	}

	uint64 HelicopterComponent::getGroundLightEntityId() const
	{
		return mGroundLightEntityId;
	}

	void HelicopterComponent::setGroundLightEntityId(uint64 groundLightEntitId)
	{
		mGroundLightEntityId = groundLightEntitId;
	}

	const glm::vec3& HelicopterComponent::getCollisionExtentsOnGround() const
	{
		return mCollisionExtentsOnGround;
	}

	void HelicopterComponent::setCollisionExtentsOnGround(const glm::vec3& collisionExtentsOnGround)
	{
		mCollisionExtentsOnGround = collisionExtentsOnGround;
	}

	bool HelicopterComponent::isFlying() const
	{
		return mIsFlying;
	}

	void HelicopterComponent::setFlying(bool flying)
	{
		mIsFlying = flying;
		FireComponent* fireComponent = getEntity().getComponent<FireComponent>();
		if (fireComponent)
		{
			fireComponent->setActive(!flying);
		}
	}

	void HelicopterComponent::forceFlying(bool flying)
	{
		setLightEnabled(flying);

		mIsFlying = flying;

		const qsf::Map& map = getEntity().getMap();
		for (uint64 rotorId : mVehicleRotorIds)
		{
			// Get rotor entity
			const qsf::Entity* entity = map.getEntityById(rotorId);
			if (nullptr != entity)
			{
				HelicopterRotorComponent* rotorComponent = entity->getComponent<HelicopterRotorComponent>();
				QSF_CHECK(rotorComponent != nullptr, "HelicopterRotorComponent::updateRotor(): To vehicle connected rotor has no em5::HelicopterRotorComponent. Check the editing of the prefab, skip this entity in update", continue);

				rotorComponent->setRotorRealSpeed(mIsFlying ? rotorComponent->SPEED_LIMIT : 0.0f);

				if (mIsFlying)
				{
					// Police drone
					if (EntityHelper(getEntity()).isUnit(CommandableComponent::UNITTAG_POLICE_DRONE))
					{
						AudioHelper::playSEKDroneMove(mAudioProxy, getEntity());
					}

					// Firefighter drone
					else if (EntityHelper(getEntity()).isUnit(CommandableComponent::UNITTAG_FIREFIGHTERS_DRONE))
					{
						AudioHelper::playFWDroneMove(mAudioProxy, getEntity());
					}

					// Normal helicopter
					else
					{
						AudioHelper::playRotorNormalSound(mAudioProxy, getEntity());
					}
				}
			}
		}

		startSwing();
	}

	bool HelicopterComponent::isRotorAtFullSpeed() const
	{
		const qsf::Map& map = getEntity().getMap();
		for (uint64 rotorId : mVehicleRotorIds)
		{
			// Get rotor entity
			const qsf::Entity* entity = map.getEntityById(rotorId);
			if (nullptr != entity)
			{
				const HelicopterRotorComponent* rotorComponent = entity->getComponent<HelicopterRotorComponent>();
				if (nullptr != rotorComponent && rotorComponent->getRotorRealSpeed() < (rotorComponent->SPEED_LIMIT - glm::epsilon<float>()))
				{
					return false;
				}
			}
		}

		return true;
	}

	bool HelicopterComponent::isRotorStopped() const
	{
		const qsf::Map& map = getEntity().getMap();
		for (uint64 rotorId : mVehicleRotorIds)
		{
			// Get rotor entity
			const qsf::Entity* entity = map.getEntityById(rotorId);
			if (nullptr != entity)
			{
				const HelicopterRotorComponent* rotorComponent = entity->getComponent<HelicopterRotorComponent>();
				if (nullptr != rotorComponent && rotorComponent->getRotorRealSpeed() > (0 + glm::epsilon<float>()))
				{
					return false;
				}
			}
		}

		return true;
	}

	bool HelicopterComponent::getCanLandOnGround() const
	{
		return mCanLandOnGround;
	}

	void HelicopterComponent::setCanLandOnGround(bool canLandOnGround)
	{
		mCanLandOnGround = canLandOnGround;
	}

	void HelicopterComponent::startDustEffects()
	{
		if (mDustEffectAsset.isValid() && !mDustEffectEntity.valid())
		{
			qsf::Map& map = getEntity().getMap();
			glm::vec3 position = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>().getPosition();

			float groundHeight = 0.0f;

			// TODO(sw) Uses deprecated ground map query constructor
			qsf::GroundMapQuery(map, nullptr, GroundMaps::FILTER_HELICOPTER_GROUND).getHeightAt(position, groundHeight);
			position.y = groundHeight;

			// Spawn dust effect on the ground
			mDustEffectEntity = map.createObjectByLocalPrefabAssetId(qsf::StringHash(mDustEffectAsset.getLocalAssetName()));
			if (mDustEffectEntity.valid())
			{
				mDustEffectEntity->getOrCreateComponentSafe<qsf::TransformComponent>().setPosition(position);
			}
		}
	}

	void HelicopterComponent::stopDustEffects()
	{
		if (mDustEffectEntity.valid())
		{
			MapHelper::destroyEntity(mDustEffectEntity.getSafe());
			mDustEffectEntity.clear();
		}
	}

	void HelicopterComponent::startSwing()
	{
		SwingComponent& swingComponent = getEntity().getOrCreateComponentSafe<SwingComponent>();
		swingComponent.setAnimationType(SwingComponent::HELICOPTERSWING);
		swingComponent.setAnimationCycle(30.0f);
		swingComponent.setAnimationIntensity(4.0f);
		swingComponent.setVelocitySkew(-0.15f);
		swingComponent.setActive(true);
	}

	void HelicopterComponent::stopSwing()
	{
		getEntity().getOrCreateComponentSafe<SwingComponent>().setActive(false);
	}

	void HelicopterComponent::setCollisionShapeAir()
	{
		qsf::BulletBoxCollisionComponent* bulletBoxCollisionComponent = getEntity().getComponent<qsf::BulletBoxCollisionComponent>();
		if (nullptr != bulletBoxCollisionComponent)
		{
			bulletBoxCollisionComponent->setHalfExtents(mCollisionExtentsInAir);

			// We need to update the router component as well
			qsf::ai::RouterComponent* routerComponent = getEntity().getComponent<qsf::ai::RouterComponent>();
			if (nullptr != routerComponent)
			{
				routerComponent->recreateCollisionBox();

				// Set the terrain class back to airplane
				qsf::ai::EGenericObject* gObject = routerComponent->getEGenericObject();
				if (nullptr != gObject && gObject->GetType() == qsf::ai::EAT_VEHICLE)
				{
					static_cast<EVehicle*>(gObject)->setHelicopterToGround(false);
				}
			}
		}
	}

	void HelicopterComponent::setCollisionShapeGround()
	{
		qsf::BulletBoxCollisionComponent* bulletBoxCollisionComponent = getEntity().getComponent<qsf::BulletBoxCollisionComponent>();
		if (nullptr != bulletBoxCollisionComponent)
		{
			bulletBoxCollisionComponent->setHalfExtents(mCollisionExtentsOnGround);

			// We need to update the router component as well
			qsf::ai::RouterComponent* routerComponent = getEntity().getComponent<qsf::ai::RouterComponent>();
			if (nullptr != routerComponent)
			{
				routerComponent->recreateCollisionBox();

				// Set the terrain class to vehicle
				qsf::ai::EGenericObject* gObject = routerComponent->getEGenericObject();
				if (nullptr != gObject && gObject->GetType() == qsf::ai::EAT_VEHICLE)
				{
					static_cast<EVehicle*>(gObject)->setHelicopterToGround(true);
				}
			}
		}
	}

	bool HelicopterComponent::isStartingOrLanding() const
	{
		return mIsStartingOrLanding;
	}

	void HelicopterComponent::setIsStartingOrLanding(bool startingOrLading)
	{
		mIsStartingOrLanding = startingOrLading;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::DamageComponent methods           ]
	//[-------------------------------------------------------]
	void HelicopterComponent::onSetDamagedLevel2()
	{
		// Call the base implementation
		VehicleComponent::onSetDamagedLevel2();
		if (isSimulating())
		{
			qsf::Entity& entity = getEntity();

			// Make it immovable
			EntityHelper(entity).makeImmovable();
			StatusOverlayComponent::showIcon(entity, StatusOverlayComponent::STATUS_ICON_DEFECT_VEHICLE, true);
		}
	}

	void HelicopterComponent::onSetDestroyed()
	{
		// Call the base implementation
		VehicleComponent::onSetDestroyed();

		if (isSimulating())
		{
			qsf::Entity& entity = getEntity();

			// Make it immovable
			EntityHelper(entity).makeImmovable();
			StatusOverlayComponent::showIcon(entity, StatusOverlayComponent::STATUS_ICON_DEFECT_VEHICLE, true);
		}
	}
	void HelicopterComponent::onFireStarted(FreeplayEvent* freeplayEvent)
	{
		// Only when not flying let the persons flee
		if (!isFlying())
		{
			personsFleeFromFire(false);
		}

		// Call base implementation
		VehicleComponent::onFireStarted(freeplayEvent);
	}

	bool HelicopterComponent::checkReceiveDamage(damagetype::DamageType damageType, qsf::Component* sender)
	{
		// No damage for flying helicopter
		if (isFlying())
		{
			return false;
		}

		// Call base class implementation
		return VehicleComponent::checkReceiveDamage(damageType, sender);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	void HelicopterComponent::onShutdown()
	{
		// Unregister jobs
		mAnimationJobProxy.unregister();

		// Destroy linked items
		qsf::Map& map = getEntity().getMap();
		if (mMarkerLightEntity.valid())
		{
			map.destroyEntityById(mMarkerLightEntity->getId());
			mMarkerLightEntity.clear();
		}
		if (mMarkerSelectionEntity.valid())
		{
			map.destroyEntityById(mMarkerSelectionEntity->getId());
			mMarkerSelectionEntity.clear();
		}

		// Call base class implementation
		VehicleComponent::onShutdown();
	}

	void HelicopterComponent::onSetSimulating(bool simulating)
	{
		// Call base class implementation
		VehicleComponent::onSetSimulating(simulating);

		if (simulating)
		{
			qsf::Map& map = getEntity().getMap();

			// Disable all lights and rotor
			forceFlying(false);

			// Register jobs
			mAnimationJobProxy.registerAt(Jobs::ANIMATION_VEHICLE, boost::bind(&HelicopterComponent::updateAnimation, this, _1));

			// Helicopter starts flying, flying helicopter can't get damage from explosions, disable the fire component at start
			FireComponent* fireComponent = getEntity().getComponent<FireComponent>();
			if (fireComponent)
			{
				fireComponent->setActive(false);
			}

			// TODO(co) Test
			if (mGroundMarkerAsset.isValid())
			{
				mMarkerSelectionEntity = map.createObjectByLocalPrefabAssetId(mGroundMarkerAsset.getLocalAssetId());
				QSF_CHECK(mMarkerSelectionEntity.valid(), "EM5: HelicopterComponent failed to create the marker selection entity instance", return);

				// Delete fire component for marker
				mMarkerSelectionEntity->destroyComponent<FireComponent>();

				// Link to parent
				qsf::LinkComponent* linkComponent = mMarkerSelectionEntity->getOrCreateComponent<qsf::LinkComponent>();
				linkComponent->setParentId(getEntityId());
				linkComponent->setParentLinkType(qsf::LinkComponent::HARD_LINK);
			}

			// Get the light marker
			if (qsf::isInitialized(mGroundLightEntityId))
			{
				mMarkerLightEntity = map.getEntityById(mGroundLightEntityId);
				QSF_CHECK(mMarkerLightEntity.valid(), "EM5: HelicopterComponent failed to find the spot light entity instance", return);

				// Link to parent
				qsf::LinkComponent* linkComponent = mMarkerLightEntity->getOrCreateComponent<qsf::LinkComponent>();
				linkComponent->setParentId(getEntityId());
				linkComponent->setParentLinkType(qsf::LinkComponent::HARD_LINK);

				// Backup the initial value for collision in air (edited values)
				qsf::BulletBoxCollisionComponent* bulletBoxCollisionComponent = getEntity().getComponent<qsf::BulletBoxCollisionComponent>();
				if (nullptr != bulletBoxCollisionComponent)
				{
					mCollisionExtentsInAir = bulletBoxCollisionComponent->getHalfExtents();
				}
			}
		}
		else
		{
			// Unregister jobs
			mAnimationJobProxy.unregister();
		}
	}

	void HelicopterComponent::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base class implementation
		VehicleComponent::serialize(serializer);

		// TODO(mk) Complete serialization
		serializer.serialize(mVehicleDoorsIds);
		serializer.serialize(mVehicleRotorIds);
		serializer.serialize(mMarkerLightEnabled);
		serializer.serialize(mMarkerSelectorEnabled);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void HelicopterComponent::updateAnimation(const qsf::JobArguments& jobArguments)
	{
		// Rotor
		updateRotor(jobArguments.getTimePassed());

		// Update positions of ground marker and light marker
		updateGroundMarker();
	}

	void HelicopterComponent::updateRotor(const qsf::Time& timePassed)
	{
		const qsf::Map& map = getEntity().getMap();
		for (uint64 rotorId : mVehicleRotorIds)
		{
			// Get rotor entity
			const qsf::Entity* entity = map.getEntityById(rotorId);
			if (nullptr != entity)
			{
				HelicopterRotorComponent* rotorComponent = entity->getComponent<HelicopterRotorComponent>();
				QSF_CHECK(rotorComponent != nullptr, "HelicopterRotorComponent::updateRotor(): To vehicle connected rotor has no em5::HelicopterRotorComponent. Check the editing of the prefab, skip this entity in update", continue);

				// Update the speed
				if (mIsFlying)
				{
					// Accelerate the rotor
					if (!rotorComponent->hasMaximalSpeed())
					{
						// Give start boost
						if (rotorComponent->getRotorRealSpeed() < glm::pi<float>() / 10.0f)
						{
							rotorComponent->setRotorRealSpeed(glm::pi<float>() / 10.0f);

							// Police drone
							if (EntityHelper(getEntity()).isUnit(CommandableComponent::UNITTAG_POLICE_DRONE))
							{
								AudioHelper::playSEKDroneMove(mAudioProxy, getEntity());
							}

							// Firefighter drone
							else if (EntityHelper(getEntity()).isUnit(CommandableComponent::UNITTAG_FIREFIGHTERS_DRONE))
							{
								AudioHelper::playFWDroneMove(mAudioProxy, getEntity());
							}

							else
							{
								AudioHelper::playRotorAcceleratingSound(mAudioProxy, getEntity());
							}
						}

						const float acceleration = std::pow(rotorComponent->SPEED_ACCELERATION_FACTOR, timePassed.getSeconds());
						rotorComponent->setRotorRealSpeed(rotorComponent->getRotorRealSpeed() * acceleration);

						if (rotorComponent->hasMaximalSpeed())
						{
							rotorComponent->setRotorRealSpeed(rotorComponent->SPEED_LIMIT);

							// Police drone
							if (EntityHelper(getEntity()).isUnit(CommandableComponent::UNITTAG_POLICE_DRONE))
							{
								//AudioHelper::playSEKDroneMove(mAudioProxy, getEntity());	// TODO(co) Why is this commented?
							}

							// Firefighter drone
							else if (EntityHelper(getEntity()).isUnit(CommandableComponent::UNITTAG_FIREFIGHTERS_DRONE))
							{
								//AudioHelper::playFWDroneMove(mAudioProxy, getEntity());	// TODO(co) Why is this commented?
							}

							else
							{
								AudioHelper::playRotorNormalSound(mAudioProxy, getEntity());
							}
						}
					}
				}
				else
				{
					if (rotorComponent->hasMaximalSpeed())
					{
						if (EntityHelper(getEntity()).isUnit(CommandableComponent::UNITTAG_POLICE_DRONE) ||
							EntityHelper(getEntity()).isUnit(CommandableComponent::UNITTAG_FIREFIGHTERS_DRONE))
						{
							mAudioProxy.stop();
						}
						else
						{
							AudioHelper::playRotorDecceleratingSound(mAudioProxy, getEntity());
						}
					}

					// Deccelerate the rotor
					if (rotorComponent->getRotorRealSpeed() > 0.0f + glm::epsilon<float>())
					{
						const float deceleration = std::pow(rotorComponent->SPEED_DECELERATION_FACTOR, -timePassed.getSeconds());
						rotorComponent->setRotorRealSpeed(rotorComponent->getRotorRealSpeed() * deceleration);
						if (rotorComponent->getRotorRealSpeed() < glm::pi<float>() / 10.0f)
						{
							rotorComponent->setRotorRealSpeed(0);
						}
					}
				}

				// Rotate all rotors
				rotorComponent->updateRotor(timePassed);
			}
		}
	}

	void HelicopterComponent::updateGroundMarker()
	{
		if (mMarkerLightEnabled || mMarkerSelectorEnabled)
		{
			const glm::vec3& position = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>().getPosition();

			// Orientate the selection marker on terrain
			glm::vec3 groundNormal;
			glm::vec3 groundPosition = position;
			qsf::GroundMapQuery(getEntity().getMap()).getHeightAndNormalAtXZPosition(groundPosition.x, groundPosition.z, groundPosition.y, groundNormal);
			groundPosition.y += 0.25f;
			glm::quat groundRotation = qsf::Math::getLookAtQuaternion(		// A zero ground normal vector3 causes problems
				(groundNormal != qsf::Math::GLM_VEC3_ZERO) ? groundNormal : qsf::Math::GLM_VEC3_UNIT_Y, qsf::CoordinateSystem::getUp());

			if (mMarkerSelectionEntity.valid())
			{
				qsf::LinkComponent* markerSelectionLinkComponent = mMarkerSelectionEntity->getComponent<qsf::LinkComponent>();
				if (nullptr != markerSelectionLinkComponent)
				{
					qsf::Entity* parentEntity = getEntity().getMap().getEntityById(markerSelectionLinkComponent->getParentId());
					if (nullptr != parentEntity)
					{
						qsf::TransformComponent& parentTransformComponent = parentEntity->getOrCreateComponentSafe<qsf::TransformComponent>();

						markerSelectionLinkComponent->setLocalPosition(groundPosition - parentTransformComponent.getPosition());
						markerSelectionLinkComponent->setLocalRotation(groundRotation * glm::inverse(parentTransformComponent.getRotation()));
					}
				}
			}

			if (mMarkerLightEntity.valid())
			{
				// Set the near clip plane of the light entity
				const float heightOverGround = position.y - groundPosition.y;
				const float lightNearClipDistance = heightOverGround - 0.5f;

				qsf::LightComponent* lightComponent = mMarkerLightEntity->getComponent<qsf::LightComponent>();
				if (nullptr != lightComponent)
				{
					// Clip a little bit under over the ground
					lightComponent->setSpotlightNearClipDistance(lightNearClipDistance);
				}

				// Same for all linked childs
				qsf::LinkComponent* linkComponent = mMarkerLightEntity->getComponent<qsf::LinkComponent>();
				if (nullptr != linkComponent)
				{
					for (const qsf::LinkComponent* childComponent : linkComponent->getChildLinks())
					{
						qsf::LightComponent* childLightComponent = childComponent->getEntity().getComponent<qsf::LightComponent>();
						if (nullptr != childLightComponent)
						{
							childLightComponent->setSpotlightNearClipDistance(lightNearClipDistance);
						}
					}
				}
			}
		}
	}

	void HelicopterComponent::enableLights(bool enable)
	{
		mLightEnabled = enable;

		const qsf::Map& map = getEntity().getMap();
		for (uint64 lightEntityId : mVehicleLightIds)
		{
			// Get the entity
			const qsf::Entity* lightEntity = map.getEntityById(lightEntityId);
			if (nullptr != lightEntity)
			{
				qsf::game::LightControllerComponent* lightControllerComponent = lightEntity->getComponent<qsf::game::LightControllerComponent>();
				if (nullptr != lightControllerComponent)
				{
					lightControllerComponent->setActive(enable);
				}
			}
		}
	}

	void HelicopterComponent::initializePrefabClearComponentExceptions()
	{
		mComponentExceptionIds.reserve(2);
		mComponentExceptionIds.push_back(FireComponent::COMPONENT_ID);
		mComponentExceptionIds.push_back(qsf::ParticlesComponent::COMPONENT_ID);
	}

	void HelicopterComponent::restoreEntitiesFromPrefab(const qsf::Prototype& prototype, std::unordered_map<uint64, uint64>& prototypeIdMap)
	{
		const qsf::Entity& entity = getEntity();
		qsf::Map& map = entity.getMap();

		const HelicopterComponent* prototypeHelicopterComponent = prototype.getComponent<HelicopterComponent>();
		const qsf::LinkComponent* prototypeLinkComponent = prototype.getComponent<qsf::LinkComponent>();
		if (nullptr != prototypeHelicopterComponent && nullptr != prototypeLinkComponent)
		{
			// Additional components which need to be restored
			std::vector<uint32> componentIds;
			componentIds.push_back(HelicopterHatchComponent::COMPONENT_ID);

			cloneEntitiesFromPrefabByComponentId(map, entity, componentIds, *prototypeLinkComponent, prototypeIdMap);

			// Doors (VehicleComponent)
			cloneEntitiesFromPrefab(map, entity, prototypeHelicopterComponent->mVehicleDoorsIds, mVehicleDoorsIds, *prototypeLinkComponent, prototypeIdMap);
			precalculateDoorsToEntitiesMap();

			// Rotor
			cloneEntitiesFromPrefab(map, entity, prototypeHelicopterComponent->mVehicleRotorIds, mVehicleRotorIds, *prototypeLinkComponent, prototypeIdMap);
		}
		else
		{
			QSF_WARN("QSF failed to restore the original prefab of entity ID " << entity.getId(), QSF_REACT_NONE);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
