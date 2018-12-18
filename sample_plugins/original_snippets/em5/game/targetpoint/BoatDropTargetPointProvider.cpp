// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/game/targetpoint/BoatDropTargetPointProvider.h"
#include "em5/component/objects/BoatDropPointComponent.h"

#include <qsf_logic/targetpoint/TargetPointManager.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/map/Entity.h>
#include <qsf/math/CoordinateSystem.h>
#include <qsf/math/EulerAngles.h>
#include <qsf/math/Math.h>
#include <qsf/math/Transform.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 BoatDropTargetPointProvider::TARGET_POINT_ID = qsf::StringHash("BoatDropTargetPointProvider");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	BoatDropTargetPointProvider::BoatDropTargetPointProvider(qsf::logic::TargetPointManager* manager) :
		qsf::logic::TargetPointProvider(manager)
	{
		// Register callbacks
		manager->registerProvider(TARGET_POINT_ID, this);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::logic::TargetPointProvider methods]
	//[-------------------------------------------------------]
	void BoatDropTargetPointProvider::calculateTargetPoints(qsf::Entity& caller, const qsf::Transform& callerTransform, qsf::Entity& target, const qsf::Transform& targetTransform, std::vector<qsf::logic::TargetPoint>& targetPoints) const
	{
		const glm::quat rotation = targetTransform.getRotation() * qsf::EulerAngles::eulerToQuaternion(glm::pi<float>() / 2.0f, 0.0f, 0.0f);

		// Get an 2D (x,z) rotation
		const glm::quat rotationInXZPlane = qsf::Math::getRotationByUpVector(qsf::CoordinateSystem::getUp(), rotation);

		// Try get transform from the boat drop entity, which specifies the position where the boat should be dropped/loaded
		qsf::TransformComponent* boatDropTransformComponent = nullptr;
		{
			BoatDropPointComponent* boatDropPointComponent = target.getComponent<BoatDropPointComponent>();
			if (nullptr != boatDropPointComponent)
			{
				boatDropTransformComponent = boatDropPointComponent->getBoatDropTransform();
			}
		}

		const glm::vec3 targetPosition = targetTransform.getPosition();
		glm::vec3 boatPosition;
		if (nullptr != boatDropTransformComponent)
		{
			boatPosition = boatDropTransformComponent->getPosition();

			// From the boat drop entity transform we only need the xz coordinates
			// As y cooridnate we use the target transform
			boatPosition.y = targetPosition.y;
		}
		else
		{
			boatPosition = targetPosition + glm::vec3(-5.0f, 0.0f, 0.0f) * glm::inverse(rotation);
		}

		targetPoints.emplace_back(boatPosition, false, 0.0f, 2.5f, rotationInXZPlane);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
