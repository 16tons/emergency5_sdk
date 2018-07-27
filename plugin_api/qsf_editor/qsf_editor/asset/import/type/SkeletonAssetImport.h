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
namespace Ogre
{
	namespace v1
	{
		class Mesh;
		class Skeleton;
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
		*    Skeleton asset import class
		*/
		class QSF_EDITOR_API_EXPORT SkeletonAssetImport : public AssetImport
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
			*    UTF-8 filename extension to check, e.g. ".skeleton"
			*
			*  @return
			*    "true" if the given filename extension is supported, else "false"
			*/
			static bool isFilenameExtensionSupported(const std::string& extension);


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
			inline SkeletonAssetImport(AssetPackage& assetPackage, const std::string& ogreResourceGroup, const std::string& resourceDirectory);

			/**
			*  @brief
			*    Destructor
			*
			*  @note
			*    - Not virtual by default
			*/
			inline ~SkeletonAssetImport();

			/**
			*  @brief
			*    Import asset
			*
			*  @param[in] absoluteFilename
			*    Absolute UTF-8 filename in platform-independent notation of the asset to import
			*  @param[in] parentOgreMesh
			*    Parent OGRE mesh in case this is a skeleton which is used by a LOD mesh, can be a null pointer
			*
			*  @return
			*    The global asset ID of the imported asset, "qsf::getUninitialized<GlobalAssetId>()" if invalid
			*/
			GlobalAssetId importByAbsoluteFilename(const std::string& absoluteFilename, Ogre::v1::Mesh* parentOgreMesh) const;


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
			*    Import single skeleton animation asset
			*
			*  @param[in] filename
			*    UTF-8 filename (not absolute nor relative, just the filename without path) in platform-independent notation of the asset to import
			*  @param[in] ogreSkeleton
			*    OGRE skeleton containing the animation
			*
			*  @return
			*    The global asset ID of the imported asset, "qsf::getUninitialized<GlobalAssetId>()" if invalid
			*/
			GlobalAssetId importSingleSkeletonByFilename(const std::string& filename, Ogre::v1::Skeleton& ogreSkeleton) const;

			/**
			*  @brief
			*    Serialize OGRE skeleton
			*
			*  @param[in] filename
			*    UTF-8 filename (not absolute nor relative, just the filename without path) in platform-independent notation of the asset to import
			*  @param[in] ogreSkeleton
			*    OGRE skeleton to serialize
			*  @param[in] skeletonAsset
			*    Asset of the OGRE skeleton to serialize
			*/
			void serializeOgreSkeleton(const std::string& filename, Ogre::v1::Skeleton& ogreSkeleton, Asset& skeletonAsset) const;


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
#include "qsf_editor/asset/import/type/SkeletonAssetImport-inl.h"
