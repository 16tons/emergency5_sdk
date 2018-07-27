// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/asset/import/AssetImport.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class BaseCachedAsset;
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
		*    Texture asset import class
		*/
		class QSF_EDITOR_API_EXPORT TextureAssetImport : public AssetImport
		{


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Return whether or not the given filename extension is supported
			*
			*  @param[in] extension
			*    UTF-8 filename extension to check, e.g. ".tif"
			*
			*  @return
			*    "true" if the given filename extension is supported, else "false"
			*/
			static bool isFilenameExtensionSupported(const std::string& extension);

			static bool compileTexture(const std::string& absoluteSourceFilename, const std::string& absoluteDestinationFilename, BaseCachedAsset& baseCachedAsset, uint32 typeId);


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
			*  @param[in] compileTexture
			*    Whether or not to compile the texture (=generate a cached asset file). If false, this texture is considered to be processed by the
			*    material-texture-compositing. In case no QSF material depends from this texture, compiling is enfoced.
			*/
			inline TextureAssetImport(AssetPackage& assetPackage, const std::string& ogreResourceGroup, const std::string& resourceDirectory, bool compileTexture);

			/**
			*  @brief
			*    Destructor
			*
			*  @note
			*    - Not virtual by default
			*/
			inline ~TextureAssetImport();


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
			bool		mCompileTexture;	///< Whether or not to compile the texture (=generate a cached asset file)


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor/asset/import/type/TextureAssetImport-inl.h"
