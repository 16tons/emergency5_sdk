// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"

#include <qsf/log/LogSystem.h>
#include <qsf/asset/AssetSystemTypes.h>

#include <boost/noncopyable.hpp>

#include <string>
#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
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
		*    Abstract asset import base class
		*
		*  @note
		*    - TODO(co) For now, the asset import classes are kept simple. Later on we might want to make this to pluginables and more generic and powerful.
		*/
		class QSF_EDITOR_API_EXPORT AssetImport : public boost::noncopyable
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const std::string ASSET_CATEGORY_NAME;		///< Asset category newly imported assets will be in
			static const std::string ASSET_DATA_NAME_PREFIX;	///< Prefix used for intermediate asset data names


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Return the asset package instance new imported assets should be registered in
			*
			*  @return
			*    The asset package instance new imported assets should be registered in
			*/
			inline AssetPackage& getAssetPackage() const;


		//[-------------------------------------------------------]
		//[ Public virtual qsf::editor::base::AssetImport methods ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Import asset
			*
			*  @param[in] absoluteFilename
			*    Absolute UTF-8 filename in platform-independent notation of the asset to import
			*
			*  @return
			*    The global asset ID of the imported asset, "qsf::getUninitialized<GlobalAssetId>()" if invalid
			*/
			virtual GlobalAssetId importByAbsoluteFilename(const std::string& absoluteFilename) const = 0;


		//[-------------------------------------------------------]
		//[ Protected static                                      ]
		//[-------------------------------------------------------]
		protected:
			/**
			*  @brief
			*    Print a text into the log
			*
			*  @param[in] severityLevel
			*    Severity level
			*  @param[in] text
			*    UTF-8 text which should be written into the log
			*
			*  @remarks
			*    The import of certain asset types might take a moment, so, add messages by using this
			*    method where useful so that the user has a chance to understand what's going on.
			*/
			static void print(LogMessage::SeverityLevel severityLevel, const std::string& text);

			/**
			*  @brief
			*    Call this method in order to tell others about the asset import progress
			*
			*  @param[in] message
			*    Progress message
			*  @param[in] progress
			*    Progress between [0..1] (1 means done)
			*/
			static void emitProgress(const std::string& message, float progress = 0.0f);


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] assetPackage
			*    Asset package instance new imported assets should be registered in, instance must stay valid as long as this asset import instance exists
			*/
			inline explicit AssetImport(AssetPackage& assetPackage);

			/**
			*  @brief
			*    Destructor
			*
			*  @note
			*    - Not virtual by default
			*/
			inline ~AssetImport();

			// TODO(co) Comment
			// Throws an exception in case of error
			void copyAbsoluteAssetDataFile(const std::string& absoluteFilename, const std::string& relativeAssetPackageFilename, Asset& asset) const;
			void copyRelativeAssetDataFile(const std::string& resourceDirectory, const std::string& filename, const std::string& relativeAssetPackageFilename, Asset& asset) const;

			/**
			*  @brief
			*    Return the asset data name without prefab
			*
			*  @param[in] assetDataName
			*    Asset data name with prefix (see "qsf::editor::AssetImport::ASSET_DATA_NAME_PREFIX"), e.g. "_qsf_asset_import_my_material"
			*
			*  @return
			*    Asset data name without prefix, e.g. "my_material"
			*/
			std::string getAssetDataNameWithoutPrefix(const std::string& assetDataName) const;

			/**
			*  @brief
			*    Correct asset data name, if required
			*
			*  @param[in] assetDataName
			*    Asset data name to correct if required, e.g. "Diffusew/Mask"
			*
			*  @return
			*    Correct asset data name, e.g. "diffusew_mask"
			*
			*  @remarks
			*    Sadly, artists tend to use all possible and unpossible combinations everywhere. When it comes to filenames
			*    this will introduce tons of issues. For example, the characters
			*      \/:*?"<>|
			*    are not allowed inside filenames. The MS Windows file system is not case sensitive while the Unix file system is.
			*    In order to avoid case issues, we output only lower case names.
			*/
			std::string correctAssetDataName(const std::string& assetDataName) const;

			/**
			*  @brief
			*    Copy the provided asset source into the directory where the asset storage manager temporally locally stores assets to be edited by the user
			*
			*  @note
			*    - Ease-of-use shortcut for derived asset import classes
			*/
			void copyToAssetStorageManagerSourceAssetDirectory(const std::string& absoluteFilename, GlobalAssetId globalAssetId) const;

			/**
			*  @brief
			*    Copy the provided compiled asset into the directory where the asset storage manager temporally locally stores assets to be edited by the user
			*
			*  @note
			*    - Ease-of-use shortcut for derived asset import classes
			*/
			void copyCompiledToAssetStorageManagerSourceAssetDirectory(const std::string& absoluteFilename, GlobalAssetId globalAssetId) const;

			/**
			*  @brief
			*    Add the given global asset ID to the imported QSF materials list, so its source material will be either moved
			*    out from the import directory inside our material datasource or deleted, depending on whether the import is accepted or not
			*
			*  @note
			*    - Ease-of-use shortcut for derived asset import classes
			*/
			void addImportedQsfMaterial(GlobalAssetId globalAssetId) const;

			/**
			*  @brief
			*    Schedule all qsf material assets that directly depend on this asset for recompilation
			*
			*  @note
			*    - Ease-of-use shortcut for derived asset import classes
			*/
			void scheduleDependantQsfMaterialsForRecompilation(GlobalAssetId globalAssetId) const;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			AssetPackage* mAssetPackage;	/// Asset package instance new imported assets should be registered in, always valid, do not destroy the instance


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor/asset/import/AssetImport-inl.h"
