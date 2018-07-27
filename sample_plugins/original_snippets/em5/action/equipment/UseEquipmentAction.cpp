// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/equipment/UseEquipmentAction.h"
#include "em5/action/equipment/GetEquipmentAction.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/game/animation/AnimationHelper.h"

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/math/Random.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier UseEquipmentAction::ACTION_ID = "em5::UseEquipmentAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	UseEquipmentAction::UseEquipmentAction() :
		Action(ACTION_ID),
		mType(EQUIPMENT_MOBILE),
		mCurrentState(STATE_INIT)
	{
		// Nothing here
	}

	UseEquipmentAction::~UseEquipmentAction()
	{
		// Nothing here
	}

	void UseEquipmentAction::init(EquipmentType type)
	{
		mType = type;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void UseEquipmentAction::serialize(qsf::BinarySerializer& serializer)
	{
		// TODO(mk) Serialize this
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	qsf::action::Result UseEquipmentAction::updateAction(const qsf::Clock&)
	{
		AnimationHelper animationHelper(getEntity());

		switch (mCurrentState)
		{
			case STATE_INIT:
			{
				animationHelper.clearAnimation();

				// Get the equipment
				getComponent().pushAction<GetEquipmentAction>(action::AUTOMATISM_STD, qsf::action::INSERT_BEFORE_CURRENT).init(equipment::MOBILEPHONE);
				getComponent().pushAction<PlayAnimationAction>(action::AUTOMATISM_STD, qsf::action::INSERT_BEFORE_CURRENT).init(animationHelper.getAnimationMobilephoneGetOutOfPocket(), true, false, false, qsf::Time::fromSeconds(0.5f));

				mCurrentState = STATE_PUSH_ANIMATIONS;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_PUSH_ANIMATIONS:
			{
				if (animationHelper.isAnimationPlaying())
				{
					// Wait till animation is finished
					return qsf::action::RESULT_CONTINUE;
				}

				// Chance to play one animation with higher optical impact
				if (qsf::Random::getRandomChance(0.5f))
				{
					getComponent().pushAction<PlayAnimationAction>(action::AUTOMATISM_STD, qsf::action::INSERT_BEFORE_CURRENT).init(animationHelper.getAnimationMobilephoneUsePhotograph(), true, false, false, qsf::Time::fromSeconds(0.5f));
				}

				// Push normal idle 2-3 times, the concrete animation is random in the getAnimation function
				const int numberOfAnimations = qsf::Random::getRandomInt(2, 3);
				for (int i = 0; i < numberOfAnimations; ++i)
				{
					getComponent().pushAction<PlayAnimationAction>(action::AUTOMATISM_STD, qsf::action::INSERT_BEFORE_CURRENT).init(animationHelper.getAnimationMobilephoneUseTalk(), true, false, false, qsf::Time::fromSeconds(0.5f));
				}

				mCurrentState = STATE_HIDE_EQUIPMENT;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_HIDE_EQUIPMENT:
			{
				if (animationHelper.isAnimationPlaying())
				{
					// Wait till animation is finished
					return qsf::action::RESULT_CONTINUE;
				}
				getComponent().pushAction<PlayAnimationAction>(action::AUTOMATISM_STD, qsf::action::INSERT_BEFORE_CURRENT).init(animationHelper.getAnimationMobilephonePutInPocket(), true, false, false, qsf::Time::fromSeconds(0.5f));
				getComponent().pushAction<GetEquipmentAction>(action::AUTOMATISM_STD, qsf::action::INSERT_BEFORE_CURRENT).init("");

				mCurrentState = STATE_DONE;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_DONE:
				// Nothing to do in this case
				break;
		}

		// Clear the last animation to force an idle animation
		animationHelper.clearAnimation();
		return qsf::action::RESULT_DONE;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
