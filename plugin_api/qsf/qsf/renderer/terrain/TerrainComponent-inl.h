// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline TerrainComponent::TerrainComponent(Prototype* prototype) :
		RendererComponent(prototype),
		// Connected to the CAMP reflection system
		mHeightMapSize(1025),
		mColorMapSize(1024),
		mBlendMapSize(1024),
		mTerrainChunksPerEdge(16),
		mTerrainWorldSize(1500.0f),
		mSkirtSize(2.0f),
		mMaxPixelError(8.0f),
		// Internal only
		mOgreTerrainGlobalOptions(nullptr),
		mOgreTerrainGroup(nullptr),
		mTerrainDefinition(nullptr),
		mIsEditing(false)
	{
		// Nothing to do in here
	}

	inline uint32 TerrainComponent::getHeightMapSize() const
	{
		return mHeightMapSize;
	}

	inline uint32 TerrainComponent::getColorMapSize() const
	{
		return mColorMapSize;
	}

	inline uint32 TerrainComponent::getBlendMapSize() const
	{
		return mBlendMapSize;
	}

	inline int TerrainComponent::getTerrainChunksPerEdge() const
	{
		return mTerrainChunksPerEdge;
	}

	inline float TerrainComponent::getTerrainWorldSize() const
	{
		return mTerrainWorldSize;
	}

	inline float TerrainComponent::getSkirtSize() const
	{
		return mSkirtSize;
	}

	inline float TerrainComponent::getMaxPixelError() const
	{
		return mMaxPixelError;
	}

	inline Ogre::TerrainGlobalOptions* TerrainComponent::getTerrainGlobals() const
	{
		return mOgreTerrainGlobalOptions;
	}

	inline Ogre::TerrainGroup* TerrainComponent::getOgreTerrainGroup() const
	{
		return mOgreTerrainGroup;
	}

	inline const AssetProxy& TerrainComponent::getTerrainAsset() const
	{
		return mTerrainAsset;
	}

	inline bool TerrainComponent::getEditing() const
	{
		return mIsEditing;
	}

	inline const TerrainDefinition* TerrainComponent::getTerrainDefinition() const
	{
		return mTerrainDefinition;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
