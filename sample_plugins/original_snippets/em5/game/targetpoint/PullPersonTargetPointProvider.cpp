// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/game/targetpoint/PullPersonTargetPointProvider.h"

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
	const uint32 PullPersonTargetPointProvider::TARGET_POINT_ID = qsf::StringHash("PullPersonTargetPointProvider");


	//[---------------------------------------------------]
	//[ Public static methods                             ]
	//[---------------------------------------------------]
	glm::vec3 PullPersonTargetPointProvider::getOffset()
	{
		return glm::vec3(0, 0, 2.139);	// TODO(mz): only works for adult man, woman and children need other offsets
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PullPersonTargetPointProvider::PullPersonTargetPointProvider(qsf::logic::TargetPointManager* manager) :
		qsf::logic::TargetPointProvider(manager)
	{
		// Register callbacks
		manager->registerProvider(TARGET_POINT_ID, this);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::logic::TargetPointProvider methods ]
	//[-------------------------------------------------------]
	void PullPersonTargetPointProvider::calculateTargetPoints(qsf::Entity& caller, const qsf::Transform& callerTransform, qsf::Entity& target, const qsf::Transform& targetTransform, std::vector<qsf::logic::TargetPoint>& targetPoints) const
	{
		glm::vec3 targetPosition = targetTransform.getPosition();

		// Add offset to target position
		glm::vec3 offsetLocal(getOffset());	// Exact offset received from artists
		glm::vec3 offsetWorld = targetTransform.vec3DirectionLocalToWorld(offsetLocal);

		targetPosition += offsetWorld;

		glm::quat orientation = qsf::Math::getLookAtQuaternion(-offsetWorld, qsf::CoordinateSystem::getUp());
		targetPoints.emplace_back(targetPosition, false, 0.0f, 0.1f, orientation);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
