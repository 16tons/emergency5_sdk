// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/game/targetpoint/FollowMagnetTargetPointProvider.h"
#include "em5/component/movement/PersonMagnetComponent.h"

#include <qsf_logic/targetpoint/TargetPointManager.h>

#include <qsf/map/Entity.h>
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
	const uint32 FollowMagnetTargetPointProvider::TARGET_POINT_ID = qsf::StringHash("FollowMagnetTargetPointProvider");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	FollowMagnetTargetPointProvider::FollowMagnetTargetPointProvider(qsf::logic::TargetPointManager* manager) :
		qsf::logic::TargetPointProvider(manager)
	{
		// Register callbacks
		manager->registerProvider(TARGET_POINT_ID, this);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::logic::TargetPointProvider methods ]
	//[-------------------------------------------------------]
	void FollowMagnetTargetPointProvider::calculateTargetPoints(qsf::Entity& caller, const qsf::Transform& callerTransform, qsf::Entity& target, const qsf::Transform& targetTransform, std::vector<qsf::logic::TargetPoint>& targetPoints) const
	{
		glm::vec3 targetPos = targetTransform.getPosition();

		PersonMagnetComponent* personMagnetComponent = target.getComponent<PersonMagnetComponent>();
		if (nullptr != personMagnetComponent)
		{
			// Deliberately no rotation transformation for the offset. Persons should _not_ walk to the other side when the squad view direction changes..
			targetPos += personMagnetComponent->getFollowOffset(caller.getId(), true, false);
		}

		targetPoints.emplace_back(targetPos, true, 0.f, 0.5f);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
