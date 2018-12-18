// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/fire/DecontaminatePersonAction.h"
#include "em5/action/fire/PullPersonAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/ai/MovementModes.h"
#include "em5/audio/AudioHelper.h"
#include "em5/command/CommandContext.h"
#include "em5/command/fire/DecontaminatePersonCommand.h"
#include "em5/component/effect/FadeEffectComponent.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/component/movement/MoveToDespawnComponent.h"
#include "em5/component/vehicle/VehicleComponent.h"
#include "em5/game/Game.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/game/selection/SelectionManager.h"
#include "em5/health/HealthComponent.h"
#include "em5/logic/local/MiniMapIconRegistrationLogic.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/specs/ActionRangeSpecsGroup.h"
#include "em5/EM5Helper.h"

#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/QsfGameHelper.h>

#include <qsf_ai/navigation/NavigationComponent.h>
#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>
#include <qsf_ai/navigation/em4Router/RouterComponent.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/component/move/MovableComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/action/WaitAction.h>
#include <qsf/map/Map.h>
#include <qsf/math/Random.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/physics/collision/BulletBoxCollisionComponent.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Anonymous namespace                                   ]
//[-------------------------------------------------------]
namespace
{
	em5::DoorComponent::DoorType DECONTAMIMATION_DOOR = em5::DoorComponent::DOORTYPE_CODRIVER;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier DecontaminatePersonAction::ACTION_ID = "em5::DecontaminatePersonAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	DecontaminatePersonAction::DecontaminatePersonAction() :
		Action(ACTION_ID),
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mCurrentState(STATE_INIT),
		mFadeOutTime(qsf::Time::fromSeconds(1.2f)),
		mFadeInTime(qsf::Time::fromSeconds(1.2f))
	{
		// Nothing here
	}

	DecontaminatePersonAction::~DecontaminatePersonAction()
	{
		// Remove reservation again
		ReserveLogic::deleteOwnReservation(getMap(), mTargetEntityId, getEntityId());
	}

	void DecontaminatePersonAction::init(const qsf::Entity& targetEntity, const qsf::Time& decontaminationTime)
	{
		mTargetEntityId = targetEntity.getId();
		mDecontaminationTime = decontaminationTime;
		if (mDecontaminationTime.getSeconds() < 0.1f)	// Prevent a div/0 later on
			mDecontaminationTime = qsf::Time::fromSeconds(0.1f);
	}

	uint64 DecontaminatePersonAction::getTargetEntityId() const
	{
		return mTargetEntityId;
	}

	uint64 DecontaminatePersonAction::getContaminatedEntityId() const
	{
		return mContaminatedEntity.valid() ? mContaminatedEntity->getId() : qsf::getUninitialized<uint64>();
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void DecontaminatePersonAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetEntityId);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool DecontaminatePersonAction::onStartup()
	{
		// Get the caller and target entity
		qsf::Entity& callerEntity = getEntity();
		mTargetEntity = callerEntity.getMap().getEntityById(mTargetEntityId);
		if (!mTargetEntity.valid())
			return false;

		{ // Build context and check command
			CommandContext commandContext;
			commandContext.mCaller = mTargetEntity.get();	// Careful caller and target are switched here (because the command is for the unit, while the action is for the vehicle)
			commandContext.mTargetEntity = &callerEntity;

			// Call "checkCaller" && "checkContext" of the commando for validation
			DecontaminatePersonCommand* command = static_cast<DecontaminatePersonCommand*>(QSFGAME_COMMAND.getCommandManager().getById<DecontaminatePersonCommand>(DecontaminatePersonCommand::PLUGINABLE_ID));
			QSF_CHECK(nullptr != command, "DecontaminatePersonAction::onStartup(): Could not find DecontaminatePersonCommand instance", return false);
			if (!(command->checkCaller(*commandContext.mCaller) && command->checkContext(commandContext)))
				return false;
		}

		{	// Make the distance check
			// TODO(mh): use her another action range
			const float maxDistance = ActionRangeSpecsGroup::getInstanceSafe().getPullPersonDistance();
			const float currentDistance = qsf::game::DistanceHelper::getDistance(*mTargetEntity, callerEntity);
			if (currentDistance > maxDistance)
				return false;
		}

		if (!ReserveLogic::checkReservation(getEntity(), mTargetEntityId))
			return false;

		// Reservate the entity
		ReserveLogic::createReservation<ReserveDecontaminatePersonLogic>(getEntity(), mTargetEntityId);

		// Get the contaminated target, is carried by the target (firefighter)
		uint64 contaminatedEntityId = EntityHelper(*mTargetEntity).getCarryTarget(static_cast<EntityHelper::CarryLoad>(EntityHelper::CARRY_PERSON | EntityHelper::PULL_PERSON));
		mContaminatedEntity = mTargetEntity->getMap().getEntityById(contaminatedEntityId);

		return true;
	}

	void DecontaminatePersonAction::onShutdown()
	{
		ReserveLogic::deleteOwnReservation(getEntity(), mTargetEntityId);
	}

	qsf::action::Result DecontaminatePersonAction::updateAction(const qsf::Clock& clock)
	{
		// Get the target entity instance
		QSF_CHECK(mTargetEntity.valid(), "DecontaminatePersonAction::updateAction() was unable to retrieve the target entity instance", return qsf::action::RESULT_DONE);

		VehicleComponent* vehicleComponent = getEntity().getComponent<VehicleComponent>();
		if (nullptr == vehicleComponent)
			return qsf::action::RESULT_DONE;

		if (!mContaminatedEntity.valid())
			return qsf::action::RESULT_DONE;

		switch (mCurrentState)
		{
			case STATE_INIT:
			{
				vehicleComponent->openDoor(DECONTAMIMATION_DOOR, true);

				// Block the actions from the firefighter
				qsf::ActionComponent& actionComponentUnit = mTargetEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
				actionComponentUnit.clearPlan();
				actionComponentUnit.pushAction<qsf::WaitAction>(action::BLOCKING).init(mDecontaminationTime);	// Time should be a bit longer as there's also fading, but user can't do anything with unit as it's invisible anyway by then so should not matter.

				// Remove minimap icon
				EM5_GAME.getMiniMapIconRegistrationLogic().unregisterIcon(mContaminatedEntity.getSafe().getId());

				mCurrentState = STATE_OPEN_DOOR1;

				// Immediately go to the next state; no break by intent
			}

			case STATE_OPEN_DOOR1:
			{
				if (!vehicleComponent->areAllDoorsOfTypeInState(DECONTAMIMATION_DOOR, DoorComponent::DOOR_OPEN))
				{
					// Wait for ending of the animation
					return qsf::action::RESULT_CONTINUE;
				}

				// Fade-out squad
				mTargetEntity->getOrCreateComponentSafe<FadeEffectComponent>().fadeOut(mFadeOutTime);
				EM5_GAME.getSelectionManager().removeIdFromSelection(mTargetEntity->getId());
				QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_CHANGE_SELECTION));

				// Fade-out contaminated
				mContaminatedEntity->getOrCreateComponentSafe<FadeEffectComponent>().fadeOut(mFadeOutTime);

				vehicleComponent->openDoor(DECONTAMIMATION_DOOR, false);
				AudioHelper::playDecontaminatePerson(mAudioProxy, getEntity());
				mAudioProxy.synchronizeWithMultiplay();

				mCurrentState = STATE_CLOSE_DOOR1;

				// Immediately go to the next state; no break by intent
			}

			case STATE_CLOSE_DOOR1:
			{
				if (!vehicleComponent->areAllDoorsOfTypeInState(DECONTAMIMATION_DOOR, DoorComponent::DOOR_CLOSED))
				{
					// Wait for ending of the animation
					return qsf::action::RESULT_CONTINUE;
				}

				mDecontaminationTimeRest = mDecontaminationTime;
				mCurrentState = STATE_DECONTAMINATION;

				// Immediately go to the next state; no break by intent
			}

			case STATE_DECONTAMINATION:
			{
				// Wait for decontamination to finish and update an overlay bar for the timer
				mDecontaminationTimeRest -= clock.getTimePassed();
				if (mDecontaminationTimeRest.getSeconds() > 0.0f)
				{
					StatusOverlayComponent::showBarOverlay(getEntity(), StatusOverlayComponent::BAR_TYPE_PROGRESS, 1.0f - (mDecontaminationTimeRest.getSeconds() / mDecontaminationTime.getSeconds()));
					return qsf::action::RESULT_CONTINUE;
				}
				StatusOverlayComponent::hideBarOverlay(getEntity(), StatusOverlayComponent::BAR_TYPE_PROGRESS);

				// Turn the squad and the contaminated (just looks better if they come out the other way round)
				qsf::TransformComponent& transformComponent = mTargetEntity->getOrCreateComponentSafe<qsf::TransformComponent>();
				transformComponent.setRotation(transformComponent.getRotation() * glm::quat(0.0f, 0.0f, 1.0f, 0.0f));	// 180° rotation

				// Decontaminate the person (must be done here already to ensure contamination icon is gone)
				HealthComponent* healthComponent = mContaminatedEntity->getComponent<HealthComponent>();
				if (nullptr != healthComponent)
				{
					// Register the player as "has interacted with" event object
					EventIdComponent::registerPlayer(*mContaminatedEntity, getEntity());
					healthComponent->decontaminate();

					if (healthComponent->isInjured())
					{
						// Move unit further away from the car when an injured is lying (would be below the car now otherwise)
						qsf::TransformComponent& transformComponentVehicle = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();
						const glm::vec3 awayVec(glm::normalize(transformComponent.getPosition() - transformComponentVehicle.getPosition()) * 2.0f);
						transformComponent.setPosition(transformComponent.getPosition() + awayVec);
					}
					else
					{
						// Make healthy persons wait a little or they start walking away already while half invisible
						qsf::ActionComponent& contaminatedActionComponent = mContaminatedEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
						contaminatedActionComponent.pushAction<qsf::WaitAction>(action::BLOCKING).init(qsf::Time::fromSeconds(3.0f));
					}
				}

				mAudioProxy.stop();
				mAudioProxy.synchronizeWithMultiplay();
				vehicleComponent->openDoor(DECONTAMIMATION_DOOR, true);

				mCurrentState = STATE_OPEN_DOOR2;

				// Immediately go to the next state; no break by intent
			}

			case STATE_OPEN_DOOR2:
			{
				if (!vehicleComponent->areAllDoorsOfTypeInState(DECONTAMIMATION_DOOR, DoorComponent::DOOR_OPEN))
				{
					// Wait for ending of the animation
					return qsf::action::RESULT_CONTINUE;
				}

				// Fade-in squad
				mTargetEntity->getOrCreateComponentSafe<FadeEffectComponent>().fadeIn(mFadeInTime);

				// Fade-in carried
				mContaminatedEntity->getOrCreateComponentSafe<FadeEffectComponent>().fadeIn(mFadeInTime);

				mCurrentState = STATE_LEAVE_VEHICLE;

				// Immediately go to the next state; no break by intent
			}

			case STATE_LEAVE_VEHICLE:
			{
				// Wait for fade-in to complete
				if (mTargetEntity->getComponent<FadeEffectComponent>() != nullptr)
					return qsf::action::RESULT_CONTINUE;

				vehicleComponent->openDoor(DECONTAMIMATION_DOOR, false);

				// Emit message to decide whether or not to let the victim go
				bool letGoAfterwards = true;
				QSF_MESSAGE.emitMessageWithResponse(qsf::MessageConfiguration(Messages::EM5_PERSON_EXITS_AFTER_DECONTAMINATION, mContaminatedEntity->getId()), letGoAfterwards);

				HealthComponent* healthComponent = mContaminatedEntity->getComponent<HealthComponent>();
				if (nullptr != healthComponent && healthComponent->isInjured())
				{
					qsf::ActionComponent& actionComponentUnit = mTargetEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
					// No need to start pulling again here, as pulling is actually still active

					// Unit must move the injured a little bit away
					const qsf::logic::TargetPoint targetPoint(findPositionForInjured(), false, 0.5f, 0.5f);
					actionComponentUnit.pushAction<MoveAction>(action::COMMAND_STD).init(new qsf::ai::ReachSinglePointGoal(targetPoint), MovementModes::MOVEMENT_MODE_WALK_BACKWARDS);

					if (letGoAfterwards)
					{
						actionComponentUnit.pushAction<PullPersonAction>(getPriority()).init(mContaminatedEntity->getId(), PullPersonAction::STOP);
					}
				}
				else
				{
					if (letGoAfterwards)
					{
						// Reactivate the navigation component
						qsf::ai::NavigationComponent* navigationComponent = mContaminatedEntity->getComponent<qsf::ai::NavigationComponent>();
						if (nullptr != navigationComponent)
						{
							navigationComponent->setActive(true);
						}

						qsf::MovableComponent* movableComponent = mContaminatedEntity->getComponent<qsf::MovableComponent>();
						if (movableComponent != nullptr)
						{
							movableComponent->setActive(true);
						}

						if (!EntityHelper(mContaminatedEntity.getSafe()).isCommandable())
						{
							// Non-injured civil persons should always start moving (react to "Messages::EM5_PERSON_EXITED_AFTER_DECONTAMINATION" if you need to un-do this, e.g. ME310)
							mContaminatedEntity->getOrCreateComponentSafe<MoveToDespawnComponent>().setActive(true);
						}

						AnimationHelper(mTargetEntity.getSafe()).clearAnimation();
						AnimationHelper(mContaminatedEntity.getSafe()).clearAnimation();

						// Unlink decontaminated from unit
						qsf::LinkComponent* linkComponent = mContaminatedEntity->getComponent<qsf::LinkComponent>();
						if (linkComponent != nullptr)
						{
							linkComponent->unlinkFromParent();
						}

						// Remove carry flag
						EntityHelper targetEntityHelper(*mTargetEntity);
						targetEntityHelper.clearCarryTarget(static_cast<EntityHelper::CarryLoad>(EntityHelper::CARRY_PERSON | EntityHelper::PULL_PERSON));

						// Show equipment again (in case the fireman has saved something in his inventory before guiding the contaminated)
						targetEntityHelper.storeInventoryEquipmentAsActive();

						// Reactivate the bullet box collision component
						qsf::BulletBoxCollisionComponent* bulletBoxCollisionComponent = mContaminatedEntity->getComponent<qsf::BulletBoxCollisionComponent>();
						if (nullptr != bulletBoxCollisionComponent)
						{
							bulletBoxCollisionComponent->setActive(true);
						}

						// To be sure we reset our router component
						qsf::ai::RouterComponent& routerComponent = mContaminatedEntity->getOrCreateComponentSafe<qsf::ai::RouterComponent>();
						routerComponent.recreateCollisionBox();
						routerComponent.setActive(true);
					}
				}

				// Emit message just when action is done
				QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_PERSON_EXITED_AFTER_DECONTAMINATION, mContaminatedEntity->getId()));
				break;
			}
		}

		// Finish action
		return qsf::action::RESULT_DONE;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	glm::vec3 DecontaminatePersonAction::findPositionForInjured() const
	{
		// Find some place where would like to put an injured and decontaminated person
		const glm::vec3 offsetLocal(qsf::Random::getRandomFloat(-3.0f, 3.0f), 0.0f, 8.0f + qsf::Random::getRandomFloat(0.0f, 3.0f));	// A few meters behind the vehicle (TODO(mz) using some random values until findFreePosition is available to avoid placing them all over each other)

		const qsf::TransformComponent& transformComponent = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();
		const glm::vec3 targetPosition(transformComponent.getPosition() + transformComponent.getTransform().vec3DirectionLocalToWorld(offsetLocal));

		// TODO(mz): check if this position is fine (wait until AI has functions for that)

		return targetPosition;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
