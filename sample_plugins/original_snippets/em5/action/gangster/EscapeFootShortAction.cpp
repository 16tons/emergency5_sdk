// Copyright (C) 2012-2015 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/gangster/EscapeFootShortAction.h"
#include "em5/ai/MovementModes.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/game/Game.h"
#include "em5/game/component/GameComponent.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/component/spawnpoint/civilist/PersonSpawnPointComponent.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include "em5/plugin/Messages.h"
#include "em5/specs/EscapeSpecsGroup.h"
#include "em5/game/Game.h"
#include "em5/EM5Helper.h"

#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>
#include <qsf_ai/navigation/NavigationComponent.h>
#include <qsf_ai/navigation/Waypoint.h>
#include <qsf_ai/navigation/Path.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/action/WaitAction.h>
#include <qsf/map/Map.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/math/Random.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier EscapeFootShortAction::ACTION_ID = "em5::EscapeFootShortAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	EscapeFootShortAction::EscapeFootShortAction() :
		EscapeAction(ACTION_ID),
		mIsCivilPerson(false),
		mStuckTime(qsf::Time::ZERO),
		mElapsedTime(qsf::Time::ZERO)
	{
		// Nothing here
	}

	EscapeFootShortAction::~EscapeFootShortAction()
	{
		// Nothing here
	}

	void EscapeFootShortAction::catched()
	{
		mElapsedTime = mPhaseEndTime;
	}

	void EscapeFootShortAction::init(const std::string& escapeTargetTag)
	{
		// TODO(fw): Evaluate escape target tag!

		mEscapeTargetTag = escapeTargetTag;

		mTargetPosition = getClosestTargetPoint(true);
		if (qsf::isUninitialized(mTargetPosition))
			mTargetPosition = getClosestTargetPoint(false);

		qsf::logic::TargetPoint moveTargetPoint(mTargetPosition, false, 0.0f, 2.0f);

		const EscapeSpecsGroup& specsGroup = EscapeSpecsGroup::getInstanceSafe();
		mPhaseEndTime = qsf::Time::fromSeconds(qsf::Random::getRandomFloat(specsGroup.getRunTimeFootShortMin(), specsGroup.getRunTimeFootShortMax()));

		QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_STARTED, ACTION_ID));

		// Call base class implementation
		qsf::ai::MoveAction::init(new qsf::ai::ReachSinglePointGoal(moveTargetPoint), MovementModes::MOVEMENT_MODE_RUN, nullptr);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Action methods                    ]
	//[-------------------------------------------------------]
	void EscapeFootShortAction::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base implementation
		qsf::ai::MoveAction::serialize(serializer);

		serializer.serialize(mIsCivilPerson);
		serializer.serialize(mStuckTime);
		serializer.serialize(mElapsedTime);
		serializer.serialize(mPhaseEndTime);
		serializer.serialize(mTargetPosition);
		serializer.serialize(mOldPosition);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	qsf::action::Result EscapeFootShortAction::updateAction(const qsf::Clock& clock)
	{
		// Call base implementation
		qsf::action::Result result = qsf::ai::MoveAction::updateAction(clock);

		// In EM5, a failed AI movement should not lead to automatic clearing of the action plan
		if (qsf::action::RESULT_CLEARLIST == result)
		{
#if 0
			if (EntityHelper(getEntity()).isCloseToMapBoundaries(1.0f))
			{

				// TODO(ks) According to vs, something like this should work, but unfortunately it does not.
				// So we just destroy the entity

				// Gangster is close to map boundaries and thus should simply go straight to the despawn point (regardless of collisions etc.)
				qsf::ai::NavigationComponent* navigationComponent = getEntity().getOrCreateComponent<qsf::ai::NavigationComponent>();

				// Assume this entity is about to despawn but collides with the map boundaries. Remove collision flags.
				navigationComponent->setNativeCollisionTypesToAvoid(0);
				navigationComponent->clearGoal();
				navigationComponent->clearPath();

				qsf::ai::ReachSinglePointGoal* spg = new qsf::ai::ReachSinglePointGoal(qsf::logic::TargetPoint(mTargetPosition, false, 0.0f, 2.0f));

				const glm::vec3& currentPos = getEntity().getComponent<qsf::TransformComponent>()->getPosition();
				glm::vec3 direction = glm::normalize(mTargetPosition - currentPos);

				qsf::ai::DynamicPortal dynPortal(currentPos, 0);
				qsf::ai::DynamicPortal dynPortal2(mTargetPosition, 0);

				std::deque<qsf::ai::Waypoint> directPath;
				directPath.emplace_back(currentPos, direction, dynPortal, !true, true);
				directPath.emplace_back(mTargetPosition, direction, dynPortal2, !true, true);
				getComponent().pushAction<MoveAction>(action::Priority::BLOCKING, qsf::action::INSERT_AT_FRONT).init(spg, MovementModes::MOVEMENT_MODE_RUN, new qsf::ai::Path(directPath));

				return qsf::action::RESULT_CONTINUE;
				//result = qsf::action::RESULT_DONE;

			}
			else
#endif
			{
				// Some AI movement failed, cancel this action...
				result = qsf::action::RESULT_DONE;
			}
		}


		mElapsedTime += clock.getTimePassed();

		if (mElapsedTime > mPhaseEndTime)
		{
			const EscapeSpecsGroup& specsGroup = EscapeSpecsGroup::getInstanceSafe();
			mPhaseEndTime = mElapsedTime + qsf::Time::fromSeconds(qsf::Random::getRandomFloat(specsGroup.getRunTimeFootShortMin(), specsGroup.getRunTimeFootShortMax()));
			qsf::ActionComponent& actionComponent = getEntity().getOrCreateComponentSafe<qsf::ActionComponent>();
			AnimationHelper animationHelper(getEntity());
			qsf::Time time = qsf::Time::fromSeconds(qsf::Random::getRandomFloat(specsGroup.getBreatheTimeFootShortMin(), specsGroup.getBreatheTimeFootShortMax()));
			actionComponent.pushAction<qsf::WaitAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(time);
			actionComponent.pushAction<PlayAnimationAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(animationHelper.getAnimationBreatheDeeply(), false, true, false, qsf::Time::ZERO, time);
		}

		EntityHelper entityHelper(getEntity());
		const bool isInMapBoundaries = entityHelper.isInMapBoundaries();

		if (result == qsf::action::RESULT_DONE || !isInMapBoundaries)
		{
			const glm::vec3& currentPos = getEntity().getComponent<qsf::TransformComponent>()->getPosition();

			const bool closeToDespawnPoint = (glm::distance2(currentPos, mTargetPosition) < 5.0f);

			// The gangster escapes and the respective entity is destroyed when the gangster is close to
			// map boundaries or the desired despawn point
			if (closeToDespawnPoint || !isInMapBoundaries || entityHelper.isCloseToMapBoundaries())
			{
				// Send message
				qsf::MessageParameters parameters;
				parameters.setParameter("GangsterId", getEntityId());
				QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_GANGSTER_ESCAPED, getEntityId()), parameters);

				// Destroy the entity
				MapHelper::destroyEntity(getEntity());
				return qsf::action::RESULT_CLEARLIST;
			}
		}

		return result;
	}

	bool EscapeFootShortAction::onStartup()
	{
		// Call base class implementation
		if (!qsf::ai::MoveAction::onStartup())
			return false;

		const EntityHelper entityHelper(getEntity());
		mIsCivilPerson = entityHelper.isCivilPerson();

		// TODO(mk) I assume to reach the goal of perfect persons, we keep this line here
		if (mIsCivilPerson)
		{
			getEntity().getOrCreateComponentById("em5::PersonPlacementComponent");
		}

		return true;
	}

	void EscapeFootShortAction::onShutdown()
	{
		// Call base implementation
		qsf::ai::MoveAction::onShutdown();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	uint32 EscapeFootShortAction::getMovementMode(uint32 movementModeId)
	{
		// If argument is invalid, check for possible movement mode
		if (qsf::isUninitialized(movementModeId))
		{
			EntityHelper entityHelper(getEntity());
			if (entityHelper.isSquadPerson())
			{
				// TODO(mk) I build this with paramedic in head, for other carry persons, maybe fire fighters or police/gangster, we need probability other checks
				if (entityHelper.isCarryingAnotherPerson())
				{
					return MovementModes::MOVEMENT_MODE_CARRY_PERSON;
				}
			}
		}
		return movementModeId;
	}

	bool EscapeFootShortAction::checkAllowUpdate()
	{
		// TODO(mk) To optimize this function we can keep the gameComponent as member
		if (mIsCivilPerson)
		{
			return getMap().getCoreEntity().getOrCreateComponent<GameComponent>()->getCivilPersonMovement();
		}
		return true;
	}

	bool EscapeFootShortAction::checkPositionStuck()
	{
		glm::vec3 newPosition = EntityHelper(getEntity()).getPosition();
		bool positionStuck = (mOldPosition == newPosition ? true : false);
		mOldPosition = newPosition;

		return positionStuck;
	}

	glm::vec3 EscapeFootShortAction::getClosestTargetPoint(bool onlyMapExits)
	{
		std::vector<PersonSpawnPointComponent*> exitPersonSpawnPoints;

		// Sort the vector for entry and exit
		for (PersonSpawnPointComponent* personSpawnPointComponent : qsf::ComponentMapQuery(getMap()).getAllInstances<PersonSpawnPointComponent>())
		{
			// Test if we have the right gate
			if (personSpawnPointComponent->isActive() && personSpawnPointComponent->isExit())
			{
				// onlyMapExits -> !isInBoundaries
				if (!onlyMapExits || !EntityHelper(personSpawnPointComponent->getEntity()).isInMapBoundaries())
					exitPersonSpawnPoints.push_back(personSpawnPointComponent);
			}
		}

		if (exitPersonSpawnPoints.empty())
			return qsf::getUninitialized<glm::vec3>();

		const qsf::TransformComponent& selfTransformComponent = getEntity().getComponentSafe<qsf::TransformComponent>();
		glm::vec3 selfPosition = selfTransformComponent.getPosition();
		float shortestSquaredDistance = std::numeric_limits<float>::max();
		glm::vec3 position;

		for (PersonSpawnPointComponent* personSpawnPointComponent : exitPersonSpawnPoints)
		{
			const qsf::TransformComponent& spawnPointTransformComponent = personSpawnPointComponent->getEntity().getComponentSafe<qsf::TransformComponent>();

			const glm::vec3& pointPosition = spawnPointTransformComponent.getPosition();
			const float squaredDistance = glm::distance2(pointPosition, selfPosition);
			if (squaredDistance < shortestSquaredDistance)
			{
				shortestSquaredDistance = squaredDistance;
				position = pointPosition;
			}
		}

		return position;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
