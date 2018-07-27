// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline BaseAssetPackage::BaseAssetPackage() :
		mIsManaged(true)
	{
		// Nothing to do in here
	}

	inline BaseAssetPackage::BaseAssetPackage(const BaseAssetPackage& source) :
		mName(source.mName),
		mDescription(source.mDescription),
		mMountPoint(source.mMountPoint),
		mIsManaged(source.mIsManaged)
	{
		// Nothing to do in here
	}

	inline BaseAssetPackage::~BaseAssetPackage()
	{
		// Nothing to be done in here
	}

	inline const std::string& BaseAssetPackage::getName() const
	{
		return mName;
	}

	inline void BaseAssetPackage::setName(const std::string& name)
	{
		mName = name;
	}

	inline const std::string& BaseAssetPackage::getDescription() const
	{
		return mDescription;
	}

	inline void BaseAssetPackage::setDescription(const std::string& description)
	{
		mDescription = description;
	}

	inline const std::string& BaseAssetPackage::getMountPoint() const
	{
		return mMountPoint;
	}

	inline void BaseAssetPackage::setMountPoint(const std::string& mountPoint)
	{
		mMountPoint = mountPoint;
	}

	inline bool BaseAssetPackage::isManaged() const
	{
		return mIsManaged;
	}

	inline void BaseAssetPackage::setManaged(bool isMounted)
	{
		mIsManaged = isMounted;
	}

	inline bool BaseAssetPackage::isGameModification() const
	{
		return false;
	}

	inline bool BaseAssetPackage::isMultiplayerAuthenticated() const
	{
		return true;
	}

	inline BaseAssetPackage& BaseAssetPackage::operator=(const BaseAssetPackage& source)
	{
		if (&source != this)
		{
			mName = source.mName;
			mDescription = source.mDescription;
			mMountPoint = source.mMountPoint;
			mIsManaged = source.mIsManaged;
		}
		return *this;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
