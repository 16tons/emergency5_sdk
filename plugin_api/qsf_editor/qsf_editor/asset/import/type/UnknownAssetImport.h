// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/asset/import/AssetImport.h"

#include <qsf/base/NamedIdentifier.h>


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
		*    Unknown asset import class
		*/
		class QSF_EDITOR_API_EXPORT UnknownAssetImport : public AssetImport
		{


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Try to detect the asset type
			*
			*  @param[in] absoluteFilename
			*    Absolute UTF-8 filename in platform-independent notation
			*
			*  @return
			*    The asset type asset, "qsf::getUninitialized<NamedIdentifier>()" on failure (you should not continue in this case because there's e.g. a JSON syntax error)
			*
			*  @todo
			*    - TODO(co) This might be a candidate for "qsf::AssetTypeManager"
			*/
			static NamedIdentifier getAssetTypeByAbsoluteFilename(const std::string& absoluteFilename);


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] assetPackage
			*    Asset package instance new imported assets should be registered in, instance must stay valid as long as this asset import instance exists
			*  @param[in] ogreResourceGroup
			*    UTF-8 name of the OGRE resource group to use
			*  @param[in] resourceDirectory
			*    Resource directory where to search for further dependencies
			*/
			inline UnknownAssetImport(AssetPackage& assetPackage, const std::string& ogreResourceGroup, const std::string& resourceDirectory);

			/**
			*  @brief
			*    Destructor
			*
			*  @note
			*    - Not virtual by default
			*/
			inline ~UnknownAssetImport();


		//[-------------------------------------------------------]
		//[ Public virtual qsf::editor::base::AssetImport methods ]
		//[-------------------------------------------------------]
		public:
			virtual GlobalAssetId importByAbsoluteFilename(const std::string& absoluteFilename) const override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			std::string mOgreResourceGroup;	///< UTF-8 name of the OGRE resource group to use
			std::string mResourceDirectory;	///< UTF-8 name of the resource directory where to search for further dependencies


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor/asset/import/type/UnknownAssetImport-inl.h"
