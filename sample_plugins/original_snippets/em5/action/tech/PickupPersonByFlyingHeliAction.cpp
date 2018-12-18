// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/tech/PickupPersonByFlyingHeliAction.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/base/ContainerCategory.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/command/tech/PickupPersonByFlyingHeliCommand.h"
#include "em5/command/CommandContext.h"
#include "em5/component/effect/FadeEffectComponent.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/component/vehicle/parts/HelicopterHatchComponent.h"
#include "em5/component/vehicle/HelicopterComponent.h"
#include "em5/component/person/PersonPlacementComponent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/health/HealthComponent.h"
#include "em5/specs/ActionRangeSpecsGroup.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/audio/AudioHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Messages.h"

#include <qsf_ai/navigation/NavigationComponent.h>

#include <qsf_game/command/CommandManager.h>
#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/component/base/BoneLinkComponent.h>
#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/QsfGameHelper.h>

#include <qsf/message/MessageSystem.h>
#include <qsf/logic/action/WaitAction.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/move/MovableComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/Map.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier PickupPersonByFlyingHeliAction::ACTION_ID = "em5::PickupPersonByFlyingHeliAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PickupPersonByFlyingHeliAction::PickupPersonByFlyingHeliAction() :
		Action(ACTION_ID),
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mState(STATE_INIT),
		mIsTargetInjured(false)
	{
		// Nothing to do in here
	}

	PickupPersonByFlyingHeliAction::~PickupPersonByFlyingHeliAction()
	{
		// Remove reservation again
		if (mTargetEntity.valid())
		{
			// Even if the person is injured and not in the vehicle, its ok to delete the reservation (can not be target of something else)
			ReserveLogic::deleteOwnReservation(mTargetEntity, getEntityId());

			// Make entity movable again
			qsf::ai::NavigationComponent* navigationComponent = mTargetEntity->getComponent<qsf::ai::NavigationComponent>();
			if (nullptr != navigationComponent)
			{
				navigationComponent->setActive(true);
			}
		}
	}

	void PickupPersonByFlyingHeliAction::init(const qsf::Entity& targetEntity)
	{
		mTargetEntityId = targetEntity.getId();
	}

	uint64 PickupPersonByFlyingHeliAction::getTargetId() const
	{
		return mTargetEntityId;
	}

	bool PickupPersonByFlyingHeliAction::isStrecherWaitForLoading()
	{
		return (mState == STATE_WAIT_FOR_LOADING);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void PickupPersonByFlyingHeliAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetEntityId);
		serializer.serializeAs<uint8>(mState);
		serializer.serialize(mIsTargetInjured);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool PickupPersonByFlyingHeliAction::onStartup()
	{
		// Get the target entity
		const qsf::Map& map = getEntity().getMap();
		mTargetEntity = map.getEntityById(mTargetEntityId);
		if (!mTargetEntity.valid())
			return false;

		if (!checkActionCondition(mTargetEntity.getSafe()))
			return false;

		// Check if entity is right and get the helicopter hatch component
		HelicopterComponent* helicopterComponent = getEntity().getComponent<HelicopterComponent>();
		if (nullptr == helicopterComponent)
			return false;

		qsf::Entity* specialEntity = map.getEntityById(helicopterComponent->getSpecialEntity());
		if (nullptr == specialEntity)
			return false;

		mHelicopterHatchComponent = specialEntity->getComponent<HelicopterHatchComponent>();
		if (!mHelicopterHatchComponent.valid())
			return false;

		// Try to get event ID component lock
		if (!EventIdComponent::tryLockForTeam(mTargetEntity.getSafe(), getEntity()))
		{
			return false;
		}

		return true;
	}

	qsf::action::Result PickupPersonByFlyingHeliAction::updateAction(const qsf::Clock&)
	{
		// Check for components
		if (!mTargetEntity.valid() || !mHelicopterHatchComponent.valid())
			return qsf::action::RESULT_DONE;

		HelicopterComponent* helicopterComponent = getEntity().getComponent<HelicopterComponent>();
		if (nullptr == helicopterComponent)
			return qsf::action::RESULT_DONE;

		switch (mState)
		{
			case STATE_INIT:
			{
				// Reservate the entity
				ReserveLogic::createReservation<ReserveEnterVehicleLogic>(mTargetEntity.getSafe(), getEntityId());

				EntityHelper entityHelper(mTargetEntity.getSafe());
				entityHelper.makeImmovable();
				mIsTargetInjured = entityHelper.isPersonInjured();

				mHelicopterHatchComponent->openHatch(mTargetEntity.getSafe(), mIsTargetInjured);

				if (!mIsTargetInjured)
				{
					// Prevent extra damage for the entity and reset the injury icon (to avoid showing fire for this entity)
					HealthComponent* healthComponent = mTargetEntity->getComponent<HealthComponent>();
					if (nullptr != healthComponent)
					{
						healthComponent->setInvincible(true);
						healthComponent->updateInjuredIcon();
					}

					// Push animation
					qsf::ActionComponent& actionComponent = mTargetEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
					actionComponent.pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::REPLACE_WHOLE_PLAN).init(AnimationHelper(mTargetEntity.getSafe()).getAnimationLookingUp());
				}

				// Play rope install and move sound
				AudioHelper::playHelicopterRopeMove(mAudioProxy, getEntity());
				mAudioProxy.synchronizeWithMultiplay();

				mState = STATE_LOWER_STRETCHER;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_LOWER_STRETCHER:
			{
				if (mHelicopterHatchComponent->isLowerStretcher())
				{
					// Wait till the stretcher is on ground
					return qsf::action::RESULT_CONTINUE;
				}

				// Stop swing to avoid problems with the animations
				helicopterComponent->stopSwing();

				if (mIsTargetInjured)
				{
					mTargetEntity->getOrCreateComponentSafe<FadeEffectComponent>().fadeOut();

					// Push wait action
					getComponent().pushAction<qsf::WaitAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(qsf::Time::fromSeconds(1.f));
				}
				else
				{
					// Push animation
					qsf::ActionComponent& actionComponent = mTargetEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
					actionComponent.pushAction<PlayAnimationAction>(action::BLOCKING).init(AnimationHelper(mTargetEntity.getSafe()).getAnimationRescueWithHeli01());

					// Here we have to disable the person animation controller, otherwise the animation is "overwritten" by the controller
					PersonComponent* personComponent = mTargetEntity->getComponent<PersonComponent>();
					if (nullptr != personComponent)
					{
						personComponent->preventUpdateAnimations(true);
					}
				}

				AudioHelper::playHelicopterRopeAttach(mAudioProxy, getEntity());
				mAudioProxy.synchronizeWithMultiplay();

				mState = STATE_WAIT_FOR_LOADING;
				return qsf::action::RESULT_CONTINUE;
			}
			case STATE_WAIT_FOR_LOADING:
			{
				if (AnimationHelper(mTargetEntity.getSafe()).isAnimationPlaying(AnimationHelper(mTargetEntity.getSafe()).getAnimationRescueWithHeli01()))
				{
					// Wait till load time is over
					return qsf::action::RESULT_CONTINUE;
				}

				// Pass injured flag, to indicate if the stretcher has to go in the helicopter or not
				mHelicopterHatchComponent->placeTargetOnStretcher();
				mHelicopterHatchComponent->raiseStretcher(mIsTargetInjured);

				if (mIsTargetInjured)
				{
					// Push an transport animation for the non injured person
					mTargetEntity->getOrCreateComponentSafe<FadeEffectComponent>().fadeIn();
				}

				AudioHelper::playHelicopterRopeMove(mAudioProxy, getEntity());
				mAudioProxy.synchronizeWithMultiplay();

				mState = STATE_RAISE_STRETCHER;
				return qsf::action::RESULT_CONTINUE;
			}
			case STATE_RAISE_STRETCHER:
			{
				if (mHelicopterHatchComponent->isRaiseStretcher())
				{
					// Wait till the stretcher is on the top
					return qsf::action::RESULT_CONTINUE;
				}

				if (!mIsTargetInjured)
				{
					// Reactivate the person animations (its important closely after we exit the vehicle again)
					PersonComponent* personComponent = mTargetEntity->getComponent<PersonComponent>();
					if (nullptr != personComponent)
					{
						personComponent->preventUpdateAnimations(false);
					}

					// Reactivate the health component
					HealthComponent* healthComponent = mTargetEntity->getComponent<HealthComponent>();
					if (nullptr != healthComponent)
					{
						healthComponent->setInvincible(false);
						healthComponent->updateInjuredIcon();
					}

					putPersonInVehicleAndSendMessage();
				}

				// Start swing again, no we switch back to "normal" helicopter behavior
				helicopterComponent->startSwing();

				mAudioProxy.stop();
				mAudioProxy.synchronizeWithMultiplay();

				mState = STATE_FINISH;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_FINISH:
				// Nothing to do in this case
				break;
		}

		return qsf::action::RESULT_DONE;
	}

	void PickupPersonByFlyingHeliAction::onShutdown()
	{
		if (mTargetEntity.valid())
		{
			EventIdComponent::releaseTeamLock(mTargetEntity.getSafe(), getEntity());
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	bool PickupPersonByFlyingHeliAction::checkActionCondition(qsf::Entity& targetEntity)
	{
		// Get the caller and target entity
		qsf::Entity& callerEntity = getEntity();

		// Build context
		CommandContext commandContext;
		commandContext.mCaller = &callerEntity;
		commandContext.mTargetEntity = &targetEntity;

		// Call "checkCaller" && "checkContext" of the command for validation
		PickupPersonByFlyingHeliCommand* pickupPersonByFlyingHeliCommand = static_cast<PickupPersonByFlyingHeliCommand*>(QSFGAME_COMMAND.getCommandManager().getById<PickupPersonByFlyingHeliCommand>(PickupPersonByFlyingHeliCommand::PLUGINABLE_ID));
		QSF_CHECK(nullptr != pickupPersonByFlyingHeliCommand, "PickupPersonByFylingHeliAction::onStartup(): Could not pickup person by flying helicopter command instance", return false);
		if (!(pickupPersonByFlyingHeliCommand->checkCallerWithoutPriority(*commandContext.mCaller) && pickupPersonByFlyingHeliCommand->checkContext(commandContext)))
			return false;

		// Make the distance check
		{
			const qsf::TransformComponent* transformComponentTarget = targetEntity.getComponent<qsf::TransformComponent>();
			glm::vec3 callerPositionWithoutHeight = transformComponentTarget->getPosition();
			callerPositionWithoutHeight.y = 0;

			const qsf::TransformComponent* transformComponentCaller = callerEntity.getComponent<qsf::TransformComponent>();
			glm::vec3 targetPositionWithoutHeight = transformComponentCaller->getPosition();
			targetPositionWithoutHeight.y = 0;

			float maxDistance = ActionRangeSpecsGroup::getInstanceSafe().getEnterVehicleDistance();
			const float currentDistance = qsf::game::DistanceHelper::getDistance(targetPositionWithoutHeight, callerPositionWithoutHeight);
			if (currentDistance > (maxDistance))
				return false;
		}

		// Check for the reserve logic
		if (!ReserveLogic::checkReservation(targetEntity, getEntityId()))
			return false;

		// Every check is valid, return true
		return true;
	}

	void PickupPersonByFlyingHeliAction::placeTargetOnStretcher()
	{
		qsf::Entity* stretcherEntity = mHelicopterHatchComponent->getStretcherEntity();

		qsf::LinkComponent& linkComponent = mTargetEntity->getOrCreateComponentSafe<qsf::LinkComponent>();
		linkComponent.setParentId(stretcherEntity->getId());
		linkComponent.setParentLinkType(qsf::LinkComponent::HARD_LINK);

		// Add bone link
		qsf::game::BoneLinkComponent& bonelinkComponent = mTargetEntity->getOrCreateComponentSafe<qsf::game::BoneLinkComponent>();
		bonelinkComponent.setBoneName(equipment::EQUIPMENT_BONE_TECH_STRETCHER);

		// Start animation
		qsf::ActionComponent& actionComponent = mTargetEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
		actionComponent.pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(AnimationHelper(mTargetEntity.getSafe()).getAnimationVictimOnStretcher(), false, true);
	}

	void PickupPersonByFlyingHeliAction::putPersonInVehicleAndSendMessage()
	{
		EntityHelper(mTargetEntity.getSafe()).enterContainer(getEntity(), container::CONTAINERTYPE_PASSENGER);

		// For having persons on rooftops, ships or other special locations we need to delete the person placement component.
		// After the helicopter lift them up, we can create an new person placement (if necessary)
		mTargetEntity->getOrCreateComponent<PersonPlacementComponent>();

		// Send Message for rescued person
		qsf::MessageParameters parameters;
		parameters.setParameter("IsInjured", mIsTargetInjured);
		parameters.setParameter("EntityId", mTargetEntityId);
		QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_PICKUP_PERSON_WITH_RESCUEHELICOPTER, mTargetEntityId, getEntityId()), parameters);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
