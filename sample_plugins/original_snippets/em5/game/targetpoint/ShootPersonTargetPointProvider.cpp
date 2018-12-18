// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/game/targetpoint/ShootPersonTargetPointProvider.h"
#include "em5/action/base/ShootAction.h"
#include "em5/action/base/HuntAndShootAction.h"

#include <qsf_logic/targetpoint/TargetPointManager.h>

#include <qsf/math/Math.h>
#include <qsf/math/CoordinateSystem.h>
#include <qsf/logic/action/ActionComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 ShootPersonTargetPointProvider::TARGET_POINT_ID = qsf::StringHash("ShootPersonTargetPointProvider");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	ShootPersonTargetPointProvider::ShootPersonTargetPointProvider(qsf::logic::TargetPointManager* manager) :
		qsf::logic::TargetPointProvider(manager)
	{
		// Register callbacks
		manager->registerProvider(TARGET_POINT_ID, this);
	}

	float ShootPersonTargetPointProvider::getShootRange(qsf::Entity& shooter)
	{
		qsf::ActionComponent* actionComponent = shooter.getComponent<qsf::ActionComponent>();
		if (nullptr != actionComponent)
		{
			// TODO(fw): Would be nice if we could
			//  a) Unify both types of shoot action
			//  b) Get the shoot range directly from the person or the equipment

			ShootAction* shootAction = actionComponent->getAction<ShootAction>();
			if (nullptr != shootAction)
			{
				return shootAction->getShootRange();
			}

			HuntAndShootAction* huntAndShootAction = actionComponent->getAction<HuntAndShootAction>();
			if (nullptr != huntAndShootAction)
			{
				return huntAndShootAction->getShootRange();
			}
		}

		// TODO(mk) 18 is an unknown, not tested value for range weapons. Make this editable via specs
		return 18.0f;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::logic::TargetPointProvider methods ]
	//[-------------------------------------------------------]
	void ShootPersonTargetPointProvider::calculateTargetPoints(qsf::Entity& caller, const qsf::Transform& callerTransform, qsf::Entity& target, const qsf::Transform& targetTransform, std::vector<qsf::logic::TargetPoint>& targetPoints) const
	{
		glm::vec3 callerPosition = callerTransform.getPosition();
		glm::vec3 targetPosition = targetTransform.getPosition();

		// Calculate difference
		glm::vec3 difference = targetPosition - callerPosition;
		difference.y = 0;

		glm::quat orientation = qsf::Math::getLookAtQuaternion(difference, qsf::CoordinateSystem::getUp());
		float shootRange = getShootRange(caller) * 0.8f;
		targetPoints.emplace_back(targetPosition, true, shootRange, 2.0f, orientation);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
