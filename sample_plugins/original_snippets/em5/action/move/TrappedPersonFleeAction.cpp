// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/move/TrappedPersonFleeAction.h"
#include "em5/component/movement/MoveToDespawnComponent.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"
#include "em5/ai/MovementModes.h"
#include "em5/health/HealthHelper.h"
#include "em5/map/CollisionHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Jobs.h"

#include <qsf_game/component/base/SelectableComponent.h>

#include <qsf_ai/navigation/NavigationComponent.h>
#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>

#include <qsf/logic/action/ActionComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier TrappedPersonFleeAction::ACTION_ID = "em5::TrappedPersonFleeAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	TrappedPersonFleeAction::TrappedPersonFleeAction() :
		qsf::Action(ACTION_ID),
		mWasSelectable(false),
		mRunningTime(qsf::Time::fromSeconds(5.0f))
	{
		// Nothing here
	}

	void TrappedPersonFleeAction::init(const qsf::Time& runningTime, BuildingComponent& buildingComponent)
	{
		mRunningTime = runningTime;
		mEntityToFleeFrom = buildingComponent.getEntity();

		// Disable move to despawn when existing
		//  -> We do this here already, for there might be other actions like waiting or similar pushed meanwhile
		MoveToDespawnComponent* moveToDespawnComponent = getEntity().getComponent<MoveToDespawnComponent>();
		if (moveToDespawnComponent)
		{
			moveToDespawnComponent->setActive(false);
		}
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	bool TrappedPersonFleeAction::onStartup()
	{
		if (!mEntityToFleeFrom.valid())
			return false;

		qsf::Entity& personEntity = getEntity();

		// Temporarily immunize the person from any damage
		HealthComponent* healthComponent = personEntity.getComponent<HealthComponent>();
		if (healthComponent)
		{
			// Backup immunities
			mImmunitiesBackup = healthComponent->getImmunitiesAsString();
			HealthHelper(*healthComponent).setImmuneToInjurySources(true, true);
		}

		qsf::ai::NavigationComponent* navigationComponent = personEntity.getComponent<qsf::ai::NavigationComponent>();
		if (nullptr != navigationComponent)
		{
			navigationComponent->setActive(true);
		}

		// Destroy selectable component
		mWasSelectable = personEntity.destroyComponent<qsf::game::SelectableComponent>();

		// Flee from the danger
		//  -> This would be a use-case for "qsf::ai::AvoidThreatsGoal", but this one is not implemented and simply does nothing right now
		//  -> Instead choose a random point away from the building to run to
		const glm::vec3 personPosition = EntityHelper(personEntity).getPosition();
		const glm::vec3 buildingPosition = EntityHelper(mEntityToFleeFrom).getPosition();
		const glm::vec3 fleeDirection = glm::normalize(personPosition - buildingPosition);

		glm::vec3 targetPosition = personPosition + fleeDirection * 30.0f;
		if (CollisionHelper(getMap()).findFreePositionEM3(personEntity, targetPosition, 20.0f, 0.5f, targetPosition))
		{
			getComponent().pushAction<MoveAction>(action::DANGER_IMMINENT, qsf::action::INSERT_AT_FRONT).init(new qsf::ai::ReachSinglePointGoal(qsf::logic::TargetPoint(targetPosition, false, 0.0f, 5.0f)), MovementModes::MOVEMENT_MODE_RUN);

			// Register job
			qsf::jobs::JobConfiguration jobConfiguration;
			jobConfiguration.mTimeBetweenCalls = qsf::Time::fromSeconds(0.2f);
			mUpdateJobProxy.registerAt(Jobs::SIMULATION_LOGIC, boost::bind(&TrappedPersonFleeAction::updateSimulation, this, _1), jobConfiguration);

			return true;
		}

		return false;
	}

	void TrappedPersonFleeAction::onShutdown()
	{
		qsf::Entity& personEntity = getEntity();

		// Re-add selectable component if the entity was selectable before
		if (mWasSelectable)
			personEntity.createComponent<qsf::game::SelectableComponent>();

		// Reset immunities to the initial value before this logic started
		HealthComponent* healthComponent = personEntity.getComponent<HealthComponent>();
		if (healthComponent)
		{
			healthComponent->setImmunitiesByString(mImmunitiesBackup);
		}

		if (EntityHelper(personEntity).isCivilPerson())
		{
			// Let the person move away if civil
			MoveToDespawnComponent* moveToDespawnComponent = personEntity.getOrCreateComponent<MoveToDespawnComponent>();
			if (moveToDespawnComponent)
			{
				moveToDespawnComponent->setActive(true);
			}
		}
	}

	qsf::action::Result TrappedPersonFleeAction::updateAction(const qsf::Clock& clock)
	{
		// No reaction here; note that the job will clear the action plan
		return qsf::action::RESULT_CONTINUE;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void TrappedPersonFleeAction::updateSimulation(const qsf::JobArguments& jobArguments)
	{
		// This update is used to countdown the flee time while the move action we pushed in startup is running
		mRunningTime -= jobArguments.getTimePassed();
		if (mRunningTime <= qsf::Time::ZERO)
		{
			// Stop when flee time elapsed
			getComponent().clearPlan();
		}
		else if (nullptr == getComponent().getAction<MoveAction>())
		{
			// Stop when move action is gone for whatever reason
			getComponent().clearPlan();
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
