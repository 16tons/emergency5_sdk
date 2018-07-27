// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/move/escape/EscapeFootLongAction.h"
#include "em5/action/move/escape/EscapeFootShortAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/ai/MovementModes.h"
#include "em5/component/spawnpoint/civilist/PersonSpawnPointComponent.h"
#include "em5/game/groundmap/GroundMaps.h"
#include "em5/map/CollisionHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/specs/EscapeSpecsGroup.h"

#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/map/query/GroundMapQuery.h>
#include <qsf/math/CoordinateSystem.h>
#include <qsf/math/Random.h>
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
	const qsf::NamedIdentifier EscapeFootLongAction::ACTION_ID = "em5::EscapeFootLongAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	EscapeFootLongAction::EscapeFootLongAction() :
		EscapeAction(ACTION_ID),
		mMovementModeRun(true),
		mEscapeMode(EscapeMode::LONG)
	{
		// This is the default in case we don't overwrite it
		mMaximumTime = qsf::Time::fromSeconds(EscapeSpecsGroup::getInstanceSafe().getEscapeTimeFootLong());
	}

	EscapeFootLongAction::~EscapeFootLongAction()
	{
		// Nothing here
	}

	void EscapeFootLongAction::catched()
	{
		mTimeUntilNextPause = qsf::Time::ZERO;
	}

	void EscapeFootLongAction::init(bool movementModeRun)
	{
		mMovementModeRun = movementModeRun;

		runTowardsDistantPosition();

		QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_STARTED, ACTION_ID));
	}

	void EscapeFootLongAction::init(const qsf::Time& escapeLength, bool movementModeRun)
	{
		mMaximumTime = escapeLength;
		init(movementModeRun);
	}

	void EscapeFootLongAction::init(const std::string& escapeTargetTag, bool movementModeRun)
	{
		// Use fixed target
		mEscapeTargetTag = escapeTargetTag;
		mMovementModeRun = movementModeRun;

		qsf::Entity* targetEntity = getTargetEntity();
		if (nullptr != targetEntity)
		{
			runTowardsPosition(targetEntity->getOrCreateComponentSafe<qsf::TransformComponent>().getPosition());
			mEscapeMode = EscapeMode::FIXED;

			QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_STARTED, ACTION_ID));
		}
		else
		{
			init(movementModeRun);
		}
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Action methods                    ]
	//[-------------------------------------------------------]
	void EscapeFootLongAction::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base implementation
		qsf::ai::MoveAction::serialize(serializer);

		// TODO(fw): Implement this
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	qsf::action::Result EscapeFootLongAction::updateAction(const qsf::Clock& clock)
	{
		// Call base implementation
		qsf::action::Result result = qsf::ai::MoveAction::updateAction(clock);

		// In EM5, a failed AI movement should not lead to automatic clearing of the action plan
		const bool isCurrentlyMoving = (qsf::action::RESULT_CONTINUE == result);

		if (isCurrentlyMoving)
		{
			mTimeUntilNextPause -= clock.getTimePassed();
			if (mTimeUntilNextPause <= qsf::Time::ZERO)
			{
				const EscapeSpecsGroup& specsGroup = EscapeSpecsGroup::getInstanceSafe();
				mTimeUntilNextPause = qsf::Time::fromSeconds(qsf::Random::getRandomFloat(specsGroup.getRunTimeFootLongMin(), specsGroup.getRunTimeFootLongMax()));

				const qsf::Time pauseTime = qsf::Time::fromSeconds(qsf::Random::getRandomFloat(specsGroup.getBreatheTimeFootLongMin(), specsGroup.getBreatheTimeFootLongMax()));
				pushBreathAnimation(pauseTime);
			}
		}

		mTimeRunning += clock.getTimePassed();

		if (mEscapeMode == EscapeMode::FIXED || mEscapeMode == EscapeMode::SHORT)
		{
			// The gangster escapes and the respective entity is destroyed when the gangster is close to map boundaries or the desired despawn point
			bool targetReached = false;
			{
				EntityHelper entityHelper(getEntity());
				if (entityHelper.isCloseToMapBoundaries())
				{
					targetReached = true;
				}
				else if (!isCurrentlyMoving)
				{
					targetReached = (mEscapeMode == EscapeMode::SHORT) || (glm::distance2(entityHelper.getPosition(), mCurrentTargetPosition) < 5.0f);
					if (!targetReached)
					{
						// Switch from FIXED to LONG escape mode, as fixed target does not seem to be reachable
						runTowardsDistantPosition();
					}
				}
			}

			if (targetReached)
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
		else  // EscapeMode::LONG
		{
			const qsf::Time timeLeft = mMaximumTime - mTimeRunning;
			if (timeLeft <= qsf::Time::fromSeconds(10.0f))
			{
				// Now it's time to search for the closest escape point
				runTowardsClosestEscapePoint();
			}
			else
			{
				// Is near current target, but too much time left? Then search a new target point
				if (glm::distance2(mCurrentTargetPosition, EntityHelper(getEntity()).getPosition()) < 10.0f * 10.0f)
				{
					if (timeLeft.getSeconds() > 10.0f)
					{
						init(mMaximumTime, mMovementModeRun);
					}
				}

				if (!isCurrentlyMoving)
				{
					// Search new target
					runTowardsDistantPosition();
				}
			}
		}

		return qsf::action::RESULT_CONTINUE;
	}

	void EscapeFootLongAction::onShutdown()
	{
		// Call base implementation
		EscapeAction::onShutdown();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void EscapeFootLongAction::runTowardsClosestEscapePoint()
	{
		const qsf::Transform& selfTransform = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>().getTransform();

		glm::vec3 position = selfTransform.getPosition();	// Initialize with a fallback value...
		{
			// Search a point around a center some meters ahead, so points in front of the escaper get preferred
			const glm::vec3 targetCenterPosition = selfTransform.vec3PositionLocalToWorld(qsf::CoordinateSystem::getIn() * 20.0f);

			std::vector<PersonSpawnPointComponent*> exitPersonSpawnPoints;
			if (collectDespawnPoints(exitPersonSpawnPoints, false))
			{
				// Take the closest one
				float shortestSquaredDistance = std::numeric_limits<float>::max();

				for (PersonSpawnPointComponent* personSpawnPointComponent : exitPersonSpawnPoints)
				{
					const qsf::TransformComponent& spawnPointTransformComponent = personSpawnPointComponent->getEntity().getComponentSafe<qsf::TransformComponent>();

					const glm::vec3& pointPosition = spawnPointTransformComponent.getPosition();
					const float squaredDistance = glm::distance2(pointPosition, targetCenterPosition);
					if (squaredDistance < shortestSquaredDistance)
					{
						shortestSquaredDistance = squaredDistance;
						position = pointPosition;
					}
				}
			}
		}

		runTowardsPosition(position);
		mEscapeMode = EscapeMode::SHORT;
	}

	void EscapeFootLongAction::runTowardsDistantPosition()
	{
		const glm::vec3& selfPosition = EntityHelper(getEntity()).getPosition();

		glm::vec3 position = selfPosition;	// Initialize with a fallback value...
		{
			std::vector<PersonSpawnPointComponent*> exitPersonSpawnPoints;
			if (collectDespawnPoints(exitPersonSpawnPoints, true))
			{
				// Take any of these, but with at least a certain distance
				for (float minDistance = 500.0f; minDistance > 0.0f; minDistance -= 50.0f)
				{
					PersonSpawnPointComponent* point = qsf::Random::getAnyOf(exitPersonSpawnPoints);
					const glm::vec3 pointPosition = EntityHelper(point->getEntity()).getPosition();
					if (glm::distance2(pointPosition, selfPosition) >= minDistance * minDistance)
					{
						position = pointPosition;
						break;
					}
				}
			}
		}

		runTowardsPosition(position);
		mEscapeMode = EscapeMode::LONG;
	}

	void EscapeFootLongAction::runTowardsPosition(const glm::vec3& position)
	{
		mCurrentTargetPosition = position;
		qsf::logic::TargetPoint moveTargetPoint(mCurrentTargetPosition, false, 0.0f, 2.0f);

		// Call base class implementation
		unsigned int movementModeId = (mMovementModeRun ? MovementModes::MOVEMENT_MODE_RUN : MovementModes::MOVEMENT_MODE_DEFAULT);
		qsf::ai::MoveAction::init(new qsf::ai::ReachSinglePointGoal(moveTargetPoint), movementModeId);

		if (getState() != qsf::process::WAITING)
		{
			// Call move action startup again
			qsf::ai::MoveAction::onStartup();
		}

		const EscapeSpecsGroup& specsGroup = EscapeSpecsGroup::getInstanceSafe();
		mTimeUntilNextPause.setSeconds(qsf::Random::getRandomFloat(specsGroup.getRunTimeFootLongMin(), specsGroup.getRunTimeFootLongMax()));
	}

	bool EscapeFootLongAction::collectDespawnPoints(std::vector<PersonSpawnPointComponent*>& outPoints, bool onlyOutsideMapBoundaries)
	{
		for (PersonSpawnPointComponent* personSpawnPointComponent : qsf::ComponentMapQuery(getMap()).getAllInstances<PersonSpawnPointComponent>())
		{
			// Test if we have the right gate
			if (personSpawnPointComponent->isActive() && personSpawnPointComponent->isExit())
			{
				// Filter out points inside the map boundaries
				if (onlyOutsideMapBoundaries)
				{
					if (EntityHelper(personSpawnPointComponent->getEntity()).isInMapBoundaries())
						continue;
				}

				outPoints.push_back(personSpawnPointComponent);
			}
		}

		return !outPoints.empty();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
