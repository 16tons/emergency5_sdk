// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/navigation/goal/NavigationGoal.h"

#include <qsf/serialization/binary/BasicTypeSerialization.h>

#include <memory>

namespace qsf
{
	namespace ai
	{
		// Dummy class providing only one static member to create navigation goals from a binary stream
		class NavigationGoalFactory
		{
		public:
			// Main method creating the goals from a binary stream
			static std::auto_ptr<NavigationGoal> createGoalInstance(BinarySerializer& serializer);
		};
	}
}
