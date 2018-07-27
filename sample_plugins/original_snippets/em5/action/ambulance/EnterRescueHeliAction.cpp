// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/ambulance/EnterRescueHeliAction.h"
#include "em5/action/base/PlacePersonAction.h"
#include "em5/action/move/LinearMoveAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/base/ContainerCategory.h"
#include "em5/component/vehicle/HelicopterComponent.h"
#include "em5/component/overlay/StatusOverlayComponent.h"
#include "em5/command/move/ReturnToBaseCommand.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/game/targetpoint/EnterRescueHeliTargetPointProvider.h"
#include "em5/health/HealthHelper.h"
#include "em5/logic/local/base/CarryPersonLogic.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Messages.h"

#include <qsf_ai/navigation/NavigationComponent.h>

#include <qsf_game/component/base/BoneToBoneLinkComponent.h>
#include <qsf_game/component/base/HiddenComponent.h>

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/map/Entity.h>
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
	const qsf::NamedIdentifier EnterRescueHeliAction::ACTION_ID = "em5::EnterRescueHeliAction";

	const qsf::Time TIME_DOORS_CLOSED = qsf::Time::fromSeconds(6.0f);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	EnterRescueHeliAction::EnterRescueHeliAction() :
		ExitRescueHeliAction(ACTION_ID),
		mState(STATE_INIT),
		mTimeTillDoorsClosed(TIME_DOORS_CLOSED)
	{
		// Nothing here
	}

	EnterRescueHeliAction::~EnterRescueHeliAction()
	{
		deleteOpticalEntities();

		// Remove reservation again
		ReserveLogic::deleteOwnReservation(mTargetEntity, getEntityId());
	}

	void EnterRescueHeliAction::init(qsf::Entity& targetEntity)
	{
		mTargetEntity = targetEntity;

		// To avoid multiple actors at the same target, we reserve the target here.
		// It is only checked in command for automatism. If the doctor starts treatment, the logic is changed. Only reserve if no other reserve the target (its not bad if a moveTo reservation is missing)
		// Reserve the entity
		if (ReserveLogic::checkReservation(targetEntity, getEntityId()))
		{
			ReserveLogic::createReservation<ReserveMoveToTargetLogic>(targetEntity, getEntityId());
		}
	}

	uint64 EnterRescueHeliAction::getTargetId() const
	{
		return (mTargetEntity.valid() ? mTargetEntity->getId() : qsf::getUninitialized<uint64>());
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void EnterRescueHeliAction::serialize(qsf::BinarySerializer& serializer)
	{
		// TODO(mk) Add serialize function
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool EnterRescueHeliAction::onStartup()
	{
		if (!mTargetEntity.valid())	// Need to call init() before
			return false;

		mHelicopterComponent = mTargetEntity->getComponent<HelicopterComponent>();
		if (!mHelicopterComponent.valid())
			return false;

		if (mHelicopterComponent->isFlying())
			return false;

		return true;
	}

	qsf::action::Result EnterRescueHeliAction::updateAction(const qsf::Clock& clock)
	{
		if (!mTargetEntity.valid())
			return qsf::action::RESULT_DONE;

		mTimeTillDoorsClosed -= clock.getTimePassed();

		switch (mState)
		{
			case STATE_INIT:
			{
				// Change reserve logic from "moveto" to "entervehicle" (has higher priority)
				ReserveLogic::deleteOwnReservation(mTargetEntity, getEntityId());
				ReserveLogic::createReservation<ReserveEnterVehicleLogic>(mTargetEntity.getSafe(), getEntityId());

				const qsf::Transform& helicopterTransform = mTargetEntity.getSafe().getOrCreateComponentSafe<qsf::TransformComponent>().getTransform();
				glm::vec3 targetEnterPosition = helicopterTransform.vec3PositionLocalToWorld(EnterRescueHeliTargetPointProvider::FINAL_PARAMEDIC_OFFSET);
				glm::quat targetEnterRotation = helicopterTransform.quatRotationLocalToWorld(EnterRescueHeliTargetPointProvider::FINAL_PARAMEDIC_ROTATION);

				getComponent().pushAction<LinearMoveAction>(getPriority(), qsf::action::INSERT_BEFORE_CURRENT).init(targetEnterPosition, 0.5f, 10.0f);
				getComponent().pushAction<TurnToAction>(getPriority(), qsf::action::INSERT_BEFORE_CURRENT).initWithRotation(targetEnterRotation, 135.0f);

				mState = STATE_ENTER;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_ENTER:
			{
				const glm::vec3& paramedicPosition = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>().getPosition();
				const qsf::Transform& helicopterTransform = mTargetEntity.getSafe().getOrCreateComponentSafe<qsf::TransformComponent>().getTransform();
				glm::vec3 targetEnterPosition = helicopterTransform.vec3PositionLocalToWorld(EnterRescueHeliTargetPointProvider::FINAL_PARAMEDIC_OFFSET);

				// If the distance is still more then one meter from enter position we stop the action
				if (glm::distance2(paramedicPosition, targetEnterPosition) > 1.0f)
					return qsf::action::RESULT_DONE;

				createOpticalEntities(mTargetEntity.getSafe(), true);
				prepareParamedicForEnter();
				mHelicopterComponent->openDoor(DoorComponent::DOORTYPE_TRUNK, true);

				mState = STATE_CLOSE_TRUNK_DOOR;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_CLOSE_TRUNK_DOOR:
			{
				if (mTimeTillDoorsClosed > qsf::Time::ZERO)
				{
					// Wait till the door can closed
					return qsf::action::RESULT_CONTINUE;
				}

				mHelicopterComponent->openDoor(DoorComponent::DOORTYPE_TRUNK, false);
				mHelicopterComponent->openDoor(DoorComponent::DOORTYPE_PASSENGER, true);

				mState = STATE_UPDATE;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_UPDATE:
			{
				if ((mParamedicBackEntity.valid() && AnimationHelper(mParamedicBackEntity.getSafe()).isAnimationPlaying()) ||
					(mParamedicFrontEntity.valid() && AnimationHelper(mParamedicFrontEntity.getSafe()).isAnimationPlaying()) ||
					(mStretcherEntity.valid() && AnimationHelper(mStretcherEntity.getSafe()).isAnimationPlaying()))
				{
					// Wait till the animation is finished
					return qsf::action::RESULT_CONTINUE;
				}

				mHelicopterComponent->openDoor(DoorComponent::DOORTYPE_PASSENGER, false);

				// In case we loaded and patient, send message and if vehicle ready to return to base
				if (mPatientEntity.valid())
				{
					EntityHelper(mPatientEntity.getSafe()).enterContainer(mTargetEntity.getSafe(), container::CONTAINERTYPE_PASSENGER);

					// Emit message "Person placed in ambulance"
					qsf::MessageParameters parameters;
					parameters.setParameter("personId", mPatientEntity->getId());
					parameters.setParameter("vehicleId", mTargetEntity->getId());
					parameters.setParameter("actorId", getEntityId());
					QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_PERSON_PLACED_IN_AMBULANCE, mPatientEntity->getId()), parameters);

					// The carried is already in the target vehicle, check if the vehicle is now full, and can drive back home
					const bool sendVehicleBackToBase = !EntityHelper(mPatientEntity.getSafe()).checkVehicleHasFreeSeats(mTargetEntity.getSafe());
					if (sendVehicleBackToBase)
					{
						PlacePersonAction::lookForIdleMedicsInArea(*mTargetEntity);
						ReturnToBaseCommand::pushReturnToBaseAction(*mTargetEntity.get(), action::Priority::COMMAND_STD);
					}
				}
				deleteOpticalEntities();

				mState = STATE_CLOSE_PASSENGER_DOOR;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_CLOSE_PASSENGER_DOOR:
			{
				if (!mHelicopterComponent->areAllDoorsOfTypeInState(DoorComponent::DOORTYPE_PASSENGER, DoorComponent::DOOR_CLOSED))
				{
					// Wait till all the doors are closed
					return qsf::action::RESULT_CONTINUE;
				}

				// Delete the carried person from the logic
				CarryPersonLogic* carryPersonLogic = EntityHelper(getEntity()).getGameLogic<CarryPersonLogic>();
				if (nullptr != carryPersonLogic)
				{
					carryPersonLogic->setCarryPerson(qsf::getUninitialized<uint64>());
				}

				mState = STATE_DONE;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_DONE:
			{
				break;
			}
		}

		return qsf::action::RESULT_DONE;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void EnterRescueHeliAction::prepareParamedicForEnter()
	{
		// Get the carried person
		EntityHelper entityHelper(getEntity());
		mPatientEntity = getMap().getEntityById(entityHelper.getCarryTarget(EntityHelper::CARRY_PERSON));
		if (mPatientEntity.valid())
		{
			// Link the carried person before entering the container with the parent
			qsf::LinkComponent* linkComponent = mPatientEntity->getComponent<qsf::LinkComponent>();
			if (nullptr != linkComponent)
			{
				// In case the patient has linked childs (e.g. arm splints), don't remove them. Only the parent link.
				linkComponent->setParentId(qsf::getUninitialized<uint64>());
			}

			mPatientEntity->destroyComponent<qsf::game::BoneToBoneLinkComponent>();
			mPatientEntity->destroyComponent<StatusOverlayComponent>();
			HealthHelper(mPatientEntity.getSafe()).linkToStretcher(mStretcherEntity.getSafe());
		}

		entityHelper.enterContainer(mTargetEntity.getSafe(), container::CONTAINERTYPE_SQUAD);
		qsf::game::HiddenComponent::setHidden(getEntity(), true);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
