// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/base/UnsignedFloat.h"
#include "qsf_ai/base/Percentage.h"


namespace qsf
{
	namespace ai
	{
		class NavigationComponent;

		/**
		* Minor helper struct to hold an entity configuration with regard to how the entity is constraint when it should move through curves.
		* The constraints are made up of a minimal turning radius some free space requirements in several directions.
		*/
		class TurningConstraint
		{
		public:
			TurningConstraint(); // Sets all values to zero effectively not constraining movement at all
			TurningConstraint(UnsignedFloat radius, Percentage curveSmoothing, UnsignedFloat lateralFreeSpace, UnsignedFloat forwardFreeSpace, UnsignedFloat maxFreeSpace);
			TurningConstraint(const NavigationComponent& navi);

			bool operator ==(const TurningConstraint& other) const;

			// Shortcuts to evaluate the movement options single flags that are relevant for turning
			//@{
			bool canManeuver() const;
			bool canShrinkTurningRadius() const;
			//@}


			// public data
			UnsignedFloat mTurningRadiusRequired;
			UnsignedFloat mLateralFreeSpaceRequired;
			UnsignedFloat mForwardFreeSpaceRequired;
			UnsignedFloat mMaxFreeSpaceRequired; // when turning on the spot

			// A factor that defines which turning radius to use if the entity has a window of options
			// It isn't used as long as the entity needs to reduce its turning radius.
			// But otherwise there is a window from the required turning radius (minimum) to the largest turn possible to stay inside the lane (maximum).
			// In this cases this percentage defines which of the curves is used in these cases.
			Percentage mCurveSmoothingFactor;
			short mMovementFlags; // The options to be used during movement, see NavigationComponent::MovementFlags
		};
	}
}

#include "qsf_ai/navigation/TurningConstraint-inl.h"
