// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/equipment/GetDogFromVehicleAction.h"
#include "em5/action/base/DisappearAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/action/move/LinearMoveAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/logic/local/ambulance/RescueDogLeaderLogic.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/map/EntityHelper.h"

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier GetDogFromVehicleAction::ACTION_ID = "em5::GetDogFromVehicleAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GetDogFromVehicleAction::GetDogFromVehicleAction() :
		GetEquipmentAction(ACTION_ID),
		mDogEnterVehicle(true),
		mPreventUpdateAnimationsActive(false)
	{
		// Nothing here
	}

	GetDogFromVehicleAction::~GetDogFromVehicleAction()
	{
		ReserveLogic::deleteOwnReservation(getMap(), mTargetEntityId, getEntityId());
	}

	void GetDogFromVehicleAction::init(qsf::Entity& targetEntity, bool dogEnterVehicle)
	{
		mTargetEntityId = targetEntity.getId();
		mTargetEntity = &targetEntity;
		mDogEnterVehicle = dogEnterVehicle;

		mRoadVehicleComponent = targetEntity.getComponent<RoadVehicleComponent>();
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void GetDogFromVehicleAction::serialize(qsf::BinarySerializer& serializer)
	{
		em5::GetEquipmentAction::serialize(serializer);
		serializer.serialize(mTargetEntityId);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	void GetDogFromVehicleAction::onShutdown()
	{
		// In case this action got aborted by someone else, clean up
		if (mCurrentState != STATE_DONE)
		{
			if (mRoadVehicleComponent.valid())
			{
				mRoadVehicleComponent->openDoor(mUsedDoorType, false);
			}

			if (mPreventUpdateAnimationsActive && mRescueDogEntity.valid())
			{
				PersonComponent* personComponent = mRescueDogEntity->getComponent<PersonComponent>();
				if (nullptr != personComponent)
				{
					personComponent->preventUpdateAnimations(false);
				}
			}
		}
	}

	qsf::action::Result GetDogFromVehicleAction::updateAction(const qsf::Clock&)
	{
		QSF_CHECK(mTargetEntity.valid(), "Target entity not valid anymore", QSF_REACT_NONE);
		QSF_CHECK(mRoadVehicleComponent.valid(), "Road vehicle component not valid anymore", QSF_REACT_NONE);

		if (!mTargetEntity.valid() || !mRoadVehicleComponent.valid())
		{
			// Error vehicle is not exist. Can happen if the vehicle is destroyed while the action is active
			// In both cases (get dog/put dog) we will end the action with an valid rescue dog
			RescueDogLeaderLogic::createRescueDogEntity(getEntity());
			AnimationHelper(getEntity()).clearAnimation();

			// Error case, delete action plan
			return qsf::action::RESULT_CLEARLIST;
		}

		// Get the entity of the caller
		switch (mCurrentState)
		{
			case STATE_INIT:
			{
				// Reserve the entity
				ReserveLogic::createReservation<ReserveWaitForFinishLogic>(mTargetEntity.getSafe(), getEntityId());

				getAndTurnToClosestEquipmentDoor();

				// Start the animation of opening the doors
				mRoadVehicleComponent->openDoor(mUsedDoorType, true);

				getComponent().pushAction<TurnToAction>(getPriority(), qsf::action::INSERT_AT_FRONT).init(mTargetEntityId);

				// Use linear move action to get to a better position
				if (mDogEnterVehicle)
				{
					VehicleComponent* vehicleComponent = mTargetEntity.getSafe().getComponent<VehicleComponent>();
					if (nullptr != vehicleComponent)
					{
						std::vector<qsf::Transform> trunkDoors;
						qsf::Transform& transform = mTargetEntity.getSafe().getOrCreateComponentSafe<qsf::TransformComponent>().getTransform();
						vehicleComponent->getDoorPositions(DoorComponent::DOORTYPE_TRUNK, trunkDoors, transform);

						// We have usually only one trunk door !
						if (!trunkDoors.empty())
						{
							getComponent().pushAction<LinearMoveAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(trunkDoors.front().getPosition(), 0.5f);
						}
					}
				}

				mCurrentState = STATE_OPEN_EQUIPMENT_DOOR;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_OPEN_EQUIPMENT_DOOR:
			{
				if (!mRoadVehicleComponent->areAllDoorsOfTypeInState(mUsedDoorType, DoorComponent::DOOR_OPEN))
				{
					// Wait for ending of the animation
					return qsf::action::RESULT_CONTINUE;
				}

				playUseAnimation();
				playRescueDogAnimation();

				mCurrentState = STATE_PLAY_USEANIMATION;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_PLAY_USEANIMATION:
			{
				if (mUseAnimationChannel.valid() && !mUseAnimationChannel->isFinished())
				{
					// Wait till animation is finished
					return qsf::action::RESULT_CONTINUE;
				}

				deleteEnteredDog();
				PersonComponent* personComponent = mRescueDogEntity->getComponent<PersonComponent>();
				if (nullptr != personComponent)
				{
					personComponent->preventUpdateAnimations(false);
					mPreventUpdateAnimationsActive = false;
				}

				// Start the animation of closing the doors
				mRoadVehicleComponent->openDoor(mUsedDoorType, false);

				mCurrentState = STATE_CLOSE_EQUIPMENT_DOOR;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_CLOSE_EQUIPMENT_DOOR:
			{
				if (!mRoadVehicleComponent->areAllDoorsOfTypeInState(mUsedDoorType, DoorComponent::DOOR_CLOSED))
				{
					// Wait for ending of the animation
					return qsf::action::RESULT_CONTINUE;
				}

				mCurrentState = STATE_DONE;
				break;
			}

			case STATE_TURN_TO_DOOR:
			case STATE_GET_EQUIPMENT:
			case STATE_DONE:
				// Nothing to do in this case
				break;
		}

		return qsf::action::RESULT_DONE;
	}


	//[-------------------------------------------------------]
	//[ Protected virtual em5::GetEquipmentAction methods     ]
	//[-------------------------------------------------------]
	std::vector<DoorComponent*> GetDogFromVehicleAction::getEquipmentDoorsFromTarget() const
	{
		std::vector<DoorComponent*> doorComponents;
		const qsf::Map& map = getMap();
		for (uint64 doorId : mRoadVehicleComponent->getVehicleDoors())
		{
			const qsf::Entity* doorEntity = map.getEntityById(doorId);
			if (nullptr != doorEntity)
			{
				DoorComponent* doorComponent = doorEntity->getComponent<DoorComponent>();
				if (nullptr != doorComponent && doorComponent->getDoorType() == DoorComponent::DOORTYPE_TRUNK)
				{
					// Test valid, use this component for future
					doorComponents.push_back(doorComponent);
				}
			}
		}
		return doorComponents;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void GetDogFromVehicleAction::playRescueDogAnimation()
	{
		std::string animationName;
		qsf::Time blendTime = qsf::Time::ZERO;
		if (mDogEnterVehicle)
		{
			getRescueDogFromGameLogic();
			animationName = AnimationHelper(mRescueDogEntity.getSafe()).getAnimationBoardAnimation();

			EntityHelper(mRescueDogEntity.getSafe()).fadeOut(qsf::Time::fromSeconds(2.0f));
		}
		else
		{
			// Create and attach the dog entity
			mRescueDogEntity = RescueDogLeaderLogic::createRescueDogEntity(getEntity());
			RescueDogLeaderLogic::linkDogWithLeader(getEntity(), true);

			animationName = AnimationHelper(mRescueDogEntity.getSafe()).getAnimationDeboardAnimation();
			blendTime = qsf::Time::fromSeconds(0.2f);

			// Abort animation for the rescue dog leader
			AnimationHelper animationHelper(getEntity());
			AnimationHelper::AnimationMaskData animationMaskData;
			animationHelper.playAnimation(animationHelper.getIdleAnimation(animationMaskData));

			getComponent().pushAction<TurnToAction>(action::BLOCKING, qsf::action::INSERT_BEFORE_CURRENT).init(mTargetEntityId, true);
		}

		// Play animation for the dog
		if (mRescueDogEntity.valid())
		{
			PersonComponent* personComponent = mRescueDogEntity->getComponent<PersonComponent>();
			if (nullptr != personComponent)
			{
				personComponent->preventUpdateAnimations(true);
				mPreventUpdateAnimationsActive = true;
			}

			mUseAnimationChannel = AnimationHelper(*mRescueDogEntity).playAnimation(animationName, false, false, blendTime);
			if (EntityHelper(getEntity()).isEntityVisibleOnScreen())
			{
				// Avoid visual one frame glitches
				mRescueDogEntity->getOrCreateComponentSafe<qsf::MeshAnimationComponent>().enforceGraphicsSynchronization();
			}
		}
	}

	void GetDogFromVehicleAction::getRescueDogFromGameLogic()
	{
		qsf::GameLogicComponent& gamelogicComponent = getEntity().getOrCreateComponentSafe<qsf::GameLogicComponent>();
		mRescueDogEntity = gamelogicComponent.getOrCreateGameLogicSafe<RescueDogLeaderLogic>().getRescueDog();
	}

	void GetDogFromVehicleAction::deleteEnteredDog() const
	{
		if (mDogEnterVehicle)
		{
			// Unlink the dog from the leader
			qsf::LinkComponent& linkComponent = mRescueDogEntity->getOrCreateComponentSafe<qsf::LinkComponent>();
			linkComponent.setParentId(qsf::getUninitialized<uint64>());

			// Setup the rescue dog leader logic
			qsf::GameLogicComponent& gamelogicComponent = getEntity().getOrCreateComponentSafe<qsf::GameLogicComponent>();
			RescueDogLeaderLogic& rescueDogLeaderLogic = gamelogicComponent.getOrCreateGameLogicSafe<RescueDogLeaderLogic>();
			rescueDogLeaderLogic.setRescueDog(nullptr);
			rescueDogLeaderLogic.setIsDogUnitedWithLeader(false);

			qsf::ActionComponent& actionComponent = mRescueDogEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
			actionComponent.pushAction<DisappearAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(qsf::Time::fromSeconds(1.5f), true);
		}
		else
		{
			// TODO(mk) Not the perfect place
			mRescueDogEntity->getOrCreateComponentSafe<qsf::LinkComponent>().linkToParent(getEntity());
			RescueDogLeaderLogic::linkDogWithLeader(getEntity());
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
