// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/police/SpeakToPersonAction.h"
#include "em5/command/police/SpeakToPersonCommand.h"
#include "em5/command/CommandContext.h"
#include "em5/component/person/SpeakToPersonComponent.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/specs/ActionRangeSpecsGroup.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Messages.h"

#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/command/CommandManager.h>
#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/QsfGameHelper.h>

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/action/TurnToAction.h>
#include <qsf/map/Map.h>
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
	const qsf::NamedIdentifier SpeakToPersonAction::ACTION_ID	 = "em5::SpeakToPersonAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	SpeakToPersonAction::SpeakToPersonAction() :
		Action(ACTION_ID),
		mCurrentState(STATE_INIT)
	{
		// Nothing here
	}

	SpeakToPersonAction::~SpeakToPersonAction()
	{
		// Remove reservation again
		ReserveLogic::deleteOwnReservation(mTargetEntity, getEntityId());
	}

	void SpeakToPersonAction::init(qsf::Entity& targetEntity)
	{
		mTargetEntity = targetEntity;

		// To avoid multiple units interacting at the same target, we reserve the target here.
		// It is only checked in command for automatism. If the unit starts the action, the logic is changed. Only reserve if no other reserve the target (its not bad if a moveTo reservation is missing)
		// Reserve the entity
		if (ReserveLogic::checkReservation(mTargetEntity.getSafe(), getEntityId()))
		{
			ReserveLogic::createReservation<ReserveMoveToTargetLogic>(mTargetEntity.getSafe(), getEntityId());
		}
	}

	uint64 SpeakToPersonAction::getTargetEntityId() const
	{
		return mTargetEntity.valid() ? mTargetEntity->getId() : qsf::getUninitialized<uint64>();
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void SpeakToPersonAction::serialize(qsf::BinarySerializer& serializer)
	{
		// TODO(mk) Add serializing here
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool SpeakToPersonAction::onStartup()
	{
		qsf::Entity& callerEntity = getEntity();
		if (!mTargetEntity.valid())
			return false;

		// Make the distance check
		const float maxDistance = ActionRangeSpecsGroup::getInstanceSafe().getTreatPersonDistance();
		const float currentDistance = qsf::game::DistanceHelper::getDistance(*mTargetEntity, callerEntity);
		if (currentDistance > maxDistance)
			return false;

		// Build context
		CommandContext commandContext;
		commandContext.mCaller = &callerEntity;
		commandContext.mTargetEntity = mTargetEntity.get();

		// Call "checkCaller" && "checkContext" of the commando for validation
		SpeakToPersonCommand* speakToPersonCommand = static_cast<SpeakToPersonCommand*>(QSFGAME_COMMAND.getCommandManager().getById<SpeakToPersonCommand>(SpeakToPersonCommand::PLUGINABLE_ID));
		QSF_CHECK(nullptr != speakToPersonCommand, "SpeakToPersonAction::onStartup(): Could not find talk to person command instance", return false);
		if (!(speakToPersonCommand->checkCaller(*commandContext.mCaller) && speakToPersonCommand->checkContext(commandContext)))
			return false;

		// Check for the reserve logic
		if (!ReserveLogic::checkReservation(mTargetEntity.getSafe(), getEntityId()))
			return false;

		return true;
	}

	qsf::action::Result SpeakToPersonAction::updateAction(const qsf::Clock&)
	{
		switch (mCurrentState)
		{
			case STATE_INIT:
			{
				getComponent().pushAction<qsf::TurnToAction>(getPriority(), qsf::action::INSERT_AT_FRONT).init(mTargetEntity->getId());

				// Turn target to caller
				if (mTargetEntity != nullptr)
				{
					mTargetEntity->getOrCreateComponentSafe<qsf::ActionComponent>().pushAction<qsf::TurnToAction>(getPriority(), qsf::action::INSERT_AT_FRONT).init(getEntityId());
				}
				// Change reserve logic from "moveto" to "SpeakToPerson" (has higher priority)
				ReserveLogic::deleteOwnReservation(getMap(), mTargetEntity->getId(), getEntityId());
				ReserveLogic::createReservation<SpeakToPersonLogic>(mTargetEntity.getSafe(), getEntityId());

				mCurrentState = STATE_TURN_TO_TARGET;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_TURN_TO_TARGET:
			{
				if (getComponent().getAction<qsf::TurnToAction>() != nullptr ||
					mTargetEntity->getOrCreateComponentSafe<qsf::ActionComponent>().getAction<qsf::TurnToAction>() != nullptr)
				{
					// Wait for end of turning
					return qsf::action::RESULT_CONTINUE;
				}

				// Push talking animations
				AnimationHelper callerAnimationHelper = AnimationHelper(getEntity());
				callerAnimationHelper.playAnimation(callerAnimationHelper.getAnimationTalkStanding01());

				AnimationHelper targetAnimationHelper = AnimationHelper(mTargetEntity.getSafe());
				targetAnimationHelper.playAnimation(targetAnimationHelper.getAnimationTalkStanding02());

				mCurrentState = STATE_TALK;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_TALK:
			{
				AnimationHelper callerAnimationHelper = AnimationHelper(getEntity());
				AnimationHelper targetAnimationHelper = AnimationHelper(mTargetEntity.getSafe());
				if (callerAnimationHelper.isAnimationPlaying(callerAnimationHelper.getAnimationTalkStanding01()) ||
					targetAnimationHelper.isAnimationPlaying(targetAnimationHelper.getAnimationTalkStanding02()))
				{
					// Wait for animation finish
					return qsf::action::RESULT_CONTINUE;
				}

				mTargetEntity->destroyComponent<SpeakToPersonComponent>();

				// Send message
				qsf::MessageParameters parameters;
				parameters.setParameter("PolicemanId", getEntityId());
				parameters.setParameter("PersonId", getTargetEntityId());
				QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_SPEAK_TO_PERSON, getTargetEntityId()), parameters);

				mCurrentState = STATE_DONE;
				return qsf::action::RESULT_CONTINUE;
			}
		}


		return qsf::action::RESULT_DONE;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
