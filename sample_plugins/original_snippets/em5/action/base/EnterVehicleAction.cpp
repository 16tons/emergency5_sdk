// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/base/EnterVehicleAction.h"
#include "em5/action/base/WaitForEnterVehicleAction.h"
#include "em5/action/police/SEK/WaitForRobotAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/base/ContainerCategory.h"
#include "em5/command/base/EnterVehicleCommand.h"
#include "em5/command/CommandContext.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/component/effect/FadeEffectComponent.h"
#include "em5/component/vehicle/VehicleComponent.h"
#include "em5/specs/ActionRangeSpecsGroup.h"
#include "em5/game/selection/SelectionManager.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/game/Game.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/logic/local/firefighters/FiremanLogic.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/EM5Helper.h"

#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/QsfGameHelper.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/move/MovableComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/map/Map.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier EnterVehicleAction::ACTION_ID = "em5::EnterVehicleAction";


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	bool EnterVehicleAction::isInDoorRange(qsf::Entity& callerEntity, qsf::Entity& targetEntity, DoorComponent::DoorType doorType, float maxDistance)
	{
		// Distance check for the closest door
		VehicleComponent* vehicleComponent = targetEntity.getComponent<VehicleComponent>();
		if (nullptr == vehicleComponent)
			return false;

		glm::vec3 callerPosition = EntityHelper(callerEntity).getPosition();
		qsf::Entity* nearestDoorEntity = vehicleComponent->findNearestDoor(doorType, callerPosition);
		if (nullptr == nearestDoorEntity)
			return false;

		DoorComponent* doorComponent = nearestDoorEntity->getComponent<DoorComponent>();
		if (nullptr == doorComponent)
			return false;

		// Make the distance check
		const float currentDistance = qsf::game::DistanceHelper::getDistance(doorComponent->getDoorOffsetWorldSpacePosition(), callerPosition);
		return (currentDistance <= maxDistance);
	}

	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	EnterVehicleAction::EnterVehicleAction() :
		Action(ACTION_ID),
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mDoorType(DoorComponent::DOORTYPE_CODRIVER),
		mCurrentState(STATE_INIT)
	{
		// Nothing here
	}

	EnterVehicleAction::~EnterVehicleAction()
	{
		// Remove reservation again
		ReserveLogic::deleteOwnReservation(getMap(), mTargetEntityId, getEntityId());
	}

	void EnterVehicleAction::init(const qsf::Entity& targetEntity, DoorComponent::DoorType doorType)
	{
		mTargetEntityId = targetEntity.getId();
		mDoorType = doorType;

		if (mDoorType == DoorComponent::DOORTYPE_UNDEFINED)
		{
			VehicleComponent* vehicleComponent = targetEntity.getComponent<VehicleComponent>();
			if (nullptr != vehicleComponent)
			{
				mDoorType = vehicleComponent->getMainDoorType();
			}
		}
	}

	uint64 EnterVehicleAction::getTargetId() const
	{
		return mTargetEntityId;
	}

	DoorComponent::DoorType EnterVehicleAction::getTargetDoorType() const
	{
		return mDoorType;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void EnterVehicleAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetEntityId);
		serializer.serializeAs<uint16>(mDoorType);
		serializer.serializeAs<uint16>(mCurrentState);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool EnterVehicleAction::onStartup()
	{
		return checkActionCondition();
	}

	qsf::action::Result EnterVehicleAction::updateAction(const qsf::Clock& clock)
	{
		// Get the most important objects first
		qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
		if (nullptr == targetEntity)
			return qsf::action::RESULT_DONE;

		VehicleComponent* vehicleComponent = targetEntity->getComponent<VehicleComponent>();
		if (nullptr == vehicleComponent)
			return qsf::action::RESULT_DONE;

		// Handle the current state
		switch (mCurrentState)
		{
			case STATE_INIT:
			{
				if (!checkActionCondition())
				{
					// Action condition has changed, maybe we have no free seat or the vehicle is gone
					return qsf::action::RESULT_DONE;
				}

				// Check for the reserve logic
				if (!ReserveLogic::checkReservation(*targetEntity, getEntityId()))
				{
					// Wait till the reservation is unlocked
					return qsf::action::RESULT_CONTINUE;
				}

				// Clear the waiting action if the target is a SEKRT
				qsf::ActionComponent* targetActionComponent = targetEntity->getComponent<qsf::ActionComponent>();
				if (nullptr != targetActionComponent)
				{
					const WaitForRobotAction* waitForRobotAction = targetActionComponent->getAction<WaitForRobotAction>();
					if (nullptr != waitForRobotAction)
					{
						targetActionComponent->clearPlan();
					}
				}

				// Reserve the entity
				ReserveLogic::createReservation<ReserveEnterVehicleLogic>(*targetEntity, getEntityId());

				// And block the target to avoid leaving passengers from target while this action is happen
				targetEntity->getOrCreateComponentSafe<qsf::ActionComponent>().pushAction<WaitForEnterVehicleAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(getEntityId(), qsf::Time::fromSeconds(3.0f));

				// Rotate personal in final direction
				{
					const std::vector<uint64>& doorIds = vehicleComponent->getVehicleDoorIdsByType(mDoorType);
					if (!doorIds.empty())
					{
						const qsf::Entity* doorEntity = getMap().getEntityById(doorIds[0]);
						if (nullptr != doorEntity)
						{
							const DoorComponent* doorComponent = doorEntity->getComponent<DoorComponent>();
							if (nullptr != doorComponent)
							{
								// TODO(mk) Push action for smooth turning
								qsf::TransformComponent* transformComponent = getEntity().getComponent<qsf::TransformComponent>();
								if (nullptr != transformComponent)
								{
									transformComponent->setRotation(doorComponent->getDoorRotationWorldSpaceRotation());
								}
							}
						}
					}
				}

				// Open the door
				vehicleComponent->openDoor(mDoorType, true);

				// Immediately go to the next state; no break by intent
				mCurrentState = STATE_OPEN_DOOR;
			}

			case STATE_OPEN_DOOR:
			{
				if (!vehicleComponent->areAllDoorsOfTypeInState(mDoorType, DoorComponent::DOOR_OPEN, 0.7f))
				{
					if (vehicleComponent->areAllDoorsOfTypeInState(mDoorType, DoorComponent::DOOR_CLOSING))
					{
						// Force an opening of the door to speed up the entering process
						vehicleComponent->openDoor(mDoorType, true);
					}

					// Wait for ending of the animation
					return qsf::action::RESULT_CONTINUE;
				}

				// Play animation
				if (EntityHelper(getEntity()).isEntityPerson())
				{
					AnimationHelper animationHelper(getEntity());
					mMeshAnimationChannel = animationHelper.playAnimation(animationHelper.getAnimationBoardAnimation());
				}

				if (EntityHelper(getEntity()).isEntityPerson() && mMeshAnimationChannel.valid())
				{
					// Half of the enter animation length; with this fade-out length, it looks good
					mWaitTime = mMeshAnimationChannel->getLength() * 0.5f;
				}
				else
				{
					// Default fade-out time
					mWaitTime = qsf::Time::fromSeconds(0.75f);
				}

				// Start visual fade out
				getEntity().getOrCreateComponentSafe<FadeEffectComponent>().fadeOut(mWaitTime);

				mCurrentState = STATE_ENTER;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_ENTER:
			{
				mWaitTime -= clock.getTimePassed();
				if (mWaitTime > qsf::Time::ZERO)
				{
					// Wait till the animation is far enough
					return qsf::action::RESULT_CONTINUE;
				}

				// Immediately go to the next state; no break by intent
				mCurrentState = STATE_IS_ENTERED;
			}

			case STATE_IS_ENTERED:
			{
				// Handling for selection, in case only the entering person is selected, select the vehicle after this
				SelectionManager& selectionManager = EM5_GAME.getSelectionManager();
				const bool selectVehicleAfterThis = (selectionManager.isIdSelected(getEntityId()) && selectionManager.getSelectionSize() == 1);

				// Via entering container we lose the selection
				container::ContainerTypes containerType = (EntityHelper(getEntity()).isCommandable() ? container::CONTAINERTYPE_SQUAD : container::CONTAINERTYPE_PASSENGER);	// The robot is a squad road vehicle
				EntityHelper(getEntity()).enterContainer(*targetEntity, containerType);

				if (selectVehicleAfterThis)
				{
					selectionManager.addIdToSelection(mTargetEntityId);
					QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_CHANGE_SELECTION));
				}

				QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_PERSON_ENTER_VEHICLE, getEntityId(), targetEntity->getId()));

				if (checkForNextUserInArea(*targetEntity))
				{
					// Abort action here to speed up the process for the next
					return qsf::action::RESULT_DONE;
				}

				// Close the door again
				vehicleComponent->openDoor(mDoorType, false);

				// Immediately go to the next state; no break by intent
				mCurrentState = STATE_CLOSE_DOOR;
			}

			case STATE_CLOSE_DOOR:
			{
				if (!vehicleComponent->areAllDoorsOfTypeInState(mDoorType, DoorComponent::DOOR_CLOSED))
				{
					// Wait for ending of the animation
					return qsf::action::RESULT_CONTINUE;
				}

				// Everything is finished correctly
				return qsf::action::RESULT_DONE;
			}
		}

		// Error
		return qsf::action::RESULT_DONE;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	bool EnterVehicleAction::checkActionCondition()
	{
		// Get the caller and target entity
		qsf::Entity& callerEntity = getEntity();
		qsf::Entity* targetEntity = callerEntity.getMap().getEntityById(mTargetEntityId);
		if (nullptr == targetEntity)
			return false;

		if (EntityHelper(callerEntity).isSquadPerson())
		{
			// Build context
			CommandContext commandContext;
			commandContext.mCaller = &callerEntity;
			commandContext.mTargetEntity = targetEntity;

			// Call "checkCaller" && "checkContext" of the commando for validation
			EnterVehicleCommand* enterVehicleCommand = static_cast<EnterVehicleCommand*>(QSFGAME_COMMAND.getCommandManager().getById<EnterVehicleCommand>(EnterVehicleCommand::PLUGINABLE_ID));
			QSF_CHECK(nullptr != enterVehicleCommand, "EnterVehicleAction::onStartup(): Could not find enter vehicle command instance", return false);
			if (!(enterVehicleCommand->checkCallerWithoutPriority(*commandContext.mCaller) && enterVehicleCommand->checkContext(commandContext)))
				return false;
		}
		// TODO(mk) Else civil persons can enter an vehicle without checking for correct seats. Hope we don't get trouble because this

		if (!EnterVehicleAction::isInDoorRange(callerEntity, *targetEntity, mDoorType, ActionRangeSpecsGroup::getInstanceSafe().getEnterVehicleDistance()))
			return false;

		// Check if target is moving
		const qsf::MovableComponent* movableComponent = targetEntity->getComponent<qsf::MovableComponent>();
		if (nullptr != movableComponent && movableComponent->isMoving())
		{
			return false;
		}

		// Extra check for firefighter with hose, don't let them enter. (we can't check this in the EnterVehicleCommand because we try to avoid this in the FireHoseCheckAction, this cold fail in some cases)
		const FiremanLogic* firemanLogic = EntityHelper(callerEntity).getGameLogic<FiremanLogic>();
		if (nullptr != firemanLogic && firemanLogic->isConnectedToHydrant())
		{
			// Fireman is connected to hose
			return false;
		}

		// Every check is valid, return true
		return true;
	}

	bool EnterVehicleAction::checkForNextUserInArea(qsf::Entity& targetEntity) const
	{
		// This code is the same as GetEquipmentAction::checkForNextUserInArea()
		std::vector<qsf::ActionComponent*> actionComponentList;
		static const float searchRadius = 10.0f;
		qsf::ComponentMapQuery(getMap()).getInstancesInCircle(EntityHelper(getEntity()).getPosition(), searchRadius, actionComponentList);

		// No entities found
		if (actionComponentList.empty())
			return false;

		// Try to find a other entity in range
		for (const qsf::ActionComponent* actionComponent : actionComponentList)
		{
			const EnterVehicleAction* enterVehicleAction = actionComponent->getAction<EnterVehicleAction>();
			if (nullptr == enterVehicleAction)
				continue;

			// Exclude yourself
			if (enterVehicleAction == this)
				continue;

			// Check for seat inside the vehicle
			if (!EntityHelper(actionComponent->getEntity()).checkVehicleHasFreeSeats(targetEntity))
				continue;

			if (enterVehicleAction->mTargetEntityId == mTargetEntityId)
			{
				// Found an entity with the same target in range
				return true;
			}
		}

		// No entities in range
		return false;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
