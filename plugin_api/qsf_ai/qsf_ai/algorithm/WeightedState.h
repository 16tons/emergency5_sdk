// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/base/UnsignedFloat.h"

namespace qsf
{
	namespace ai
	{
		// A search state weighted with a cost
		// Default copy C'tor and assignment allowed by design.
		template <typename SearchState>
		struct WeightedState
		{
			WeightedState(SearchState* state, const typename SearchState::Cost& costs) :
				mState(state),
				mCosts(costs)
			{}

			SearchState* mState;
			typename SearchState::Cost mCosts;
		};

		// Predicate to create an ordering among weighted states ascending with costs
		template <typename SearchState>
		struct SearchStateCostMore
		{
			bool operator ()(const WeightedState<SearchState>& lhs, const WeightedState<SearchState>& rhs) const
			{
				return lhs.mCosts > rhs.mCosts;
			}
		};
	}
}
