// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/game/targetpoint/AttackPersonTargetPointProvider.h"
#include "em5/component/person/PersonComponent.h"

#include <qsf_logic/targetpoint/TargetPointManager.h>

#include <qsf/math/Math.h>
#include <qsf/math/CoordinateSystem.h>
#include <qsf/map/Entity.h>



//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 AttackPersonTargetPointProvider::TARGET_POINT_ID = qsf::StringHash("AttackPersonTargetPointProvider");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	AttackPersonTargetPointProvider::AttackPersonTargetPointProvider(qsf::logic::TargetPointManager* manager) :
		qsf::logic::TargetPointProvider(manager)
	{
		// Register callbacks
		manager->registerProvider(TARGET_POINT_ID, this);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::logic::TargetPointProvider methods ]
	//[-------------------------------------------------------]
	void AttackPersonTargetPointProvider::calculateTargetPoints(qsf::Entity& caller, const qsf::Transform& callerTransform, qsf::Entity& target, const qsf::Transform& targetTransform, std::vector<qsf::logic::TargetPoint>& targetPoints) const
	{
		glm::vec3 callerPosition = callerTransform.getPosition();
		glm::vec3 targetPosition = targetTransform.getPosition();

		// Calculate difference
		glm::vec3 difference = targetPosition - callerPosition;
		difference.y = 0.0f;

		if (glm::length2(difference) <= 0.001f)
		{
			// Just to avoid warning due to zero vector as direction
			difference = glm::vec3(1, 0, 0);
		}

		float distance = 0.3f;

		// TODO(tl): This is a momentary hack to prevent a bug where the dog will be stuck with a failed em3 path search
		// Dogs should have a higher distance from targets
		PersonComponent* personComponent = caller.getComponent<PersonComponent>();
		if (personComponent != nullptr)
		{
			if (personComponent->getGender() == PersonComponent::Gender::GENDER_ANIMAL)
			{
				distance = 1.5f;
			}
		}

		glm::quat orientation = qsf::Math::getLookAtQuaternion(difference, qsf::CoordinateSystem::getUp());
		targetPoints.emplace_back(targetPosition, true, distance, 0.5f, orientation);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
