// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/gangster/PillageBuildingEvent.h"
#include "em5/action/move/escape/EscapeFootLongAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/freeplay/eventspread/EventSpreadHelper.h"
#include "em5/freeplay/observer/Observer.h"
#include "em5/freeplay/objective/ObjectiveHelper.h"
#include "em5/logic/local/police/GangsterInBuildingLogic.h"
#include "em5/logic/cleanup/LayerCleanup.h"
#include "em5/logic/HintHelper.h"
#include "em5/map/CollisionHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/plugin/Messages.h"

#include <qsf/audio/component/DynamicMusicCompositorComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/action/WaitAction.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/log/LogSystem.h>
#include <qsf/math/Random.h>
#include <qsf/map/layer/LayerManager.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 PillageBuildingEvent::FREEPLAY_EVENT_ID = qsf::StringHash("em5::PillageBuildingEvent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PillageBuildingEvent::PillageBuildingEvent() :
		mGangsterInsideBuilding(3),
		mGangsterOutsideBuilding(3),
		mGangsterLogicName("em5::GangsterCivilUnarmedLogic"),
		mGangsterInBuilding(true),
		mFailAttackBuildingTimer(qsf::Time::fromSeconds(180.0f))
	{
		// Nothing here
	}

	PillageBuildingEvent::~PillageBuildingEvent()
	{
		// Nothing here
	}

	void PillageBuildingEvent::init(qsf::Entity& targetBuilding)
	{
		mTargetBuilding = &targetBuilding;
	}

	void PillageBuildingEvent::setAmbientLayerName(const std::string& layerName)
	{
		mAmbientLayerName = layerName;
	}

	void PillageBuildingEvent::setGangsterLogicName(const std::string& gangsterLogicName)
	{
		mGangsterLogicName = gangsterLogicName;
	}

	const std::string& PillageBuildingEvent::getGangsterLogicName() const
	{
		return mGangsterLogicName;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	bool PillageBuildingEvent::onStartup()
	{
		if (!mTargetBuilding.valid())
			return false;

		BuildingComponent* buildingComponent = mTargetBuilding->getComponent<BuildingComponent>();
		if (nullptr == buildingComponent)
			return false;

		// Register the building to the event
		EventSpreadHelper::setEventIdComponent(this, mTargetBuilding.getSafe(), eventspreadreason::NO_REASON);

		// Spawn gangster inside the building
		buildingComponent->spawnGangsterInside(mGangsterInsideBuilding, 0, 0);

		// Activate ambient layer, this ambient layer also contains the flee points, so technical its not ambient
		// Activate the layer before the gangster spawnpoints are set
		if (!mAmbientLayerName.empty())
		{
			MapHelper(getMap()).activateLayerByName(mAmbientLayerName, true);
		}

		glm::vec3 finalPosition = buildingComponent->getDoorPosition();
		glm::quat finalRotation = buildingComponent->getDoorRotation();
		qsf::Transform doorTransform(finalPosition, finalRotation);
		for (int i = 0; i < mGangsterOutsideBuilding; ++i)
		{
			spawnGangsterOutsideBuilding(doorTransform, i);
		}

		// TODO(mk) Read value from definition
		// Allow normal police to enter
		if (true)
		{
			qsf::GameLogicComponent* gameLogicComponent = mTargetBuilding->getComponent<qsf::GameLogicComponent>();
			if (nullptr != gameLogicComponent)
			{
				GangsterInBuildingLogic* gangsterInBuildingLogic = gameLogicComponent->getGameLogic<GangsterInBuildingLogic>();
				if (nullptr != gangsterInBuildingLogic)
				{
					gangsterInBuildingLogic->setAllowNormalPolice(true);
				}
			}
		}

		// Start the event
		setRunning();

		// Register message handler for person rescued from building
		mAttackBuildingFinishedMessageProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_ACTION_ATTACK_BUILDING_FINISH, mTargetBuilding->getId()), boost::bind(&PillageBuildingEvent::onAttackBuilding, this, _1));

		return true;
	}

	void PillageBuildingEvent::onShutdown()
	{
		mAttackBuildingFinishedMessageProxy.unregister();

		// Ensure everything from the event is cleaned up.
		if (!mAmbientLayerName.empty())
		{
			qsf::Layer* layer = getMap().getLayerManager().getLayerByName(qsf::StringHash(mAmbientLayerName));
			if (nullptr != layer)
			{
				if (isMiniEvent())
				{
					MapHelper(getMap()).deactivateLayerByName(mAmbientLayerName, false);
				}
				else
				{
					LayerCleanup::createLayerCleanup(*layer);
				}
			}
		}
	}

	void PillageBuildingEvent::onRun()
	{
		// Show supervisor message
		showSupervisorMessage(HintHelper::getRandomStringOfIdString(HintHelper(*this).getHintParameters().mSupervisorMessageIds));
	}

	void PillageBuildingEvent::updateFreeplayEvent(const qsf::Time& timePassed)
	{
		// Check objectives for success or failure
		checkObjectivesState();

		// Update event fail check
		if (mGangsterInBuilding)
		{
			mFailAttackBuildingTimer -= timePassed;
			if (mFailAttackBuildingTimer <= qsf::Time::ZERO)
			{
				QSF_LOG_PRINTS(DEBUG, "PillageBuilding event failed: Event time was over");
				setFinished(false);
			}
		}
	}

	bool PillageBuildingEvent::onFailure(EventResult& eventResult)
	{
		if (isMiniEvent() || (mGangsterInBuilding && mFailAttackBuildingTimer <= qsf::Time::ZERO))
		{
			// To solve this issue we play this hint in every case for the
			eventResult.mFailedText = "EM5X_MINI06_FAIL01";
			const uint32 failObjective = getFailReasonObjectiveTypeId(eventResult.mObjectives);
			if (ObjectiveHelper::OBJECTIVE_FAIL_DEADPERSONS == failObjective)
			{
				eventResult.mFailedText = "EM5_GE07_FAIL02";
			}
			if (ObjectiveHelper::OBJECTIVE_FAIL_DEADSQUADPERSONS == failObjective)
			{
				eventResult.mFailedText = "EM5_GE08_FAIL02";
			}

			if (isMiniEvent())
			{
				showSupervisorMessage("EM5X_MINI06_SUPERV_FAIL01");
			}
			return true;
		}

		// Call base implementation
		return FreeplayEvent::onFailure(eventResult);
	}

	bool PillageBuildingEvent::addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason)
	{
		if (eventspreadreason::INJURY == eventSpreadReason)
		{
			// Manually decrement the transport gangster objective
			if (EntityHelper(targetEntity).isGangsterPerson())
			{
				ObjectiveHelper objectiveHelper(*this);
				Objective& arrestObjective = objectiveHelper.getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_ARRESTPERSONS);
				if (arrestObjective.getNeededNumber() > 1)
				{
					arrestObjective.setNeededNumber(arrestObjective.getNeededNumber() - 1);
				}
				else
				{
					objectiveHelper.removeObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_ARRESTPERSONS);
				}
			}
		}

		// Call base implementation
		return FreeplayEvent::addEntityToEvent(targetEntity, eventSpreadReason, newReason);
	}

	void PillageBuildingEvent::hintCallback(Observer& hintObserver)
	{
		// TODO(mk) This function is copy pasted from (BuildingCollapseEvent) check if this is ok here
		const std::string& observerName = hintObserver.getName();

		if (!wasHintShown(observerName))
		{
			showHint(observerName, true);

			// Short-Action-Music on EM5_EVENT_BUILDING_COLLAPSED_HINT_02
			getDynamicMusicCompositor().setMusicLevel(3);
		}
	}

	bool PillageBuildingEvent::checkEventConfiguration()
	{
		return true;
	}

	const qsf::Entity* PillageBuildingEvent::getFocusEntity()
	{
		// Show random event entity
		return getRandomEventEntity();
	}

	void PillageBuildingEvent::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base implementation
		FreeplayEvent::serialize(serializer);

		// TODO(mk) Add serialization
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void PillageBuildingEvent::onAttackBuilding(const qsf::MessageParameters& parameters)
	{
		// Disable a event fail
		mGangsterInBuilding = false;
	}

	void PillageBuildingEvent::spawnGangsterOutsideBuilding(const qsf::Transform& spawnPosition, int gangsterIndex)
	{
		qsf::Entity* gangster = MapHelper(QSF_MAINMAP).getOrCreateGangsterCandidateInGangsterEventArea(GangsterPool::DEFAULT_EXTENDED);
		if (gangster)
		{
			EntityHelper entityHelper(*gangster);
			entityHelper.turnIntoGangster(qsf::StringHash(mGangsterLogicName), false);

			// Place the gangster in front of the building
			glm::vec3 finalPosition = spawnPosition.getPosition();
			CollisionHelper(getMap()).findFreePositionEM3Safety(*gangster, spawnPosition, 2.0f, 7.0f, 15.0f, 1.0f, finalPosition);
			entityHelper.setPosition(finalPosition);

			qsf::ActionComponent& actionComponent = gangster->getOrCreateComponentSafe<qsf::ActionComponent>();

			// Clear remaining move and despawn actions
			actionComponent.pushAction<qsf::WaitAction>(action::AUTOMATISM_LOW, qsf::action::REPLACE_WHOLE_PLAN).init(qsf::Time::fromSeconds(2.0f));

			// The level has extra edited points for fleeing
			std::stringstream builder;
			builder << "Pillage_Supermarket_flee" << (gangsterIndex + 1);
			actionComponent.pushAction<EscapeFootLongAction>(action::AUTOMATISM_STD, qsf::action::APPEND_TO_BACK).init(builder.str());

			// Add the person to the event
			EventSpreadHelper::setEventIdComponent(this, *gangster, eventspreadreason::GANGSTER);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
