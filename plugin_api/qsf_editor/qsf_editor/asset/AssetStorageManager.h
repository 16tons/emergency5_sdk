// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/asset/AssetStorageManagerTypes.h"

#include <qsf_editor_base/asset/AssetStorageManager.h>
#include <qsf_editor_base/network/NetworkTypes.h>
#include <qsf_editor_base/network/EditorCampStructs.h>

#include <qsf/asset/AssetSystemTypes.h>
#include <qsf/job/JobProxy.h>

#include <boost/function/function_fwd.hpp>
#include <boost/signals2/connection.hpp>

#include <QtCore/qobject.h>

#include <unordered_map>
#include <unordered_set>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class AssetPackage;
	class AssetSystem;
	class BaseAssetPackage;
	class BaseProject;
	class Project;
	class CachedAsset;
	class FileSystem;
	namespace editor
	{
		namespace base
		{
			class AssetIssue;
		}
		class AssetClient;
		class EditorClient;
		class EditorSettingsGroup;
		class OnlineSettingsGroup;
		class AssetPriorityProvider;
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
		*    QSF editor asset storage manager
		*
		*  @remarks
		*    Gateway to the QSF asset server manager which has the control over certain locally cached files like:
		*     <base>\temp\asset\86748 *asset dump* (content, attached files etc.)
		*     <base>\temp\asset\42 *asset dump* (content, attached files etc.)
		*     <base>\temp\incoming\
		*     <base>\temp\outgoing\
		*
		*    == Local Asset Datasource ==
		*    The asset processing pipeline has to work on source assets. When using the offline version of the QSF editor, the QSF editor needs to know
		*    where to find the source assets. For example, the QSF editor will convert tif-textures into compressed textures which can be used in an
		*    efficient way during runtime. For this to work, the location of the source tif texture has to be known. Each asset stores a relative path
		*    to a local datasource directory it was imported from ("DatasourcePath" asset property). For example, when importing a mesh asset from the
		*    "em5datasource" SVN repository, each imported depending asset will store a path relative to the "em5datasource" datasource.
		*
		*    During asset import, the QSF will automatically locate local asset datasources by looking for known version-control subdirectories (".svn" for Subversion, ".git" for GIT). Example:
		*    - Imported mesh asset "D:\Projects\SVN\em5\_datasource\em5datasource\data\mesh\building\building_airport_hangar02.mesh"
		*    - "D:\Projects\SVN\em5\_datasource\em5datasource\" will be detected as local asset datasource because it has a ".svn" subdirectory
		*    - In case there's no local asset datasource named "em5datasource" currently registered inside the QSF editor settings, the QSF editor will
		*      ask the user whether or not an local asset datasource "em5datasource" pointing to the local absolute directory "D:\Projects\SVN\em5\_datasource\em5datasource\"
		*      should be added. The asset import will fail if the user rejects.
		*
		*    Inside the QSF editor, the local asset datasources are saved inside "qsf::editor::EditorSettingsGroup::LocalAssetDatasources". The content
		*    of the semicolon-separated string looks like the following: "datasource_A;C:/path/to/datasource/;datasource_B;C:/some/other/path/"
		*/
		class AssetStorageManager : public QObject, public base::AssetStorageManager
		{


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
			friend class AssetProtocol;
			friend class EditorProtocol;


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
					// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const std::string TEMP_TRANSFER_DIRECTORY;		///< UTF-8 name of the local directory in platform-independent notation where pending non-source asset transfers are stored
			static const std::string TEMP_SOURCE_ASSET_DIRECTORY;	///< UTF-8 name of the local directory in platform-independent notation where uncompressed source assets (including attachments) are stored to be edited by the user
			static const std::string TEMP_MAP_SNAPSHOT_FILENAME;	///< UTF-8 name of the local filename in platform-independent notation where the temporary map snapshot is stored
			static const std::string CACHE_SOURCE_ZIP_DIRECTORY;	///< UTF-8 name of the local directory in platform-independent notation where compressed source assets are stored (cache & transfers)

			typedef boost::function<void (GlobalAssetId, bool)> AssetRequestCallback;


		//[-------------------------------------------------------]
		//[ Public Qt signals (MOC)                               ]
		//[-------------------------------------------------------]
		Q_SIGNALS:
			//[-------------------------------------------------------]
			//[ Asset transfer                                        ]
			//[-------------------------------------------------------]
			void transferStarted(AssetTransferType type, GlobalAssetId globalAssetId);
			void transferProgress(AssetTransferType type, GlobalAssetId globalAssetId, uint64 current, uint64 total);
			void transferCanceled(AssetTransferType type, GlobalAssetId globalAssetId);
			void transferFinished(AssetTransferType type, GlobalAssetId globalAssetId);

			//[-------------------------------------------------------]
			//[ Asset editing                                         ]
			//[-------------------------------------------------------]
			void assetLock(GlobalAssetId globalAssetId, uint32 userId);	// Called after asset was locked on the server
			void assetUnlock(GlobalAssetId globalAssetId);	// Called after asset was unlocked on the server
			void sourceDataPathReady(GlobalAssetId globalAssetId, const std::string& path);

			//[-------------------------------------------------------]
			//[ Project                                               ]
			//[-------------------------------------------------------]
			void projectCreated(const Project& project);	///< Emitted after creation
			void projectDeleted(ProjectId projectId);	///< Emitted after destruction
			void projectEdited(const Project& project);		///< Emitted after editing

			//[-------------------------------------------------------]
			//[ Asset package                                         ]
			//[-------------------------------------------------------]
			void assetPackageCreated(const AssetPackage& assetPackage);	///< Emitted after creation
			void assetPackageDeleted(ProjectId projectId, const std::string& assetPackageName);	///< Emitted after destruction
			void assetPackageEdited(const AssetPackage& assetPackage);	///< Emitted after editing

			//[-------------------------------------------------------]
			//[ Asset maintenance                                     ]
			//[-------------------------------------------------------]
			void assetUpdate(GlobalAssetId globalAssetId);	// Called after asset was updated in the asset system
			void assetDelete(GlobalAssetId globalAssetId);	// Called after asset was flagged as deleted in the asset system
			void assetDeleteRequestResult(GlobalAssetId globalAssetId, bool success, const std::vector<GlobalAssetId>& dependencyList); // Called after we have received an asset deletion response from the asset server
			void receiveAssetIssueList(const std::vector<std::shared_ptr<base::AssetIssue>>& issueList);

			//[-------------------------------------------------------]
			//[ Asset sync                                            ]
			//[-------------------------------------------------------]
			void syncStarted();
			void syncProgress(size_t pendingUpdates, size_t totalTransfers, size_t remainingTransfers);
			void syncCanceled();
			void syncFinished();


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] assetPriorityProvider
			*    Asset priority provider instance to use, must stay valid as long as this asset storage manager instance exists
			*/
			explicit AssetStorageManager(AssetPriorityProvider& assetPriorityProvider);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~AssetStorageManager();

			//[-------------------------------------------------------]
			//[ Lifecycle                                             ]
			//[-------------------------------------------------------]
			bool onStartup();
			void onShutdown();
			inline bool isRunning() const;
			inline bool isAssetDbLite() const;

			//[-------------------------------------------------------]
			//[ Global asset ID provider                              ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Return an unused global asset ID
			*
			*  @return
			*    Unused global asset ID, "qsf::getUninitialized<GlobalAssetId>()" if invalid
			*/
			GlobalAssetId acquireGlobalAssetId();

			/**
			*  @brief
			*    Free a global asset ID
			*
			*  @param[in] globalAssetId
			*    Global asset ID to free
			*/
			inline void freeGlobalAssetId(GlobalAssetId globalAssetId);

			//[-------------------------------------------------------]
			//[ Entity ID provider                                    ]
			//[-------------------------------------------------------]
			bool requestFirstEntityIds();

			/**
			*  @brief
			*    Return an unused entity ID
			*
			*  @return
			*    Unused entity ID, "qsf::getUninitialized<uint64>()" if invalid
			*/
			uint64 acquireEntityId();

			/**
			*  @brief
			*    Free an entity ID
			*
			*  @param[in] entityId
			*    Entity ID to free
			*/
			inline void freeEntityId(uint64 entityId);

			//[-------------------------------------------------------]
			//[ Asset editing                                         ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Try to acquire a server lock on the asset for editing
			*
			*  @param[in] globalAssetId
			*    Global asset ID of the asset to lock for editing
			*  @param[in] callback
			*    Callback to call when the result of the locking attempt is known
			*
			*  @note
			*    - Result is not returned but delivered through the supplied callback:
			*        void callback(GlobalAssetId globalAssetId, bool success)
			*    - Don't forget to unlock the asset using unlockAssetForEditing(...)
			*/
			void requestLockAssetForEditing(GlobalAssetId globalAssetId, const AssetRequestCallback& callback);

			/**
			*  @brief
			*    Release a server lock on the asset after editing
			*
			*  @param[in] globalAssetId
			*    Global asset ID of the asset to unlock after editing
			*/
			void unlockAssetForEditing(GlobalAssetId globalAssetId);

			/**
			*  @brief
			*    Destroy the asset with the given global asset ID
			*
			*  @param[in] globalAssetId
			*    Global asset ID of the asset to destroy
			*
			*  @return
			*    "true" if all went fine, else "false"
			*
			*  @note
			*    - Destroys the asset, cached asset and also unregisters it in case the asset package is currently mounted
			*    - Emits the "qsf::editor::AssetStorageManager::assetDeleteRequestResult(GlobalAssetId globalAssetId, bool success, const std::vector<GlobalAssetId>& dependencyList)"
			*      signal as soon as the client receives the response from the asset server
			*/
			bool destroyAssetByGlobalAssetId(GlobalAssetId globalAssetId);

			/**
			*  @brief
			*    Destroy the cached asset of the asset with the given global asset ID
			*
			*  @param[in] globalAssetId
			*    Global asset ID of the asset whose cached asset to destroy
			*
			*  @return
			*    "true" if all went fine, else "false"
			*/
			bool destroyCachedAssetByGlobalAssetId(GlobalAssetId globalAssetId);

			void listAssetLocks(boost::container::flat_map<GlobalAssetId, uint32>& assetLocks) const;

			bool isAssetLocked(GlobalAssetId globalAssetId, uint32* lockedBy = nullptr) const;

			//[-------------------------------------------------------]
			//[ Asset maintenance                                     ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Request the asset issue list
			*
			*  @remarks
			*    In online mode the asset storage manager will ask the server to provide its complete asset issue list.
			*    In offline mode it will collect a local asset issue list and directly call through to the signal.
			*
			*  @note
			*    - As soon as a asset issue list is available the signal "receiveAssetIssueList" will be emitted.
			*/
			void requestAssetIssueList();

			//[-------------------------------------------------------]
			//[ Asset transfer                                        ]
			//[-------------------------------------------------------]
			bool startSync(bool isSyncAll);

			/**
			*  @brief
			*    Immediately start downloading specified cached asset
			*
			*  @param[in] projectId
			*    ID of the project containing the cached asset to download
			*  @param[in] assetPackageName
			*    name of the asset package containing the cached asset to download
			*  @param[in] globalAssetId
			*    Global asset ID of the cached asset to download
			*  @param[in] cachedAssetRevision
			*    Revision of the cached asset to download
			*
			*  @note
			*    - Use the transferXXX() signals to track the download's progress
			*/
			void downloadCachedAsset(ProjectId projectId, const std::string& assetPackageName, GlobalAssetId globalAssetId, AssetRevision cachedAssetRevision);

			/**
			*  @brief
			*    Queue specified cached asset for downloading
			*
			*  @param[in] projectId
			*    ID of the project containing the cached asset to download
			*  @param[in] assetPackageName
			*    name of the asset package containing the cached asset to download
			*  @param[in] globalAssetId
			*    Global asset ID of the cached asset to download
			*  @param[in] cachedAssetRevision
			*    Revision of the cached asset to download
			*
			*  @note
			*    - Use the transferXXX() signals to track the download's progress
			*/
			void downloadCachedAssetInBackground(ProjectId projectId, const std::string& assetPackageName, GlobalAssetId globalAssetId, AssetRevision cachedAssetRevision);

			/**
			*  @brief
			*    Immediately start downloading specified asset source
			*
			*  @param[in] globalAssetId
			*    Global asset ID of the asset to download
			*  @param[in] revision
			*    Revision of the asset to download
			*
			*  @note
			*    - Use the transferXXX() signals to track the download's progress
			*/
			inline void downloadAssetSource(GlobalAssetId globalAssetId, AssetRevision revision);

			/**
			*  @brief
			*    Cancel specified asset source download
			*
			*  @param[in] globalAssetId
			*    Global asset ID of the asset to cancel the download for
			*  @param[in] revision
			*    Revision of the asset to cancel the download for
			*
			*  @note
			*    - You may call this method when you asked for asset source files, but the reason you needed them went away since then
			*/
			void cancelDownloadAssetSource(GlobalAssetId globalAssetId, AssetRevision revision);

			/**
			*  @brief
			*    Immediately start uploading specified asset and its source data
			*
			*  @param[in] asset
			*    Asset to upload (can be deleted afterwards)
			*
			*  @remarks
			*    The asset storage manager will push the new asset version to the QSF editor server, which in turn will notify the editor once the asset becomes
			*    available for placement (which will be almost immediately after calling this method).
			*
			*    The method expects that the asset source data is already inside the "<qsf::AssetStorageManager::TEMP_ASSET_DIRECTORY>/<global asset ID>"
			*    directory. This method creates a compressed copy of the directory so it can safely be modified and/or deleted afterwards.
			*
			*  @note
			*    - Use the transferXXX() signals to track the uploads's progress
			*    - When updating an existing asset make sure you have successfully acquired a lock prior to calling this method!
			*    - The Revision, Author and DateModified properties of the asset will be set by the server regardless of the values provided in this asset
			*/
			bool uploadAssetSource(const Asset& asset, const CachedAsset* cachedAsset = nullptr);

			/**
			*  @brief
			*    Upload meta-only changes to the specified asset
			*
			*  @param[in] asset
			*    Asset to upload (can be deleted afterwards)
			*
			*  @remarks
			*    The asset storage manager will push the new asset version to the QSF editor server, which in turn will notify the editor once the asset becomes
			*    available for placement (which will be almost immediately after calling this method).
			*
			*  @note
			*    - When updating an existing asset make sure you have successfully acquired a lock prior to calling this method!
			*    - The Revision, Author and DateModified properties of the asset will be set by the server regardless of the values provided in this asset
			*/
			bool uploadAssetMetaOnly(const Asset& asset, const CachedAsset* cachedAsset = nullptr);

			/**
			*  @brief
			*    Check if the cached asset with the given global asset ID is currently outdated (=scheduled for download)
			*
			*  @param[in] globalAssetId
			*    Global asset ID of the asset
			*  @param[in] assetPackage
			*    The asset package that contains the asset. Provide this optional parameter if you already have it, so we can avoid that not too cheap lookup.
			*/
			bool isCachedAssetOutdated(GlobalAssetId globalAssetId, const AssetPackage* assetPackage = nullptr) const;

			/**
			*  @brief
			*    Update the asset in the asset system by assigning the modified asset copy
			*
			*  @param[in] modifiedAssetCopy
			*    The modified asset copy to submit to the server/asset system
			*  @param[in] uploadSourceAsset
			*    Provide "true" if the source asset from the temporary directory should be submitted with this change, "false" otherwise
			*
			*  @remarks
			*    Submits the changes to the server when in online mode, applies changes locally if not. Updates Author, DateModified and Revision accordingly.
			*
			*  @note
			*    - Before calling this method ensure that you successfully received a lock ("qsf::editor::AssetStorageManager::requestLockAssetForEditing()") of this asset, else the editor server will disconnect the client
			*/
			bool submitAssetChanges(Asset& modifiedAssetCopy, bool uploadSourceAsset, const CachedAsset* cachedAsset = nullptr, AssetPackage* targetAssetPackage = nullptr);

			size_t countPendingAssetSourceUploads() const;
			size_t countPendingAssetSourceDownloads() const;
			size_t countPendingCachedAssetDownloads() const;

			//[-------------------------------------------------------]
			//[ Project management                                    ]
			//[-------------------------------------------------------]
			bool createProject(const BaseProject& project);
			bool deleteProject(ProjectId projectId);
			bool editProject(const Project& project);

			//[-------------------------------------------------------]
			//[ Asset package management                              ]
			//[-------------------------------------------------------]
			bool createAssetPackage(ProjectId projectId, const BaseAssetPackage& assetPackage);
			bool deleteAssetPackage(ProjectId projectId, const std::string& assetPackageName);
			bool editAssetPackage(ProjectId projectId, const BaseAssetPackage& assetPackage);

			/**
			*  @brief
			*    Moves an asset from one asset package to another (possibly across projects, although this breaks the local asset names in that case...)
			*
			*  @param[in] keepSource
			*    Keep the source? With this set to "true" the method behaves like a "copy to asset package" (copy has the same global asset ID)
			*
			*  @note
			*    - Attention: ZIP-archive asset packages are currently unsupported!
			*/
			bool moveAssetBetweenAssetPackages(ProjectId sourceProjectId, const std::string& sourceAssetPackageName, ProjectId destinationProjectId, const std::string& destinationAssetPackageName, GlobalAssetId globalAssetId, bool keepSource = false);

			/** This method only intended for use in online mode */
			bool addAssetToAssetPackage(ProjectId projectId, const std::string& assetPackageName, GlobalAssetId globalAssetId, AssetPackage* sourceAssetPackage = nullptr);
			bool removeAssetFromAssetPackage(ProjectId projectId, const std::string& assetPackageName, GlobalAssetId globalAssetId);

			void notifyModifiedAssetPackage(AssetPackage* assetPackage);
			void notifyModifiedAssetPackage(ProjectId projectId, const std::string& assetPackageName);

			//[-------------------------------------------------------]
			//[ Utility                                               ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Return the path of the local directory where the temporary local source asset is stored
			*
			*  @param[in] globalAssetId
			*    Global asset ID of the asset to return the local directory for
			*  @param[in] revision
			*    Revision of the asset to return the local directory for
			*  @param[in] create
			*    Whether to create the directory if it doesn't exist yet or not
			*
			*  @return
			*    Relative UTF-8 name of the local directory in platform-independent notation where the temporally local source asset is stored to be edited by the user, empty string on error
			*/
			std::string getTempAssetDirectoryByGlobalAssetId(GlobalAssetId globalAssetId, AssetRevision revision, bool create);

			/**
			*  @brief
			*    Validate or add local asset datasource
			*
			*  @param[in] filename
			*    Absolute UTF-8 asset data filename to check for or add a local asset datasource
			*
			*  @return
			*    UTF-8 portable datasource path consisting of the datasource identifier and the path within the datasource, empty string on error or if no match was found.
			*/
			std::string validateOrAddLocalDatasource(const std::string& absoluteFilename) const;

			/**
			*  @brief
			*    Check if the absolute path points within any configured local datasource
			*
			*  @param[in] absolutePath
			*    Absolute path to match to the local datasource list.
			*
			*  @return
			*    UTF-8 portable datasource path consisting of the datasource identifier and the path within the datasource, empty string on error or if no match was found.
			*
			*  @remarks
			*    Checks if the absolute path points within any configured local datasource.
			*    If so returns a portable datasource path consisting of the datasource identifier and the path within the datasource.
			*    Example: "em5datasource/data/mesh/object/bench_01/export/bench_01_c.tif"
			*
			*  @note
			*    - The passed absolute path must only use forward slashes ("/") as path separators.
			*/
			std::string absoluteToDatasourcePath(const std::string& absolutePath) const;

			std::string datasourceToAbsolutePath(const std::string& datasourcePath) const;
			std::string getAbsoluteAssetDatasourcePath(GlobalAssetId globalAssetId) const;

			std::string scanLocalDatasourcesForAsset(GlobalAssetId globalAssetId) const;

			std::string getAssetSourcePath(Asset& asset);

			//[-------------------------------------------------------]
			//[ Source asset cache                                    ]
			//[-------------------------------------------------------]
			bool createSourceAssetDirectory(GlobalAssetId globalAssetId, AssetRevision revision);

			bool createSourceAssetZipFromDirectory(GlobalAssetId globalAssetId, AssetRevision revision);
			bool createSourceAssetDirectoryFromZip(GlobalAssetId globalAssetId, AssetRevision revision);

			void deleteSourceAssetZip(GlobalAssetId globalAssetId, AssetRevision revision);
			void deleteSourceAssetDirectory(GlobalAssetId globalAssetId, AssetRevision revision);

			std::string getSourceAssetZipPath(GlobalAssetId globalAssetId, AssetRevision revision);
			std::string getSourceAssetDirectoryPath(GlobalAssetId globalAssetId, AssetRevision revision);

			void requestAssetSourceDataPath(GlobalAssetId globalAssetId);


		//[-------------------------------------------------------]
		//[ Public virtual qsf::editor::base::AssetStorageManager methods ]
		//[-------------------------------------------------------]
		public:
			virtual std::string getAssetSourcePath(GlobalAssetId globalAssetId) override;
			virtual AssetRevision getAssetSourceRevision(GlobalAssetId globalAssetId) const override;
			virtual bool getAssetsDerivedFrom(const BaseCachedAsset& derivedFrom, std::unordered_map<uint32, DerivedAssetInfo>& derivedAssetList) override;
			virtual bool deriveAssetsFrom(BaseCachedAsset& deriveFrom, std::unordered_map<uint32, DerivedAssetInfo>& derivedAssetList) override;
			virtual bool commitDerivedAssets(BaseCachedAsset& derivedFrom, std::unordered_map<uint32, DerivedAssetInfo>& derivedAssetList) override;
			virtual const BaseAsset* getAssetInfo(GlobalAssetId globalAssetId) const override;


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			void downloadAssetSource(GlobalAssetId globalAssetId, AssetRevision revision, bool requestedAssetSource);
			void downloadCachedAssetInBackgroundSync(ProjectId projectId, const std::string& assetPackageName, GlobalAssetId globalAssetId, AssetRevision cachedAssetRevision);

			void updateSyncProgress();

			void pushUnusedGlobalAssetIds(const std::vector<GlobalAssetId>& unusedGlobalAssetIds);
			void pushUnusedEntityIds(const std::vector<uint64>& unusedEntityIds);

			void pushAssetLock(GlobalAssetId globalAssetId, uint32 userId);
			void pushAssetUnlock(GlobalAssetId globalAssetId);
			void pushAssetLockRequestResult(GlobalAssetId globalAssetId, bool success);
			void pushAssetUpdate(Asset& asset);
			void pushAssetDelete(GlobalAssetId globalAssetId);
			void pushEditorAssetUpdate(ProjectId projectId, const std::string& assetPackageName, const BaseAsset& asset, AssetRevision cachedAssetRevision);
			void pushAssetDeleteRequestResult(GlobalAssetId globalAssetId, bool success, const std::vector<GlobalAssetId>& dependencyList);

			//[-------------------------------------------------------]
			//[ Project management                                    ]
			//[-------------------------------------------------------]
			void pushProjectList(const std::vector<base::EditorProjectProperties>& projectList);
			void pushCreateProject(const BaseProject& project);
			void pushDeleteProject(ProjectId projectId);
			void pushEditProject(const BaseProject& project);

			//[-------------------------------------------------------]
			//[ Asset package management                              ]
			//[-------------------------------------------------------]
			void pushAssetPackageList(ProjectId projectId, const std::vector<base::EditorAssetPackageContainer>& assetPackageList);
			bool pushCreateAssetPackage(ProjectId projectId, const BaseAssetPackage& assetPackage);
			bool pushDeleteAssetPackage(ProjectId projectId, const std::string& assetPackageName);
			bool pushEditAssetPackage(ProjectId projectId, const BaseAssetPackage& assetPackage);
			bool pushRemoveAssetFromPackage(GlobalAssetId globalAssetId, ProjectId projectId, const std::string& assetPackageName);

			std::string getExpectedSourceAssetFilename(const Asset& asset) const;

			inline std::string getTempTransferPath(GlobalAssetId globalAssetId) const;
			inline std::string getTempSourceAssetDirectoryPath(GlobalAssetId globalAssetId, AssetRevision revision) const;
			inline std::string getCacheSourceAssetZipPath(GlobalAssetId globalAssetId, AssetRevision revision) const;

			/**
			*  @brief
			*   Destroy an asset locally by using the global asset ID
			*
			*  @param[in] globalAssetId
			*    Global asset ID of the asset to destroy
			*
			*  @return
			*    "true" if all went fine, else "false"
			*
			*  @note
			*    - Called after server response. Do not call directly. Use "qsf::editor::AssetStorageManager::destroyAssetByGlobalAssetId()" instead.
			*    - Destroys the asset, cached asset and also unregisters it in case the asset package is currently mounted
			*/
			bool destroyAssetLocally(GlobalAssetId globalAssetId);

			bool applyLocalAssetChanges(AssetPackage& assetPackage, const Asset& newAssetRevision, const CachedAsset* newCachedAsset = nullptr);


		//[-------------------------------------------------------]
		//[ Private definitions                                   ]
		//[-------------------------------------------------------]
		private:
			struct AssetTransferInfo
			{
				AssetTransferType type;
				GlobalAssetId globalAssetId;
				AssetRevision revision;
				bool requestedAssetSource;
			};

			struct EditorTransferInfo
			{
				ProjectId projectId;
				std::string assetPackageName;
				GlobalAssetId globalAssetId;
				AssetRevision cachedAssetRevision;
			};

			struct ProjectPackageAssignment
			{
				ProjectId projectId;
				std::string assetPackageName;

				inline ProjectPackageAssignment() : projectId(getUninitialized<ProjectId>()) {}
				inline ProjectPackageAssignment(ProjectId projectId, const std::string& assetPackageName) : projectId(projectId), assetPackageName(assetPackageName) {}

				inline bool operator<(const ProjectPackageAssignment& rhs) const
				{
					return (projectId < rhs.projectId || (projectId == rhs.projectId && assetPackageName < rhs.assetPackageName));
				}
			};

			struct SourceAssetCacheEntry
			{
				bool hasDirectory;
				bool hasZip;
			};

			typedef std::pair<GlobalAssetId, AssetRevision> SourceAssetCacheKey;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void deleteOnLock(GlobalAssetId globalAssetId, bool isSuccess);

			void onAssetUpdate(ProjectId projectId, const std::string& assetPackgeName, GlobalAssetId globalAssetId, AssetRevision cachedAssetRevision);

			void startNextEditorCachedAssetTransfer();

			//[-------------------------------------------------------]
			//[ Source asset cache                                    ]
			//[-------------------------------------------------------]
			void registerSourceAssetZip(GlobalAssetId globalAssetId, AssetRevision revision);

			//[-------------------------------------------------------]
			//[ Slots                                                 ]
			//[-------------------------------------------------------]
			void onAssetTransferStarted(base::NetworkTransferId transferId);
			void onAssetTransferProgress(base::NetworkTransferId transferId, uint64 current, uint64 total);
			void onAssetTransferFinished(base::NetworkTransferId transferId, const std::string& fileExtension);
			void onAssetTransferCanceled(base::NetworkTransferId transferId);

			void onAssetsMounted(const Assets& assets);
			void onAssetsUnmounted(const Assets& assets);

			/**
			*  @brief
			*    Main update function that is passed to the job manager
			*
			*  @param[in] jobArguments
			*    Job arguments
			*/
			void updateJob(const JobArguments& jobArguments);

			/**
			*  @brief
			*    Show a temporary status bar message
			*
			*  @param[in] message
			*    Temporary message to show
			*  @param[in] timeout
			*    Duration in milliseconds to show the message, can be 0 for no timeout
			*/
			void showMessage(const QString& message, int timeout = 2000) const;

			/**
			*  @brief
			*    Show a message informing the user about the failure of acquiring an asset lock
			*
			*  @param[in] globalAssetId
			*    Global asset ID of the asset we failed to get the lock for
			/*/
			void showLockFailureMessage(GlobalAssetId globalAssetId) const;


		//[-------------------------------------------------------]
		//[ Private Qt Slots                                      ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onEditorTransferStarted(base::NetworkTransferId transferId);
			void onEditorTransferProgress(base::NetworkTransferId transferId, uint64 current, uint64 total);
			void onEditorTransferFinished(base::NetworkTransferId transferId, const std::string& fileExtension, const BaseCachedAsset& baseCachedAsset);
			void onEditorTransferCanceled(base::NetworkTransferId transferId);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			AssetPriorityProvider&										mAssetPriorityProvider;	///< Asset priority provider instance to use, must stay valid as long as this asset storage manager instance exists
			bool														mIsRunning;
			FileSystem&													mFileSystem;
			AssetSystem&												mAssetSystem;
			EditorSettingsGroup&										mEditorSettingsGroup;
			OnlineSettingsGroup&										mOnlineSettingsGroup;
			AssetClient&												mAssetClient;
			EditorClient&												mEditorClient;
			std::set<std::tuple<ProjectId, std::string, GlobalAssetId>>	mDirtyAssets;
			boost::container::flat_set<GlobalAssetId>					mUnusedGlobalAssetIds;
			bool														mNewGlobalAssetIdsRequested;
			boost::container::flat_set<uint64>							mUnusedEntityIds;
			bool														mNewEntityIdsRequested;
			std::unordered_map<GlobalAssetId, AssetRequestCallback>		mPendingLockRequests;
			boost::container::flat_map<GlobalAssetId, uint32>			mAssetLocks;
			std::unordered_map<base::NetworkTransferId, AssetTransferInfo>	mAssetTransferInfo;
			std::unordered_map<base::NetworkTransferId, EditorTransferInfo>	mEditorTransferInfo;
			std::set<EditorCachedAssetDownload>							mBackgroundEditorPendingCachedAssetDownloads;
			GlobalAssetId												mBackgroundEditorCachedAssetTransferGlobalAssetId;
			std::set<std::tuple<ProjectId, std::string, GlobalAssetId>>	mEditorSyncPendingAssetUpdates;
			std::set<EditorCachedAssetDownload>							mEditorSyncPendingCachedAssetDownloads;
			size_t														mEditorSyncTransferCount;
			bool														mEditorSyncIsDownloading;
			bool														mEditorSyncIsProjectListProcessed;
			uint32														mEditorSyncUnprocessedAssetPackgeLists;
			bool														mEditorIsSyncing;
			bool														mEditorIsSyncAll;
			boost::container::flat_map<SourceAssetCacheKey, SourceAssetCacheEntry>	mEditorSourceAssetCache;
			boost::container::flat_map<uint32, GlobalAssetId>			mDerivedAssetMap;
			std::set<std::pair<ProjectId, std::string>>					mModifiedAssetPackages;
			JobProxy													mUpdateJobProxy;	///< Job proxy

			boost::signals2::scoped_connection slot_assetTransferStarted;
			boost::signals2::scoped_connection slot_assetTransferProgress;
			boost::signals2::scoped_connection slot_assetTransferFinished;
			boost::signals2::scoped_connection slot_assetTransferCanceled;

			boost::signals2::scoped_connection slot_assetsMounted;
			boost::signals2::scoped_connection slot_assetsUnmounted;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor/asset/AssetStorageManager-inl.h"
