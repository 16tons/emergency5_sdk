// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/game/targetpoint/GetEquipmentTargetPointProvider.h"
#include "em5/component/vehicle/VehicleComponent.h"
#include "em5/map/EntityHelper.h"

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
	const uint32 GetEquipmentTargetPointProvider::TARGET_POINT_ID = qsf::StringHash("GetEquipmentTargetPointProvider");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GetEquipmentTargetPointProvider::GetEquipmentTargetPointProvider(qsf::logic::TargetPointManager* manager) :
		qsf::logic::TargetPointProvider(manager)
	{
		// Register callbacks
		manager->registerProvider(TARGET_POINT_ID, this);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::logic::TargetPointProvider methods ]
	//[-------------------------------------------------------]
	void GetEquipmentTargetPointProvider::calculateTargetPoints(qsf::Entity& caller, const qsf::Transform& callerTransform, qsf::Entity& target, const qsf::Transform& targetTransform, std::vector<qsf::logic::TargetPoint>& targetPoints) const
	{
		EntityHelper targetHelper(target);
		if (targetHelper.isSquadRoadVehicle())
		{
			VehicleComponent& vehicleComponent = target.getComponentSafe<VehicleComponent>();

			std::vector<qsf::Transform> equipmentLeft;
			std::vector<qsf::Transform> equipmentRight;
			vehicleComponent.getDoorPositions(DoorComponent::DOORTYPE_EQUIPMENTLEFT, equipmentLeft, targetTransform);
			vehicleComponent.getDoorPositions(DoorComponent::DOORTYPE_EQUIPMENTRIGHT, equipmentRight, targetTransform);

			for (const qsf::Transform& transform : equipmentLeft)
			{
				targetPoints.emplace_back(transform.getPosition(), true, 0.0f, 0.1f, transform.getRotation());
			}

			for (const qsf::Transform& transform : equipmentRight)
			{
				targetPoints.emplace_back(transform.getPosition(), true, 0.0f, 0.1f, transform.getRotation());
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
