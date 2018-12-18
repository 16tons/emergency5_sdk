// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/navigation/goal/NavigationGoal.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		* Navigation goal to reach a single static position.
		*/
		class QSF_AI_API_EXPORT ReachSinglePointGoal : public NavigationGoal
		{
		public:
			friend class NavigationGoalFactory; // for using the private default constructor

			// Provide all the target data combined into a target point.
			// The dynamic flag is ignored and doesn't really make sense here actually.
			explicit ReachSinglePointGoal(const logic::TargetPoint& targetConfig);

			// Navigation goal interface implementation
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
			ReachSinglePointGoal(); // hidden so it can be only used by friends
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf
