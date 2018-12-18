// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		bool AssetStorageManager::isRunning() const
		{
			return mIsRunning;
		}

		bool AssetStorageManager::isAssetDbLite() const
		{
			return true;
		}

		void AssetStorageManager::freeGlobalAssetId(GlobalAssetId globalAssetId)
		{
			mUnusedGlobalAssetIds.insert(globalAssetId);
		}

		void AssetStorageManager::freeEntityId(uint64 entityId)
		{
			mUnusedEntityIds.insert(entityId);
		}

		std::string AssetStorageManager::getTempTransferPath(GlobalAssetId globalAssetId) const
		{
			return TEMP_TRANSFER_DIRECTORY + '/' + std::to_string(globalAssetId);
		}

		std::string AssetStorageManager::getTempSourceAssetDirectoryPath(GlobalAssetId globalAssetId, AssetRevision revision) const
		{
			return TEMP_SOURCE_ASSET_DIRECTORY + '/' + std::to_string(globalAssetId) + '_' + std::to_string(revision);
		}

		std::string AssetStorageManager::getCacheSourceAssetZipPath(GlobalAssetId globalAssetId, AssetRevision revision) const
		{
			return CACHE_SOURCE_ZIP_DIRECTORY + '/' + std::to_string(globalAssetId) + '_' + std::to_string(revision) + ".zip";
		}

		void AssetStorageManager::downloadAssetSource(GlobalAssetId globalAssetId, AssetRevision revision)
		{
			downloadAssetSource(globalAssetId, revision, false);
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
