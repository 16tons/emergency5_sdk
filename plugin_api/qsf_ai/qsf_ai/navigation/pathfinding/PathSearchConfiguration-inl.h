// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/navigation/goal/NavigationGoalFactory.h"

#include <qsf/serialization/binary/BasicTypeSerialization.h>


namespace qsf
{
	namespace ai
	{
		inline PathSearchConfiguration::PathSearchConfiguration(const glm::vec3& startPos, const glm::quat& startOrientation, const NavigationGoal& goal, unsigned int moverType, unsigned int primaryMapId) :
		mMoverType(moverType),
		mPrimaryMapId(primaryMapId),
		mSecondaryMapId(getUninitialized(mSecondaryMapId)),
		mSearchingEntityId(getUninitialized(mSearchingEntityId)),
		mStartPosition(startPos),
		mStartOrientation(startOrientation),
		mGoal(goal.clone()),
		mBackwardsCostMultiplier(getUninitialized(mBackwardsCostMultiplier)),
		mLastKnownMapElementId(getUninitialized(mLastKnownMapElementId)),
		mMovementOptions(std::numeric_limits<short>::max()),
		mDebugPortals(false)
		{}

		inline PathSearchConfiguration::PathSearchConfiguration() :
		mMoverType(0),
		mPrimaryMapId(0),
		mSecondaryMapId(getUninitialized(mSecondaryMapId)),
		mSearchingEntityId(getUninitialized(mSearchingEntityId)),
		mLastKnownMapElementId(getUninitialized(mLastKnownMapElementId)),
		mMovementOptions(0),
		mDebugPortals(false)
		{}
	}

	// declaration of the serialization specialization
	namespace serialization
	{
		template <>
		struct serializer<ai::PathSearchConfiguration>
		{
			static void serialize(BinarySerializer& serializer, ai::PathSearchConfiguration& configuration);
		};
	}
}
