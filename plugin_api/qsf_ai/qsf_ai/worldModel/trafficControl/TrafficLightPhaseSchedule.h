// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/worldModel/trafficControl/TrafficLightPhaseConfiguration.h"

#include <vector>
#include <string>

namespace qsf
{
	namespace ai
	{
		/**
		* A traffic light phase schedule is a cyclic schedule that defines which type of traffic lights are in what state at each point in time.
		* It is used by a TrafficLightTimerComponent to orchestrate the signals for the managed traffic lights.
		*/
		class QSF_AI_API_EXPORT TrafficLightPhaseSchedule
		{
		public:
			friend class TrafficLightPhaseScheduleJsonSerializer;

			// Pushes the phase to the back of the schedule
			void pushBackPhase(const TrafficLightPhaseConfiguration& config);

			// Returns the current phase assuming the schedule started at Time::ZERO.
			// You need to pass the total time passed since the program started.
			// TODO(vs)  We could think about creating a somewhat more efficient version using deltas but this needs additional structures.
			// I didn't want to incorporate this timing information into the schedule.
			// Returns nullptr if the schedule is empty or an error occurred.
			const TrafficLightPhaseConfiguration* tryGetCurrentPhase(const Time& totalTime) const;

			bool isEmpty() const;
			void clear();

		private:
			std::vector<TrafficLightPhaseConfiguration> mPhases;
			Time mTotalScheduleTime; // Just a sum of the single phase durations to avoid recalculating this in each getter access
		};


		// inline implementation
		inline void TrafficLightPhaseSchedule::pushBackPhase(const TrafficLightPhaseConfiguration& config)
		{
			mPhases.push_back(config);
			mTotalScheduleTime += config.getDuration();
		}

		inline bool TrafficLightPhaseSchedule::isEmpty() const
		{
			return mPhases.empty();
		}

		inline void TrafficLightPhaseSchedule::clear()
		{
			mPhases.clear();
			mTotalScheduleTime = Time::ZERO;
		}
	}
}
