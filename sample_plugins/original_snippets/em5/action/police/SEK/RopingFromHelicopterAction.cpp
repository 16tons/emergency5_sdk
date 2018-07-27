// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/police/SEK/RopingFromHelicopterAction.h"
#include "em5/action/move/StartHelicopterAction.h"
#include "em5/action/move/LandHelicopterAction.h"
#include "em5/action/police/SEK/RopeDownSEKFromHelicopterAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/command/base/ExitVehicleCommand.h"
#include "em5/component/effect/FadeEffectComponent.h"
#include "em5/component/vehicle/parts/SupportLegsComponent.h"
#include "em5/component/vehicle/HelicopterComponent.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/base/DistanceHelper.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier RopingFromHelicopterAction::ACTION_ID = "em5::RopingFromHelicopterAction";

	static const DoorComponent::DoorType USED_DOOR_TYPE = DoorComponent::DOORTYPE_PASSENGER;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	RopingFromHelicopterAction::RopingFromHelicopterAction() :
		Action(ACTION_ID),
		mState(STATE_INIT)
	{
		// Nothing here
	}

	RopingFromHelicopterAction::~RopingFromHelicopterAction()
	{
		// Nothing here
	}

	void RopingFromHelicopterAction::init(const glm::vec3& targetPosition)
	{
		mTargetPosition = targetPosition;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void RopingFromHelicopterAction::serialize(qsf::BinarySerializer& serializer)
	{
		// TODO(mk) Add serialize function
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool RopingFromHelicopterAction::onStartup()
	{
		mHelicopterComponent = getEntity().getComponent<HelicopterComponent>();
		if (!mHelicopterComponent.valid())
		{
			return false;
		}

		return true;
	}

	qsf::action::Result RopingFromHelicopterAction::updateAction(const qsf::Clock&)
	{
		switch (mState)
		{
			case STATE_INIT:
			{
				// Push an helicopter hovering action
				getComponent().pushAction<LandHelicopterAction>(action::BLOCKING, qsf::action::INSERT_BEFORE_CURRENT).init(true);

				// Make sure the optical personal is always visible
				showPersonalInside(true);

				// Stop swing
				mHelicopterComponent->stopSwing();

				mState = STATE_OPEN_DOOR;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_OPEN_DOOR:
			{
				// Open the door
				mHelicopterComponent->openDoor(USED_DOOR_TYPE, true);

				mState = STATE_EXTEND_FAST_ROPE;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_EXTEND_FAST_ROPE:
			{
				if (!mHelicopterComponent->areAllDoorsOfTypeInState(USED_DOOR_TYPE, DoorComponent::DOOR_OPEN))
				{
					// Wait for opening the doors
					return qsf::action::RESULT_CONTINUE;
				}

				SupportLegsComponent* supportLegsComponent = getEntity().getComponent<SupportLegsComponent>();
				if (nullptr != supportLegsComponent)
				{
					supportLegsComponent->startExtending();
				}

				mState = STATE_SPAWN_PERSONAL;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_SPAWN_PERSONAL:
			{
				SupportLegsComponent* supportLegsComponent = getEntity().getComponent<SupportLegsComponent>();
				if (nullptr != supportLegsComponent)
				{
					if (supportLegsComponent->isAnimationPlaying())
					{
						// Wait for animation
						return qsf::action::RESULT_CONTINUE;
					}
				}

				spawnUnits();
				showPersonalInside(false);

				mState = STATE_ROPING;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_ROPING:
			{
				if (isPersonInRopingProcess())
				{
					// Wait till all persons are finish there process
					return qsf::action::RESULT_CONTINUE;
				}

				SupportLegsComponent* supportLegsComponent = getEntity().getComponent<SupportLegsComponent>();
				if (nullptr != supportLegsComponent)
				{
					supportLegsComponent->hideExtending();
				}

				mState = STATE_HIDE_FAST_ROPE;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_HIDE_FAST_ROPE:
			{
				SupportLegsComponent* supportLegsComponent = getEntity().getComponent<SupportLegsComponent>();
				if (nullptr != supportLegsComponent)
				{
					if (supportLegsComponent->isAnimationPlaying())
					{
						// Wait for animation
						return qsf::action::RESULT_CONTINUE;
					}
				}

				mHelicopterComponent->openDoor(USED_DOOR_TYPE, false);

				mState = STATE_CLOSE_DOOR;
				return qsf::action::RESULT_CONTINUE;
			}
			case STATE_CLOSE_DOOR:
			{
				if (!mHelicopterComponent->areAllDoorsOfTypeInState(USED_DOOR_TYPE, DoorComponent::DOOR_CLOSED))
				{
					// Wait for opening the doors
					return qsf::action::RESULT_CONTINUE;
				}

				// Return to normal fly height
				getComponent().pushAction<StartHelicopterAction>(action::COMMAND_LOW);

				// Start swing
				mHelicopterComponent->startSwing();

				// Everything done, finish action
			}
		}

		return qsf::action::RESULT_DONE;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void RopingFromHelicopterAction::showPersonalInside(bool show)
	{
		// The special entity is an visible entity, we show it only in the beginning of process and hide it in the moment if the policeman jumps out of the heli
		qsf::Entity* personalInsideEntity = getMap().getEntityById(mHelicopterComponent->getSpecialEntity());
		if (nullptr != personalInsideEntity)
		{
			personalInsideEntity->getOrCreateComponentSafe<FadeEffectComponent>().init(show);
		}
	}

	void RopingFromHelicopterAction::spawnUnits()
	{
		// Get the door position
		glm::vec3 placePosition_right;
		glm::quat placeRotation_right;
		glm::vec3 placePosition_left;
		glm::quat placeRotation_left;
		std::vector<uint64> rightDoorIds = mHelicopterComponent->getVehicleDoorIdsByType(USED_DOOR_TYPE);
		if (rightDoorIds.size() > 1)
		{
			// The SEK-HC has 1 co driver in the front right and 2 passenger doors in the side (back left, back right)
			// We can't definitely say what order is left or right. I think it is not so important.
			qsf::Entity* rightDoorEntity = getMap().getEntityById(rightDoorIds[0]);
			DoorComponent* rightDoorComponent = rightDoorEntity->getComponent<DoorComponent>();
			if (nullptr != rightDoorComponent)
			{
				placePosition_right = rightDoorComponent->getDoorOffsetWorldSpacePosition();
				placeRotation_right = rightDoorComponent->getDoorRotationWorldSpaceRotation();
			}

			qsf::Entity* leftDoorEntity = getMap().getEntityById(rightDoorIds[1]);
			DoorComponent* leftDoorComponent = leftDoorEntity->getComponent<DoorComponent>();
			if (nullptr != leftDoorComponent)
			{
				placePosition_left = leftDoorComponent->getDoorOffsetWorldSpacePosition();
				placeRotation_left = leftDoorComponent->getDoorRotationWorldSpaceRotation();
			}
		}
		else
		{
			// Error
			placePosition_right = placePosition_left = EntityHelper(getEntity()).getPosition();
		}

		// Here we are using the support leg component, because it has the flexibility for this case. Don't be confused by the name.
		// We can't definitely say what order is left or right. I think it is not so important.
		// Get the fastRope entities
		qsf::Entity* fastRopeEntity_right = nullptr;
		qsf::Entity* fastRopeEntity_left = nullptr;
		SupportLegsComponent* supportLegsComponent = getEntity().getComponent<SupportLegsComponent>();
		QSF_ASSERT(nullptr != supportLegsComponent, "RopingFromHelicopterAction: can't find the support legs component with the fast rope entity. Action stuck.", return);
		if (nullptr != supportLegsComponent)
		{
			const qsf::Map& map = getMap();
			for (uint64 legId : supportLegsComponent->SupportLegsArray.getVariable())
			{
				if (nullptr == fastRopeEntity_right)
				{
					fastRopeEntity_right = map.getEntityById(legId);
				}
				else
				{
					// Don't handle more than 2
					fastRopeEntity_left = map.getEntityById(legId);
				}
			}

			// Detect the "left/right" entity
			if (nullptr != fastRopeEntity_left && nullptr != fastRopeEntity_right &&
				qsf::game::DistanceHelper::get2dDistance(placePosition_right, *fastRopeEntity_left) <
				qsf::game::DistanceHelper::get2dDistance(placePosition_right, *fastRopeEntity_right))
			{
				// Swap left and right
				qsf::Entity* buffer = fastRopeEntity_left;
				fastRopeEntity_left = fastRopeEntity_right;
				fastRopeEntity_right = buffer;
			}
		}

		// Exit the entities now
		ExitVehicleCommand::getEntitiesToExit(getEntity(), mEntitiesToExit);
		for (size_t i = 0; i < mEntitiesToExit.size(); ++i)
		{
			// Pointer must always be valid, else something went horribly wrong
			qsf::Entity* containedEntity = mEntitiesToExit[i];

			// Found correct placing position
			const bool even = (i%2 == 0);

			qsf::TransformComponent& transformComponent = containedEntity->getOrCreateComponentSafe<qsf::TransformComponent>();
			glm::quat personRotation = (even ? placeRotation_right : placeRotation_left);
			transformComponent.setRotation(personRotation);

			// Push extra action for all the entity details
			// For even "true": 1,2,3 else 3,2,1
			const int numberPersonOnFastRopeUtility = (even ? ((static_cast<int>(i)/2) + 1) : (static_cast<int>(mEntitiesToExit.size())/2 - static_cast<int>(i)/2));
			qsf::Entity* fastRopeEntity = (even ? fastRopeEntity_right : fastRopeEntity_left);
			qsf::ActionComponent* actionComponent = containedEntity->getOrCreateComponent<qsf::ActionComponent>();
			QSF_CHECK(nullptr != fastRopeEntity, "RopingFromHelicopterAction: can't find the support legs component with the fast rope entity. Action stuck.", continue);
			actionComponent->pushAction<RopeDownSEKFromHelicopterAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(getEntity(), *fastRopeEntity, numberPersonOnFastRopeUtility);
		}
	}

	bool RopingFromHelicopterAction::isPersonInRopingProcess()
	{
		for (qsf::Entity* ropingPerson : mEntitiesToExit)
		{
			qsf::ActionComponent& actionComponent = ropingPerson->getOrCreateComponentSafe<qsf::ActionComponent>();
			const qsf::Action* action = actionComponent.getActionByTypeId(RopeDownSEKFromHelicopterAction::ACTION_ID);
			if (nullptr != action)
			{
				// One or more persons is doing his "RopeDownSEKFromHelicopterAction"
				return true;
			}
		}

		return false;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
