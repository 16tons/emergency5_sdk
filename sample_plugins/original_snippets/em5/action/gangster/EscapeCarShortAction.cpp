// Copyright (C) 2012-2015 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/gangster/EscapeCarShortAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/ai/MovementModes.h"
#include "em5/map/EntityHelper.h"
#include "em5/game/Game.h"
#include "em5/game/component/GameComponent.h"
#include "em5/component/spawnpoint/civilist/VehicleSpawnPointComponent.h"
#include "em5/plugin/Messages.h"
#include "em5/specs/SpecsGroupManager.h"
#include "em5/logic/local/gangsters/GangsterCarHijackerLogic.h"
#include "em5/map/MapHelper.h"
#include "em5/EM5Helper.h"

#include <qsf_game/base/DistanceHelper.h>

#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>
#include <qsf_ai/navigation/NavigationComponent.h>

#include <qsf/logic/gamelogic/GameLogicComponent.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/Map.h>
#include <qsf/map/query/ComponentMapQuery.h>
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
	const qsf::NamedIdentifier EscapeCarShortAction::ACTION_ID = "em5::EscapeCarShortAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	EscapeCarShortAction::EscapeCarShortAction() :
		EscapeAction(ACTION_ID),
		mIsCivilPerson(false),
		mStuckTime(qsf::Time::ZERO)
	{
		// Nothing here
	}

	EscapeCarShortAction::~EscapeCarShortAction()
	{
		// Nothing here
	}

	void EscapeCarShortAction::init(const std::string& escapeTargetTag)
	{
		// TODO(fw): Evaluate escape target tag!

		mEscapeTargetTag = escapeTargetTag;

		std::vector<VehicleSpawnPointComponent*> exitVehicleSpawnPoints;

		// Sort the vector for entry and exit
		for (VehicleSpawnPointComponent* vehicleSpawnPointComponent : qsf::ComponentMapQuery(getMap()).getAllInstances<VehicleSpawnPointComponent>())
		{
			// Test if we have the right gate
			if (vehicleSpawnPointComponent->isActive() && vehicleSpawnPointComponent->isExit())
			{
				exitVehicleSpawnPoints.push_back(vehicleSpawnPointComponent);
			}
		}

		const qsf::TransformComponent& selfTransformComponent = getEntity().getComponentSafe<qsf::TransformComponent>();
		const glm::vec3& selfPosition = selfTransformComponent.getPosition();
		float shortestSquaredDistance = std::numeric_limits<float>::max();
		qsf::logic::TargetPoint moveTargetPoint;

		for (VehicleSpawnPointComponent* vehicleSpawnPointComponent : exitVehicleSpawnPoints)
		{
			const qsf::TransformComponent& spawnPointTransformComponent = vehicleSpawnPointComponent->getEntity().getComponentSafe<qsf::TransformComponent>();

			const glm::vec3& pointPosition = spawnPointTransformComponent.getPosition();
			const float squaredDistance = glm::distance2(pointPosition, selfPosition);
			if (squaredDistance < shortestSquaredDistance)
			{
				shortestSquaredDistance = squaredDistance;
				moveTargetPoint.mPosition = pointPosition;
			}
		}
		mTargetPosition = moveTargetPoint.mPosition;

		// Call base class implementation
		qsf::ai::MoveAction::init(new qsf::ai::ReachSinglePointGoal(moveTargetPoint), MovementModes::MOVEMENT_MODE_RUN, nullptr);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Action methods                    ]
	//[-------------------------------------------------------]
	void EscapeCarShortAction::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base implementation
		qsf::ai::MoveAction::serialize(serializer);

		serializer.serialize(mIsCivilPerson);
		serializer.serialize(mStuckTime);
		serializer.serialize(mTargetPosition);
		serializer.serialize(mOldPosition);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	qsf::action::Result EscapeCarShortAction::updateAction(const qsf::Clock& clock)
	{
		if (!checkAllowUpdate())
		{
			return qsf::action::RESULT_CONTINUE;
		}

		// Call base implementation
		qsf::action::Result result = qsf::ai::MoveAction::updateAction(clock);

		// In EM5, a failed AI movement should not lead to automatic clearing of the action plan
		if (qsf::action::RESULT_CLEARLIST == result)
			result = qsf::action::RESULT_DONE;

		if (result == qsf::action::RESULT_DONE)
		{
			EntityHelper entityHelper(getEntity());
			qsf::Entity* car = entityHelper.getContainerEntity();
			if (nullptr != car && entityHelper.isInMapBoundaries())
				if (qsf::game::DistanceHelper::getDistance(mTargetPosition, car->getOrCreateComponent<qsf::TransformComponent>()->getPosition()) >= 20.f)
				{
					entityHelper.turnIntoGangster("em5::GangsterCarHijackerLogic", true);
					qsf::GameLogicComponent& gameLogicComponent = getEntity().getOrCreateComponentSafe<qsf::GameLogicComponent>();
					GangsterCarHijackerLogic* gameLogic = gameLogicComponent.getGameLogic<GangsterCarHijackerLogic>();
					gameLogic->escapeOnFoot(*car);
					return result;
				}
			// Send message
			qsf::MessageParameters parameters;
			parameters.setParameter("GangsterId", getEntityId());
			QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_GANGSTER_ESCAPED, getEntityId()), parameters);

			MapHelper::destroyEntity(getEntity());
			if (nullptr != car)
				MapHelper::destroyEntity(*car);
		}

		return result;
	}

	bool EscapeCarShortAction::onStartup()
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

	void EscapeCarShortAction::onShutdown()
	{
		// Call base implementation
		qsf::ai::MoveAction::onShutdown();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	uint32 EscapeCarShortAction::getMovementMode(uint32 movementModeId) const
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

	bool EscapeCarShortAction::checkAllowUpdate() const
	{
		// TODO(mk) To optimize this function we can keep the gameComponent as member
		if (mIsCivilPerson)
		{
			return getMap().getCoreEntity().getOrCreateComponent<GameComponent>()->getCivilPersonMovement();
		}
		return true;
	}

	bool EscapeCarShortAction::checkPositionStuck()
	{
		glm::vec3 newPosition = EntityHelper(getEntity()).getPosition();
		bool positionStuck = (mOldPosition == newPosition ? true : false);
		mOldPosition = newPosition;

		return positionStuck;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
