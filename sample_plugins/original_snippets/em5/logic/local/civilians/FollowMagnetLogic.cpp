// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/logic/local/civilians/FollowMagnetLogic.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"
#include "em5/component/person/GaperComponent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/component/movement/PersonMagnetComponent.h"
#include "em5/component/movement/MoveToDespawnComponent.h"
#include "em5/game/targetpoint/FollowMagnetTargetPointProvider.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/plugin/Jobs.h"

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf_game/base/DistanceHelper.h>

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/action/WaitAction.h>
#include <qsf/map/Map.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 FollowMagnetLogic::GAMELOGIC_TYPE_ID = qsf::StringHash("em5::FollowMagnetLogic");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 FollowMagnetLogic::ACTION_PRIORITY = action::DANGER_URGENT;	// Can't get below this - maybe other actions set DANGER_IMMINENT too often?


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	FollowMagnetLogic::FollowMagnetLogic() :
		GameLogic(GAMELOGIC_TYPE_ID),
		mTargetMagnetEntityId(qsf::getUninitialized<uint64>()),
		mIsInjured(false),
		mMovementMode(MovementModes::MOVEMENT_MODE_MAGNET)
	{
		// Nothing here
	}

	void FollowMagnetLogic::setTargetMagnet(uint64 magnetEntityId, const glm::vec3& relativeDistance)
	{
		mTargetMagnetEntityId = magnetEntityId;
		mRelativeDistance = relativeDistance;
	}

	uint64 FollowMagnetLogic::getTargetMagnetEntityId() const
	{
		return mTargetMagnetEntityId;
	}

	void FollowMagnetLogic::setMovementMode(MovementModes::Modes movementMode)
	{
		mMovementMode = movementMode;
	}

	MovementModes::Modes FollowMagnetLogic::getMovementMode() const
	{
		return mMovementMode;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	bool FollowMagnetLogic::onStartup()
	{
		QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_MAGNET_PRE_START_FOLLOW, getEntityId()));

		// Ambient persons can have animations like, sitting on a chair, doing some strange gesture, etc
		EntityHelper(getEntitySafe()).disableMeshAnimationTestComponent();

		qsf::ai::NavigationComponent* navigationComponent = getEntity()->getComponent<qsf::ai::NavigationComponent>();
		if (nullptr != navigationComponent)
		{
			// Delete the old "knowledge" about the goal
			navigationComponent->clearGoal();
		}

		mSimulationJobProxy.registerAt(Jobs::SIMULATION_LOGIC, boost::bind(&FollowMagnetLogic::updateSimulation, this, _1), qsf::jobs::JobConfiguration());

		// If the person is injured, stop this logic
		mOnInjuredProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_COMPONENT_GETSINJURED_PERSON, getEntityId()), boost::bind(&FollowMagnetLogic::onInjured, this, _1));
		mOnContaminatedProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_COMPONENT_HEALTH_PERSON_CONTAMINATED, getEntityId()), boost::bind(&FollowMagnetLogic::onInjured, this, _1));
		return true;
	}

	void FollowMagnetLogic::onShutdown()
	{
		mSimulationJobProxy.unregister();
		mOnInjuredProxy.unregister();
		mOnContaminatedProxy.unregister();

		if (!mIsInjured)
		{
			qsf::Entity* entity = getEntity();
			if (nullptr != entity)
			{
				qsf::ActionComponent& actionComponent = entity->getOrCreateComponentSafe<qsf::ActionComponent>();
				actionComponent.clearPlan();
				actionComponent.pushAction<qsf::WaitAction>(ACTION_PRIORITY).init(qsf::Time::fromSeconds(2.5f));

				// Restart random movement
				entity->getOrCreateComponentSafe<MoveToDespawnComponent>();
			}
		}

		QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_MAGNET_STOP_FOLLOW, getEntityId()));
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void FollowMagnetLogic::updateSimulation(const qsf::JobArguments& jobArguments)
	{
		if (qsf::isUninitialized(mTargetMagnetEntityId))
			return;

		qsf::Entity* entity = getEntity();
		qsf::Entity* targetEntity = getMap().getEntityById(mTargetMagnetEntityId);
		if (nullptr != entity && nullptr != targetEntity)
		{
			qsf::ActionComponent& actionComponent = entity->getOrCreateComponentSafe<qsf::ActionComponent>();
			qsf::action::Priority currentPriority = actionComponent.getCurrentPriority(); // Extra variable for easier debugging
			if (currentPriority < ACTION_PRIORITY)
			{
				actionComponent.clearPlan();
			}

			if (actionComponent.getPlan().isEmpty())
			{
				const float MIN_DIST_TO_WALK = 1.5f;	// try & error

				// Check the distance to the point where the person would like to be positioned
				float distance = FLT_MAX;
				qsf::TransformComponent& transformComponent = entity->getOrCreateComponentSafe<qsf::TransformComponent>();
				PersonMagnetComponent* personMagnetComponent = targetEntity->getComponent<PersonMagnetComponent>();
				if (nullptr != personMagnetComponent)
				{
					distance = qsf::game::DistanceHelper::get2dDistance(personMagnetComponent->getFollowOffset(entity->getId(), false, true), transformComponent.getPosition());
				}

				// Check the distance to the target unit
				if (distance >= MIN_DIST_TO_WALK)
				{
					distance = qsf::game::DistanceHelper::get2dDistance(*entity, *targetEntity);
				}

				if (distance < MIN_DIST_TO_WALK)
				{
					// Check again later
					actionComponent.pushAction<qsf::WaitAction>(ACTION_PRIORITY).init(qsf::Time::fromSeconds(1.f));
				}
				else
				{
					qsf::ai::ReachObjectGoal* reachObjectGoal = new qsf::ai::ReachObjectGoal(*entity, *targetEntity, FollowMagnetTargetPointProvider::TARGET_POINT_ID);
					actionComponent.pushAction<MoveAction>(ACTION_PRIORITY).init(reachObjectGoal, mMovementMode);
				}
			}
		}
	}

	void FollowMagnetLogic::onInjured(const qsf::MessageParameters& parameters)
	{
		mIsInjured = true;
		removeThis();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
