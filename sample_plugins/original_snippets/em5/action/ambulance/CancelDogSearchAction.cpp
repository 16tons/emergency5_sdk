// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/ambulance/CancelDogSearchAction.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/audio/AudioHelper.h"
#include "em5/logic/local/ambulance/RescueDogLeaderLogic.h"
#include "em5/game/animation/AnimationHelper.h"

#include <qsf/logic/action/ActionComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier CancelDogSearchAction::ACTION_ID = "em5::CancelDogSearchAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	CancelDogSearchAction::CancelDogSearchAction() :
		Action(ACTION_ID),
		mState(STATE_INIT)
	{
		// Nothing here
	}

	CancelDogSearchAction::~CancelDogSearchAction()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void CancelDogSearchAction::serialize(qsf::BinarySerializer& serializer)
	{
		// TODO(mk) Add serialize function
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool CancelDogSearchAction::onStartup()
	{
		// Action is send without move order before, no additional checks necessary
		mRescueDogEntity = RescueDogLeaderLogic::getRescueDogEntityFromLeader(getEntity());
		return mRescueDogEntity.valid();
	}

	qsf::action::Result CancelDogSearchAction::updateAction(const qsf::Clock&)
	{
		// Get the target entity instance
		QSF_CHECK(mRescueDogEntity.valid(), "CancelDogSearchAction::updateAction() unable to retrieve the target entity instance", return qsf::action::RESULT_DONE);

		switch (mState)
		{
			case STATE_INIT:
			{
				qsf::Entity* rescueDog = RescueDogLeaderLogic::getRescueDogEntityFromLeader(getEntity());
				if (nullptr != rescueDog)
				{
					getComponent().pushAction<TurnToAction>(action::BLOCKING, qsf::action::INSERT_BEFORE_CURRENT).init(rescueDog->getId());
				}

				mState = STATE_TURN_TO_DOG;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_TURN_TO_DOG:
			{
				playRescueDogLeaderAnimation();

				// Play audio
				AudioHelper::playRescueDogWhistle(mAudioProxy, getEntity(), qsf::Time::fromSeconds(1.0f));
				mAudioProxy.synchronizeWithMultiplay();

				RescueDogLeaderLogic::callRescueDogBack(getEntity());

				mState = STATE_SENDDOG;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_SENDDOG:
			{
				if (AnimationHelper(getEntity()).isAnimationPlaying())
				{
					// Wait for ending of the animation
					return qsf::action::RESULT_CONTINUE;
				}
				break;
			}
		}

		return qsf::action::RESULT_DONE;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void CancelDogSearchAction::playRescueDogLeaderAnimation() const
	{
		getComponent().pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(AnimationHelper(getEntity()).getAnimationRescueDogLeaderCancelDog());
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
