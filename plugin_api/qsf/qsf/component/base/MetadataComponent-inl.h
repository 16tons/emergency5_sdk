// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline const AssetProxy& MetadataComponent::getBasePrefab() const
	{
		return mBasePrefab;
	}

	inline void MetadataComponent::setBasePrefab(const AssetProxy& prefab)
	{
		assignAndPromoteChange(mBasePrefab, prefab, PREFAB);
	}

	inline uint64 MetadataComponent::getBasePrototypeId() const
	{
		return mBasePrototypeId;
	}

	inline void MetadataComponent::setBasePrototypeId(uint64 prototypeId)
	{
		assignAndPromoteChange(mBasePrototypeId, prototypeId, PROTOTYPE);
	}

	inline uint64 MetadataComponent::getSourcePrototypeId() const
	{
		return mSourcePrototypeId;
	}

	inline void MetadataComponent::setSourcePrototypeId(uint64 prototypeId)
	{
		mSourcePrototypeId = prototypeId;
	}

	inline const std::string& MetadataComponent::getName() const
	{
		return mName;
	}

	inline void MetadataComponent::setName(const std::string& name)
	{
		assignAndPromoteChange(mName, name, NAME);
	}

	inline const std::string& MetadataComponent::getDescription() const
	{
		return mDescription;
	}

	inline void MetadataComponent::setDescription(const std::string& description)
	{
		assignAndPromoteChange(mDescription, description, DESCRIPTION);
	}

	inline const std::string& MetadataComponent::getTags() const
	{
		return mTags;
	}

	inline void MetadataComponent::setTags(const std::string& tags)
	{
		assignAndPromoteChange(mTags, tags, TAGS);
	}

	inline uint32 MetadataComponent::getLayerId() const
	{
		return mLayerId;
	}

	inline MetadataComponent::Quality MetadataComponent::getQuality() const
	{
		return mQuality;
	}

	inline void MetadataComponent::setQuality(Quality quality)
	{
		assignAndPromoteChange(mQuality, quality, QUALITY);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
