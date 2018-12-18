// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/game/targetpoint/LeadPersonTargetPointProvider.h"
#include "em5/game/units/UnitHelper.h"

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
	const uint32 LeadPersonTargetPointProvider::TARGET_POINT_ID = qsf::StringHash("LeadPersonTargetPointProvider");


	//[---------------------------------------------------]
	//[ Public static methods                             ]
	//[---------------------------------------------------]
	glm::vec3 LeadPersonTargetPointProvider::getOffset(qsf::Entity& target)
	{
		if (UnitHelper(target).isAmbulanceParamedics())
		{
			// We lead ambulances together
			return glm::vec3(-0.347f, 0.0f, 2.0f);	// Try & error
		}

		return glm::vec3(-0.347f, 0.0f, 0.395f);	// Exact offset received from artists
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	LeadPersonTargetPointProvider::LeadPersonTargetPointProvider(qsf::logic::TargetPointManager* manager) :
		qsf::logic::TargetPointProvider(manager)
	{
		// Register callbacks
		if (manager != nullptr)
			manager->registerProvider(TARGET_POINT_ID, this);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::logic::TargetPointProvider methods ]
	//[-------------------------------------------------------]
	void LeadPersonTargetPointProvider::calculateTargetPoints(qsf::Entity& caller, const qsf::Transform& callerTransform, qsf::Entity& target, const qsf::Transform& targetTransform, std::vector<qsf::logic::TargetPoint>& targetPoints) const
	{
		glm::vec3 targetPosition = targetTransform.getPosition();

		// Add offset to target position
		glm::vec3 offsetLocal(getOffset(target));
		glm::vec3 offsetWorld = targetTransform.vec3DirectionLocalToWorld(offsetLocal);

		targetPosition += offsetWorld;

		glm::quat orientation = qsf::Math::getLookAtQuaternion(-offsetWorld, qsf::CoordinateSystem::getUp());
		targetPoints.emplace_back(targetPosition, false, 0.8f, 0.0f, orientation);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
