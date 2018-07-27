// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline AssetProxy::AssetProxy(const AssetProxy& assetProxy) :
		mGlobalAssetId(assetProxy.mGlobalAssetId),
		mLocalAssetName(assetProxy.mLocalAssetName),
		mLocalAssetId(assetProxy.mLocalAssetId),
		mCachedAssetDataFilename(assetProxy.mCachedAssetDataFilename)
	{
		// Nothing to do in here
	}

	inline AssetProxy::~AssetProxy()
	{
		// Nothing to do in here
	}

	inline void AssetProxy::clear()
	{
		setUninitialized(mGlobalAssetId);
		setUninitialized(mLocalAssetId);
		mLocalAssetName = mCachedAssetDataFilename = "";
	}

	inline bool AssetProxy::operator ==(const AssetProxy& assetProxy) const
	{
		return (getGlobalAssetId() == assetProxy.getGlobalAssetId() && getLocalAssetId() == assetProxy.getLocalAssetId());
	}

	inline bool AssetProxy::operator !=(const AssetProxy& assetProxy) const
	{
		return (getGlobalAssetId() != assetProxy.getGlobalAssetId() || getLocalAssetId() != assetProxy.getLocalAssetId());
	}

	inline AssetProxy::operator const std::string&() const
	{
		return getLocalAssetName();
	}

	inline bool AssetProxy::isLocalAssetIdInitialized() const
	{
		return isInitialized(mLocalAssetId);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
