// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/GetUninitialized.h"
#include "qsf/base/error/BoostAssertHandler.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline const AssetSystem::AssetDataByGlobalAssetIdMap& AssetSystem::getAssetDataByGlobalAssetIdMap() const
	{
		return mAssetDataByGlobalAssetId;
	}

	inline AssetTypeManager& AssetSystem::getAssetTypeManager()
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mAssetTypeManager, "The asset type manager instance is invalid", QSF_REACT_THROW);
		return *mAssetTypeManager;
	}

	inline const AssetTypeManager& AssetSystem::getAssetTypeManager() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mAssetTypeManager, "The asset type manager instance is invalid", QSF_REACT_THROW);
		return *mAssetTypeManager;
	}

	inline ProjectManager& AssetSystem::getProjectManager()
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mProjectManager, "The project manager instance is invalid", QSF_REACT_THROW);
		return *mProjectManager;
	}

	inline const ProjectManager& AssetSystem::getProjectManager() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mProjectManager, "The project manager instance is invalid", QSF_REACT_THROW);
		return *mProjectManager;
	}

	inline GlobalAssetId AssetSystem::mapLocalToGlobalAssetId(LocalAssetId localAssetId) const
	{
		return getAssetDataById(mAssetDataByLocalId, localAssetId).mGlobalAssetId;
	}

	inline LocalAssetId AssetSystem::mapGlobalToLocalAssetId(GlobalAssetId globalAssetId) const
	{
		return getAssetDataById(mAssetDataByGlobalAssetId, globalAssetId).mLocalAssetId;
	}

	inline AssetPackage* AssetSystem::mapGlobalAssetIdToAssetPackage(GlobalAssetId globalAssetId) const
	{
		return getAssetDataById(mAssetDataByGlobalAssetId, globalAssetId).mAssetPackage;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::System methods                    ]
	//[-------------------------------------------------------]
	inline const char* AssetSystem::getName() const
	{
		return "Asset system";
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	template<typename IdType>
	AssetSystem::AssetData& AssetSystem::getAssetDataById(const std::unordered_map<IdType, AssetDataList>& dataArrayMap, IdType id) const
	{
		static AssetData defaultAssetData(getUninitialized<uint64>(), getUninitialized<uint32>(), nullptr);

		const typename std::unordered_map<IdType, AssetDataList>::const_iterator mapIterator = dataArrayMap.find(id);
		return (dataArrayMap.cend() != mapIterator) ? *mapIterator->second.front() : defaultAssetData;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
