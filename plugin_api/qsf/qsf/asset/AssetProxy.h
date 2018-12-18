// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/asset/AssetSystemTypes.h"
#include "qsf/base/Proxy.h"
#include "qsf/base/flyweight/FlyweightString.h"
#include "qsf/serialization/binary/BasicTypeSerialization.h"

#include <string>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class AssetPackage;
	class CachedAsset;
	class BinarySerializer;
}


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
	*    Asset proxy class
	*
	*  @remarks
	*    Here's a tiny asset proxy usage example:
	*    @code
	*    qsf::AssetProxy assetProxy("sample/mesh/house/house01");
	*    std::string filename(assetProxy.getCachedAssetDataFilename());
	*    @endcode
	*
	*  @note
	*    - Use "qsf::AssetProxy" instead of "std::string" to store asset references
	*    - It's recommended to access assets only via asset proxies
	*/
	class QSF_API_EXPORT AssetProxy : public Proxy
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const AssetProxy EMPTY;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		AssetProxy();

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] assetProxy
		*    Asset proxy to copy from
		*/
		inline AssetProxy(const AssetProxy& assetProxy);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] globalAssetId
		*    Global ID of the asset this asset proxy wraps up, "qsf::getUninitialized<GlobalAssetId>()" if invalid
		*/
		explicit AssetProxy(GlobalAssetId globalAssetId);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] localAssetName
		*    Local UTF-8 name of the asset this asset proxy should wrap up (e.g. "sample/mesh/house/house01")
		*/
		explicit AssetProxy(const std::string& localAssetName);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] localAssetId
		*    Local asset id of the asset this asset proxy should wrap up, "qsf::getUninitialized<LocalAssetId>()" if invalid
		*/
		explicit AssetProxy(LocalAssetId localAssetId);

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Not virtual by intent
		*/
		inline ~AssetProxy();

		/**
		*  @brief
		*    Reset the asset proxy to initial default values
		*/
		inline void clear();

		/**
		*  @brief
		*    Return whether or not the asset proxy does wrap up an asset
		*
		*  @return
		*    "true" if the asset proxy wraps up and asset and hence is considered to be valid, else "false"
		*/
		bool isValid() const;

		/**
		*  @brief
		*    Asset proxy equal operator
		*
		*  @param[in] assetProxy
		*    The other asset proxy to compare this asset proxy with
		*
		*  @return
		*    "true" if both asset proxies wrap up the same asset, else "false"
		*/
		inline bool operator ==(const AssetProxy& assetProxy) const;

		/**
		*  @brief
		*    Asset proxy not equal operator
		*
		*  @param[in] assetProxy
		*    The other asset proxy to compare this asset proxy with
		*
		*  @return
		*    "true" if both asset proxies do not wrap up the same asset, else "false"
		*/
		inline bool operator !=(const AssetProxy& assetProxy) const;

		//[-------------------------------------------------------]
		//[ Global                                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the global ID of the asset this asset proxy wraps up
		*
		*  @return
		*    Global ID of the asset this asset proxy wraps up, "qsf::getUninitialized<GlobalAssetId>()" if invalid
		*/
		GlobalAssetId getGlobalAssetId() const;

		/**
		*  @brief
		*    Set the global ID of the asset this asset proxy wraps up
		*
		*  @param[in] globalAssetId
		*    Global ID of the asset this asset proxy wraps up, "qsf::getUninitialized<GlobalAssetId>()" if invalid
		*/
		void setGlobalAssetId(GlobalAssetId globalAssetId);

		//[-------------------------------------------------------]
		//[ Local                                                 ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the local name of the asset this asset proxy wraps up
		*
		*  @return
		*    Local UTF-8 name of the asset this asset proxy wraps up (e.g. "sample/mesh/house/house01")
		*/
		const std::string& getLocalAssetName() const;

		/**
		*  @brief
		*    Return the local name of the asset this asset proxy wraps up
		*
		*  @return
		*    Local UTF-8 name of the asset this asset proxy wraps up (e.g. "sample/mesh/house/house01")
		*
		*  @note
		*    - Ease-of-use operator returning the same result as "qsf::AssetProxy::getLocalAssetName()"
		*/
		inline operator const std::string&() const;

		/**
		*  @brief
		*    Return whether or not the local asset ID is already initialized
		*
		*  @return
		*    "true" if the local asset ID is already initialized, else "false"
		*/
		inline bool isLocalAssetIdInitialized() const;

		/**
		*  @brief
		*    Return the local ID of the asset this asset proxy wraps up
		*
		*  @return
		*    Local ID of the asset this asset proxy wraps up (e.g. qsf::StringHash("sample/mesh/house/house01")), "qsf::getUninitialized<LocalAssetId>()" if invalid
		*
		*  @note
		*    - The local asset ID is generated by using "qsf::StringHash" on the local UTF-8 name of the asset this asset proxy wraps up
		*/
		LocalAssetId getLocalAssetId() const;

		/**
		*  @brief
		*    Set the local name of the asset this asset proxy wraps up
		*
		*  @param[in] localAssetName
		*    Local UTF-8 name of the asset this asset proxy should wrap up (e.g. "sample/mesh/house/house01")
		*/
		void setLocalAssetName(const std::string& localAssetName);

		/**
		*  @brief
		*    Return the virtual filename of the locally cached asset data of the asset this asset proxy wraps up
		*
		*  @return
		*    The virtual UTF-8 filename of the locally cached asset data of the asset this asset proxy wraps up (e.g. "sample/mesh/house/house01.mesh")
		*/
		std::string getCachedAssetDataFilename() const;

		/**
		*  @brief
		*    Return the absolute filename of the locally cached asset data of the asset this asset proxy wraps up
		*
		*  @return
		*    The absolute UTF-8 filename of the locally cached asset data of the asset this asset proxy wraps up (e.g. "c:/application/data/sample/mesh/house/house01.mesh")
		*/
		std::string getAbsoluteCachedAssetDataFilename() const;

		/**
		*  @brief
		*    Return the asset
		*
		*  @return
		*    The asset, null pointer on error, do not destroy the instance
		*
		*  @note
		*    - Ease-of-use method, don't use this method if you don't have to (performance)
		*/
		Asset* getAsset() const;

		/**
		*  @brief
		*    Return the cached asset
		*
		*  @return
		*    The cached asset, null pointer on error, do not destroy the instance
		*
		*  @note
		*    - Ease-of-use method, don't use this method if you don't have to (performance)
		*/
		CachedAsset* getCachedAsset() const;

		/**
		*  @brief
		*    Return the type name of the cached asset
		*
		*  @return
		*    The UTF-8 type name (example: "dds") of the cached asset, do no destroy the returned instance, completely lower case
		*
		*  @note
		*    - Ease-of-use method, don't use this method if you don't have to (performance)
		*/
		std::string getCachedAssetType() const;

		/**
		*  @brief
		*    Return the asset package the asset resides in
		*
		*  @return
		*    The asset package the asset resides in, do no destroy the returned instance, null pointer on error
		*
		*  @note
		*    - Ease-of-use method, don't use this method if you don't have to (performance)
		*/
		AssetPackage* getAssetPackage() const;

		/**
		*  @brief
		*    Return the id of the project the asset resides in
		*
		*  @return
		*    The id of the project the asset resides in, uninitialized on error
		*
		*  @note
		*    - Ease-of-use method, don't use this method if you don't have to (performance)
		*/
		ProjectId getProjectId() const;

		//[-------------------------------------------------------]
		//[ Serialization                                         ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Serialize or deserialize the asset proxy using a binary serializer
		*
		*  @param[in] serializer
		*    The serializer, which can be either in read or write mode
		*/
		void serialize(BinarySerializer& serializer);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		mutable GlobalAssetId	mGlobalAssetId;				///< Global ID of the asset this asset proxy wraps up, "qsf::getUninitialized<GlobalAssetId>()" if invalid
		mutable FlyweightString	mLocalAssetName;			///< Local UTF-8 name of the asset this asset proxy should wrap up (e.g. "sample/mesh/house/house01")
		mutable LocalAssetId	mLocalAssetId;				///< Local ID of the asset this asset proxy wraps up (e.g. qsf::StringHash("sample/mesh/house/house01")), "qsf::getUninitialized<LocalAssetId>()" if invalid
		mutable FlyweightString mCachedAssetDataFilename;	///< The virtual UTF-8 filename of the locally cached asset data of the asset this asset proxy wraps up (e.g. "sample/mesh/house/house01.mesh")


	};


	namespace serialization
	{
		// Serialization specialization for AssetProxy for use with the QSF binary serialization
		template<>
		struct serializer<AssetProxy>
		{
			inline static void serialize(BinarySerializer& serializer, AssetProxy& value)
			{
				value.serialize(serializer);
			}
		};
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


// Support for qsf::AssetProxy output in std::ostream
inline std::ostream& operator<<(std::ostream& ostream, const qsf::AssetProxy& assetProxy)
{
	if (assetProxy.isValid())
		ostream << '"' << assetProxy.getLocalAssetName() << "\" (global asset ID " << assetProxy.getGlobalAssetId() << ')';
	else
		ostream << "<invalid asset proxy>";
	return ostream;
}


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/asset/AssetProxy-inl.h"
