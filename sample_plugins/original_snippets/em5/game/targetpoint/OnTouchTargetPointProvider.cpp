// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/game/targetpoint/OnTouchTargetPointProvider.h"
#include "em5/game/collision/CollisionTypes.h"
#include "em5/game/units/UnitHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/CollisionHelper.h"

#include <qsf_logic/targetpoint/TargetPointManager.h>

#include <qsf/physics/collision/CollisionComponent.h>
#include <qsf/map/Entity.h>
#include <qsf/math/Math.h>
#include <qsf/math/CoordinateSystem.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 OnTouchTargetPointProvider::TARGET_POINT_ID = qsf::StringHash("OnTouchTargetPointProvider");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	OnTouchTargetPointProvider::OnTouchTargetPointProvider(qsf::logic::TargetPointManager* manager) :
		qsf::logic::TargetPointProvider(manager)
	{
		// Register callbacks
		manager->registerProvider(TARGET_POINT_ID, this);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::logic::TargetPointProvider methods ]
	//[-------------------------------------------------------]
	void OnTouchTargetPointProvider::calculateTargetPoints(qsf::Entity& caller, const qsf::Transform& callerTransform, qsf::Entity& target, const qsf::Transform& targetTransform, std::vector<qsf::logic::TargetPoint>& targetPoints) const
	{
		bool dynamicTarget = false;
		glm::vec3 targetPosition = targetTransform.getPosition();

		// Get a good radius for the caller
		float callerRadius = 0.5f;
		if (!EntityHelper(caller).isEntityPerson())
		{
			qsf::CollisionComponent* callerCollisionComponent = target.getComponent<qsf::CollisionComponent>();
			if (nullptr != callerCollisionComponent)
			{
				float innerRadius = 0.f;
				glm::vec3 corner1, corner2;
				if (callerCollisionComponent->getAsAxisAlignedBoundingBox(corner1, corner2))
				{
					innerRadius = std::min(glm::abs(corner1.x-corner2.x), glm::abs(corner1.z-corner2.z)) * 0.5f;
				}
				if (innerRadius > 0.0f)
					callerRadius = innerRadius;

				if (UnitHelper(target).isAmbulanceRescueDogLeader())
				{
					// Special case for the rescue dog how is moving back to the leader
					dynamicTarget = true;
				}
			}
		}

		callerRadius += 0.5f;	// Add 0.5 meters for better tolerance in uneaven terrain

		// Find points around the target component
		qsf::CollisionComponent* targetCollisionComponent = target.getComponent<qsf::CollisionComponent>();
		if (targetCollisionComponent)
		{
			// Put a cross - cruzification style not X style - on top of the boundingbox and use its endpoints
			glm::vec3 anchorPoint;
			glm::quat rotation;
			glm::vec3 extensions;
			if (targetCollisionComponent->getAsOrientedBoundingBox(anchorPoint, rotation, extensions))
			{
				// Multiply by 0.5f because we want the radius, not diameter
				glm::vec3 extensionsX(extensions.x * 0.5f, 0.0f, 0.0f);
				glm::vec3 extensionsZ(0.0f, 0.0f, extensions.z * 0.5f);

				// Keep a distance to the boundingbox by the size of the caller radius
				extensionsX.x += callerRadius;
				extensionsZ.z += callerRadius;

				qsf::Transform transform;
				transform.setPosition(targetPosition);
				transform.setRotation(rotation);

				// Find the points for the x-axis (local in target coordinate system)
				targetPoints.emplace_back(transform.vec3PositionLocalToWorld(extensionsX), dynamicTarget, 0.0f, 0.5f);
				targetPoints.emplace_back(transform.vec3PositionLocalToWorld(-extensionsX), dynamicTarget, 0.0f, 0.5f);

				// Find the points for the z-axis (local in target coordinate system)
				targetPoints.emplace_back(transform.vec3PositionLocalToWorld(extensionsZ), dynamicTarget, 0.0f, 0.5f);
				targetPoints.emplace_back(transform.vec3PositionLocalToWorld(-extensionsZ), dynamicTarget, 0.0f, 0.5f);

				// Go through all target points again and correct them to a reachable position
				for (qsf::logic::TargetPoint& targetPoint : targetPoints)
				{
					CollisionHelper collisionHelper(caller.getMap());
					float SEARCH_RADIUS = 3.0f;	// rule of thumb
					glm::vec3 outPos;
					if (collisionHelper.findFreePositionEM3(caller, targetPoint.mPosition, SEARCH_RADIUS, 0.0f, outPos))
					{
						targetPoint.mPosition = outPos;
					}
				}
			}
		}

		// Target probably had no collision component
		if (targetPoints.empty())
		{
			// Try it with findFreePosition
			CollisionHelper collisionHelper(caller.getMap());
			float SEARCH_RADIUS = 3.0f;	// rule of thumb
			glm::vec3 outPos;
			if (collisionHelper.findFreePositionEM3(caller, targetTransform, SEARCH_RADIUS, 0.0f, outPos))
			{
				targetPoints.emplace_back(outPos, dynamicTarget, 0.0f, 0.5f);
			}
			else
			{
				// Last fallback - just try the targetPosition. As findFreePosition failed chances are high AI will fail as well now.
				targetPoints.emplace_back(targetPosition, dynamicTarget, 0.0f, 0.5f);
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
