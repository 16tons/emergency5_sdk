// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/vehicle/BoatComponent.h"
#include "em5/base/GameplayAssets.h"
#include "em5/component/overlay/StatusOverlayComponent.h"
#include "em5/component/vehicle/parts/WaterCannonComponent.h"
#include "em5/component/spawnpoint/SpawnPointComponent.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/component/effect/FadeEffectComponent.h"
#include "em5/fire/component/ComplexFireComponent.h"
#include "em5/game/units/UnitHelper.h"
#include "em5/plugin/GameCounters.h"
#include "em5/plugin/Messages.h"
#include "em5/audio/AudioHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"

#include <qsf_game/component/base/LightControllerComponent.h>

#include <qsf/component/link/LinkComponent.h>
#include <qsf/component/rotation/LinearRotationComponent.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/renderer/particles/ParticlesComponent.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/math/Random.h>
#include <qsf/math/EulerAngles.h>
#include <qsf/plugin/QsfJobs.h>
#include <qsf/map/Map.h>
#include <qsf/map/Entity.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 BoatComponent::COMPONENT_ID = qsf::StringHash("em5::BoatComponent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	BoatComponent::BoatComponent(qsf::Prototype* prototype) :
		VehicleComponent(prototype),
		mLakeId(qsf::getUninitialized<uint64>()),
		mRadarEntityId(qsf::getUninitialized<uint64>()),
		mLightEnabled(false),
		mPropellerParticleEnabled(false),
		mRadarEnabled(false),
		mSinkAnimationSecondsPassed(0.0f),
		mIsSirenSFXSet(false),
		VehicleLightsArray(*this, mVehicleLightIds),
		DrowningSpawnPointArray(*this, mDrowningSpawnPointIds),
		PropellerParticlesArray(*this, mPropellerParticleIds)
	{
		// Nothing to do in here
	}

	BoatComponent::~BoatComponent()
	{
		// Nothing to do in here
	}

	uint64 BoatComponent::getLakeId() const
	{
		return mLakeId;
	}

	void BoatComponent::setLakeId(uint64 lakeId)
	{
		mLakeId = lakeId;
	}

	void BoatComponent::enableLights(bool enable)
	{
		if (mLightEnabled != enable)
		{
			mLightEnabled = enable;

			const qsf::Map& map = getEntity().getMap();
			for (uint64 lightEntityId : mVehicleLightIds)
			{
				// Get the entity
				qsf::Entity* lightEntity = map.getEntityById(lightEntityId);
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
	}

	bool BoatComponent::isPropellerParticlesEnabled() const
	{
		return mPropellerParticleEnabled;
	}

	void BoatComponent::enablePropellerParticles(bool enable)
	{
		if (mPropellerParticleEnabled != enable)
		{
			mPropellerParticleEnabled = enable;

			const qsf::Map& map = getEntity().getMap();
			for (uint64 particleId : mPropellerParticleIds)
			{
				// Get the entity
				qsf::Entity* particleEntity = map.getEntityById(particleId);
				if (nullptr != particleEntity)
				{
					qsf::ParticlesComponent* particleComponent = particleEntity->getComponent<qsf::ParticlesComponent>();
					if (nullptr != particleComponent)
					{
						particleComponent->setEmitterEnabled(enable);
					}
				}
			}
		}
	}

	uint64 BoatComponent::getRadarEntityId() const
	{
		return mRadarEntityId;
	}

	void BoatComponent::setRadarEntityId(uint64 radarEntityId)
	{
		mRadarEntityId = radarEntityId;
	}

	bool BoatComponent::isRadarEnabled() const
	{
		return mRadarEnabled;
	}

	void BoatComponent::enableRadar(bool enable)
	{
		if (mRadarEnabled != enable)
		{
			mRadarEnabled = enable;

			const qsf::Entity* radarEntity = getEntity().getMap().getEntityById(mRadarEntityId);
			if (nullptr == radarEntity)
				return;

			qsf::LinearRotationComponent* linearRotationComponent = radarEntity->getComponent<qsf::LinearRotationComponent>();
			if (nullptr == linearRotationComponent)
				return;

			linearRotationComponent->setActive(enable);
		}
	}

	WaterCannonComponent* BoatComponent::getWaterCannonComponent() const
	{
		// Ask all linked childs for component
		qsf::LinkComponent* linkComponent = getEntity().getComponent<qsf::LinkComponent>();
		return (nullptr != linkComponent) ? linkComponent->getComponentFromEntityOrLinkedChild<WaterCannonComponent>() : nullptr;
	}

	const BoatComponent::EntityIdArray& BoatComponent::getSpawnPointIds() const
	{
		return mDrowningSpawnPointIds;
	}

	SpawnPointComponent* BoatComponent::getRandomSpawnPointComponent() const
	{
		if (mDrowningSpawnPointIds.empty())
			return nullptr;

		// Get a random ID from array
		const uint64 spawnPointId = qsf::Random::getAnyOf(mDrowningSpawnPointIds);

		// Get entity
		qsf::Entity* spawnPointEntity = getEntity().getMap().getEntityById(spawnPointId);
		if (nullptr == spawnPointEntity)
			return nullptr;

		return spawnPointEntity->getComponent<SpawnPointComponent>();
	}

	void BoatComponent::onSetDestroyed()
	{
		if (isSimulating())
		{
			// Turn off fire
			setFireStateDestroyed();

			// Sink animation
			mSinkAnimationSecondsPassed = 0.0f;
			mSinkAnimationJobProxy.registerAt(qsf::QsfJobs::ANIMATION_GENERAL, boost::bind(&BoatComponent::updateSinkAnimation, this, _1));

			// Make it immovable
			EntityHelper(getEntity()).makeImmovable();

			// Send message
			QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_BOAT_DESTROYED, getEntityId()));
		}

		setDamageOverlaysActive(true, false);
		updateTintableMesh();
	}

	void BoatComponent::onFireStarted(FreeplayEvent* freeplayEvent)
	{
		personsFleeFromFire(false);

		// Call base implementation
		VehicleComponent::onFireStarted(freeplayEvent);

		// Make it immovable
		EntityHelper(getEntity()).makeImmovable();
	}

	void BoatComponent::onFireStopped()
	{
		// Commandable Boats are always destroyed after fire, normal boats only after they are destroyed by fire
		bool isBurned = false;
		ComplexFireComponent* complexFireComponent = getEntity().getComponent<ComplexFireComponent>();
		if (nullptr != complexFireComponent)
		{
			isBurned = complexFireComponent->isBurned();
		}

		if (isBurned || EntityHelper(getEntity()).isCommandable())
		{
			setDamageState(DamageComponent::DAMAGESTATE_DESTROYED);
		}

		// Make sure boats can't be towed
		//  -> Move this e.g. into startup if there is ever need of a boat to be towed
		setCanReallyNotBeTowed(true);

		// Turn off the status overlays (usually done in vehicleComponent::onFireStopped(), but we don't call the base class here)
		StatusOverlayComponent::hideBarOverlay(getEntity(), StatusOverlayComponent::BAR_TYPE_SMALL_HEALTH);
		StatusOverlayComponent::hideBarOverlay(getEntity(), StatusOverlayComponent::BAR_TYPE_FIRE);
		GameCounters::decrementGameCounter(GameCounters::VEHICLES_BURNING);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	void BoatComponent::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base class implementation
		em5::VehicleComponent::serialize(serializer);
	}

	bool BoatComponent::onStartup()
	{
		mPropellerParticleEnabled = true;	// To force update
		enablePropellerParticles(false);

		return true;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void BoatComponent::updateSinkAnimation(const qsf::JobArguments& arguments)
	{
		mSinkAnimationSecondsPassed += arguments.getSecondsPassed();

		float sinkSpeed = 0.8f * arguments.getSecondsPassed() * (mSinkAnimationSecondsPassed - 2.0f);
		float rotateSpeed = 0.2f * arguments.getSecondsPassed() * mSinkAnimationSecondsPassed;

		qsf::TransformComponent& transformComponent = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();

		if (mSinkAnimationSecondsPassed < 2.0f)
		{
			transformComponent.setRotation(transformComponent.getRotation() * qsf::EulerAngles::eulerToQuaternion(0.0f, 0.0f, rotateSpeed));
		}

		if (mSinkAnimationSecondsPassed >= 2.0f && mSinkAnimationSecondsPassed < 5.0f)
		{
			if (!mAudioProxy.isPlaying())
			{
				MapHelper(getEntity().getMap()).spawnParticle(assets::PARTICLE_WATERSPRAY2, 4.0f, transformComponent.getPosition());
				AudioHelper::playDiveSpot(mAudioProxy, getEntity());
			}
			transformComponent.setPosition(transformComponent.getPosition() - glm::vec3(0.0f, sinkSpeed, 0.0f));
		}

		if (mSinkAnimationSecondsPassed >= 4.0f && mSinkAnimationSecondsPassed < 5.0f)
		{
			FadeEffectComponent& fadeEffect = getEntity().getOrCreateComponentSafe<FadeEffectComponent>();
			if (!fadeEffect.isFading())
			{
				fadeEffect.fadeOut(qsf::Time::fromSeconds(1.5f), true);
			}
		}

		if (mSinkAnimationSecondsPassed >= 5.0f)
		{
			mAudioProxy.stop();
			mSinkAnimationJobProxy.unregister();
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
