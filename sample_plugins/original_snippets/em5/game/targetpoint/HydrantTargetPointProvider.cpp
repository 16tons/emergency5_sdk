// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/game/targetpoint/HydrantTargetPointProvider.h"
#include "em5/fire/component/HydrantComponent.h"

#include <qsf_logic/targetpoint/TargetPointManager.h>

#include <qsf/math/EulerAngles.h>
#include <qsf/map/Entity.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 HydrantTargetPointProvider::TARGET_POINT_ID = qsf::StringHash("HydrantTargetPointProvider");

	static const glm::vec3 HYDRANT_OFFSET(0.0f, 0.0f, -0.824f);
	const glm::vec3 HydrantTargetPointProvider::HYDRANT_GROUND_OFFSET(0.0f, 0.0f, -0.5f);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	HydrantTargetPointProvider::HydrantTargetPointProvider(qsf::logic::TargetPointManager* manager) :
		qsf::logic::TargetPointProvider(manager)
	{
		// Register callbacks
		manager->registerProvider(TARGET_POINT_ID, this);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::logic::TargetPointProvider methods ]
	//[-------------------------------------------------------]
	void HydrantTargetPointProvider::calculateTargetPoints(qsf::Entity& caller, const qsf::Transform& callerTransform, qsf::Entity& target, const qsf::Transform& targetTransform, std::vector<qsf::logic::TargetPoint>& targetPoints) const
	{
		glm::vec3 targetPosition = targetTransform.vec3PositionLocalToWorld(HYDRANT_OFFSET);
		// Check if target is a ground hydrant, then take other offset
		HydrantComponent* hydrantComponent = target.getComponent<HydrantComponent>();
		if (nullptr != hydrantComponent)
		{
			if (hydrantComponent->getHydrantType() == HydrantComponent::GROUND_HYDRANT)
			{
				targetPosition = targetTransform.vec3PositionLocalToWorld(HYDRANT_GROUND_OFFSET);
			}
		}

		static const float ROTATION_IN_RADIANDS = (180 * glm::pi<float>()/180.f);
		glm::quat targetRotation = targetTransform.quatRotationLocalToWorld(qsf::EulerAngles::eulerToQuaternion(ROTATION_IN_RADIANDS, 0, 0));

		targetPoints.emplace_back(targetPosition, false, 0.0f, 0.1f, targetRotation);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
