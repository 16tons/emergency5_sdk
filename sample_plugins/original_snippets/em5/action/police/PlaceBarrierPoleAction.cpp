// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/police/PlaceBarrierPoleAction.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/equipment/DropEquipmentAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/specs/LogicSpecsGroup.h"
#include "em5/component/objects/BarrierTapeComponent.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/base/DistanceHelper.h>

#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/math/Math.h>
#include <qsf/math/CoordinateSystem.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier PlaceBarrierPoleAction::ACTION_ID = "em5::PlaceBarrierPoleAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PlaceBarrierPoleAction::PlaceBarrierPoleAction() :
		Action(ACTION_ID),
		mState(STATE_INIT)
	{
		// Nothing here
	}

	PlaceBarrierPoleAction::~PlaceBarrierPoleAction()
	{
		// Nothing here
	}

	void PlaceBarrierPoleAction::init(BarrierTapeComponent* barrierTapeComponent)
	{
		mBarrierTapeComponent = barrierTapeComponent;
	}

	PlaceBarrierPoleAction::State PlaceBarrierPoleAction::getActionState() const
	{
		return mState;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void PlaceBarrierPoleAction::serialize(qsf::BinarySerializer& serializer)
	{
		// Nothing to do
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool PlaceBarrierPoleAction::onStartup()
	{
		if (!mBarrierTapeComponent.valid())
			return false;

		if (mBarrierTapeComponent->getNumPoles() != 2)
			return false;

		mState = STATE_INIT;

		return true;
	}

	void PlaceBarrierPoleAction::onShutdown()
	{
		if (mBarrierTapeComponent.valid() && !mBarrierTapeComponent->isFinishedBuilding())
		{
			EntityHelper(getEntity()).deleteActiveEquipment();
			MapHelper(getMap()).destroyEntity(mBarrierTapeComponent->getEntity());

			AnimationHelper::AnimationMaskData animationMaskData;
			AnimationHelper animationHelper(getEntity());
			animationHelper.playAnimation(animationHelper.getIdleAnimation(animationMaskData), true, false);
		}
	}


	qsf::action::Result PlaceBarrierPoleAction::updateAction(const qsf::Clock&)
	{
		if (!mBarrierTapeComponent.valid())
			return qsf::action::RESULT_DONE;

		qsf::Entity* firstPoleEntity = mBarrierTapeComponent->getPoleEntity(0);
		qsf::Entity* secondPoleEntity = mBarrierTapeComponent->getPoleEntity(1);
		if (nullptr == firstPoleEntity || nullptr == secondPoleEntity)
			return qsf::action::RESULT_DONE;

		const glm::vec3& secondPolePosition = secondPoleEntity->getOrCreateComponentSafe<qsf::TransformComponent>().getPosition();
		const glm::vec3& firstPolePosition = firstPoleEntity->getOrCreateComponentSafe<qsf::TransformComponent>().getPosition();

		switch (mState)
		{
			case STATE_INIT:
			{
				const float transparency = LogicSpecsGroup::getInstanceSafe().getPoliceBarrierTransparency();

				mBarrierTapeComponent->createTape(BarrierTapeComponent::POLE_TO_POLE);

				mBarrierTapeComponent->deletePoleMarker(0);
				mBarrierTapeComponent->deletePoleMarker(1);

				mBarrierTapeComponent->setPoleTransparency(0, transparency);
				mBarrierTapeComponent->setPoleTransparency(1, transparency);
				mBarrierTapeComponent->setTapeTransparency(transparency);

				mState = STATE_MOVE_TO_POLE0;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_MOVE_TO_POLE0:
			{
				// Move to pole 0
				getComponent().pushAction<TurnToAction>(getPriority(), qsf::action::INSERT_AT_FRONT).init(firstPoleEntity->getId());
				getComponent().pushAction<MoveAction>(getPriority(), qsf::action::INSERT_AT_FRONT).init(new qsf::ai::ReachSinglePointGoal(qsf::logic::TargetPoint(firstPolePosition, false, 0.5f, 0.1f)));

				mState = STATE_DROP_POLE0;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_DROP_POLE0:
			{
				// Place pole 0
				if (!validateUserAndTarget(firstPolePosition))
					return qsf::action::RESULT_DONE;

				if (!mBarrierTapeComponent->isPolePositionValid(0))
					return qsf::action::RESULT_DONE;

				mBarrierTapeComponent->setPoleTransparency(0, 0.0f);

				// Attach tape animation
				getComponent().pushAction<PlayAnimationAction>(getPriority(), qsf::action::INSERT_AT_FRONT).init(AnimationHelper(getEntity()).getAnimationPoliceBarrierTape01(), true);

				mState = STATE_MOVE_TO_POLE1;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_MOVE_TO_POLE1:
			{
				// Attach tape to pole 0
				mBarrierTapeComponent->createTape(BarrierTapeComponent::POLE_TO_SQUAD, &getEntity());
				mBarrierTapeComponent->setTapeTransparency(0.0f);

				// Move to pole 1
				const glm::vec3 secondTargetPoint = secondPolePosition + glm::normalize(secondPolePosition - firstPolePosition) * 0.5f;

				getComponent().pushAction<TurnToAction>(getPriority(), qsf::action::INSERT_AT_FRONT).init(secondPoleEntity->getId());
				getComponent().pushAction<MoveAction>(getPriority(), qsf::action::INSERT_AT_FRONT).init(new qsf::ai::ReachSinglePointGoal(qsf::logic::TargetPoint(secondTargetPoint, false, 0.0f, 0.2f)));

				mState = STATE_DROP_POLE1;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_DROP_POLE1:
			{
				// Place pole 0
				if (!validateUserAndTarget(secondPolePosition))
					return qsf::action::RESULT_DONE;

				if (!mBarrierTapeComponent->isPolePositionValid(1))
					return qsf::action::RESULT_DONE;

				// Set final tape
				mBarrierTapeComponent->createTape(BarrierTapeComponent::POLE_TO_POLE);
				mBarrierTapeComponent->setPoleTransparency(1, 0.0f);

				// Finish creating the barrier
				mBarrierTapeComponent->createBarrier();
				mBarrierTapeComponent->setFinishedBuilding(true);

				// Attach tape animation and drop equipment
				getComponent().pushAction<DropEquipmentAction>(getPriority(), qsf::action::INSERT_AT_FRONT).init();
				getComponent().pushAction<PlayAnimationAction>(getPriority(), qsf::action::INSERT_AT_FRONT).init(AnimationHelper(getEntity()).getAnimationPoliceBarrierTape03(), true);

				mState = STATE_FINISH;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_FINISH:
			{
				// Nothing more to do
				return qsf::action::RESULT_DONE;
			}
		}

		return qsf::action::RESULT_DONE;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	bool PlaceBarrierPoleAction::validateUserAndTarget(const glm::vec3& targetPosition) const
	{
		const float MAX_DISTANCE = 1.5f; // Be careful not to make it too small as there is a deliberate offset to the target.
		qsf::TransformComponent& squadTransformComponent = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();
		float currentDistanceToTarget = qsf::game::DistanceHelper::get2dDistance(squadTransformComponent.getPosition(), targetPosition);
		if (currentDistanceToTarget > MAX_DISTANCE)
			return false;

		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
