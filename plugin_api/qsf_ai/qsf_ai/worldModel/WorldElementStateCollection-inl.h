// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/worldModel/trafficControl/NavigationElementConnector.h"

#include <qsf/base/error/ErrorHandling.h>
#include <qsf/serialization/binary/BinarySerializer.h>
#include <qsf/serialization/binary/StlTypeSerialization.h>


namespace qsf
{
	namespace ai
	{
		inline WorldElementStateCollection::WorldElementStateCollection()
		{}

		inline WorldElementStateCollection::WorldElementStateCollection(unsigned int numAreas, unsigned int numNodes) :
			mAreaStates(numAreas),
			mNodeStates(numNodes)
		{}

		inline const worldElement::State& WorldElementStateCollection::getAreaState(unsigned int area) const
		{
			QSF_CHECK(area < mAreaStates.size(), "index " << area << " out of bounds when getting world area status",
				QSF_REACT_THROW);

			return mAreaStates[area];
		}

		inline void WorldElementStateCollection::setAreaState(unsigned int area, const worldElement::State& state)
		{
			QSF_CHECK(area < mAreaStates.size(), "index " << area << " out of bounds when setting world area status",
				QSF_REACT_THROW);

			mAreaStates[area] = state;
		}

		inline const worldElement::State& WorldElementStateCollection::getNodeState(unsigned int node) const
		{
			QSF_CHECK(node < mNodeStates.size(), "index " << node << " out of bounds when accessing world node status",
				QSF_REACT_THROW);

			return mNodeStates[node];
		}

		inline void WorldElementStateCollection::setNodeState(unsigned int node, const worldElement::State& state)
		{
			QSF_CHECK(node < mNodeStates.size(), "index " << node << " out of bounds when accessing world node status",
				QSF_REACT_THROW);

			mNodeStates[node] = state;
		}

		inline void WorldElementStateCollection::registerElementConnector(NavigationElementConnector& connector)
		{
			auto& container = connector.managesArea() ? mAreaConnectors : mNodeConnectors;

			const auto insertRestult = container.emplace(connector.getManagedElementAiId(), &connector);
			QSF_CHECK(insertRestult.second, "Trying to add two navigation element connectors for " << (connector.managesArea() ? " area " : "node ") <<
				connector.getManagedElementAiId() << " in nav map " << connector.getWorldId(),
				QSF_REACT_NONE);
			// TODO(vs) set bool init successful to the component for increased safety
		}

		inline void WorldElementStateCollection::unregisterElementConnector(NavigationElementConnector& connector)
		{
			auto& container = connector.managesArea() ? mAreaConnectors : mNodeConnectors;

			const auto registeredEntry = container.find(connector.getManagedElementAiId());
			if (registeredEntry == container.end() || registeredEntry->second != &connector)
				return; // not registered

			container.erase(registeredEntry);
		}

		inline NavigationElementConnector* WorldElementStateCollection::tryGetConnectorForArea(unsigned int area) const
		{
			auto registered = mAreaConnectors.find(area);
			return registered != mAreaConnectors.end() ? registered->second : nullptr;
		}

		inline NavigationElementConnector* WorldElementStateCollection::tryGetConnectorForNode(unsigned int node) const
		{
			auto registered = mNodeConnectors.find(node);
			return registered != mNodeConnectors.end() ? registered->second : nullptr;
		}

		inline void WorldElementStateCollection::clearAllElementConnectors()
		{
			mAreaConnectors.clear();
			mNodeConnectors.clear();
		}

		inline void WorldElementStateCollection::addObstructingCollision(unsigned int area, const ObstructingCollision& collision)
		{
			mObstructingCollisions.emplace(area, collision);
		}

		inline void WorldElementStateCollection::clearObstructingCollision(unsigned int area, uint64 entityId)
		{
			// Currently not issuing a warning if we want to remove something that isn't there.
			// The add task could have been interrupted and this is currently not tracked.
			const CollisionRange allAreaObstructers = getObstructingCollisions(area);
			for (auto iter = allAreaObstructers.first; iter != allAreaObstructers.second;) // nonstandard increment inside the body because of possible erase operations
			{
				if (iter->second.mEntityId == entityId)
					iter = mObstructingCollisions.erase(iter);
				else
					++iter;
			}
		}

		inline WorldElementStateCollection::CollisionRange WorldElementStateCollection::getObstructingCollisions(unsigned int area) const
		{
			return mObstructingCollisions.equal_range(area);
		}
	}

	namespace serialization
	{
		template <>
		struct serializer<ai::WorldElementStateCollection>
		{
			inline static void serialize(BinarySerializer& serializer, ai::WorldElementStateCollection& data)
			{
				serializer & data.mAreaStates;
				serializer & data.mNodeStates;
				// The connector maps are not serialized since this information is static and recreated during the startup phase
				serializer & data.mObstructingCollisions;
			}
		};
	}
}
