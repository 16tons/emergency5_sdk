// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/objects/DivingSpotComponent.h"
#include "em5/component/spawnpoint/SpawnPointComponent.h"
#include "em5/component/vehicle/VehicleComponent.h"
#include "em5/map/EntityHelper.h"
#include "em5/health/HealthHelper.h"

#include <qsf/serialization/binary/BasicTypeSerialization.h>
#include <qsf/map/Entity.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/renderer/particles/ParticlesComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 DivingSpotComponent::COMPONENT_ID = qsf::StringHash("em5::DivingSpotComponent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	DivingSpotComponent::DivingSpotComponent(qsf::Prototype* prototype) :
		qsf::Component(prototype),
		mIntact(true),
		mAlternativeTargetId(qsf::getUninitialized<uint64>()),
		mFreeplayEvent(nullptr)
	{
		// Nothing to do in here
	}

	DivingSpotComponent::~DivingSpotComponent()
	{
		// Nothing to do in here
	}

	bool DivingSpotComponent::isIntact() const
	{
		return mIntact;
	}

	void DivingSpotComponent::setIntact(bool intact, FreeplayEvent* freeplayEvent)
	{
		mIntact = intact;
		mFreeplayEvent = freeplayEvent;
		enableParticleEffects(intact);
	}

	FreeplayEvent* DivingSpotComponent::getFreeplayEvent() const
	{
		return mFreeplayEvent;
	}

	qsf::Entity* DivingSpotComponent::spawnRandomEntityFromPool() const
	{
		// For spawning an entity its necessary that the owner has an valid spawn point. Handle spawn logic completely via the spawn point component
		qsf::Entity* spawnedEntity = nullptr;
		SpawnPointComponent* spawnPointComponent = getEntity().getComponent<SpawnPointComponent>();
		if (nullptr != spawnPointComponent)
		{
			spawnedEntity = spawnPointComponent->spawnRandom(false);
			if (nullptr != spawnedEntity)
			{
				EntityHelper entityHelper(*spawnedEntity);
				entityHelper.makeImmovable();

				if (entityHelper.isCivilPerson())
				{
					HealthHelper(*spawnedEntity).drownPerson(mFreeplayEvent);
				}

				if (entityHelper.isVehicle())
				{
					// Destroy vehicle
					VehicleComponent& vehicleComponent = spawnedEntity->getOrCreateComponentSafe<VehicleComponent>();
					vehicleComponent.setDamageState(DamageComponent::DamageState::DAMAGESTATE_DESTROYED);
				}

				// Moving the new spawned entity in the owner layer is essential for the cleanup mechanic. Otherwise the vehicle would stay in the map after main event restart.
				EntityHelper(getEntity()).moveEntityInOwnerLayer(*spawnedEntity);
			}
		}

		// Return entity, can be a null pointer
		return spawnedEntity;
	}

	uint64 DivingSpotComponent::getAlternativeTargetId() const
	{
		return mAlternativeTargetId;
	}

	void DivingSpotComponent::setAlternativeTargetId(uint64 alternativeTargetId)
	{
		mAlternativeTargetId = alternativeTargetId;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	void DivingSpotComponent::onSetSimulating(bool simulating)
	{
		if (simulating)
		{
			// Make sure the associated spawn point component is not active for automatic spawning
			SpawnPointComponent* spawnPointComponent = getEntity().getComponent<SpawnPointComponent>();
			if (nullptr != spawnPointComponent)
			{
				spawnPointComponent->setActive(false);
			}
		}
	}

	void DivingSpotComponent::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mIntact);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void DivingSpotComponent::enableParticleEffects(bool enable)
	{
		// Enable/disable the particle effect(s)
		const qsf::LinkComponent* linkComponent = getEntity().getComponent<qsf::LinkComponent>();
		if (nullptr != linkComponent)
		{
			for (const qsf::LinkComponent* childLinkComponent : linkComponent->getChildLinks())
			{
				qsf::ParticlesComponent* particlesComponent = childLinkComponent->getEntity().getComponent<qsf::ParticlesComponent>();
				if (particlesComponent != nullptr)
				{
					particlesComponent->setActive(enable);
				}
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
