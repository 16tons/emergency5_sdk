// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline const std::string& AssetType::getTypeName() const
	{
		return mTypeName;
	}

	inline AssetTypeId AssetType::getTypeId() const
	{
		return mTypeId;
	}

	inline const boost::container::flat_set<AssetTypeId>& AssetType::getDependencyTypeIdList() const
	{
		return mDependencyTypeIdList;
	}

	inline bool AssetType::isSyncBeforeMapLoad() const
	{
		return mIsSyncBeforeMapLoad;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	inline AssetType::AssetType(const StringHash& typeId) :
		mTypeId(typeId),
		mIsSyncBeforeMapLoad(true)
	{
		// Nothing to do in here
	}

	inline AssetType::~AssetType()
	{
		// Nothing to do in here
	}

	inline void AssetType::addDependencyTypeId(AssetTypeId typeId)
	{
		mDependencyTypeIdList.insert(typeId);
	}

	inline void AssetType::removeDependencyTypeId(AssetTypeId typeId)
	{
		mDependencyTypeIdList.erase(typeId);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
