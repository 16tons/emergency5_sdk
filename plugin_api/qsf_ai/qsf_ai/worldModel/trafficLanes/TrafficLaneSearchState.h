// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/algorithm/IndexedSearchState.h"
#include "qsf_ai/algorithm/UnsignedFloatCosts.h"
#include "qsf_ai/navigation/NavigationWorldModel.h"
#include "qsf_ai/navigation/pathfinding/CombinedSearchConnectionPoint.h"

#include <glm/glm.hpp>

namespace qsf
{
	namespace ai
	{
		/**
		* An atomic state when performing a search on a traffic lane world.
		* There are two types of states.
		* A.) A node in the graph being exactly reached and
		* B.) some intermediate position inside one of the lanes.
		* Besides the optional reached node index and the spatial position, a current or last lane is stored.
		* In case A.), this is the last lane we arrived at the node while in case B it is the current lane.
		* Furthermore we have a flag whether we are currently moving forward.
		* The direction is not normalized - its length actually denotes the distance to the last search state.
		* This is important because it is used inside the search algorithms.
		* To ensure this most direction parameters are actually calculated by the function TrafficLaneNavigation::calculateIdealDirection().
		*
		* @note The superclass IndexedSearchState has no virtual destructor to avoid a vtable pointer per object.
		* Don't dynamically create these objects and delete them via a superclass pointer.
		*/
		class TrafficLaneSearchState : public IndexedSearchState<UnsignedFloatCosts>
		{
		public:
			typedef UnsignedFloatCosts Cost;

			// The state index is unique among all search states while the node index points into the array of end nodes.
			TrafficLaneSearchState(std::size_t stateIndex, unsigned int nodeIndex, unsigned int laneId, unsigned int segmentId, const glm::vec3& position, const glm::vec3& direction,
				bool forward, bool isFinalGoal, const CombinedSearchConnectionPoint* transition = nullptr);

			unsigned int getNodeId() const; // uninitialized if not at a node
			unsigned int getLaneId() const;
			unsigned int getSegmentId() const;
			const CombinedSearchConnectionPoint* getTransition() const; // nullptr if we don't arrived at a transition point
			navigation::GoalType getGoalType() const; // whether the state is a goal state and if yes of which type of goal
			const glm::vec3& getPosition() const;
			const glm::vec3& getDirection() const;
			bool isMovingForward() const;

		private:
			unsigned int mNodeId; // Index into the array of end nodes of this traffic lane world, uninitialized if we don't are exactly at a node
			unsigned int mLaneId;
			unsigned int mSegmentId;
			// Uninitialized means no transition goal reached, This is actually the default. Only set when we reach a transition point.
			// Then it is an index into the transition list from the current to the connected nav map
			const CombinedSearchConnectionPoint* mTransition;
			glm::vec3 mPosition;
			glm::vec3 mDirection; // The direction we are facing
			bool mMovingForward;
			bool mIsFinalGoal;
		};


		// inline implementation
		inline TrafficLaneSearchState::TrafficLaneSearchState(std::size_t stateIndex, unsigned int nodeIndex, unsigned int laneId, unsigned int segmentId,
			const glm::vec3& position, const glm::vec3& direction, bool forward, bool isFinalGoal, const CombinedSearchConnectionPoint* transition) :
		IndexedSearchState(stateIndex),
		mNodeId(nodeIndex),
		mLaneId(laneId),
		mSegmentId(segmentId),
		mTransition(transition),
		mPosition(position),
		mDirection(direction),
		mMovingForward(forward),
		mIsFinalGoal(isFinalGoal)
		{}

		inline unsigned int TrafficLaneSearchState::getNodeId() const
		{
			return mNodeId;
		}

		inline unsigned int TrafficLaneSearchState::getLaneId() const
		{
			return mLaneId;
		}

		inline unsigned int TrafficLaneSearchState::getSegmentId() const
		{
			return mSegmentId;
		}

		inline const CombinedSearchConnectionPoint* TrafficLaneSearchState::getTransition() const
		{
			return mTransition;
		}

		inline const glm::vec3& TrafficLaneSearchState::getPosition() const
		{
			return mPosition;
		}

		inline const glm::vec3& TrafficLaneSearchState::getDirection() const
		{
			return mDirection;
		}

		inline bool TrafficLaneSearchState::isMovingForward() const
		{
			return mMovingForward;
		}

		inline navigation::GoalType TrafficLaneSearchState::getGoalType() const
		{
			if (mIsFinalGoal)
				return navigation::TARGET_REACHED;
			if (mTransition)
				return navigation::TRANSITION_REACHED;
			return navigation::NO_GOAL;
		}
	}
}
