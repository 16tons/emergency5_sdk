// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/System.h"
#include "qsf/asset/AssetSystemTypes.h"

#include <boost/signals2.hpp>
#include <boost/container/flat_map.hpp>
#include <boost/container/flat_set.hpp>

#include <vector>
#include <list>
#include <unordered_map>
#include <unordered_set>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class CachedAsset;
	class AssetPackage;
	class ProjectManager;
	class AssetTypeManager;
	class GlobalAssetIdProvider;
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
	*    Asset system class
	*
	*  @note
	*    - High level resource (please note that we use the term "asset") handling, use the file system for low level file handling
	*    - It's recommended to access assets only via asset proxies (see "qsf::AssetProxy" for details)
	*/
	class QSF_API_EXPORT AssetSystem : public System
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class AssetPackage;	// The asset package mounts/unmounts assets and cached assets


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		struct AssetData
		{
			GlobalAssetId mGlobalAssetId;
			LocalAssetId  mLocalAssetId;
			AssetPackage* mAssetPackage;

			AssetData(GlobalAssetId globalAssetId, LocalAssetId localAssetId, AssetPackage* assetPackage);
		};

		typedef boost::container::flat_map<GlobalAssetId, Asset*>		AssetMap;		///< Key = global asset ID
		typedef boost::container::flat_map<GlobalAssetId, CachedAsset*>	CachedAssetMap;	///< Key = global asset ID
		typedef std::unordered_set<AssetData*>							AssetDataSet;
		typedef std::list<AssetData*>									AssetDataList;
		typedef std::unordered_map<GlobalAssetId, AssetDataList>		AssetDataByGlobalAssetIdMap;
		typedef std::unordered_map<LocalAssetId, AssetDataList>			AssetDataByLocalIdMap;


	//[-------------------------------------------------------]
	//[ Public Boost signals                                  ]
	//[-------------------------------------------------------]
	public:
		boost::signals2::signal<void (const Assets&)> AssetsMounted;	///< This Boost signal is emitted after assets have been mounted; parameter: mounted assets (does not imply that there's also a cached asset)
		boost::signals2::signal<void (const Assets&)> AssetsUnmounted;	///< This Boost signal is emitted after assets have been unmounted; parameter: unmounted assets (does not imply that there's also a cached asset)


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Calculate the hash of a file specified by a given absolute filename
		*
		*  @param[in] absoluteFilename
		*    Absolute UTF-8 filename of the file to calculate the hash for
		*
		*  @return
		*    Calculated hash (example: "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855") as ASCII string, empty string on error
		*/
		static std::string calculateHashFromFileByAbsoluteFilename(const std::string& absoluteFilename);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		AssetSystem();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~AssetSystem();

		/**
		*  @brief
		*    Gather missing dependencies
		*
		*  @param[in] globalAssetId
		*    Global asset ID of the asset to gather missing dependencies for
		*  @param[out] missingDependencies
		*    Receives a list of missing dependency global asset IDs
		*
		*  @note
		*    - In case the current asset has no cached asset, it's considered to be missing
		*/
		void gatherMissingDependencies(GlobalAssetId globalAssetId, boost::container::flat_set<GlobalAssetId>& missingDependencies) const;

		/**
		*  @brief
		*    Load projects as sub-directories of a given path
		*
		*  @param[in] localPath
		*    Local UTF-8 path to search for projects in sub-directories, relative to base directory
		*  @param[out] baseDirectory
		*    Absolute UTF-8 path of base directory
		*
		*  @return
		*    "false" if an error occurs, e.g. invalid paths, "true" if everything went fine (no matter if a project was found or not)
		*/
		bool loadProjectsFromPath(const std::string& localPath, const std::string& baseDirectory);

		//[-------------------------------------------------------]
		//[ Mounted assets                                        ]
		//[-------------------------------------------------------]
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
		Asset* getAssetByGlobalAssetId(GlobalAssetId globalAssetId) const;

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
		CachedAsset* getCachedAssetByGlobalAssetId(GlobalAssetId globalAssetId) const;

		/**
		*  @brief
		*    Return all assets of a given type
		*
		*  @param[in]  assetTypeId
		*    ID of the asset type to return all assets from
		*  @param[out] assets
		*    Will receive all assets of the given asset type, list is not cleared before new entries are added
		*  @param[in]  category
		*    The category is used as an optional filter for the assets to be returned, can be a null pointer
		*/
		void getAssetsOfType(AssetTypeId assetTypeId, std::vector<Asset*>& assets, const std::string* category = nullptr) const;

		/**
		*  @brief
		*    Return the internal "asset data by global asset ID"-map
		*
		*  @note
		*    - Do not use unless there really is no other way
		*/
		inline const AssetDataByGlobalAssetIdMap& getAssetDataByGlobalAssetIdMap() const;

		//[-------------------------------------------------------]
		//[ Manager                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the asset type manager instance
		*
		*  @return
		*    The asset type manager instance, do no destroy the returned instance
		*/
		inline AssetTypeManager& getAssetTypeManager();
		inline const AssetTypeManager& getAssetTypeManager() const;

		/**
		*  @brief
		*    Return the project manager instance
		*
		*  @return
		*    The project manager instance, do no destroy the returned instance
		*/
		inline ProjectManager& getProjectManager();
		inline const ProjectManager& getProjectManager() const;

		//[-------------------------------------------------------]
		//[ ID mapping                                            ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Map the given local asset ID to the corresponding global asset ID
		*
		*  @param[in] localAssetId
		*    Local asset ID to map to the global asset ID
		*
		*  @return
		*    Corresponding global asset ID of the given local asset ID, "qsf::getUninitialized<LocalAssetId>()" if invalid
		*
		*  @note
		*    - Only works for assets currently mounted inside the asset system
		*/
		inline GlobalAssetId mapLocalToGlobalAssetId(LocalAssetId localAssetId) const;

		/**
		*  @brief
		*    Map the given global asset ID to the corresponding local asset ID
		*
		*  @param[in] globalAssetId
		*    Global asset ID to map to the local asset ID
		*
		*  @return
		*    Corresponding local asset ID of the given global asset ID, "qsf::getUninitialized<LocalAssetId>()" if invalid
		*
		*  @note
		*    - Only works for assets currently mounted inside the asset system
		*/
		inline LocalAssetId mapGlobalToLocalAssetId(GlobalAssetId globalAssetId) const;

		/**
		*  @brief
		*    Map the given global asset ID to the asset package which mounted the asset
		*
		*  @param[in] globalAssetId
		*    Global asset ID
		*
		*  @return
		*    Asset package which mounted the asset, do not destroy the instance, null pointer on error
		*
		*  @note
		*    - Only works for assets currently mounted inside the asset system
		*/
		inline AssetPackage* mapGlobalAssetIdToAssetPackage(GlobalAssetId globalAssetId) const;

		/**
		*  @brief
		*    Interpret string global asset ID content as global asset ID
		*
		*  @param[in] string
		*    String containing global asset ID to return as global asset ID
		*
		*  @return
		*    The global asset ID, "qsf::getUninitialized<GlobalAssetId>()" if invalid
		*/
		GlobalAssetId globalAssetIdAsStringToGlobalAssetId(const std::string& string) const;

		/**
		*  @brief
		*    Interpret string global asset ID content as local asset ID
		*
		*  @param[in] string
		*    String containing global asset ID to return as local asset ID
		*
		*  @return
		*    The local asset ID, "qsf::getUninitialized<LocalAssetId>()" if invalid
		*/
		LocalAssetId globalAssetIdAsStringToLocalAssetId(const std::string& string) const;

		//[-------------------------------------------------------]
		//[ Global asset ID provider                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return an unused global asset ID
		*
		*  @return
		*    Unused global asset ID, "qsf::getUninitialized<GlobalAssetId>()" if invalid
		*
		*  @note
		*    - Will only return a valid global asset ID if there's a global asset ID provider set
		*/
		GlobalAssetId acquireGlobalAssetId() const;

		/**
		*  @brief
		*    Set the global asset ID provider
		*
		*  @param[in] globalAssetIdProvider
		*    Global asset ID provider to set, can be a null pointer, the asset system takes over
		*    the memory control and will destroy the given instance as soon as it's no longer required
		*/
		void setGlobalAssetIdProvider(GlobalAssetIdProvider* globalAssetIdProvider);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::System methods                    ]
	//[-------------------------------------------------------]
	public:
		inline virtual const char* getName() const override;
		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual bool onStartup(bool serverMode) override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		template<typename IdType>
		AssetData& getAssetDataById(const std::unordered_map<IdType, AssetDataList>& dataArrayMap, IdType id) const;

		void registerAssetData(GlobalAssetId globalAssetId, LocalAssetId localAssetId, AssetPackage& assetPackage);
		void unregisterAssetData(GlobalAssetId globalAssetId, const AssetPackage& assetPackage);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		AssetDataSet				mAssetDataSet;				///< Set of managed asset data structure, one for each mounted asset; we're responsible for destroying the instances
		AssetDataByLocalIdMap		mAssetDataByLocalId;		///< Map the given local asset ID to the corresponding asset data list. Key is local ID, value is an asset data list.
		AssetDataByGlobalAssetIdMap	mAssetDataByGlobalAssetId;	///< Map the given global asset ID to the corresponding asset data list. Key is global ID, value is an asset data list.
		AssetTypeManager*			mAssetTypeManager;			///< Asset type manager instance, always valid, we're responsible for destroying the instance in case we no longer need it
		ProjectManager*				mProjectManager;			///< Project manager instance, always valid while the asset system is up-and-running, we're responsible for destroying the instance in case we no longer need it
		GlobalAssetIdProvider*		mGlobalAssetIdProvider;		///< Global asset ID provider to set, can be a null pointer, destroy the instance in case you no longer need it


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/asset/AssetSystem-inl.h"
