// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/base/Math.h"

#include <qsf/math/CoordinateSystem.h>

namespace qsf
{
	namespace ai
	{
		namespace steering
		{
			inline SteeringControlPoint::SteeringControlPoint(Time reached, UnsignedFloat desiredSpeed, const glm::vec3& wpPos, const glm::vec3& wpDirection, UnsignedFloat distance, UnsignedFloat maxSegmentSpeed, Reaction reaction,
				bool isMovingForward, float accelerationUsed, const detail::ReservationContainerEntry& reservation, const boost::optional<TurningConfiguration>& turningConfig) :
				mTimeToReach(reached),
				mDesiredSpeed(desiredSpeed),
				mAccelerationUsed(accelerationUsed),
				mPosition(wpPos),
				mDirection(wpDirection),
				mDistance(distance),
				mMaxSegmentSpeed(maxSegmentSpeed),
				mReaction(reaction),
				mIsMovingForward(isMovingForward),
				mReservation(reservation),
				mTurningConfiguration(turningConfig),
				mBrakingReason(NO_STOP)
			{
				setPositionSafe(wpPos);
			}

			inline void SteeringControlPoint::setPositionSafe(const glm::vec3& vec)
			{
				// made asserts of these due to performance impact
				QSF_ASSERT(vec == vec, "trying to set waypoint position to NaN", QSF_REACT_THROW);
				QSF_ASSERT(glm::length(vec) < std::numeric_limits<float>::infinity(), "trying to set waypoint position to inf", QSF_REACT_THROW);

				mPosition = vec;
			}
		}
	}
}
