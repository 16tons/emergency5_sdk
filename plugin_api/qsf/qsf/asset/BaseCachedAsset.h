// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/asset/AssetSystemTypes.h"
#include "qsf/base/flyweight/FlyweightString.h"
#include "qsf/reflection/object/Object.h"

#include <boost/container/flat_map.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Cached asset class
	*/
	class QSF_API_EXPORT BaseCachedAsset : public Object
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef boost::container::flat_map<uint32, GlobalAssetId> DerivedAssetMap;	///< An indexed list of global asset ids of the assets that have been derived from this asset
		typedef boost::container::flat_map<NamedIdentifier, std::string> DynamicPropertyMap;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] globalAssetId
		*    The global asset identifier
		*/
		explicit BaseCachedAsset(GlobalAssetId globalAssetId);

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Not virtual by intent
		*/
		inline ~BaseCachedAsset();

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] source
		*    Source to copy from
		*
		*  @return
		*    Reference to this base asset
		*/
		inline BaseCachedAsset& operator=(const BaseCachedAsset& source);

		//[-------------------------------------------------------]
		//[ Properties                                            ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the global asset identifier of the cached asset
		*
		*  @return
		*    The global asset identifier of the cached asset
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline GlobalAssetId getGlobalAssetId() const;

		/**
		*  @brief
		*    Return the asset revision of the cached asset
		*
		*  @return
		*    The asset revision of the cached asset
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline AssetRevision getRevision() const;

		/**
		*  @brief
		*    Set the asset revision of the cached asset
		*
		*  @param[in] revision
		*    The new asset revision of the cached asset
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setRevision(AssetRevision revision);

		/**
		*  @brief
		*    Return the type name of the cached asset
		*
		*  @return
		*    The UTF-8 type name (example: "dds") of the cached asset, do no destroy the returned instance, completely lower case
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const std::string& getType() const;

		/**
		*  @brief
		*    Set the type name of the cached asset
		*
		*  @param[in] type
		*    The UTF-8 type name (example: "dds") of the cached asset, completely lower case
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setType(const std::string& type);

		/**
		*  @brief
		*    Return the hash of the cached asset
		*
		*  @return
		*    The SHA-2 256 bit hash (example: "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855") of the cached asset as ASCII string, do no destroy the returned instance
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const std::string& getHash() const;

		/**
		*  @brief
		*    Set the hash of the cached asset
		*
		*  @param[in] hash
		*    The SHA-2 256 bit hash (example: "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855") of the cached asset as ASCII string
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setHash(const std::string& hash);

		/**
		*  @brief
		*    Return derived asset map
		*
		*  @remarks
		*    At first it may look odd that the cached asset stores the information about the derived assets, the reason lies on the server side:
		*    1. The asset compiler may not change the asset itself, since this would generate a new asset revision, which in turn would trigger recompilation
		*       (no exceptions here - asset.json files never get overridden on the server-side for safety reasons)
		*    2. Asset compilers are part of the project server alias "editor server" of which multiple instances with varying asset compiler configurations may
		*       exist at the same time, which might share the same asset but generate different cached assets out of it (e.g. em5_pc + em5_ipad)
		*    => In other words: This property is stored inside the cached asset for consistency reasons with the design/architecture of QSF, since the derived assets
		*       (including the knowledge about their existence) are products of the compilation process, just like the cached asset itself.
		*/
		inline DerivedAssetMap& getDerivedAssetMap();

		/**
		*  @brief
		*    Get derived asset map
		*
		*  @see
		*    - Non-const "qsf::BaseCachedAsset::getDerivedAssetMap()"
		*/
		inline const DerivedAssetMap& getDerivedAssetMap() const;

		/**
		*  @brief
		*    Get derived asset map serialized into a string
		*
		*  @see
		*    - Non-const "qsf::BaseCachedAsset::getDerivedAssetMap()"
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		std::string getDerivedAssetMapAsString() const;

		/**
		*  @brief
		*    Set derived asset map from a string
		*
		*  @see
		*    - Non-const "qsf::BaseCachedAsset::getDerivedAssetMap()"
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setDerivedAssetMapFromString(const std::string& derivedAssetMapAsString);

		//[-------------------------------------------------------]
		//[ Dynamic properties                                    ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the dynamic properties
		*
		*  @return
		*    The dynamic properties
		*/
		inline const DynamicPropertyMap& getDynamicPropertyMap() const;

		/**
		*  @brief
		*    Return whether or not a dynamic property value exists
		*
		*  @param[in] propertyName
		*    Name of the dynamic property to check for
		*
		*  @return
		*    "true" if the dynamic property exists, else "false"
		*/
		inline bool hasDynamicPropertyValue(const NamedIdentifier& propertyName) const;

		/**
		*  @brief
		*    Return a dynamic property value
		*
		*  @param[in] propertyName
		*    Name of the dynamic property to return
		*  @param[in] defaultValue
		*    Default value to use in case the dynamic property value does not exist
		*
		*  @return
		*    Dynamic property value
		*/
		inline std::string getDynamicPropertyValueString(const NamedIdentifier& propertyName, const std::string& defaultValue = "") const;

		/**
		*  @brief
		*    Set a dynamic property value
		*
		*  @param[in] propertyName
		*    Name of the dynamic property to set
		*  @param[in] propertyValue
		*    Dynamic property value to set
		*/
		inline void setDynamicPropertyValueString(const NamedIdentifier& propertyName, const std::string& propertyValue);

		/**
		*  @brief
		*    Return a dynamic property value
		*
		*  @param[in] propertyName
		*    Name of the dynamic property to return
		*
		*  @return
		*    Dynamic property value
		*
		*  @note
		*    - Throws an exception if the dynamic property does not exist or when there's a value type mismatch
		*/
		template <typename T> T getDynamicPropertyValue(const NamedIdentifier& propertyName) const;

		/**
		*  @brief
		*    Return a dynamic property value
		*
		*  @param[in] propertyName
		*    Name of the dynamic property to return
		*  @param[in] defaultValue
		*    Default value to use in case the dynamic property value does not exist or when there's a value type mismatch
		*
		*  @return
		*    Dynamic property value
		*/
		template <typename T> T getDynamicPropertyValueSafe(const NamedIdentifier& propertyName, const T& defaultValue) const;

		/**
		*  @brief
		*    Set a dynamic property value
		*
		*  @param[in] propertyName
		*    Name of the dynamic property to set
		*  @param[in] propertyValue
		*    Dynamic property value to set
		*
		*  @note
		*    - Throws an exception if the dynamic property can't be converted into a string
		*/
		template <typename T> void setDynamicPropertyValue(const NamedIdentifier& propertyName, T propertyValue);

		/**
		*  @brief
		*    Return the dynamic properties as a composed string
		*
		*  @return
		*    The dynamic properties as a composed string
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*    - Example: TextureStreamingFlags=\"32\" ImageWidth=\"64\"
		*/
		std::string getDynamicPropertiesAsString() const;

		/**
		*  @brief
		*    Set the dynamic properties as composed string
		*
		*  @param[in] dynamicPropertiesAsString
		*    The new dynamic properties as composed string
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*    - See "qsf::BaseCachedAsset::getDynamicPropertiesAsString()"
		*/
		void setDynamicPropertiesAsString(const std::string& dynamicPropertiesAsString);


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		inline void setGlobalAssetId(GlobalAssetId globalAssetId);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		// Connected to the CAMP reflection system
		GlobalAssetId		mGlobalAssetId;			///< The global asset identifier of the cached asset
		AssetRevision		mRevision;				///< The asset revision of the cached asset
		FlyweightString		mType;					///< UTF-8 type name of the cached asset, completely lower case
		std::string			mHash;					///< The SHA-2 256 bit hash (example: "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855") of the cached asset as ASCII string
		DerivedAssetMap		mDerivedAssetMap;		///< An indexed list of global asset ids of the assets that have been derived from this asset
		DynamicPropertyMap	mDynamicPropertyMap;	///< Dynamic property map


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/asset/BaseCachedAsset-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::BaseCachedAsset)
