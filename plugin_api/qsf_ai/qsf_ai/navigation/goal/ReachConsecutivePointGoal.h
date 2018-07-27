// Copyright (C) 2012-2017 Promotion Software GmbH


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
		class QSF_AI_API_EXPORT ReachConsecutivePointGoal : public NavigationGoal
		{
		public:
			friend class NavigationGoalFactory; // for using the private default constructor

			static const float SQUARED_DISTANCE_FOR_RECALCULATION;

			// Provide all the target data combined into a target point.
			explicit ReachConsecutivePointGoal(const std::vector<logic::TargetPoint>& targetConfig);

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
			bool changeToNextGoal();	// change to the next consecutive goal or give back false

			ReachConsecutivePointGoal(); // hidden so it can be only used by friends

			std::vector<logic::TargetPoint> mConsecutiveGoals;
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf
