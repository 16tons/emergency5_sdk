// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/selection/entity/EntitySelectionManager.h"

#include <qsf/base/manager/Manager.h>
#include <qsf/asset/AssetSystemTypes.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
QT_BEGIN_NAMESPACE
	class QSettings;
QT_END_NAMESPACE
namespace qsf
{
	struct LogMessage;
	class Entity;
	class Project;
	class Prototype;
	class FileSystem;
	class AssetSystem;
	class AssetPackage;
	class PrefabContent;
	namespace editor
	{
		class AssetImportListener;
		class AssetStorageManager;
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
		*    QSF editor asset import manager
		*
		*  @remarks
		*    All import processes are going through the asset import manager. An import process always starts by
		*    creating an intermediate asset package where temporary asset import results will be managed in.
		*
		*   Drag'n'drop asset import preview is handled by the asset import edit mode ("qsf::editor::AssetImportEditMode").
		*   Editing of the intermediate asset package is done by the asset import dialog ("qsf::editor::ImportAssetDialog").
		*/
		class QSF_EDITOR_API_EXPORT AssetImportManager : public QObject, public Manager
		{


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
					// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			typedef boost::function<void(bool isSuccess)> AssetLockCallback;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			AssetImportManager();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~AssetImportManager();

			/**
			*  @brief
			*    Read settings by using Qt's "QSettings"
			*
			*  @param[in] qSettings
			*    Qt settings instance to use
			*/
			void readSettings(QSettings& qSettings);

			/**
			*  @brief
			*    Write settings by using Qt's "QSettings"
			*
			*  @param[in] qSettings
			*    Qt settings instance to use
			*/
			void writeSettings(QSettings& qSettings);

			//[-------------------------------------------------------]
			//[ Default destination project asset package             ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Return the default destination project ID
			*
			*  @return
			*    The default destination project ID
			*/
			ProjectId getDefaultDestinationProjectId();

			/**
			*  @brief
			*    Return the project name of the default destination asset package
			*
			*  @return
			*    The project name of the default destination asset package
			*/
			std::string getDefaultDestinationAssetPackageProjectName() const;

			/**
			*  @brief
			*    Return the name of the default destination asset package
			*
			*  @return
			*    The name of the default destination asset package
			*/
			std::string getDefaultDestinationAssetPackageName() const;

			/**
			*  @brief
			*    Return the project the default destination asset package is in
			*
			*  @return
			*    The project the default destination asset package is in
			*/
			Project* getDefaultDestinationAssetPackageProject() const;

			/**
			*  @brief
			*    Return the default destination asset package
			*
			*  @return
			*    The default destination asset package
			*/
			AssetPackage* getDefaultDestinationAssetPackage();

			/**
			*  @brief
			*    Set default destination asset package
			*
			*  @param[in] projectName
			*    Name of the project the new default destination asset package is in
			*  @param[in] assetPackageName
			*    Name of the new default destination asset package
			*/
			void setDefaultDestinationAssetPackage(const std::string& projectName, const std::string& assetPackageName);

			//[-------------------------------------------------------]
			//[ Asset import startup                                  ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Startup prefab group asset import by using a given set of entity IDs
			*
			*  @param[in] entityIdSet
			*    Set of entities to import as prefab group
			*  @param[in] existingPrefabAsset
			*    Existing prefab asset, can be a null pointer
			*
			*  @return
			*    "true" if all went fine, else "false"
			*
			*  @remarks
			*    Prefab group import steps:
			*    1. Prepare the prototype which will act as prefab group
			*    2. Create an intermediate asset package for the prefab group asset
			*    3. Continue with the usual intermediate asset package import
			*
			*  @note
			*   - This method creates an intermediate asset package and continues with showing the asset import dialog
			*/
			bool startupPrefabGroupImportByEntityIdSet(const editor::EntitySelectionManager::IdSet& entityIdSet, const Asset* existingPrefabAsset = nullptr);

			/**
			*  @brief
			*    Startup prefab asset import by using a given entity
			*
			*  @param[in] entity
			*    Entity to import as prefab
			*  @param[in] existingPrefabAsset
			*    Existing prefab asset, can be a null pointer
			*  @param[in] removeInheritance
			*    If "true", all base prefab / prototype references should be removed in the imported prefab
			*  @param[in] switchToPrefabInstancingEditMode
			*    Switch to prefab instancing edit mode after the import so the user can directly place prefab instances?
			*
			*  @return
			*    "true" if all went fine, else "false"
			*
			*  @remarks
			*    Prefab import steps:
			*    1. Prepare the prototype which will act as prefab
			*    2. Create an intermediate asset package for the prefab asset
			*    3. Continue with the usual intermediate asset package import
			*
			*  @note
			*   - This method creates an intermediate asset package and continues with showing the asset import dialog
			*/
			bool startupPrefabImportByEntity(const Entity& entity, const Asset* existingPrefabAsset = nullptr, bool removeInheritance = true, bool switchToPrefabInstancingEditMode = true);

			/**
			*  @brief
			*    Return whether or not an import preview by using a local filename is possible
			*
			*  @param[in] absoluteFilename
			*    Local absolute filename of the asset to check for import preview capabilities
			*
			*  @return
			*    "true" if an import preview is possible, else "false" (e.g. unknown asset data or there's no meaningful asset data preview possible)
			*/
			bool isImportPreviewByAbsoluteFilenamePossible(const QString& absoluteFilename) const;

			/**
			*  @brief
			*    Initiate asset import by using a local filename and an asset import preview
			*
			*  @param[in] absoluteFilename
			*    Local absolute filename of the asset to import
			*  @param[in] assetImportListener
			*    Optional asset import listener, can be a null pointer
			*
			*  @note
			*   - On successful intermediate asset import, the method automatically switches into the asset import edit mode
			*   - On acceptance, the asset import edit mode will continue with the asset import dialog
			*/
			void startupImportPreviewByAbsoluteFilename(const QString& absoluteFilename, AssetImportListener* assetImportListener = nullptr);

			/**
			*  @brief
			*    Initiate asset import by using a local filename
			*
			*  @param[in] absoluteFilename
			*    Local absolute filename of the asset to import
			*  @param[in] assetImportListener
			*    Optional asset import listener, can be a null pointer
			*
			*  @return
			*    "true" if all went fine, else "false"
			*/
			bool startupImportByAbsoluteFilename(const QString& absoluteFilename, AssetImportListener* assetImportListener = nullptr);

			//[-------------------------------------------------------]
			//[ Asset import in-process                               ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Return whether or not there's currently an asset import in progress
			*
			*  @return
			*    "true" if currently an asset import in progress, else "false"
			*/
			bool isImportInProgress() const;

			/**
			*  @brief
			*    Return whether or not a log error during the asset import was registered
			*
			*  @return
			*    "true" in case a log error during the asset import was registered, else "false"
			*
			*  @remarks
			*    During asset import we also check for log messages which might be critical. Sadly, e.g. OGRE does only write
			*    log messages for certain situations, so an import might still be possible even if OGRE noticed an issue while
			*    e.g. loading a mesh. During asset import we need to be pretty restrictive what gets through into the asset database
			*    in order to avoid catastrophic situations during runtime.
			*/
			bool hasLogErrorDuringImport() const;

			/**
			*  @brief
			*    Reset log error during import; don't use this method unless you know exactly what you're doing
			*/
			void resetLogErrorDuringImport();

			/**
			*  @brief
			*    Return the original filename of the asset to import
			*
			*  @return
			*    The original absolute UTF-8 filename in platform-independent notation of the asset to import
			*/
			const std::string& getOriginalAssetAbsoluteFilename() const;

			/**
			*  @brief
			*    Return the intermediate asset package instance the asset import manager is feeding
			*
			*  @return
			*    The intermediate asset package instance, can be a null pointer, do not destroy the instance
			*/
			AssetPackage* getIntermediateAssetPackage() const;

			/**
			*  @brief
			*    Return the global ID of the primary asset to import
			*
			*  @return
			*    Global ID of the primary asset to import, "qsf::getUninitialized<GlobalAssetId>()" if invalid
			*/
			GlobalAssetId getPrimaryGlobalAssetId() const;

			void setAssetLockCallback(const AssetLockCallback& callback);
			void clearAssetLockCallback();

			/**
			*  @brief
			*    Returns if this asset can be excluded from the import
			*/
			bool canExcludeAssetFromImport(GlobalAssetId globalAssetId);

			/**
			*  @brief
			*    Exclude this asset from the import
			*
			*  @note
			*    - Only assets that did exist prior to the import can be excluded
			*    - The primary import asset cannot be excluded
			*/
			bool excludeAssetFromImport(GlobalAssetId globalAssetId);

			/**
			*  @brief
			*    Copy the provided asset source into the directory where the asset storage manager temporally locally stores assets to be edited by the user
			*
			*  @note
			*    - This method does nothing when not connected to an asset database server
			*    - This method does nothing when the editor is in asset-db-lite mode
			*/
			void copyToAssetStorageManagerSourceAssetDirectory(const std::string& absoluteFilename, GlobalAssetId globalAssetId) const;

			/**
			*  @brief
			*    Copy the provided compiled asset into the directory where the asset storage manager temporally locally stores assets to be edited by the user
			*
			*  @note
			*    - This method does nothing when not connected to an asset database server
			*    - This method does nothing when operating in asset-db-full mode
			*/
			void copyCompiledToAssetStorageManagerSourceAssetDirectory(const std::string& absoluteFilename, GlobalAssetId globalAssetId) const;

			/**
			*  @brief
			*    Add the given global asset ID to the imported QSF materials list, so its source material will be either moved
			*    out from the import directory inside our material datasource or deleted, depending on whether the import is accepted or not
			*/
			void addImportedQsfMaterial(GlobalAssetId globalAssetId);

			/**
			*  @brief
			*    Schedule this QSF material asset for recompilation if the import is canceled (when importing a texture all QSF materials that use this texture are automatically recompiled)
			*/
			void addAffectedQsfMaterial(GlobalAssetId globalAssetId);

			//[-------------------------------------------------------]
			//[ Asset import shutdown                                 ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Finalize the asset import
			*
			*  @param[in] accepted
			*    "true" if the asset import has been accepted, else "false" in case it was rejected
			*
			*  @note
			*    - Usually only called by "qsf::editor::ImportAssetDialog::hideEvent()"
			*/
			void shutdownImport(bool accepted);

			//[-------------------------------------------------------]
			//[ Asset utility methods                                 ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Create an asset package and add it to the current project by importing the data from an existing file system structure
			*
			*  @param[in] name
			*    UTF-8 name of the asset package to import to the current project (e.g. "my_content" directory name which is inside the virtual path of the project the asset package is in)
			*
			*  @return
			*    Pointer on the added asset package on success, else null pointer, do not destroy the returned instance
			*
			*  @note
			*    - In case there's already a "asset_package.json"-file it will be replaced by a new generated one
			*/
			AssetPackage* importLocalAssetPackageByName(const std::string& name);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Called on asset import startup
			*/
			void onStartupAssetDataImport();

			/**
			*  @brief
			*    Called when a new log message has been added
			*
			*  @param[in] message
			*    The new UTF-8 log message
			*
			*  @see
			*    - "qsf::editor::AssetImportManager::hasLogErrorDuringImport()"
			*/
			void onNewLogMessage(const LogMessage& message);

			/**
			*  @brief
			*    Called on asset import shutdown
			*/
			void onShutdownAssetDataImport();

			/**
			*  @brief
			*    Show asset import dialog
			*
			*  @return
			*    "true" if the asset import has been accepted, else "false"
			*/
			bool showAssetImportDialog();

			//[-------------------------------------------------------]
			//[ Asset package                                         ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Set a decent default destination asset package
			*/
			void findDefaultDestinationAssetPackage();

			/**
			*  @brief
			*    Return the current destination asset package
			*
			*  @return
			*    The current destination asset package
			*
			*  @note
			*    - If a destination asset package was explicitly set, this one will be returned, else the default one
			*/
			AssetPackage* getDestinationAssetPackage();

			/**
			*  @brief
			*    Create intermediate asset package
			*
			*  @param[in] assetPackageName
			*    UTF-8 name of the intermediate asset package
			*
			*  @return
			*    The created intermediate asset package instance, can be a null pointer, do not destroy the instance
			*/
			AssetPackage* createIntermediateAssetPackage(const std::string& assetPackageName);

			/**
			*  @brief
			*    Merge intermediate asset package into final asset package
			*
			*  @param[in] destinationAssetPackage
			*    Destination asset package, if null pointer, this method will do nothing
			*/
			void mergeIntermediateAssetPackage(AssetPackage* destinationAssetPackage);

			/**
			*  @brief
			*    Callback for asset lock requests
			*/
			void onAssetLockResponse(GlobalAssetId globalAssetId, bool isSuccess);

			/**
			*  @brief
			*    Queue the (edited) assets for upload
			*
			*  @param[in] destinationAssetPackage
			*    Destination asset package, if null pointer, this method will do nothing
			*
			*  @note
			*    - Only allowed to be called in online editor mode
			*/
			void queueAssetStorageManagerTempAssetDirectoryUploads(AssetPackage* destinationAssetPackage);

			/**
			*  @brief
			*    Clear the temp-directories of the (edited) assets
			*/
			void clearAssetStorageManagerTempAssetDirectories();

			/**
			*  @brief
			*    Destroy the intermediate asset import package including its files
			*
			*  @param[in] accepted
			*    "true" if the asset import has been accepted, else "false" in case it was rejected
			*/
			void destroyIntermediateAssetPackage(bool accepted);

			//[-------------------------------------------------------]
			//[ Prefab and prototype                                  ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Rescue an already existing prefab
			*
			*  @param[in] filename
			*    Filename used to identify the prefab
			*  @param[out] prefabContent
			*    Prefab content which will be found with the found already existing prefab
			*/
			void rescueExistingPrefabByFilename(const QString& filename, PrefabContent& prefabContent);

			/**
			*  @brief
			*    Import the given prefab
			*
			*  @param[in] prefabContent
			*    Intermediate prefab to import
			*  @param[in] existingPrefabAsset
			*    Existing prefab asset, can be a null pointer
			*
			*  @return
			*    "true" if all went fine, else "false"
			*
			*  @note
			*    - This method will destroy the intermediate prefab (meaning all prototypes involved)
			*/
			bool importPrefab(PrefabContent& prefabContent, const Asset* existingPrefabAsset = nullptr);

			/**
			*  @brief
			*    Import the given prefab asset
			*
			*  @param[in] prefabContent
			*    Intermediate prefab to import
			*  @param[in] prefabAsset
			*    Already existing prefab asset to directly reuse
			*  @param[in] preview
			*    Prefab asset preview?
			*
			*  @return
			*    "true" if all went fine, else "false"
			*
			*  @note
			*    - This method will destroy the intermediate prefab (meaning all prototypes involved)
			*/
			bool importPrefabAsset(PrefabContent& prefabContent, Asset& prefabAsset, bool preview = false);

			/**
			*  @brief
			*    Match the given prefab; TODO(fw): More desc
			*/
			void matchPrefab(const Prototype& importMainPrototype, Prototype& originalMainPrototype);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			const FileSystem&		mFileSystem;
			const AssetSystem&		mAssetSystem;
			AssetStorageManager&	mAssetStorageManager;
			std::string				mDefaultDestinationProjectName;			///< Default destination project name as defined by the user
			std::string				mDefaultDestinationAssetPackageName;	///< Default destination asset package name as defined by the user
			std::string				mDestinationProjectName;				///< Current project name as defined by the user, if not explicitly set, the default one is used
			std::string				mDestinationAssetPackageName;			///< Current destination asset package name as defined by the user, if not explicitly set, the default one is used
			std::string				mOriginalAssetAbsoluteFilename;			///< Original absolute UTF-8 filename in platform-independent notation of the asset to import
			std::string				mIntermediateAssetPackageName;			///< UTF-8 name of the intermediate asset package
			bool					mSwitchToPrefabInstancingEditMode;		///< Switch to prefab instancing edit mode after the import so the user can directly place prefab instances?
			GlobalAssetId			mPrimaryGlobalAssetId;					///< Global ID of the primary asset to import, "qsf::getUninitialized<GlobalAssetId>()" if invalid
			PrefabContent*			mPreviewPrefabContent;					///< Prefab content which is used for the asset import preview, can be a null pointer, we're responsible for destroying the instance in case we no longer need it
			// TODO(ca) What about the case when the material gets excluded from the import?
			boost::container::flat_set<GlobalAssetId> mImportedQsfMaterials;	///< Set of qsf materials that are getting imported
			// TODO(ca) What about the case when the texture affecting the material gets excluded from the import?
			boost::container::flat_set<GlobalAssetId> mAffectedQsfMaterials;	///< Set of qsf materials that got affected during the import directly or indirectly through a changed texture
			boost::container::flat_set<GlobalAssetId> mPendingAssetLocks;		///< Once a lock for a global asset ID in this set is acquired it is removed from this set.
			bool									  mAssetLockFailed;			///< Set to "true" in case an asset lock failed, cleared on import shutdown
			AssetLockCallback						  mAssetLockCallback;		///< Callback function that is called once all asset lock requests have been answered, cleared on import shutdown or once answered
			bool									  mLogErrorDuringImport;	///< Log error noticed during import?


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
