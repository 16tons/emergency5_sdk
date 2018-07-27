// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/fire/AreaExtinguishAction.h"
#include "em5/audio/AudioHelper.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/base/GameplayAssets.h"
#include "em5/component/vehicle/parts/HelicopterHatchComponent.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include "em5/fire/component/FireComponent.h"
#include "em5/health/HealthComponent.h"
#include "em5/game/groundmap/GroundMaps.h"
#include "em5/game/minimap/MinimapHelper.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/logic/local/firefighters/CommandoCarLogic.h"
#include "em5/logic/HintHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/specs/RoadVehicleSpecsGroup.h"
#include "em5/specs/FireSpecsGroup.h"
#include "em5/EM5Helper.h"

#include <qsf_game/base/DistanceHelper.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/component/move/MovableComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/math/Math.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/map/component/MapPropertiesComponent.h>
#include <qsf/map/query/GroundMapQuery.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/renderer/particles/ParticlesComponent.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier AreaExtinguishAction::ACTION_ID = "em5::AreaExtinguishAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	AreaExtinguishAction::AreaExtinguishAction() :
		Action(ACTION_ID),
		mState(STATE_INIT),
		mCanPlayPersonDamageHint(true),
		mDurration(qsf::Time::ZERO),
		mDurrationAirEffect(qsf::Time::ZERO),
		mTotalDuration(qsf::Time::fromSeconds(1.0f)),
		mExtinguishPowerPerSecond(30),
		mCoolingPowerPerSecond(500),
		mRadius(10),
		mTimeLastGroundEffect(qsf::Time::ZERO)
	{
		// Nothing here
	}

	AreaExtinguishAction::~AreaExtinguishAction()
	{
		// Nothing here
	}

	void AreaExtinguishAction::init(glm::vec3 targetPosition, qsf::Time durration, float radius, float extinguishPowerPerSecond, float coolingPowerPerSecond)
	{
		mTargetPosition = targetPosition;
		mDurration = qsf::Time::ZERO;
		mTotalDuration = durration;
		mRadius = radius;
		mExtinguishPowerPerSecond = extinguishPowerPerSecond;
		mCoolingPowerPerSecond = coolingPowerPerSecond;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void AreaExtinguishAction::serialize(qsf::BinarySerializer& serializer)
	{
		// TODO(mk) Add serialize function
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool AreaExtinguishAction::onStartup()
	{
		// Action is send without move order before, no additional checks necessary
		return true;
	}

	void AreaExtinguishAction::onShutdown()
	{
		deleteParticleEffects();

		if (mAirplaneEntity.valid())
		{
			MapHelper(getMap()).destroyEntity(mAirplaneEntity.getSafe());
		}
	}

	qsf::action::Result AreaExtinguishAction::updateAction(const qsf::Clock& clock)
	{
		if (mAirplaneEntity.valid())
		{
			// Valid after the initialization state
			updateAirplanePosition(clock.getTimePassed());
		}

		const float distanceToTarget = (mAirplaneEntity.valid() ? qsf::game::DistanceHelper::get2dDistance(mTargetPosition, mAirplaneEntity.getSafe()) : 0);

		switch (mState)
		{
			case STATE_INIT:
			{
				// Sound for giving the command (should not synchronized with multiplayer, only the local player gets this feedback)
				AudioHelper::playCommandoCarUseFirePlane(mAudioProxyUseAirplaneFeedback);

				createAirPlaneEntity();
				createGroundEffectMarker();

				// Get the radar entity from the commando car
				RoadVehicleComponent* roadVehicleComponent = getEntity().getComponent<RoadVehicleComponent>();
				if (nullptr != roadVehicleComponent)
				{
					mRadarEntity = getMap().getEntityById(roadVehicleComponent->getSpecialEntity());
					if (mRadarEntity.valid())
					{
						mInitialRadarRotation = mRadarEntity->getOrCreateComponentSafe<qsf::TransformComponent>().getRotation();

						// Play sound
						AudioHelper::playCommandoCarRadarSound(mAudioProxyRadarNoise, mRadarEntity.getSafe());
						mAudioProxyRadarNoise.synchronizeWithMultiplay();
					}
				}

				// Activate the skill cool down
				CommandoCarLogic* commandoCarLogic = EntityHelper(getEntity()).getGameLogic<CommandoCarLogic>();
				if (nullptr != commandoCarLogic)
				{
					commandoCarLogic->activateAreaExtinguishSkill();
				}

				mState = STATE_FLY_TO_POSITION;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_FLY_TO_POSITION:
			{
				if (distanceToTarget > 20)
				{
					return qsf::action::RESULT_CONTINUE;
				}

				// Create particle effect entities
				createAirParticleEffect();

				// Add a short delay between the creating of the effect in air and the effect on ground
				mState = STATE_WATER_IMPACT_DELAY;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_WATER_IMPACT_DELAY:
			{
				// The first ground particle has an delay (to simulate the "falling" from the air)
				const qsf::Time startDelay = qsf::Time::fromSeconds(FireSpecsGroup::getInstanceSafe().getCommandoCarExtinguishStartDelay());
				if (mDurrationAirEffect < startDelay)
				{
					mDurrationAirEffect += clock.getTimePassed();	// The duration here is used as delay for the first ground effect
				}

				// Create the first effect with a delay
				createGroundParticleEffect();
				mTimeLastGroundEffect = qsf::Time::ZERO;

				mExtinguishFireComponents.clear();
				qsf::ComponentMapQuery(getMap()).getInstancesInCircle(mTargetPosition, mRadius, mExtinguishFireComponents);
				for (FireComponent* fireComponent : mExtinguishFireComponents)
				{
					// Register as cooling energy source and the value
					fireComponent->addCoolingEnergySource(getEntityId(), mExtinguishPowerPerSecond, mCoolingPowerPerSecond);
				}

				damagePersonsInArea(clock.getTimePassed().getSeconds());

				mState = STATE_EXTINGUISH;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_EXTINGUISH:
			{
				if (mDurrationAirEffect > mTotalDuration)
				{
					// Disable the air effect, this must happen before the
					deactivateEffectParticleAir();
				}

				if (mDurration < mTotalDuration)
				{
					damagePersonsInArea(clock.getTimePassed().getSeconds());

					// Extinguish all the time
					mDurration += clock.getTimePassed();
					mDurrationAirEffect += clock.getTimePassed();

					// TODO(mk) Detect if the fire components with cooling energy is still in the radius of the targetPosition

					updateGroundParticle(clock.getTimePassed());

					return qsf::action::RESULT_CONTINUE;
				}

				// Deactivate the emitter of the effect particles, to slowly fade out the effect
				deactivateEffectParticle();
				closeHatch();

				// Unregister as cooling energy source and the value
				for (FireComponent* fireComponent : mExtinguishFireComponents)
				{
					fireComponent->removeCoolingEnergySource(getEntityId());
				}

				// Rotate the radar entity back to initial position
				if (mRadarEntity.valid())
				{
					qsf::ActionComponent& actionComponent = mRadarEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
					actionComponent.pushAction<TurnToAction>().initWithRotation(mInitialRadarRotation);

					mAudioProxyRadarNoise.stop();
					mAudioProxyRadarNoise.synchronizeWithMultiplay();
				}

				mState = STATE_FLY_AWAY;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_FLY_AWAY:
			{
				if (EntityHelper(mAirplaneEntity.getSafe()).isInMapBoundaries())
				{
					return qsf::action::RESULT_CONTINUE;
				}

				if (mAirplaneEntity.valid())
				{
					QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_MINIMAP_REMOVE_ENTITY_ICON, mAirplaneEntity->getId()));
					MapHelper(getMap()).destroyEntity(mAirplaneEntity.getSafe());
				}
			}
		}

		return qsf::action::RESULT_DONE;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void AreaExtinguishAction::createAirPlaneEntity()
	{
		// Create and positioning of the "extinguish airplane" entity
		qsf::Map& map = getMap();
		mAirplaneEntity = MapHelper(map).createObjectByLocalPrefabAssetId(assets::PREFAB_FIREFIGHTING_AIRPLANE);
		if (mAirplaneEntity.valid())
		{
			glm::vec3 placePosition = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>().getPosition();	// Default position, should never used

			qsf::MapPropertiesComponent* mapPropertiesComponent = map.getCoreEntity().getComponent<qsf::MapPropertiesComponent>();
			if (nullptr != mapPropertiesComponent)
			{
				placePosition = mapPropertiesComponent->getMapBoundaryBottomRight();
			}

			// Turn airplane to target
			placePosition.y = mTargetPosition.y;	// Temporary lift the airplane on the same level as the target
			glm::quat rotation = qsf::Math::getLookAtQuaternion(placePosition, mTargetPosition, qsf::CoordinateSystem::getUp());

			// Get the helicopter height for the position
			float groundHeight = 0.0f;
			qsf::GroundMapQuery(map, GroundMaps::FILTER_HELICOPTER_GROUND).getHeightAt(placePosition, groundHeight);
			placePosition.y = groundHeight + RoadVehicleSpecsGroup::getInstanceSafe().getHelicopterFlyHeight();

			qsf::MovableComponent& movableComponent = mAirplaneEntity->getOrCreateComponentSafe<qsf::MovableComponent>();
			movableComponent.warpToPositionAndRotation(placePosition, rotation);

			// Setup ghost component for airplane
			EntityHelper::setupEntityAsGhost(*mAirplaneEntity, getEntity());

			// Added sound
			AudioHelper::playFirePlaneEffectStart(mAudioProxyAirplane, mAirplaneEntity.getSafe());
			mAudioProxyAirplane.synchronizeWithMultiplay();

			// Create minimap icon
			MinimapHelper::registerIcon(*mAirplaneEntity, MiniMapIconType::UNIT_VEHICLE_FIREPLANE);
		}
	}

	void AreaExtinguishAction::createGroundEffectMarker()
	{
		// Create the effect entity again
		mGroundMarkerEntity = MapHelper(getMap()).createObjectByLocalPrefabAssetId(assets::PREFAB_FIREFIGHTING_AIRPLANE_GROUNDMARKER);
		qsf::MovableComponent& movableComponent = mGroundMarkerEntity->getOrCreateComponentSafe<qsf::MovableComponent>();
		movableComponent.warpToPositionAndRotation(mTargetPosition, movableComponent.getRotation());
	}

	void AreaExtinguishAction::createAirParticleEffect()
	{
		mEffectParticleAir = MapHelper(getMap()).createObjectByLocalPrefabAssetId(assets::PREFAB_FIREFIGHTING_AIRPLANE_AIREFFECT);
		if (mEffectParticleAir.valid())
		{
			// Link to the hatch child of the airplane
			const qsf::LinkComponent& linkComponent = mAirplaneEntity->getOrCreateComponentSafe<qsf::LinkComponent>();
			const HelicopterHatchComponent* helicopterHatchComponent = linkComponent.getComponentFromEntityOrLinkedChild<HelicopterHatchComponent>();
			if (nullptr != helicopterHatchComponent)
			{
				qsf::Entity& helicopterHatchEntity = helicopterHatchComponent->getEntity();

				// Place the new effect
				qsf::TransformComponent& transformComponent = helicopterHatchEntity.getOrCreateComponentSafe<qsf::TransformComponent>();
				glm::vec3 effectPosition = transformComponent.getPosition();

				// Backup the safe position
				mEffectStartPosition = effectPosition;
				effectPosition.y -= 5;	// 5m below the real hatch position

				mEffectParticleAir->getOrCreateComponentSafe<qsf::TransformComponent>().setPosition(effectPosition);

				// Link both together
				mEffectParticleAir->getOrCreateComponentSafe<qsf::LinkComponent>().linkToParent(helicopterHatchEntity);

				// Use this moment to open the helicopter hatch
				AnimationHelper animationHelper(helicopterHatchEntity);
				animationHelper.playAnimation(animationHelper.getAnimationAirplaneHatchOpen());

			}

			// Sound for the effect
			AudioHelper::playFirePlaneEffectStart(mAudioProxyEffectStart, mEffectParticleAir.getSafe());
			mAudioProxyEffectStart.synchronizeWithMultiplay();
		}
	}

	void AreaExtinguishAction::createGroundParticleEffect()
	{
		// Effect is for the impact of the water on the ground. We place it over near to the ground
		qsf::Entity* effectParticle = MapHelper(getMap()).createObjectByLocalPrefabAssetId(assets::PREFAB_FIREFIGHTING_AIRPLANE_GROUNDEFFECT);
		if (nullptr != effectParticle)
		{
			// Place the effect over the ground
			qsf::MovableComponent& transformComponent = effectParticle->getOrCreateComponentSafe<qsf::MovableComponent>();
			const qsf::MovableComponent& airplaneTransformComponent = mAirplaneEntity->getOrCreateComponentSafe<qsf::MovableComponent>();

			const float POSITION_OFFSET = FireSpecsGroup::getInstanceSafe().getCommandoCarExtinguishOffsetPosition();
			const glm::vec3 positionOffset = airplaneTransformComponent.getRotation() * qsf::CoordinateSystem::getIn() * (POSITION_OFFSET * mEffectParticlesGround.size());
			glm::vec3 positionOnGround = mEffectStartPosition + positionOffset;

			// Find the position on the ground
			float groundHeight = 0.0f;
			qsf::GroundMapQuery(getMap(), GroundMaps::FILTER_MASK_ALL).getHeightAt(positionOnGround, groundHeight);
			positionOnGround.y = groundHeight + 0.5f;

			transformComponent.warpToPositionAndRotation(positionOnGround, airplaneTransformComponent.getRotation());

			// Sound for the effect
			AudioHelper::playFirePlaneEffectStart(mAudioProxyEffectImpact, *effectParticle);
			mAudioProxyEffectImpact.synchronizeWithMultiplay();

			mEffectParticlesGround.push_back(effectParticle);
		}
	}

	void AreaExtinguishAction::updateAirplanePosition(qsf::Time elapsedTime)
	{
		qsf::MovableComponent& movableComponent = mAirplaneEntity->getOrCreateComponentSafe<qsf::MovableComponent>();
		const glm::vec3& currentPosition = movableComponent.getPosition();

		const float flySpeed = 60.0f * elapsedTime.getSeconds();
		const glm::vec3 difference = movableComponent.getRotation() * qsf::CoordinateSystem::getIn() * flySpeed;
		glm::vec3 newPosition = difference + currentPosition;

		// Lift the plane to the perfect fly height (adding 10m to ensure an better optical view and being higher than helicopter)
		float optimalFlyHeight = 0.0f;
		qsf::GroundMapQuery(getMap(), GroundMaps::FILTER_HELICOPTER_GROUND).getHeightAt(newPosition, optimalFlyHeight);
		optimalFlyHeight = optimalFlyHeight + RoadVehicleSpecsGroup::getInstanceSafe().getHelicopterFlyHeight() + 10;

		// Changed the fly height slowly, to avoid hard movements in the air
		const float flyHeightChangeSpeed = RoadVehicleSpecsGroup::getInstanceSafe().getHelicopterFlyHeightSpeed() * elapsedTime.getSeconds();
		if (newPosition.y < optimalFlyHeight)
		{
			newPosition.y += flyHeightChangeSpeed;
			if (newPosition.y > optimalFlyHeight)
			{
				newPosition.y = optimalFlyHeight;
			}
		}
		else if (newPosition.y > optimalFlyHeight)
		{
			newPosition.y -= flyHeightChangeSpeed;
			if (newPosition.y < optimalFlyHeight)
			{
				newPosition.y = optimalFlyHeight;
			}
		}

		// Move the airplane
		movableComponent.moveToPosition(newPosition);

		if (mRadarEntity.valid())
		{
			const float yaw = TurnToAction::calcYawRotationToTarget(mAirplaneEntity.getSafe(), mRadarEntity.getSafe(), false);
			TurnToAction::turnToTarget(mRadarEntity.getSafe(), elapsedTime.getSeconds(), yaw);
		}
	}

	void AreaExtinguishAction::updateGroundParticle(qsf::Time elapsedTime)
	{
		// Create ground particle in time interval
		const qsf::Time timeToNextEffect = qsf::Time::fromSeconds(FireSpecsGroup::getInstanceSafe().getCommandoCarExtinguishOffsetTime());
		if (mTimeLastGroundEffect > timeToNextEffect)
		{
			mTimeLastGroundEffect -= timeToNextEffect;
			createGroundParticleEffect();
		}

		// Update the time
		mTimeLastGroundEffect += elapsedTime;
	}

	void AreaExtinguishAction::deactivateEffectParticle()
	{
		deactivateEffectParticleAir();

		for (const qsf::WeakPtr<qsf::Entity>& effectEntity : mEffectParticlesGround)
		{
			if (effectEntity.valid())
			{
				qsf::ParticlesComponent* particlesComponent = effectEntity->getComponent<qsf::ParticlesComponent>();
				if (nullptr != particlesComponent)
				{
					particlesComponent->setEmitterEnabled(false);
				}
			}
		}

		// Delete the ground marker
		if (mGroundMarkerEntity.valid())
		{
			MapHelper(getMap()).destroyEntityById(mGroundMarkerEntity->getId());
		}
	}

	void AreaExtinguishAction::deactivateEffectParticleAir()
	{
		if (mEffectParticleAir.valid())
		{
			qsf::ParticlesComponent* particlesComponent = mEffectParticleAir->getComponent<qsf::ParticlesComponent>();
			if (nullptr != particlesComponent)
			{
				particlesComponent->setEmitterEnabled(false);
			}
		}
	}

	void AreaExtinguishAction::deleteParticleEffects()
	{
		MapHelper mapHelper(getMap());
		if (mEffectParticleAir.valid())
		{
			mapHelper.destroyEntityById(mEffectParticleAir->getId());
		}

		for (const qsf::WeakPtr<qsf::Entity>& effectEntity : mEffectParticlesGround)
		{
			if (effectEntity.valid())
			{
				mapHelper.destroyEntityById(effectEntity->getId());
			}
		}
		mEffectParticlesGround.clear();

		if (mGroundMarkerEntity.valid())
		{
			mapHelper.destroyEntityById(mGroundMarkerEntity->getId());
		}
	}

	void AreaExtinguishAction::closeHatch()
	{
		// Link to the hatch child of the airplane
		const qsf::LinkComponent& linkComponent = mAirplaneEntity->getOrCreateComponentSafe<qsf::LinkComponent>();
		const HelicopterHatchComponent* helicopterHatchComponent = linkComponent.getComponentFromEntityOrLinkedChild<HelicopterHatchComponent>();
		if (nullptr != helicopterHatchComponent)
		{
			qsf::Entity& helicopterHatchEntity = helicopterHatchComponent->getEntity();

			// Use this moment to open the helicopter hatch
			AnimationHelper animationHelper(helicopterHatchEntity);
			animationHelper.playAnimation(animationHelper.getAnimationAirplaneHatchOpen(), false, true);
		}
	}

	void AreaExtinguishAction::damagePersonsInArea(float timePassedInSeconds)
	{
		const float maxDamageInPercent = 0.51f;
		const float percentDamage = (maxDamageInPercent / mTotalDuration.getSeconds()) * timePassedInSeconds;

		std::vector<HealthComponent*> healthComponents;
		qsf::ComponentMapQuery(getMap()).getInstancesInCircle(mTargetPosition, mRadius, healthComponents);
		for (HealthComponent* healthComponent : healthComponents)
		{
			if (!healthComponent->isHealthy())
			{
				continue;
			}

			if (healthComponent->isInvincible())
			{
				continue;
			}

			// No damage if the person is not in the map boundaries
			EntityHelper targetHelper(healthComponent->getEntity());
			if (targetHelper.isInMapBoundaries() == false)
			{
				continue;
			}

			// No damage if the person is in a vehicle/building/ etc
			if (nullptr != targetHelper.getContainerEntity())
			{
				continue;
			}

			// Assume that not everybody has the same maximum health
			const float individualDamage = healthComponent->getMaximumHealthEnergy() * percentDamage;
			healthComponent->receiveDamage(individualDamage, damagetype::DamageType::OTHER, getEntity().getComponent<qsf::TransformComponent>());

			// Play hint only once per action
			if (mCanPlayPersonDamageHint)
			{
				mCanPlayPersonDamageHint = false;
				HintHelper::showSupervisorMessage("EM5_GE11_HINT08");
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
