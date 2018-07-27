// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/police/historic/RescueWitchAction.h"
#include "em5/action/move/LinearMoveAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/command/police/historic/RescueWitchCommand.h"
#include "em5/command/CommandContext.h"
#include "em5/component/movement/MoveToDespawnComponent.h"
#include "em5/component/objects/StakeComponent.h"
#include "em5/component/overlay/StatusOverlayComponent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/specs/ActionRangeSpecsGroup.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Messages.h"

#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/command/CommandManager.h>
#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/QsfGameHelper.h>

#include <qsf/component/link/LinkComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/map/Map.h>
#include <qsf/map/Entity.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier RescueWitchAction::ACTION_ID = "em5::RescueWitchAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	RescueWitchAction::RescueWitchAction() :
		Action(ACTION_ID),
		mCurrentState(STATE_INIT),
		mProgressTime(qsf::Time::ZERO),
		mProgressTimeTotall(qsf::Time::ZERO),
		mSwordPlaced(false),
		mPreventAnimation(false)
	{
		// Nothing here
	}

	RescueWitchAction::~RescueWitchAction()
	{
		if (mTargetEntity.valid() && mCurrentState != STATE_INIT)
		{
			StatusOverlayComponent::hideBarOverlay(*mTargetEntity, StatusOverlayComponent::BAR_TYPE_PROGRESS);
		}
		ReserveLogic::deleteOwnReservation(mTargetEntity, getEntityId());

		if (mSwordPlaced)
		{
			takeSword();
		}

		if (mTargetEntity.valid() && mCurrentState != STATE_DONE)
		{
			// Action was abort before finishing
			StakeComponent* stakeComponent = mTargetEntity->getComponent<StakeComponent>();
			if (nullptr != stakeComponent)
			{
				stakeComponent->setcanBeIgnited(true);
			}
		}

		if (mPreventAnimation)
		{
			// Stop manipulating the animations by hand
			PersonComponent* personComponent = getEntity().getComponent<PersonComponent>();
			if (nullptr != personComponent)
			{
				personComponent->preventUpdateAnimations(false);
			}
		}
	}

	void RescueWitchAction::init(qsf::Entity& targetEntity)
	{
		mTargetEntity = targetEntity;
	}

	uint64 RescueWitchAction::getTargetEntityId() const
	{
		return (mTargetEntity.valid() ? mTargetEntity->getId() : qsf::getUninitialized<uint64>());
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void RescueWitchAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serializeAs<uint16>(mCurrentState);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool RescueWitchAction::onStartup()
	{
		if (nullptr == mTargetEntity->getComponent<StakeComponent>())
			return false;

		uint64 witchEntityId = mTargetEntity->getComponent<StakeComponent>()->getWitchEntity();
		mWitchEntity = getMap().getEntityById(witchEntityId);
		if (!mWitchEntity.valid())
			return false;

		if (!validateUserAndTarget())
			return false;

		// Make the distance check
		const float maxDistance = ActionRangeSpecsGroup::getInstanceSafe().getArrestGangsterDistance();
		const float currentDistance = qsf::game::DistanceHelper::getDistance(mTargetEntity.getSafe(), getEntity());
		if (currentDistance > maxDistance)
			return false;

		// Build context
		CommandContext commandContext;
		commandContext.mCaller = &getEntity();
		commandContext.mTargetEntity = mTargetEntity.get();

		// Call "checkCaller" && "checkContext" of the commando for validation
		RescueWitchCommand* rescueWitchCommand = static_cast<RescueWitchCommand*>(QSFGAME_COMMAND.getCommandManager().getById<RescueWitchCommand>(RescueWitchCommand::PLUGINABLE_ID));
		QSF_CHECK(nullptr != rescueWitchCommand, "RescueWitchAction::onStartup(): Could not find rescue witch command instance", return false);
		if (!rescueWitchCommand->checkContext(commandContext))
			return false;

		// Check for the reserve logic
		if (!ReserveLogic::checkReservation(mTargetEntity.getSafe(), getEntityId()))
			return false;

		return true;
	}

	qsf::action::Result RescueWitchAction::updateAction(const qsf::Clock& clock)
	{
		if (validateUserAndTarget())
		{
			switch (mCurrentState)
			{
				case STATE_INIT:
				{
					StakeComponent* stakeComponent = mTargetEntity->getComponent<StakeComponent>();

					stakeComponent->setcanBeIgnited(false);

					getComponent().pushAction<TurnToAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).initWithRotation(stakeComponent->getActionTransform().getRotation());

					// Use a linear move action to position yourself to the stakes
					const float OFFSET_FROM_TARGET = 0.3f;

					glm::vec3 targetPosition = stakeComponent->getActionTransform().getPosition();
					const glm::vec3& entityPosition = EntityHelper(getEntity()).getPosition();

					if (glm::length(entityPosition - targetPosition) > OFFSET_FROM_TARGET)
					{
						targetPosition += glm::normalize(entityPosition - targetPosition) * OFFSET_FROM_TARGET;

						getComponent().pushAction<LinearMoveAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(targetPosition, 0.1f);
					}

					mCurrentState = STATE_START_PROGRESS;
					return qsf::action::RESULT_CONTINUE;
				}

				case STATE_START_PROGRESS:
				{
					StakeComponent* stakeComponent = mTargetEntity->getComponent<StakeComponent>();

					mProgressTimeTotall = (stakeComponent != nullptr ? stakeComponent->getRescueTime() : qsf::Time::fromSeconds(10.0f));
					mProgressTime = mProgressTimeTotall;

					ReserveLogic::createReservation<RescueWitchLogic>(mTargetEntity.getSafe(), getEntityId());

					// Temporary remove the sword equipment
					mSwordPlaced = true;
					EntityHelper(getEntity()).deleteActiveEquipment();

					AnimationHelper animationHelper(getEntity());
					animationHelper.playAnimation(animationHelper.getAnimationWitchRescuing(), true, false, qsf::Time::fromSeconds(0.3f));

					PersonComponent* personComponent = getEntity().getComponent<PersonComponent>();
					if (nullptr != personComponent)
					{
						// Using linear move action leads to errors with animation. visible if action is started with wrong movement speed (animation is interrupted by person animation controller) or abort while animation is playing. Pause the automatic animation)
						mPreventAnimation = true;
						personComponent->preventUpdateAnimations(true);
					}

					mCurrentState = STATE_DO_PROGRESS;
					// Fall through by design
				}

				case STATE_DO_PROGRESS:
				{
					if (mProgressTime > qsf::Time::ZERO)
					{
						mProgressTime -= clock.getTimePassed();
						StatusOverlayComponent::showBarOverlay(mTargetEntity.getSafe(), StatusOverlayComponent::BAR_TYPE_PROGRESS, 1.0f - (mProgressTime.getSeconds() / mProgressTimeTotall.getSeconds()));

						// Wait till time is over
						return qsf::action::RESULT_CONTINUE;
					}

					StatusOverlayComponent::hideBarOverlay(mTargetEntity.getSafe(), StatusOverlayComponent::BAR_TYPE_PROGRESS);
					mCurrentState = STATE_FREE_WITCH;
					// Fall through by design
				}

				case STATE_FREE_WITCH:
				{
					freeWitch();

					// We need time to create the collision before setting the entity to the final place
					mCurrentState = STATE_DONE;
					return qsf::action::RESULT_CONTINUE;
				}

				case STATE_DONE:
				{
					StakeComponent* stakeComponent = mTargetEntity->getComponent<StakeComponent>();
					if (nullptr != stakeComponent)
					{
						// Turn inactive to shutdown the icons and logic
						stakeComponent->setActive(false);
					}

					// Nothing here
					break;
				}
			}
		}

		return qsf::action::RESULT_DONE;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	bool RescueWitchAction::validateUserAndTarget()
	{
		if (!mTargetEntity.valid())
			return false;

		if (!mWitchEntity.valid())
			return false;

		return true;
	}

	void RescueWitchAction::freeWitch()
	{
		// Unlink Witch from parent
		mWitchEntity->destroyComponent<qsf::LinkComponent>();
		EntityHelper(*mWitchEntity).disableMeshAnimationTestComponent();
		StatusOverlayComponent::showIcon(mWitchEntity.getSafe(), StatusOverlayComponent::STATUS_ICON_WITCH, true);

		// Send message
		qsf::MessageParameters parameters;
		parameters.setParameter("GuardsmanId", getEntityId());
		parameters.setParameter("WitchId", mWitchEntity->getId());
		parameters.setParameter("PyreId", getTargetEntityId());
		QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_RESCUE_WITCH_FINISH, getTargetEntityId()), parameters);
	}

	void RescueWitchAction::takeSword()
	{
		mSwordPlaced = false;

		EntityHelper(getEntity()).showEquipment(equipment::POLICE_SWORD);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
