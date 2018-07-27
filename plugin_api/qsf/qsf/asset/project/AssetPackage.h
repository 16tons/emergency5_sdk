// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/asset/project/BaseAssetPackage.h"
#include "qsf/asset/Asset.h"
#include "qsf/asset/CachedAsset.h"
#include "qsf/base/manager/ElementManager.h"

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Project;
	class AssetSystem;
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
	*    Asset package class
	*
	*  @remarks
	*    Each project is using 0-n assets which are grouped into asset packages in order to be able to keep an overview.
	*    When shipping products, each asset package can be e.g. zipped. Outside the project context asset packages are
	*    not directly visible to e.g. the asset system.
	*
	*    As soon as the project and the asset packages are loaded (!= mounted), the asset package is automatically mounted
	*    inside the QSF file system under a mount point which matches the physical file system structure. This is required
	*    to be able to load in the asset package information file "asset_package.json" in case the asset package is zipped.
	*
	*    The underlying QSF file system supports configuration of whether or not to append the asset package at the end of
	*    the virtual file system search path or  to prepend it (in case of overlapping files the new directory or archive
	*    is the preferred one). In order to keep it simple for our developer team and the mod-community, we don't expose
	*    the "append/prepend"-setting. Asset packages are automatically prepended, meaning asset packages will automatically
	*    "cover/override" other files inside the virtual file system in case of identical file locations.
	*/
	class QSF_API_EXPORT AssetPackage : public BaseAssetPackage
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class Project;	// Only projects can create asset package instances


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const std::string							FILE_NAME;			///< Asset package file name without file extension ("asset_package")
		typedef ElementManager<GlobalAssetId, Asset>		AssetManager;		///< Key = global asset ID
		typedef ElementManager<GlobalAssetId, CachedAsset>	CachedAssetManager;	///< Key = global asset ID
		typedef AssetManager::ElementMap					AssetMap;			///< Key = global asset ID
		typedef CachedAssetManager::ElementMap				CachedAssetMap;		///< Key = global asset ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~AssetPackage();

		/**
		*  @brief
		*    Return the project this asset package is in
		*
		*  @return
		*    Project this asset package is in, do not destroy the instance
		*/
		inline Project& getProject() const;

		/**
		*  @brief
		*    Return the whether or not this is an intermediate asset package
		*
		*  @return
		*    "true" if this is an intermediate asset package, else "false"
		*
		*  @note
		*    - An intermediate asset package is technically just like an ordinary asset package, but with the use case of just being used for processing (e.g. asset import inside the editor)
		*/
		inline bool isIntermediate() const;

		/**
		*  @brief
		*    Clear the asset package
		*
		*  @note
		*    - In case the asset package is currently mounted, it's unmounted automatically
		*/
		void clear();

		/**
		*  @brief
		*    Load an asset package by using a given filename
		*
		*  @param[in] filename
		*    UTF-8 filename in platform-independent notation of the asset package to load
		*
		*  @return
		*    "true" if all went fine, else "false" (file not found?)
		*
		*  @note
		*    - The current asset package will get lost, so, you might want to save a manipulated asset package before loading a new one
		*/
		bool loadByFilename(const std::string& filename);

		/**
		*  @brief
		*    Save the asset package by using a given filename
		*
		*  @param[in] filename
		*    UTF-8 filename in platform-independent notation of the asset package to save
		*
		*  @return
		*    "true" if all went fine, else "false"
		*/
		bool saveByFilename(const std::string& filename);

		/**
		*  @brief
		*    Return the origin name of the asset package
		*
		*  @return
		*    The UTF-8 origin name of the asset package
		*
		*  @remarks
		*    In case the asset package is stored inside a directory in the file system the origin name is identical
		*    to the asset package name returned by "qsf::AssetPackage::getName()". If the asset package originates
		*    from an archive the name of the archive will be returned (e.g. "my_asset_package.zip").
		*/
		std::string getOriginName() const;

		/**
		*  @brief
		*    Change an asset's global asset ID
		*
		*  @param[in] oldGlobalAssetId
		*    Global asset ID to replace, identifies the asset whose ID to change
		*  @param[in] newGlobalAssetId
		*    Global asset ID to set as new global asset ID for the asset
		*
		*  @return
		*    "true" if all went fine, "false" if the asset with the given old global asset ID does not exist
		*/
		bool changeGlobalAssetId(GlobalAssetId oldGlobalAssetId, GlobalAssetId newGlobalAssetId);

		//[-------------------------------------------------------]
		//[ Properties                                            ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Set the location in the interpolated virtual file system tree that this asset package will be "mounted"
		*
		*  @param[in] mountPoint
		*    UTF-8 location in the interpolated virtual file system tree that this asset package will be "mounted", "" is equivalent to "/"
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*    - Changing the mount point of the asset package is only possible if the asset package is currently not mounted
		*/
		void setMountPoint(const std::string& mountPoint);

		//[-------------------------------------------------------]
		//[ Content                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the local filename of the asset package
		*
		*  @return
		*    The local UTF-8 filename in platform-independent notation of the asset package
		*/
		inline std::string getFilename() const;

		/**
		*  @brief
		*    Set the local filename of the asset package
		*
		*  @param[in] filename
		*    The local UTF-8 filename in platform-independent notation of the asset package
		*/
		inline void setFilename(const std::string& filename);

		/**
		*  @brief
		*    Return the physical relative directory of the asset package
		*
		*  @return
		*    The UTF-8 relative directory name in platform-independent notation (e.g. on Windows: "data/my_project/my_asset_package")
		*/
		std::string getRelativeDirectory() const;

		/**
		*  @brief
		*    Return the asset manager of this asset package
		*
		*  @return
		*    The internal manager of assets of this asset package, do not destroy the instance, the global asset ID is used as key
		*/
		inline AssetManager& getAssetManager();
		inline const AssetManager& getAssetManager() const;

		/**
		*  @brief
		*    Return an asset by using a given global asset ID
		*
		*  @param[in] globalAssetId
		*    Global asset ID of the asset to return
		*
		*  @return
		*    Corresponding asset, null pointer on error, do not destroy the instance
		*
		*  @note
		*    - Only works for assets currently mounted inside the asset system
		*/
		inline Asset* getAssetByGlobalAssetId(GlobalAssetId globalAssetId) const;

		/**
		*  @brief
		*    Return an asset by using a given local asset ID
		*
		*  @param[in] localAssetId
		*    Local asset ID of the asset to return
		*
		*  @return
		*    Corresponding asset, null pointer on error, do not destroy the instance
		*
		*  @note
		*    - Also works if the asset package is currently not mounted
		*    - This method is ineffective, don't use it if you don't have to - and especially don't use it inside performance critical situations
		*/
		Asset* getAssetByLocalAssetId(LocalAssetId localAssetId) const;

		/**
		*  @brief
		*    Return the cached asset manager of this asset package
		*
		*  @return
		*    The internal manager of cached assets of this asset package, do not destroy the instance, the global asset ID is used as key
		*/
		inline CachedAssetManager& getCachedAssetManager();
		inline const CachedAssetManager& getCachedAssetManager() const;

		/**
		*  @brief
		*    Return an cached asset by using a given global asset ID
		*
		*  @param[in] globalAssetId
		*    Global asset ID of the cached asset to return
		*
		*  @return
		*    Corresponding cached asset, null pointer on error, do not destroy the instance
		*
		*  @note
		*    - Only works for assets currently mounted inside the asset system
		*/
		inline CachedAsset* getCachedAssetByGlobalAssetId(GlobalAssetId globalAssetId) const;

		/**
		*  @brief
		*    Return all assets of a given type
		*
		*  @param[in]  assetTypeId
		*    ID of the asset type to return all assets from
		*  @param[out] assets
		*    Will receive all assets of the given asset type, list is not cleared before new entities are added
		*/
		void getAssetsOfType(AssetTypeId assetTypeId, std::vector<Asset*>& assets) const;

		/**
		*  @brief
		*    Destroy an asset by using the global asset ID
		*
		*  @param[in] globalAssetId
		*    Global asset ID of the asset to destroy
		*
		*  @return
		*    "true" if all went fine, else "false"
		*
		*  @note
		*    - Destroys the asset, cached asset and also unregisters it in case the asset package is currently mounted
		*/
		bool destroyAssetByGlobalAssetId(GlobalAssetId globalAssetId);

		//[-------------------------------------------------------]
		//[ Mounting                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return whether or not the asset package is currently mounted
		*
		*  @return
		*    "true" if the asset package is currently mounted, else "false"
		*/
		inline bool isMounted() const;

		/**
		*  @brief
		*    Mount the asset package
		*
		*  @return
		*    "true" if all went fine, else "false" (maybe already mounted?)
		*/
		bool mount();

		/**
		*  @brief
		*    Unmount the asset package
		*
		*  @return
		*    "true" if all went fine, else "false" (maybe not mounted?)
		*/
		bool unmount();

		/**
		*  @brief
		*    Mount an asset given by the global asset ID
		*
		*  @param[in] globalAssetId
		*    Global asset ID of the asset to mount
		*
		*  @note
		*    - Don't use this method unless you know exactly what you're doing
		*    - Only mounts the asset in case the asset package is currently mounted
		*/
		inline void mountAssetByGlobalAssetId(GlobalAssetId globalAssetId);

		/**
		*  @brief
		*    Unmount an asset given by the global asset ID
		*
		*  @param[in] globalAssetId
		*    Global asset ID of the asset to unmount
		*
		*  @note
		*    - Don't use this method unless you know exactly what you're doing
		*    - Only unmounts the asset in case the asset package is currently mounted
		*/
		void unmountAssetByGlobalAssetId(GlobalAssetId globalAssetId);

		/**
		*  @brief
		*    Update a cached asset's data filename
		*
		*  @param[in] cachedAsset
		*    The cached asset to update
		*/
		void updateCachedAssetDataFilename(CachedAsset& cachedAsset);

		//[-------------------------------------------------------]
		//[ Filename transformation                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Transform a given absolute asset filename into a local asset name
		*
		*  @param[in] absoluteFilename
		*    Absolute UTF-8 filename in platform-independent notation to convert to an local asset name
		*
		*  @return
		*    The local UTF-8 asset name, might be an invalid local asset name on error
		*/
		std::string absoluteFilenameToLocalAssetName(const std::string& absoluteFilename) const;

		/**
		*  @brief
		*    Generate an relative filename for the given global asset ID
		*
		*  @param[in] globalAssetId
		*    Global asset ID of the asset to generate the relative filename for
		*
		*  @return
		*    The generated relative UTF-8 filename for the given global asset ID, empty string if there's e.g. no cached asset available for the asset
		*
		*  @note
		*    - Only works for assets currently mounted inside the asset system
		*/
		std::string generateRelativeFilename(GlobalAssetId globalAssetId) const;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] project
		*    Project this asset package is in, the project instance must stay valid as long as the asset package instance exists
		*  @param[in] intermediate
		*    An intermediate asset package is technically just like an ordinary asset package, but with the use case of just being used for processing (e.g. asset import inside the editor)
		*/
		AssetPackage(Project& project, bool intermediate);

		/**
		*  @brief
		*    Mount an asset
		*
		*  @param[in] asset
		*    Asset to mount
		*/
		void mountAsset(Asset& asset);

		/**
		*  @brief
		*    Mount an asset and emit signal
		*
		*  @param[in] asset
		*    Asset to mount
		*/
		void mountAssetAndSignal(Asset& asset);

		/**
		*  @brief
		*    Unmount an asset
		*
		*  @param[in] asset
		*    Asset to unmount
		*/
		void unmountAsset(Asset& asset);

		/**
		*  @brief
		*    Unmount an asset and emit signal
		*
		*  @param[in] asset
		*    Asset to unmount
		*/
		void unmountAssetAndSignal(Asset& asset);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		AssetSystem&	   mAssetSystem;
		Project&		   mProject;			///< Project this asset package is in, do not destroy the instance
		bool			   mIntermediate;		///<  An intermediate asset package is technically just like an ordinary asset package, but with the use case of just being used for processing (e.g. asset import inside the editor)
		std::string		   mFilename;			///< Local UTF-8 filename in platform-independent notation of the asset package
		AssetManager	   mAssetManager;		///< Asset manager of this asset package, the global asset ID is used as key
		CachedAssetManager mCachedAssetManager;	///< Cached asset manager of this asset package, the global asset ID is used as key
		bool			   mMounted;			///< Is the asset package currently mounted?


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
#include "qsf/asset/project/AssetPackage-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::AssetPackage)
