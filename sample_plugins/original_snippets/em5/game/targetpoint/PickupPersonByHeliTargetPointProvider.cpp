// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/game/targetpoint/PickupPersonByHeliTargetPointProvider.h"
#include "em5/game/collision/CollisionTypes.h"
#include "em5/map/EntityHelper.h"

#include <qsf_logic/targetpoint/TargetPointManager.h>

#include <qsf/physics/collision/CollisionComponent.h>
#include <qsf/physics/collision/CollisionHelper.h>
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
	const uint32 PickupPersonByHeliTargetPointProvider::TARGET_POINT_ID = qsf::StringHash("PickupPersonByHeliTargetPointProvider");


	//[---------------------------------------------------]
	//[ Public static methods                             ]
	//[---------------------------------------------------]
	glm::vec3 PickupPersonByHeliTargetPointProvider::getOffset()
	{
		return glm::vec3(0.208f, 0, -0.138f);
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PickupPersonByHeliTargetPointProvider::PickupPersonByHeliTargetPointProvider(qsf::logic::TargetPointManager* manager) :
		qsf::logic::TargetPointProvider(manager)
	{
		// Register callbacks
		manager->registerProvider(TARGET_POINT_ID, this);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::logic::TargetPointProvider methods ]
	//[-------------------------------------------------------]
	void PickupPersonByHeliTargetPointProvider::calculateTargetPoints(qsf::Entity& caller, const qsf::Transform& callerTransform, qsf::Entity& target, const qsf::Transform& targetTransform, std::vector<qsf::logic::TargetPoint>& targetPoints) const
	{
		// Add offset to target position
		glm::vec3 offsetLocal(getOffset());	// Exact offset received from artists
		glm::vec3 offsetWorld = targetTransform.vec3DirectionLocalToWorld(offsetLocal);
		glm::vec3 targetPosition = targetTransform.getPosition() + offsetWorld;

		glm::quat orientation = qsf::Math::getLookAtQuaternion(-offsetWorld, qsf::CoordinateSystem::getUp());
		targetPoints.emplace_back(targetPosition, true, 0.0f, 0.1f, orientation);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
