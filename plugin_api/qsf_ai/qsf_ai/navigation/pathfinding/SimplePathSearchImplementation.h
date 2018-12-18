// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/navigation/pathfinding/PathSearch.h"
#include "qsf_ai/navigation/pathfinding/SearchContactPoint.h"
#include "qsf_ai/algorithm/AStar.h"

#include <qsf/base/manager/FastPodAllocator.h>

#include <boost/optional.hpp>
#include <boost/container/flat_map.hpp>

namespace qsf
{
	namespace ai
	{
		/** Templated implementation of a simple running path search on a world model passed as template parameter.
		* It is technically only a thin layer around the general AStar with the world model template parameter acting as search mechanics.
		*/
		template <typename NavigationMechanics>
		class SimplePathSearchImplementation : public PathSearch, public FastPodAllocator<SimplePathSearchImplementation<NavigationMechanics>>
		{
		public:
			// Takes ownership on the passed path search configuration, the pointer is reset during this call
			SimplePathSearchImplementation(unsigned int taskId, const typename NavigationMechanics::WorldModel& model, std::auto_ptr<PathSearchConfiguration> searchConfig);

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
			void setupStartAndGoalStates();

			// helper function whether it there is a possibility that we can reach the goal.
			// It does not say that there actually is a path for the entity towards the goal.
			bool isGoalReachable() const;

			typedef boost::container::flat_map<std::size_t, SearchContactPoint> ConnectionList; // the search state index is used as key to a connected entry point data structure
			typedef std::pair<typename ConnectionList::const_iterator, typename ConnectionList::const_iterator> ConnectionPair;

			NavigationMechanics mNavigationMechanics;
			AStar<NavigationMechanics> mSearchState;

			ConnectionList mStartEntryPoints; // Store the exact entry configurations for search start points here
			bool mStartAndGoalInitialized; // Flag whether the start and goal states have been setup before starting the first search step
			bool mRunning; // Flag whether the search is currently running, used by the interruption mechanism
		};
	}
}

#include "qsf_ai/navigation/pathfinding/SimplePathSearchImplementation-inl.h"
