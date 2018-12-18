// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/algorithm/SearchMechanics.h"
#include "qsf_ai/base/Direction.h"
#include "qsf_ai/navigation/Waypoint.h"
#include "qsf_ai/navigation/pathfinding/SearchContactPoint.h"
#include "qsf_ai/navigation/pathfinding/PathSearchConfiguration.h"
#include "qsf_ai/worldModel/InterMapConnections.h"
#include "qsf_ai/navigation/Path.h"

#include <qsf_logic/targetpoint/TargetPoint.h>

#include <glm/fwd.hpp>

#include <boost/optional.hpp>

#include <vector>

namespace qsf
{
	namespace ai
	{
		namespace navigation
		{
			// Differentiate between different types of goals.
			enum GoalType
			{
				NO_GOAL,
				TARGET_REACHED, // The real target position was reached
				TRANSITION_REACHED, // A transition to a connected nav map was reached
			};
		}

		/** Interface for accessing an abstraction of the world for the navigation systems.
		* It provides the search mechanic details for translating real 3D positions to the abstract A* structures needed.
		* Like the search mechanics it extends it is only a templated interface declaration to be used as a reference without an implementation available.
		*/
		template <typename _WorldModel, typename _SearchState>
		class NavigationWorldModel : public SearchMechanics<_SearchState>
		{
		public:
			// Necessary typdefs to be used with the templated implementation of the AStar and pathfinding search
			//@{
			typedef _SearchState SearchState; // Defines an atomic search state
			typedef _WorldModel WorldModel; // Defines the underlying persistent world representation used
			//@}

			// The path search configuration is kept alive for the lifetime of the navigation world model.
			// Only store a reference, the NavigationWorldModel does 'not' assume ownership on the path search configuration
			NavigationWorldModel(const WorldModel& world, const PathSearchConfiguration& searchConfig);

			// Access to the world model passed as creation parameter
			const WorldModel& getWorldModel() const;

			// Convert a state to a unique numerical index or id
			std::size_t getIndex(const SearchState& state) const;

			// Create (or retrieve if already created) a fitting search state and set the index for the search contact point passed
			void setSearchStateIndex(SearchContactPoint& searchContactPoint, bool movingForward, bool isGoal);

			// Trigger the creation of the navigation goal search states from the navigation goal
			// Returns the distance to the legal space ofthe closest nodes found, uninitialized if no nodes where found at all
			UnsignedFloat setupNavigationGoalStates();

			// Erase the list of navigation goal states. Is Usually called if an illegal position should not be corrected because of the settings
			void clearNavigationGoalStates();

			// Determine a list of nodes that can be directly moved towards from the target point.
			// These are all the nodes where we don't need to ask the graph structure for a path and this is used to determine the start and goal nodes.
			// The isGoal flag governs whether the start or finish error correction settings are used and in which direction the node needs to be connected.
			void writeConnectedNodes(const logic::TargetPoint& targetPoint, bool isGoal, std::vector<WeightedSearchContactPoint<typename SearchState::Cost>>& connectedNodes);

			// Write the ids of the world elements that contain at least one final goal state the vector passed as reference.
			void writeGoalWorldElements(std::vector<unsigned int>& goalWorldElementIds) const;

			// Create a search entry point dynamically if we would plan to leave at the search state with the given index.
			// This only makes sense for goal types that don't create their goal states in advance.
			SearchContactPoint createGoalExitPointForState(std::size_t stateIndex) const;

			// Transfer an abstract node path to a real 3D path and apply necessary smoothing
			// The flag for being the primary map is there for technical reasons and should normally just be incorporated into each Waypoint created.
			// The optional current transform contains the values at this very point in time while the path start contains the values when the request was launched.
			// If a change was planned while the entity was moving it has typically moved between requesting and fetching the result and the two data sets might be used to integrate these changes
			void createSmoothPath(const SearchContactPoint& searchStart, const Transform* currentTransform, const std::vector<const SearchState*>& abstractPath, const SearchContactPoint& searchEnd,
				bool primaryMap, Path& result) const;

			// Return the goal type of the last state that was evaluated for being a goal.
			navigation::GoalType getGoalTypeOfLastStateReached() const;

			// For statistic purposes return the number of states created
			std::size_t getNumExpandedSearchStates() const;

			// Should return the most promising state that was yet expanded. If a goal is found this should of course be returned.
			// Otherwise it is up to the implementation to select the optimal state among all expanded.
			// May return a nullptr but should normally only do so in case there was not a single state expanded yet.
			SearchState* getMostPromisingExpandedState() const;

			// Make transitioning to the connected map reference parameter a goal for this search.
			// May pass a maximum distance to the final goal for limiting the activations
			// May be called several times throughout a search and expects to overwrite the state
			void activateTransitionGoals(const WorldModel& connectedNavMap, UnsignedFloat maxDistanceToGoal = std::numeric_limits<float>::max());

			// Return the possible transition from the search state passed.
			// This only returns anything valid if the state indicates a reached transition.
			// Otherwise returns a nullptr.
			const CombinedSearchConnectionPoint* tryGetActiveTransitionFrom(const SearchState& state) const;

			// limit the max search distance to look away from the start position.
			// This is actually used dynamically from inside the combined path search and may change between subsequent calls
			void setMaxSearchDistanceLimit(UnsignedFloat limit);
		};
	}
}
