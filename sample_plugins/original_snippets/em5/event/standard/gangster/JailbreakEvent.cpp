// Copyright (C) 2012-2014 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/gangster/JailbreakEvent.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/command/drone/SEKDroneSearchCommand.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/freeplay/objective/ObjectiveHelper.h"
#include "em5/logic/local/gangsters/GangsterCivilArmedLogic.h"
#include "em5/logic/ObserverHelper.h"
#include "em5/logic/observer/hint/HintMessageGeneralObserver.h"
#include "em5/logic/observer/PersonDiedObserver.h"
#include "em5/logic/observer/GangsterEscapedObserver.h"
#include "em5/logic/observer/TransportedToHqObserver.h"
#include "em5/logic/HintHelper.h"
#include "em5/game/Game.h"
#include "em5/event/EventHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/EM5Helper.h"

#include <qsf_game/component/base/HiddenComponent.h>
#include <qsf_game/timer/GameTimerManager.h>

#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/map/Map.h>
#include <qsf/math/Random.h>
#include <qsf/message/MessageProxy.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Local definitions                                     ]
	//[-------------------------------------------------------]
	namespace
	{
		// Event hints
		const std::string JAILBREAK_HINT_01 = "EM5_EVENT_JAILBREAK_HINT_01";
		const std::string JAILBREAK_HINT_02 = "EM5_EVENT_JAILBREAK_HINT_02";

		// Local messages
		const qsf::StringHash MESSAGE_ID_TIMER("em5::JailbreakEvent_Timer");
	}


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 JailbreakEvent::FREEPLAY_EVENT_ID = qsf::StringHash("em5::JailbreakEvent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	JailbreakEvent::JailbreakEvent() :
		mArmedChance(0.5f),
		mPrisonersSeekTime(qsf::Time::fromSeconds(300.0f)),
		mHint01WaitTime(qsf::Time::fromSeconds(120.0f)),
		mAllGangsterDetectByDrone(false)
	{
		// Nothing here
	}

	JailbreakEvent::~JailbreakEvent()
	{
		// Nothing here
	}

	std::vector<uint64>& JailbreakEvent::getPrisonerIds()
	{
		return mPrisonerIds;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	bool JailbreakEvent::onStartup()
	{
		// Create message proxies
		mPrisonerFoundMessageProxies.resize(mPrisonerIds.size());

		// Add prisoners
		for (size_t index = 0; index < mPrisonerIds.size(); ++index)
		{
			qsf::Entity* gangsterEntity = getMap().getEntityById(mPrisonerIds[index]);
			QSF_CHECK(gangsterEntity, "Can't find gangster entity; JailbreakEvent method: onStartup", QSF_REACT_NONE);
			if (nullptr == gangsterEntity)
				return false;

			setupGangster(*gangsterEntity);

			// Add new objectives
			Objective& needFindKillerObjective = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_FINDKILLERWITHSEKDRONE);
			needFindKillerObjective.increaseNeededNumber(gangsterEntity);

			Objective& failGangsterNotFoundObjective = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_FAIL_GANGSTERNOTFOUND);
			failGangsterNotFoundObjective.setNeededNumber(1);

			// Register find gangster message proxy (use find hidden person for this)
			qsf::MessageConfiguration message(Messages::EM5_ACTION_FIND_HIDDEN_PERSON_WITH_SEK_DRONE, gangsterEntity->getId());
			mPrisonerFoundMessageProxies[index].registerAt(message, boost::bind(&JailbreakEvent::onPrisonerFound, this, _1));

			EventIdComponent::addToEvent(*gangsterEntity, *this, eventspreadreason::EVENT_SPECIFIC);
		}

		setRunning();

		// Done
		return true;
	}

	void JailbreakEvent::onShutdown()
	{
		for (qsf::MessageProxy& messageProxy : mPrisonerFoundMessageProxies)
		{
			messageProxy.unregister();
		}

		mGameTimerMessageProxy.unregister();
	}

	void JailbreakEvent::onRun()
	{
		// Play supervisor message
		showSupervisorMessage(HintHelper::getRandomStringOfIdString(HintHelper(*this).getHintParameters().mSupervisorMessageIds));

		// Activate the hint observers
		startHintObservers();

		// Register for game timer messages
		mGameTimerMessageProxy.registerAt(qsf::MessageConfiguration(MESSAGE_ID_TIMER, getId()), boost::bind(&JailbreakEvent::onGameTimerMessage, this, _1));

		// Start timer for fail condition
		EM5_GAMETIMERS.addTimer(qsf::MessageConfiguration(MESSAGE_ID_TIMER, getId(), 0), mPrisonersSeekTime);
	}

	bool JailbreakEvent::onFailure(EventResult& eventResult)
	{
		// Call base implementation
		return FreeplayEvent::onFailure(eventResult);
	}

	void JailbreakEvent::updateFreeplayEvent(const qsf::Time& timePassed)
	{
		checkObjectivesState();
	}

	void JailbreakEvent::hintCallback(Observer& hintObserver)
	{
		showHint(hintObserver.getName(), false);
	}

	const qsf::Entity* JailbreakEvent::getFocusEntity()
	{
		return EventHelper(*this).getRandomEventEntity();
	}

	bool JailbreakEvent::checkEventConfiguration()
	{
		// No prisoners
		if (mPrisonerIds.empty())
			return false;

		for (uint64 entityId : mPrisonerIds)
		{
			qsf::Entity* entity = getMap().getEntityById(entityId);
			if (nullptr == entity)
				return false;
		}

		return true;
	}

	void JailbreakEvent::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base implementation
		FreeplayEvent::serialize(serializer);

		serializer.serialize(mPrisonerIds);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void JailbreakEvent::startHintObservers()
	{
		// hint 01: no sek drone search action started after x sec.
		createGeneralObserver<HintMessageGeneralObserver>(JAILBREAK_HINT_01).initialize(
			qsf::MessageConfiguration(Messages::EM5_POST_COMMAND_EXECUTE, SEKDroneSearchCommand::PLUGINABLE_ID), mHint01WaitTime, false);
	}

	void JailbreakEvent::onGameTimerMessage(const qsf::MessageParameters& parameters)
	{
		const uint32 timerId = parameters.getFilter<uint32>(2);
		switch (timerId)
		{
			case 0:
			{
				if (!mAllGangsterDetectByDrone)
				{
					// Abort event, one ore more gangster are invisible
					ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_FAIL_GANGSTERNOTFOUND).increaseCurrentNumber();
				}
				break;
			}
		}
	}

	bool JailbreakEvent::addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason)
	{
		if (eventSpreadReason == eventspreadreason::INJURY)
		{
			Objective& failConditionPersonDiedObjective = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_FAIL_DEADPERSONS);
			failConditionPersonDiedObjective.setNeededNumber(1);
			createObserver<PersonDiedObserver>(targetEntity.getId()).connectToObjective(failConditionPersonDiedObjective);
		}
		return FreeplayEvent::addEntityToEvent(targetEntity, eventSpreadReason, newReason);
	}

	void JailbreakEvent::setupGangster(qsf::Entity& gangsterEntity)
	{
		// Hide the gangster
		qsf::game::HiddenComponent::setHidden(gangsterEntity, true, qsf::game::HiddenComponent::HIDDENTYPE_DEFAULT);

		// Turn into gangster, use civil armed because gangster should flee from police
		EntityHelper(gangsterEntity).turnIntoGangster("em5::GangsterBaseLogic", false);
	}

	void JailbreakEvent::onPrisonerFound(const qsf::MessageParameters& parameters)
	{
		// Get prisoner ID
		uint64 entityId = parameters.getFilter(1);
		qsf::Entity* gangsterEntity = getMap().getEntityById(entityId);
		if (nullptr == gangsterEntity)
		{
			QSF_WARN("Could not find Gangster " << entityId << " for jail break event!", QSF_REACT_NONE);
			return;
		}

		// Final setup for gangster
		EntityHelper(gangsterEntity).turnIntoGangsterByType("JailBreakGangster");
		GangsterBaseLogic* gangsterBaseLogic = gangsterEntity->getOrCreateComponent<qsf::GameLogicComponent>()->getGameLogic<GangsterBaseLogic>();

		// Random chance to have armed gangster
		if (qsf::Random::getRandomChance(mArmedChance))
		{
			gangsterBaseLogic->setAutoTargetReaction(GangsterBaseLogic::AutoReaction::ATTACK);
		}

		// Set the escape target tag
		gangsterBaseLogic->setEscapeTargetTag(mEscapeTargetTag);

		// Increase gangster seek objective
		ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_FINDKILLERWITHSEKDRONE).increaseCurrentNumber(gangsterEntity);

		// Add new objectives
		{
			ObjectiveHelper objectiveHelper(*this);
			ObserverHelper observerHelper(*this);

			// Get or create conditions
			Objective& failConditionPersonDiedObjective = objectiveHelper.getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_FAIL_DEADPERSONS);
			failConditionPersonDiedObjective.setNeededNumber(1);

			Objective& failConditionPersonEscapedObjective = objectiveHelper.getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_FAIL_PERSONSESCAPE);
			failConditionPersonEscapedObjective.setNeededNumber(1);

			Objective& needConditionArrestPersonObjective = objectiveHelper.getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_ARRESTPERSONS);
			needConditionArrestPersonObjective.increaseNeededNumber();

			// Create and connect observers
			PersonDiedObserver& observerDied = observerHelper.createObserver<PersonDiedObserver>(entityId);
			observerDied.connectToObjective(failConditionPersonDiedObjective);

			TransportedToHqObserver& observerTransported = observerHelper.createObserver<TransportedToHqObserver>(entityId);
			observerTransported.connectToObjective(needConditionArrestPersonObjective);

			GangsterEscapedObserver& observerEscaped = observerHelper.createObserver<GangsterEscapedObserver>(entityId);
			observerEscaped.connectToObjective(failConditionPersonEscapedObjective);
		}

		// Find and unregister the message proxy
		for (size_t index = 0; index < mPrisonerIds.size(); ++index)
		{
			if (mPrisonerIds[index] == entityId)
			{
				mPrisonerFoundMessageProxies[index].unregister();
				break;
			}
		}

		// Unregister the timer if all proxies are unregistered (this means all prisoners are found)
		mAllGangsterDetectByDrone = true;
		for (qsf::MessageProxy& messageProxy : mPrisonerFoundMessageProxies)
		{
			if (messageProxy.isValid())
			{
				mAllGangsterDetectByDrone = false;
				break;
			}
		}

		// Show hint 02
		showHint(JAILBREAK_HINT_02, false);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
