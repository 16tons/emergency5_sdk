// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/serialization/binary/BasicTypeSerialization.h>
#include <qsf/serialization/binary/BoostTypeSerialization.h>

namespace qsf
{
	namespace ai
	{
		inline MovementMode::MovementMode(unsigned int id) :
			mId(id),
			mMayUseStuckResolving(true)
		{}
	}

	namespace serialization
	{
		template <>
		struct serializer<ai::MovementMode>
		{
			inline static void serialize(BinarySerializer& serializer, ai::MovementMode& mode)
			{
				serializer & mode.mId;
				serializer & mode.mPrimaryNavMap;
				serializer & mode.mSecondaryNavMap;
				serializer & mode.mCollisionAvoidanceFlags;
				serializer & mode.mCollisionAvoidanceMeasures;
				serializer & mode.mSpeedFactor;
				serializer & mode.mSpeedOverwrite;
				serializer & mode.mDistanceToKeepAhead;
				serializer & mode.mMayMoveForward;
				serializer & mode.mMayMoveBackward;
				serializer & mode.mMayManeuver;
				serializer & mode.mMayReduceTurningRadius;
				serializer & mode.mSyncVelocityToDirection;
				serializer & mode.mMayUseStuckResolving;
				serializer & mode.mIgnoreWorldStatesUpTo;
				serializer & mode.mForceUseFunnelSmoothing;
				serializer & mode.mCorrectGoalToIdealPosition;
			}
		};
	}
}
