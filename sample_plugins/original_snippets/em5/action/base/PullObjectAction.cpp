// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/base/PullObjectAction.h"
#include "em5/action/base/DisappearAction.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/logic/local/base/PullObjectLogic.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/specs/ActionRangeSpecsGroup.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/base/DistanceHelper.h>

#include <qsf/map/Map.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier PullObjectAction::ACTION_ID = "em5::PullObjectAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PullObjectAction::PullObjectAction() :
		Action(ACTION_ID),
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mPull(START),
		mTargetState(ON_FLOOR),
		mClearActionsOnFail(false)
	{
		// Nothing here
	}

	PullObjectAction::~PullObjectAction()
	{
		// Nothing here
	}

	void PullObjectAction::init(qsf::Entity& targetEntity, Pull pull, bool clearActionsOnFail)
	{
		mTargetEntityId = targetEntity.getId();
		mPull = pull;
		mTargetState = (mPull == START) ? ON_FLOOR : GRABBED;
		mClearActionsOnFail = clearActionsOnFail;

		// To avoid multiple actors at the same target, we reserve the target here.
		// It is only checked in command for automatism. If the action starts, the logic is changed. Only reserve if no other reserve the target (its not bad if a moveTo reservation is missing)
		// Reserve the entity
		if (ReserveLogic::checkReservation(targetEntity, getEntityId()))
		{
			ReserveLogic::createReservation<ReserveMoveToTargetLogic>(targetEntity, getEntityId());
		}
	}

	uint64 PullObjectAction::getTargetEntityId() const
	{
		return mTargetEntityId;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void PullObjectAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetEntityId);
		serializer.serializeAs<uint8>(mPull);
		serializer.serializeAs<uint8>(mTargetState);
		serializer.serialize(mClearActionsOnFail);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool PullObjectAction::onStartup()
	{
		// Get the caller and target entity
		const qsf::Entity& callerEntity = getEntity();
		qsf::Entity* targetEntity = callerEntity.getMap().getEntityById(mTargetEntityId);
		if (nullptr == targetEntity)
		{
			if (mClearActionsOnFail)
			{
				getComponent().clearPlan();
			}
			return false;
		}

		if (mPull == START)
		{
			// Make the distance check
			const float maxDistance = ActionRangeSpecsGroup::getInstanceSafe().getPullObjectDistance();
			const float currentDistance = qsf::game::DistanceHelper::getDistance(*targetEntity, callerEntity);
			if (mPull == START && currentDistance > maxDistance)
			{
				if (mClearActionsOnFail)
				{
					getComponent().clearPlan();
				}
				return false;
			}

			// Object can pulled by only one
			if (!ReserveLogic::checkReservation(*targetEntity, getEntityId()))
			{
				if (mClearActionsOnFail)
				{
					getComponent().clearPlan();
				}
				return false;
			}
		}

		// Get the target's game logic component
		qsf::GameLogicComponent* gameLogicComponent = callerEntity.getComponent<qsf::GameLogicComponent>();
		QSF_CHECK(nullptr != gameLogicComponent, "The EM5 pull object action was unable to retrieve the game logic component", return false);

		// Create the pull object logic
		const PullObjectLogic* pullObjectLogic = gameLogicComponent->getOrCreateGameLogic<PullObjectLogic>();
		QSF_CHECK(nullptr != pullObjectLogic, "The EM5 pull object action was unable to retrieve the object logic", return false);

		// Try to get event ID component lock
		if (!EventIdComponent::tryLockForTeam(*targetEntity, getEntity()))
		{
			return false;
		}

		return true;
	}

	qsf::action::Result PullObjectAction::updateAction(const qsf::Clock&)
	{
		// Get the entity of the caller
		qsf::Entity& callerEntity = getEntity();

		// Get the target entity instance
		qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
		QSF_CHECK(nullptr != targetEntity, "The EM5 pull object action was unable to retrieve the target entity instance", return qsf::action::RESULT_DONE);

		// Update the link component of the target entity
		qsf::LinkComponent& linkComponent = targetEntity->getOrCreateComponentSafe<qsf::LinkComponent>();

		// Get the target's game logic component
		const qsf::GameLogicComponent* gameLogicComponent = callerEntity.getComponent<qsf::GameLogicComponent>();
		QSF_CHECK(nullptr != gameLogicComponent, "The EM5 pull object action was unable to retrieve the game logic component", return qsf::action::RESULT_DONE);

		// Get the pull object logic
		PullObjectLogic* pullObjectLogic = gameLogicComponent->getGameLogic<PullObjectLogic>();
		QSF_CHECK(nullptr != pullObjectLogic, "The EM5 pull object action was unable to retrieve the object logic", return qsf::action::RESULT_DONE);

		if (mPull == START)
		{
			if (mTargetState == ON_FLOOR)
			{
				mTargetState = GRABBED;

				// Change Reserve logic from "moveto" to "ReservePullingLogic" (has higher priority)
				ReserveLogic::deleteOwnReservation(*targetEntity, getEntityId());
				ReserveLogic::createReservation<ReservePullingLogic>(*targetEntity, getEntityId());

				// Show unit picking it up
				AnimationHelper animationHelper(getEntity());
				getComponent().pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(animationHelper.getAnimationFiremanPullObjectStart(), true);

				// Hide current equipment
				EntityHelper(callerEntity).storeActiveEquipmentInInventory();

				return qsf::action::RESULT_CONTINUE;
			}
			else
			{
				// Link the object
				linkComponent.setParentId(getEntityId());
				linkComponent.setParentLinkType(qsf::LinkComponent::SOFT_LINK);
				pullObjectLogic->setPulledObject(targetEntity);
				EntityHelper::setupEntityAsGhost(*targetEntity, getEntity());
			}
		}
		else if (mPull == STOP)
		{
			if (mTargetState == GRABBED)
			{
				mTargetState = ON_FLOOR;

				// Show unit dropping it
				AnimationHelper animationHelper(getEntity());
				getComponent().pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(animationHelper.getAnimationFiremanPullObjectEnd(), true);

				return qsf::action::RESULT_CONTINUE;
			}
			else
			{
				qsf::Entity* entity = pullObjectLogic->getPulledObject();
				if (nullptr != entity)
				{
					// Unlink the object
					linkComponent.setParentId(qsf::getUninitialized<uint64>());
					linkComponent.setParentLinkType(qsf::LinkComponent::INVALID_LINK_TYPE);
					pullObjectLogic->setPulledObject(nullptr);

					EntityHelper::teardownEntityAsGhost(*targetEntity);

					// Make the object vanish
					qsf::ActionComponent& actionComponent = entity->getOrCreateComponentSafe<qsf::ActionComponent>();
					actionComponent.pushAction<DisappearAction>(action::BLOCKING).init(qsf::Time::fromSeconds(2.0f), true);

					// Reset animations
					AnimationHelper(getEntity()).clearAnimation();
				}

				// Clear the reservation
				ReserveLogic::deleteOwnReservation(*targetEntity, getEntityId());

				// Show current equipment
				EntityHelper(callerEntity).storeInventoryEquipmentAsActive();
			}
		}

		return qsf::action::RESULT_DONE;
	}

	void PullObjectAction::onShutdown()
	{
		// Clear the reservation
		ReserveLogic::deleteOwnReservation(getMap(), mTargetEntityId, getEntityId());

		EventIdComponent::releaseTeamLock(mTargetEntityId, getEntity());
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
