// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/game/targetpoint/ThrowStunGrenadeTargetPointProvider.h"
#include "em5/specs/ActionRangeSpecsGroup.h"
#include "em5/EM5Helper.h"
#include "em5/map/EntityHelper.h"

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
	const uint32 ThrowStunGrenadeTargetPointProvider::TARGET_POINT_ID = qsf::StringHash("ThrowStunGrenadeTargetPointProvider");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	ThrowStunGrenadeTargetPointProvider::ThrowStunGrenadeTargetPointProvider(qsf::logic::TargetPointManager* manager) :
		qsf::logic::TargetPointProvider(manager)
	{
		// Register callbacks
		manager->registerProvider(TARGET_POINT_ID, this);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::logic::TargetPointProvider methods ]
	//[-------------------------------------------------------]
	void ThrowStunGrenadeTargetPointProvider::calculateTargetPoints(qsf::Entity& caller, const qsf::Transform& callerTransform, qsf::Entity& target, const qsf::Transform& targetTransform, std::vector<qsf::logic::TargetPoint>& targetPoints) const
	{
		// TODO(mz):This is a dummy so far - until AchieveLineOfSightGoal is written it's hard to tell what kind of points we will need there.
		// So current solution just makes sure we run toward the target and stop some point before reaching it.
		// I guess in the end we have to give a bunch of target points here.

		glm::vec3 toTarget = targetTransform.getPosition()-callerTransform.getPosition();

#if 0
		// TODO(mz): Idea was to stop moving when the target vehicle is out of range. But it seems currently empty targetPoints are not allowed.
		//			 Would be a nice feature, but not going to bring that up today (alpha-release).
		const float NO_FOLLOW_DISTANCE = ActionRangeSpecsGroup::getInstanceSafe().getStunGrenadeNoFollowDistance();
		if (glm::length(toTarget) > NO_FOLLOW_DISTANCE && EntityHelper(target).isGangsterRoadVehicle())
		{
			// give up
			targetPoints.clear();
			return;
		}
#endif

		const float THROW_DISTANCE = ActionRangeSpecsGroup::getInstanceSafe().getStunGrenadeThrowDistance();
		glm::vec3 targetPosition = targetTransform.getPosition() - glm::normalize(toTarget) * THROW_DISTANCE;

		targetPoints.emplace_back(targetPosition, true, 0.5f, 0.5f);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
