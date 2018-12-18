// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/destruction/DefectEboxEvent.h"
#include "em5/audio/AudioHelper.h"
#include "em5/audio/AudioProxy.h"
#include "em5/component/objects/EboxComponent.h"
#include "em5/component/objects/UsableByEngineerComponent.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/freeplay/objective/ObjectiveHelper.h"
#include "em5/game/Game.h"
#include "em5/logic/ObserverHelper.h"
#include "em5/logic/observer/PersonDiedObserver.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/EM5Helper.h"

#include <qsf/map/Entity.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 DefectEboxEvent::FREEPLAY_EVENT_ID = qsf::StringHash("em5::DefectEboxEvent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	DefectEboxEvent::DefectEboxEvent() :
		mGapeChance(0.25f),
		mReactionRadius(10.0f),
		mInjuredPersonsToStartFire(4),
		mFireStartTime(qsf::Time::fromSeconds(180.0f)),
		mRepairTime(qsf::Time::fromSeconds(10.0f)),
		mTimeSinceStart(qsf::Time::ZERO),
		mCurrentEboxState(EboxState::STATE_DEFECT)
	{
		// Nothing here
	}

	DefectEboxEvent::~DefectEboxEvent()
	{
		// Nothing here
	}

	void DefectEboxEvent::setEboxEntity(qsf::Entity& entity)
	{
		mEboxEntity = entity;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	bool DefectEboxEvent::onStartup()
	{
		// Check if the ebox entity exists
		if (!mEboxEntity.valid())
			return false;

		mCurrentEboxState = EboxState::STATE_DEFECT;

		// Initialize defect ebox
		{
			EboxComponent& eboxComponent = mEboxEntity->getOrCreateComponentSafe<EboxComponent>();
			eboxComponent.setEboxDefectiveWithGaping(mGapeChance, mReactionRadius);

			// Add component for using with engineer
			UsableByEngineerComponent& usableByEngineerComponent = mEboxEntity->getOrCreateComponentSafe<UsableByEngineerComponent>();
			usableByEngineerComponent.initWithUseTime(mRepairTime);

			// Add to event
			EventIdComponent::addToEvent(mEboxEntity.getSafe(), *this, eventspreadreason::EVENT_SPECIFIC);

			// Create event location entity
			createEventLocationEntity(mEboxEntity.getSafe());
		}

		// Create message listener for repairing
		mRepairMessageProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_ACTION_REPAIRED_OR_USED, mEboxEntity.getSafe().getId()), boost::bind(&DefectEboxEvent::onRepaired, this, _1));

		// Event is now running
		setRunning();

		// Done
		return true;
	}

	void DefectEboxEvent::onShutdown()
	{
		if (mEboxEntity.valid())
		{
			// Stop defect simulation
			EboxComponent* eBoxComponent = mEboxEntity->getComponent<EboxComponent>();
			if (nullptr != eBoxComponent)
			{
				eBoxComponent->setEboxIntact();
			}
		}
		mRepairMessageProxy.unregister();
	}

	void DefectEboxEvent::onRun()
	{
		// Play supervisor message
		showSupervisorMessage("EM5_EVENT_SUPERV_OBJ_CONTROLBOX_01");

		startObjectives(mEboxEntity.getSafe());
	}

	void DefectEboxEvent::updateFreeplayEvent(const qsf::Time& time)
	{
		mTimeSinceStart += time;

		switch (mCurrentEboxState)
		{
			case STATE_INTACT:
			{
				break;
			}

			case STATE_DEFECT:
			{
				ObjectiveHelper objectiveHelper(*this);
				Objective* treatPersonsObjective = objectiveHelper.getObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_TREATPERSONS);

				// Check if its repaired by a engineer
				UsableByEngineerComponent* usableByEngineerComponent = mEboxEntity->getComponent<UsableByEngineerComponent>();
				if (nullptr != usableByEngineerComponent && usableByEngineerComponent->getIsInUse())
				{
					break;
				}
				// Check if the ebox is repaired
				else if (!mEboxEntity.getSafe().getOrCreateComponentSafe<EboxComponent>().isDefect())
				{
					mCurrentEboxState = EboxState::STATE_INTACT;
				}
				// Check if ebox should start burning
				else if (mTimeSinceStart > mFireStartTime || (nullptr != treatPersonsObjective && treatPersonsObjective->getCurrentNumber() >= mInjuredPersonsToStartFire))
				{
					// Stop simulation
					mEboxEntity.getSafe().getOrCreateComponentSafe<EboxComponent>().setEboxIntact();

					// Remove objectives
					objectiveHelper.removeObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_REPAIREBOXES);

					// Remove repairable component
					mEboxEntity->destroyComponent<UsableByEngineerComponent>();

					// Start fire
					EntityHelper(mEboxEntity.getSafe()).startFire(this);

					// Play audio
					AudioProxy audioProxy;
					AudioHelper::playControlBoxStartBurning(audioProxy, mEboxEntity.getSafe());
					audioProxy.detach();

					mCurrentEboxState = EboxState::STATE_BURNING;
				}
				break;
			}

			case STATE_BURNING:
			{
				// Nothing here, but indicates that the "main" part of the event is over
				break;
			}
		}

		// Check objectives for success or failure
		checkObjectivesState();
	}

	bool DefectEboxEvent::addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason)
	{
		if (eventspreadreason::INJURY == eventSpreadReason)
		{
			// Get or create fail conditions
			Objective& failConditionPersonDiedObjective = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_FAIL_DEADPERSONS);

			// Create and connect observers
			PersonDiedObserver& observerDied = ObserverHelper(*this).createObserver<PersonDiedObserver>(targetEntity.getId());
			observerDied.connectToObjective(failConditionPersonDiedObjective);

			// Call base implementation
			FreeplayEvent::addEntityToEvent(targetEntity, eventSpreadReason, newReason);

			// Done
			return true;
		}

		// Call base implementation
		return FreeplayEvent::addEntityToEvent(targetEntity, eventSpreadReason, newReason);
	}

	void DefectEboxEvent::hintCallback(Observer& hintObserver)
	{
		showHint(hintObserver.getName(), false);
	}

	const qsf::Entity* DefectEboxEvent::getFocusEntity()
	{
		return getEventLocationEntity();
	}

	bool DefectEboxEvent::checkEventConfiguration()
	{
		return true;
	}

	void DefectEboxEvent::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base implementation
		FreeplayEvent::serialize(serializer);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void DefectEboxEvent::startObjectives(const qsf::Entity&)
	{
		// Create the need condition
		Objective& needConditionRepairEBox = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_REPAIREBOXES);
		needConditionRepairEBox.setNeededNumber(1); // Hard scripted value

		// Create the fail condition
		Objective& failConditionPersonDiedObjective = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_FAIL_DEADPERSONS);
		failConditionPersonDiedObjective.setNeededNumber(1); // Hard scripted value
	}

	void DefectEboxEvent::onRepaired(const qsf::MessageParameters& parameters)
	{
		// Complete objective
		ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_REPAIREBOXES).setAccomplished();

		// Remove repairable component
		mEboxEntity->destroyComponent<UsableByEngineerComponent>();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
