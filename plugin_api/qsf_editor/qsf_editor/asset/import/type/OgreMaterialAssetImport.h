// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/asset/import/AssetImport.h"

#include <qsf_editor_base/asset/TextureSemanticManager.h>

#include <boost/container/flat_map.hpp>
#include <boost/container/flat_set.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class Pass;
	class Material;
	class TextureUnitState;
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
		*    OGRE material asset import class
		*/
		class QSF_EDITOR_API_EXPORT OgreMaterialAssetImport : public AssetImport
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			typedef boost::container::flat_map<std::string, Asset*> OgreMaterialMap;

			static const std::string MAPS_SCHEME;	///< Maps scheme


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Return whether or not the given filename extension is supported
			*
			*  @param[in] extension
			*    UTF-8 filename extension to check, e.g. ".material"
			*
			*  @return
			*    "true" if the given filename extension is supported, else "false"
			*/
			static bool isFilenameExtensionSupported(const std::string& extension);

			static bool compileQsfMaterial(const std::string& absoluteSourceFilename, const std::string& absoluteDestinationFilename, GlobalAssetId globalAssetId, bool compileTextures = true);


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
			*  @param[in] qsfBaseMaterial
			*    If QSF material, which is the QSF base material?
			*/
			inline OgreMaterialAssetImport(AssetPackage& assetPackage, const std::string& ogreResourceGroup, const std::string& resourceDirectory, const std::string& qsfBaseMaterial = "qsf_mesh_base");

			/**
			*  @brief
			*    Destructor
			*
			*  @note
			*    - Not virtual by default
			*/
			inline ~OgreMaterialAssetImport();

			/**
			*  @brief
			*    Import OGRE material by using a given OGRE material instance
			*
			*  @param[in] ogreMaterial
			*    OGRE material instance to use
			*  @param[in, out] ogreMaterials
			*    Used to resolve OGRE material instancing issues
			*
			*  @return
			*    The global asset ID of the imported asset, "qsf::getUninitialized<GlobalAssetId>()" if invalid
			*/
			GlobalAssetId importByOgreMaterial(Ogre::Material& ogreMaterial, OgreMaterialMap& ogreMaterials) const;


		//[-------------------------------------------------------]
		//[ Public virtual qsf::editor::base::AssetImport methods ]
		//[-------------------------------------------------------]
		public:
			virtual GlobalAssetId importByAbsoluteFilename(const std::string& absoluteFilename) const override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Add texture asset
			*
			*  @param[in]  ogrePass
			*    OGRE material technique pass to check for duplicates
			*  @param[in]  filename
			*    UTF-8 filename of the texture to add as texture asset
			*  @param[in] resourceDirectory
			*    UTF-8 name of the resource directory where to search for further dependencies
			*  @param[in] ignoreEmptyTextureUnit
			*    Ignore empty texture unit instead of outputting errors?
			*  @param[out] textureSemantic
			*    In case of success, will receive a pointer to the texture semantic, null pointer on failure
			*  @param[out] directGlobalAssetIdDependencies
			*    Receives the direct global asset ID dependencies, list is not cleared before new entries are added
			*
			*  @return
			*    The global asset ID of the added texture asset, "qsf::getUninitialized<GlobalAssetId>()" if invalid
			*/
			GlobalAssetId addTextureAsset(Ogre::Pass& ogrePass, const std::string& filename, const std::string& resourceDirectory, bool ignoreEmptyTextureUnit, const base::TextureSemanticManager::Semantic** textureSemantic, boost::container::flat_set<GlobalAssetId>& directGlobalAssetIdDependencies) const;

			/**
			*  @brief
			*    Import OGRE material by using a given OGRE material instance
			*
			*  @param[in]  textureStem
			*    Texture stem, e.g. "wind_engine_c.tif" will result in texture stem "wind_engine"
			*  @param[in]  ogrePass
			*    OGRE material technique pass to gather the textures for
			*  @param[in] resourceDirectory
			*    UTF-8 name of the resource directory where to search for further dependencies
			*  @param[out] directGlobalAssetIdDependencies
			*    Receives the direct global asset ID dependencies, list is not cleared before new entries are added
			*/
			void gatherAdditionalTextures(const std::string& textureStem, Ogre::Pass& ogrePass, const std::string& resourceDirectory, boost::container::flat_set<GlobalAssetId>& directGlobalAssetIdDependencies) const;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			std::string mOgreResourceGroup;	///< UTF-8 name of the OGRE resource group to use
			std::string mResourceDirectory;	///< UTF-8 name of the resource directory where to search for further dependencies
			std::string mQsfBaseMaterial;	///< If QSF material, which is the QSF base material?


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor/asset/import/type/OgreMaterialAssetImport-inl.h"
