// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/asset/import/AssetImport.h"
#include "qsf_editor/asset/import/type/OgreMaterialAssetImport.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	namespace v1
	{
		class Mesh;
	}
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
		*    Mesh asset import class
		*/
		class QSF_EDITOR_API_EXPORT MeshAssetImport : public AssetImport
		{


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
			*/
			inline MeshAssetImport(AssetPackage& assetPackage, const std::string& ogreResourceGroup);

			/**
			*  @brief
			*    Destructor
			*
			*  @note
			*    - Not virtual by default
			*/
			inline ~MeshAssetImport();

			/**
			*  @brief
			*    Return the global asset ID of the skeleton asset gathered during the last OGRE mesh import to use as animation source
			*
			*  @return
			*    The global asset ID of the skeleton asset gathered during the last OGRE mesh import to use as animation source
			*
			*  @note
			*    - By our convention, one skeleton asset contains only a single animation with the global skeleton asset ID as animation name
			*/
			inline GlobalAssetId getGlobalSkeletonAnimationAssetId() const;

			/**
			*  @brief
			*    Import asset
			*
			*  @param[in] absoluteFilename
			*    Absolute UTF-8 filename in platform-independent notation of the asset to import
			*  @param[in] parentOgreMesh
			*    Parent OGRE mesh in case this is a LOD mesh, can be a null pointer
			*  @param[in, out] ogreMaterials
			*    Used to resolve OGRE material instancing issues
			*  @param[in, out] enforceSkeletonGlobalAssetId
			*    If not uninitialized, the mesh is enforced to use the given skeleton global asset ID instead of its own one
			*
			*  @return
			*    The global asset ID of the imported asset, "qsf::getUninitialized<GlobalAssetId>()" if invalid
			*/
			GlobalAssetId importByAbsoluteFilename(const std::string& absoluteFilename, Ogre::v1::Mesh* parentOgreMesh, OgreMaterialAssetImport::OgreMaterialMap& ogreMaterials, GlobalAssetId enforceSkeletonGlobalAssetId = getUninitialized<GlobalAssetId>()) const;


		//[-------------------------------------------------------]
		//[ Public virtual qsf::editor::base::AssetImport methods ]
		//[-------------------------------------------------------]
		public:
			virtual GlobalAssetId importByAbsoluteFilename(const std::string& absoluteFilename) const override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
					std::string	  mOgreResourceGroup;				///< UTF-8 name of the OGRE resource group to use
			mutable GlobalAssetId mGlobalSkeletonAnimationAssetId;	///< The global asset ID of the skeleton asset gathered during the last OGRE mesh import to use as animation source // TODO(co) Review additional gathered import information (this solution in here is not multi-threading safe)


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor/asset/import/type/MeshAssetImport-inl.h"
