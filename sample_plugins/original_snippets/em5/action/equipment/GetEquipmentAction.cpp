// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/equipment/GetEquipmentAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/component/equipment/InventoryComponent.h"
#include "em5/component/vehicle/VehicleComponent.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/map/EntityHelper.h"
#include "em5/specs/ActionRangeSpecsGroup.h"

#include <qsf_game/base/DistanceHelper.h>

#include <qsf_ai/navigation/NavigationComponent.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/action/WaitAction.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	// TODO(mk) We need this function for the different game speeds of the equipped units. But this place is not very good. Its better to do this in the FiremanLogic / Police(sek)Logic.
	void changeMovementSpeedForFFAndPoliceMen(qsf::Entity& caller)
	{
		const CommandableComponent* commandableComponent = caller.getComponent<CommandableComponent>();
		if (nullptr != commandableComponent)
		{
			EntityHelper entityHelper(caller);

			// Special case for firefighter
			if (commandableComponent->isFireFighterPerson())
			{
				qsf::ai::NavigationComponent* navigationComponent = caller.getComponent<qsf::ai::NavigationComponent>();
				if (nullptr != navigationComponent)
				{
					const bool hasHeavyEquipment = (entityHelper.checkIsActiveEquipment(equipment::RESCUE_CUTTER) || entityHelper.checkIsActiveEquipment(equipment::CHAIN_SAW) || entityHelper.checkIsActiveEquipment(equipment::CHAIN_SAW_HISTORICAL));
					navigationComponent->setNativeMaxForwardSpeed(hasHeavyEquipment ? 2.35f : 3.3f);
				}
			}
			else if (commandableComponent->isPoliceSEKPerson())
			{
				qsf::ai::NavigationComponent* navigationComponent = caller.getComponent<qsf::ai::NavigationComponent>();
				if (nullptr != navigationComponent)
				{
					const bool hasHeavyEquipment = (entityHelper.checkIsActiveEquipment(equipment::POLICE_SNIPERRIFLE));
					navigationComponent->setNativeMaxForwardSpeed(hasHeavyEquipment ? 2.5f : 3.3f);
				}
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier GetEquipmentAction::ACTION_ID = "em5::GetEquipmentAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GetEquipmentAction::GetEquipmentAction() :
		Action(ACTION_ID),
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mUsedDoorType(DoorComponent::DOORTYPE_UNDEFINED),
		mCurrentState(STATE_INIT),
		mNeedsTarget(false),
		mDoctorUsesBackDoor(false),
		mForceTintColorIndex(-1)
	{
		// Nothing here
	}

	GetEquipmentAction::GetEquipmentAction(uint32 identifier) :
		Action(identifier),
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mUsedDoorType(DoorComponent::DOORTYPE_UNDEFINED),
		mCurrentState(STATE_INIT),
		mNeedsTarget(false),
		mDoctorUsesBackDoor(false),
		mForceTintColorIndex(-1)
	{
		// Nothing here
	}

	GetEquipmentAction::~GetEquipmentAction()
	{
		// Clear animation
		AnimationHelper(getEntity()).clearAnimation();

		if (mNeedsTarget)
		{
			ReserveLogic::deleteOwnReservation(getMap(), mTargetEntityId, getEntityId());
		}
	}

	void GetEquipmentAction::init(const qsf::Entity& targetEntity, const std::string& equipmentName)
	{
		mTargetEntityId = targetEntity.getId();
		mEquipmentName = equipmentName;
		mNeedsTarget = qsf::isInitialized(mTargetEntityId);
	}

	void GetEquipmentAction::init(const std::string& equipmentName, int forceTintColorIndex)
	{
		mEquipmentName = equipmentName;
		mNeedsTarget = false;
		mForceTintColorIndex = forceTintColorIndex;
	}

	GetEquipmentAction::State GetEquipmentAction::getState()
	{
		return mCurrentState;
	}

	const std::string& GetEquipmentAction::getEquipmentName() const
	{
		return mEquipmentName;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void GetEquipmentAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetEntityId);
		serializer.serialize(mEquipmentName);
		serializer.serializeAs<uint16>(mUsedDoorType);
		serializer.serializeAs<uint16>(mCurrentState);
		serializer.serialize(mNeedsTarget);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool GetEquipmentAction::onStartup()
	{
		// Get the caller and target entity
		qsf::Entity& callerEntity = getEntity();

		// Already got that equipment
		EntityHelper entityHelper(getEntity());
		if (entityHelper.checkIsActiveEquipment(mEquipmentName))
			return false;

		// In case the target is not set, equipment is switched immediately
		qsf::Entity* targetEntity = callerEntity.getMap().getEntityById(mTargetEntityId);
		if (nullptr != targetEntity)
		{
			// Extra check for doctors, rescue dog leaders and engineers, there can be only one per vehicle
			CommandableComponent* commandableComponent = callerEntity.getComponent<CommandableComponent>();
			if (nullptr != commandableComponent)
			{
				if (commandableComponent->isAmbulanceDoctor() || commandableComponent->isAmbulanceRescueDogLeader() || commandableComponent->isTechEngineer())
				{
					mDoctorUsesBackDoor = true;
					if (!ReserveLogic::checkReservation(*targetEntity, getEntityId()))
					{
						// The put medikit back to vehicle back door logic is not able to do the job correctly. clear the action plan (move to/enter/reserve the vehicle)
						getComponent().clearPlan();
						return false;
					}

					// Abort the movement action of the vehicle to avoid driving away while manipulating the back door
					qsf::ActionComponent* targetActionComponent = targetEntity->getComponent<qsf::ActionComponent>();
					if (nullptr != targetActionComponent)
					{
						MoveAction* moveAction = targetActionComponent->getAction<MoveAction>();
						if (nullptr != moveAction)
						{
							targetActionComponent->removeAction(*moveAction);
						}
					}
				}
			}

			// Make the distance check
			const float maxDistance = ActionRangeSpecsGroup::getInstanceSafe().getTakeEquipmentDistance();
			const float currentDistance = qsf::game::DistanceHelper::getDistance(*targetEntity, callerEntity);
			if (currentDistance > maxDistance)
			{
				// Caller is not in the range of the target
				if (mDoctorUsesBackDoor)
				{
					getComponent().clearPlan();
				}
				return false;
			}
		}

		// Create inventory component for later use
		callerEntity.getOrCreateComponent<InventoryComponent>();

		return true;
	}

	qsf::action::Result GetEquipmentAction::updateAction(const qsf::Clock&)
	{
		// Get the entity of the caller and the target
		qsf::Entity& callerEntity = getEntity();

		VehicleComponent* vehicleComponent = nullptr;
		if (mNeedsTarget)
		{
			const qsf::Entity* targetEntity = callerEntity.getMap().getEntityById(mTargetEntityId);
			if (nullptr == targetEntity)
				return qsf::action::RESULT_DONE;

			vehicleComponent = targetEntity->getComponent<VehicleComponent>();
			if (nullptr == vehicleComponent)
				return qsf::action::RESULT_DONE;
		}

		switch (mCurrentState)
		{
			case STATE_INIT:
			{
				if (mNeedsTarget)
				{
					// Overrides other reserves. To indicate that someone is working with the vehicle.
					// Reserve the entity
					qsf::Entity* targetEntity = callerEntity.getMap().getEntityById(mTargetEntityId);
					ReserveLogic::createReservation<ReserveWaitForFinishLogic>(*targetEntity, getEntityId());

					getAndTurnToClosestEquipmentDoor();

					mCurrentState = STATE_OPEN_EQUIPMENT_DOOR;

					// Fallthrough to next case by design
				}
				else
				{
					// TODO(mk) Handle the get equipment animation as extra property inside the equipment component
					// Special handling for an animation with extra get equipment animation
					if (mEquipmentName == equipment::POLICE_PISTOL)
					{
						AnimationHelper animationHelper(callerEntity);
						mUseAnimationChannel = animationHelper.playAnimation(animationHelper.getAnimationSEKSpecialistGetPistol());
					}
					else if (mEquipmentName == equipment::POLICE_SWORD)
					{
						AnimationHelper animationHelper(callerEntity);
						mUseAnimationChannel = animationHelper.playAnimation(animationHelper.getAnimationGuardsmanEquipSword());
					}

					// Immediately change equipment
					mCurrentState = STATE_PLAY_USEANIMATION;

					return qsf::action::RESULT_CONTINUE;
				}
			}

			case STATE_OPEN_EQUIPMENT_DOOR:
			{
				QSF_CHECK(mNeedsTarget, "GetEquipmentAction::updateAction(): State STATE_OPEN_EQUIPMENT_DOOR is invalid in case no target vehicle is needed", QSF_REACT_THROW);

				// Start the animation of opening the doors (if not done already)
				vehicleComponent->openDoor(mUsedDoorType, true);

				const float minimumDoorState = 0.5f;
				if (!vehicleComponent->areAllDoorsOfTypeInState(mUsedDoorType, DoorComponent::DOOR_OPEN, minimumDoorState))
				{
					// Wait for ending of the animation
					return qsf::action::RESULT_CONTINUE;
				}

				playUseAnimation();

				// Delete the active equipment after some time
				EntityHelper(callerEntity).deleteActiveEquipment(qsf::Time::fromSeconds(1.2f));

				mCurrentState = STATE_PLAY_USEANIMATION;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_PLAY_USEANIMATION:
			{
				if (mUseAnimationChannel.valid() && mUseAnimationChannel->getTimePosition() < qsf::Time::fromSeconds(0.5f))
				{
					// Wait till animation is finished
					return qsf::action::RESULT_CONTINUE;
				}

				mCurrentState = STATE_GET_EQUIPMENT;

				// Fallthrough by design
			}

			case STATE_GET_EQUIPMENT:
			{
				// Create and attach the equipment
				EntityHelper entityHelper(getEntity());
				entityHelper.showEquipment(mEquipmentName, mForceTintColorIndex);

				// TODO(mk) This function call here should be in the logic of the specific units
				changeMovementSpeedForFFAndPoliceMen(getEntity());

				if (mNeedsTarget)
				{
					if (checkForNextUserInArea())
					{
						return qsf::action::RESULT_DONE;
					}

					mCurrentState = STATE_CLOSE_EQUIPMENT_DOOR;
					return qsf::action::RESULT_CONTINUE;
				}

				// Special handling for the signaling disc
				if (mEquipmentName == equipment::POLICE_SIGNALLINGDISC)
				{
					entityHelper.setFlareActive(false);
				}

				// Finish, getting equipment instantly
				return qsf::action::RESULT_DONE;
			}

			case STATE_CLOSE_EQUIPMENT_DOOR:
			{
				QSF_CHECK(mNeedsTarget, "GetEquipmentAction::updateAction(): State STATE_CLOSE_EQUIPMENT_DOOR is invalid in case no target vehicle is needed", QSF_REACT_THROW);

				if (mUseAnimationChannel.valid() && mUseAnimationChannel->getTimePosition() < qsf::Time::fromSeconds(1.0f))
				{
					// Switch to idle animation
					AnimationHelper animationHelper(getEntity());
					AnimationHelper::AnimationMaskData animationMaskData;
					animationHelper.playAnimation(animationHelper.getIdleAnimation(animationMaskData), false, false, qsf::Time::fromSeconds(0.4f));

					return qsf::action::RESULT_CONTINUE;
				}

				// Start the animation of closing the doors
				vehicleComponent->openDoor(mUsedDoorType, false);

				mCurrentState = STATE_DONE;
				// Fallthrough by design
			}

			case STATE_DONE:
			{
				if (mNeedsTarget)
				{
					// Shortly wait (because of animation), then turn away from door
					getAndTurnToClosestEquipmentDoor(true, action::AUTOMATISM_LOW);
					getComponent().pushAction<qsf::WaitAction>(action::AUTOMATISM_LOW, qsf::action::INSERT_AT_FRONT).init(qsf::Time::fromSeconds(0.25f));
				}

				// Everything done
				return qsf::action::RESULT_DONE;
			}
		}

		return qsf::action::RESULT_DONE;
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	void GetEquipmentAction::getAndTurnToClosestEquipmentDoor()
	{
		getAndTurnToClosestEquipmentDoor(false, action::BLOCKING);
	}

	void GetEquipmentAction::getAndTurnToClosestEquipmentDoor(bool faceAway, qsf::action::Priority actionPriority)
	{
		const DoorComponent* nearestDoorComponent = getNearestDoorFromList(getEquipmentDoorsFromTarget());
		if (nullptr != nearestDoorComponent)
		{
			mUsedDoorType = nearestDoorComponent->getDoorType();

			glm::quat rotation = nearestDoorComponent->getDoorRotationWorldSpaceRotation();
			if (faceAway)
				rotation = glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * rotation;

			// Push action
			getComponent().pushAction<TurnToAction>(actionPriority, qsf::action::INSERT_AT_FRONT).initWithRotation(rotation, 540.0f);
		}
	}

	DoorComponent* GetEquipmentAction::getNearestDoorFromList(const std::vector<DoorComponent*>& doorComponents) const
	{
		DoorComponent* shortestDoorComponent = nullptr;
		if (!doorComponents.empty())
		{
			const glm::vec3& callerPosition = getEntity().getComponentSafe<qsf::TransformComponent>().getPosition();
			float shortestDistance = 0.0f;

			// Find the shortest door from current position
			for (DoorComponent* currentDoorComponent : doorComponents)
			{
				const float currentDistance = qsf::game::DistanceHelper::getDistance(currentDoorComponent->getDoorOffsetWorldSpacePosition(), callerPosition);
				if (shortestDistance > currentDistance || nullptr == shortestDoorComponent)
				{
					shortestDoorComponent = currentDoorComponent;
					shortestDistance = currentDistance;
				}
			}
		}

		return shortestDoorComponent;
	}

	bool GetEquipmentAction::checkForNextUserInArea()
	{
		if (mDoctorUsesBackDoor)
			return false;	// No other users for this type of units

		std::vector<qsf::ActionComponent*> actionComponentList;
		static const float searchRadius = 10.0f;
		qsf::ComponentMapQuery(getMap()).getInstancesInCircle(EntityHelper(getEntity()).getPosition(), searchRadius, actionComponentList);

		// No entities found
		if (actionComponentList.empty())
			return false;

		// Try to find a other entity in range
		for (const qsf::ActionComponent* actionComponent : actionComponentList)
		{
			const GetEquipmentAction* getEquipmentAction = actionComponent->getAction<GetEquipmentAction>();
			if (nullptr == getEquipmentAction)
				continue;

			// Exclude yourself
			if (getEquipmentAction == this)
				continue;

			if (getEquipmentAction->mTargetEntityId != mTargetEntityId)
				continue;

			if (getEquipmentAction->mUsedDoorType == mUsedDoorType)
			{
				// Found an entity with the same target in range
				return true;
			}
		}

		// No entities in range
		return false;
	}

	void GetEquipmentAction::playUseAnimation()
	{
		AnimationHelper animationHelper(getEntity());
		std::string animationName = animationHelper.getAnimationUseObject();

		// Special handling for ambulance doctor and engineer
		const CommandableComponent* commandableComponent = getEntity().getComponent<CommandableComponent>();
		if (nullptr != commandableComponent)
		{
			// Use extra animations for doctor
			if (commandableComponent->isAmbulanceDoctor())
			{
				animationName = animationHelper.getAnimationDoctorGetEquipmentFromCar();
			}
			// Use extra animations for engineer
			else if (commandableComponent->isTechEngineer())
			{
				animationName = animationHelper.getAnimationEngineerGetEquipmentFromCar();
			}
		}

		mUseAnimationChannel = animationHelper.playAnimation(animationName);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual em5::GetEquipmentAction methods     ]
	//[-------------------------------------------------------]
	std::vector<DoorComponent*> GetEquipmentAction::getEquipmentDoorsFromTarget() const
	{
		std::vector<DoorComponent*> doorComponents;
		const qsf::Map& map = getMap();
		const qsf::Entity* targetEntity = map.getEntityById(mTargetEntityId);
		if (nullptr != targetEntity)
		{
			const VehicleComponent* vehicleComponent = targetEntity->getComponent<VehicleComponent>();
			if (nullptr != vehicleComponent)
			{
				// Collect all interesting door components from target
				for (uint64 doorId : vehicleComponent->getVehicleDoors())
				{
					qsf::Entity* doorEntity = map.getEntityById(doorId);
					if (nullptr != doorEntity)
					{
						DoorComponent* doorComponent = doorEntity->getComponent<DoorComponent>();
						if (nullptr != doorComponent)
						{
							if (doorComponent->getDoorType() == DoorComponent::DOORTYPE_EQUIPMENTLEFT ||
								doorComponent->getDoorType() == DoorComponent::DOORTYPE_EQUIPMENTRIGHT)
							{
								// Test valid, use this component for future
								doorComponents.push_back(doorComponent);
							}
						}
					}
				}
			}
		}

		return doorComponents;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
