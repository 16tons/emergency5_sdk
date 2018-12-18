// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/base/CloseQuartersCombatAction.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/gangster/ReachVictimAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/event/standard/gangster/RobberyEvent.h"
#include "em5/freeplay/objective/ObjectiveHelper.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/game/targetpoint/AttackPersonTargetPointProvider.h"
#include "em5/logic/local/gangsters/GangsterCivilUnarmedLogic.h"
#include "em5/logic/observer/PersonDiedObserver.h"
#include "em5/logic/HintHelper.h"
#include "em5/health/HealthComponent.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/CollisionHelper.h"

#include <qsf_game/base/DistanceHelper.h>

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/audio/component/DynamicMusicCompositorComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/action/WaitAction.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/math/Random.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 RobberyEvent::FREEPLAY_EVENT_ID = qsf::StringHash("em5::RobberyEvent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	RobberyEvent::RobberyEvent() :
		mPerpetrating(false),
		mPerpetrated(false),
		mRescue(false),
		mVictimDuckTime(0.0f)
	{
		// Nothing here
	}

	RobberyEvent::~RobberyEvent()
	{
		// Nothing here
	}

	float RobberyEvent::getVictimDuckTime() const
	{
		return mVictimDuckTime;
	}

	void RobberyEvent::setVictimDuckTime(float duckTime)
	{
		mVictimDuckTime = duckTime;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	bool RobberyEvent::onStartup()
	{
		QSF_CHECK(mGangster.valid(), "Can't find gangster entity; RobberyEvent method: onStartup", return false);
		QSF_CHECK(mVictim.valid(), "Can't find victim entity; RobberyEvent method: onStartup", return false);

		// Setup gangster logic
		GangsterBaseLogic* gangsterLogic = getGangsterLogic();
		if (nullptr != gangsterLogic)
		{
			gangsterLogic->setEscapeTargetTag(mEscapeTargetTag);
		}

		// Set event ID
		EventIdComponent::addToEvent(*mGangster, *this, eventspreadreason::EVENT_SPECIFIC);
		EventIdComponent::addToEvent(*mVictim, *this, eventspreadreason::EVENT_SPECIFIC);

		// Order gangster to run to victim
		{
			// Place the gangster near to the victim. We have trouble with gangsters need ages to reach there goals, place them near the goals.
			qsf::TransformComponent& transformComponent = mVictim->getOrCreateComponentSafe<qsf::TransformComponent>();
			glm::vec3 newGangsterPosition = transformComponent.getPosition();
			CollisionHelper(mGangster->getMap()).findFreePositionEM3(mGangster.getSafe(), transformComponent.getPosition(), 10, 0.5, newGangsterPosition);
			mGangster->getOrCreateComponentSafe<qsf::TransformComponent>().setPosition(newGangsterPosition);

			qsf::ActionComponent& gangsterActionComponent = mGangster->getOrCreateComponentSafe<qsf::ActionComponent>();
			gangsterActionComponent.clearPlan();

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

	void RobberyEvent::onShutdown()
	{
		// Nothing to do here
	}

	void RobberyEvent::onRun()
	{
		// Activate the hint observers
		startHintObservers();
	}

	bool RobberyEvent::onFailure(EventResult& eventResult)
	{
		const int i = qsf::Random::getRandomUint(0, 1);
		Objective* failConditionPersonEscapedObjective = eventResult.mObjectives.getObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_FAIL_PERSONSESCAPE);
		if (nullptr != failConditionPersonEscapedObjective && failConditionPersonEscapedObjective->checkAccomplished())
		{
			showHint("EM5_EVENT_GANGSTER_ESCAPING_FAIL_HINT_02", false);
		}
		else
		{
			showHint((i == 0) ? "EM5_EVENT_TOO_MANY_PERSONS_DIED_HINT_03" : "EM5_EVENT_TOO_MANY_PERSONS_DIED_HINT_04", false);
		}

		// Call base implementation
		return FreeplayEvent::onFailure(eventResult);
	}

	void RobberyEvent::updateFreeplayEvent(const qsf::Time& timePassed)
	{
		// Check objectives for success or failure
		checkObjectivesState();
		if (getState() != State::STATE_RUNNING && getState() != State::STATE_HIDDEN)
			return;

		if (mGangster.valid() && EntityHelper(*mGangster).isArrested())
			return;

		// Check if the gangster reached the victim
		if (!mPerpetrated && !mPerpetrating)
		{
			QSF_CHECK(mGangster.valid(), "Can't find gangster entity; RobberyEvent method: updateFreeplayEvent", abortEvent(); return);
			QSF_CHECK(mVictim.valid(), "Can't find victim entity; RobberyEvent method: updateFreeplayEvent", abortEvent(); return);
			if (!checkIfIsHealthy(*mVictim) || !checkIfIsHealthy(*mGangster))
			{
				abortEvent();
			}

			if (checkIfReachedVictim(*mGangster, *mVictim))
			{
				QSF_CHECK(qsf::game::DistanceHelper::getDistance(*mGangster, *mVictim) < 2.0f, "Robber could not reach target; RobberyEvent method: updateFreeplayEvent", abortEvent(); return);
				qsf::ActionComponent& actionComponent = mGangster->getOrCreateComponentSafe<qsf::ActionComponent>();
				actionComponent.pushAction<CloseQuartersCombatAction>().init(*mVictim, weapon::FIST, weapon::FIST, 0, 0.25f, 0.05f);
				actionComponent.pushAction<PlayAnimationAction>().init(AnimationHelper(*mGangster).getAnimationManipulateObjectKneeling(), true);

				qsf::ActionComponent& victimActionComponent = mVictim->getOrCreateComponentSafe<qsf::ActionComponent>();
				victimActionComponent.clearPlan();

				mPerpetrating = true;
				showSupervisorMessage("EM5_EVENT_SUPERV_OBJ_ROBBERY_STREET_01");
				EntityHelper(*mGangster).turnIntoGangster(qsf::StringHash("em5::GangsterCivilUnarmedLogic"), false);

				EventIdComponent::addToEvent(*mGangster, *this, eventspreadreason::GANGSTER);
				setRunning();
			}
		}
		else if (mPerpetrating)
		{
			QSF_CHECK(mGangster.valid(), "Can't find gangster entity; RobberyEvent method: updateFreeplayEvent", abortEvent(); return);
			QSF_CHECK(mVictim.valid(), "Can't find victim entity; RobberyEvent method: updateFreeplayEvent", abortEvent(); return);
			qsf::ActionComponent& actionComponent = mGangster->getOrCreateComponentSafe<qsf::ActionComponent>();
			if (nullptr == actionComponent.getAction<CloseQuartersCombatAction>() && nullptr == actionComponent.getAction<PlayAnimationAction>())
			{
				// Gangster is neither fighting nor robbing the victim, i.e. the perpetration is done
				if (checkIfIsHealthy(*mGangster))
				{
					// Only healthy gangsters can escape
					escape(*mGangster);
				}

				mPerpetrating = false;
				mPerpetrated = true;
			}
		}
		else
		{
			if (!mRescue && mGangster.valid() && !checkIfIsHealthy(*mGangster))
			{
				ObjectiveHelper(*this).removeObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_ARRESTPERSONS);
				mRescue = true;
			}
		}
	}

	void RobberyEvent::hintCallback(Observer& hintObserver)
	{
		showHint("EM5_EVENT_VICTIM_DIES_HINT_01", false);

		// Music
		getDynamicMusicCompositor().setMusicLevel(3);
	}

	const qsf::Entity* RobberyEvent::getFocusEntity()
	{
		if (mGangster.valid())
		{
			const GangsterBaseLogic* gangsterLogic = getGangsterLogic();
			if (nullptr != gangsterLogic && GangsterBaseLogic::STATE_ARRESTED == gangsterLogic->getCurrentState())
			{
				// Gangster is arrested, return victim
				return mVictim.get();
			}
			return mGangster.get();
		}
		return mVictim.get();
	}

	bool RobberyEvent::checkEventConfiguration()
	{
		return (mGangster.valid() && mVictim.valid());
	}

	void RobberyEvent::serialize(qsf::BinarySerializer& serializer)
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
	void RobberyEvent::startHintObservers()
	{
		if (mVictim.valid())
		{
			HintHelper(*this).createAndInitializeHintsAtLowLife(*mVictim);
		}
	}

	bool RobberyEvent::checkIfReachedVictim(qsf::Entity& gangster, qsf::Entity& victim)
	{
		const qsf::ActionComponent& actionComponent = gangster.getOrCreateComponentSafe<qsf::ActionComponent>();
		return actionComponent.getPlan().isEmpty() ? true : (nullptr == actionComponent.getAction<ReachVictimAction>());
	}

	void RobberyEvent::escape(qsf::Entity& gangster)
	{
		showHint("EM5_EVENT_GANGSTER_ESCAPING_HINT_01", false);

		qsf::ActionComponent& actionComponent = gangster.getOrCreateComponentSafe<qsf::ActionComponent>();
		actionComponent.clearPlan();
		qsf::GameLogicComponent* gameLogicComponent = gangster.getComponent<qsf::GameLogicComponent>();
		GangsterBaseLogic* gameLogic = gameLogicComponent->getGameLogic<GangsterCivilUnarmedLogic>();
		gameLogic->escape(GangsterBaseLogic::ESCAPE_FOOT_LONG);

		// Music
		getDynamicMusicCompositor().setMusicLevel(3);
	}

	bool RobberyEvent::checkIfIsHealthy(const qsf::Entity& personEntity)
	{
		return !personEntity.getComponent<HealthComponent>()->isInjured();
	}

	bool RobberyEvent::addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason)
	{
		if (eventSpreadReason == eventspreadreason::INJURY)
		{
			Objective& failConditionPersonDiedObjective = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_FAIL_DEADPERSONS);
			failConditionPersonDiedObjective.setNeededNumber(1);
			createObserver<PersonDiedObserver>(targetEntity.getId()).connectToObjective(failConditionPersonDiedObjective);
		}
		return FreeplayEvent::addEntityToEvent(targetEntity, eventSpreadReason, newReason);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
