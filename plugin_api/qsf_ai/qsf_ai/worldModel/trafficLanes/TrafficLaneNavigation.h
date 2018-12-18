// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/navigation/NavigationWorldModel.h"
#include "qsf_ai/worldModel/trafficLanes/TrafficLaneWorld.h"
#include "qsf_ai/worldModel/InterMapConnections.h"
#include "qsf_ai/worldModel/trafficLanes/TrafficLaneSearchSpace.h"
#include "qsf_ai/worldModel/trafficLanes/PathUnderConstruction.h"
#include "qsf_ai/base/UnsignedFloat.h"
#include "qsf_ai/base/Radians.h"

#include <boost/container/flat_set.hpp>
#include <boost/optional.hpp>

namespace qsf
{
	namespace ai
	{
		class TemporaryPathNode;
		class AreaType;
		class LaneBound;

		/**
		* @brief Provides the search mechanics details for navigating on a world description made up of traffic lanes.
		*/
		class TrafficLaneNavigation
		{
		public:
			// Navigation world model interface implementation
			//@{
			typedef TrafficLaneWorld WorldModel; // The basic world description
			typedef TrafficLaneSearchState SearchState; // The atomic state of a search on a traffic lane world
			typedef TrafficLaneSearchState::Cost Cost; // The costs to be used to weigh the search states
			typedef std::vector<WeightedState<SearchState>> WeightedSearchStateList; // Additional comfort typedef that is not required by the interface
			typedef std::vector<WeightedSearchContactPoint<Cost>> WeightedContactPointList; // Additional comfort typedef that is not required by the interface

			// Standard constructor providing the world structure, goal and start.
			TrafficLaneNavigation(const WorldModel& world, const PathSearchConfiguration& searchConfig);

			const TrafficLaneWorld& getWorldModel() const;

			std::size_t getIndex(const SearchState& state) const;

			void setSearchStateIndex(SearchContactPoint& searchEntryPoint, bool movingForwards, bool isGoal);
			void writeConnectedNodes(const logic::TargetPoint& targetPoint, bool isGoal, WeightedContactPointList& connectedNodes);
			UnsignedFloat setupNavigationGoalStates();
			void clearNavigationGoalStates();
			SearchContactPoint createGoalExitPointForState(std::size_t stateIndex) const;
			void writeGoalWorldElements(std::vector<unsigned int>& goalWorldElementIds) const;
			void createSmoothPath(const SearchContactPoint& start, const Transform* currentTransform, const std::vector<const SearchState*>& abstractPath, const SearchContactPoint& finish, bool primaryMap, Path& path) const;

			navigation::GoalType getGoalTypeOfLastStateReached() const;
			std::size_t getNumExpandedSearchStates() const;
			SearchState* getMostPromisingExpandedState() const;

			void activateTransitionGoals(const WorldModel& connectedNavMap, UnsignedFloat maxDistanceToGoal = std::numeric_limits<float>::max());
			const CombinedSearchConnectionPoint* tryGetActiveTransitionFrom(const SearchState& state) const;

			void setMaxSearchDistanceLimit(UnsignedFloat limit);
			//@}

			// Search mechanics interface implementation
			//@{
			SearchState& getState(std::size_t index) const;
			Cost estimateCostsToGoal(const SearchState& state) const;
			bool isGoal(const SearchState& state);
			void expand(SearchState& current, WeightedSearchStateList& followUpStates);
			//@}

		private:
			// Extracted part of the writeConnectedNodes algorithm to avoid code repetition
			// Investigate one lane for better closest points than the ones currently found and update the reference parameters passed according to the result.
			// These describe the quality of the best solutions found before and after the function and can be used to prune the search because no results of worse quality need to be explored
			// Returns whether the solutions found inside this lane are ideal.
			// An ideal solution is one that has a zero distance and is inside the lane body.
			// A position in the outer half circle of the lane ending circles is not as good as one being inside the trapezoid defined by the two outer tangent edges.
			// This technically doesn't mean that a solution was actually written but always indicates to look for other solutions that are not ideal themselves.
			bool createSearchEntryPoints(const logic::TargetPoint& targetPoint, bool isGoal, unsigned int laneId,
				UnsignedFloat& bestDistanceFound, bool& solutionsAreInsideLaneBody, WeightedContactPointList& connectedNodes);

			// Helper function calculating the ideal direction of crossing an end node of a lane from the adjacent segment
			// The direction is not normalized by design to carry the distance information to the preceding node via its length.
			glm::vec3 calculateIdealDirection(unsigned int laneId, unsigned int endNodeId, bool towardsNode) const;

			// Try to push the follow up state that is a place holder for the maneuvering in place onto the list of created states
			// Silently does nothing if the space is to small for example
			void tryCreateAbstractManeuverFollowupState(const SearchState& currentState, unsigned int goalLaneIndex, UnsignedFloat maneuverSpace, bool leaveForward,
				WeightedSearchStateList& stateList);

			// Check whether the current state may actually lead to goal states and if yes, create the appropriate states and push them onto the parameter vector
			// The moveAlongLane indicates the direction we are moving along the used lane from the current state.
			void pushReachedGoalStates(const SearchState& current, unsigned int usedLane, bool moveAlongLane, bool moveForward, Cost additionalCost, UnsignedFloat laneCostFactor, WeightedSearchStateList& results);

			// Internal version of the exported function for iterative use with several target points.
			// It starts with the data from the best lane found already initialized from earlier target points
			void writeConnectedNodes(const logic::TargetPoint& targetPoint, bool isGoal, UnsignedFloat& bestDistanceFound, bool& bestSolutionFoundIsInsideIdealBody, WeightedContactPointList& connectedNodes);

			// Comfort getter that defines the smoothing method to use based on navigation map and entity configuration settings
			bool usesFunnelSmoothing() const;

			// Checks whether a complete or partial lane is obstructed by dynamic obstacles.
			// The upper and lower node bounds only consider the default lane direction and don't consider the direction of movement along the lane.
			// You always need to provide an end position that marks the end of the lane usage in this direction, being either a start or goal
			// This is compared to the entered obstacles to decide whether this are valid or not
			bool isLaneObstructedByDynamicObstacles(unsigned int laneIndex, const glm::vec3& position,
				const boost::optional<LaneBound>& lowerSegmentBound = boost::optional<LaneBound>(),
				const boost::optional<LaneBound>& upperSegmentBound = boost::optional<LaneBound>()) const;

			// Helper function for the path creation that removes initial nodes if the entity has moved between requesting the path planning and applying the result
			void removeCrossedWaypoints(const SearchContactPoint& plannedStart, const Transform& realStart, std::deque<Waypoint>& waypoints) const;

			// Checks the reached position against the optional max search distance limit to see whether we may move to this point
			bool isPositionWithinSearchRange(const glm::vec3& pos) const;
			// Checks a position to see whether it is close to a reachable goal
			bool isPositionWithinGoalRange(const glm::vec3& pos, UnsignedFloat maxDistance) const;

			// Static search configuration options
			//@{
			const PathSearchConfiguration& mSearchConfig;
			const WorldModel& mWorld;
			const std::vector<AreaType>& mAllLaneTypes;
			boost::optional<unsigned int> mEntityConfiguration;
			//@}

			// The search entry point configurations that were identified to lead directly to the final goal
			WeightedContactPointList mFinalGoalConfigurations;

			navigation::GoalType mGoalTypeOfLastEvaluatedState; // Always tracks the goal type of the last state that was checked for being a goal.
			TrafficLaneSearchSpace mSearchSpace; // The dynamically expanded search space

			// Optional limit to the max distance to search away from the goal.
			// Set to numeric_limits<float>::max() by default and in case it isn't used
			UnsignedFloat mMaxSearchDistanceLimit;

			// Inner helper class encapsulating information about transitioning to a connected navigation map
			class TransitionData
			{
			public:
				TransitionData();

				// Setup function that may be called once before real search startup
				void setupTransitions(const InterMapConnections* connections, const TrafficLaneNavigation& searchMechanics, const WorldModel& connectedNavMap,
					const std::vector<AreaType>& allAreaTypes, UnsignedFloat maxDistanceToGoal);

				const InterMapConnections* mInterMapConnections; // Connections to a secondary map that may be used during pathfinding. This is a nullptr during a regular single world search.
				// Flags for each transition whether it is a preferred transition.
				// This is usually the one closest to one of the lane ends with the most free space and closest to the end nodes.
				// The size is the same as the number of inter map connections - zero if this is a nullptr.
				std::vector<bool> mPreferredTransitions;
			};

			TransitionData mTransitionData; // Information about how we can switch to a connected navigation map during this search
		};
	}
}
