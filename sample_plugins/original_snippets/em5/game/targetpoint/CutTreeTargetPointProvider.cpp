// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/game/targetpoint/CutTreeTargetPointProvider.h"
#include "em5/map/EntityHelper.h"

#include <qsf_logic/targetpoint/TargetPointManager.h>

#include <qsf/math/Math.h>
#include <qsf/math/CoordinateSystem.h>
#include <qsf/physics/collision/CollisionComponent.h>
#include <qsf/map/Entity.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 CutTreeTargetPointProvider::TARGET_POINT_ID = qsf::StringHash("CutTreeTargetPointProvider");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	CutTreeTargetPointProvider::CutTreeTargetPointProvider(qsf::logic::TargetPointManager* manager) :
		qsf::logic::TargetPointProvider(manager)
	{
		// Register callbacks
		manager->registerProvider(TARGET_POINT_ID, this);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::logic::TargetPointProvider methods ]
	//[-------------------------------------------------------]
	void CutTreeTargetPointProvider::calculateTargetPoints(qsf::Entity& caller, const qsf::Transform& callerTransform, qsf::Entity& target, const qsf::Transform& targetTransform, std::vector<qsf::logic::TargetPoint>& targetPoints) const
	{
		const auto& groundPlaneProjector = qsf::CoordinateSystem::getTopViewProjector();

		const qsf::CollisionComponent* collision = caller.getComponent<qsf::CollisionComponent>();
		// Determine distance to keep as the sum of the collision radii
		float distanceToKeep = 0.0f;

		glm::vec3 collisionCenter; // information is not needed but required by the interface
		glm::quat collisionRotation; // information is not needed but required by the interface
		glm::vec3 extensions;
		if (collision && collision->getAsOrientedBoundingBox(collisionCenter, collisionRotation, extensions))
		{
			// TODO(vs) Move this calculation into the collision component interface
			// restricting the collision to 2D and ignoring the height value
			glm::vec2 boundingBox2D = groundPlaneProjector.dropValue(extensions);
			// calculating the radius of a circumscribed circle in 2D, taking only the half extension length
			const float collisionRadius = std::sqrt(glm::dot(boundingBox2D * 0.5f, boundingBox2D * 0.5f));
			distanceToKeep += collisionRadius;
		}

		collision = target.getComponent<qsf::CollisionComponent>();
		if (collision && collision->getAsOrientedBoundingBox(collisionCenter, collisionRotation, extensions))
		{
			// see above for comments
			glm::vec2 boundingBox2D = groundPlaneProjector.dropValue(extensions);
			const float collisionRadius = std::sqrt(glm::dot(boundingBox2D * 0.5f, boundingBox2D * 0.5f));
			distanceToKeep += collisionRadius;
		}

		// Required orientation towards the entity in local coordinates
		glm::quat orientation; // leads to a default orientation

		// trees that are cut down are not dynamic by definition here
		targetPoints.emplace_back(targetTransform.getPosition(), false, distanceToKeep, 0.1f, orientation);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
