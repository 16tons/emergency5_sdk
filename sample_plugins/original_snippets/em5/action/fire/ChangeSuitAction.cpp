// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/fire/ChangeSuitAction.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/component/effect/FadeEffectComponent.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/map/EntityHelper.h"

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/renderer/mesh/MeshComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier ChangeSuitAction::ACTION_ID = "em5::ChangeSuitAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	ChangeSuitAction::ChangeSuitAction() :
		Action(ACTION_ID),
		mState(STATE_INIT),
		mFadeEquipmentTime(qsf::Time::fromSeconds(1.f))
	{
		// Nothing here
	}

	ChangeSuitAction::~ChangeSuitAction()
	{
		// Nothing here
	}

	void ChangeSuitAction::init(const qsf::AssetProxy& targetSuit, const qsf::Time& fadeOutTime, const qsf::Time& fadeInTime)
	{
		mTargetSuit = targetSuit;
		mFadeOutTime = fadeOutTime;
		mFadeInTime = fadeInTime;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void ChangeSuitAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serializeAs<uint8>(mState);
		mTargetSuit.serialize(serializer);
		serializer.serialize(mFadeOutTime);
		serializer.serialize(mFadeInTime);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	qsf::action::Result ChangeSuitAction::updateAction(const qsf::Clock& clock)
	{
		switch (mState)
		{
			case STATE_INIT:
			{
				// Fade-out the equipment
				qsf::Entity* equipmentEntity = EntityHelper(getEntity()).getActiveEquipmentEntity();
				if (nullptr != equipmentEntity)
				{
					equipmentEntity->getOrCreateComponentSafe<FadeEffectComponent>().fadeOut(mFadeEquipmentTime);

					// For multiplay we need time between two fades to know if the fading has changed...
					mTimeUntilStateSwitch = mFadeEquipmentTime + qsf::Time::fromSeconds(0.1f);
				}

				mState = STATE_FADE_OUT_EQUIPMENT;

				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_FADE_OUT_EQUIPMENT:
			{
				mTimeUntilStateSwitch -= clock.getTimePassed();
				if (mTimeUntilStateSwitch.getSeconds() <= 0 && getEntity().getComponent<FadeEffectComponent>() == nullptr)
				{
					// Play some transition animation
					getComponent().pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(AnimationHelper(getEntity()).getAnimationFiremanPutOnAbcSuit01(), true, false, false, qsf::Time::fromSeconds(0.3f));

					mState = STATE_FADE_OUT_START;
				}

				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_FADE_OUT_START:
			{
				// Start fade-out
				getEntity().getOrCreateComponentSafe<FadeEffectComponent>().fadeOut(mFadeOutTime);

				// For multiplay we need time between two fades to know if the fading has changed...
				mTimeUntilStateSwitch = mFadeOutTime + qsf::Time::fromSeconds(0.1f);

				mState = STATE_FADE_OUT;

				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_FADE_OUT:
			{
				mTimeUntilStateSwitch -= clock.getTimePassed();

				if (mTimeUntilStateSwitch.getSeconds() <= 0 && getEntity().getComponent<FadeEffectComponent>() == nullptr)
				{
					// Change the suit (aka the mesh)
					getEntity().getComponentSafe<qsf::MeshComponent>().setMesh(mTargetSuit);

					// Start fade-in
					getEntity().getOrCreateComponentSafe<FadeEffectComponent>().fadeIn(mFadeInTime);

					// For multiplay we need time between two fades to know if the fading has changed...
					mTimeUntilStateSwitch = mFadeInTime + qsf::Time::fromSeconds(0.1f);

					AnimationHelper animationHelper(getEntity());
					animationHelper.playAnimation(animationHelper.getAnimationFiremanPutOnAbcSuit02());

					mState = STATE_FADE_IN;
				}

				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_FADE_IN:
			{
				mTimeUntilStateSwitch -= clock.getTimePassed();

				if (mTimeUntilStateSwitch.getSeconds() > 0 || getEntity().getComponent<FadeEffectComponent>() != nullptr)
					return qsf::action::RESULT_CONTINUE;

				AnimationHelper animationHelper(getEntity());
				if (animationHelper.isAnimationPlaying(animationHelper.getAnimationFiremanPutOnAbcSuit02()))
					return qsf::action::RESULT_CONTINUE;

				// Fade-in the equipment
				qsf::Entity* equipmentEntity = EntityHelper(getEntity()).getActiveEquipmentEntity();
				if (nullptr != equipmentEntity)
				{
					equipmentEntity->getOrCreateComponentSafe<FadeEffectComponent>().fadeIn(mFadeEquipmentTime);
					mTimeUntilStateSwitch = mFadeEquipmentTime;
				}

				mState = STATE_FADE_IN_EQUIPMENT;

				// Immediately go to the next state; no break by intent
			}

			case STATE_FADE_IN_EQUIPMENT:
			{
				mTimeUntilStateSwitch -= clock.getTimePassed();
				if (mTimeUntilStateSwitch.getSeconds() > 0 || getEntity().getComponent<FadeEffectComponent>() != nullptr)
				{
					return qsf::action::RESULT_CONTINUE;
				}

				// Immediately go to the next state; no break by intent
			}
		}

		// Clear animation as animation state is usually wrong now
		AnimationHelper(getEntity()).clearAnimation();

		// TODO(mz): There is some short sliding going on now when he starts walking again (as in other places)

		// Done
		return qsf::action::RESULT_DONE;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
