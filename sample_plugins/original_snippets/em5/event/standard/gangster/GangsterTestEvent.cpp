// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/gangster/GangsterTestEvent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/freeplay/objective/ObjectiveHelper.h"
#include "em5/freeplay/observer/Observer.h"
#include "em5/logic/HintHelper.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/map/EntityHelper.h"

#include <qsf/logic/action/ActionComponent.h>
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
	const uint32 GangsterTestEvent::FREEPLAY_EVENT_ID = qsf::StringHash("em5::GangsterTestEvent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GangsterTestEvent::GangsterTestEvent() :
		mGangsterId(qsf::getUninitialized<uint64>())
	{
		// Nothing here
	}

	GangsterTestEvent::~GangsterTestEvent()
	{
		// Nothing here
	}

	uint64 GangsterTestEvent::getGangsterId() const
	{
		return mGangsterId;
	}

	void GangsterTestEvent::setGangster(const qsf::Entity& entity)
	{
		mGangsterId = entity.getId();
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	bool GangsterTestEvent::onStartup()
	{
		qsf::Entity* gangsterEntity = getMap().getEntityById(mGangsterId);
		QSF_CHECK(gangsterEntity, "Can't find gangster entity; GangsterTestEvent method: onStartup", return false);

		// Remove former "jobs"
		qsf::ActionComponent& actionComponent = gangsterEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
		actionComponent.clearPlan();

		PersonComponent* personComponent = gangsterEntity->getComponent<PersonComponent>();
		if (personComponent)
		{
			if (!personComponent->getGangsterFlag())
			{
				// No gangster yet!
				EntityHelper(*gangsterEntity).turnIntoGangster(qsf::StringHash("em5::GangsterKillerPsychoLogic"), false);
			}
		}

		// Make gangster a lot bigger (Remember that this is only a test event)
		//gangsterEntity->getOrCreateComponent<qsf::TransformComponent>()->setScale(glm::vec3(2.0f));

		gangsterEntity->getOrCreateComponent<EventIdComponent>()->setEvent(*this, eventspreadreason::GANGSTER);

		// Event is now running
		setRunning();

		// Done
		return true;
	}

	void GangsterTestEvent::onShutdown()
	{
		// Nothing here
	}

	void GangsterTestEvent::onRun()
	{
		// Activate the hint observers
		startHintObservers();
	}

	bool GangsterTestEvent::onFailure(EventResult& eventResult)
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

	void GangsterTestEvent::updateFreeplayEvent(const qsf::Time& timePassed)
	{
		// Right now, this event only spawns a Killer psycho (very similar to the RunningAmok event.
		// So nothing to do here.
	}

	void GangsterTestEvent::hintCallback(Observer& hintObserver)
	{
		showHint(hintObserver.getName(), false);
	}

	const qsf::Entity* GangsterTestEvent::getFocusEntity()
	{
		return getMap().getEntityById(mGangsterId);
	}

	bool GangsterTestEvent::checkEventConfiguration()
	{
		return (nullptr != getMap().getEntityById(mGangsterId));
	}

	void GangsterTestEvent::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base implementation
		FreeplayEvent::serialize(serializer);

		serializer.serialize(mGangsterId);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void GangsterTestEvent::startHintObservers()
	{
		qsf::Entity* gangsterEntity = getMap().getEntityById(mGangsterId);
		HintHelper(*this).createAndInitializeHintsAtInjured(*gangsterEntity);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
