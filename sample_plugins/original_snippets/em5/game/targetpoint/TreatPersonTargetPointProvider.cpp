// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/game/targetpoint/TreatPersonTargetPointProvider.h"
#include "em5/health/HealthComponent.h"
#include "em5/health/HealthHelper.h"
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
	const uint32 TreatPersonTargetPointProvider::TARGET_POINT_ID = qsf::StringHash("TreatPersonTargetPointProvider");

	const float TreatPersonTargetPointProvider::TargetPersonXOffset = 0.95f;
	const float TreatPersonTargetPointProvider::TargetPersonZOffsetMale = -1.248f;
	const float TreatPersonTargetPointProvider::TargetPersonZOffsetFemale = -1.139f;

	const float TreatPersonTargetPointProvider::TargetPersonXOffsetStanding = 0.806f;
	const float TreatPersonTargetPointProvider::TargetPersonZOffsetStanding = -0.0f;


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	void TreatPersonTargetPointProvider::calculateTargetPoints_static(qsf::Entity& target, const qsf::Transform& targetTransform, std::vector<qsf::logic::TargetPoint>& targetPoints, bool originalSide)
	{
		const glm::vec3& targetPosition = targetTransform.getPosition();
		const glm::quat& targetRotation = targetTransform.getRotation();

		// Get an 2D (x,z) rotation
		const glm::quat rotationInXZPlane = qsf::Math::getRotationByUpVector(qsf::CoordinateSystem::getUp(), targetRotation);

		// Detect situation, patient is lying on ground (kneeing switches to lying) or standing
		float targetPersonXOffset, targetPersonZOffset;
		calculateOffsets(target, targetPersonXOffset, targetPersonZOffset);

		// The offset of the target to the right
		glm::vec3 localPlaceOffset;
		localPlaceOffset.x = originalSide ? targetPersonXOffset : -targetPersonXOffset;
		localPlaceOffset.z = -targetPersonZOffset;

		const glm::vec3 rightPosition = targetPosition + rotationInXZPlane * localPlaceOffset;

		targetPoints.emplace_back(rightPosition, true, 0.0f, 0.0f);
	}

	glm::quat TreatPersonTargetPointProvider::calculateRotation_static(const qsf::Transform& targetTransform, bool originalSide)
	{
		// Let's keep things simple, and use a fixed relative rotation (the former solution here was complicated and also buggy)
		const float differenceAngle = originalSide ? 90.0f : -90.0f;
		return targetTransform.getRotation() * qsf::Math::getRotationAroundAxis(qsf::CoordinateSystem::getUp(), glm::radians(differenceAngle));
	}

	void TreatPersonTargetPointProvider::calculateOffsets(qsf::Entity& target, float& xOffset, float& zOffset)
	{
		// Get person's gender
		const PersonComponent* personComponent = target.getComponent<PersonComponent>();
		const PersonComponent::Gender gender = (nullptr != personComponent) ? personComponent->getGender() : PersonComponent::GENDER_MALE;

		xOffset = TargetPersonXOffset;
		zOffset = (gender == PersonComponent::GENDER_FEMALE) ? TargetPersonZOffsetFemale : TargetPersonZOffsetMale;		// TODO(fw): We need more differenciation

		HealthComponent* healthComponent = target.getComponent<HealthComponent>();
		if (nullptr != healthComponent)
		{
			if (healthComponent->isHealthy())
			{
				xOffset = TargetPersonXOffsetStanding;
				zOffset = TargetPersonZOffsetStanding;
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	TreatPersonTargetPointProvider::TreatPersonTargetPointProvider(qsf::logic::TargetPointManager* manager) :
		qsf::logic::TargetPointProvider(manager)
	{
		// Register callbacks
		manager->registerProvider(TARGET_POINT_ID, this);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::logic::TargetPointProvider methods ]
	//[-------------------------------------------------------]
	void TreatPersonTargetPointProvider::calculateTargetPoints(qsf::Entity& caller, const qsf::Transform& callerTransform, qsf::Entity& target, const qsf::Transform& targetTransform, std::vector<qsf::logic::TargetPoint>& targetPoints) const
	{
		calculateTargetPoints_static(target, targetTransform, targetPoints, true);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
