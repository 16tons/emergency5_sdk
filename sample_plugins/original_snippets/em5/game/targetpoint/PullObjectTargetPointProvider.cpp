// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/game/targetpoint/PullObjectTargetPointProvider.h"

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
	const uint32 PullObjectTargetPointProvider::TARGET_POINT_ID = qsf::StringHash("PullObjectTargetPointProvider");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PullObjectTargetPointProvider::PullObjectTargetPointProvider(qsf::logic::TargetPointManager* manager) :
		qsf::logic::TargetPointProvider(manager)
	{
		// Register callbacks
		manager->registerProvider(TARGET_POINT_ID, this);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::logic::TargetPointProvider methods ]
	//[-------------------------------------------------------]
	void PullObjectTargetPointProvider::calculateTargetPoints(qsf::Entity& caller, const qsf::Transform& callerTransform, qsf::Entity& target, const qsf::Transform& targetTransform, std::vector<qsf::logic::TargetPoint>& targetPoints) const
	{
		glm::vec3 callerPosition = callerTransform.getPosition();
		glm::vec3 targetPosition = targetTransform.getPosition();

		// Calculate difference
		glm::vec3 difference = targetPosition - callerPosition;
		difference.y = 0;

		if (glm::length(difference) != 0.0f)
		{
			// Add offset to targetPosition
			targetPosition -= glm::normalize(difference) * 2.0f;
		}

		glm::quat orientation = qsf::Math::getLookAtQuaternion(difference, qsf::CoordinateSystem::getUp());
		targetPoints.emplace_back(targetPosition, false, 0.0f, 0.1f, orientation);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
