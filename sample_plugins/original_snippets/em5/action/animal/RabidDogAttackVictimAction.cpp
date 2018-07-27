// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/animal/RabidDogAttackVictimAction.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/logic/local/ReserveLogic.h"

#include <qsf/logic/action/ActionComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier RabidDogAttackVictimAction::ACTION_ID = "em5::RabidDogAttackVictimAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	RabidDogAttackVictimAction::RabidDogAttackVictimAction() :
		Action(ACTION_ID),
		mState(STATE_INIT)
	{
		// Nothing here
	}

	RabidDogAttackVictimAction::~RabidDogAttackVictimAction()
	{
		ReserveLogic::deleteOwnReservation(mTargetEntity, getEntityId());
	}

	void RabidDogAttackVictimAction::init(qsf::Entity& targetEntity)
	{
		mTargetEntity = &targetEntity;
	}

	uint64 RabidDogAttackVictimAction::getTargetEntityId() const
	{
		return mTargetEntity.valid() ? mTargetEntity->getId() : qsf::getUninitialized<uint64>();
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void RabidDogAttackVictimAction::serialize(qsf::BinarySerializer& serializer)
	{
		// TODO(mk) Add serialize function
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool RabidDogAttackVictimAction::onStartup()
	{
		// Action is send without move order before, no additional checks necessary
		return mTargetEntity.valid();
	}

	qsf::action::Result RabidDogAttackVictimAction::updateAction(const qsf::Clock&)
	{
		// Get the target entity instance
		QSF_CHECK(mTargetEntity.valid(), "The EM5 rabid dog attack action was unable to retrieve the target entity instance", return qsf::action::RESULT_DONE);

		// Get the AnimationComponent from the target
		qsf::ActionComponent& targetActionComponent = mTargetEntity->getOrCreateComponentSafe<qsf::ActionComponent>();

		switch (mState)
		{
			case STATE_INIT:
			{
				// Play animation for dog attack
				getComponent().pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(AnimationHelper(getEntity()).getAnimationRescueDogFoundVictim());

				// PLay animation for victim gets hit
				targetActionComponent.pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(AnimationHelper(getEntity()).getAnimationGettingHit());

				mState = STATE_FOUND_VICTIM;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_FOUND_VICTIM:
			{
				if (AnimationHelper(getEntity()).isAnimationPlaying())
				{
					// Wait for ending of the animation
					return qsf::action::RESULT_CONTINUE;
				}

				// Play animation dog threatening
				getComponent().pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(AnimationHelper(getEntity()).getAnimationRescueDogSittingBark(), false, true);

				// Injure target with rabies
				// TODO(co) Is anything missing in here? Or is the comment about bogus?

				//// Send minimap ping
				//MiniMap* minimap = EM5_GUI.getIngameHud().getMiniMap();
				//if (nullptr != minimap)
				//{
				//	minimap->getRenderer().addPingAtWorldPosition(EntityHelper(getEntity()).getPosition());
				//}

				//// Send message
				//qsf::MessageParameters parameters;
				//parameters.setParameter("FoundId", mTargetEntity->getId());
				//QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_RESCUE_DOG_FOUND_PERSON, mTargetEntity->getId()), parameters);

				mState = STATE_LOOPACTION;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_LOOPACTION:
			{
				// Detect end of reservation
				if (ReserveLogic::checkReservation(mTargetEntity.getSafe(), getEntityId()))
				{
					// Loop endless
					return qsf::action::RESULT_CONTINUE;
				}

				return qsf::action::RESULT_DONE;
			}
		}

		return qsf::action::RESULT_DONE;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
