// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/gangster/BeThrownWeaponAction.h"
#include "em5/action/move/FullBrakeAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/fire/component/FireComponent.h"
#include "em5/health/HealthComponent.h"
#include "em5/component/vehicle/VehicleComponent.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/specs/ActionRangeSpecsGroup.h"
#include "em5/audio/AudioHelper.h"

#include <qsf/audio/AudioProxy.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/move/MovableComponent.h>
#include <qsf/component/utility/SelfDestructComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/map/query/EntityMapQuery.h>
#include <qsf/map/query/GroundMapQuery.h>
#include <qsf/map/Map.h>
#include <qsf/math/Math.h>
#include <qsf/physics/collision/BulletBoxCollisionComponent.h>
#include <qsf/physics/collision/CollisionHelper.h>
#include <qsf/physics/PhysicsWorldComponent.h>
#include <qsf/serialization/binary/GlmTypeSerialization.h>

#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include <boost/foreach.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier BeThrownWeaponAction::ACTION_ID = "em5::BeThrownWeaponAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	BeThrownWeaponAction::BeThrownWeaponAction() :
		Action(ACTION_ID),
		mAccumulatedTime(0.0f),
		mThrowerEntityId(qsf::getUninitialized<uint64>()),
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mCurrentState(STATE_INIT),
		mThrownWeapon(weapon::STONE),
		mThrowToGround(false),
		mCanPlayAudio(true)
	{
		// Nothing here
	}

	BeThrownWeaponAction::~BeThrownWeaponAction()
	{
		// Nothing here
	}

	void BeThrownWeaponAction::init(uint64 throwerEntityId, uint64 targetEntityId, weapon::Weapon thrownWeapon, bool throwToGround)
	{
		mThrowerEntityId = throwerEntityId;
		mTargetEntityId = targetEntityId;
		mInitialPosition = getEntity().getOrCreateComponent<qsf::TransformComponent>()->getPosition();
		mThrownWeapon = thrownWeapon;
		mThrowToGround = throwToGround;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void BeThrownWeaponAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mAccumulatedTime);
		serializer.serialize(mTargetEntityId);
		serializer.serialize(mTargetPosition);
		serializer.serializeAs<uint16>(mCurrentState);
		serializer.serializeAs<uint16>(mThrownWeapon);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	void BeThrownWeaponAction::onShutdown()
	{
		// Destroy the thrown object
		//  -> Don't do it directly, as we're currently still using the entity
		//  -> Let a component do the dirty work when it's a safe time to do so
		getEntity().getOrCreateComponentSafe<qsf::SelfDestructComponent>().setTimeout(qsf::Time::ZERO);
	}

	qsf::action::Result BeThrownWeaponAction::updateAction(const qsf::Clock& clock)
	{
		mAccumulatedTime += clock.getTimePassed().getSeconds();

		const qsf::Entity& throwableEntity = getEntity();
		qsf::Map& map = getMap();

		QSF_CHECK(throwableEntity.getComponent<qsf::MovableComponent>(), "No MovableComponent attached! This is crucial for physics stuff!", return qsf::action::RESULT_DONE);
		QSF_CHECK(throwableEntity.getComponent<qsf::BulletCollisionComponent>(), "No BulletCollisionComponent attached! This is crucial for physics stuff!", return qsf::action::RESULT_DONE);
		QSF_CHECK(qsf::isInitialized(mTargetEntityId), "No valid target entity ID!", return qsf::action::RESULT_DONE);

		// The actual target
		qsf::Entity* targetEntity = map.getEntityById(mTargetEntityId);
		if (nullptr == targetEntity)
		{
			// Target despawned or similar...
			return qsf::action::RESULT_DONE;
		}

		// TODO(ks) set via specs
		const float timeFactor = 1.0f;

		switch (mCurrentState)
		{
			case STATE_INIT:
			{
				// Initialization
				const qsf::TransformComponent* targetTransformComponent = targetEntity->getOrCreateComponent<qsf::TransformComponent>();

				// This is initialized only here. So if the target moves, it might not be hit (intended!)
				qsf::BulletBoxCollisionComponent* bulletBoxCollisionComponent = targetEntity->getComponent<qsf::BulletBoxCollisionComponent>();
				if (!mThrowToGround && nullptr != bulletBoxCollisionComponent)
				{
					// Use bounding box center if possible
					const btVector3& boundingBoxCenter = bulletBoxCollisionComponent->getTransform().getOrigin();
					mTargetPosition = glm::vec3(boundingBoxCenter.x(), boundingBoxCenter.y(), boundingBoxCenter.z());
				}
				else
				{
					mTargetPosition = targetTransformComponent->getPosition();
				}

				mAccumulatedTime = 0.0f;

				mCurrentState = STATE_FLYING;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_FLYING:
			{
				// Update position of flying object

				const float t = timeFactor * mAccumulatedTime;
				const float speedXZ = 10.0f;

				// It is not really realistic to mess around with gravity, but this yields best results
				float dist = glm::length(mTargetPosition - mInitialPosition);
				dist = glm::clamp(0.1f * dist, 0.5f, 20.0f);
				const float aY = 1.0f / (dist * -9.81f);

				// Not yet reached the goal
				const glm::vec3 diff = (mTargetPosition - mInitialPosition);

				glm::vec3 diffXZ = diff;
				diffXZ.y = 0.0f;
				const float tmax = glm::length(diffXZ) / speedXZ;
				const float vY = (diff.y - 0.5f * tmax * tmax * aY) / tmax;

				const glm::vec3 velocity = glm::normalize(diffXZ) * speedXZ + glm::vec3(0, vY, 0);

				// Fly a simple parabola...
				const glm::vec3 newPos = mInitialPosition + t * velocity + 0.5f * t * t * glm::vec3(0, aY, 0);

				throwableEntity.getComponent<qsf::MovableComponent>()->moveToPositionAndRotation(newPos, qsf::Math::GLM_QUAT_IDENTITY);

				const qsf::PhysicsWorldComponent* physicsWorldComponent = map.getCoreEntity().getComponent<qsf::PhysicsWorldComponent>();
				if (nullptr == physicsWorldComponent)
				{
					return qsf::action::RESULT_DONE;
				}

				const auto& allCollisisons = physicsWorldComponent->getCurrentCollisions();

				qsf::BulletCollisionComponent* ownCollisionComponent = throwableEntity.getComponent<qsf::BulletCollisionComponent>();

				// Check for collision with other bullet objects
				const auto collisionRange = allCollisisons.equal_range(&ownCollisionComponent->getWrappedBulletCollisionObject());
				{
					// Default reaction is stop immediately on any real (entity) collision.
					// These can be detected by a user pointer that is not null
					// TODO(sw) We cannot use a range based for loop here because collisionRange is a std::pair which defines the range
					//          But the range base for loop expects a begin() and end() methods
					BOOST_FOREACH(auto collision, collisionRange)
					{
						// Check for valid BulletCollisionComponent this object has collided with
						if (collision.second->getUserPointer())
						{
							// Collision!
							qsf::BulletCollisionComponent& collisionComponent = *static_cast<qsf::BulletCollisionComponent*>(collision.second->getUserPointer());
							qsf::Entity& hitEntity = collisionComponent.getEntity();

							// Ignore thrower himself (we collide with him at first)
							if (hitEntity.getId() != mThrowerEntityId)
							{
								onCollision(&hitEntity);
								return qsf::action::RESULT_DONE;
							}
						}
					}
				}

				// No collision yet; check for collision with ground
				float height = 0.0f;
				if (qsf::GroundMapQuery(map).getHeightAtXZPosition(newPos.x, newPos.z, height))
				{
					if (newPos.y < height)
					{
						// Hit the ground
						onCollision(nullptr);
						return qsf::action::RESULT_DONE;
					}
				}

				// When thrown weapon is still alive after 30 seconds, assume something went wrong
				if (t >= 30.0f)
				{
					// No hit
					onCollision(nullptr);
					return qsf::action::RESULT_DONE;
				}

				return qsf::action::RESULT_CONTINUE;
			}
		}

		return qsf::action::RESULT_DONE;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void BeThrownWeaponAction::onCollision(qsf::Entity* hitTarget)
	{
		// TransformComponent always exists
		const qsf::TransformComponent& transformComponent = getEntity().getComponentSafe<qsf::TransformComponent>();

		switch (mThrownWeapon)
		{
			case weapon::STONE:
			{
				// Stone hit target
				if (nullptr != hitTarget)
				{
					static const float DAMAGE_TO_PERSONS = 0.05f;
					static const float DAMAGE_TO_VEHICLES = 0.02f;

					HealthComponent* healthComponent = hitTarget->getComponent<HealthComponent>();
					if (nullptr != healthComponent)
					{
						// Whenever the physics for persons works, "hitTarget" might be a person and should thus be harmed
						float newHealthEnergy = healthComponent->getHealthEnergy() - healthComponent->getMaximumHealthEnergy() * DAMAGE_TO_PERSONS;
						if (newHealthEnergy < 0.0f)
						{
							healthComponent->injurePersonByEventById(injury::INNER_BLEEDING, EntityHelper(getEntity()).getFreeplayEvent());
						}
						else
						{
							healthComponent->setHealthEnergy(newHealthEnergy);
						}
					}
					else
					{
						VehicleComponent* vehicleComponent = hitTarget->getComponent<VehicleComponent>();
						if (nullptr != vehicleComponent)
						{
							// A vehicle was hit!
							vehicleComponent->applyDamage(DAMAGE_TO_VEHICLES, EntityHelper(getEntity()).getFreeplayEvent());
						}
					}
				}

				// TODO(fw): Removed the particle effect here, as we don't seem to have an appropriate one
				#if 0
					static const qsf::StringHash stoneBreakParticles = "em5/prefab/particles/molotov_cocktail_explosion";
					const float timeout = 5.0f;	// TODO(mz): just set some likely value, was not deleting itself at all before, might not be correct
					MapHelper(getEntity().getMap()).spawnParticle(stoneBreakParticles, timeout, transformComponent.getPosition(), glm::vec3(0.05f));
				#endif

				break;
			}

			case weapon::MOLOTOV:
			{
				// Check for direct hit
				if (nullptr != hitTarget)
				{
					static const float DAMAGE_TO_PERSONS = 0.4f;
					static const float DAMAGE_TO_VEHICLES = 0.2f;

					HealthComponent* healthComponent = hitTarget->getComponent<HealthComponent>();
					if (nullptr != healthComponent)
					{
						// Whenever the physics for persons works, "hitTarget" might be a person and should thus be harmed
						healthComponent->setHealthEnergy(healthComponent->getHealthEnergy() - healthComponent->getMaximumHealthEnergy() * DAMAGE_TO_PERSONS);
					}
					else
					{
						VehicleComponent* vehicleComponent = hitTarget->getComponent<VehicleComponent>();
						if (nullptr != vehicleComponent)
						{
							// A vehicle was hit!
							vehicleComponent->applyDamage(DAMAGE_TO_VEHICLES, EntityHelper(getEntity()).getFreeplayEvent());
						}
					}
				}

				static const qsf::StringHash explosionId("em5/prefab/particles/molotov_cocktail_explosion");
				const float timeout = 5.0f; // TODO(mz): just set some likely value, was not deleting itself at all before, might not be correct
				MapHelper(getEntity().getMap()).spawnParticle(explosionId, timeout, transformComponent.getPosition(), glm::vec3(1.f));

				// Get freeplay event from target or owner
				FreeplayEvent* freeplayEvent = nullptr;

				// Assume throwable has an EventIdComponent and is part of an event (It should, as "ThrowWeaponAction" did that)
				EventIdComponent* eventIdComponent = getEntity().getComponent<EventIdComponent>();
				if (nullptr != eventIdComponent)
				{
					freeplayEvent = eventIdComponent->getFreeplayEvent();
				}

				// Check for a valid FreeplayEvent to set
				if (nullptr != freeplayEvent)
				{
					// TODO(ks) set via specs
					const float explosionAndFireRadius = 5.0f;

					std::vector<FireComponent*> fireComponentVector;
					qsf::ComponentMapQuery(getMap()).getInstancesInCircle(transformComponent.getPosition(), explosionAndFireRadius, fireComponentVector);

					// Start fire
					for (FireComponent* fireComponent : fireComponentVector)
					{
						fireComponent->startFire(freeplayEvent);
					}
				}

				break;
			}

			case weapon::STUNGRENADE:
			{
				// Show the explosion effect
				static const qsf::StringHash particleId("em5/prefab/particles/flashgrenade");
				const float timeout = 1.5f; // Seems to be approximately the length where stuff happens
				MapHelper(getEntity().getMap()).spawnParticle(particleId, timeout, transformComponent.getPosition(), glm::vec3(1.5f));


				// Find all persons in the target radius
				const float stunGrenadeRadius = ActionRangeSpecsGroup::getInstanceSafe().getStunGrenadeRadius();
				std::vector<PersonComponent*> persons;
				qsf::ComponentMapQuery(getMap()).getInstancesInCircle<PersonComponent>(transformComponent.getPosition(), stunGrenadeRadius, persons);

				const float stunTime = ActionRangeSpecsGroup::getInstanceSafe().getStunGrenadeStunTime();

				for (PersonComponent* personComponent : persons)
				{
					// Squads are immune
					EntityHelper entityHelper(personComponent->getEntity());
					if (entityHelper.isSquadPerson())
						continue;

					if (entityHelper.isPersonInjured())
						continue;

					// Everyone one else is stunned
					personComponent->setStunned(mThrowerEntityId, qsf::Time::fromSeconds(stunTime), false);

					if (mCanPlayAudio)
					{
						mCanPlayAudio = false;

						// Play audio
						AudioProxy audioProxy;
						AudioHelper::playSEKFlashGrenade(audioProxy, personComponent->getEntity());
						audioProxy.detach();
						audioProxy.synchronizeWithMultiplay();
					}
				}

				// Find all vehicles in the target radius
				std::vector<RoadVehicleComponent*> vehicles;
				qsf::ComponentMapQuery(getMap()).getInstancesInCircle<RoadVehicleComponent>(transformComponent.getPosition(), stunGrenadeRadius, vehicles);

				for (const RoadVehicleComponent* roadVehicleComponent : vehicles)
				{
					qsf::Entity& vehicleEntity = roadVehicleComponent->getEntity();

					// Squads drive on
					if (!EntityHelper(vehicleEntity).isSquadRoadVehicle())
					{
						// Everyone one else hit's the brakes
						qsf::ActionComponent& actionComponent = vehicleEntity.getOrCreateComponentSafe<qsf::ActionComponent>();
						actionComponent.pushAction<FullBrakeAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(true);		// then remove all actions
						actionComponent.pushAction<FullBrakeAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(false);	// first slow down
					}
				}

				break;
			}

			default:
				// Nothing to do in this case
				break;
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
