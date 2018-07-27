// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/movement/RandomMovementComponent.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include "em5/action/move/MoveAction.h"
#include "em5/health/HealthComponent.h"

#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>

#include <qsf_logic/targetpoint/TargetPoint.h>

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/Entity.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 RandomMovementComponent::COMPONENT_ID = qsf::StringHash("em5::RandomMovementComponent");


	//[-------------------------------------------------------]
	//[ Public methods                                    ]
	//[-------------------------------------------------------]
	RandomMovementComponent::RandomMovementComponent(qsf::Prototype* prototype):
		qsf::game::RandomMovementComponent(prototype)
	{
		// Nothing here
	}

	RandomMovementComponent::~RandomMovementComponent()
	{
		// Nothing here
	}


	//[-----------------------------------------------------------]
	//[ Public virtual qsf::game::RandomMovementComponent methods ]
	//[-----------------------------------------------------------]
	bool RandomMovementComponent::onUpdateSimulation()
	{
		const HealthComponent* healthComponent = getEntity().getComponent<HealthComponent>();
		if (nullptr != healthComponent && !healthComponent->isMovable())
		{
			return false;
		}
		else
		{
			// Filter out damaged vehicles
			const RoadVehicleComponent* roadVehicleComponent = getEntity().getComponent<RoadVehicleComponent>();
			if (nullptr != roadVehicleComponent && !roadVehicleComponent->isMovable())
			{
				return false;
			}
		}
		return true;
	}


	//[-----------------------------------------------------------]
	//[ Private virtual qsf::game::RandomMovementComponent methods ]
	//[-----------------------------------------------------------]
	void RandomMovementComponent::pushMoveAction(qsf::ActionComponent& actionComponent, qsf::logic::TargetPoint& moveTargetPoint)
	{
		actionComponent.pushAction<MoveAction>(getActionPriority()).init(new qsf::ai::ReachSinglePointGoal(moveTargetPoint));
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
