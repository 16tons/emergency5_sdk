// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/destruction/BuildingNearlyCollapseEvent.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/component/objects/UsableByEngineerComponent.h"
#include "em5/component/overlay/StatusOverlayComponent.h"
#include "em5/freeplay/objective/ObjectiveHelper.h"
#include "em5/freeplay/observer/Observer.h"
#include "em5/logic/ObserverHelper.h"
#include "em5/logic/cleanup/LayerCleanup.h"
#include "em5/logic/HintHelper.h"
#include "em5/game/Game.h"
#include "em5/game/minimap/MinimapHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/EM5Helper.h"

#include <qsf/audio/component/DynamicMusicCompositorComponent.h>
#include <qsf/math/Random.h>
#include <qsf/map/layer/LayerManager.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 BuildingNearlyCollapseEvent::FREEPLAY_EVENT_ID = qsf::StringHash("em5::BuildingNearlyCollapseEvent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	BuildingNearlyCollapseEvent::BuildingNearlyCollapseEvent() :
		mIsTutorial(false),
		mTutorialHint1Played(false),
		mTutorialHint2Played(false),
		mTutorialHint3Played(false),
		mIsBuildingEvacuated(false),
		mIsBuildingReparied(false),
		mFailEvacuateBuildingTimer(qsf::Time::fromSeconds(160.f)),
		mFailRepairBuildingTimer(qsf::Time::fromSeconds(240.f))
	{
		// Nothing here
	}

	BuildingNearlyCollapseEvent::~BuildingNearlyCollapseEvent()
	{
		// Nothing here
	}

	void BuildingNearlyCollapseEvent::init(qsf::Entity& targetBuilding, bool isTutorial)
	{
		mTargetBuilding = &targetBuilding;
		mIsTutorial = isTutorial;
	}

	void BuildingNearlyCollapseEvent::setAmbientLayerName(const std::string& layerName)
	{
		mAmbientLayerName = layerName;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	bool BuildingNearlyCollapseEvent::onStartup()
	{
		if (!mTargetBuilding.valid())
			return false;

		BuildingComponent* buildingComponent = mTargetBuilding->getComponent<BuildingComponent>();
		if (nullptr == buildingComponent)
			return false;

		// Trap persons inside the building
		buildingComponent->setupTrappedInjuredPersons(2, 4, 1, 1, true, true);
		buildingComponent->createTrappedPerson(*this);

		// Destroy the building for reaching a better optical effect
		buildingComponent->setDamageState(DamageComponent::DAMAGESTATE_DAMAGE_LEVEL2);

		// Activate ambient layer
		if (!mAmbientLayerName.empty())
		{
			MapHelper(getMap()).activateLayerByName(mAmbientLayerName, true);
		}

		// Start the event
		setRunning();

		// Register message handler for person rescued from building
		mRescuedFromBuildingMessageProxy.registerAt(qsf::MessageConfiguration(
			Messages::EM5_ACTION_FREE_PERSON_FROM_BUILDING), boost::bind(&BuildingNearlyCollapseEvent::onPersonRescueFromBuilding, this, _1));

		mBreakBuildingDoorMessageProxy.registerAt(qsf::MessageConfiguration(
			Messages::EM5_ACTION_BREAK_BUILDING_DOOR_OPEN_END), boost::bind(&BuildingNearlyCollapseEvent::onBreakBuildingDoor, this, _1));

		mTutorialHint1Timer = qsf::Time::fromSeconds(15.f);

		return true;
	}

	void BuildingNearlyCollapseEvent::onShutdown()
	{
		mRescuedFromBuildingMessageProxy.unregister();
		mRepairBuildingMessageProxy.unregister();
		mBreakBuildingDoorMessageProxy.unregister();

		// Ensure everything from the event is cleaned up
		if (!mAmbientLayerName.empty())
		{
			qsf::Layer* layer = getMap().getLayerManager().getLayerByName(qsf::StringHash(mAmbientLayerName));
			if (nullptr != layer)
			{
				LayerCleanup::createLayerCleanup(*layer);
			}
		}

		// Remove extra created icon
		if (mTargetBuilding.valid() && !mIsBuildingEvacuated)	// Special case, if persons still inside, keep the icon
		{
			StatusOverlayComponent::hideIcon(*mTargetBuilding, StatusOverlayComponent::STATUS_ICON_EVENT_VARIOUS);
			MinimapHelper::unregisterIcon(*mTargetBuilding);
		}
	}

	void BuildingNearlyCollapseEvent::onRun()
	{
		// Show supervisor message
		showSupervisorMessage(HintHelper::getRandomStringOfIdString(HintHelper(*this).getHintParameters().mSupervisorMessageIds));

		// Setup fail conditions - note that most are checked by hand in "updateFreeplayEvent"!
		ObjectiveHelper objectiveHelper(*this);
		objectiveHelper.addDeadPersonsFailCondition(2);
	}

	void BuildingNearlyCollapseEvent::updateFreeplayEvent(const qsf::Time& timePassed)
	{
		// Check objectives for success or failure
		checkObjectivesState();

		if (mIsTutorial)
		{
			// Show tutorial hints
			if (!mTutorialHint1Played)
			{
				mTutorialHint1Timer -= timePassed;
				if (mTutorialHint1Timer <= qsf::Time::ZERO)
				{
					// Play hint
					showSupervisorMessage("EM5X_MINI01_SUPERV_TUT01");
					mTutorialHint1Played = true;
				}
			}
		}

		// Update evacuate building fail timer
		if (!mIsBuildingEvacuated)
		{
			mFailEvacuateBuildingTimer -= timePassed;
			if (mFailEvacuateBuildingTimer <= qsf::Time::ZERO)
			{
				setFinished(false);
			}
		}

		if (!mIsBuildingReparied)
		{
			if (mTargetBuilding.valid())
			{
				// Minimap icon for repairing the building
				StatusOverlayComponent::showIcon(*mTargetBuilding, StatusOverlayComponent::STATUS_ICON_EVENT_VARIOUS, true);
				MinimapHelper::registerEventIcon(*mTargetBuilding);
			}

			mFailRepairBuildingTimer -= timePassed;
			if (mFailRepairBuildingTimer <= qsf::Time::ZERO)
			{
				setFinished(false);
			}
		}

		// update tutorial hint
		if (mIsTutorial && !mTutorialHint3Played)
		{
			// Play hint only in the possible time frame...
			if (mIsBuildingEvacuated && !mIsBuildingReparied)
			{
				// ... and with all conditions matching
				BuildingComponent* buildingComponent = mTargetBuilding->getComponent<BuildingComponent>();
				if (buildingComponent != nullptr && !buildingComponent->isContainingTrappedPersons())
				{
					// Play hint after all trapped persons are outside the building
					showSupervisorMessage("EM5X_MINI01_SUPERV_TUT03");
					mTutorialHint3Played = true;
				}
			}
		}
	}

	bool BuildingNearlyCollapseEvent::onFailure(EventResult& eventResult)
	{
		// Check the event fail reason
		const uint32 failObjective = getFailReasonObjectiveTypeId(eventResult.mObjectives);
		if (!mIsBuildingEvacuated && mFailEvacuateBuildingTimer <= qsf::Time::ZERO)
		{
			setFailHintsForMiniEvent(eventResult, "EM5X_MINI01_FAIL01", "EM5X_MINI01_SUPERV_FAIL01");
		}
		else if (!mIsBuildingReparied && mFailRepairBuildingTimer <= qsf::Time::ZERO)
		{
			setFailHintsForMiniEvent(eventResult, "EM5X_MINI01_FAIL02", "EM5X_MINI01_SUPERV_FAIL02");
		}
		else if (failObjective == ObjectiveHelper::OBJECTIVE_FAIL_DEADPERSONS)
		{
			setFailHintsForMiniEvent(eventResult, "EM5_GE14_FAIL01", "EM5_GE01_SUPERV_FAIL02");
		}
		else
		{
			// Call base implementation
			return FreeplayEvent::onFailure(eventResult);
		}

		return true;
	}

	bool BuildingNearlyCollapseEvent::addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason)
	{
		ObserverHelper observerHelper(*this);

		// Call base implementation
		return FreeplayEvent::addEntityToEvent(targetEntity, eventSpreadReason, newReason);
	}

	void BuildingNearlyCollapseEvent::hintCallback(Observer& hintObserver)
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

	bool BuildingNearlyCollapseEvent::checkEventConfiguration()
	{
		return true;
	}

	const qsf::Entity* BuildingNearlyCollapseEvent::getFocusEntity()
	{
		// Show random event entity
		return getRandomEventEntity();
	}

	void BuildingNearlyCollapseEvent::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base implementation
		FreeplayEvent::serialize(serializer);

		// TODO(mk) Add serialization
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void BuildingNearlyCollapseEvent::onPersonRescueFromBuilding(const qsf::MessageParameters& parameters)
	{
		uint64 buildingId = qsf::getUninitialized<uint64>();
		parameters.getParameter("buildingId", buildingId);

		// Check if the message is send to us
		if (buildingId == mTargetBuilding->getId())
		{
			mRescuedFromBuildingMessageProxy.unregister();
			// Allow the engineer to enter the building
			Objective& repairBuildingObjective = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_REPAIRBUILDING);
			repairBuildingObjective.setNeededNumber(1);

			// This is enough to enable the engineer to enter the building
			mTargetBuilding->getOrCreateComponent<UsableByEngineerComponent>();

			// Register message handler for person rescued from building
			mRepairBuildingMessageProxy.registerAt(qsf::MessageConfiguration(
				Messages::EM5_ACTION_REPAIRED_OR_USED, mTargetBuilding->getId()), boost::bind(&BuildingNearlyCollapseEvent::onBuildingRepaired, this, _1));

			mIsBuildingEvacuated = true;
		}
	}

	void BuildingNearlyCollapseEvent::onBreakBuildingDoor(const qsf::MessageParameters& parameters)
	{
		if (mIsTutorial && !mTutorialHint2Played)
		{
			// Play hint
			showSupervisorMessage("EM5X_MINI01_SUPERV_TUT02");
			mTutorialHint2Played = true;
		}
	}

	void BuildingNearlyCollapseEvent::onBuildingRepaired(const qsf::MessageParameters& parameters)
	{
		mRepairBuildingMessageProxy.unregister();

		// Complete the objective
		Objective& repairBuildingObjective = ObjectiveHelper(*this).getOrCreateObjective(ObjectiveHelper::OBJECTIVE_NEED_REPAIRBUILDING, Objective::ObjectiveType::OBJECTIVETYPE_REQUIRED);
		repairBuildingObjective.setAccomplished();

		mIsBuildingReparied = true;

		if (mTargetBuilding.valid())
		{
			StatusOverlayComponent::hideIcon(*mTargetBuilding, StatusOverlayComponent::STATUS_ICON_EVENT_VARIOUS);
			MinimapHelper::unregisterIcon(*mTargetBuilding);
		}

		// update the icons, maybe there is something the building wants to create the icon
		BuildingComponent* buildingComponent = mTargetBuilding->getComponent<BuildingComponent>();
		if (buildingComponent != nullptr)
		{
			buildingComponent->updateIcons();
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
