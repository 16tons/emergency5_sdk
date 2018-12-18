// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/game/targetpoint/FollowWithHeliTargetTargetPointProvider.h"
#include "em5/specs/RoadVehicleSpecsGroup.h"
#include "em5/EM5Helper.h"

#include <qsf_logic/targetpoint/TargetPointManager.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 FollowWithHeliTargetTargetPointProvider::TARGET_POINT_ID = qsf::StringHash("FollowWithHeliTargetTargetPointProvider");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	FollowWithHeliTargetTargetPointProvider::FollowWithHeliTargetTargetPointProvider(qsf::logic::TargetPointManager* manager) :
		qsf::logic::TargetPointProvider(manager)
	{
		// Register callbacks
		manager->registerProvider(TARGET_POINT_ID, this);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::logic::TargetPointProvider methods ]
	//[-------------------------------------------------------]
	void FollowWithHeliTargetTargetPointProvider::calculateTargetPoints(qsf::Entity& caller, const qsf::Transform& callerTransform, qsf::Entity& target, const qsf::Transform& targetTransform, std::vector<qsf::logic::TargetPoint>& targetPoints) const
	{
		// Get the following distance from the specs file
		const RoadVehicleSpecsGroup& roadVehicleSpecsGroup = RoadVehicleSpecsGroup::getInstanceSafe();

		targetPoints.emplace_back(targetTransform.getPosition(), true, roadVehicleSpecsGroup.getHelicopterFollowDistance(), 1.0f);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
