// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/fire/PickupDrowningPersonWithDiverAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/logic/local/firefighters/DiverLogic.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/specs/ActionRangeSpecsGroup.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/game/targetpoint/DiverPickupDrowningTargetPointProvider.h"
#include "em5/health/HealthHelper.h"

#include <qsf_game/base/DistanceHelper.h>

#include <qsf_ai/navigation/em4Router/RouterComponent.h>
#include <qsf_ai/navigation/NavigationComponent.h>

#include <qsf/map/Map.h>
#include <qsf/component/move/MovableComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/math/CoordinateSystem.h>
#include <qsf/math/Math.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier PickupDrowningPersonWithDiverAction::ACTION_ID = "em5::PickupDrowningPersonWithDiverAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PickupDrowningPersonWithDiverAction::PickupDrowningPersonWithDiverAction() :
		Action(ACTION_ID),
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mState(STATE_INIT)
	{
		// Nothing here
	}

	PickupDrowningPersonWithDiverAction::~PickupDrowningPersonWithDiverAction()
	{
		// Remove reservation again
		ReserveLogic::deleteOwnReservation(getMap(), mTargetEntityId, getEntityId());
	}

	void PickupDrowningPersonWithDiverAction::init(uint64 targetEntityId)
	{
		mTargetEntityId = targetEntityId;
	}

	uint64 PickupDrowningPersonWithDiverAction::getTargetEntityId() const
	{
		return mTargetEntityId;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void PickupDrowningPersonWithDiverAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetEntityId);
		serializer.serializeAs<uint8>(mState);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool PickupDrowningPersonWithDiverAction::onStartup()
	{
		// Get the caller and target entity
		qsf::Entity& callerEntity = getEntity();
		qsf::Entity* targetEntity = callerEntity.getMap().getEntityById(mTargetEntityId);
		if (nullptr == targetEntity)
			return false;

		// Get the game logic component
		const qsf::GameLogicComponent* gameLogicComponent = callerEntity.getComponent<qsf::GameLogicComponent>();
		if (nullptr == gameLogicComponent)
			return false;

		// Get the diver logic
		const DiverLogic* diverLogic= gameLogicComponent->getGameLogic<DiverLogic>();
		if (nullptr == diverLogic)
			return false;

		// Check if the person is already carrying someone
		if (qsf::isInitialized(diverLogic->getCarryPerson()))
			return false;

		// Make the distance check
		const float maxDistance = ActionRangeSpecsGroup::getInstanceSafe().getCarryPersonDistance();
		const float currentDistance = qsf::game::DistanceHelper::get2dDistance(*targetEntity, callerEntity);
		if (currentDistance > maxDistance)
			return false;

		// Reserve the entity
		ReserveLogic::createReservation<ReserveGuidePersonLogic>(*targetEntity, getEntityId());

		// Try to get event ID component lock
		if (!EventIdComponent::tryLockForTeam(*targetEntity, getEntity()))
		{
			return false;
		}

		// Make the diver invincible
		HealthHelper healthHelper(callerEntity);
		healthHelper.setInvincible(true);

		return true;
	}

	qsf::action::Result PickupDrowningPersonWithDiverAction::updateAction(const qsf::Clock&)
	{
		// Get the entity of the caller
		qsf::Entity& callerEntity = getEntity();

		// Get the target entity instance
		qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
		QSF_CHECK(nullptr != targetEntity, "The EM5 guide person action was unable to retrieve the target entity instance", return qsf::action::RESULT_DONE);

		switch (mState)
		{
			case STATE_INIT:
			{
				{ // Update the transform component of the target entity to ensure it has the right distance
					qsf::TransformComponent& targetTransformComponent = targetEntity->getOrCreateComponentSafe<qsf::TransformComponent>();
					const qsf::TransformComponent& callerTransformComponent = callerEntity.getOrCreateComponentSafe<qsf::TransformComponent>();

					glm::vec3 position = callerTransformComponent.getPosition();
					const glm::vec3 offsetLocal(DiverPickupDrowningTargetPointProvider::getOffset());
					const glm::vec3 offsetWorld = callerTransformComponent.getTransform().vec3DirectionLocalToWorld(offsetLocal);
					position -= offsetWorld;
					targetTransformComponent.setPositionAndRotation(position, callerTransformComponent.getRotation());
				}

				// Remove movable component of the target entity
				targetEntity->destroyComponent<qsf::MovableComponent>();

				{ // Update the link component of the target entity (picked up person is now a child of the diver)
					qsf::LinkComponent& linkComponent = targetEntity->getOrCreateComponentSafe<qsf::LinkComponent>();
					linkComponent.linkToParent(getEntity());
					linkComponent.setLocalPosition(DiverPickupDrowningTargetPointProvider::getOffset());
					linkComponent.setLocalRotation(qsf::Math::getRotationAroundAxis(qsf::CoordinateSystem::getUp(), glm::two_over_pi<float>()));
				}

				{ // Set the carried person to the carry person logic
					// Get the game logic component
					const qsf::GameLogicComponent* gameLogicComponent = callerEntity.getComponent<qsf::GameLogicComponent>();
					if (nullptr == gameLogicComponent)
						return qsf::action::RESULT_DONE;

					// Get the carry person logic
					DiverLogic* diverLogic = gameLogicComponent->getGameLogic<DiverLogic>();
					if (nullptr == diverLogic)
						return qsf::action::RESULT_DONE;

					// Set the carried person to the logic
					diverLogic->setCarryPerson(mTargetEntityId);
				}

				// Play animations for diver and drowning person
				getComponent().pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(AnimationHelper(getEntity()).getAnimationGrabDrowning(), true);
				qsf::ActionComponent& actionComponent = targetEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
				actionComponent.pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(AnimationHelper(*targetEntity).getAnimationDrowningGetRescuedStart(), true);

				mState = STATE_PICKUP;

				// Give it 1 frame to start the animation
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_PICKUP:
			{
				// Play animations for diver rescuing drowning person
				getComponent().pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(AnimationHelper(getEntity()).getAnimationRescueDrowningWater(), true);
				qsf::ActionComponent& actionComponent = targetEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
				actionComponent.pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(AnimationHelper(*targetEntity).getAnimationDrowningGetRescuedMove(), true);

				// Remove the collision, otherwise the diver and the target would stack together
				qsf::ai::RouterComponent* routerComponent = targetEntity->getComponent<qsf::ai::RouterComponent>();
				if (nullptr != routerComponent)
					routerComponent->removeCollisionBox();

				// With an active navigation component we are still using ai reservations from this entity, so we need to disable it
				qsf::ai::NavigationComponent* navigationComponent = targetEntity->getComponent<qsf::ai::NavigationComponent>();
				if (nullptr != navigationComponent)
				{
					navigationComponent->setActive(false);
				}

				// Immediately go to the next state; no break by intent
			}
		}

		return qsf::action::RESULT_DONE;
	}

	void PickupDrowningPersonWithDiverAction::onShutdown()
	{
		EventIdComponent::releaseTeamLock(mTargetEntityId, getEntity());
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
