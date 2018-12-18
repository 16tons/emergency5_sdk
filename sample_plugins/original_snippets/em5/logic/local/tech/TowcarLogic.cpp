// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/logic/local/tech/TowcarLogic.h"
#include "em5/logic/local/EntityLogicHelper.h"
#include "em5/plugin/Jobs.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include "em5/command/tech/PickupVehicleByTowcarCommand.h"
#include "em5/action/ActionPriority.h"
#include "em5/specs/LogicSpecsGroup.h"
#ifndef ENDUSER
	#include "em5/debug/GameDebugGroup.h"
#endif
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/EM5Helper.h"

#include <qsf/map/Map.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/job/JobArguments.h>
#ifndef ENDUSER
	#include <qsf/debug/DebugDrawManager.h>
	#include <qsf/debug/request/TextDebugDrawRequest.h>
#endif
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 TowcarLogic::GAMELOGIC_TYPE_ID = qsf::StringHash("em5::TowcarLogic");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 TowcarLogic::ACTION_PRIORITY = action::AUTOMATISM_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	TowcarLogic::TowcarLogic() :
		qsf::GameLogic(GAMELOGIC_TYPE_ID)
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	bool TowcarLogic::onStartup()
	{
		// Register job
		// -> For performance reasons, do updates not in every tick, but in fixed time intervals
		qsf::jobs::JobConfiguration jobConfiguration;
		jobConfiguration.mTimeBetweenCalls = qsf::Time::fromSeconds(1.0f);
		mSimulationJobProxy.registerAt(Jobs::SIMULATION_LOGIC, boost::bind(&TowcarLogic::updateSimulation, this, _1), jobConfiguration);
		return true;
	}

	void TowcarLogic::onShutdown()
	{
		// Unregister job
		mSimulationJobProxy.unregister();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void TowcarLogic::updateSimulation(const qsf::JobArguments& jobArguments)
	{
		// Access the caller's action plan
		const qsf::ActionComponent& actionComponent = getEntity()->getOrCreateComponentSafe<qsf::ActionComponent>();

		// Check if the paramedic is currently busy
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
			if (mTimeWaiting >= qsf::Time::fromSeconds(LogicSpecsGroup::getInstanceSafe().getTowcarMinimumWaitingTime()))
			{
				// Perform the automatism now
				pickupVehicleAutomatism();

				// Reset wait time
				mTimeWaiting = qsf::Time::ZERO;
			}
		}
	}

	void TowcarLogic::pickupVehicleAutomatism()
	{
		// Get the transform component for the position
		const qsf::TransformComponent* transformComponent = getEntity()->getComponent<qsf::TransformComponent>();
		if (nullptr == transformComponent)
			return;

		// Search for health components in the radius
		std::vector<RoadVehicleComponent*> roadVehicleComponentVector;
		qsf::ComponentMapQuery(getMap()).getInstancesInCircle(transformComponent->getPosition(), LogicSpecsGroup::getInstanceSafe().getTowcarSearchRadius(), roadVehicleComponentVector);

		for (RoadVehicleComponent* roadVehicleComponent : roadVehicleComponentVector)
		{
			if (EntityLogicHelper::checkAndExecuteAutomatism<PickupVehicleByTowcarCommand>(getEntity(), &roadVehicleComponent->getEntity()))
			{
				#ifndef ENDUSER
					if (GameDebugGroup::getInstanceSafe().getShowAutomatismDebug())
					{
						QSF_DEBUGDRAW.requestDraw(qsf::TextDebugDrawRequest("auto:PickupVehicleByTowcar", transformComponent->getPosition(), qsf::Color4::YELLOW), qsf::DebugDrawLifetimeData(qsf::Time::fromSeconds(5.0f)));
					}
				#endif
				break;
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
