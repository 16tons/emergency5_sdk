// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/base/StopGuidePersonAction.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/component/movement/MoveToDespawnComponent.h"
#include "em5/logic/local/base/CarryPersonLogic.h"
#include "em5/logic/local/gangsters/GangsterBaseLogic.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/map/EntityHelper.h"

#include <qsf_ai/navigation/NavigationComponent.h>
#include <qsf_ai/navigation/em4Router/RouterComponent.h>

#include <qsf/map/Map.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/component/move/MovableComponent.h>
#include <qsf/physics/collision/BulletBoxCollisionComponent.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier StopGuidePersonAction::ACTION_ID = "em5::StopGuidePersonAction";


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	void StopGuidePersonAction::restorePolicemanAndGangster(qsf::Entity& policeman, qsf::Entity& gangster)
	{
		// Update the collision information of the caller and target
		qsf::BulletBoxCollisionComponent* bulletBoxCollisionComponent = policeman.getComponent<qsf::BulletBoxCollisionComponent>();
		if (nullptr != bulletBoxCollisionComponent)
		{
			glm::vec3 currentCollisionDimensions = bulletBoxCollisionComponent->getHalfExtents();
			bulletBoxCollisionComponent->setHalfExtents(currentCollisionDimensions * glm::vec3(0.5f, 1.0f, 0.5f));

			// We need to update the router component as well
			qsf::ai::RouterComponent* routerComponent = policeman.getComponent<qsf::ai::RouterComponent>();
			if (nullptr != routerComponent)
			{
				routerComponent->recreateCollisionBox();
			}

			qsf::BulletBoxCollisionComponent* targetbulletBoxCollisionComponent = gangster.getComponent<qsf::BulletBoxCollisionComponent>();
			if (nullptr != targetbulletBoxCollisionComponent)
			{
				// Enable collision
				targetbulletBoxCollisionComponent->setActive(true);
			}
		}
	}

	bool StopGuidePersonAction::freeGangster(qsf::Entity& policeman, qsf::Entity& gangster)
	{
		{ // Update the link component of the target entity
			qsf::LinkComponent& linkComponent = gangster.getOrCreateComponentSafe<qsf::LinkComponent>();
			linkComponent.unlinkFromParent();
			EntityHelper::teardownEntityAsGhost(gangster);
		}

		PersonComponent* personComponent = gangster.getComponent<PersonComponent>();
		if (nullptr != personComponent)
		{
			personComponent->setPersonFlags(PersonComponent::FLAG_HANDCUFFED, false);
			EntityHelper(gangster).deleteActiveEquipment();
		}

		// Set the gangster's state to not-ARRESTED
		qsf::GameLogicComponent* gameLogicComponentTarget = gangster.getComponent<qsf::GameLogicComponent>();
		if (nullptr != gameLogicComponentTarget)
		{
			GangsterBaseLogic* gangsterLogic = gameLogicComponentTarget->getGameLogic<GangsterBaseLogic>();
			if (nullptr != gangsterLogic)
			{
				gangsterLogic->setCurrentState(GangsterBaseLogic::STATE_NORMAL);

				if (nullptr != personComponent)
				{
					personComponent->setGangsterFlag(true);
				}
			}

			// Make sure the gangster doesn't follow some default paths
			MoveToDespawnComponent* moveToDespawnComponent = gangster.getComponent<MoveToDespawnComponent>();
			if (moveToDespawnComponent != nullptr)
			{
				moveToDespawnComponent->setActive(false);
				moveToDespawnComponent->setActive(true);
			}
		}

		// Can walk on its own again
		qsf::ai::NavigationComponent* navigationComponent = gangster.getComponent<qsf::ai::NavigationComponent>();
		if (navigationComponent != nullptr)
		{
			navigationComponent->setActive(true);
		}

		qsf::MovableComponent* movableComponent = gangster.getComponent<qsf::MovableComponent>();
		if (movableComponent != nullptr)
		{
			movableComponent->setActive(true);
		}

		// Clear animation
		AnimationHelper(gangster).clearAnimation();

		// Update the collision information of the caller and target
		restorePolicemanAndGangster(policeman, gangster);

		// Show current equipment
		EntityHelper(policeman).storeInventoryEquipmentAsActive();

		return true;
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	StopGuidePersonAction::StopGuidePersonAction() :
		Action(ACTION_ID),
		mTargetEntityId(qsf::getUninitialized<uint64>())
	{
		// Nothing here
	}

	StopGuidePersonAction::~StopGuidePersonAction()
	{
		// Nothing to do in here
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void StopGuidePersonAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetEntityId);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool StopGuidePersonAction::onStartup()
	{
		// Get the caller and target entity
		const qsf::Entity& callerEntity = getEntity();

		// Get the game logic component
		const qsf::GameLogicComponent* gameLogicComponent = callerEntity.getComponent<qsf::GameLogicComponent>();
		if (nullptr == gameLogicComponent)
			return false;

		// Get the carry person logic
		const CarryPersonLogic* carryPersonLogic = gameLogicComponent->getGameLogic<CarryPersonLogic>();
		if (nullptr == carryPersonLogic)
			return false;

		// Check if the person is carry someone
		return qsf::isInitialized(carryPersonLogic->getCarryPerson());
	}

	qsf::action::Result StopGuidePersonAction::updateAction(const qsf::Clock&)
	{
		// Get the target entity instance (we checked the components before)
		const qsf::GameLogicComponent& gameLogicComponent = getEntity().getComponentSafe<qsf::GameLogicComponent>();
		CarryPersonLogic* carryPersonLogic = gameLogicComponent.getGameLogic<CarryPersonLogic>();
		if (nullptr == carryPersonLogic)
			return qsf::action::RESULT_DONE;

		qsf::Entity* targetEntity = getEntity().getMap().getEntityById(carryPersonLogic->getCarryPerson());
		QSF_CHECK(nullptr != targetEntity, "The EM5 stop guide person action was unable to retrieve the target entity instance",
			return qsf::action::RESULT_DONE);

		carryPersonLogic->setCarryPerson(qsf::getUninitialized<uint64>());

		freeGangster(getEntity(), *targetEntity);

		return qsf::action::RESULT_DONE;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
