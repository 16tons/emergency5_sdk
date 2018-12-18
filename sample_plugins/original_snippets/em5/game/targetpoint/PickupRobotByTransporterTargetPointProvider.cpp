// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/game/targetpoint/PickupRobotByTransporterTargetPointProvider.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"

#include <qsf_logic/targetpoint/TargetPointManager.h>

#include <qsf/map/Entity.h>
#include <qsf/math/Math.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 PickupRobotByTransporterTargetPointProvider::TARGET_POINT_ID = qsf::StringHash("PickupRobotByTransporterTargetPointProvider");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PickupRobotByTransporterTargetPointProvider::PickupRobotByTransporterTargetPointProvider(qsf::logic::TargetPointManager* manager) :
		qsf::logic::TargetPointProvider(manager)
	{
		// Register callbacks
		manager->registerProvider(TARGET_POINT_ID, this);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::logic::TargetPointProvider methods ]
	//[-------------------------------------------------------]
	void PickupRobotByTransporterTargetPointProvider::calculateTargetPoints(qsf::Entity& caller, const qsf::Transform& callerTransform, qsf::Entity& target, const qsf::Transform& targetTransform, std::vector<qsf::logic::TargetPoint>& targetPoints) const
	{
		glm::vec3 doorPosition;

		RoadVehicleComponent* roadVehicleComponent = caller.getComponent<RoadVehicleComponent>();
		if (nullptr == roadVehicleComponent)
			return;

		roadVehicleComponent->getDoorPosition(DoorComponent::DOORTYPE_TRUNK, doorPosition);

		// Calculate the distance transporter to door
		const glm::vec3 doorOffset = callerTransform.getPosition() - doorPosition;
		const glm::vec3 targetPosition = targetTransform.getPosition();
		const glm::quat transporterRotation = qsf::Math::getLookAtQuaternion(doorOffset, qsf::CoordinateSystem::getUp());

		// The center position + offset
		targetPoints.emplace_back(targetPosition + doorOffset, false, 0.0f, 0.1f, transporterRotation);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
