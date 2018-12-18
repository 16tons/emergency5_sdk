// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/game/targetpoint/GetRescueDogFromVehicleTargetPointProvider.h"
#include "em5/logic/local/ambulance/RescueDogLeaderLogic.h"
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
	const uint32 GetRescueDogFromVehicleTargetPointProvider::TARGET_POINT_ID = qsf::StringHash("GetRescueDogFromVehicleTargetPointProvider");

	static const glm::vec3 GETDOG_FROM_VEHICLE_OFFSET(0.8f, 0.f, 0.3f);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GetRescueDogFromVehicleTargetPointProvider::GetRescueDogFromVehicleTargetPointProvider(qsf::logic::TargetPointManager* manager) :
		qsf::logic::TargetPointProvider(manager)
	{
		// Register callbacks
		manager->registerProvider(TARGET_POINT_ID, this);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::logic::TargetPointProvider methods ]
	//[-------------------------------------------------------]
	void GetRescueDogFromVehicleTargetPointProvider::calculateTargetPoints(qsf::Entity& caller, const qsf::Transform& callerTransform, qsf::Entity& target, const qsf::Transform& targetTransform, std::vector<qsf::logic::TargetPoint>& targetPoints) const
	{
		VehicleComponent* vehicleComponent = target.getComponent<VehicleComponent>();
		if (nullptr != vehicleComponent)
		{
			// Ask the rescue dog logic what case we have, getting dog from vehicle or putting dog back in vehicle
			const bool isConnectedWithRescueDog = nullptr != RescueDogLeaderLogic::getRescueDogEntityFromLeader(caller);

			std::vector<qsf::Transform> trunkDoors;
			vehicleComponent->getDoorPositions(DoorComponent::DOORTYPE_TRUNK, trunkDoors, targetTransform);

			for (const qsf::Transform& transform : trunkDoors)
			{
				const glm::vec3 position = (isConnectedWithRescueDog ? transform.getPosition() : transform.vec3PositionLocalToWorld(GETDOG_FROM_VEHICLE_OFFSET));
				targetPoints.emplace_back(position, true, 0.0f, 0.1f, transform.getRotation());
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
