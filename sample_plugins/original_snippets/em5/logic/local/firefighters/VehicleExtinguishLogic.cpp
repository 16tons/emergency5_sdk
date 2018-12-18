// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/logic/local/firefighters/VehicleExtinguishLogic.h"
#include "em5/logic/local/EntityLogicHelper.h"
#include "em5/action/ActionPriority.h"
#include "em5/command/fire/ExtinguishWithCannonCommand.h"
#include "em5/fire/FireHelper.h"
#include "em5/fire/component/FireComponent.h"
#ifndef ENDUSER
	#include "em5/debug/GameDebugGroup.h"
#endif
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/plugin/Jobs.h"
#include "em5/settings/GameSettingsGroup.h"
#include "em5/specs/LogicSpecsGroup.h"
#include "em5/EM5Helper.h"

#include <qsf/component/base/TransformComponent.h>
#ifndef ENDUSER
	#include <qsf/debug/DebugDrawManager.h>
	#include <qsf/debug/request/TextDebugDrawRequest.h>
#endif
#include <qsf/job/JobArguments.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>
#include <qsf/math/Convert.h>
#include <qsf/math/Math.h>
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
	const uint32 VehicleExtinguishLogic::GAMELOGIC_TYPE_ID = qsf::StringHash("em5::VehicleExtinguishLogic");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	VehicleExtinguishLogic::VehicleExtinguishLogic() :
		qsf::GameLogic(GAMELOGIC_TYPE_ID),
		mTimeWaiting(qsf::Time::ZERO)
	{
		// Nothing here
	}

	void VehicleExtinguishLogic::extinguishAutomatism()
	{
		// Optimization: To avoid constant allocations/deallocations, use a static instance (not multi-threading safe, of course)
		static std::vector<FireComponent*> burningFireComponentVector;
		FireHelper::getSortedTargetsInRange(burningFireComponentVector, *getEntity(), FireHelper::getAutomatismDistanceFromCaller(*getEntity()));

		for (FireComponent* fireComponent : burningFireComponentVector)
		{
			// Use here the main entity, because the fire component might be part of an entity which is only a child of another entity (e.g. building)
			if (EntityLogicHelper::checkAndExecuteAutomatism<ExtinguishWithCannonCommand>(getEntity(), &fireComponent->getMainEntity()))
			{
				#ifndef ENDUSER
					if (GameDebugGroup::getInstanceSafe().getShowAutomatismDebug())
					{
						const glm::vec3 ownerPosition = getEntity()->getComponentSafe<qsf::TransformComponent>().getPosition();
						QSF_DEBUGDRAW.requestDraw(qsf::TextDebugDrawRequest("auto:ExtinguishWithExtinguisherCommand", ownerPosition, qsf::Color4::YELLOW), qsf::DebugDrawLifetimeData(qsf::Time::fromSeconds(5.0f)));
					}
				#endif
				break;
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	bool VehicleExtinguishLogic::onStartup()
	{
		// Register job
		// -> For performance reasons, do updates not in every tick, but in fixed time intervals
		qsf::jobs::JobConfiguration jobConfiguration;
		jobConfiguration.mTimeBetweenCalls = qsf::Time::fromSeconds(1.0f);
		mSimulationJobProxy.registerAt(Jobs::SIMULATION_LOGIC, boost::bind(&VehicleExtinguishLogic::updateSimulation, this, _1), jobConfiguration);

		return true;
	}

	void VehicleExtinguishLogic::onShutdown()
	{
		// Unregister job
		mSimulationJobProxy.unregister();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void VehicleExtinguishLogic::updateSimulation(const qsf::JobArguments& jobArguments)
	{
		// Access the caller's action plan
		const qsf::ActionComponent& actionComponent = getEntity()->getOrCreateComponentSafe<qsf::ActionComponent>();

		// Check if the owner is currently busy
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
			if (mTimeWaiting >= qsf::Time::fromSeconds(LogicSpecsGroup::getInstanceSafe().getFiremanMinimumWaitingTime()))
			{
				// Perform the automatism now
				performAutomatism();

				// Reset wait time
				mTimeWaiting = qsf::Time::ZERO;
			}
		}
	}

	void VehicleExtinguishLogic::performAutomatism()
	{
		GameSettingsGroup::AutomatismValue automatismValue = GameSettingsGroup::getInstanceSafe().getAutomatismValue();
		if (automatismValue >= GameSettingsGroup::AUTOMATISM_MEDIUM)
		{
			extinguishAutomatism();
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
