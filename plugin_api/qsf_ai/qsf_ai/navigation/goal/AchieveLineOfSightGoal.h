// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/navigation/goal/NavigationGoal.h"
#include "qsf_ai/navigation/goal/DynamicTargetTracker.h"


namespace qsf
{
	class Entity;

	namespace ai
	{
		/**
		* Navigation goal to achieve a line of sight towards a given entity and a target point.
		* The range data is read from the target point.
		*/
		class QSF_AI_API_EXPORT AchieveLineOfSightGoal : public NavigationGoal
		{
		public:
			friend class NavigationGoalFactory; // for using the private default constructor

			// The collision flags are used to determine which kind of obstacles block the line of sight.
			// See the bullet collision flags in qsf::BulletCollisionComponent for details.
			AchieveLineOfSightGoal(const Entity& caller, const Entity& target, uint32 targetPoint, short traceLineCollisionFlags);

			// navigation goal interface implementation
			//@{
			virtual process::State evaluateState(const Entity& entity) override;
			virtual bool checkForChangedGoalSituation(const Entity& entity) override;
			virtual bool isTargetMovingCloser() const override;
			virtual UnsignedFloat estimateDistanceToTarget(const glm::vec3& fromPos) const override;
			virtual std::auto_ptr<NavigationGoal> clone() const override;
			//@}
		protected:
			virtual void serializeSpecificData(BinarySerializer& serializer) override;

		private:
			AchieveLineOfSightGoal(); // hidden so it can be only used by friends

			DynamicTargetTracker mTargetEvaluator;
			uint32 mTargetPointId;
			short mTraceLineCollisionFlags;
		};
	}
}
