// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/base/UnsignedFloat.h"
#include "qsf_ai/worldModel/WorldElementState.h"

#include <qsf/base/GetUninitialized.h>

#include <boost/optional.hpp>

namespace qsf
{
	namespace ai
	{
		/** A movement mode describes the circumstances of an entity moving for some period of time, usually an executed move to action.
		* It modifies some of the navigation aspects of the entity.
		* The current modes can't be used during planning but are set by the move to action initially.
		*/
		class MovementMode
		{
		public:
			MovementMode(unsigned int id = getUninitialized<unsigned int>());

			unsigned int mId; // Needs to be a unique id among all used movement modes

			// All these values act as temporal optional modifiers or overrides for the values inside the navigation component.
			// See the variables with the same name inside NavigationComponent for what each variable means.
			// A mode that doesn't change anything is possible this way and legal but doesn't make sense.
			//@{
			boost::optional<unsigned int> mPrimaryNavMap;
			boost::optional<unsigned int> mSecondaryNavMap;
			boost::optional<short> mCollisionAvoidanceFlags;
			boost::optional<short> mCollisionAvoidanceMeasures;
			boost::optional<UnsignedFloat> mSpeedFactor; // setting a factor as well as an absolute overwrite doesn't make sense
			boost::optional<UnsignedFloat> mSpeedOverwrite; // setting a factor as well as an absolute overwrite doesn't make sense
			boost::optional<UnsignedFloat> mDistanceToKeepAhead;
			boost::optional<bool> mMayMoveForward;
			boost::optional<bool> mMayMoveBackward;
			boost::optional<bool> mMayManeuver;
			boost::optional<bool> mMayReduceTurningRadius;
			boost::optional<bool> mSyncVelocityToDirection;
			boost::optional<bool> mCorrectGoalToIdealPosition;
			//@}

			// Allows to disable stuck detection during a move action. If set to false, stuck detection is disabled.
			 // Otherwise it depends on the stuck resolver component being there
			bool mMayUseStuckResolving;

			// This allows to ignore node states that are equal or less severe than the one defined in here.
			// See the ordering of the StateType and enum. Physical restrictions are also to be considered more severe than non physical restrictions.
			boost::optional<worldElement::State> mIgnoreWorldStatesUpTo;
			// Temporary expert option to overwrite the path smoothing method used.
			// This is normally tied to the nav map used but may be overridden here for very special cases.
			// Consult VS before using it.
			// It is a three value logic with uninitialized meaning don't force a specific smoothing at all but use the default one defined with the map.
			// A true value means the entity should use funnel smoothing whereas a false value means the entity should use the curved ideal lane following.
			boost::optional<bool> mForceUseFunnelSmoothing;
		};
	}
}

#include "qsf_ai/navigation/MovementMode-inl.h"
