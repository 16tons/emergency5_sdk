// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/algorithm/WeightedState.h"
#include "qsf_ai/algorithm/SearchMechanics.h"

#include <queue>
#include <vector>


namespace qsf
{
	namespace ai
	{
		/** Templated A* implementation that gets the details for search state expansion and heuristics from the template parameter instance passed during construction.
		* It supports adding start states at any time as a nonstandard extension.
		* A typical usecase is searching with prioritized starts states.
		* The only tricky part is when this feature is used a situation may arise that is normally a sign of an algorithmic error:
		* We can find a better path to an already expanded node.
		* This is ignored in all cases but the warning about a potential error is suppressed when a late start is detected.
		* It also finds several solutions in succession since additional calls to search after one solution is found just keeps exploring the search space and potentially finding further solutions.
		*/
		template <typename SearchMechanics>
		class AStar
		{
		public:
			typedef typename SearchMechanics::SearchState SearchState;
			typedef WeightedState<SearchState> WeightedSearchState;
			typedef std::priority_queue<WeightedSearchState, std::deque<WeightedSearchState>, SearchStateCostMore<SearchState> > PriorityQueue;

			AStar(SearchMechanics& mechanics);

			bool isSuccess() const;
			bool isFailed() const;
			void failSearch();

			// Returns true if there is no search state on the open list
			bool isOpenListEmpty() const;
			const WeightedSearchState* tryPeakOpenList() const; // returns the top of the open list if it is not empty otherwise nullptr

			// Can be called at any time adding an alternative start even during a search.
			// Returns whether the start actually opened up new options and is scheduled for expansion or the node was already expanded before and the new start option therefore ignored.
			bool addStart(std::size_t newStartIndex, typename SearchState::Cost costsToStart);

			// executes one search step by expanding the node on top of the open list
			// does nothing if the open list is empty
			void searchStep();
			// searches until the search is failed or succeeded
			void execute();

			// Returns the found goal state and throws an exception if no goal was found yet.
			const SearchState& getGoalFound() const;

			// Sets the goal found to null.
			// This is only needed during the nonstandard extensions that trigger several searches in parallel and need to find goals iteratively.
			void clearGoalFound();

			/** Writes the path leading up to a state as a chain of connected search states from the closest start state.
			* The typical usecase is to call this with the result of getGoalFound() after a successful search.
			* The result is written to the path reference parameter.
			* It always has a pointer to the goal state as last entry and starts with a start state.
			*/
			void writePathFound(const SearchState& goal, std::vector<const SearchState*>& path) const;

		private:
			// noncopyable
			AStar(const AStar<SearchMechanics>&);
			AStar& operator=(const AStar<SearchMechanics>&);

			SearchMechanics& mSearchMechanics;

			PriorityQueue mOpenList;

			SearchState* mGoalFound;

			// Helper flag tracking whether the expansion phase has already started to determine the more important uses late starts flag below.
			bool mHadSearchStep;

			// Usually all start states are added in one go before the expansion phase starts.
			// If the special feature is used that we may add start states at a later point during the search this flag is set to true and used to suppress a warning.
			bool mUsesLateStarts;
		};
	}
}

#include "qsf_ai/algorithm/AStar-inl.h"
