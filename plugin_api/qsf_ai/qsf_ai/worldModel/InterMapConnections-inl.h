// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/GetUninitialized.h>
#include <qsf/serialization/binary/BinarySerializer.h>
#include <qsf/serialization/binary/StlTypeSerialization.h>

namespace qsf
{
	namespace ai
	{
		inline InterMapConnections::InterMapConnections() :
			mGoalMapId(getUninitialized(mGoalMapId)),
			mLowestNewId(0)
		{}

		inline InterMapConnections::InterMapConnections(unsigned int goalMapId) :
			mGoalMapId(goalMapId),
			mLowestNewId(0)
		{}

		inline InterMapConnections::ConnectionRange InterMapConnections::getConnectionsFrom(unsigned int laneId) const
		{
			return mConnections.equal_range(laneId);
		}

		inline const InterMapConnections::ConnectedStates& InterMapConnections::getAllConnections() const
		{
			return mConnections;
		}

		inline unsigned int InterMapConnections::getConnectedMapId() const
		{
			return mGoalMapId;
		}

		inline void InterMapConnections::addConnection(CombinedSearchConnectionPoint& connection)
		{
			connection.mId = fetchUniqueId();

			mConnections.emplace(connection.mStartingWorldElementId, connection);
		}

		inline void InterMapConnections::eraseAllConnectionsFrom(unsigned int worldElementId)
		{
			const auto toBeErased = mConnections.equal_range(worldElementId);
			for (auto iter = toBeErased.first; iter != toBeErased.second; ++iter)
				mTemporarilyUnusedIds.insert(iter->second.mId);

			mConnections.erase(toBeErased.first, toBeErased.second);
		}

		inline unsigned int InterMapConnections::getNumberOfCreatedConnections() const
		{
			return mLowestNewId;
		}
	}

	namespace serialization
	{
		template <>
		struct serializer<ai::InterMapConnections>
		{
			inline static void serialize(BinarySerializer& serializer, ai::InterMapConnections& connections)
			{
				serializer & connections.mGoalMapId;
				serializer & connections.mConnections;
				serializer & connections.mLowestNewId;
				serializer & connections.mTemporarilyUnusedIds;
			}
		};
	}
}
