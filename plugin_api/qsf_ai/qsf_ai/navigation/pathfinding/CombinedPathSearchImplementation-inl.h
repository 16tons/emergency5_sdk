// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/navigation/NavigationWorldModel.h"
#include "qsf_ai/navigation/Path.h"
#include "qsf_ai/navigation/pathfinding/PathfindingSystem.h"
#include "qsf_ai/worldModel/trafficLanes/TurningRadiusCalculator.h"
#include "qsf_ai/worldModel/WorldModelManager.h"
#include "qsf_ai/base/StdMapHelper.h"

#include <qsf/base/error/ErrorHandling.h>
#include <qsf/debug/request/CompoundDebugDrawRequest.h>


namespace qsf
{
	namespace ai
	{
		// Implementation of core search algorithm
		template <typename NavigationMechanicsA, typename NavigationMechanicsB>
		CombinedPathSearchImplementation<NavigationMechanicsA, NavigationMechanicsB>::CombinedPathSearchImplementation(unsigned int taskId, const typename NavigationMechanicsA::WorldModel& modelA,
			const typename NavigationMechanicsB::WorldModel& modelB, std::auto_ptr<PathSearchConfiguration> searchConfig) :
			PathSearch(taskId, searchConfig),
			mAreaTypes(WorldModelManager::getInstance().getAreaTypes()),
			mNavigationMechanicsA(modelA, *mSearchConfig),
			mSearchOnA(mNavigationMechanicsA),
			mNavigationMechanicsB(modelB, *mSearchConfig),
			mSearchOnB(mNavigationMechanicsB),
			mGoalTransitionRadiusAToB(std::numeric_limits<float>::max()),
			mStartTransitionRadiusBToA(std::numeric_limits<float>::max()),
			mTransitionPenalty(PathfindingSystem::getInstance().getTransitionPenalty()),
			mMaxCrossingDiamter(PathfindingSystem::getInstance().getMaxCrossingDiameter()),
			mStartAndGoalInitialized(false),
			mRunning(false)
		{}

		template <typename NavigationMechanicsA, typename NavigationMechanicsB>
		process::State CombinedPathSearchImplementation<NavigationMechanicsA, NavigationMechanicsB>::getState() const
		{
			if (!mStartAndGoalInitialized)
				return process::WAITING;

			if ((mSearchOnA.isSuccess() && mNavigationMechanicsA.getGoalTypeOfLastStateReached() == navigation::TARGET_REACHED) ||
				(mSearchOnB.isSuccess() && mNavigationMechanicsB.getGoalTypeOfLastStateReached() == navigation::TARGET_REACHED))
				return process::FINISHED;

			if (mSearchOnA.isOpenListEmpty() && mSearchOnB.isOpenListEmpty())
				return process::FAILED;

			if (!mRunning)
				return process::INTERRUPTED;

			return process::RUNNING;
		}

		template <typename NavigationMechanicsA, typename NavigationMechanicsB>
		void CombinedPathSearchImplementation<NavigationMechanicsA, NavigationMechanicsB>::interrupt()
		{
			mRunning = false;
		}

		template <typename NavigationMechanicsA, typename NavigationMechanicsB>
		void CombinedPathSearchImplementation<NavigationMechanicsA, NavigationMechanicsB>::fail()
		{
			mSearchOnA.failSearch();
			mSearchOnB.failSearch();
		}

		template <typename NavigationMechanicsA, typename NavigationMechanicsB>
		std::size_t CombinedPathSearchImplementation<NavigationMechanicsA, NavigationMechanicsB>::getNumExpandedSearchStates() const
		{
			return mNavigationMechanicsA.getNumExpandedSearchStates() + mNavigationMechanicsB.getNumExpandedSearchStates();
		}

		namespace
		{
			// Local subparts of the algorithm that are extracted as a template for technical reasons to reduce code duplication
			template <typename SearchMechanics, typename Search>
			navigation::GoalType executeSearchStep(const SearchMechanics& searchRules, Search& search)
			{
				search.clearGoalFound();
				if (search.isOpenListEmpty())
					return navigation::NO_GOAL;
					// If we can't advance at all we don't need to start the search
					// Returning the last state reached would return the state reached during the last search phase.
				search.searchStep();

				return searchRules.getGoalTypeOfLastStateReached();
			}

			// Create new starts in B from the currently reached goal in A.
			// Returns whether at least one new start was added that will actually be used later on.
			// Attention - these A and B are local and may not correspond to the instance definition inside the combined path search.
			template <typename SearchMechanicsA, typename SearchMechanicsB, typename SearchB, typename TransitionsToB>
			void transitionToOtherMap(const PathSearchConfiguration& searchConfig, const std::vector<AreaType>& areaTypes,
				const SearchMechanicsA& logicA, const typename SearchMechanicsA::SearchState& reachedGoalOnA, SearchMechanicsB& logicB, SearchB& searchB, TransitionsToB& transitionsToB,
				UnsignedFloat transitionPenalty)
			{
				const CombinedSearchConnectionPoint* connectionData = logicA.tryGetActiveTransitionFrom(reachedGoalOnA);
				QSF_CHECK(connectionData, "Unable to retrieve connection data during combined path search",
					return);

				SearchContactPoint reachedStartOnB = connectionData->convertToSearchEntryPointOnReachedWorld(reachedGoalOnA.getPosition(), reachedGoalOnA.getDirection());
				logicB.setSearchStateIndex(reachedStartOnB, reachedGoalOnA.isMovingForward(), false); // create new search state in reached world
				if (searchB.addStart(reachedStartOnB.mSearchStateIndex, reachedGoalOnA.getCostsToGetHere() + transitionPenalty))
				{
					efficientInsertOrOverwrite(transitionsToB, reachedStartOnB.mSearchStateIndex,
						UsedTransitionData(connectionData, reachedGoalOnA.getPosition(), reachedGoalOnA.getDirection(), reachedGoalOnA.getIndex()));
				}
			}
		}

		template <typename NavigationMechanicsA, typename NavigationMechanicsB>
		process::State CombinedPathSearchImplementation<NavigationMechanicsA, NavigationMechanicsB>::execute()
		{
			mRunning = true;

			if (!mStartAndGoalInitialized)
			{
				mRunning = setupStartAndGoalStates();
				mStartAndGoalInitialized = true;
			}

			while (mRunning && (!mSearchOnA.isOpenListEmpty() || !mSearchOnB.isOpenListEmpty()))
			{
				// Determine on which world to execute a search step
				const bool searchOnA = mSearchOnB.isOpenListEmpty() ||
					(!mSearchOnA.isOpenListEmpty() && mSearchOnA.tryPeakOpenList()->mCosts <= mSearchOnB.tryPeakOpenList()->mCosts);

				const navigation::GoalType reachedGoalType = searchOnA ? executeSearchStep(mNavigationMechanicsA, mSearchOnA) : executeSearchStep(mNavigationMechanicsB, mSearchOnB);
				switch (reachedGoalType)
				{
				case navigation::NO_GOAL:
					break;
				case navigation::TARGET_REACHED:
					mRunning = false; // found a goal, stop running
					break;
				case navigation::TRANSITION_REACHED:
					{
						const auto& currentSearchLogic = searchOnA ? mNavigationMechanicsA : mNavigationMechanicsB;
						const auto& currentSearch = searchOnA ? mSearchOnA : mSearchOnB;
						auto& reachedSearchLogic = searchOnA ? mNavigationMechanicsB : mNavigationMechanicsA;
						auto& reachedSearch = searchOnA ? mSearchOnB : mSearchOnA;
						auto& transitionsFromCurrent = searchOnA ? mTransitionsToB : mTransitionsToA;

						if (!searchOnA)
						{
							// Check the transition radius
							const UnsignedFloat distanceToTransitionPos = getLength(CoordinateSystem::getTopViewProjector().dropValue(currentSearch.getGoalFound().getPosition() - mSearchConfig->mStartPosition));
							if (isUninitialized(mStartTransitionRadiusBToA))
							{
								setupStartTransitionRadius(distanceToTransitionPos + mMaxCrossingDiamter);
							}
							// First transition found in a scenario where the transition range was not limited initially
							else if (distanceToTransitionPos > mStartTransitionRadiusBToA)
							{
								break; // skip a transition that is outside the desired radius. This might happen as the state may have been created before setting the radius but investigated afterwards
							}
						}

						transitionToOtherMap(*mSearchConfig, mAreaTypes, currentSearchLogic, currentSearch.getGoalFound(), reachedSearchLogic, reachedSearch, transitionsFromCurrent, mTransitionPenalty);
						break;
					}
				default:
					QSF_ERROR("Unexpected goal type value " << reachedGoalType,
						QSF_REACT_THROW);
				}
			}

			mRunning = false;
			return getState();
		}

		template <typename NavigationMechanicsA, typename NavigationMechanicsB>
		void CombinedPathSearchImplementation<NavigationMechanicsA, NavigationMechanicsB>::setupStartTransitionRadius(UnsignedFloat radius)
		{
			mStartTransitionRadiusBToA = radius;
			mNavigationMechanicsB.setMaxSearchDistanceLimit(mStartTransitionRadiusBToA); // set the search distance limit to limit the transitions activated
			mNavigationMechanicsB.activateTransitionGoals(mNavigationMechanicsA.getWorldModel());
			mNavigationMechanicsB.setMaxSearchDistanceLimit(getUninitialized<UnsignedFloat>()); // clear the search radius limit immediately
		}

		template <typename NavigationMechanicsA, typename NavigationMechanicsB>
		bool CombinedPathSearchImplementation<NavigationMechanicsA, NavigationMechanicsB>::setupStartAndGoalStates()
		{
			bool goalReachable = mSearchConfig->mGoal->doesRemainingDistanceIndicateGoalState() || mSearchConfig->shouldMoveToClosestInCaseOfFailure();

			UnsignedFloat closestGoalDistanceOnAFound = mNavigationMechanicsA.setupNavigationGoalStates();
			UnsignedFloat closestGoalDistanceOnBFound = mNavigationMechanicsB.setupNavigationGoalStates();

			// Only keep the closer goal states and only allow corrections if the settings say so
			if ((closestGoalDistanceOnAFound > closestGoalDistanceOnBFound + SearchContactPoint::MAX_ROUND_OFF_DISTANCE_ERROR) ||
				(!mSearchConfig->mPositionCorrectionSettings.mGoalSettings.mCorrectIllegalState && closestGoalDistanceOnAFound > SearchContactPoint::MAX_ROUND_OFF_DISTANCE_ERROR))
			{
				mNavigationMechanicsA.clearNavigationGoalStates();
				setUninitialized(closestGoalDistanceOnAFound);
			}
			if ((closestGoalDistanceOnBFound > closestGoalDistanceOnAFound + SearchContactPoint::MAX_ROUND_OFF_DISTANCE_ERROR) ||
				(!mSearchConfig->mPositionCorrectionSettings.mGoalSettings.mCorrectIllegalState && closestGoalDistanceOnBFound > SearchContactPoint::MAX_ROUND_OFF_DISTANCE_ERROR))
			{
				mNavigationMechanicsB.clearNavigationGoalStates();
				setUninitialized(closestGoalDistanceOnBFound);
			}

			goalReachable = goalReachable || isInitialized(closestGoalDistanceOnAFound) || isInitialized(closestGoalDistanceOnBFound);
			if (!goalReachable)
				return false; // avoid a search if there is no hope at all

			// Determine start nodes on A
			typedef WeightedSearchContactPoint<typename NavigationMechanicsA::SearchState::Cost> EntryPointA;
			typedef std::vector<EntryPointA> NodeListA;
			NodeListA startNodesOnA;

			const logic::TargetPoint currentStateAsTargetPoint(mSearchConfig->mStartPosition, true, 0.f, 0.f, mSearchConfig->mStartOrientation);
			mNavigationMechanicsA.writeConnectedNodes(currentStateAsTargetPoint, false, startNodesOnA);
			UnsignedFloat referenceStartDistanceA = startNodesOnA.empty() ? UnsignedFloat(std::numeric_limits<float>::max()) : startNodesOnA.front().mSearchContactPoint.mDistanceToMoveToLegalPosition;

			// Determine start nodes on B
			typedef WeightedSearchContactPoint<typename NavigationMechanicsB::SearchState::Cost> EntryPointB;
			typedef std::vector<EntryPointB> NodeListB;
			NodeListB startNodesOnB;

			mNavigationMechanicsB.writeConnectedNodes(currentStateAsTargetPoint, false, startNodesOnB);
			UnsignedFloat referenceStartDistanceB = startNodesOnB.empty() ? UnsignedFloat(std::numeric_limits<float>::max()) : startNodesOnB.front().mSearchContactPoint.mDistanceToMoveToLegalPosition;

			// now cleanup the start states with a similar logic to the goal nodes
			// Only keep the closer goal states and only allow corrections if the settings say so
			if ((referenceStartDistanceA > referenceStartDistanceB + SearchContactPoint::MAX_ROUND_OFF_DISTANCE_ERROR) ||
				(!mSearchConfig->mPositionCorrectionSettings.mStartSettings.mCorrectIllegalState && referenceStartDistanceA > SearchContactPoint::MAX_ROUND_OFF_DISTANCE_ERROR))
			{
				startNodesOnA.clear();
				setUninitialized(referenceStartDistanceA);
			}
			if ((referenceStartDistanceB > referenceStartDistanceA + SearchContactPoint::MAX_ROUND_OFF_DISTANCE_ERROR) ||
				(!mSearchConfig->mPositionCorrectionSettings.mStartSettings.mCorrectIllegalState && referenceStartDistanceB > SearchContactPoint::MAX_ROUND_OFF_DISTANCE_ERROR))
			{
				startNodesOnB.clear();
				setUninitialized(referenceStartDistanceB);
			}

			if (startNodesOnA.empty() && startNodesOnB.empty())
				return false; // unable to use any of the nodes

			for (const auto& entry : startNodesOnA)
			{
				const SearchContactPoint& scp = entry.mSearchContactPoint;
				mSearchOnA.addStart(scp.mSearchStateIndex, entry.mCosts);
				mStartsOnA.emplace(scp.mSearchStateIndex, scp);
			}
			for (const auto& entry : startNodesOnB)
			{
				const SearchContactPoint& scp = entry.mSearchContactPoint;
				mSearchOnB.addStart(scp.mSearchStateIndex, entry.mCosts);
				mStartsOnB.emplace(scp.mSearchStateIndex, scp);
			}

			// Activate transitions around the start on B
			// This allows making a shortcut on crossings
			if (!mStartsOnA.empty() && referenceStartDistanceA < SearchContactPoint::MAX_ROUND_OFF_DISTANCE_ERROR)
				setupStartTransitionRadius(mMaxCrossingDiamter);
			else
				mNavigationMechanicsB.activateTransitionGoals(mNavigationMechanicsA.getWorldModel()); // activate all transitions initially

			// Also enable selectively the transitions back to B in the vicinity of goals on B.
			// We can't just search because searching backwards is not supported as of now.
			// We try to work with the bee line distance here
			mNavigationMechanicsA.activateTransitionGoals(mNavigationMechanicsB.getWorldModel(), closestGoalDistanceOnAFound + mMaxCrossingDiamter);

			return true;
		}

		namespace
		{
			typedef boost::container::flat_map<std::size_t, SearchContactPoint> ConnectionList;
			typedef std::map<std::size_t, UsedTransitionData> TransitionMap;

			// Parts of the write path algorithms are extracted here for technical reasons to avoid code duplication.
			// The optional goal needs to be initialized when entering the function but is either overwritten or reset during the function
			template <typename SearchMechanics, typename Search>
			void writePathSegment(const PathSearchConfiguration& searchConfig, const Transform& currentTransform, const SearchMechanics& mechanics, const Search& search, const ConnectionList& starts,
				const TransitionMap& transitions, bool isPrimaryMap, navigation::GoalType goalType,
				Path& resultingPath, boost::optional<SearchContactPoint>& currentGoal, SearchContactPoint& lastStart) // these data fields are written by this function
			{
				typedef typename SearchMechanics::SearchState State;
				QSF_CHECK(currentGoal, "Sanity check failed, calling write path segment with uninitialized goal",
					return);

				// Retrieve goal state moving towards the switch position
				const State& goalState = mechanics.getState(currentGoal->mSearchStateIndex);

				std::vector<const State*> states;
				search.writePathFound(goalState, states);
				QSF_CHECK(!states.empty(), "Found empty path segment during combined search",
					QSF_REACT_THROW);

				// Determine whether this was an original start or a used transition
				const auto& firstState = *states.front();
				const std::size_t firstStateIndex = mechanics.getIndex(firstState);
				const UsedTransitionData* connectionData = nullptr; // This remains null if we found an original start

				{
					const ConnectionList::const_iterator usedStart = starts.find(firstStateIndex);
					if (usedStart != starts.end())
						lastStart = usedStart->second;
					else
					{
						const TransitionMap::const_iterator usedTransition = transitions.find(firstStateIndex);
						QSF_CHECK(usedTransition != transitions.end(), "Corruption during path creation. Intermediate start node not found",
							QSF_REACT_THROW);

						// Recreate the local start search entry point
						connectionData = &usedTransition->second;
						lastStart = connectionData->convertToSearchEntryPointOnReachedWorld();
						lastStart.mSearchStateIndex = firstStateIndex;
					}
				}

				const bool isTransitionGoal = connectionData != nullptr;

				// write gateways - and only pass the transform for the initial part
				Path partialPath;
				mechanics.createSmoothPath(lastStart, isTransitionGoal ? nullptr : &currentTransform, states, *currentGoal, isPrimaryMap, partialPath);
				resultingPath.prependNodes(partialPath.getNodes());

				// Now update the goal for the next path part
				if (isTransitionGoal)
					currentGoal = connectionData->convertToSearchEntryPointOnStartWorld(); // The starting part of the transition taken will become the new goal when searching on the other map
				else
					currentGoal.reset(); // found the original start and the path should be finished
			}
		}

		template <typename NavigationMechanicsA, typename NavigationMechanicsB>
		void CombinedPathSearchImplementation<NavigationMechanicsA, NavigationMechanicsB>::writeResultingPath(const Transform& currentTransform, Path& path) const
		{
			// Track which map we are currently reading from
			bool readOnA = true;

			// We track the immediate goal state id on this variable.
			// This will be the final goal at the beginning and transitions states later on.
			// The value is only valid on the current map.
			boost::optional<SearchContactPoint> currentGoalState;

			if (mNavigationMechanicsA.getGoalTypeOfLastStateReached() == navigation::TARGET_REACHED)
			{
				const std::size_t goalStateIndex = mNavigationMechanicsA.getIndex(mSearchOnA.getGoalFound());
				currentGoalState = mNavigationMechanicsA.createGoalExitPointForState(goalStateIndex);
			}
			else if (mNavigationMechanicsB.getGoalTypeOfLastStateReached() == navigation::TARGET_REACHED)
			{
				const std::size_t goalStateIndex = mNavigationMechanicsB.getIndex(mSearchOnB.getGoalFound());
				currentGoalState = mNavigationMechanicsB.createGoalExitPointForState(goalStateIndex);
				readOnA = false;
			}
			else if (mSearchConfig->shouldMoveToClosestInCaseOfFailure())
			{
				typedef typename NavigationMechanicsA::SearchState SearchState;
				SearchState* candidateA = mNavigationMechanicsA.getMostPromisingExpandedState();
				SearchState* candidateB = mNavigationMechanicsB.getMostPromisingExpandedState();
				const UnsignedFloat remainingDistanceOnA = candidateA ? mSearchConfig->mGoal->estimateDistanceToTarget(candidateA->getPosition()) : UnsignedFloat(std::numeric_limits<float>::max());
				const UnsignedFloat remainingDistanceOnB = candidateB ? mSearchConfig->mGoal->estimateDistanceToTarget(candidateB->getPosition()) : UnsignedFloat(std::numeric_limits<float>::max());
				const UnsignedFloat initialRemainingDistance = mSearchConfig->mGoal->estimateDistanceToTarget(mSearchConfig->mStartPosition); // we compare to the initial distance to avoid a movement if there is no improvement possible

				if (candidateA && (remainingDistanceOnA <= std::min(remainingDistanceOnB, initialRemainingDistance)))
				{
					const std::size_t stateIndex = mNavigationMechanicsA.getIndex(*candidateA);
					currentGoalState = mNavigationMechanicsA.createGoalExitPointForState(stateIndex);
				}
				else if (candidateB && remainingDistanceOnB < initialRemainingDistance)
				{
					const std::size_t stateIndex = mNavigationMechanicsB.getIndex(*candidateB);
					currentGoalState = mNavigationMechanicsB.createGoalExitPointForState(stateIndex);
					readOnA = false;
				}
			}

			// we track the type of sub goal to reach here
			navigation::GoalType currentGoalType = navigation::TARGET_REACHED; // the first goal to reach backwards is the final target
			SearchContactPoint lastStart; // This is used to retrieve the initial start configuration at the end of the loop
			while (currentGoalState)
			{
				if (readOnA)
					writePathSegment(*mSearchConfig, currentTransform, mNavigationMechanicsA, mSearchOnA, mStartsOnA, mTransitionsToA, true, currentGoalType, path, currentGoalState, lastStart);
				else
					writePathSegment(*mSearchConfig, currentTransform, mNavigationMechanicsB, mSearchOnB, mStartsOnB, mTransitionsToB, false, currentGoalType, path, currentGoalState, lastStart);

				readOnA = !readOnA;
				currentGoalType = currentGoalState ? navigation::TRANSITION_REACHED : navigation::NO_GOAL; // everything but the first goal is a transition until we reached the start of the path and don't have any goal anymore
			}
		}

		template <typename NavigationMechanicsA, typename NavigationMechanicsB>
		bool CombinedPathSearchImplementation<NavigationMechanicsA, NavigationMechanicsB>::isGoalReachable() const
		{
			if (mSearchConfig->mGoal->doesRemainingDistanceIndicateGoalState())
				return true; // no early out possible

			std::vector<unsigned int> goalElementIds;
			mNavigationMechanicsA.writeGoalWorldElements(goalElementIds);
			if (!goalElementIds.empty())
				return true;

			mNavigationMechanicsB.writeGoalWorldElements(goalElementIds);
			return !goalElementIds.empty();
		}

		template <typename NavigationMechanicsA, typename NavigationMechanicsB>
		void CombinedPathSearchImplementation<NavigationMechanicsA, NavigationMechanicsB>::createStateDebug(CompoundDebugDrawRequest& cddr) const
		{
			// TODO(vs) extend with output for further states
			if (!isGoalReachable())
				cddr.mTexts.emplace_back("Goal is illegal", mSearchConfig->mStartPosition);
		}
	}
}
