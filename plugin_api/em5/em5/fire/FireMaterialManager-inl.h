// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline bool FireMaterialManager::getContentHasChanged() const
	{
		return mContentHasChanged;
	}

	inline void FireMaterialManager::setContentHasChanged(bool contentHasChanged)
	{
		mContentHasChanged = contentHasChanged;
	}

	// TODO(sw) remove the qsf::AssetRevision parameter as soon as this information can be gathered inside the manager
	inline void FireMaterialManager::setIsEditing(bool isEditing, qsf::AssetRevision assetRevision)
	{
		mIsEditing = isEditing;
		mAssetRevision = assetRevision;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
