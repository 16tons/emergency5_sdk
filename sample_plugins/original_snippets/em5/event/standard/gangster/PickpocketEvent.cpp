// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/gangster/PickpocketEvent.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/gangster/ReachVictimAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/freeplay/objective/ObjectiveHelper.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/game/targetpoint/AttackPersonTargetPointProvider.h"
#include "em5/logic/local/gangsters/GangsterCivilUnarmedLogic.h"
#include "em5/logic/ObserverHelper.h"
#include "em5/logic/observer/hint/HintShotAtObserver.h"
#include "em5/health/HealthComponent.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/map/CollisionHelper.h"
#include "em5/map/EntityHelper.h"

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/audio/component/DynamicMusicCompositorComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/action/WaitAction.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/map/Map.h>
#include <qsf/math/Random.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 PickpocketEvent::FREEPLAY_EVENT_ID = qsf::StringHash("em5::PickpocketEvent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PickpocketEvent::PickpocketEvent() :
		mPerpetrating(false),
		mPerpetrated(false),
		mDontShootTriggered(false)
	{
		// Nothing here
	}

	PickpocketEvent::~PickpocketEvent()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	bool PickpocketEvent::onStartup()
	{
		QSF_CHECK(mGangster.valid(), "Can't find gangster entity; PickpocketEvent method: onStartup", return false);
		QSF_CHECK(mVictim.valid(), "Can't find victim entity; PickpocketEvent method: onStartup", return false);

		// Set event ID
		EventIdComponent::addToEvent(*mGangster, *this, eventspreadreason::EVENT_SPECIFIC);

		// Order gangster to run to victim
		{
			// Place the gangster near to the victim. We have trouble with gangsters need ages to reach there goals, place them near the goals.
			qsf::TransformComponent& transformComponent = mVictim->getOrCreateComponentSafe<qsf::TransformComponent>();
			glm::vec3 newGangsterPosition = transformComponent.getPosition();
			CollisionHelper(mGangster->getMap()).findFreePositionEM3(mGangster.getSafe(), transformComponent.getPosition(), 10, 0.5, newGangsterPosition);
			mGangster->getOrCreateComponentSafe<qsf::TransformComponent>().setPosition(newGangsterPosition);

			qsf::ActionComponent& gangsterActionComponent = mGangster->getOrCreateComponentSafe<qsf::ActionComponent>();
			gangsterActionComponent.clearPlan();

			// Move to target
			qsf::ai::ReachObjectGoal* reachObjectGoal = new qsf::ai::ReachObjectGoal(*mGangster, *mVictim, AttackPersonTargetPointProvider::TARGET_POINT_ID);
			gangsterActionComponent.pushAction<MoveAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(reachObjectGoal);
			gangsterActionComponent.pushAction<ReachVictimAction>(action::COMMAND_HIGH, qsf::action::APPEND_TO_BACK).init(*mVictim);

			// Let the victim wait to better reach it
			qsf::ActionComponent& victimActionComponent = mVictim->getOrCreateComponentSafe<qsf::ActionComponent>();
			victimActionComponent.pushAction<qsf::WaitAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(qsf::Time::fromSeconds(15.0f));
		}

		// Done
		return true;
	}

	void PickpocketEvent::onShutdown()
	{
		// Nothing to do here
	}

	void PickpocketEvent::onRun()
	{
		// Activate the hint observers
		startHintObservers();
	}

	bool PickpocketEvent::onFailure(EventResult& eventResult)
	{
		Objective* failConditionPersonEscapedObjective = eventResult.mObjectives.getObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_FAIL_PERSONSESCAPE);
		if (nullptr != failConditionPersonEscapedObjective && failConditionPersonEscapedObjective->checkAccomplished())
		{
			showHint("EM5_EVENT_GANGSTER_ESCAPING_FAIL_HINT_02", false);
		}
		else
		{
			int i = qsf::Random::getRandomUint(0, 1);
			showHint((i == 0) ? "EM5_EVENT_TOO_MANY_PERSONS_DIED_HINT_03" : "EM5_EVENT_TOO_MANY_PERSONS_DIED_HINT_04", false);
		}

		// Call base implementation
		return FreeplayEvent::onFailure(eventResult);
	}

	void PickpocketEvent::updateFreeplayEvent(const qsf::Time& timePassed)
	{
		// Check objectives for success or failure
		checkObjectivesState();
		if (getState() != State::STATE_RUNNING && getState() != State::STATE_HIDDEN)
			return;

		if (!mGangster.valid())
			return;	// Happens if the gangster is arrested or injured and transported to hq, valid case, no future update

		if (EntityHelper(*mGangster).isArrested())
			return;

		// Check if the gangster reached the victim
		if (!mPerpetrated && !mPerpetrating)
		{
			if (!mVictim.valid() || !checkIfIsHealthy(*mVictim) || !checkIfIsHealthy(*mGangster))
			{
				// Error, don't start the event
				abortEvent();
				return;
			}

			if (checkIfReachedVictim(*mGangster, *mVictim))
			{
				// Make victim crouch
				{
					qsf::ActionComponent& actionComponent = mVictim->getOrCreateComponentSafe<qsf::ActionComponent>();
					AnimationHelper animationHelper(*mVictim);
					actionComponent.pushAction<PlayAnimationAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(animationHelper.getAnimationDuckDownEnd(), true);
					actionComponent.pushAction<PlayAnimationAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(animationHelper.getAnimationDuckDownIdle(), true, true, false, qsf::Time::ZERO, qsf::Time::fromSeconds(2.0f));
					actionComponent.pushAction<PlayAnimationAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(animationHelper.getAnimationDuckDownStart(), true);
				}

				// Make gangster pick pocket
				{
					qsf::ActionComponent& actionComponent = mGangster->getOrCreateComponentSafe<qsf::ActionComponent>();
					AnimationHelper animationHelper(*mGangster);
					actionComponent.pushAction<PlayAnimationAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(animationHelper.getAnimationManipulateObjectStanding(), true, true, false, qsf::Time::ZERO, qsf::Time::fromSeconds(2.0f));
					actionComponent.pushAction<TurnToAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(mVictim->getId());
				}

				setRunning();
				mPerpetrating = true;
			}
		}
		else if (mPerpetrating)
		{
			QSF_CHECK(mVictim.valid(), "Can't find victim entity; PickpocketEvent method: updateFreeplayEvent", abortEvent(); return);
			qsf::ActionComponent& actionComponent = mGangster->getOrCreateComponentSafe<qsf::ActionComponent>();
			if (nullptr == actionComponent.getAction<PlayAnimationAction>())
			{
				setupGangster(*mGangster, *mVictim);
				mPerpetrating = false;
				mPerpetrated = true;
			}
		}
	}

	bool PickpocketEvent::addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason)
	{
		if (eventspreadreason::INJURY == eventSpreadReason && &targetEntity == mGangster.get())
			ObjectiveHelper(*this).removeObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_ARRESTPERSONS);

		return FreeplayEvent::addEntityToEvent(targetEntity, eventSpreadReason, newReason);
	}

	void PickpocketEvent::hintCallback(Observer& hintObserver)
	{
		if (mDontShootTriggered)
		{
			return;
		}

		mDontShootTriggered = true;

		showHint("EM5_EVENT_DONT_SHOOT_HINT_02", true);
	}

	const qsf::Entity* PickpocketEvent::getFocusEntity()
	{
		// Show random event entity
		return getRandomEventEntity();
	}

	bool PickpocketEvent::checkEventConfiguration()
	{
		return mGangster.valid() && mVictim.valid();
	}

	void PickpocketEvent::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base implementation
		FreeplayEvent::serialize(serializer);

		// TODO(fw): Implement this (not actually needed by EM5)
		//serializer.serialize(mGangsterId);
		//serializer.serialize(mVictimId);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void PickpocketEvent::startHintObservers()
	{
		ObserverHelper(*this).createObserver<HintShotAtObserver>(getGangsterId()).initialize();
	}

	bool PickpocketEvent::checkIfReachedVictim(qsf::Entity& gangster, qsf::Entity& victim)
	{
		qsf::ActionComponent& actionComponent = gangster.getOrCreateComponentSafe<qsf::ActionComponent>();
		return actionComponent.getCurrentAction()->getTypeId() != MoveAction::ACTION_ID;
	}

	void PickpocketEvent::setupGangster(qsf::Entity& gangster, qsf::Entity& victim)
	{
		showSupervisorMessage("EM5_EVENT_SUPERV_OBJ_PICKPOCKET_01");
		EntityHelper(gangster).turnIntoGangsterByType("Pickpocketer");

		qsf::ActionComponent& actionComponent = victim.getOrCreateComponentSafe<qsf::ActionComponent>();
		AnimationHelper animationHelper(victim);
		actionComponent.pushAction<PlayAnimationAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(animationHelper.getAnimationWaveHelpStanding(), true);
		actionComponent.pushAction<TurnToAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(gangster.getId());
		escape(gangster);

		// Set all observers and objectives for the target
		EventIdComponent::addToEvent(gangster, *this, eventspreadreason::GANGSTER);
	}

	void PickpocketEvent::escape(qsf::Entity& gangster)
	{
		showHint("EM5_EVENT_GANGSTER_ESCAPING_HINT_01", false);

		qsf::ActionComponent& actionComponent = gangster.getOrCreateComponentSafe<qsf::ActionComponent>();
		actionComponent.clearPlan();

		qsf::GameLogicComponent* gameLogicComponent = gangster.getComponent<qsf::GameLogicComponent>();
		if (nullptr != gameLogicComponent)
		{
			GangsterBaseLogic* gangsterLogic = gameLogicComponent->getGameLogic<GangsterCivilUnarmedLogic>();
			if (nullptr != gangsterLogic)
			{
				gangsterLogic->setEscapeTargetTag(mEscapeTargetTag);	// Give them a escape target after the gangster logic is initialized
				gangsterLogic->escape(GangsterBaseLogic::ESCAPE_FOOT_LONG);
			}
		}

		// Music
		getDynamicMusicCompositor().setMusicLevel(3);
	}

	bool PickpocketEvent::checkIfIsHealthy(qsf::Entity& personEntity)
	{
		return !personEntity.getComponent<HealthComponent>()->isInjured();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
