// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/objects/EboxComponent.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/audio/AudioHelper.h"
#include "em5/audio/AudioProxy.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/component/person/GaperComponent.h"
#include "em5/fire/component/FireComponent.h"
#include "em5/game/Game.h"
#include "em5/game/targetpoint/GapeFromDistanceTargetPointProvider.h"
#include "em5/game/targetpoint/OnTouchTargetPointProvider.h"
#include "em5/health/HealthHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/EM5Helper.h"

#include <qsf_game/base/DistanceHelper.h>

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/math/Random.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/plugin/QsfJobs.h>
#include <qsf/renderer/mesh/MeshComponent.h>


//[-------------------------------------------------------]
//[ Anonymous namespace                                   ]
//[-------------------------------------------------------]
namespace
{
	const float UPDATE_FREQUENCY = 1.0f;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 EboxComponent::COMPONENT_ID = qsf::StringHash("em5::EboxComponent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	void EboxComponent::setEboxDefective()
	{
		setEboxDefectiveWithGaping(0.0f, 0.0f);
	}

	void EboxComponent::setEboxDefectiveWithGaping(float gapeChance, float reactionRadius)
	{
		mGapeChance = gapeChance;
		mReactionRadius = reactionRadius;

		// Set defect flag
		mIsDefect = true;
		setMeshToDefect(true);

		// Turn out fire component so the ebox won't catch fire as long as it's defective
		activateFireComponents(false);

		// Register status overlay icon
		StatusOverlayComponent::showIcon(getEntity(), StatusOverlayComponent::STATUS_ICON_DEFECT, true);

		// Start simulation
		registerUpdateProxy();
	}

	void EboxComponent::setEboxIntact()
	{
		// Set defect flag
		mIsDefect = false;
		setMeshToDefect(false);

		// Turn on fire component again
		activateFireComponents(true);

		// Unregister status overlay icon
		StatusOverlayComponent::hideIcon(getEntity(), StatusOverlayComponent::STATUS_ICON_DEFECT);

		// Remove gaper components
		for (uint64 entityId : mAffectedPersons)
		{
			qsf::Entity* entity = getEntity().getMap().getEntityById(entityId);
			if (nullptr != entity)
			{
				entity->destroyComponent<GaperComponent>();
			}
		}

		// Clear the affected entities
		mSnoopingPersons.clear();
		mAffectedPersons.clear();

		// Deactivate the simulation
		mUpdateJobProxy.unregister();
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::DamageComponent methods           ]
	//[-------------------------------------------------------]
	void EboxComponent::cleanup()
	{
		// Set mesh to intact
		setMeshToDefect(false);

		// Call base class method
		ObjectDamageComponent::cleanup();
	}


	//[-------------------------------------------------------]
	//[ Protected virtual em5::DamageComponent methods        ]
	//[-------------------------------------------------------]
	void EboxComponent::onSetDamageState(DamageState damageState)
	{
		// Call the base implementation
		DamageComponent::onSetDamageState(damageState);

		// TODO(tl): No overlayicon for damaged eboxes
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	bool EboxComponent::onStartup()
	{
		// Save the intact ebox mesh
		mIntactMesh = getPrototype().getComponentSafe<qsf::MeshComponent>().getMesh();

		// Done
		return true;
	}

	void EboxComponent::onShutdown()
	{
		if (isActive())
		{
			mUpdateJobProxy.unregister();
		}
	}

	void EboxComponent::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mFlashParticlesIds);
		serializer.serialize(mSparksParticlesIds);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void EboxComponent::updateJob(const qsf::JobArguments& jobArguments)
	{
		// Sparks effect from time to time
		updateSparks(jobArguments.getTimePassed());

		// Catch gapers and punish them for being too curious
		updateGapersAndShocks(jobArguments.getTimePassed());
	}

	void EboxComponent::updateSparks(const qsf::Time& timePassed)
	{
		// Is it time for next sparks effect?
		if (mSecondsToNextSparks < qsf::Time::ZERO)
		{
			mSecondsToNextSparks = qsf::Time::fromSeconds(qsf::Random::getRandomFloat(mSparksWaitTime.x, mSparksWaitTime.y));

			// Start particles
			showSparksParticles();

			// Play audio
			AudioProxy audioProxy;
			AudioHelper::playControlBoxSparks(audioProxy, getEntity());
			audioProxy.detach();
		}
		else
		{
			mSecondsToNextSparks -= timePassed;
		}
	}

	void EboxComponent::updateGapersAndShocks(const qsf::Time& timePassed)
	{
		if (mGapeChance <= 0.0f || mReactionRadius <= 0.0f)
		{
			// Early out: Nothing to do
			return;
		}

		// Search for persons in range
		std::vector<PersonComponent*> personComponents;
		qsf::ComponentMapQuery(getEntity().getMap()).getInstancesInCircle(EntityHelper(getEntity()).getPosition(), mReactionRadius, personComponents);
		for (PersonComponent* personComponent : personComponents)
		{
			qsf::Entity& personEntity = personComponent->getEntity();

			// No effect for squad persons
			if (EntityHelper(personEntity).isSquadPerson())
				continue;

			// No effect for already injured persons
			if (EntityHelper(personEntity).isPersonInjured())
				continue;

			// Gangsters must be excluded as well
			if (personComponent->getGangsterFlag())
				continue;

			// Check if the person is already in the affected persons list
			auto iterator = std::find(mAffectedPersons.begin(), mAffectedPersons.end(), personEntity.getId());
			if (iterator != mAffectedPersons.end())
				continue;

			// Else add them to the list
			mAffectedPersons.push_back(personEntity.getId());

			if (qsf::Random::getRandomChance(mGapeChance))
			{
				// Add snooping persons to snooping person list
				mSnoopingPersons.push_back(personEntity.getId());
				makePersonSnooper(personEntity);
			}
			else
			{
				// This switch case is to get the right target type
				switch (qsf::Random::getRandomUint(0, 1))
				{
					// Gape from distance
					case 0:
					{
						makePersonGaper(personEntity);
						break;
					}

					// Go Away
					case 1:
					{
						// TODO(db): The person should pass the ebox but come not to close
					}
				}
			}
		}

		// Update timer for particle effect
		// - If new shock particle effect is starting and person is close enough check if it's on the snooping person list
		// - Injure the person
		bool shock = false;

		// Is it time for next flash effect?
		if (mSecondsToNextFlash < qsf::Time::ZERO)
		{
			mSecondsToNextFlash = qsf::Time::fromSeconds(qsf::Random::getRandomFloat(mFlashWaitTime.x, mFlashWaitTime.y));

			for (uint64 entityId : mSnoopingPersons)
			{
				qsf::Entity* personEntity = getEntity().getMap().getEntityById(entityId);
				if (nullptr != personEntity && !EntityHelper(*personEntity).isPersonInjured())
				{
					shock = true;
					break;
				}
			}
		}
		else
		{
			mSecondsToNextFlash -= timePassed;
		}

		// Injure persons that are too close
		if (shock == true)
		{
			// Injure only the persons which are gaping from close distance
			for (uint64 entityId : mSnoopingPersons)
			{
				qsf::Entity* personEntity = getEntity().getMap().getEntityById(entityId);
				if (nullptr != personEntity)
				{
					qsf::TransformComponent* personTransform = personEntity->getComponent<qsf::TransformComponent>();
					qsf::TransformComponent* eboxTransform = getEntity().getComponent<qsf::TransformComponent>();

					// If persons are close enough
					if (qsf::game::DistanceHelper::get2dDistance(personTransform->getPosition(), eboxTransform->getPosition()) < 4.0f)
					{
						// Injure a limited amount of persons
						if (mMaxInjuredPersons > mCurrentInjuredPersons)
						{
							injurePerson(*personEntity);
							++mCurrentInjuredPersons;
						}
					}
				}
			}

			// Start particles
			showFlashParticles();

			// Play audio
			AudioProxy audioProxy;
			AudioHelper::playControlBoxFlash(audioProxy, getEntity());
			audioProxy.detach();
		}
	}

	void EboxComponent::setMeshToDefect(bool damage)
	{
		// Update the mesh
		qsf::MeshComponent* meshComponent = getPrototype().getComponent<qsf::MeshComponent>();
		if (nullptr != meshComponent)
		{
			qsf::AssetProxy& meshAsset = (damage && nullptr != mDamageMesh.getCachedAsset()) ? mDamageMesh : mIntactMesh;
			meshComponent->setMesh(meshAsset);
		}
	}

	void EboxComponent::activateFireComponents(bool active)
	{
		std::vector<FireComponent*> fireComponents;
		EntityHelper(getEntity()).getComponentsFromChildren(fireComponents);
		for (FireComponent* fireComponent : fireComponents)
		{
			fireComponent->setActive(active);
		}
	}

	void EboxComponent::injurePerson(qsf::Entity& entity) const
	{
		// First destroy the gaper component
		entity.destroyComponent<GaperComponent>();

		HealthComponent* healthComponent = entity.getComponent<HealthComponent>();
		if (nullptr != healthComponent)
		{
			// Get the freeplay event
			FreeplayEvent* freeplayEvent = EntityHelper(getEntity()).getFreeplayEvent();

			// No null pointer check for freeplay event, if the event is null it will not connected to a freeplay event
			HealthHelper(*healthComponent).injurePersonByElectricShock(freeplayEvent);
		}
	}

	void EboxComponent::showSparksParticles()
	{
		MapHelper(getEntity().getMap()).showEntityParticleIds(mSparksParticlesIds, true, false, false, 0, true);
	}

	void EboxComponent::showFlashParticles()
	{
		MapHelper(getEntity().getMap()).showEntityParticleIds(mFlashParticlesIds, true, false, false, 0, true);
	}

	void EboxComponent::registerUpdateProxy()
	{
		qsf::jobs::JobConfiguration jobConfig;
		jobConfig.mTimeBetweenCalls = qsf::Time::fromSeconds(UPDATE_FREQUENCY);
		mUpdateJobProxy.registerAt(qsf::QsfJobs::SIMULATION, boost::bind(&EboxComponent::updateJob, this, _1), jobConfig);
	}

	void EboxComponent::makePersonGaper(qsf::Entity& entity)
	{
		qsf::ActionComponent& actionComponent = entity.getOrCreateComponentSafe<qsf::ActionComponent>();
		actionComponent.clearPlan();

		// Move away
		actionComponent.pushAction<MoveAction>(action::AUTOMATISM_HIGH).init(new qsf::ai::ReachObjectGoal(entity, getEntity(), GapeFromDistanceTargetPointProvider::TARGET_POINT_ID));

		// Turn to
		actionComponent.pushAction<TurnToAction>(action::AUTOMATISM_HIGH).init(getEntityId());

		// Let the person gape
		entity.getOrCreateComponentSafe<GaperComponent>();
	}

	void EboxComponent::makePersonSnooper(qsf::Entity& entity)
	{
		qsf::ActionComponent& actionComponent = entity.getOrCreateComponentSafe<qsf::ActionComponent>();
		actionComponent.clearPlan();

		// Move closer
		actionComponent.pushAction<MoveAction>(action::AUTOMATISM_HIGH).init(new qsf::ai::ReachObjectGoal(entity, getEntity(), OnTouchTargetPointProvider::TARGET_POINT_ID));

		// Turn to
		actionComponent.pushAction<TurnToAction>(action::AUTOMATISM_HIGH).init(getEntityId());

		// Let the person gape
		entity.getOrCreateComponentSafe<GaperComponent>();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
