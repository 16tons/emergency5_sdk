// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline BaseAsset::~BaseAsset()
	{
		// Nothing to do in here
	}

	inline BaseAsset& BaseAsset::operator =(const BaseAsset& source)
	{
		// Just copy over the content, no need for additional work to be done
		mTypeId			  = source.mTypeId;
		// TODO(ca) Review: Do we really want to copy over the global asset ID? (consider the global asset ID to be 'content'?) If so we might want to add a setter for the global asset ID aswell...
		mGlobalAssetId	  = source.mGlobalAssetId;
		mRevision		  = source.mRevision;
		mTypeName		  = source.mTypeName;
		mCategory		  = source.mCategory;
		mName			  = source.mName;
		mDescription	  = source.mDescription;
		mTags			  = source.mTags;
		mAuthor			  = source.mAuthor;
		mDateCreated	  = source.mDateCreated;
		mDateModified	  = source.mDateModified;
		mHash			  = source.mHash;
		mDependencies	  = source.mDependencies;
		mDerivedFrom	  = source.mDerivedFrom;
		mDeriveranceIndex = source.mDeriveranceIndex;
		mDatasourcePath	  = source.mDatasourcePath;
		return *this;
	}

	inline GlobalAssetId BaseAsset::getGlobalAssetId() const
	{
		return mGlobalAssetId;
	}

	inline AssetRevision BaseAsset::getRevision() const
	{
		return mRevision;
	}

	inline void BaseAsset::setRevision(AssetRevision revision)
	{
		mRevision = revision;
	}

	inline const std::string& BaseAsset::getTypeName() const
	{
		return mTypeName;
	}

	inline AssetTypeId BaseAsset::getTypeId() const
	{
		return mTypeId;
	}

	inline const std::string& BaseAsset::getCategory() const
	{
		return mCategory;
	}

	inline void BaseAsset::setCategory(const std::string& category)
	{
		mCategory = category;
	}

	inline const std::string& BaseAsset::getName() const
	{
		return mName;
	}

	inline void BaseAsset::setName(const std::string& name)
	{
		mName = name;
	}

	inline const std::string& BaseAsset::getDescription() const
	{
		return mDescription;
	}

	inline void BaseAsset::setDescription(const std::string& description)
	{
		mDescription = description;
	}

	inline const std::string& BaseAsset::getTags() const
	{
		return mTags;
	}

	inline void BaseAsset::setTags(const std::string& tags)
	{
		mTags = tags;
	}

	inline const std::string& BaseAsset::getAuthor() const
	{
		return mAuthor;
	}

	inline void BaseAsset::setAuthor(const std::string& author)
	{
		mAuthor = author;
	}

	inline const std::string& BaseAsset::getDateCreated() const
	{
		return mDateCreated;
	}

	inline void BaseAsset::setDateCreated(const std::string& dateCreated)
	{
		mDateCreated = dateCreated;
	}

	inline const std::string& BaseAsset::getDateModified() const
	{
		return mDateModified;
	}

	inline void BaseAsset::setDateModified(const std::string& dateModified)
	{
		mDateModified = dateModified;

		// In case there's no created date, do also set it at once (a modified date without having a created data looks somewhat funny)
		if (mDateCreated.empty())
		{
			mDateCreated = dateModified;
		}
	}

	inline const std::string& BaseAsset::getHash() const
	{
		return mHash;
	}

	inline void BaseAsset::setHash(const std::string& hash)
	{
		mHash = hash;
	}

	inline const std::vector<GlobalAssetId>& BaseAsset::getDependencies() const
	{
		return mDependencies;
	}

	inline void BaseAsset::setDependencies(const std::vector<GlobalAssetId>& dependencies)
	{
		mDependencies = dependencies;
	}

	inline const BaseAsset::DerivedFromSet& BaseAsset::getDerivedFrom() const
	{
		return mDerivedFrom;
	}

	inline void BaseAsset::setDerivedFrom(const DerivedFromSet& derivedFrom)
	{
		mDerivedFrom = derivedFrom;
	}

	inline uint32 BaseAsset::getDeriveranceIndex() const
	{
		return mDeriveranceIndex;
	}

	inline void BaseAsset::setDeriveranceIndex(uint32 deriveranceIndex)
	{
		mDeriveranceIndex = deriveranceIndex;
	}

	inline const std::string& BaseAsset::getDatasourcePath() const
	{
		return mDatasourcePath;
	}

	inline void BaseAsset::setDatasourcePath(const std::string& datasourcePath)
	{
		mDatasourcePath = datasourcePath;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	inline void BaseAsset::setGlobalAssetId(GlobalAssetId globalAssetId)
	{
		mGlobalAssetId = globalAssetId;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
