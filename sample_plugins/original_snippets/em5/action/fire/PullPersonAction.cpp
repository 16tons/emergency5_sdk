// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/fire/PullPersonAction.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/component/objects/BuryComponent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/logic/local/firefighters/FiremanLogic.h"
#include "em5/specs/ActionRangeSpecsGroup.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/game/targetpoint/PullPersonTargetPointProvider.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/base/DistanceHelper.h>

#include <qsf_ai/navigation/NavigationComponent.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/move/MovableComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier PullPersonAction::ACTION_ID = "em5::PullPersonAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PullPersonAction::PullPersonAction() :
		Action(ACTION_ID),
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mPull(START),
		mForcePosition(false),
		mTargetState(ON_FLOOR),
		mPreventUpdateAnimations(false),
		mWaitedExtraFrameForApplyPlacement(false)
	{
		// Nothing here
	}

	PullPersonAction::~PullPersonAction()
	{
		// Nothing here
	}

	void PullPersonAction::init(uint64 targetEntityId, Pull pull, bool forcePosition)
	{
		mTargetEntityId = targetEntityId;
		mPull = pull;
		mForcePosition = forcePosition;

		if (mPull == START)
		{
			mTargetState = ON_FLOOR;
		}
		else
		{
			mTargetState = GRABBED;

			// Stop command does not have the pulled entity, but whatever got clicked (e.g. the street)
			qsf::setUninitialized(mTargetEntityId);
		}
	}

	uint64 PullPersonAction::getTargetEntityId() const
	{
		return mTargetEntityId;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void PullPersonAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetEntityId);
		serializer.serializeAs<uint8>(mPull);
		serializer.serializeAs<uint8>(mTargetState);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool PullPersonAction::onStartup()
	{
		if (mPull == STOP)
		{
			// We have to find it out what is our actual pull target
			FiremanLogic* firemanLogic = EntityHelper(getEntity()).getGameLogic<FiremanLogic>();
			QSF_CHECK(nullptr != firemanLogic, "The EM5 pull person action was unable to retrieve the fireman logic", return false);

			mTargetEntityId = firemanLogic->getPullingPersonId();
		}

		if (qsf::isUninitialized(mTargetEntityId))
			return false;

		// Get the caller and target entity
		const qsf::Entity& callerEntity = getEntity();
		qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
		if (nullptr == targetEntity)
			return true;

		if (mPull == START)
		{
			// Make the distance check
			const float maxDistance = ActionRangeSpecsGroup::getInstanceSafe().getPullPersonDistance();
			const float currentDistance = qsf::game::DistanceHelper::getDistance(*targetEntity, callerEntity);
			if (currentDistance > maxDistance)
				return false;

			// Object can pulled by only one
			if (!ReserveLogic::checkReservation(*targetEntity, getEntityId()))
				return false;

			// Check if the target is already linked to some parent
			qsf::LinkComponent* linkComponent = targetEntity->getComponent<qsf::LinkComponent>();
			if (nullptr != linkComponent && linkComponent->hasParent())
				return false;
		}

		// No automatic animations interfering with this action
		PersonComponent* personComponent = getEntity().getComponent<PersonComponent>();
		if (nullptr != personComponent)
		{
			mPreventUpdateAnimations = true;
			personComponent->preventUpdateAnimations(true);
		}

		// Try to get event ID component lock
		if (!EventIdComponent::tryLockForTeam(*targetEntity, getEntity()))
		{
			return false;
		}

		return true;
	}

	void PullPersonAction::onShutdown()
	{
		// Ensure we get unlinked even if the action fails otherwise (but don't remove the other unlink, that cares about correct animations more)
		if (mPull == STOP)
		{
			unlink();
		}

		// Back to automatic animations
		PersonComponent* personComponent = getEntity().getComponent<PersonComponent>();
		if (nullptr != personComponent && mPreventUpdateAnimations)
		{
			// Only reset prevent update animation if we set it on startup
			mPreventUpdateAnimations = false;
			personComponent->preventUpdateAnimations(false);
		}

		// Clear the reservation
		ReserveLogic::deleteOwnReservation(getMap(), mTargetEntityId, getEntityId());

		EventIdComponent::releaseTeamLock(mTargetEntityId, getEntity());
	}

	qsf::action::Result PullPersonAction::updateAction(const qsf::Clock&)
	{
		// Get the entity of the caller
		qsf::Entity& callerEntity = getEntity();

		// Get the target entity instance
		qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
		QSF_CHECK(nullptr != targetEntity, "The EM5 pull person action was unable to retrieve the target entity instance", return qsf::action::RESULT_DONE);

		// Update the link component of the target entity
		qsf::LinkComponent* linkComponent = targetEntity->getOrCreateComponent<qsf::LinkComponent>();
		QSF_CHECK(nullptr != linkComponent, "The EM5 pull person action was unable to retrieve a link component instance", QSF_REACT_THROW);

		// Get the game logic component
		const qsf::GameLogicComponent* gameLogicComponent = callerEntity.getComponent<qsf::GameLogicComponent>();
		QSF_CHECK(nullptr != gameLogicComponent, "The EM5 pull person action was unable to retrieve the game logic component", return qsf::action::RESULT_DONE);

		// Get the fireman logic
		FiremanLogic* firemanLogic = gameLogicComponent->getGameLogic<FiremanLogic>();
		QSF_CHECK(nullptr != firemanLogic, "The EM5 pull person action was unable to retrieve the fireman logic", return qsf::action::RESULT_DONE);

		if (mForcePosition)
		{
			// Enforce the correct position (teleporting)
			qsf::TransformComponent * entityTransformComponent = callerEntity.getComponent<qsf::TransformComponent>();
			qsf::TransformComponent * targetTransformComponent = targetEntity->getComponent<qsf::TransformComponent>();
			if (entityTransformComponent != nullptr && targetTransformComponent != nullptr)
			{
				const glm::vec3 offsetWorld = entityTransformComponent->getTransform().vec3DirectionLocalToWorld(PullPersonTargetPointProvider::getOffset());
				targetTransformComponent->setPositionAndRotation(entityTransformComponent->getPosition() - offsetWorld, entityTransformComponent->getRotation());
			}
		}

		if (mPull == START)
		{
			if (mTargetState == ON_FLOOR)
			{
				mTargetState = GRABBED;

				ReserveLogic::createReservation<ReservePullingLogic>(*targetEntity, getEntityId());

				// Show unit picking it up
				AnimationHelper animationHelper(getEntity());
				getComponent().pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(animationHelper.getAnimationFiremanPullStart(), true);

				// Show target being picked up
				AnimationHelper animationHelperTarget(*targetEntity);
				qsf::ActionComponent& actionComponentTarget = targetEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
				actionComponentTarget.pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(animationHelperTarget.getAnimationInjuredPulledLoop(), false); // Playing the pulled here already avoid a jump in the animation when changing
				actionComponentTarget.pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(animationHelperTarget.getAnimationInjuredPulledStart(), true);

				// Hide current equipment
				EntityHelper(callerEntity).storeActiveEquipmentInInventory();

				// Enable instant rotation for firefighter while pulling a person
				qsf::ai::NavigationComponent* navigationComponent = callerEntity.getComponent<qsf::ai::NavigationComponent>();
				if (nullptr != navigationComponent)
				{
					navigationComponent->setLimitDirectionChange(false);
				}

				return qsf::action::RESULT_CONTINUE;
			}
			else
			{
				// Remove movable component of the target entity
				targetEntity->destroyComponent<qsf::MovableComponent>();

				// Link the object
				linkComponent->setParentId(getEntityId());
				linkComponent->setParentLinkType(qsf::LinkComponent::SOFT_LINK);

				// Set the pull state
				firemanLogic->setPullingPersonId(targetEntity->getId());

				EntityHelper::setupEntityAsGhost(*targetEntity, getEntity());
			}
		}
		else
		{
			if (mWaitedExtraFrameForApplyPlacement)
			{
				// Apply placement to have an valid position on the ground
				EntityHelper(*targetEntity).applyPlacement();

				return qsf::action::RESULT_DONE;
			}

			if (mTargetState == GRABBED)
			{
				mTargetState = ON_FLOOR;

				// Show unit dropping target
				AnimationHelper animationHelper(getEntity());
				getComponent().pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(animationHelper.getAnimationFiremanPullEnd(), true);

				// Show target being dropped
				AnimationHelper animationHelperTarget(*targetEntity);
				qsf::ActionComponent& actionComponentTarget = targetEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
				actionComponentTarget.pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(animationHelperTarget.getAnimationInjuredPulledEnd(), true);

				firemanLogic->setPullingPersonId(qsf::getUninitialized<uint64>());

				// Disable instant rotation again
				qsf::ai::NavigationComponent* navigationComponent = callerEntity.getComponent<qsf::ai::NavigationComponent>();
				if (nullptr != navigationComponent)
				{
					navigationComponent->setLimitDirectionChange(true);
				}

				return qsf::action::RESULT_CONTINUE;
			}
			else
			{
				unlink();
				EntityHelper::teardownEntityAsGhost(*targetEntity);

				// Reset animations
				AnimationHelper(getEntity()).clearAnimation();

				// Clear the reservation
				ReserveLogic::deleteOwnReservation(*targetEntity, getEntityId());

				// Show current equipment
				EntityHelper(callerEntity).storeInventoryEquipmentAsActive();

				mWaitedExtraFrameForApplyPlacement = true;
				return qsf::action::RESULT_CONTINUE;
			}
		}
		return qsf::action::RESULT_DONE;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void PullPersonAction::unlink()
	{
		qsf::Entity* entity = getMap().getEntityById(mTargetEntityId);
		if (nullptr != entity)
		{
			qsf::LinkComponent* linkComponent = entity->getComponent<qsf::LinkComponent>();
			if (nullptr != linkComponent)
			{
				linkComponent->unlinkFromParent();
			}

			// Destroy possible buried connects
			entity->destroyComponent<BuryComponent>();
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
