// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline DecalComponent::DecalComponent(Prototype* prototype) :
		NodesComponent(prototype),
		mTextureAtlasEntriesU(1),
		mTextureAtlasEntriesV(1)
	{
		// Nothing here
	}

	inline DecalComponent::~DecalComponent()
	{
		// Nothing here
	}

	inline const AssetProxy& DecalComponent::getTextureAtlas() const
	{
		return mTextureAtlas;
	}

	inline void DecalComponent::setTextureAtlas(const AssetProxy& textureAtlas)
	{
		assignAndPromoteChange(mTextureAtlas, textureAtlas, TEXTURE_ATLAS);
	}

	inline const size_t DecalComponent::getTextureAtlasEntriesU() const
	{
		return mTextureAtlasEntriesU;
	}

	inline void DecalComponent::setTextureAtlasEntriesU(size_t textureAtlasEntriesU)
	{
		assignAndPromoteChange(mTextureAtlasEntriesU, textureAtlasEntriesU, TEXTURE_ATLAS_ENTRIES_U);
	}

	inline const size_t DecalComponent::getTextureAtlasEntriesV() const
	{
		return mTextureAtlasEntriesV;
	}

	inline void DecalComponent::setTextureAtlasEntriesV(size_t textureAtlasEntriesV)
	{
		assignAndPromoteChange(mTextureAtlasEntriesV, textureAtlasEntriesV, TEXTURE_ATLAS_ENTRIES_V);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::NodesComponent methods         ]
	//[-------------------------------------------------------]
	inline bool DecalComponent::closedContinuousLines() const
	{
		return false;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
