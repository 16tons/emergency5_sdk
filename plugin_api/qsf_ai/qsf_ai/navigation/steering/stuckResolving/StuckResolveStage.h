// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


namespace qsf
{
	namespace ai
	{
		namespace stuckResolve
		{
			// Current measure to use to avoid stuck situation
			enum Stage
			{
				WAIT_FOR_EVADE,
				BLOCK_LANE_AND_REPLAN,
				FAIL,
			};

			// Result of a stuck situation analysis
			// Mostly used for debugging reasons
			enum Analysis
			{
				DETECTION_DISABLED_NO_STUCK,
				NO_NAVI_NO_STUCK,
				NO_GOAL_NO_STUCK,
				NO_MOVABLE_NO_STUCK,
				MOVING_NO_STUCK,
				PLANNING_LOCAL_EVASION_NO_STUCK,
				RESERVATION_MISSED_STUCK,
				WAITING_AT_TRAFFIC_LIGHT_NO_STUCK,
				WAITING_AT_TRAFFIC_LIGHT_REGARDING_AS_STUCK,
				UNCLEAR_REASON_STUCK,
				INVALID_OBSTACLE_NO_STUCK,
				BEHIND_IMMOBILE_STUCK,
				BEHIND_MOVING_NO_STUCK,
				BEHIND_NOT_MOVING_STUCK,
				BEHIND_NO_NAVI_STUCK,
				BEHIND_NO_GOAL_STUCK,
				BEHIND_STUCK,
				BEHIND_WAITING_FOR_A_REASON_NO_STUCK,
				BEHIND_WAITING_FOR_A_REASON_REGARDING_AS_STUCK,
				CYCLIC_STUCK,
			};

			// convert to printable debug text
			const char* getDebugText(Analysis analysis);

			// helper to distinguish whether the entity is stuck in the current situation
			bool isStuck(Analysis analysis);

			// increments up to maximum
			inline Stage incrementStage(Stage current)
			{
				return current != FAIL ? static_cast<Stage>(current + 1) : FAIL;
			}
		}
	}
}
