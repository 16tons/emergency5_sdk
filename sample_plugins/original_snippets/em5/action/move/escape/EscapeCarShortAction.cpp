// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/move/escape/EscapeCarShortAction.h"
#include "em5/ai/MovementModes.h"
#include "em5/component/spawnpoint/civilist/VehicleSpawnPointComponent.h"
#include "em5/logic/local/gangsters/GangsterCarHijackerLogic.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/specs/SpecsGroupManager.h"
#include "em5/plugin/Messages.h"

#include <qsf_game/base/DistanceHelper.h>

#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>

#include <qsf/logic/gamelogic/GameLogicComponent.h>

#include <qsf/component/base/TransformComponent.h>
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
		EscapeAction(ACTION_ID)
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

		serializer.serialize(mTargetPosition);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	qsf::action::Result EscapeCarShortAction::updateAction(const qsf::Clock& clock)
	{
		if (!EscapeAction::checkAllowUpdate())
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
			{
				if (qsf::game::DistanceHelper::getDistance(mTargetPosition, car->getOrCreateComponent<qsf::TransformComponent>()->getPosition()) >= 20.f)
				{
					entityHelper.turnIntoGangster("em5::GangsterCarHijackerLogic", true);
					qsf::GameLogicComponent& gameLogicComponent = getEntity().getOrCreateComponentSafe<qsf::GameLogicComponent>();
					GangsterCarHijackerLogic* gameLogic = gameLogicComponent.getGameLogic<GangsterCarHijackerLogic>();
					gameLogic->escapeOnFoot(*car);
					return result;
				}
			}

			// Gangster is outside the map, remove him
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

	void EscapeCarShortAction::onShutdown()
	{
		// Call base implementation
		EscapeAction::onShutdown();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
