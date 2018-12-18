// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/game/targetpoint/PickupPersonWithParamedicTargetPointProvider.h"

#include <qsf_logic/targetpoint/TargetPointManager.h>

#include <qsf/math/CoordinateSystem.h>
#include <qsf/math/Math.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 PickupPersonWithParamedicTargetPointProvider::TARGET_POINT_ID = qsf::StringHash("PickupPersonWithParamedicTargetPointProvider");

	static const glm::vec3 PERSON_OFFSET(-0.829f, 0.0f, 0.845f);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PickupPersonWithParamedicTargetPointProvider::PickupPersonWithParamedicTargetPointProvider(qsf::logic::TargetPointManager* manager) :
		qsf::logic::TargetPointProvider(manager)
	{
		// Register callbacks
		manager->registerProvider(TARGET_POINT_ID, this);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::logic::TargetPointProvider methods ]
	//[-------------------------------------------------------]
	void PickupPersonWithParamedicTargetPointProvider::calculateTargetPoints(qsf::Entity& caller, const qsf::Transform& callerTransform, qsf::Entity& target, const qsf::Transform& targetTransform, std::vector<qsf::logic::TargetPoint>& targetPoints) const
	{
		glm::vec3 targetPosition = targetTransform.vec3PositionLocalToWorld(PERSON_OFFSET);
		glm::quat targetRotation = targetTransform.getRotation();

		// Turn 180° to the target (to have the persons head on the same position as before)
		targetRotation = targetRotation * qsf::Math::getRotationAroundAxis(qsf::CoordinateSystem::getUp(), glm::pi<float>());

		targetPoints.emplace_back(targetPosition, false, 0.0f, 0.3f, targetRotation);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
