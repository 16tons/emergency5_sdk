// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/algorithm/WeightedState.h"
#include "qsf_ai/algorithm/Costs.h"

#include <vector>

namespace qsf
{
	namespace ai
	{
		/** This defines the interface for a search mechanics to be used with the A* search template.
		* A search mechanics provides the details of how to expand nodes during a search and contains the cost functions g and h.
		* It is only a templated interface declaration to be used as a reference without an implementation available.
		*/
		template <typename SearchState>
		class SearchMechanics
		{
		public:
			// Convert an index into a state and vice versa.
			// This may actually create the state on the fly for dynamically allocated search spaces.
			SearchState& getState(std::size_t index);
			// This may simply read the index from the state.
			std::size_t getIndex(const SearchState& state);

			// The search states needs to have a typedef for the costs that are used to describe it.
			typename SearchState::Cost estimateCostsToGoal(const SearchState& state) const;
			bool isGoal(const SearchState& state) const;

			// Expand the current state by creating follow up states in the reference parameter.
			// The followUpStates need to be weighted by the costs to reach them (g function), not the total costs (f function)
			void expand(SearchState& current, std::vector<WeightedState<SearchState> >& followUpStates);
		};
	}
}
