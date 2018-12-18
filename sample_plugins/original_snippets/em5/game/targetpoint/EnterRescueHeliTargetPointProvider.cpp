// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/game/targetpoint/EnterRescueHeliTargetPointProvider.h"
#include "em5/component/vehicle/VehicleComponent.h"

#include <qsf_logic/targetpoint/TargetPointManager.h>

#include <qsf/map/Entity.h>
#include <qsf/math/EulerAngles.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 EnterRescueHeliTargetPointProvider::TARGET_POINT_ID = qsf::StringHash("EnterRescueHeliTargetPointProvider");

	const glm::vec3 EnterRescueHeliTargetPointProvider::FINAL_PARAMEDIC_OFFSET = glm::vec3(1.382f, 0.f, 3.35f);
	const glm::quat EnterRescueHeliTargetPointProvider::FINAL_PARAMEDIC_ROTATION = qsf::EulerAngles::eulerToQuaternion(51.198f, 0.f, 0.f);

	const glm::vec3 EnterRescueHeliTargetPointProvider::COLLISIONFREE_PARAMEDIC_OFFSET = glm::vec3(3.20f, 0.f, 3.40f);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	EnterRescueHeliTargetPointProvider::EnterRescueHeliTargetPointProvider(qsf::logic::TargetPointManager* manager) :
		qsf::logic::TargetPointProvider(manager)
	{
		// Register callbacks
		manager->registerProvider(TARGET_POINT_ID, this);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::logic::TargetPointProvider methods ]
	//[-------------------------------------------------------]
	void EnterRescueHeliTargetPointProvider::calculateTargetPoints(qsf::Entity& caller, const qsf::Transform& callerTransform, qsf::Entity& target, const qsf::Transform& targetTransform, std::vector<qsf::logic::TargetPoint>& targetPoints) const
	{
		glm::vec3 finalParamedicPosition = targetTransform.vec3PositionLocalToWorld(COLLISIONFREE_PARAMEDIC_OFFSET);

		targetPoints.emplace_back(finalParamedicPosition, true, 0.0f, 0.1f);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
