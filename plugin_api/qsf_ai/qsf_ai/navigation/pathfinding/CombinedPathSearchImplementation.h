// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/algorithm/AStar.h"
#include "qsf_ai/navigation/pathfinding/PathSearch.h"
#include "qsf_ai/navigation/pathfinding/CombinedSearchConnectionPoint.h"
#include "qsf_ai/worldModel/AreaType.h"

#include <qsf/math/CoordinateSystem.h>

#include <glm/fwd.hpp>

#include <boost/optional.hpp>
#include <boost/container/flat_map.hpp>

#include <map>

namespace qsf
{
	namespace ai
	{
		/** A combined navigation search on two different world representations.
		* The search may transition between the two worlds according to the rules defined by the world.
		* Currently this is only supported for two different lane worlds with predefined intersection points read from file.
		* There is currently no real hierarchy between the two worlds.
		* The entity might transition as often as they want as long as it brings them closer to their goals.
		* The penalties are defined by the movement penalty factors defined for the areas.
		* We could also think about a transition penalty.
		* TODO(vs) Make different usage preferences configurable.
		*/
		template <typename NavigationMechanicsA, typename NavigationMechanicsB>
		class CombinedPathSearchImplementation : public PathSearch
		{
		public:
			// Takes ownership of the path search configuration object, the pointer is reset during the call
			CombinedPathSearchImplementation(unsigned int taskId, const typename NavigationMechanicsA::WorldModel& modelA, const typename NavigationMechanicsB::WorldModel& modelB,
				std::auto_ptr<PathSearchConfiguration> searchConfig);

			// public NavigationTask interface
			//@{
			virtual process::State getState() const override;
			virtual process::State execute() override;
			virtual void interrupt() override;
			virtual void fail() override;
			virtual void writeResultingPath(const Transform& currentTransform, Path& path) const override;
			virtual std::size_t getNumExpandedSearchStates() const override;
			virtual void createStateDebug(CompoundDebugDrawRequest& cddr) const override;
			//@}

		private:
			// One time initialize function before the actual search is executed for start and goal search states
			bool setupStartAndGoalStates(); // returns whether we need to keep the algorithm running after the algorithm or have already found a solution

			// helper function whether it there is a possibility that we can reach the goal.
			// It does not say that there actually is a path for the entity towards the goal.
			bool isGoalReachable() const;

			// Initialize search states and enter start nodes
			void enterStartNodes(const PathSearchConfiguration& searchConfig);

			// Store the initial radius for transitioning back to A
			void setupStartTransitionRadius(UnsignedFloat radius);

			const std::vector<AreaType>& mAreaTypes; // a bit ugly to keep a reference here

			NavigationMechanicsA mNavigationMechanicsA;
			AStar<NavigationMechanicsA> mSearchOnA;
			NavigationMechanicsB mNavigationMechanicsB;
			AStar<NavigationMechanicsB> mSearchOnB;

			// A transition map defines for each state that was reached by a transition from the other map a combined search connection point configuration that was used to access this state
			// and an identifier for the goal state in the other search space.
			typedef std::map<std::size_t, UsedTransitionData> TransitionMap; // keys are indices of search states
			TransitionMap mTransitionsToA;
			TransitionMap mTransitionsToB;

			// Maps a search state index to a search entry point data set
			typedef boost::container::flat_map<std::size_t, SearchContactPoint> ConnectionList;
			typedef std::pair<typename ConnectionList::const_iterator, typename ConnectionList::const_iterator> ConnectionPair;

			ConnectionList mStartsOnA;
			ConnectionList mStartsOnB;

			// Store a radius limit for the distance towards the start / the goal for which transitions to use
			// This is dynamically tweaked during the search
			UnsignedFloat mGoalTransitionRadiusAToB; // distance around the goal on A where we may transition back to B
			UnsignedFloat mStartTransitionRadiusBToA; // distance around the start on B where we may transition back to A

			UnsignedFloat mTransitionPenalty; // just a copy from the Pathfinding systems configuration value to avoid constant lookup
			UnsignedFloat mMaxCrossingDiamter; // just a copy from the Pathfinding systems configuration value to avoid constant lookup
			bool mStartAndGoalInitialized; // Flag whether the start and goal states have been setup before starting the first search step
			bool mRunning; // Flag whether the search is currently running, used by the interruption mechanism
		};
	}
}

#include "qsf_ai/navigation/pathfinding/CombinedPathSearchImplementation-inl.h"
