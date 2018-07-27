// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/serialization/binary/StlTypeSerialization.h>

namespace qsf
{
	namespace ai
	{
		inline WorldModel::WorldModel(unsigned int id) :
			mId(id),
			mGlobalAssetId(getUninitialized(mGlobalAssetId))
		{}

		inline WorldModel::~WorldModel()
		{}

		inline unsigned int WorldModel::getId() const
		{
			return mId;
		}

		inline const std::string& WorldModel::getName() const
		{
			return mName;
		}

		inline void WorldModel::setName(const std::string& name)
		{
			mName = name;
		}

		inline GlobalAssetId WorldModel::getGlobalAssetId() const
		{
			return mGlobalAssetId;
		}

		inline void WorldModel::setGlobalAssetId(GlobalAssetId globalAssetId)
		{
			mGlobalAssetId = globalAssetId;
		}

		inline const WorldModel::MapConnections& WorldModel::getAllInterMapConnections() const
		{
			return mMapConnections;
		}

		inline const InterMapConnections* WorldModel::tryGetInterMapConnections(unsigned int mapId) const
		{
			for (std::size_t counter = 0; counter < mMapConnections.size(); ++counter)
				if (mMapConnections[counter].getConnectedMapId() == mapId)
					return &mMapConnections[counter];

			return nullptr;
		}

		inline InterMapConnections* WorldModel::tryGetInterMapConnections(unsigned int mapId)
		{
			for (std::size_t counter = 0; counter < mMapConnections.size(); ++counter)
				if (mMapConnections[counter].getConnectedMapId() == mapId)
					return &mMapConnections[counter];

			return nullptr;
		}

		inline void WorldModel::prepareInterMapConnections(unsigned int mapId)
		{
			const InterMapConnections emptyConnections(mapId);
			setInterMapConnections(emptyConnections);
		}

		inline void WorldModel::onLoaded(std::vector<NavigationTask*>& taskList)
		{
			// by default don't require any post processing
		}
	}
}
