// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/logic/local/civilians/EvadePlayerUnitsLogic.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"
#include "em5/plugin/Jobs.h"
#include "em5/ai/MovementModes.h"
#include "em5/ai/MoverType.h"

#include <qsf_ai/worldModel/trafficLanes/VehicleEvadeToTheSideLogic.h>
#include <qsf_ai/worldModel/trafficLanes/PersonEvadeToTheSideLogic.h>
#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>
#include <qsf_ai/navigation/NavigationComponent.h>

#include <qsf/job/JobArguments.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/action/WaitAction.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Local definitions                                     ]
	//[-------------------------------------------------------]
	// TODO(vs) Make this configurable? Time delta that is a higher border for when to react to an approaching vehicle
	const qsf::Time gMaxTimeDeltaToReactToApproach = qsf::Time::fromSeconds(3.0f);


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 EvadePlayerUnitsLogic::GAMELOGIC_TYPE_ID = qsf::StringHash("em5::EvadePlayerUnitsLogic");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 EvadePlayerUnitsLogic::ACTION_PRIORITY = action::COMMAND_STD; // indirect reaction to allow a player command - is this the correct prio?


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	EvadePlayerUnitsLogic::EvadePlayerUnitsLogic() :
		GameLogic(GAMELOGIC_TYPE_ID),
		mHadRelevantApproachInLastUpdate(false)
	{
		// Nothing here
	}

	bool EvadePlayerUnitsLogic::hasActiveApproach() const
	{
		return mHadRelevantApproachInLastUpdate;
	}

	const qsf::ai::SpatialConfiguration3D& EvadePlayerUnitsLogic::getLastApproachConfiguration() const
	{
		return mNearestApproachConfig;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	bool EvadePlayerUnitsLogic::onStartup()
	{
		// TODO(vs) Do we need an own job manager here?
		mSimulationJobProxy.registerAt(Jobs::SIMULATION_LOGIC, boost::bind(&EvadePlayerUnitsLogic::updateSimulation, this, _1), qsf::jobs::JobConfiguration());
		return true;
	}

	void EvadePlayerUnitsLogic::onShutdown()
	{
		mSimulationJobProxy.unregister();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void EvadePlayerUnitsLogic::registerApproachFromEmergencyVehicle(qsf::Time approachTime, const qsf::ai::SpatialConfiguration3D& approachConfig)
	{
		if (!mNearestApproachRegistered || *mNearestApproachRegistered < approachTime)
		{
			mNearestApproachRegistered = approachTime;
			mNearestApproachConfig = approachConfig;
		}
	}

	void EvadePlayerUnitsLogic::updateSimulation(const qsf::JobArguments& jobArguments)
	{
		evadeIfNecessary(jobArguments.getClock().getCurrentTime());
		mNearestApproachRegistered.reset();	// We have already recalculated the boolean for the current update, restart collecting the approaches for the next update
	}

	void EvadePlayerUnitsLogic::evadeIfNecessary(const qsf::Time& now)
	{
		mHadRelevantApproachInLastUpdate = (mNearestApproachRegistered && (*mNearestApproachRegistered < gMaxTimeDeltaToReactToApproach));
		if (!mHadRelevantApproachInLastUpdate)
			return;

		qsf::ActionComponent* actionComponent = getEntity()->getComponent<qsf::ActionComponent>();
		if (!actionComponent || actionComponent->getCurrentPriority() >= ACTION_PRIORITY)
			return;	// No action component or the entity is currently doing something with at least equal priority

		qsf::ai::NavigationComponent* navigationComponent = getEntity()->getComponent<qsf::ai::NavigationComponent>();
		if (!navigationComponent || !navigationComponent->isActive())
			return; // No or deactivated navigation component means vehicle can't or won't move anyway

		const qsf::Entity* owner = getEntity();
		QSF_CHECK(owner, "Unable to evade without an owning entity",
			return);

		// insert a move and a wait action afterwards at the front of a plan.
		// Pushing in wrong order when pushing to the front
		std::unique_ptr<qsf::ai::Path> evasionPath(new qsf::ai::Path);
		bool evasionSuccess = false;

		if (navigationComponent->getMoverType() == MoverType::CIVIL_PERSON)
		{
			// We could read the free space required here but I think it's easier to say we want some minimum distance to all vehicles here.
			// No player unit in EM5 seems to be wider than 2 meters and we want -> Hard coded magic number here
			const float desiredLateralDistance = 1.f;
			qsf::ai::PersonEvadeToTheSideLogic evasionPlanning(*navigationComponent, mNearestApproachConfig, desiredLateralDistance);
			evasionSuccess = evasionPlanning.tryCalculateEvadePath(*evasionPath);
		}
		else
		{
			qsf::ai::VehicleEvadeToTheSideLogic evasionPlanning(*navigationComponent);
			evasionSuccess = evasionPlanning.tryCalculateEvadePath(*evasionPath);
		}


		if (evasionSuccess)
		{
			navigationComponent->clearPath(); // clear current path
			actionComponent->pushAction<qsf::WaitAction>(ACTION_PRIORITY, qsf::action::INSERT_AT_FRONT).init(qsf::Time::fromSeconds(2.0f));

			if (!evasionPath->getNodes().empty())
			{
				// According to the API, an empty path might be returned in case the entity is perfectly evaded and doesn't need to move.
				// We don't need to push a move action here
				const qsf::logic::TargetPoint dummyGoalTargetPoint(evasionPath->getNodes().back().mPosition, false, 0.0f, 0.1f); // make a target point for the goal location although we have already calculated the path
				actionComponent->pushAction<MoveAction>(ACTION_PRIORITY, qsf::action::INSERT_AT_FRONT).init(
					new qsf::ai::ReachSinglePointGoal(dummyGoalTargetPoint), MovementModes::MOVEMENT_MODE_CIVIL_CAR_MOVES_TO_THE_SIDE, evasionPath.release());

				navigationComponent->setNextUpdateTime(now); // clear any waiting time that we normally use because the entity should react immediately
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
