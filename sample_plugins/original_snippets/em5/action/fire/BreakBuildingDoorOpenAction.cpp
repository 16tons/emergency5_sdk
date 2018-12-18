// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/fire/BreakBuildingDoorOpenAction.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/audio/AudioHelper.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/base/GameplayAssets.h"
#include "em5/command/CommandContext.h"
#include "em5/command/fire/BreakBuildingDoorOpenCommand.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/health/HealthHelper.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/specs/ActionRangeSpecsGroup.h"

#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/component/base/BoneLinkComponent.h>
#include <qsf_game/QsfGameHelper.h>

#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/Map.h>
#include <qsf/math/CoordinateSystem.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/renderer/particles/ParticlesComponent.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier BreakBuildingDoorOpenAction::ACTION_ID = "em5::BreakBuildingDoorOpenAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	BreakBuildingDoorOpenAction::BreakBuildingDoorOpenAction() :
		Action(ACTION_ID),
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mState(STATE_INIT)
	{
		// Nothing here
	}

	BreakBuildingDoorOpenAction::~BreakBuildingDoorOpenAction()
	{
		// Remove reservation again
		ReserveLogic::deleteOwnReservation(getMap(), mTargetEntityId, getEntityId());

		// Remove invincibility
		HealthHelper(getEntity()).setInvincible(false);
	}

	void BreakBuildingDoorOpenAction::init(const qsf::Entity& targetEntity)
	{
		mTargetEntityId = targetEntity.getId();

		// The full action should take ~10s
		mTime = qsf::Time::fromSeconds(10.0f);	// TODO(mk) Hard coded value - Read timing from setting/specs file
	}

	uint64 BreakBuildingDoorOpenAction::getTargetEntityId() const
	{
		return mTargetEntityId;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void BreakBuildingDoorOpenAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetEntityId);
		serializer.serializeAs<uint16>(mState);
		serializer.serialize(mTime);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool BreakBuildingDoorOpenAction::onStartup()
	{
		// Get the caller and target entity
		qsf::Entity& callerEntity = getEntity();
		qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
		if (nullptr == targetEntity)
			return false;

		// Get the building component
		mTargetBuildingComponent = targetEntity->getComponent<BuildingComponent>();
		if (!mTargetBuildingComponent.valid())
			return false;

		// Get main door entity
		const qsf::Entity* targetDoorEntity = getMap().getEntityById(mTargetBuildingComponent->getMainDoorId());
		if (nullptr == targetDoorEntity)
			return false;

		// Make the distance check
		const float maxDistance = ActionRangeSpecsGroup::getInstanceSafe().getEnterBuildingDistance();
		const float currentDistance = qsf::game::DistanceHelper::getDistance(mTargetBuildingComponent->getDoorPosition(), callerEntity);
		if (currentDistance > maxDistance)
			return false;

		// Build context
		CommandContext commandContext;
		commandContext.mCaller = &callerEntity;
		commandContext.mTargetEntity = targetEntity;

		// Call "checkCaller" && "checkContext" of the commando for validation
		BreakBuildingDoorOpenCommand* breakBuildingDoorOpenCommand = static_cast<BreakBuildingDoorOpenCommand*>(QSFGAME_COMMAND.getCommandManager().getById<BreakBuildingDoorOpenCommand>(BreakBuildingDoorOpenCommand::PLUGINABLE_ID));
		QSF_CHECK(nullptr != breakBuildingDoorOpenCommand, "BreakBuildingDoorOpenAction::onStartup(): Could not find break building door open command instance", return false);
		if (!(breakBuildingDoorOpenCommand->checkCallerWithoutPriority(*commandContext.mCaller) && breakBuildingDoorOpenCommand->checkContext(commandContext)))
			return false;

		// Check for the reserve logic
		if (!ReserveLogic::checkReservation(*targetEntity, getEntityId()))
			return false;

		// Reservate the entity
		// TODO(sw) Use proper reserve logic?
		ReserveLogic::createReservation<ReserveCutRoadVehicleLogic>(*targetEntity, getEntityId());

		// Try to get event ID component lock
		if (!EventIdComponent::tryLockForTeam(*targetEntity, getEntity()))
		{
			return false;
		}

		return true;
	}

	qsf::action::Result BreakBuildingDoorOpenAction::updateAction(const qsf::Clock& clock)
	{
		// Get the target entity
		const qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
		if (nullptr == targetEntity)
			return qsf::action::RESULT_DONE;

		if (!mTargetBuildingComponent.valid())
			return qsf::action::RESULT_DONE;

		if (!mTargetBuildingComponent->isContainingTrappedPersons())
		{
			// There are no trapped persons anymore in the building (e.g. already died)
			// Reset the animation of the caller
			AnimationHelper(getEntity()).clearAnimation();
			return qsf::action::RESULT_DONE;
		}

		if (mState != STATE_INIT && mTime > qsf::Time::ZERO)
		{
			mTime -= clock.getTimePassed();
			if (mTime < qsf::Time::ZERO)
			{
				mTime = qsf::Time::ZERO;
			}
		}

		switch (mState)
		{
			case STATE_INIT:
			{
				createBreakDoorEffect();

				// Make firefighter invincible while he doing this
				HealthHelper(getEntity()).setInvincible(true);

				teleportToBuildingDoor();

				// Play animation
				getComponent().pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(AnimationHelper(getEntity()).getAnimationFirefighterUseAxe1(), false);

				sendMessageStart();

				mState = STATE_ANIMATION_1;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_ANIMATION_1:
			{
				// TODO(fw): We should *really* directly use the MeshAnimationComponent and store a WeakPtr to its animation channel to check it
				AnimationHelper animationHelper(getEntity());
				if (animationHelper.isAnimationPlaying(animationHelper.getAnimationFirefighterUseAxe1()))
				{
					// Wait for ending of the animation or timeout reached
					return qsf::action::RESULT_CONTINUE;
				}

				playAxe2Animation();

				// The ace hits the door the first time -> start door particle
				mTargetBuildingComponent->setMainDoorDamaged(true);

				// Play audio
				AudioProxy audioProxy;
				AudioHelper::playFirefighterAxe(audioProxy, getEntity());
				audioProxy.detach();
				audioProxy.synchronizeWithMultiplay();

				mState = STATE_ANIMATION_2;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_ANIMATION_2:
			{
				// TODO(fw): We should *really* directly use the MeshAnimationComponent and store a WeakPtr to its animation channel to check it
				AnimationHelper animationHelper(getEntity());
				const bool animationPlaying = animationHelper.isAnimationPlaying(animationHelper.getAnimationFirefighterUseAxe2());
				const bool timeLeft = (mTime > qsf::Time::ZERO);
				if (timeLeft || animationPlaying)
				{
					if (timeLeft && !animationPlaying)
					{
						// Play audio
						AudioProxy audioProxy;
						AudioHelper::playFirefighterAxe(audioProxy, getEntity());
						audioProxy.detach();
						audioProxy.synchronizeWithMultiplay();

						// Timeout not reached -> play animation again
						playAxe2Animation();
					}

					// Wait for ending of the animation or timeout reached
					return qsf::action::RESULT_CONTINUE;
				}

				// Stop the particle effect
				if (mEffectEntity.valid())
				{
					qsf::ParticlesComponent* effectParticlesComponent = mEffectEntity->getOrCreateComponent<qsf::ParticlesComponent>();
					if (nullptr != effectParticlesComponent)
					{
						effectParticlesComponent->setActive(false);
					}
				}

				// Timeout reached play end animation
				getComponent().pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(AnimationHelper(getEntity()).getAnimationFirefighterUseAxe3(), false);

				mState = STATE_ANIMATION_3;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_ANIMATION_3:
			{
				// TODO(fw): We should *really* directly use the MeshAnimationComponent and store a WeakPtr to its animation channel to check it
				AnimationHelper animationHelper(getEntity());

				// Wait for ending of the animation
				if (animationHelper.isAnimationPlaying(animationHelper.getAnimationFirefighterUseAxe2()))
				{
					return qsf::action::RESULT_CONTINUE;
				}

				// Reset the animation of the caller
				animationHelper.clearAnimation();

				// Let the fireman move some steps away (currently 4 meters)
				// TODO(sw): AvoidThreatsGoal is currently only a dummy
				//getComponent().pushAction<MoveAction>(action::DANGER_IMMINENT).init(new qsf::ai::AvoidThreatsGoal(2.0f, 1));

				{ // TODO(sw) Dummy implementation until AvoidThreatsGoal works as intended
					qsf::TransformComponent& unitTransformComponent = getEntity().getComponentSafe<qsf::TransformComponent>();

					const glm::vec3 moveOffset = mTargetBuildingComponent->getDoorRotation() * qsf::CoordinateSystem::getIn() * 4.0f;
					qsf::logic::TargetPoint moveTargetConfig;
					moveTargetConfig.mPosition = unitTransformComponent.getPosition() + moveOffset;
					moveTargetConfig.mTolerance = 0.25f;
					getComponent().pushAction<MoveAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(new qsf::ai::ReachSinglePointGoal(moveTargetConfig));
				}

				mState = STATE_SET_PERSONS_FREE;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_SET_PERSONS_FREE:
			{
				sendMessageEnd();

				// Let the persons flee from the building
				mTargetBuildingComponent->untrapPersons();

				// The door is no problem anymore
				mTargetBuildingComponent->setIsDoorBlocked(false);

				mState = STATE_FINISHED;
				break;
			}

			case STATE_FINISHED:
				// Nothing to do in this case
				break;
		}

		return qsf::action::RESULT_DONE;
	}

	void BreakBuildingDoorOpenAction::onShutdown()
	{
		if (mState != STATE_FINISHED && mTargetBuildingComponent.valid())
		{
			// The action was aborted -> reset the door damaged state
			mTargetBuildingComponent->setMainDoorDamaged(false);
		}

		if (mTargetBuildingComponent.valid())
		{
			EventIdComponent::releaseTeamLock(mTargetBuildingComponent->getEntity(), getEntity());
		}

		if (mEffectEntity.valid())
		{
			getMap().destroyEntityById(mEffectEntity->getId());
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void BreakBuildingDoorOpenAction::createBreakDoorEffect()
	{
		qsf::Map& map = getEntity().getMap();

		// Create the effect particle for the axe
		mEffectEntity = map.createObjectByLocalPrefabAssetId(assets::PREFAB_BREAK_DOOR_EFFECT);
		if (mEffectEntity.valid())
		{
			// Show the particle
			qsf::ParticlesComponent* effectParticlesComponent = mEffectEntity->getOrCreateComponent<qsf::ParticlesComponent>();

			effectParticlesComponent->setActive(false);
			effectParticlesComponent->showAnimation();
			effectParticlesComponent->setEmitterEnabled(true);
			effectParticlesComponent->setRepeatAnimation(false);
			effectParticlesComponent->setRestartOnActivation(true);

			mEffectEntity->getComponent<qsf::TransformComponent>()->setScale(glm::vec3(0.1f, 0.1f, 0.1f));

			// Link on equipment entity or (if this in some error case not exist) to the fireman
			qsf::Entity* targetToLinkEffect = EntityHelper(getEntity()).getActiveEquipmentEntity();
			if (nullptr == targetToLinkEffect)
			{
				targetToLinkEffect = &getEntity();
			}

			// Link effect entity to target
			mEffectEntity->getOrCreateComponentSafe<qsf::LinkComponent>().linkToParent(*targetToLinkEffect);

			// Link to bone of the parent
			qsf::game::BoneLinkComponent* boneLinkComponent = mEffectEntity->createComponent<qsf::game::BoneLinkComponent>();
			if (nullptr != boneLinkComponent)
			{
				boneLinkComponent->setBoneName(equipment::STANDARD_BONE_EFFECT);
			}
		}
	}

	void BreakBuildingDoorOpenAction::teleportToBuildingDoor()
	{
		qsf::Transform finalTransform;
		finalTransform.setPosition(mTargetBuildingComponent->getDoorPosition());
		glm::quat rotation = mTargetBuildingComponent->getDoorRotation() * glm::quat(0.0f, 0.0f, 1.0f, 0.0f); // Rotate by 180 degrees
		finalTransform.setRotation(rotation);

		qsf::TransformComponent& callerTransformComponent = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();
		callerTransformComponent.setTransform(finalTransform);
	}

	void BreakBuildingDoorOpenAction::playAxe2Animation()
	{
		if (mEffectEntity.valid())
		{
			qsf::ParticlesComponent* effectParticlesComponent = mEffectEntity->getOrCreateComponent<qsf::ParticlesComponent>();

			// Reinit particle effect
			// TODO(sw) this timing doesn't map 100% to the animation.
			if (nullptr != effectParticlesComponent)
			{
				effectParticlesComponent->setActive(false);
				effectParticlesComponent->setDelayTime(0.5f);
				effectParticlesComponent->setActive(true);
			}
		}

		getComponent().pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(AnimationHelper(getEntity()).getAnimationFirefighterUseAxe2(), false);
	}

	void BreakBuildingDoorOpenAction::sendMessageStart()
	{
		QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_BREAK_BUILDING_DOOR_OPEN_START, mTargetEntityId, getEntityId()));
	}

	void BreakBuildingDoorOpenAction::sendMessageEnd()
	{
		QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_BREAK_BUILDING_DOOR_OPEN_END, mTargetEntityId, getEntityId()));
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
