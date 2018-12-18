// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <boost/lexical_cast.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline BaseCachedAsset::~BaseCachedAsset()
	{
		// Nothing to do in here
	}

	inline BaseCachedAsset& BaseCachedAsset::operator=(const BaseCachedAsset& source)
	{
		// Just copy over the content, no need for additional work to be done
		// TODO(ca) Review: Do we really want to copy over the global asset ID? (consider the global asset ID to be 'content'?) If so we might want to add a setter for the global asset ID aswell...
		mGlobalAssetId			 = source.mGlobalAssetId;
		mRevision				 = source.mRevision;
		mType					 = source.mType;
		mHash					 = source.mHash;
		mDerivedAssetMap		 = source.mDerivedAssetMap;
		mDynamicPropertyMap		 = source.mDynamicPropertyMap;
		return *this;
	}

	inline GlobalAssetId BaseCachedAsset::getGlobalAssetId() const
	{
		return mGlobalAssetId;
	}

	inline AssetRevision BaseCachedAsset::getRevision() const
	{
		return mRevision;
	}

	inline void BaseCachedAsset::setRevision(AssetRevision revision)
	{
		mRevision = revision;
	}

	inline const std::string& BaseCachedAsset::getType() const
	{
		return mType;
	}

	inline void BaseCachedAsset::setType(const std::string& type)
	{
		mType = type;
	}

	inline const std::string& BaseCachedAsset::getHash() const
	{
		return mHash;
	}

	inline void BaseCachedAsset::setHash(const std::string& hash)
	{
		mHash = hash;
	}

	inline BaseCachedAsset::DerivedAssetMap& BaseCachedAsset::getDerivedAssetMap()
	{
		return mDerivedAssetMap;
	}

	inline const BaseCachedAsset::DerivedAssetMap& BaseCachedAsset::getDerivedAssetMap() const
	{
		return mDerivedAssetMap;
	}

	inline const BaseCachedAsset::DynamicPropertyMap& BaseCachedAsset::getDynamicPropertyMap() const
	{
		return mDynamicPropertyMap;
	}

	inline bool BaseCachedAsset::hasDynamicPropertyValue(const NamedIdentifier& propertyName) const
	{
		DynamicPropertyMap::const_iterator iterator = mDynamicPropertyMap.find(propertyName);
		return (iterator != mDynamicPropertyMap.cend());
	}

	inline std::string BaseCachedAsset::getDynamicPropertyValueString(const NamedIdentifier& propertyName, const std::string& defaultValue) const
	{
		DynamicPropertyMap::const_iterator iterator = mDynamicPropertyMap.find(propertyName);
		return (iterator != mDynamicPropertyMap.cend() ? iterator->second : defaultValue);
	}

	inline void BaseCachedAsset::setDynamicPropertyValueString(const NamedIdentifier& propertyName, const std::string& propertyValue)
	{
		mDynamicPropertyMap[propertyName] = propertyValue;
	}

	template <typename T>
	T BaseCachedAsset::getDynamicPropertyValue(const NamedIdentifier& propertyName) const
	{
		DynamicPropertyMap::const_iterator iterator = mDynamicPropertyMap.find(propertyName);
		if (iterator != mDynamicPropertyMap.cend())
		{
			return boost::lexical_cast<T>(iterator->second);
		}
		else
		{
			QSF_ERROR("QSF cached asset " << mGlobalAssetId << " has no dynamic asset property \"" << propertyName << '\"', QSF_REACT_THROW);
		}
	}

	template <typename T>
	T BaseCachedAsset::getDynamicPropertyValueSafe(const NamedIdentifier& propertyName, const T& defaultValue) const
	{
		try
		{
			DynamicPropertyMap::const_iterator iterator = mDynamicPropertyMap.find(propertyName);
			return (iterator != mDynamicPropertyMap.cend() ? boost::lexical_cast<T>(iterator->second) : defaultValue);
		}
		catch (const std::exception&)
		{
			// Just return the given default value
			return defaultValue;
		}
	}

	template <typename T>
	void BaseCachedAsset::setDynamicPropertyValue(const NamedIdentifier& propertyName, T propertyValue)
	{
		mDynamicPropertyMap[propertyName] = std::to_string(propertyValue);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	inline void BaseCachedAsset::setGlobalAssetId(GlobalAssetId globalAssetId)
	{
		mGlobalAssetId = globalAssetId;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
