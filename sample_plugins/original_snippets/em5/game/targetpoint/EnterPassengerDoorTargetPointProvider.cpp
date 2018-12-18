// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/game/targetpoint/EnterPassengerDoorTargetPointProvider.h"
#include "em5/component/vehicle/VehicleComponent.h"

#include <qsf_logic/targetpoint/TargetPointManager.h>

#include <qsf/map/Entity.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 EnterPassengerDoorTargetPointProvider::TARGET_POINT_ID = qsf::StringHash("EnterPassengerDoorTargetPoint");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	EnterPassengerDoorTargetPointProvider::EnterPassengerDoorTargetPointProvider(qsf::logic::TargetPointManager* manager) :
		qsf::logic::TargetPointProvider(manager)
	{
		// Register callbacks
		manager->registerProvider(TARGET_POINT_ID, this);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::logic::TargetPointProvider methods ]
	//[-------------------------------------------------------]
	void EnterPassengerDoorTargetPointProvider::calculateTargetPoints(qsf::Entity& caller, const qsf::Transform& callerTransform, qsf::Entity& target, const qsf::Transform& targetTransform, std::vector<qsf::logic::TargetPoint>& targetPoints) const
	{
		VehicleComponent* vehicleComponent = target.getComponent<VehicleComponent>();
		if (nullptr != vehicleComponent)
		{
			std::vector<qsf::Transform> passengerDoors;
			vehicleComponent->getDoorPositions(DoorComponent::DOORTYPE_PASSENGER, passengerDoors, targetTransform);

			for (const qsf::Transform& transform : passengerDoors)
			{
				targetPoints.emplace_back(transform.getPosition(), true, 0.0f, 0.1f, transform.getRotation());
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
