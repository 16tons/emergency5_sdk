// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/base/Direction.h"
#include "qsf_ai/navigation/Path.h"

#include <qsf/base/error/ErrorHandling.h>
#include <qsf/debug/request/CompoundDebugDrawRequest.h>

#include <vector>

namespace qsf
{
	namespace ai
	{
		template <typename NavigationMechanics>
		SimplePathSearchImplementation<NavigationMechanics>::SimplePathSearchImplementation(unsigned int taskId, const typename NavigationMechanics::WorldModel& model, std::auto_ptr<PathSearchConfiguration> searchConfig) :
			PathSearch(taskId, searchConfig),
			mNavigationMechanics(model, *mSearchConfig),
			mSearchState(mNavigationMechanics),
			mStartAndGoalInitialized(false),
			mRunning(false)
		{}

		template <typename NavigationMechanics>
		void SimplePathSearchImplementation<NavigationMechanics>::setupStartAndGoalStates()
		{
			QSF_CHECK(!mStartAndGoalInitialized, "Initialize called twice on path search",
				return);

			UnsignedFloat bestDistanceFound = mNavigationMechanics.setupNavigationGoalStates();
			if (!mSearchConfig->mPositionCorrectionSettings.mGoalSettings.mCorrectIllegalState && bestDistanceFound > SearchContactPoint::MAX_ROUND_OFF_DISTANCE_ERROR)
			{
				mNavigationMechanics.clearNavigationGoalStates(); // don't use the goal states as the position should not be corrected
				setUninitialized(bestDistanceFound);
			}

			if (isInitialized(bestDistanceFound) || mSearchConfig->mGoal->doesRemainingDistanceIndicateGoalState() || mSearchConfig->shouldMoveToClosestInCaseOfFailure()) // avoid a costly search if there is no chance to reach the goal at all
			{
				typedef std::vector<WeightedSearchContactPoint<typename NavigationMechanics::SearchState::Cost>> NodeList;

				// add start states to the A* state
				NodeList startNodes;
				const logic::TargetPoint currentStateAsTargetPoint(mSearchConfig->mStartPosition, true, 0.f, 0.f, mSearchConfig->mStartOrientation);
				mNavigationMechanics.writeConnectedNodes(currentStateAsTargetPoint, false, startNodes);

				const auto& positionCorrectionSettings = mSearchConfig->mPositionCorrectionSettings;

				for (typename NodeList::iterator i = startNodes.begin(); i != startNodes.end(); ++i)
				{
					const SearchContactPoint& sep = i->mSearchContactPoint;

					// filter illegal positions here if these are not corrected
					if (!positionCorrectionSettings.mStartSettings.mCorrectIllegalState)
					{
						if (sep.mDistanceToMoveToLegalPosition > SearchContactPoint::MAX_ROUND_OFF_DISTANCE_ERROR * UncheckedUnsignedFloat(2.f))
							break; // Found one so far off that all other need to be too far away to be considered

						if (sep.mDistanceToMoveToLegalPosition > SearchContactPoint::MAX_ROUND_OFF_DISTANCE_ERROR)
							continue; // only ignore this one selectively
					}

					mSearchState.addStart(sep.mSearchStateIndex, i->mCosts);
					mStartEntryPoints.emplace(sep.mSearchStateIndex, sep);
				}
			}

			mStartAndGoalInitialized = true;
		}

		template <typename NavigationMechanics>
		process::State SimplePathSearchImplementation<NavigationMechanics>::getState() const
		{
			if (!mStartAndGoalInitialized)
				return process::WAITING;

			if (mSearchState.isSuccess())
				return process::FINISHED;

			if (mSearchState.isFailed())
				return process::FAILED;

			if (!mRunning) // running should be true if search is neither finished nor failed - might be a data race here though
				return process::INTERRUPTED;

			return process::RUNNING;
		}

		template <typename NavigationMechanics>
		void SimplePathSearchImplementation<NavigationMechanics>::interrupt()
		{
			mRunning = false;
		}

		template <typename NavigationMechanics>
		void SimplePathSearchImplementation<NavigationMechanics>::fail()
		{
			mSearchState.failSearch();
		}

		template <typename NavigationMechanics>
		std::size_t SimplePathSearchImplementation<NavigationMechanics>::getNumExpandedSearchStates() const
		{
			return mNavigationMechanics.getNumExpandedSearchStates();
		}

		template <typename NavigationMechanics>
		process::State SimplePathSearchImplementation<NavigationMechanics>::execute()
		{
			mRunning = true;

			if (!mStartAndGoalInitialized)
				setupStartAndGoalStates();

			while (mRunning && !mSearchState.isSuccess() && !mSearchState.isFailed())
				mSearchState.searchStep();

			mRunning = false;
			return getState();
		}

		template <typename NavigationMechanics>
		void SimplePathSearchImplementation<NavigationMechanics>::writeResultingPath(const Transform& currentTransform, Path& path) const
		{
			typedef typename NavigationMechanics::SearchState SearchState;

			std::vector<const SearchState*> states; // the abstract path on the search graph - remains empty for a direct solution
			const SearchState* goalState = nullptr;
			if (!mSearchState.isSuccess())
			{
				if (mSearchConfig->shouldMoveToClosestInCaseOfFailure())
				{
					goalState = mNavigationMechanics.getMostPromisingExpandedState();
					if (!goalState)
						return; // there was nothing expanded so nowhere to move to

					const UnsignedFloat remainingDistanceAfterSearch = mSearchConfig->mGoal->estimateDistanceToTarget(goalState->getPosition());
					const UnsignedFloat initialRemainingDistance = mSearchConfig->mGoal->estimateDistanceToTarget(mSearchConfig->mStartPosition);
					if (remainingDistanceAfterSearch >= initialRemainingDistance)
						goalState = nullptr; // don't use the search if we don't get closer to our goal
				}
				if (!goalState)
					return;
			}
			else
				goalState = &mSearchState.getGoalFound();

			// retrieve abstract path and smooth it via the specific mechanics
			mSearchState.writePathFound(*goalState, states);
			QSF_WARN_IF(states.empty(), "Found successful empty path. Although not an error in itself this shouldn't be created by the current implementation",
				return);

			const ConnectionList::const_iterator startEntry = mStartEntryPoints.find(mNavigationMechanics.getIndex(*states.front()));
			QSF_CHECK(startEntry != mStartEntryPoints.end(), "Successful path doesn't start at a defined starting location",
				QSF_REACT_THROW);
			const SearchContactPoint& startPoint = startEntry->second;

			const std::size_t goalStateIndex = mNavigationMechanics.getIndex(*states.back());
			const SearchContactPoint exitPoint = mNavigationMechanics.createGoalExitPointForState(goalStateIndex);

			mNavigationMechanics.createSmoothPath(startPoint, &currentTransform, states, exitPoint, true, path);
		}

		template <typename NavigationMechanics>
		bool SimplePathSearchImplementation<NavigationMechanics>::isGoalReachable() const
		{
			if (mSearchConfig->mGoal->doesRemainingDistanceIndicateGoalState())
				return true; // no early out possible

			std::vector<unsigned int> goalElementIds;
			mNavigationMechanics.writeGoalWorldElements(goalElementIds);

			return !goalElementIds.empty();
		}

		template <typename NavigationMechanics>
		void SimplePathSearchImplementation<NavigationMechanics>::createStateDebug(CompoundDebugDrawRequest& cddr) const
		{
			// TODO(vs) extend with output for further states
			if (!isGoalReachable())
				cddr.mTexts.emplace_back("Goal is illegal", mSearchConfig->mStartPosition);
		}
	}
}
