// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/game/targetpoint/CarryPersonTargetPointProvider.h"

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
	const uint32 CarryPersonTargetPointProvider::TARGET_POINT_ID = qsf::StringHash("CarryPersonTargetPointProvider");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	CarryPersonTargetPointProvider::CarryPersonTargetPointProvider(qsf::logic::TargetPointManager* manager) :
		qsf::logic::TargetPointProvider(manager)
	{
		// Register callbacks
		manager->registerProvider(TARGET_POINT_ID, this);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::logic::TargetPointProvider methods ]
	//[-------------------------------------------------------]
	void CarryPersonTargetPointProvider::calculateTargetPoints(qsf::Entity& caller, const qsf::Transform& callerTransform, qsf::Entity& target, const qsf::Transform& targetTransform, std::vector<qsf::logic::TargetPoint>& targetPoints) const
	{
		const glm::vec3 targetPosition = targetTransform.getPosition();
		const glm::quat rotation = targetTransform.getRotation();

		// Get an 2D (x,z) rotation
		const glm::quat roationInXZPlane = qsf::Math::getRotationByUpVector(qsf::CoordinateSystem::getUp(), rotation);

		// The offset of the target to the right
		const float placeOffset = 0.5f;
		const glm::vec3 positionOffset = roationInXZPlane * qsf::CoordinateSystem::getRight() * placeOffset;

		// The center position + offset to the right
		targetPoints.emplace_back(targetPosition + positionOffset, false, 0.0f, 0.5f, roationInXZPlane);

		// The center position + offset to the left
		targetPoints.emplace_back(targetPosition - positionOffset, false, 0.0f, 0.5f, roationInXZPlane);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
