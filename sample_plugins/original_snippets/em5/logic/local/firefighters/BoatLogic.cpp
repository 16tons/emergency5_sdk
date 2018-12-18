// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/logic/local/firefighters/BoatLogic.h"
#include "em5/logic/local/EntityLogicHelper.h"
#include "em5/base/ContainerCategory.h"
#include "em5/component/objects/BoatDropPointComponent.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include "em5/component/vehicle/BoatComponent.h"
#include "em5/command/tech/PickupDrowningPersonByBoatCommand.h"
#include "em5/command/tech/PlacePersonFromBoatCommand.h"
#include "em5/command/move/MoveCommand.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/action/ActionPriority.h"
#include "em5/specs/LogicSpecsGroup.h"
#include "em5/settings/GameSettingsGroup.h"
#ifndef ENDUSER
	#include "em5/debug/GameDebugGroup.h"
#endif
#include "em5/plugin/Jobs.h"
#include "em5/plugin/Messages.h"
#include "em5/EM5Helper.h"

#include <qsf_game/component/health/HealthComponent.h>
#include <qsf_game/component/event/EventTagManagerComponent.h>
#include <qsf_game/base/DistanceHelper.h>

#include <qsf/component/base/TransformComponent.h>
#ifndef ENDUSER
	#include <qsf/debug/DebugDrawManager.h>
	#include <qsf/debug/request/TextDebugDrawRequest.h>
#endif
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/math/Math.h>
#include <qsf/math/Convert.h>
#include <qsf/job/JobArguments.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>
#include <qsf/plugin/QsfJobs.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 BoatLogic::GAMELOGIC_TYPE_ID = qsf::StringHash("em5::BoatLogic");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	BoatLogic::BoatLogic() :
		qsf::GameLogic(GAMELOGIC_TYPE_ID),
		mTimeWaiting(qsf::Time::ZERO)
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	bool BoatLogic::onStartup()
	{
		// Register job
		// -> For performance reasons, do updates not in every tick, but in fixed time intervals
		qsf::jobs::JobConfiguration jobConfiguration;
		jobConfiguration.mTimeBetweenCalls = qsf::Time::fromSeconds(1.0f);
		mSimulationJobProxy.registerAt(Jobs::SIMULATION_LOGIC, boost::bind(&BoatLogic::updateSimulation, this, _1), jobConfiguration);

		return true;
	}

	void BoatLogic::onShutdown()
	{
		// Unregister job
		mSimulationJobProxy.unregister();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void BoatLogic::updateSimulation(const qsf::JobArguments& jobArguments)
	{
		// Access the caller's action plan
		const qsf::ActionComponent& actionComponent = getEntity()->getOrCreateComponentSafe<qsf::ActionComponent>();

		// Check if the dlk is currently busy
		bool isBusy = (actionComponent.getCurrentPriority() >= action::AUTOMATISM_STD);	// Overwrite idle actions and low-priority automatisms

		if (!MapHelper::isUnitAutomatismActive())
		{
			// No automatism active
			isBusy = true;
		}

		if (isBusy)
		{
			// Reset wait time
			mTimeWaiting = qsf::Time::ZERO;
		}
		else
		{
			// Wait some time until getting active
			mTimeWaiting += jobArguments.getTimePassed();

			// Waited long enough now?
			if (mTimeWaiting >= qsf::Time::fromSeconds(LogicSpecsGroup::getInstanceSafe().getMotorboatMinimumWaitingTime()))
			{
				// Perform the automatism now
				performAutomatism();

				// Reset wait time
				mTimeWaiting = qsf::Time::ZERO;
			}
		}
	}

	void BoatLogic::performAutomatism()
	{
		GameSettingsGroup::AutomatismValue automatismValue = GameSettingsGroup::getInstanceSafe().getAutomatismValue();
		if (automatismValue >= GameSettingsGroup::AUTOMATISM_ALL)
		{
			// Work with return values to avoid having more than one automatism starting at the same time
			if (rescuePersonAutomatism())
				return;

			if (placePersonAutomatism())
				return;
		}
	}

	bool BoatLogic::rescuePersonAutomatism()
	{
		glm::vec3 ownerPosition = getEntity()->getOrCreateComponentSafe<qsf::TransformComponent>().getPosition();

		// Search for health components in the radius
		std::vector<qsf::game::HealthComponent*> healthComponentVector;
		qsf::ComponentMapQuery(getMap()).getInstancesInCircle(ownerPosition, LogicSpecsGroup::getInstanceSafe().getMotorboatSearchRadius(), healthComponentVector);

		std::vector<qsf::Component*> componentVector(healthComponentVector.size());
		componentVector.clear();
		componentVector.insert(componentVector.end(), healthComponentVector.begin(), healthComponentVector.end());

		// Sort the components for the distance
		qsf::game::IsLeftComponentCloserToTargetPosition compareObject(ownerPosition);
		std::sort(componentVector.begin(), componentVector.end(), compareObject);

		for (qsf::Component* component : componentVector)
		{
			if (EntityLogicHelper::checkAndExecuteAutomatism<PickupDrowningPersonByBoatCommand>(getEntity(), &component->getEntity()))
			{
				#ifndef ENDUSER
					if (GameDebugGroup::getInstanceSafe().getShowAutomatismDebug())
					{
						QSF_DEBUGDRAW.requestDraw(qsf::TextDebugDrawRequest("auto:RescuePersonByBoat", ownerPosition, qsf::Color4::YELLOW), qsf::DebugDrawLifetimeData(qsf::Time::fromSeconds(5.0f)));
					}
				#endif
				return true;
			}
		}

		return false;
	}

	bool BoatLogic::placePersonAutomatism()
	{
		BoatComponent* boatComponent = getEntity()->getComponent<BoatComponent>();
		if (nullptr == boatComponent)
			return false;

		// Check if we have persons in the boat
		if (boatComponent->getAllLinksByContainerType(container::CONTAINERTYPE_PASSENGER).empty())
			return false;

		glm::vec3 ownerPosition = getEntity()->getOrCreateComponentSafe<qsf::TransformComponent>().getPosition();

		// Search for health components in the radius
		std::vector<BoatDropPointComponent*> boatdropComponentVector;
		qsf::ComponentMapQuery(getMap()).getInstancesInCircle(ownerPosition, LogicSpecsGroup::getInstanceSafe().getMotorboatSearchRadius(), boatdropComponentVector);

		std::vector<qsf::Component*> componentVector(boatdropComponentVector.size());
		componentVector.clear();
		componentVector.insert(componentVector.end(), boatdropComponentVector.begin(), boatdropComponentVector.end());

		// Sort the components for the distance
		qsf::game::IsLeftComponentCloserToTargetPosition compareObject(ownerPosition);
		std::sort(componentVector.begin(), componentVector.end(), compareObject);

		for (qsf::Component* component : componentVector)
		{
			if (EntityLogicHelper::checkAndExecuteAutomatism<PlacePersonFromBoatCommand>(getEntity(), &component->getEntity()))
			{
				#ifndef ENDUSER
					if (GameDebugGroup::getInstanceSafe().getShowAutomatismDebug())
					{
						QSF_DEBUGDRAW.requestDraw(qsf::TextDebugDrawRequest("auto:PlacePersonByBoat", ownerPosition, qsf::Color4::YELLOW), qsf::DebugDrawLifetimeData(qsf::Time::fromSeconds(5.0f)));
					}
				#endif
				return true;
			}
		}

		return false;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
