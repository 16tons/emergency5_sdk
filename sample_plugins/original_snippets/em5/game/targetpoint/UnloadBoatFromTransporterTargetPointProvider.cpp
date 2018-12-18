// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/game/targetpoint/UnloadBoatFromTransporterTargetPointProvider.h"

#include <qsf_logic/targetpoint/TargetPointManager.h>

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
	const uint32 UnloadBoatFromTransporterTargetPointProvider::TARGET_POINT_ID = qsf::StringHash("UnloadBoatFromTransporterTargetPointProvider");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	UnloadBoatFromTransporterTargetPointProvider::UnloadBoatFromTransporterTargetPointProvider(qsf::logic::TargetPointManager* manager) :
		qsf::logic::TargetPointProvider(manager)
	{
		// Register callbacks
		manager->registerProvider(TARGET_POINT_ID, this);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::logic::TargetPointProvider methods ]
	//[-------------------------------------------------------]
	void UnloadBoatFromTransporterTargetPointProvider::calculateTargetPoints(qsf::Entity& caller, const qsf::Transform& callerTransform, qsf::Entity& target, const qsf::Transform& targetTransform, std::vector<qsf::logic::TargetPoint>& targetPoints) const
	{
		const glm::vec3 targetPosition = targetTransform.getPosition();
		const glm::quat rotation = targetTransform.getRotation() * qsf::EulerAngles::eulerToQuaternion(glm::pi<float>() / 2.0f, 0.0f, 0.0f);

		// Get an 2D (x,z) rotation
		const glm::quat rotationInXZPlane = qsf::Math::getRotationByUpVector(qsf::CoordinateSystem::getUp(), rotation);

		targetPoints.emplace_back(targetPosition, false, 0.0f, 2.5f, rotationInXZPlane);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
