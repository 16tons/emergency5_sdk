// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/game/targetpoint/PickupVehicleByTowcarTargetPointProvider.h"

#include <qsf_game/component/event/EventTagComponent.h>

#include <qsf_logic/targetpoint/TargetPointManager.h>

#include <qsf/math/EulerAngles.h>
#include <qsf/math/Math.h>
#include <qsf/math/CoordinateSystem.h>
#include <qsf/map/Entity.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 PickupVehicleByTowcarTargetPointProvider::TARGET_POINT_ID = qsf::StringHash("PickupVehicleByTowcarTargetPointProvider");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PickupVehicleByTowcarTargetPointProvider::PickupVehicleByTowcarTargetPointProvider(qsf::logic::TargetPointManager* manager) :
		qsf::logic::TargetPointProvider(manager)
	{
		// Register callbacks
		manager->registerProvider(TARGET_POINT_ID, this);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::logic::TargetPointProvider methods ]
	//[-------------------------------------------------------]
	void PickupVehicleByTowcarTargetPointProvider::calculateTargetPoints(qsf::Entity& caller, const qsf::Transform& callerTransform, qsf::Entity& target, const qsf::Transform& targetTransform, std::vector<qsf::logic::TargetPoint>& targetPoints) const
	{
		const glm::vec3 targetPosition = targetTransform.getPosition();
		const glm::quat rotation = targetTransform.getRotation();

		// Get an 2D (x,z) rotation
		const glm::quat roationInXZPlane = qsf::Math::getRotationByUpVector(qsf::CoordinateSystem::getUp(), rotation);
		const glm::quat flipAtX = qsf::EulerAngles::eulerToQuaternion(glm::pi<float>(), 0.0f, 0.0f);

		qsf::game::EventTagComponent* eventTagComponent = target.getComponent<qsf::game::EventTagComponent>();
		if (nullptr != eventTagComponent && eventTagComponent->hasTag(qsf::StringHash("MUNICH_ME01_TRUCK")))
		{
			// Special handling for the ME01 truck
			const float placeOffset = 4.4f;
			const glm::vec3 positionOffset = roationInXZPlane * qsf::CoordinateSystem::getRight() * placeOffset;

			// The center position + offset to the left and flipped
			targetPoints.emplace_back(targetPosition - positionOffset, false, 0.0f, 2.5f, roationInXZPlane * flipAtX);
		}
		else
		{
			// The offset of the car to the right
			const float placeOffset = 3.8f;
			const glm::vec3 positionOffset = roationInXZPlane * qsf::CoordinateSystem::getRight() * placeOffset;

			// The center position + offset to the right
			targetPoints.emplace_back(targetPosition + positionOffset, false, 0.0f, 2.f, roationInXZPlane);

			// The center position + offset to the left and flipped
			targetPoints.emplace_back(targetPosition - positionOffset, false, 0.0f, 2.f, roationInXZPlane * flipAtX);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
