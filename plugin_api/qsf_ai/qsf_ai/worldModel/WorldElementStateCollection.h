// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/worldModel/WorldElementState.h"
#include "qsf_ai/worldModel/dynamicUpdate/ObstructingCollision.h"

#include <boost/noncopyable.hpp>
#include <boost/container/flat_map.hpp>

#include <vector>
#include <map>


namespace qsf
{
	namespace ai
	{
		class NavigationElementConnector;

		/**
		* An optional extension of a world description that is made up of individual elements that confirm to the conditions described in the WorldElementState comment.
		* If a ai navigation map has that kind of structure it should provide an instance of this extension.
		* It defines the state for nodes and areas of the world.
		*/
		class WorldElementStateCollection : public boost::noncopyable
		{
		public:
			friend struct serialization::serializer<ai::WorldElementStateCollection>;

			WorldElementStateCollection(); // Creates empty structures
			WorldElementStateCollection(unsigned int numAreas, unsigned int numNodes);

			// Dynamically resize for the number of elements passed.
			// This never reduces the number as even after shrinking due to a dynamic update, there might be older paths that still rely on the old number of elements.
			void resize(unsigned int numAreas, unsigned int numNodes);

			// Area state access
			//@{
			const worldElement::State& getAreaState(unsigned int area) const;
			void setAreaState(unsigned int area, const worldElement::State& state);
			//@}

			// Node state access
			//@{
			const worldElement::State& getNodeState(unsigned int node) const;
			void setNodeState(unsigned int node, const worldElement::State& state);
			//@}

			// Element connectors access
			//@{
			void registerElementConnector(NavigationElementConnector& connector);
			void unregisterElementConnector(NavigationElementConnector& connector);
			void clearAllElementConnectors();
			NavigationElementConnector* tryGetConnectorForArea(unsigned int area) const;
			NavigationElementConnector* tryGetConnectorForNode(unsigned int node) const;
			//@}

			// Obstructing collisions access
			//@{
			typedef std::multimap<unsigned int, ObstructingCollision> CollisionMap; // Obstructing collision are stored with the area id acting as key
			typedef std::pair<CollisionMap::const_iterator, CollisionMap::const_iterator> CollisionRange; // A range of collisions for one area
			void addObstructingCollision(unsigned int area, const ObstructingCollision& collision);
			void clearObstructingCollision(unsigned int area, uint64 entityId);
			CollisionRange getObstructingCollisions(unsigned int area) const; // use like the result of an equal_range call to a multimap
			//@}

			std::size_t calculateMemoryConsumption() const;

		private:
			// The core state data for each area and node of the world structure, with the index being the id of the element
			std::vector<worldElement::State> mAreaStates;
			std::vector<worldElement::State> mNodeStates;

			// Listener components connected to areas and nodes with the id acting as key
			boost::container::flat_map<unsigned int, NavigationElementConnector*> mAreaConnectors;
			boost::container::flat_map<unsigned int, NavigationElementConnector*> mNodeConnectors;

			// Obstruction information for each area with the id acting as key
			CollisionMap mObstructingCollisions;
		};
	}
}

#include "qsf_ai/worldModel/WorldElementStateCollection-inl.h"
