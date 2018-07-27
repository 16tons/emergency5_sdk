// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/fire/PickupDrowningPersonLandAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"
#include "em5/ai/MovementModes.h"
#include "em5/game/targetpoint/DiverPickupDrowningTargetPointProvider.h"
#include "em5/game/targetpoint/DiverPickupDrowningLandTargetPointProvider.h"

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/map/Map.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/logic/action/ActionComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier PickupDrowningPersonLandAction::ACTION_ID = "em5::PickupDrowningPersonLandAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PickupDrowningPersonLandAction::PickupDrowningPersonLandAction() :
		Action(ACTION_ID),
		mTowardsLand(false),
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mState(STATE_INIT)
	{
		// Nothing here
	}

	PickupDrowningPersonLandAction::~PickupDrowningPersonLandAction()
	{
		// Nothing here
	}

	void PickupDrowningPersonLandAction::init(bool towardsLand, uint64 targetEntityId)
	{
		mTowardsLand = towardsLand;
		mTargetEntityId = targetEntityId;
	}

	uint64 PickupDrowningPersonLandAction::getTargetEntityId() const
	{
		return mTargetEntityId;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void PickupDrowningPersonLandAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTowardsLand);
		serializer.serialize(mTargetEntityId);
		serializer.serializeAs<uint8>(mState);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool PickupDrowningPersonLandAction::onStartup()
	{
		return (nullptr != getEntity().getMap().getEntityById(mTargetEntityId));
	}

	qsf::action::Result PickupDrowningPersonLandAction::updateAction(const qsf::Clock&)
	{
		// Get the target entity instance
		qsf::Entity* carriedPerson = getMap().getEntityById(mTargetEntityId);
		QSF_CHECK(nullptr != carriedPerson, "PickupDrowningPersonLandAction lost the person to carry somehow", return qsf::action::RESULT_DONE);
		if (carriedPerson == nullptr)
			return qsf::action::RESULT_DONE;
		if (nullptr == carriedPerson->getComponent<qsf::ActionComponent>())
			return qsf::action::RESULT_DONE;

		switch (mState)
		{
			case STATE_INIT:
			{
				mState = STATE_PICKUP;

				if (mTowardsLand)
				{
					// Unlink carried person so we can walk to the head
					qsf::LinkComponent* linkComponent = carriedPerson->getComponent<qsf::LinkComponent>();
					if (nullptr != linkComponent)
					{
						linkComponent->unlinkFromParent();
					}

					qsf::ActionComponent& actionComponent = getComponent();
					actionComponent.pushAction<MoveAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(
							new qsf::ai::ReachObjectGoal(getEntity(), *carriedPerson, DiverPickupDrowningLandTargetPointProvider::TARGET_POINT_ID), MovementModes::MOVEMENT_MODE_WALK_BACKWARDS);
				}

				// Immediately go to the next state; no break by intent
			}

			case STATE_PICKUP:
			{
				// Get transform components
				const qsf::TransformComponent& callerTransformComponent = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();
				qsf::TransformComponent& targetTransformComponent = carriedPerson->getOrCreateComponentSafe<qsf::TransformComponent>();

				{ // Update the transform component of the target entity to ensure it has the right distance
					glm::vec3 position = callerTransformComponent.getPosition();
					glm::vec3 offsetLocal;
					if (mTowardsLand)
					{
						offsetLocal = DiverPickupDrowningLandTargetPointProvider::getOffset();
					}
					else
					{
						offsetLocal = DiverPickupDrowningTargetPointProvider::getOffset();
					}

					const glm::vec3 offsetWorld = callerTransformComponent.getTransform().vec3DirectionLocalToWorld(offsetLocal);
					position -= offsetWorld;
					targetTransformComponent.setPositionAndRotation(position, callerTransformComponent.getRotation());
				}

				if (mTowardsLand)
				{
					// Link carried person again
					qsf::LinkComponent& linkComponent = carriedPerson->getOrCreateComponentSafe<qsf::LinkComponent>();
					linkComponent.linkToParent(getEntity());
				}

				// Immediately go to the next state; no break by intent
			}
		}

		return qsf::action::RESULT_DONE;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
