// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/game/targetpoint/EnterVehicleTargetPointProvider.h"
#include "em5/component/vehicle/VehicleComponent.h"
#include "em5/game/units/UnitHelper.h"

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
	const uint32 EnterVehicleTargetPointProvider::TARGET_POINT_ID = qsf::StringHash("EnterVehicleTargetPointProvider");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	EnterVehicleTargetPointProvider::EnterVehicleTargetPointProvider(qsf::logic::TargetPointManager* manager) :
		qsf::logic::TargetPointProvider(manager)
	{
		// Register callbacks
		manager->registerProvider(TARGET_POINT_ID, this);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::logic::TargetPointProvider methods ]
	//[-------------------------------------------------------]
	void EnterVehicleTargetPointProvider::calculateTargetPoints(qsf::Entity& caller, const qsf::Transform& callerTransform, qsf::Entity& target, const qsf::Transform& targetTransform, std::vector<qsf::logic::TargetPoint>& targetPoints) const
	{
		VehicleComponent* vehicleComponent = target.getComponent<VehicleComponent>();
		if (nullptr != vehicleComponent)
		{
			// Read the door type from the edited property
			DoorComponent::DoorType usedDoorType = vehicleComponent->getMainDoorType();

			std::vector<qsf::Transform> usedDoors;
			vehicleComponent->getDoorPositions(usedDoorType, usedDoors, targetTransform);

			for (const qsf::Transform& transform : usedDoors)
			{
				targetPoints.emplace_back(transform.getPosition(), true, 0.0f, 0.1f, transform.getRotation());

				if (UnitHelper(target).isPoliceSEKHeli())
				{
					// Extra logic for sek helikopter, use only one door (we are not able to edit in an other way without breaking the perfromRopeAction)
					break;
				}
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
