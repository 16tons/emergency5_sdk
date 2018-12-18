// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/navigation/NavigationComponent.h"

#include <qsf/base/WeakPtr.h>
#include <qsf/math/Transform.h>
#include <qsf/serialization/binary/BinarySerializer.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/move/MovableComponent.h>


namespace qsf
{
	class Entity;

	namespace ai
	{
		/**
		* Helper class for tracking a dynamic target of a navigation goal.
		*/
		class DynamicTargetTracker
		{
		public:
			friend struct serialization::serializer<ai::DynamicTargetTracker>;

			// Minimal percentage (120%) between the distance to the real goal and the missing distance to the real goal on the path before we recalcate the path
			// So we will always have 20% of remaining path to run on it in comparison to the missing distance.
			static const float DISTANCE_QUOTIENT;

			// default c'tor necessary for serialization
			DynamicTargetTracker();
			DynamicTargetTracker(const Entity& caller, const Entity& target);

			void setDistanceTransformToGoal(float distance);

			// Returns whether the goal is still valid
			bool isTargetValid() const;
			// Recalculates the goal transform and returns whether it has changed
			bool updateGoalTransform(const Entity& caller);

			const Transform& getCurrentCallerTransform() const;
			const Transform& getCurrentGoalTransform() const;
			const Entity& getTargetEntity() const;

		private:
			// Tracks all relevant components for determining where to reach a moving target
			WeakPtr<NavigationComponent> mCallerNavigation;
			WeakPtr<NavigationComponent> mTargetNavigation;
			WeakPtr<TransformComponent> mCallerTransform;
			WeakPtr<TransformComponent> mTargetTransform;
			WeakPtr<MovableComponent> mTargetMovable;
			float mDistanceTransformToGoal;

			Transform mGoalTransform; // the last calculated goal transform where to reach the target
		};
	}

	namespace serialization
	{
		template <>
		struct serializer<ai::DynamicTargetTracker>
		{
			static void serialize(BinarySerializer& serializer, ai::DynamicTargetTracker& value);
		};
	}
}
