// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/logic/local/firefighters/DLKLogic.h"
#include "em5/logic/local/EntityLogicHelper.h"
#include "em5/action/fire/DLK/DLKUndeployAction.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include "em5/command/fire/DLK/DLKSafePersonCommand.h"
#include "em5/command/fire/DLK/DLKExtinguishCommand.h"
#include "em5/command/move/ReturnToBaseCommand.h"
#include "em5/command/move/MoveCommand.h"
#include "em5/command/base/ExitVehicleCommand.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/specs/LogicSpecsGroup.h"
#include "em5/settings/GameSettingsGroup.h"
#ifndef ENDUSER
	#include "em5/debug/GameDebugGroup.h"
#endif
#include "em5/plugin/Jobs.h"
#include "em5/plugin/Messages.h"
#include "em5/EM5Helper.h"

#include <qsf_game/component/event/EventTagComponent.h>
#include <qsf_game/base/DistanceHelper.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/move/MovableComponent.h>
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
	const uint32 DLKLogic::GAMELOGIC_TYPE_ID = qsf::StringHash("em5::DLKLogic");


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	BuildingComponent* DLKLogic::getPlaceOfActionBuilding(qsf::Entity& entity)
	{
		if (EntityHelper(entity).isEntityPerson())
		{
			// Get the building the person waves out
			const qsf::MovableComponent* movableComponent = entity.getComponent<qsf::MovableComponent>();
			if (nullptr != movableComponent)
			{
				const qsf::Entity* container = movableComponent->getContainerEntity();

				// Try to get the building component from the container where the person is inside
				if (nullptr != container)
				{
					return container->getComponent<BuildingComponent>();
				}
			}
		}

		// Return building component, in case of "normal" buildings or industrial plants, else return nullptr
		return entity.getComponent<BuildingComponent>();
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	DLKLogic::DLKLogic() :
		qsf::GameLogic(GAMELOGIC_TYPE_ID),
		mTimeWaiting(qsf::Time::ZERO),
		mDeployed(false)
	{
		// Nothing here
	}

	bool DLKLogic::isDeployed() const
	{
		return mDeployed;
	}

	void DLKLogic::undeployBeforeMovingNext()
	{
		mDeployed = true;
		mUndeployMessageProxyMoveCommand.registerAt(qsf::MessageConfiguration(Messages::EM5_POST_COMMAND_EXECUTE, MoveCommand::PLUGINABLE_ID, getEntityId()), boost::bind(&DLKLogic::undeploy, this, _1));
		mUndeployMessageProxyReturnToBaseCommand.registerAt(qsf::MessageConfiguration(Messages::EM5_POST_COMMAND_EXECUTE, ReturnToBaseCommand::PLUGINABLE_ID, getEntityId()), boost::bind(&DLKLogic::undeploy, this, _1));
		mUndeployMessageProxyDLKSafePersonCommand.registerAt(qsf::MessageConfiguration(Messages::EM5_POST_COMMAND_EXECUTE, DLKSafePersonCommand::PLUGINABLE_ID, getEntityId()), boost::bind(&DLKLogic::undeploy, this, _1));
		mUndeployMessageProxyDLKExtinguishCommand.registerAt(qsf::MessageConfiguration(Messages::EM5_POST_COMMAND_EXECUTE, DLKExtinguishCommand::PLUGINABLE_ID, getEntityId()), boost::bind(&DLKLogic::undeploy, this, _1));
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	bool DLKLogic::onStartup()
	{
		// Register job
		// -> For performance reasons, do updates not in every tick, but in fixed time intervals
		qsf::jobs::JobConfiguration jobConfiguration;
		jobConfiguration.mTimeBetweenCalls = qsf::Time::fromSeconds(1.0f);
		mSimulationJobProxy.registerAt(Jobs::SIMULATION_LOGIC, boost::bind(&DLKLogic::updateSimulation, this, _1), jobConfiguration);

		if (mDeployed)
		{
			undeployBeforeMovingNext();
		}
		return true;
	}

	void DLKLogic::onShutdown()
	{
		// Unregister job
		mSimulationJobProxy.unregister();

		// Unregister messages
		mUndeployMessageProxyMoveCommand.unregister();
		mUndeployMessageProxyReturnToBaseCommand.unregister();
		mUndeployMessageProxyDLKSafePersonCommand.unregister();
		mUndeployMessageProxyDLKExtinguishCommand.unregister();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void DLKLogic::updateSimulation(const qsf::JobArguments& jobArguments)
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
			if (mTimeWaiting >= qsf::Time::fromSeconds(LogicSpecsGroup::getInstanceSafe().getDlkMinimumWaitingTime()))
			{
				// Perform the automatism now
				performAutomatism();

				// Reset wait time
				mTimeWaiting = qsf::Time::ZERO;
			}
		}
	}

	void DLKLogic::performAutomatism()
	{
		GameSettingsGroup::AutomatismValue automatismValue = GameSettingsGroup::getInstanceSafe().getAutomatismValue();
		if (automatismValue >= GameSettingsGroup::AUTOMATISM_ALL)
		{
			rescuePersonAutomatism();
		}
	}

	void DLKLogic::rescuePersonAutomatism()
	{
		// Get the transform component for the position
		const qsf::TransformComponent* transformComponent = getEntity()->getComponent<qsf::TransformComponent>();
		if (nullptr == transformComponent)
			return;

		glm::vec3 ownerPosition = transformComponent->getPosition();

		// Search for event tag components in the radius
		std::vector<qsf::game::EventTagComponent*> eventTagComponentVector;
		qsf::ComponentMapQuery(getMap()).getInstancesInCircle(ownerPosition, LogicSpecsGroup::getInstanceSafe().getDlkSearchRadius(), eventTagComponentVector);

		// Search for building components in the radius
		std::vector<BuildingComponent*> buildingComponentVector;
		qsf::ComponentMapQuery(getMap()).getInstancesInCircle(ownerPosition, LogicSpecsGroup::getInstanceSafe().getDlkSearchRadius(), buildingComponentVector);

		std::vector<qsf::Component*> componentVector(eventTagComponentVector.size() + buildingComponentVector.size());
		componentVector.clear();
		componentVector.insert(componentVector.end(), eventTagComponentVector.begin(), eventTagComponentVector.end());
		componentVector.insert(componentVector.end(), buildingComponentVector.begin(), buildingComponentVector.end());

		// Sort the event tag for the distance
		qsf::game::IsLeftComponentCloserToTargetPosition compareObject(ownerPosition);
		std::sort(componentVector.begin(), componentVector.end(), compareObject);

		for (qsf::Component* component : componentVector)
		{
			if (EntityLogicHelper::checkAndExecuteAutomatism<DLKSafePersonCommand>(getEntity(), &component->getEntity()))
			{
				#ifndef ENDUSER
					if (GameDebugGroup::getInstanceSafe().getShowAutomatismDebug())
					{
						QSF_DEBUGDRAW.requestDraw(qsf::TextDebugDrawRequest("auto:RescuePersonByDlk", ownerPosition, qsf::Color4::YELLOW), qsf::DebugDrawLifetimeData(qsf::Time::fromSeconds(5.0f)));
					}
				#endif
				break;
			}
		}
	}

	void DLKLogic::undeploy(const qsf::MessageParameters& parameters)
	{
		mDeployed = false;
		mUndeployMessageProxyMoveCommand.unregister();
		mUndeployMessageProxyReturnToBaseCommand.unregister();
		mUndeployMessageProxyDLKSafePersonCommand.unregister();
		mUndeployMessageProxyDLKExtinguishCommand.unregister();

		qsf::ActionComponent& actionComponent = getEntity()->getOrCreateComponentSafe<qsf::ActionComponent>();

		actionComponent.pushAction<DLKUndeployAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
