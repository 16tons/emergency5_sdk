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
		* Navigation goal to reach an object.
		* It receives the object ID and a target point that defines where to actually approach the object.
		*/
		class QSF_AI_API_EXPORT ReachObjectGoal : public NavigationGoal
		{
		public:
			friend class NavigationGoalFactory; // for using the private default constructor

			ReachObjectGoal(const Entity& caller, const Entity& target, uint32 targetPoint, bool checkTargetPositionForLocalRouter = true);

			// navigation goal interface implementation
			//@{
			virtual process::State evaluateState(const Entity& entity) override;
			virtual bool checkForChangedGoalSituation(const Entity& entity) override;
			virtual bool isTargetMovingCloser() const override;
			virtual UnsignedFloat estimateDistanceToTarget(const glm::vec3& fromPos) const override;
			virtual std::auto_ptr<NavigationGoal> clone() const override;
			virtual bool shouldCheckTargetPositionForLocalRouter() const { return mShouldCheckTargetPositionForLocalRouter; }
			//@}

			// Give back navigation target for EPerson
			uint64 getTargetEntityId();

		protected:
			virtual void serializeSpecificData(BinarySerializer& serializer) override;

		private:
			ReachObjectGoal(); // hidden so it can be only used by friends

			DynamicTargetTracker mTargetEvaluator;
			uint32 mTargetPointId;
			bool mShouldCheckTargetPositionForLocalRouter;
		};
	}
}
