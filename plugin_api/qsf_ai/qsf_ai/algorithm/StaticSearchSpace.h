// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <boost/scoped_array.hpp>

#include <cstddef>

namespace qsf
{
	namespace ai
	{
		/** A pattern for search spaces that are completely allocated before the actual search resulting in better performance at the cost of increased peak memory consumption.
		* The whole search space remains allocated during the search.
		* The template parameter defines the atomic search state.
		* The search state needs to be default constructable to be used with this setup.
		*/
		template <typename SearchState>
		class StaticSearchSpace
		{
		public:
			// Creates the states with indices [0...numStates)
			StaticSearchSpace(std::size_t numStates);

			// Throws an exception if the index is out of bounds
			SearchState& getState(std::size_t index) const;

			// Calculates the index of a state based on the assumption that it is part of the contained states.
			// There are no sanity bound checks here because the function is expected to be often used.
			// Therefore it generates incorrect results if used with states from a different structure.
			std::size_t getIndex(const SearchState& state) const;
		private:
			boost::scoped_array<SearchState> mStates;
			std::size_t mNumStates;
		};
	}
}

#include "qsf_ai/algorithm/StaticSearchSpace-inl.h"
