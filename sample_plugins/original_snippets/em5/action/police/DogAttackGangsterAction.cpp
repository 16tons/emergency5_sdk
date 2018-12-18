// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/police/DogAttackGangsterAction.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/logic/local/ambulance/RescueDogLeaderLogic.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/audio/AudioHelper.h"

#include <qsf_game/component/base/HiddenComponent.h>

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/action/WaitAction.h>
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
	const qsf::NamedIdentifier DogAttackGangsterAction::ACTION_ID = "em5::DogAttackGangsterAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	DogAttackGangsterAction::DogAttackGangsterAction() :
		Action(ACTION_ID),
		mState(STATE_INIT)
	{
		// Nothing here
	}

	DogAttackGangsterAction::~DogAttackGangsterAction()
	{
		ReserveLogic::deleteOwnReservation(mTargetEntity, getEntityId());
	}

	void DogAttackGangsterAction::init(qsf::Entity& targetEntity, qsf::Entity& rescueDogLeaderEntity)
	{
		mTargetEntity = &targetEntity;
		mRescueDogLeaderEntity = &rescueDogLeaderEntity;
	}

	uint64 DogAttackGangsterAction::getTargetEntityId() const
	{
		return (mTargetEntity.valid() ? mTargetEntity->getId() : qsf::getUninitialized<uint64>());
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void DogAttackGangsterAction::serialize(qsf::BinarySerializer&)
	{
		// TODO(mk) Add serialize function
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool DogAttackGangsterAction::onStartup()
	{
		// TODO(mk) check target again for injuries

		return mTargetEntity.valid();
	}

	void DogAttackGangsterAction::onShutdown()
	{
		// Make sure all audio has stopped
		mAudioProxy.stop();
		mAudioProxy.synchronizeWithMultiplay();
	}

	qsf::action::Result DogAttackGangsterAction::updateAction(const qsf::Clock& clock)
	{
		// Stop if target gets injured
		if (!mTargetEntity.valid() || EntityHelper(*mTargetEntity).isPersonInjured())
		{
			dogReturnToLeader();
			return qsf::action::RESULT_DONE;
		}

		switch (mState)
		{
			case STATE_INIT:
			{
				getComponent().pushAction<TurnToAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(mTargetEntity->getId());

				// Unhide target
				if (nullptr == EntityHelper(mTargetEntity.getSafe()).getContainerEntity())
				{
					qsf::game::HiddenComponent::setHidden(mTargetEntity.getSafe(), false);
					AnimationHelper(*mTargetEntity).clearAnimation();
				}

				// Push action to block other behavior and have a better detection for the end of the animation
				qsf::ActionComponent& actionComponent = mTargetEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
				actionComponent.pushAction<qsf::WaitAction>(action::BLOCKING, qsf::action::REPLACE_WHOLE_PLAN).init(qsf::Time::fromSeconds(1.0f));	// Push this action as dummy to avoid a really fast gangster reaction between this two steps
				actionComponent.pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(AnimationHelper(*mTargetEntity).getAnimationGangsterCowerFearfullyStart(), true);
				actionComponent.pushAction<TurnToAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(mTargetEntity->getId());


				mState = STATE_TURNTO;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_TURNTO:
			{
				// Play animation
				AnimationHelper animationHelper(getEntity());
				animationHelper.playAnimation(animationHelper.getAnimationPoliceDogMenace(), true);

				// Play audio
				AudioHelper::playRescueDogBark(mAudioProxy, getEntity());
				mAudioProxy.synchronizeWithMultiplay();

				mState = STATE_FOUND_VICTIM;
				return qsf::action::RESULT_CONTINUE;

			}

			case STATE_FOUND_VICTIM:
			{
				// Wait till animation action is done
				AnimationHelper targetAnimationHelper(*mTargetEntity);
				if (targetAnimationHelper.isAnimationPlaying(targetAnimationHelper.getAnimationGangsterCowerFearfullyStart()))
				{
					// Wait for ending of the animation
					return qsf::action::RESULT_CONTINUE;
				}

				setStunTimmer();

				mState = STATE_LOOPACTION;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_LOOPACTION:
			{
				if (mAttackTimer <= qsf::Time::ZERO)
				{
					setStunTimmer();
				}
				else
				{
					mAttackTimer -= clock.getTimePassed();
				}

				// Detect end of reservation
				if (ReserveLogic::checkReservation(mTargetEntity.getSafe(), getEntityId()))
				{
					// Loop endless
					return qsf::action::RESULT_CONTINUE;
				}

				// Play animation
				AnimationHelper targetAnimationHelper(*mTargetEntity);
				targetAnimationHelper.playAnimation(targetAnimationHelper.getAnimationGangsterCowerFearfullyEnd());

				mAudioProxy.stop();
				mAudioProxy.synchronizeWithMultiplay();

				dogReturnToLeader();
				return qsf::action::RESULT_DONE;
			}
		}

		return qsf::action::RESULT_DONE;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void DogAttackGangsterAction::dogReturnToLeader() const
	{
		QSF_CHECK(mRescueDogLeaderEntity.valid(), "RescueDogLocateVictim::dogReturnToLeader() can't find an rescue dog leader for the dog " << getEntityId(), QSF_REACT_THROW);
		RescueDogLeaderLogic::callRescueDogBack(mRescueDogLeaderEntity.getSafe());
	}

	void DogAttackGangsterAction::setStunTimmer()
	{
		PersonComponent* personComponent = mTargetEntity->getComponent<PersonComponent>();
		if (personComponent != nullptr)
		{
			personComponent->setStunned(getEntityId(), qsf::Time::fromSeconds(1.0f), false, true);
		}

		// Set the attack timer slightly lower than the stun time to avoid a ending of the stunning and a "interrupted" stun animation
		mAttackTimer = qsf::Time::fromSeconds(0.8f);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
