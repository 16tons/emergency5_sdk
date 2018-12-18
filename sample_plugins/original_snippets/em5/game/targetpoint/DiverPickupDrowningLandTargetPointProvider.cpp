// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/game/targetpoint/DiverPickupDrowningLandTargetPointProvider.h"

#include <qsf_logic/targetpoint/TargetPointManager.h>

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
	const uint32 DiverPickupDrowningLandTargetPointProvider::TARGET_POINT_ID = qsf::StringHash("DiverPickupDrowningLandTargetPointProvider");


	//[---------------------------------------------------]
	//[ Public static methods                             ]
	//[---------------------------------------------------]
	glm::vec3 DiverPickupDrowningLandTargetPointProvider::getOffset()
	{
		return glm::vec3(0, 0, 0.05);	// Tiny offset to make rotate to target work (was originally larger offset which got removed on animation updates)
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	DiverPickupDrowningLandTargetPointProvider::DiverPickupDrowningLandTargetPointProvider(qsf::logic::TargetPointManager* manager) :
		qsf::logic::TargetPointProvider(manager)
	{
		// Register callbacks
		manager->registerProvider(TARGET_POINT_ID, this);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::logic::TargetPointProvider methods]
	//[-------------------------------------------------------]
	void DiverPickupDrowningLandTargetPointProvider::calculateTargetPoints(qsf::Entity& caller, const qsf::Transform& callerTransform, qsf::Entity& target, const qsf::Transform& targetTransform, std::vector<qsf::logic::TargetPoint>& targetPoints) const
	{
		glm::vec3 targetPosition = targetTransform.getPosition();

		// Add offset to target position
		glm::vec3 offsetLocal(getOffset());	// Exact offset received from artists
		glm::vec3 offsetWorld = targetTransform.vec3DirectionLocalToWorld(offsetLocal);

		targetPosition += offsetWorld;

		targetPoints.clear();
		targetPoints.emplace_back(targetPosition, false, 0.0f, 0.01f);	// TODO(mk) Check if the target can move
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
