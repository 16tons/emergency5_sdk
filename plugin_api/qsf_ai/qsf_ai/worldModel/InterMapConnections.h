// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/navigation/pathfinding/CombinedSearchConnectionPoint.h"

#include <map>
#include <vector>


namespace qsf
{
	namespace ai
	{
		/**
		* An InterMapConnections object holds data about where one AI maps is connected to another one.
		* This is usually used during navigation algorithms when the entity may move along more than one map and is allowed to switch between them.
		*/
		class InterMapConnections
		{
		public:
			friend class std::allocator<InterMapConnections>;
			friend struct serialization::serializer<InterMapConnections>;

			typedef std::multimap<unsigned int, CombinedSearchConnectionPoint> ConnectedStates; // lane id acting as key
			typedef std::pair<typename ConnectedStates::const_iterator, typename ConnectedStates::const_iterator> ConnectionRange; // Connections are returned as a pair of iterators

			// Pass the unique id of the map we are connected to here
			InterMapConnections(unsigned int goalMapId);

			unsigned int getConnectedMapId() const;

			// Query the outgoing connections for a specific lane.
			// Returns an iterator range that may be empty.
			// Use it like a regular std::multimap equal_range result.
			ConnectionRange getConnectionsFrom(unsigned int laneId) const;

			// Returns all the outgoing connections
			const ConnectedStates& getAllConnections() const;

			// Add a number of connections - may sort and thereby change the passed deque by design as well as assign a unique id
			void addConnections(std::deque<CombinedSearchConnectionPoint>& connections);
			void addConnection(CombinedSearchConnectionPoint& connection);

			void eraseAllConnectionsFrom(unsigned int worldElementId);
			void eraseAllConnectionsTo(unsigned int worldElementId);

			// Returns the next free id for a connection to be created.
			unsigned int getNumberOfCreatedConnections() const;

		private:
			InterMapConnections(); // dummy default constructor only needed for using it by binary serialization

			// this is used to assign a unique id for each added transition
			unsigned int fetchUniqueId();

			unsigned int mGoalMapId;
			ConnectedStates mConnections;

			// unique id tracking
			unsigned int mLowestNewId;
			std::set<unsigned int> mTemporarilyUnusedIds; // These were used once but were removed afterwards. Prefer to reuse these instead of fetching a new number
		};
	}
}

#include "qsf_ai/worldModel/InterMapConnections-inl.h"
