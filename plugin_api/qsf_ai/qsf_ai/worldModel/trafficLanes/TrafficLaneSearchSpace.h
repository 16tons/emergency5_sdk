// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/worldModel/trafficLanes/TrafficLaneSearchState.h"

#include <set>
#include <vector>

namespace qsf
{
	namespace ai
	{
		/** A special purpose search space for a search on a traffic lane world.
		*/
		class TrafficLaneSearchSpace
		{
		public:
			~TrafficLaneSearchSpace();

			// less operator used when determining equality of traffic lane search states for inserting into the states map
			class TrafficLaneSearchStateLess
			{
			public:
				bool operator() (const TrafficLaneSearchState* lhs, const TrafficLaneSearchState* rhs) const;
			};

			// Retrieves the unique state index from a state
			std::size_t getIndex(const TrafficLaneSearchState& state) const;

			std::size_t getNumExpandedSearchStates() const;

			// Retrieves or creates a fitting state with these values
			TrafficLaneSearchState& getOrCreateState(unsigned int nodeId, unsigned int laneId, unsigned int segmentId,
				const glm::vec3& position, const glm::vec3& direction, bool forward, bool isGoal, const CombinedSearchConnectionPoint* transition = nullptr);
			// Retrieves a state that was previously created and throws an exception if this is not the case.
			TrafficLaneSearchState& getState(unsigned int nodeId, unsigned int laneId, unsigned int segmentId,
				const glm::vec3& position, const glm::vec3& direction, bool forward, bool isGoal, const CombinedSearchConnectionPoint* transition = nullptr) const;
			// Retrieves a state that needs to be created before by its unique index
			TrafficLaneSearchState& getStateByUniqueIndex(std::size_t stateIndex) const;

			// Retrieve the state with the lowest expected cost to the target
			TrafficLaneSearchState* getStateWithLowestRemainingExpectedCosts() const;
		private:
			// Creating a simple multi-indexed access to the states for different purposes here
			// TODO(vs) See if we can use boost::multi-Index here?
			typedef std::set<TrafficLaneSearchState*, TrafficLaneSearchStateLess> StateMap;
			StateMap mStatesBySemanticEquality;
			typedef std::vector<TrafficLaneSearchState*> StateArray;
			StateArray mStatesByUniqueIndex;
		};
	}
}

#include "qsf_ai/worldModel/trafficLanes/TrafficLaneSearchSpace-inl.h"
