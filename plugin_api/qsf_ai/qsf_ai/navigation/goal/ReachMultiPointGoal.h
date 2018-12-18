// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/navigation/goal/NavigationGoal.h"

namespace qsf
{
	namespace ai
	{
		/**
		* Navigation goal to move to one of the multiple points passed.
		*/
		class QSF_AI_API_EXPORT ReachMultiPointGoal : public NavigationGoal
		{
		public:
			friend class NavigationGoalFactory; // for using the private default constructor

			explicit ReachMultiPointGoal(const std::vector<logic::TargetPoint>& goalConfigs);

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
			ReachMultiPointGoal(); // hidden so it can be only used by friends
		};
	}
}
