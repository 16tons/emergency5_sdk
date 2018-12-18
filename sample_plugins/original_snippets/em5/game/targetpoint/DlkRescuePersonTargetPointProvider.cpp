// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/game/targetpoint/DlkRescuePersonTargetPointProvider.h"
#include "em5/game/groundmap/GroundMaps.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/logic/local/firefighters/DLKLogic.h"

#include <qsf_logic/targetpoint/TargetPointManager.h>

#include <qsf/base/ScratchBuffer.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/map/Entity.h>
#include <qsf/map/EntityHelper.h>
#include <qsf/map/query/GroundMapQuery.h>
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
	const uint32 DlkRescuePersonTargetPointProvider::TARGET_POINT_ID = qsf::StringHash("DlkRescuePersonTargetPointProvider");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	DlkRescuePersonTargetPointProvider::DlkRescuePersonTargetPointProvider(qsf::logic::TargetPointManager* manager) :
		qsf::logic::TargetPointProvider(manager)
	{
		// Register callbacks
		manager->registerProvider(TARGET_POINT_ID, this);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::logic::TargetPointProvider methods ]
	//[-------------------------------------------------------]
	void DlkRescuePersonTargetPointProvider::calculateTargetPoints(qsf::Entity& caller, const qsf::Transform& callerTransform, qsf::Entity& target, const qsf::Transform& targetTransform, std::vector<qsf::logic::TargetPoint>& targetPoints) const
	{
		glm::vec3 targetPosition = targetTransform.getPosition();
		float range = 10.0f;

		// Check if we have a building as target or the target is inside a building
		BuildingComponent* buildingComponent = DLKLogic::getPlaceOfActionBuilding(target);
		if (nullptr != buildingComponent)
		{
			// Is there someone to rescue
			qsf::Entity* personEntity = buildingComponent->getPersonWavingOutOfWindow();
			if (nullptr != personEntity)
			{
				targetPosition = personEntity->getOrCreateComponentSafe<qsf::TransformComponent>().getPosition();

				// Go 3 meter away from the building and decrease the range
				targetPosition += glm::normalize(targetPosition - targetTransform.getPosition()) * 3.0f;
				range = 7.0f;
			}
		}
		else
		{
			// Search for a pre-edited target point
			// TODO(fw): I added this only for ME305, and this could definitely be improved
			qsf::EntityVectorScratchBuffer children;
			if (qsf::LinkComponent::getDirectChildren(target, 0xff, children))
			{
				for (qsf::Entity* child : children)
				{
					qsf::EntityHelper entityHelper(*child);
					if (entityHelper.getName() == "dlk_target_point")
					{
						targetPosition = entityHelper.getPosition();
						range = 0.0f;
						break;
					}
				}
			}
		}

		// Height needed for target
		float height = 0.0f;
		qsf::GroundMapQuery groundMapQuery(caller.getMap(), GroundMaps::FILTER_DEFAULT);
		if (groundMapQuery.getHeightAtXZPosition(targetPosition.x, targetPosition.z, height))
		{
			targetPosition.y = height;
		}

		targetPoints.emplace_back(targetPosition, false, range, 1.0f);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
