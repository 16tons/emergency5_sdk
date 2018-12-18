// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline CachedAsset::~CachedAsset()
	{
		// Nothing to do in here
	}

	inline CachedAsset& CachedAsset::operator=(const CachedAsset& source)
	{
		BaseCachedAsset::operator=(source);

		// Just copy over the content, no need for additional work to be done
		mLocalAssetName			 = source.mLocalAssetName;
		mCachedAssetDataFilename = source.mCachedAssetDataFilename;
		return *this;
	}

	inline CachedAsset& CachedAsset::operator=(const BaseCachedAsset& source)
	{
		BaseCachedAsset::operator=(source);
		return *this;
	}

	inline CachedAsset& CachedAsset::copyPropertiesFrom(const CachedAsset& source)
	{
		// Backup our global asset ID
		const GlobalAssetId globalAssetId = mGlobalAssetId;

		// Call the copy operator
		operator=(source);

		// Now fix the global asset ID
		mGlobalAssetId = globalAssetId;

		return *this;
	}

	inline const std::string& CachedAsset::getLocalAssetName() const
	{
		return mLocalAssetName;
	}

	inline const std::string& CachedAsset::getCachedAssetDataFilename() const
	{
		return mCachedAssetDataFilename;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
