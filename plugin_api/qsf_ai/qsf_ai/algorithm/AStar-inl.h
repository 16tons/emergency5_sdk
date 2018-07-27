// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/algorithm/OperationIndicator.h"

#include <qsf/base/error/ErrorHandling.h>

#include <algorithm>

namespace qsf
{
	namespace ai
	{
		template <typename SearchMechanics>
		AStar<SearchMechanics>::AStar(SearchMechanics& mechanics) :
			mSearchMechanics(mechanics),
			mGoalFound(nullptr),
			mHadSearchStep(false),
			mUsesLateStarts(false)
		{}

		template <typename SearchMechanics>
		bool AStar<SearchMechanics>::isSuccess() const
		{
			return mGoalFound != nullptr;
		}

		template <typename SearchMechanics>
		bool AStar<SearchMechanics>::isFailed() const
		{
			return mGoalFound == nullptr && mOpenList.empty();
		}

		template <typename SearchMechanics>
		void AStar<SearchMechanics>::failSearch()
		{
			mGoalFound = nullptr;
			// We have to use a named instance, because you cannot bind a temporary to a non const reference.
			// The first parameter of swap is a non const reference
			PriorityQueue priorityQueue;
			mOpenList.swap(priorityQueue); // should act like a clear()
		}

		template <typename SearchMechanics>
		bool AStar<SearchMechanics>::isOpenListEmpty() const
		{
			return mOpenList.empty();
		}

		template <typename SearchMechanics>
		const WeightedState<typename SearchMechanics::SearchState>* AStar<SearchMechanics>::tryPeakOpenList() const
		{
			if (mOpenList.empty())
				return nullptr;

			return &mOpenList.top();
		}

		template <typename SearchMechanics>
		const typename SearchMechanics::SearchState& AStar<SearchMechanics>::getGoalFound() const
		{
			QSF_CHECK(mGoalFound, "Accessing nullpointer for goal state reached",
				QSF_REACT_THROW);

			return *mGoalFound;
		}

		template <typename SearchMechanics>
		void AStar<SearchMechanics>::clearGoalFound()
		{
			mGoalFound = nullptr;
		}

		template <typename SearchMechanics>
		bool AStar<SearchMechanics>::addStart(std::size_t newStartIndex, typename SearchState::Cost costsToStart)
		{
			SearchState& newStart = mSearchMechanics.getState(newStartIndex);

			// Detect usage of late start feature
			if (mHadSearchStep)
				mUsesLateStarts = true;

			const operation::Indicator necessaryReaction = newStart.onPathFound(boost::optional<std::size_t>(), costsToStart, mUsesLateStarts);
			switch (necessaryReaction)
			{
			case operation::NO_OP:
				return false;
			case operation::ESTIMATE_AND_SCHEDULE:
				newStart.setEstimatedCostsToTarget(mSearchMechanics.estimateCostsToGoal(newStart));
				// fall through to EXPAND by design
			case operation::SCHEDULE:
				mOpenList.push(WeightedSearchState(&newStart, newStart.getTotalCosts()));
				return true;
			default:
				QSF_ERROR("unexpected operation indicator " << necessaryReaction, QSF_REACT_THROW);
			}
		}

		template <typename SearchMechanics>
		void AStar<SearchMechanics>::searchStep()
		{
			if (mOpenList.empty())
				return;

			mHadSearchStep = true;
			SearchState& current = *mOpenList.top().mState;
			mOpenList.pop();

			if (mSearchMechanics.isGoal(current))
			{
				mGoalFound = &current;
				return;
			}

			if (current.isExpanded())
				return; // already expanded before

			std::vector<WeightedSearchState> followUps;
			mSearchMechanics.expand(current, followUps);

			const std::size_t currentStateId = mSearchMechanics.getIndex(current);

			for (std::size_t index = 0; index < followUps.size(); ++index)
			{
				const WeightedSearchState& next = followUps[index];
				SearchState* nextState = next.mState;
				QSF_CHECK(nextState, "found nullptr as search state", QSF_REACT_THROW);

				const operation::Indicator reaction = nextState->onPathFound(currentStateId, next.mCosts, mUsesLateStarts);

				switch (reaction)
				{
				case operation::NO_OP:
					continue;
				case operation::ESTIMATE_AND_SCHEDULE:
					nextState->setEstimatedCostsToTarget(mSearchMechanics.estimateCostsToGoal(*nextState));
					// fall through to expand by design
				case operation::SCHEDULE:
					mOpenList.push(WeightedSearchState(nextState, nextState->getTotalCosts()));
					break;
				default:
					QSF_ERROR("unexpected operation indicator " << reaction, QSF_REACT_THROW);
				}
			}

			current.setExpanded();
		}

		template <typename SearchMechanics>
		void AStar<SearchMechanics>::execute()
		{
			while (!isSuccess() && !isFailed())
				searchStep();
		}

		template <typename SearchMechanics>
		void AStar<SearchMechanics>::writePathFound(const SearchState& goal, std::vector<const SearchState*>& path) const
		{
			if (!goal.wasReached())
				return;

			const SearchState* current = &goal;
			while (current)
			{
				path.push_back(current);
				if (current->getPredecessorId())
					current = &mSearchMechanics.getState(*current->getPredecessorId());
				else
					current = nullptr;
			}

			std::reverse(path.begin(), path.end());
		}
	}
}
