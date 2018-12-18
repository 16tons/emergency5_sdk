// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/logic/local/civilians/FollowedByHelicopterLogic.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/ai/MovementModes.h"
#include "em5/component/movement/PersonMagnetComponent.h"
#include "em5/game/targetpoint/FollowMagnetTargetPointProvider.h"
#include "em5/plugin/Jobs.h"
#include "em5/map/EntityHelper.h"

#include <qsf_ai/worldModel/trafficLanes/VehicleEvadeToTheSideLogic.h>
#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/action/WaitAction.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 FollowedByHelicopterLogic::GAMELOGIC_TYPE_ID = qsf::StringHash("em5::FollowedByHelicopterLogic");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 FollowedByHelicopterLogic::ACTION_PRIORITY = action::DANGER_URGENT;	// Can't get below this - maybe other actions set DANGER_IMMINENT too often?


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	FollowedByHelicopterLogic::FollowedByHelicopterLogic() :
		GameLogic(GAMELOGIC_TYPE_ID),
		mHelicopterEntityId(qsf::getUninitialized<uint64>())
	{
		// Nothing here
	}

	void FollowedByHelicopterLogic::setFollowingHelicopter(uint64 helicopterEntityId)
	{
		mHelicopterEntityId = helicopterEntityId;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	bool FollowedByHelicopterLogic::onStartup()
	{
		mSimulationJobProxy.registerAt(Jobs::SIMULATION_LOGIC, boost::bind(&FollowedByHelicopterLogic::updateSimulation, this, _1), qsf::jobs::JobConfiguration());
		return true;
	}

	void FollowedByHelicopterLogic::onShutdown()
	{
		mSimulationJobProxy.unregister();

		qsf::Entity* entity = getEntity();
		if (nullptr != entity)
		{
			qsf::ActionComponent& actionComponent = entity->getOrCreateComponentSafe<qsf::ActionComponent>();
			qsf::action::Priority currentPriority = actionComponent.getCurrentPriority(); // Extra variable for easier debugging
			if (currentPriority == ACTION_PRIORITY)
			{
				qsf::Action* currentAction = actionComponent.getCurrentAction();
				if (nullptr != currentAction && currentAction->getTypeId() == MoveAction::ACTION_ID)
				{
					// Walkaction with our priority - should be from this logic
					actionComponent.clearPlan();
				}
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void FollowedByHelicopterLogic::updateSimulation(const qsf::JobArguments& jobArguments)
	{
		if (qsf::isUninitialized(mHelicopterEntityId))
			return;

		qsf::Entity* entity = getEntity();
		if (nullptr == entity || nullptr == getMap().getEntityById(mHelicopterEntityId))
			return;

		// Get action component
		qsf::ActionComponent& actionComponent = entity->getOrCreateComponentSafe<qsf::ActionComponent>();
		qsf::action::Priority currentPriority = actionComponent.getCurrentPriority(); // Extra variable for easier debugging

		if (currentPriority < ACTION_PRIORITY)
		{
			actionComponent.clearPlan();
		}

		if (actionComponent.getPlan().isEmpty())
		{
			// If person stand and waving
			if (EntityHelper(*entity).isCivilPerson())
			{
				// Start waving animation
				AnimationHelper animationHelper(*entity);
				actionComponent.pushAction<qsf::WaitAction>(ACTION_PRIORITY).init(qsf::Time::fromSeconds(4.0f));
				actionComponent.pushAction<TurnToAction>(ACTION_PRIORITY).init(mHelicopterEntityId);
				actionComponent.pushAction<PlayAnimationAction>(ACTION_PRIORITY).init(animationHelper.getAnimationLookingUpWaving(), true);
			}

			// If vehicle stop and drive to the side
			else if (EntityHelper(*entity).isCivilRoadVehicle())
			{
				qsf::ai::NavigationComponent* navigationComponent = getEntity()->getComponent<qsf::ai::NavigationComponent>();
				if (!navigationComponent || !navigationComponent->isActive())
					return;

				qsf::ai::VehicleEvadeToTheSideLogic evasionPlanning(*navigationComponent);
				std::unique_ptr<qsf::ai::Path> evasionPath(new qsf::ai::Path);
				if (evasionPlanning.tryCalculateEvadePath(*evasionPath))
				{
					navigationComponent->clearPath(); // reset current path but keep goal intact
					actionComponent.pushAction<qsf::WaitAction>(ACTION_PRIORITY, qsf::action::INSERT_AT_FRONT).init(qsf::Time::fromSeconds(2.0f));

					if (!evasionPath->getNodes().empty())
					{
						// According to the API, an empty path might be returned in case the entity is perfectly evaded and doesn't need to move.
						// We don't need to push a move action here
						const qsf::logic::TargetPoint dummyGoalTargetPoint(evasionPath->getNodes().back().mPosition, false, 0.0f, 0.1f); // Make a target point for the goal location although we have already calculated the path

						actionComponent.pushAction<MoveAction>(ACTION_PRIORITY, qsf::action::INSERT_AT_FRONT).init(
							new qsf::ai::ReachSinglePointGoal(dummyGoalTargetPoint), MovementModes::MOVEMENT_MODE_CIVIL_CAR_MOVES_TO_THE_SIDE, evasionPath.release());
					}
				}
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
