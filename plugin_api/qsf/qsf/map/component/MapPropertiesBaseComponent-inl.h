// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline MapPropertiesBaseComponent::~MapPropertiesBaseComponent()
	{
		// Nothing to do in here
	}

	inline const std::string& MapPropertiesBaseComponent::getMapName() const
	{
		return mMapName;
	}

	inline void MapPropertiesBaseComponent::setMapName(const std::string& mapName)
	{
		assignAndPromoteChange(mMapName, mapName, MAP_NAME);
	}

	inline const AssetProxy& MapPropertiesBaseComponent::getMiniMap() const
	{
		return mMiniMap;
	}

	inline void MapPropertiesBaseComponent::setMiniMap(const AssetProxy& miniMap)
	{
		assignAndPromoteChange(mMiniMap, miniMap, MINI_MAP);
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	inline MapPropertiesBaseComponent::MapPropertiesBaseComponent(Prototype* prototype) :
		Component(prototype),
		mMapName("unnamed"),
		mMiniMap("qsf/texture/default/missing")
	{
		// Nothing to do in here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
