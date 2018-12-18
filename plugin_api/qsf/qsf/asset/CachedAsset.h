// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/asset/BaseCachedAsset.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
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
	*    Cached asset class
	*/
	class QSF_API_EXPORT CachedAsset : public BaseCachedAsset
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class AssetPackage;	// Sets the virtual UTF-8 filename of the locally cached asset data of the asset


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
		explicit CachedAsset(GlobalAssetId globalAssetId);

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Not virtual by intent
		*/
		inline ~CachedAsset();

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
		inline CachedAsset& operator=(const CachedAsset& source);

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
		inline CachedAsset& operator=(const BaseCachedAsset& source);

		/**
		*  @brief
		*    Copy method excluding GlobalAssetId
		*
		*  @param[in] source
		*    Source to copy from
		*
		*  @return
		*    Reference to this base asset
		*/
		inline CachedAsset& copyPropertiesFrom(const CachedAsset& source);

		/**
		*  @brief
		*    Serialization of the cached asset
		*/
		void serialize(BinarySerializer& serializer, uint32 version);

		//[-------------------------------------------------------]
		//[ Mounted data                                          ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the local asset name
		*
		*  @return
		*    The local asset name
		*
		*  @note
		*    - Example: "sample/mesh/house/house01", "<project name>/<asset type>/<asset category>/<asset name>"
		*    - Only valid as long as the cached asset is mounted
		*/
		inline const std::string& getLocalAssetName() const;

		/**
		*  @brief
		*    Return the virtual filename of the locally cached asset data of the asset
		*
		*  @return
		*    The virtual UTF-8 filename of the locally cached asset data of the asset
		*
		*  @note
		*    - Example: "sample/mesh/house/house01.mesh", "<project name>/<asset type>/<asset category>/<asset name>.<cached asset type>"
		*    - Only valid as long as the cached asset is mounted
		*/
		inline const std::string& getCachedAssetDataFilename() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Internal
		std::string mLocalAssetName;			///< The local asset name (e.g. "sample/mesh/house/house01", "<project name>/<asset type>/<asset category>/<asset name>"), only valid as long as the cached asset is mounted
		std::string mCachedAssetDataFilename;	///< The virtual UTF-8 filename of the locally cached asset data of the asset (e.g. "sample/mesh/house/house01.mesh", "<project name>/<asset type>/<asset category>/<asset name>.<cached asset type>"), only valid as long as the cached asset is mounted


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
#include "qsf/asset/CachedAsset-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::CachedAsset)
