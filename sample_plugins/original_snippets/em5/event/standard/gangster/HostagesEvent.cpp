// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/gangster/HostagesEvent.h"
#include "em5/base/GameplayAssets.h"
#include "em5/freeplay/eventspread/EventSpreadHelper.h"
#include "em5/freeplay/objective/ObjectiveHelper.h"
#include "em5/logic/local/police/GangsterInBuildingLogic.h"
#include "em5/logic/cleanup/LayerCleanup.h"
#include "em5/logic/HintHelper.h"
#include "em5/map/MapHelper.h"

#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/map/layer/LayerManager.h>
#include <qsf/map/layer/Layer.h>
#include <qsf/math/Random.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 HostagesEvent::FREEPLAY_EVENT_ID = qsf::StringHash("em5::HostagesEvent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	HostagesEvent::HostagesEvent() :
		mGangsterInsideBuildingAmount(1),
		mHostagesAmount(2),
		mInjuredHostageAmount(0),
		mGangsterGiveUp(false),
		mHostageSpawnPool(assets::SPAWNPOOL_DEFAULT_PERSON),
		mGangsterLogicName("em5::GangsterCivilUnarmedLogic")
	{
		// Nothing here
	}

	HostagesEvent::~HostagesEvent()
	{
		// Nothing here
	}

	void HostagesEvent::init(qsf::Entity& entity, bool gangsterGiveUp, const std::string& layerName)
	{
		mTargetBuilding = entity;
		mGangsterGiveUp = gangsterGiveUp;
		mLayerName = layerName;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	bool HostagesEvent::onStartup()
	{
		if (!mTargetBuilding.valid())
			return false;

		BuildingComponent* buildingComponent = mTargetBuilding->getComponent<BuildingComponent>();
		if (nullptr == buildingComponent)
			return false;

		if (!mLayerName.empty())
		{
			qsf::Layer* layer = getMap().getLayerManager().getLayerByName(qsf::StringHash(mLayerName));
			if (nullptr == layer || layer->isActive())
			{
				// Prevent layer cleanup afterwards, especially in case the layer is active already
				mLayerName.clear();
			}
			else
			{
				// Activate layer
				layer->activateLayer();
			}
		}

		// Register the building to the event
		// This line is very important for the event spreading of gangsters inside. See line below
		EventSpreadHelper::setEventIdComponent(this, mTargetBuilding.getSafe(), eventspreadreason::NO_REASON);

		// Spawn gangster inside the building
		buildingComponent->spawnGangsterInside(mGangsterInsideBuildingAmount, mHostagesAmount, mInjuredHostageAmount);

		// Tell the GangsterInBuildingLogic about the chance to get the person outside
		qsf::GameLogicComponent* gameLogicComponent = mTargetBuilding->getComponent<qsf::GameLogicComponent>();
		if (nullptr != gameLogicComponent)
		{
			GangsterInBuildingLogic* gangsterInBuildingLogic = gameLogicComponent->getGameLogic<GangsterInBuildingLogic>();
			if (nullptr != gangsterInBuildingLogic)
			{
				// Set the GiveUp value we get from the factory (let the factory calculate the random facts)
				gangsterInBuildingLogic->setGangsterGiveUp(mGangsterGiveUp);
			}
		}

		// Event is now running
		setRunning();

		// Done
		return true;
	}

	void HostagesEvent::onShutdown()
	{
		if (!mLayerName.empty())
		{
			// Ensure everything from the event is cleaned up
			qsf::Layer* layer = getMap().getLayerManager().getLayerByName(qsf::StringHash(mLayerName));
			if (nullptr != layer)
			{
				LayerCleanup::createLayerCleanup(*layer);
			}
		}
	}

	void HostagesEvent::onRun()
	{
		// Show supervisor message
		showSupervisorMessage(HintHelper::getRandomStringOfIdString(HintHelper(*this).getHintParameters().mSupervisorMessageIds));

		// Activate the hint observers
		startHintObservers();

		// Add fail objective
		ObjectiveHelper(*this).addDeadCivilPersonsFailCondition(1);
	}

	bool HostagesEvent::onFailure(EventResult& eventResult)
	{
		const uint32 failObjective = getFailReasonObjectiveTypeId(eventResult.mObjectives);
		if (ObjectiveHelper::OBJECTIVE_FAIL_DEADPERSONS == failObjective)
		{
			setFailHintsForMiniEvent(eventResult, "EM5_GE08_FAIL01", "EM5_GE01_SUPERV_FAIL01", "EM5_EVENT_TOO_MANY_PERSONS_DIED_HINT_03");
			return true;
		}

		// Call base implementation
		return FreeplayEvent::onFailure(eventResult);
	}

	void HostagesEvent::updateFreeplayEvent(const qsf::Time& timePassed)
	{
		// Check objectives for success or failure
		checkObjectivesState();
	}

	void HostagesEvent::hintCallback(Observer& hintObserver)
	{
		// TODO(mk) Add hint callback
	}

	const qsf::Entity* HostagesEvent::getFocusEntity()
	{
		// Show random event entity
		return getRandomEventEntity();
	}

	bool HostagesEvent::checkEventConfiguration()
	{
		// TODO(mk) Check if this is right
		// Check all gangsters
		for (uint64 gangsterId : mGangsterIds)
		{
			if (nullptr == getMap().getEntityById(gangsterId))
				return false;
		}

		// Check all hostages
		for (uint64 hostageId : mVictimIds)
		{
			if (nullptr == getMap().getEntityById(hostageId))
				return false;
		}

		return true;
	}

	void HostagesEvent::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base implementation
		FreeplayEvent::serialize(serializer);

		// TODO(mk) write serialize function
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void HostagesEvent::startHintObservers()
	{
		// TODO(mk) Implement me!
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
