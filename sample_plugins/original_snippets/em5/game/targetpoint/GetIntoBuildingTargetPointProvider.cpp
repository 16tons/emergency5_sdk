// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/game/targetpoint/GetIntoBuildingTargetPointProvider.h"
#include "em5/component/building/BuildingComponent.h"

#include <qsf_logic/targetpoint/TargetPointManager.h>

#include <qsf/map/Entity.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 GetIntoBuildingTargetPointProvider::TARGET_POINT_ID = qsf::StringHash("GetIntoBuildingTargetPointProvider");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GetIntoBuildingTargetPointProvider::GetIntoBuildingTargetPointProvider(qsf::logic::TargetPointManager* manager) :
		qsf::logic::TargetPointProvider(manager)
	{
		// Register callbacks
		manager->registerProvider(TARGET_POINT_ID, this);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::logic::TargetPointProvider methods ]
	//[-------------------------------------------------------]
	void GetIntoBuildingTargetPointProvider::calculateTargetPoints(qsf::Entity& caller, const qsf::Transform& callerTransform, qsf::Entity& target, const qsf::Transform& targetTransform, std::vector<qsf::logic::TargetPoint>& targetPoints) const
	{
		// A building should never move, ignore the hypothetic target transform and assume its always the current value
		BuildingComponent* buildingComponent = target.getComponent<BuildingComponent>();
		if (nullptr != buildingComponent)
		{
			const glm::vec3 buildingDoor = buildingComponent->getDoorPosition();
			targetPoints.emplace_back(buildingDoor, false, 0.0f, 0.1f);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
