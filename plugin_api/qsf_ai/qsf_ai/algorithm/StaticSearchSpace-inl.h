// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/error/ErrorHandling.h>

namespace qsf
{
	namespace ai
	{
		template <typename SearchState>
		StaticSearchSpace<SearchState>::StaticSearchSpace(std::size_t numStates) :
			mStates(new SearchState[numStates]),
			mNumStates(numStates)
		{}

		template <typename SearchState>
		SearchState& StaticSearchSpace<SearchState>::getState(std::size_t index) const
		{
			QSF_CHECK(index < mNumStates, "index " << index << " out of bounds when accessing search state",
				QSF_REACT_THROW);

			return mStates[index];
		}

		template <typename SearchState>
		std::size_t StaticSearchSpace<SearchState>::getIndex(const SearchState& state) const
		{
			return static_cast<std::size_t>(&state - &mStates[0]);
		}
	}
}
