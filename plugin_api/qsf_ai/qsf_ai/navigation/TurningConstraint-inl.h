// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/serialization/binary/BasicTypeSerialization.h>


namespace qsf
{
	namespace ai
	{
		inline TurningConstraint::TurningConstraint() :
			mMovementFlags(0)
		{}

		inline TurningConstraint::TurningConstraint(UnsignedFloat radius, Percentage curveSmoothing, UnsignedFloat lateralFreeSpace, UnsignedFloat forwardFreeSpace, UnsignedFloat maxFreeSpace) :
			mTurningRadiusRequired(radius),
			mLateralFreeSpaceRequired(lateralFreeSpace),
			mForwardFreeSpaceRequired(forwardFreeSpace),
			mMaxFreeSpaceRequired(maxFreeSpace),
			mCurveSmoothingFactor(curveSmoothing),
			mMovementFlags(0)
		{}

		inline bool TurningConstraint::operator ==(const TurningConstraint& other) const
		{
			return mTurningRadiusRequired == other.mTurningRadiusRequired &&
				mLateralFreeSpaceRequired == other.mLateralFreeSpaceRequired &&
				mForwardFreeSpaceRequired == other.mForwardFreeSpaceRequired &&
				mMaxFreeSpaceRequired == other.mMaxFreeSpaceRequired &&
				mCurveSmoothingFactor == other.mCurveSmoothingFactor &&
				mMovementFlags == other.mMovementFlags;
		}
	}

	namespace serialization
	{
		template <>
		struct serializer<ai::TurningConstraint>
		{
			inline static void serialize(BinarySerializer& serializer, ai::TurningConstraint& constraint)
			{
				serializer & constraint.mTurningRadiusRequired;
				serializer & constraint.mLateralFreeSpaceRequired;
				serializer & constraint.mForwardFreeSpaceRequired;
				serializer & constraint.mMaxFreeSpaceRequired;
				serializer & constraint.mCurveSmoothingFactor;
				serializer & constraint.mMovementFlags;
			}
		};
	}
}
