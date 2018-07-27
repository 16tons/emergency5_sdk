// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline AssetPackage::~AssetPackage()
	{
		// Clear the asset package
		clear();
	}

	inline Project& AssetPackage::getProject() const
	{
		return mProject;
	}

	inline bool AssetPackage::isIntermediate() const
	{
		return mIntermediate;
	}

	inline std::string AssetPackage::getFilename() const
	{
		return mFilename;
	}

	inline void AssetPackage::setFilename(const std::string& filename)
	{
		mFilename = filename;
	}

	inline AssetPackage::AssetManager& AssetPackage::getAssetManager()
	{
		return mAssetManager;
	}

	inline const AssetPackage::AssetManager& AssetPackage::getAssetManager() const
	{
		return mAssetManager;
	}

	inline Asset* AssetPackage::getAssetByGlobalAssetId(GlobalAssetId globalAssetId) const
	{
		const AssetMap& assetMap = mAssetManager.getElements();
		AssetMap::const_iterator assetIterator = assetMap.find(globalAssetId);
		return (assetIterator != assetMap.cend()) ? assetIterator->second : nullptr;
	}

	inline AssetPackage::CachedAssetManager& AssetPackage::getCachedAssetManager()
	{
		return mCachedAssetManager;
	}

	inline const AssetPackage::CachedAssetManager& AssetPackage::getCachedAssetManager() const
	{
		return mCachedAssetManager;
	}

	inline CachedAsset* AssetPackage::getCachedAssetByGlobalAssetId(GlobalAssetId globalAssetId) const
	{
		const CachedAssetMap& cachedAssetMap = mCachedAssetManager.getElements();
		CachedAssetMap::const_iterator cachedAssetIterator = cachedAssetMap.find(globalAssetId);
		return (cachedAssetIterator != cachedAssetMap.cend()) ? cachedAssetIterator->second : nullptr;
	}

	inline bool AssetPackage::isMounted() const
	{
		return mMounted;
	}

	inline void AssetPackage::mountAssetByGlobalAssetId(GlobalAssetId globalAssetId)
	{
		// Is the asset package currently mounted?
		if (mMounted)
		{
			// Get the asset ID instance...
			Asset* asset = getAssetByGlobalAssetId(globalAssetId);
			if (nullptr != asset)
			{
				// ... and mount it
				mountAssetAndSignal(*asset);
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
