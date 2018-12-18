// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/ambulance/historic/SendPlagueVictimHomeAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/base/EnterBuildingAction.h"
#include "em5/action/base/DisappearAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/SignalAction.h"
#include "em5/audio/AudioHelper.h"
#include "em5/component/objects/HistoricPlagueBuildingComponent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/command/ambulance/historic/SendPlagueVictimHomeCommand.h"
#include "em5/command/CommandContext.h"
#include "em5/game/targetpoint/GetIntoBuildingTargetPointProvider.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/specs/ActionRangeSpecsGroup.h"

#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/QsfGameHelper.h>

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/action/WaitAction.h>
#include <qsf/message/MessageSystem.h>
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
	const qsf::NamedIdentifier SendPlagueVictimHomeAction::ACTION_ID = "em5::SendPlagueVictimHomeAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	SendPlagueVictimHomeAction::SendPlagueVictimHomeAction() :
		Action(ACTION_ID),
		mState(STATE_INIT)
	{
		// Nothing here
	}

	SendPlagueVictimHomeAction::~SendPlagueVictimHomeAction()
	{
		ReserveLogic::deleteOwnReservation(mTargetEntity, getEntityId());
	}

	void SendPlagueVictimHomeAction::init(qsf::Entity& plagueVictim)
	{
		mTargetEntity = plagueVictim;
	}

	uint64 SendPlagueVictimHomeAction::getTargetEntityId() const
	{
		return (mTargetEntity.valid() ? mTargetEntity->getId() : qsf::getUninitialized<uint64>());
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void SendPlagueVictimHomeAction::serialize(qsf::BinarySerializer& serializer)
	{
		// TODO(mk) Add serialize function
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool SendPlagueVictimHomeAction::onStartup()
	{
		if (!mTargetEntity.valid())
			return false;

		// Make the distance check
		const float maxDistance = ActionRangeSpecsGroup::getInstanceSafe().getUsePepperSprayDistance();
		const float currentDistance = qsf::game::DistanceHelper::getDistance(mTargetEntity.getSafe(), getEntity());
		if (currentDistance > maxDistance)
			return false;

		// Build context
		CommandContext commandContext;
		commandContext.mCaller = &getEntity();
		commandContext.mTargetEntity = mTargetEntity.get();

		// Call "checkCaller" && "checkContext" of the commando for validation
		SendPlagueVictimHomeCommand* sendPlagueVictimHomeCommand = static_cast<SendPlagueVictimHomeCommand*>(QSFGAME_COMMAND.getCommandManager().getById<SendPlagueVictimHomeCommand>(SendPlagueVictimHomeCommand::PLUGINABLE_ID));
		QSF_CHECK(nullptr != sendPlagueVictimHomeCommand, "SendPlagueVictimHomeAction::onStartup(): Could not find send plague victim home command instance", return false);

		// Here we skip the caller check, because the states for the pestilence doctor are simple
		if (!(sendPlagueVictimHomeCommand->checkContext(commandContext)))
			return false;

		return true;
	}

	qsf::action::Result SendPlagueVictimHomeAction::updateAction(const qsf::Clock&)
	{
		// Get the target entity instance
		QSF_CHECK(mTargetEntity.valid(), "SendPlagueVictimHomeAction::updateAction() unable to retrieve the target entity instance", return qsf::action::RESULT_DONE);

		switch (mState)
		{
			case STATE_INIT:
			{
				// Reserve the entity
				ReserveLogic::createReservation<ReserveDoctorLogic>(mTargetEntity.getSafe(), getEntityId());

				getComponent().pushAction<TurnToAction>(action::BLOCKING, qsf::action::INSERT_BEFORE_CURRENT).init(mTargetEntity->getId());

				// Stop the target
				mTargetEntity->getOrCreateComponentSafe<qsf::ActionComponent>().pushAction<qsf::WaitAction>(action::COMMAND_LOW, qsf::action::INSERT_AT_FRONT).init(qsf::Time::fromSeconds(5.0f));

				mState = STATE_TURN_TO_VICTIM;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_TURN_TO_VICTIM:
			{
				playDoctorAnimation();

				// Play audio
				AudioHelper::playHistoricDoctorSendHome(mAudioProxy, getEntity());

				mState = STATE_PLAYANIMATION;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_PLAYANIMATION:
			{
				if (AnimationHelper(getEntity()).isAnimationPlaying())
				{
					// Wait for ending of the animation
					return qsf::action::RESULT_CONTINUE;
				}

				// Send victim back home
				sendVictimHome();
				break;
			}
		}

		return qsf::action::RESULT_DONE;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void SendPlagueVictimHomeAction::playDoctorAnimation() const
	{
		getComponent().pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(AnimationHelper(getEntity()).getAnimationDoctorHistoricSendPersonsHome());
	}

	void SendPlagueVictimHomeAction::sendVictimHome()
	{
		// Get home from person component
		PersonComponent* personComponent = mTargetEntity->getComponent<PersonComponent>();
		if (personComponent != nullptr)
		{
			// The trick here is to set the home building ID in the person component with the building were the persons has to go back
			qsf::Entity* homeBuldingEntity = getMap().getEntityById(personComponent->getHomeBuildingId());
			if (nullptr != homeBuldingEntity)
			{
				// Hide icons
				HistoricPlagueBuildingComponent::showSpawnedEntityIcons(*mTargetEntity, false);

				// Disable shine through for target
				EntityHelper(*mTargetEntity).enableShineThroughEffect(false);

				// Get back home
				qsf::ActionComponent& actionComponent = mTargetEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
				actionComponent.clearPlan();
				actionComponent.pushAction<MoveAction>(action::COMMAND_LOW).init(new qsf::ai::ReachObjectGoal(getEntity(), *homeBuldingEntity, GetIntoBuildingTargetPointProvider::TARGET_POINT_ID));
				actionComponent.pushAction<EnterBuildingAction>(action::BLOCKING).init(*homeBuldingEntity);

				// Push signal action so that the main event can react to the returned entity
				actionComponent.pushAction<SignalAction>(action::BLOCKING).init(
					qsf::MessageConfiguration(Messages::EM5_PLAGUEPERSON_REMOVED, true, mTargetEntity->getId()), qsf::MessageParameters(), false);
				actionComponent.pushAction<DisappearAction>(action::BLOCKING).init(qsf::Time::fromSeconds(0.1f), true);

				// Send out message
				QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_PLAGUEPERSON_SENTBACK, mTargetEntity->getId()));
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
