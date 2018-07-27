// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"

#include <qsf/asset/AssetSystemTypes.h>

#include <boost/noncopyable.hpp>

#include <string>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class CachedAsset;
	class AssetPackage;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Asset package import helper class
		*
		*  @remarks
		*    Usage example:
		*    @code
		*    // qsf::AssetPackage& assetPackage = ...
		*    qsf::AssetPackageImportHelper(assetPackage).importByDirectory();
		*
		*    // Don't forget to save the manipulated asset package when done
		*    assetPackage.saveByFilename(assetPackage.getFilename());
		*    @endcode
		*/
		class QSF_EDITOR_API_EXPORT AssetPackageImportHelper : public boost::noncopyable
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    The default constructor uses the default destination asset package
			*
			*  @see
			*    - "qsf::editor::AssetImportManager::getDefaultDestinationAssetPackage()"
			*/
			AssetPackageImportHelper();

			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] assetPackage
			*    Asset package to use, must stay valid as long as this asset package import helper instance exists
			*/
			explicit inline AssetPackageImportHelper(AssetPackage& assetPackage);

			/**
			*  @brief
			*    Destructor
			*
			*  @note
			*    - Not virtual by intent
			*/
			inline ~AssetPackageImportHelper();

			/**
			*  @brief
			*    Return the used asset package as reference
			*
			*  @return
			*    Used asset package, do not destroy the instance
			*/
			inline AssetPackage& getAssetPackageSafe() const;

			/**
			*  @brief
			*    Return the used asset package as pointer
			*
			*  @return
			*    Used asset package, do not destroy the instance
			*/
			inline AssetPackage* getAssetPackage() const;

			/**
			*  @brief
			*    Save the used asset package by using the existing filename
			*
			*  @return
			*    "true" if all went fine, else "false"
			*
			*  @note
			*    - Same as writing "assetPackage.saveByFilename(assetPackage.getFilename())"
			*/
			bool saveAssetPackage() const;

			//[-------------------------------------------------------]
			//[ Check for duplicates                                  ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Try to find an asset by using available asset information: Filename
			*
			*  @param[in] assetTypeId
			*    The asset type ID (e.g. qsf::StringHash("mesh"))
			*  @param[in] filename
			*    The UTF-8 filename in platform-independent notation of the locally stored asset to search for
			*
			*  @return
			*    The found asset, null pointer on error, do not destroy the instance
			*
			*  @note
			*    - Use this method to search for already imported assets in order to avoid duplicates
			*/
			Asset* findAssetByFilename(const AssetTypeId assetTypeId, const std::string& filename) const;
			std::pair<Asset*, CachedAsset*> findAssetAndCachedAssetByFilename(const AssetTypeId assetTypeId, const std::string& filename) const;

			/**
			*  @brief
			*    Try to find an asset by using available asset information: Hash of the cached asset file
			*
			*  @param[in] assetTypeId
			*    The asset type ID (e.g. qsf::StringHash("mesh"))
			*  @param[in] fileHash
			*    The hash (example: "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855") of the cached asset as ASCII string of the locally stored asset to search for
			*
			*  @return
			*    The found asset, null pointer on error, do not destroy the instance
			*
			*  @note
			*    - Use this method to search for already imported assets in order to avoid duplicates
			*/
			Asset* findAssetByFileHash(const AssetTypeId assetTypeId, const std::string& fileHash) const;
			std::pair<Asset*, CachedAsset*> findAssetAndCachedAssetByFileHash(const AssetTypeId assetTypeId, const std::string& fileHash) const;

			//[-------------------------------------------------------]
			//[ Import                                                ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Import an asset package by using a given directory
			*
			*  @param[in] directory
			*    UTF-8 directory in platform-independent notation of the asset package to import
			*
			*  @return
			*    "true" if all went fine, else "false" (file not found?)
			*
			*  @note
			*    - The current asset package will get lost, so, you might want to save a manipulated asset package before loading a new one
			*/
			bool importByDirectory(const std::string& directory);

			/**
			*  @brief
			*    Import a locally stored asset into the asset package
			*
			*  @param[in] typeName
			*    The UTF-8 type name (example: "mesh"), completely lower case
			*  @param[in] category
			*    The UTF-8 category name (example: "house"), completely lower case
			*  @param[in] filename
			*    The UTF-8 filename in platform-independent notation of the locally stored asset to import
			*  @param[in] fileHash
			*    Optional already available hash (example: "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855") of the cached asset as ASCII string of the locally stored asset to search for, can be a null pointer
			*
			*  @return
			*    The imported asset, null pointer on error, do not destroy the instance
			*
			*  @note
			*    - Will fail in case the asset package is currently mounted
			*    - TODO(oi) What happens if the asset file is already existing as another asset?
			*/
			Asset* importLocalAsset(const std::string& typeName, const std::string& category, const std::string& filename, const std::string* fileHash = nullptr);

			/**
			*  @brief
			*    Import or reimport a locally stored asset into the asset package
			*
			*  @param[in] typeName
			*    The UTF-8 type name (example: "mesh"), completely lower case
			*  @param[in] category
			*    The UTF-8 category name (example: "house"), completely lower case
			*  @param[in] filename
			*    The UTF-8 filename in platform-independent notation of the locally stored asset to import
			*  @param[in] fileHash
			*    Optional already available hash (example: "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855") of the cached asset as ASCII string of the locally stored asset to search for, can be a null pointer
			*
			*  @return
			*    The imported asset, null pointer on error, do not destroy the instance
			*
			*  @note
			*    - Will fail in case the asset package is currently mounted
			*/
			Asset* importOrReimportLocalAsset(const std::string& typeName, const std::string& category, const std::string& filename, const std::string* fileHash = nullptr);

			/**
			*  @brief
			*    Import all assets from another asset package
			*
			*  @param[in] sourceAssetPackage
			*    The asset package to import the all assets from from
			*/
			void importAllAssetsFromAssetPackage(const AssetPackage& sourceAssetPackage);

			/**
			*  @brief
			*    Import an asset from another asset package
			*
			*  @param[in] sourceAssetPackage
			*    The asset package to import the asset from
			*  @param[in] sourceAsset
			*    The source asset to import
			*  @param[in] sourceCachedAsset
			*    The optional source cached asset to import, can be a null pointer
			*/
			void importAssetFromAssetPackage(const AssetPackage& sourceAssetPackage, const Asset& sourceAsset, const CachedAsset* sourceCachedAsset);

			/**
			*  @brief
			*    Import a locally stored cached asset into the asset package
			*
			*  @param[in] asset
			*    Asset this is the cached asset for
			*  @param[in] filename
			*    The UTF-8 filename in platform-independent notation of the locally stored cached asset to import
			*  @param[in] fileHash
			*    Optional already available hash (example: "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855") of the cached asset as ASCII string of the locally stored asset to search for, can be a null pointer
			*
			*  @note
			*    - Before calling this method, do ensure that the asset package is currently not mounted
			*/
			void importLocalCachedAsset(Asset& asset, const std::string& filename, const std::string* fileHash = nullptr);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			//[-------------------------------------------------------]
			//[ Check for duplicates                                  ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Try to find an asset inside the given asset package by using available asset information: Name
			*
			*  @param[in] assetPackage
			*    Asset package to search in
			*  @param[in] assetTypeId
			*    The asset type ID (e.g. qsf::StringHash("mesh"))
			*  @param[in] assetName
			*    The UTF-8 name of the locally stored asset to search for
			*
			*  @return
			*    The found asset, null pointer on error, do not destroy the instance
			*
			*  @note
			*    - Use this method to search for already imported assets in order to avoid duplicates
			*/
			std::pair<Asset*, CachedAsset*> findAssetAndCachedAssetByNameInAssetPackage(const AssetPackage& assetPackage, const AssetTypeId assetTypeId, const std::string& assetName) const;

			/**
			*  @brief
			*    Try to find an asset inside the given asset package by using available asset information: Hash of the cached asset file
			*
			*  @param[in] assetPackage
			*    Asset package to search in
			*  @param[in] assetTypeId
			*    The asset type ID (e.g. qsf::StringHash("mesh"))
			*  @param[in] fileHash
			*    The hash (example: "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855") of the cached asset as ASCII string of the locally stored asset to search for
			*
			*  @return
			*    The found asset, null pointer on error, do not destroy the instance
			*
			*  @note
			*    - Use this method to search for already imported assets in order to avoid duplicates
			*/
			std::pair<Asset*, CachedAsset*> findAssetAndCachedAssetByFileHashInAssetPackage(const AssetPackage& assetPackage, const AssetTypeId assetTypeId, const std::string& fileHash) const;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			AssetPackage* mAssetPackage;	///< Asset package to use, always valid, do not destroy the instance


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor/asset/import/AssetPackageImportHelper-inl.h"
