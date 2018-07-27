// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/error/ErrorHandling.h>
#include <qsf/math/GlmHelper.h>

namespace qsf
{
	namespace ai
	{
		template <typename Vector>
		UnsignedFloat calculateRadiusOfCircle(const Vector& tangentPosition, const Vector& tangentDirection, const Vector& secondTangentPosition)
		{
			QSF_CHECK(!isZero(tangentDirection), "Unable to use zero vector as tangent direction",
				QSF_REACT_THROW);

			const Vector towardsSecondPosition = secondTangentPosition - tangentPosition;
			if (isZero(towardsSecondPosition))
				return UnsignedFloat::ZERO;

			const float cosOfAngle = dot(tangentDirection, towardsSecondPosition) / (glm::length(tangentDirection) * glm::length(towardsSecondPosition));
			const float sinOfAngle = std::sqrt(1.f - (cosOfAngle * cosOfAngle));

			return glm::length(secondTangentPosition - tangentPosition) *.5f / sinOfAngle;
		}

		inline SimpleTangentConnectionResult::SimpleTangentConnectionResult(const glm::vec2& leaveCircleAt, const glm::vec2& tangentDirection, UnsignedFloat distanceOnCircle, const TurningConfiguration& turningConfig) :
		mLeaveCircleAt(leaveCircleAt),
		mTangentDirection(tangentDirection),
		mDistanceOnCircle(distanceOnCircle),
		mTurningConfiguration(turningConfig)
		{}

		inline SimpleTangentConnectionResult::SimpleTangentConnectionResult(const glm::vec2& leaveCircleAt, const glm::vec2& tangentDirection) :
		mLeaveCircleAt(leaveCircleAt),
		mTangentDirection(tangentDirection)
		{}

		inline CircleTangentConnectionResult::CircleTangentConnectionResult(
			const SimpleTangentConnectionResult& startCircleData, const SimpleTangentConnectionResult& goalCircleData, move::Quality quality, UnsignedFloat totalDistance) :
		mStartCircleData(startCircleData),
		mGoalCircleData(goalCircleData),
		mSolutionQuality(quality),
		mTotalDistance(totalDistance)
		{}
	}
}
