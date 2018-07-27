// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/ambulance/RescueDogLocateVictimAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/audio/AudioHelper.h"
#include "em5/component/effect/FadeEffectComponent.h"
#include "em5/component/objects/TargetFoundCounterComponent.h"
#include "em5/component/objects/PoliceDogTargetComponent.h"
#include "em5/component/objects/BuryComponent.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/health/HealthComponent.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/logic/local/ambulance/RescueDogLeaderLogic.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/specs/LogicSpecsGroup.h"

#include <qsf_game/component/base/HiddenComponent.h>

#include <qsf/component/link/LinkComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/reflection/type/CampQsfTime.h>	// Needed for part "parameters.tryGetResponse" (for some reason, it works in Release builds without this as well)
#include <qsf/renderer/animation/MeshAnimationChannel.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier RescueDogLocateVictimAction::ACTION_ID = "em5::RescueDogLocateVictimAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	RescueDogLocateVictimAction::RescueDogLocateVictimAction() :
		RescueDogLocateVictimAction(ACTION_ID)
	{
		// Nothing here
	}

	RescueDogLocateVictimAction::~RescueDogLocateVictimAction()
	{
		ReserveLogic::deleteOwnReservation(mTargetEntity, getEntityId());
	}

	void RescueDogLocateVictimAction::init(qsf::Entity& targetEntity, qsf::Entity& rescueDogLeaderEntity)
	{
		mTargetEntity = &targetEntity;
		mRescueDogLeaderEntity = &rescueDogLeaderEntity;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void RescueDogLocateVictimAction::serialize(qsf::BinarySerializer&)
	{
		// TODO(mk) Add serialize function
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool RescueDogLocateVictimAction::onStartup()
	{
		// Action is send without move order before, no additional checks necessary
		return mTargetEntity.valid();
	}

	void RescueDogLocateVictimAction::onShutdown()
	{
		// Make sure all audio has stopped
		mAudioProxy.stop();
		mAudioProxy.synchronizeWithMultiplay();
	}

	qsf::action::Result RescueDogLocateVictimAction::updateAction(const qsf::Clock& clock)
	{
		// Get the target entity instance
		QSF_CHECK(mTargetEntity.valid(), "The EM5 send dog for search action was unable to retrieve the target entity instance", return qsf::action::RESULT_DONE);

		switch (mState)
		{
			case STATE_INIT:
			{
				// Optionally turn to target (relevant for police dog)
				if (mTurnToTarget)
				{
					// First turn to target entity
					getComponent().pushAction<TurnToAction>(getPriority(), qsf::action::INSERT_AT_FRONT).init(mTargetEntity.getSafe().getId());
					mState = STATE_PANT;
					return qsf::action::RESULT_CONTINUE;
				}

				mState = STATE_PANT;
				// Fallthrough by design
			}

			case STATE_PANT:
			{
				// Play animation
				AnimationHelper animationHelper(getEntity());
				mMeshAnimationChannel = animationHelper.playAnimation(animationHelper.getAnimationRescueDogFoundVictim());

				// Play audio
				AudioHelper::playRescueDogPant(mAudioProxy, getEntity());
				mAudioProxy.synchronizeWithMultiplay();

				mState = STATE_FOUND_VICTIM;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_FOUND_VICTIM:
			{
				if (mMeshAnimationChannel.valid() && !mMeshAnimationChannel->isFinished())
				{
					// Wait for ending of the animation
					return qsf::action::RESULT_CONTINUE;
				}

				// Play animation
				AnimationHelper animationHelper(getEntity());
				animationHelper.playAnimation(animationHelper.getAnimationRescueDogSittingBark(), true, false, qsf::Time::fromSeconds(1.0f));

				// Play audio
				AudioHelper::playRescueDogBark(mAudioProxy, getEntity());
				mAudioProxy.synchronizeWithMultiplay();

				// Increase the number of target searches
				mTargetEntity.getSafe().getOrCreateComponentSafe<TargetFoundCounterComponent>().increaseNumber();

				// Unhide target
				if (nullptr == EntityHelper(*mTargetEntity).getContainerEntity())
				{
					qsf::game::HiddenComponent::setHidden(*mTargetEntity, false);

					// Fade in, which also hides the first dirty frame (person animation not updated yet)
					// TODO(fw): Of course, this does not solve the first frame being dirty which is a bug on itself
					mTargetEntity->getOrCreateComponentSafe<FadeEffectComponent>().fadeIn();

					// Handle cutable trees
					qsf::LinkComponent* linkComponent = mTargetEntity->getComponent<qsf::LinkComponent>();
					if (nullptr != linkComponent)
					{
						BuryComponent* buryComponent = linkComponent->getComponentFromEntityOrLinkedParent<BuryComponent>();
						if (nullptr != buryComponent && buryComponent->getCutableBySaw())
						{
							buryComponent->showCuttableBySawIcon(true);
						}
					}

					// Update health icon for victim
					HealthComponent* healthComponent = mTargetEntity->getComponent<HealthComponent>();
					if (nullptr != healthComponent)
						healthComponent->updateInjuredIcon();

					animationHelper.clearAnimation();
				}

				// Remove police dog targets
				mTargetEntity->destroyComponent<PoliceDogTargetComponent>();

				// Remove reservation
				ReserveLogic::deleteOwnReservation(*mTargetEntity, getEntityId());

				// Send message
				qsf::MessageParameters parameters;
				parameters.setParameter("FoundId", mTargetEntity->getId());
				QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(getFoundMessageId(), mTargetEntity->getId()), parameters);

				// Wait until the dog runs back
				//  -> A message callback may respond a time to wait different from the default
				mTimeForAutoAbort = qsf::Time::fromSeconds(LogicSpecsGroup::getInstanceSafe().getPolicemanMinimumWaitingTime());
				parameters.tryGetResponse(mTimeForAutoAbort);

				mState = STATE_LOOPACTION;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_LOOPACTION:
			{
				// Detect end of reservation
				if (ReserveLogic::checkReservation(mTargetEntity.getSafe(), getEntityId()))
				{
					// In case we have Automatism active, we check here if the action as to end. Happens for RescueDog and Police Dogs
					if (MapHelper::isUnitAutomatismActive())
					{
						mTimeForAutoAbort -= clock.getTimePassed();

						if (mTimeForAutoAbort >= qsf::Time::ZERO)
						{
							// Wait till automatism acts
							return qsf::action::RESULT_CONTINUE;
						}
					}
					else
					{
						// Loop endless
						return qsf::action::RESULT_CONTINUE;
					}
				}

				mAudioProxy.stop();
				mAudioProxy.synchronizeWithMultiplay();

				dogReturnToLeader();
				return qsf::action::RESULT_DONE;
			}
		}

		return qsf::action::RESULT_DONE;
	}


	//[-------------------------------------------------------]
	//[ Protected virtual em5::RescueDogLocateVictimAction methods ]
	//[-------------------------------------------------------]
	uint32 RescueDogLocateVictimAction::getFoundMessageId() const
	{
		return Messages::EM5_RESCUE_DOG_FOUND_PERSON;
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	RescueDogLocateVictimAction::RescueDogLocateVictimAction(uint32 actionId) :
		Action(actionId),
		mState(STATE_INIT),
		mTurnToTarget(false)
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void RescueDogLocateVictimAction::dogReturnToLeader() const
	{
		QSF_CHECK(mRescueDogLeaderEntity.valid(), "RescueDogLocateVictim::dogReturnToLeader() can't find an rescue dog leader for the dog " << getEntityId(), QSF_REACT_THROW);
		RescueDogLeaderLogic::callRescueDogBack(mRescueDogLeaderEntity.getSafe());
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
