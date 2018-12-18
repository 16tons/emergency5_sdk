// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf_logic/targetpoint/TargetPoint.h>

#include <qsf/serialization/binary/BinarySerializer.h>
#include <qsf/serialization/binary/BasicTypeSerialization.h>


namespace qsf
{
	namespace ai
	{
		/**
		* A navigation target point is a target point for a path search together with state information about the effort to reach it.
		* Simple helper class that is copyable via the default mechanisms.
		*/
		class NavigationTargetPoint
		{
		public:
			NavigationTargetPoint(); // necessary for creating these inside STL containers
			// not explicit by design, should be easy to create from a logic::targetPoint
			NavigationTargetPoint(const logic::TargetPoint& targetPoint);

			logic::TargetPoint mTargetPoint;
			bool mFailed; // This flag starts at true and is set to false in case we have given up to reach it
		};
	}

	namespace serialization
	{
		template <>
		struct serializer<ai::NavigationTargetPoint>
		{
			inline static void serialize(BinarySerializer& serializer, ai::NavigationTargetPoint& data)
			{
				serializer & data.mTargetPoint;
				serializer & data.mFailed;
			}
		};
	}
}
