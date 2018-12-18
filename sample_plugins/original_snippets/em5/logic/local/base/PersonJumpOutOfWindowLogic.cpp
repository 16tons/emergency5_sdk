// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/logic/local/base/PersonJumpOutOfWindowLogic.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/component/person/PersonPlacementComponent.h"
#include "em5/freeplay/objective/ObjectiveHelper.h"
#include "em5/health/HealthComponent.h"
#include "em5/health/injury/Injuries.h"
#include "em5/map/MapHelper.h"
#include "em5/plugin/Jobs.h"
#include "em5/plugin/Messages.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/component/base/HiddenComponent.h>

#include <qsf/component/move/MovableComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/map/query/GroundMapQuery.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/math/Random.h>
#include <qsf/job/JobArguments.h>
#include <qsf/physics/PhysicsWorldComponent.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Local definitions                                     ]
	//[-------------------------------------------------------]
	const float HEALTH_DAMAGE_PER_SEC = 0.0025f;		// 0.25% of total health, so the person jumps after 400 sec


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 PersonJumpOutOfWindowLogic::GAMELOGIC_TYPE_ID = qsf::StringHash("em5::PersonJumpOutOfWindowLogic");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PersonJumpOutOfWindowLogic::PersonJumpOutOfWindowLogic() :
		qsf::GameLogic(GAMELOGIC_TYPE_ID),
		mJump(0.0f, qsf::Random::getRandomFloat(4.5f, 6.5f), -qsf::Random::getRandomFloat(1.0f, 2.0f)), // Hardcoded values
		mJumpWaitTime(qsf::Time::fromMilliseconds(500))
	{
		// Nothing here
	}

	void PersonJumpOutOfWindowLogic::updateSimulationJump(const qsf::JobArguments& jobArguments)
	{
		if (!MapHelper::isUnitAutomatismActive())
		{
			// No automatism active
			return;
		}

		if (mJumpWaitTime > qsf::Time::ZERO)
		{
			mJumpWaitTime -= jobArguments.getTimePassed();
			return;
		}

		qsf::Entity* personEntity = getEntity();
		QSF_CHECK(nullptr != personEntity, "There is no entity for this logic", return);

		qsf::MovableComponent& movableComponent = personEntity->getComponentSafe<qsf::MovableComponent>();
		const qsf::PhysicsWorldComponent& physicsWorldComponent = personEntity->getMap().getCoreEntity().getComponentSafe<qsf::PhysicsWorldComponent>();

		const float secondsPassed = jobArguments.getSecondsPassed();

		mJump += (physicsWorldComponent.getGravity() * secondsPassed);

		glm::vec3 newPosition = movableComponent.getPosition() + ((movableComponent.getRotation() * mJump) * secondsPassed);

		float currentMapHeight = 0.0f;
		qsf::GroundMapQuery(personEntity->getMap()).getHeightAtXZPosition(newPosition.x, newPosition.z, currentMapHeight);

		// Check if the person hits the ground
		if (newPosition.y < currentMapHeight)
		{
			newPosition.y = currentMapHeight;

			personHitsGround();

			// Emit message that the person is jumped
			QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_JUMPEDOUTOFWINDOW_PERSON, personEntity->getId()));

			mSimulationJumpJobProxy.unregister();
		}

		movableComponent.moveToPosition(newPosition);
	}

	void PersonJumpOutOfWindowLogic::updateSimulationBalcony(const qsf::JobArguments& jobArguments)
	{
		qsf::Entity* personEntity = getEntity();
		QSF_CHECK(nullptr != personEntity, "There is no entity for this logic", return);

		HealthComponent* healthComponent = getEntity()->getComponent<HealthComponent>();
		if (nullptr == healthComponent)
			return;

		if (healthComponent->getHealthEnergy() > 0)
		{
			// Damage the persons health
			float maxHealthEnergy = healthComponent->getMaximumHealthEnergy();
			float healthEnergy= healthComponent->getHealthEnergy();
			healthComponent->setHealthEnergy(healthEnergy - maxHealthEnergy * HEALTH_DAMAGE_PER_SEC);
		}
		else
		{
			// Start jump animation
			AnimationHelper personAnimationHelper(*personEntity);

			// Register job
			// -> For performance reasons, do updates not in every tick, but in fixed time intervals
			mSimulationJumpJobProxy.registerAt(Jobs::SIMULATION_LOGIC, boost::bind(&PersonJumpOutOfWindowLogic::updateSimulationJump, this, _1));
			mSimulationBalconyJobProxy.unregister();

			// Setup jump animation
			// Let that persons wave out of windows
			personAnimationHelper.playAnimation(personAnimationHelper.getAnimationJumpDown());
		}
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	bool PersonJumpOutOfWindowLogic::onStartup()
	{
		qsf::jobs::JobConfiguration jobConfiguration;
		jobConfiguration.mTimeBetweenCalls = qsf::Time::fromSeconds(1.0f);
		mSimulationBalconyJobProxy.registerAt(Jobs::SIMULATION_LOGIC, boost::bind(&PersonJumpOutOfWindowLogic::updateSimulationBalcony, this, _1), jobConfiguration);

		qsf::Entity* personEntity = getEntity();
		if (nullptr != personEntity)
		{
			// Remove person placement component
			personEntity->destroyComponent<PersonPlacementComponent>();
		}

		return true;
	}

	void PersonJumpOutOfWindowLogic::onShutdown()
	{
		mSimulationJumpJobProxy.unregister();
		mSimulationBalconyJobProxy.unregister();
		mMessageProxy.unregister();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void PersonJumpOutOfWindowLogic::personHitsGround()
	{
		// Get person
		qsf::Entity* personEntity = getEntity();
		if (nullptr == personEntity)
			return;

		// Get health component
		HealthComponent* healthComponent = getEntity()->getComponent<HealthComponent>();
		if (nullptr == healthComponent)
			return;

		// Get containing building
		qsf::Entity* buildingEntity = EntityHelper(*personEntity).getContainerEntity();
		if (nullptr == buildingEntity)
			return;

		BuildingComponent& buildingComponent = buildingEntity->getComponentSafe<BuildingComponent>();

		// Remove the person from the building
		buildingComponent.removeFromContainer(*personEntity);

		// Stop waving
		buildingComponent.stopPersonWaveOutOfWindow();

		// Unhide
		qsf::game::HiddenComponent::setHidden(*personEntity, false);

		// Delete link
		qsf::LinkComponent* personLinkComponent = personEntity->getComponent<qsf::LinkComponent>();
		if (nullptr != personLinkComponent)
		{
			personLinkComponent->setParentId(qsf::getUninitialized<uint64>());
		}

		// Get person the correct placement type again
		personEntity->getOrCreateComponent<PersonPlacementComponent>();


		// Get freeplay event
		FreeplayEvent* freeplayEvent = personEntity->getComponentSafe<EventIdComponent>().getFreeplayEvent();

		// Injure person
		static uint32 injuryPool[4] = { injury::BURN_3_GRADE, injury::BURN_4_GRADE, injury::BURN_OF_BREATH, injury::BROKEN_SKULL };
		const qsf::StringHash& randomInjury = injuryPool[qsf::Random::getRandomUint(0, 3)];
		healthComponent->injurePersonByEventById(randomInjury, freeplayEvent);

		// objectives are handled in PersonInWindowObserver
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
