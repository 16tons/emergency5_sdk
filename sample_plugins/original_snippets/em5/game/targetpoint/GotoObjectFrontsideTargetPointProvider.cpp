// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/game/targetpoint/GotoObjectFrontsideTargetPointProvider.h"

#include <qsf_logic/targetpoint/TargetPointManager.h>

#include <qsf/physics/collision/BulletBoxCollisionComponent.h>
#include <qsf/map/Entity.h>
#include <qsf/math/CoordinateSystem.h>
#include <qsf/math/EulerAngles.h>
#include <qsf/math/Math.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 GotoObjectFrontsideTargetPointProvider::TARGET_POINT_ID = qsf::StringHash("GotoObjectFrontsideTargetPointProvider");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GotoObjectFrontsideTargetPointProvider::GotoObjectFrontsideTargetPointProvider(qsf::logic::TargetPointManager* manager) :
		qsf::logic::TargetPointProvider(manager)
	{
		// Register callbacks
		manager->registerProvider(TARGET_POINT_ID, this);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::logic::TargetPointProvider methods ]
	//[-------------------------------------------------------]
	void GotoObjectFrontsideTargetPointProvider::calculateTargetPoints(qsf::Entity& caller, const qsf::Transform& callerTransform, qsf::Entity& target, const qsf::Transform& targetTransform, std::vector<qsf::logic::TargetPoint>& targetPoints) const
	{
		const glm::vec3 targetPosition = targetTransform.getPosition();
		const glm::quat rotation = targetTransform.getRotation();// * qsf::EulerAngles::eulerToQuaternion(glm::pi<float>() / 2.0f, 0.0f, 0.0f);	// TODO(co) Why is this commented?

		// Get an 2D (x,z) rotation
		const glm::quat rotationInXZPlane = qsf::Math::getRotationByUpVector(qsf::CoordinateSystem::getUp(), rotation);

		// Default distance from object center is 1 meter
		float distance = 1.0f;

		qsf::BulletBoxCollisionComponent* bulletBoxCollisionComponent = target.getComponent<qsf::BulletBoxCollisionComponent>();
		if (nullptr != bulletBoxCollisionComponent)
		{
			distance = bulletBoxCollisionComponent->getHalfExtents().z + 0.7f;

			// Dumb hack for EM2016 mini-event "plane crash": Use the back side instead of the front side (we have some strange editing here)
			if (target.getId() == uint64(12744954553371035260u))
			{
				distance = -distance;
			}
		}

		// Add 1 m to the front side of the target object, so that the position is in front of the target
		const glm::vec3 position = targetPosition + glm::vec3(0.0f, 0.0f, -distance) * glm::inverse(rotationInXZPlane);

		targetPoints.emplace_back(position, false, 0.0f, 0.1f);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
