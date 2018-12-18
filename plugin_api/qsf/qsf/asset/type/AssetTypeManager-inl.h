// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline AssetTypeManager::AssetTypeManager()
	{
		// Nothing to do in here
	}

	inline AssetTypeManager::~AssetTypeManager()
	{
		// Nothing to do in here
	}

	inline AssetType* AssetTypeManager::registerAssetType(const std::string& typeName, bool isSyncBeforeMapLoad)
	{
		AssetType* assetType = createElementById(StringHash(typeName));
		if (nullptr != assetType)
		{
			assetType->mTypeName = typeName;
			assetType->mIsSyncBeforeMapLoad = isSyncBeforeMapLoad;
		}
		return assetType;
	}

	inline bool AssetTypeManager::unregisterAssetType(const std::string& typeName)
	{
		return destroyElement(StringHash(typeName));
	}

	inline const AssetTypeManager::AssetTypeMap& AssetTypeManager::getAssetTypeMap() const
	{
		return getElements();
	}

	inline void AssetTypeManager::addAssetTypeDependency(AssetTypeId typeId, AssetTypeId dependencyTypeId)
	{
		AssetType* assetType = findElement(typeId);
		if (nullptr != assetType)
		{
			assetType->addDependencyTypeId(dependencyTypeId);
		}
		else
		{
			QSF_ERROR("AssetTypeManager: Unknown AssetTypeId " << typeId << "!", QSF_REACT_NONE);
		}
	}

	inline void AssetTypeManager::removeAssetTypeDependency(AssetTypeId typeId, AssetTypeId dependencyTypeId)
	{
		AssetType* assetType = findElement(typeId);
		if (nullptr != assetType)
		{
			assetType->removeDependencyTypeId(dependencyTypeId);
		}
		else
		{
			QSF_ERROR("AssetTypeManager: Unknown AssetTypeId " << typeId << "!", QSF_REACT_NONE);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
