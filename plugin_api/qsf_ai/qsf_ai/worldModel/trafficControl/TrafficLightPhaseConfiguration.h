// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/worldModel/WorldElementState.h"

#include <qsf/time/Time.h>

#include <boost/container/flat_map.hpp>


namespace qsf
{
	namespace ai
	{
		/**
		* A traffic light phase configuration defines which signals are shown for what types of traffic lights and how long this state lasts.
		* It is the atomic entry inside a traffic light configuration schedule that defines the behavior over time of all managed traffic lights.
		*
		* These are objects are copyable via the default mechanisms because they need to be managed in STL containers with linear memory.
		* A default created instance has a zero duration and an empty mapping.
		*/
		class TrafficLightPhaseConfiguration
		{
		public:
			// Typedef for the actual mapping contained.
			typedef boost::container::flat_map<unsigned int, worldElement::StateType> StatesMap;

			// Phase duration access
			//@{
			const Time& getDuration() const;
			void setDuration(const Time& duration);
			//@}

			/**
			* Adding or overwriting an entry in the mapping.
			* It is currently not allow to remove an entry.
			* This lets all traffic lights component with the given group id that are managed using a schedule containing this phase to the state passed as parameter.
			*/
			void addEntryToMapping(unsigned int groupId, worldElement::StateType state);
			// Returns a structure mapping traffic light group ids to a state that their managed world node will receive
			const StatesMap& getStatesMapping() const;

		private:
			Time mDuration;
			StatesMap mStates;
		};


		// inline implementation
		inline const Time& TrafficLightPhaseConfiguration::getDuration() const
		{
			return mDuration;
		}

		inline void TrafficLightPhaseConfiguration::setDuration(const Time& duration)
		{
			mDuration = duration;
		}

		inline void TrafficLightPhaseConfiguration::addEntryToMapping(unsigned int groupId, worldElement::StateType state)
		{
			mStates[groupId] = state;
		}

		inline const TrafficLightPhaseConfiguration::StatesMap& TrafficLightPhaseConfiguration::getStatesMapping() const
		{
			return mStates;
		}
	}
}
