// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


#ifdef QSF_PROFILING

	//[-------------------------------------------------------]
	//[ Includes                                              ]
	//[-------------------------------------------------------]
	#include "qsf/time/Time.h"


	//[-------------------------------------------------------]
	//[ Namespace                                             ]
	//[-------------------------------------------------------]
	namespace qsf
	{

		/**
		* @brief ProfilingData encapsulates the data that is collected during each logic system's activities to create a profile.
		* It currently contains only hardcoded data but may be extended by custom values for specific systems later on.
		* Lightweight Object that can be copied via the default mechanisms.
		*
		* Profiled activities are currently
		* - main system update
		* - successful update of a component managed by this system
		* - relevant successful managed component update which is an update that needed actual calculations and was not just an early out
		* - failed updates
		*/
		class QSF_API_EXPORT ProfilingData
		{
		public:
			ProfilingData();

			// Register finished individual calls to a profiled activity.
			//@{
			void registerSystemUpdate(Time duration);
			// See main description for the notion of a relevant component update
			void registerComponentUpdate(Time duration, bool wasRelevant);
			void registerFailedComponentUpdate();
			//@}

			// Evaluate the current state and write all data to the logging system.
			void logState() const;
		private:
			// Event counters for profiled activity types.
			//@{
			uint64 mNumSystemUpdates; // The total number of calls to the main update function of the profiled system.
			uint64 mNumSucceededComponentUpdates; // The total number of calls to the update function of a component managed by the profiled system that did not indicate an error.
			uint64 mNumSucceededRelevantComponentUpdates; // The total number of calls to the update function of a component that needed actual calculations and were not just early outs or errors.
			uint64 mNumFailedComponentUpdates; // The total number of component updates that failed by throwing an exception or indicating an error.
			//@}

			// Summed times spent during profiled activities.
			//@{
			Time mSystemUpdateTotal;
			Time mComponentUpdateTotal;
			Time mRelevantComponentUpdateTotal;
			//@}

			// Peak values for each profiled activity
			//@{
			Time mSystemUpdatePeak;
			Time mComponentUpdatePeak;
			Time mRelevantComponentUpdatePeak;
			//@}
		};


		// inline implementation
		inline ProfilingData::ProfilingData() :
			mNumSystemUpdates(0),
			mNumSucceededComponentUpdates(0),
			mNumSucceededRelevantComponentUpdates(0),
			mNumFailedComponentUpdates(0)
		{}

		inline void ProfilingData::registerSystemUpdate(Time duration)
		{
			mNumSystemUpdates++;
			mSystemUpdateTotal += duration;
			if (duration > mSystemUpdatePeak)
				mSystemUpdatePeak = duration;
		}

		inline void ProfilingData::registerComponentUpdate(Time duration, bool wasRelevant)
		{
			mNumSucceededComponentUpdates++;
			mComponentUpdateTotal += duration;
			if (duration > mComponentUpdatePeak)
				mComponentUpdatePeak = duration;

			if (wasRelevant)
			{
				mNumSucceededRelevantComponentUpdates++;
				mRelevantComponentUpdateTotal += duration;
				if (duration > mRelevantComponentUpdatePeak)
					mRelevantComponentUpdatePeak = duration;
			}
		}

		inline void ProfilingData::registerFailedComponentUpdate()
		{
			mNumFailedComponentUpdates++;
		}


	//[-------------------------------------------------------]
	//[ Namespace                                             ]
	//[-------------------------------------------------------]
	} // qsf

#endif
