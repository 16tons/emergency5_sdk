// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"
#include "qsf_editor/asset/AssetStorageManagerTypes.h"

#include <qsf/base/GetUninitialized.h>

#include <QtCore/qobject.h>

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <boost/container/flat_set.hpp>

#include <memory>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class AssetPackage;
	class AssetSystem;
	class CachedAsset;
	namespace editor
	{
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
		//[ Global definitions                                    ]
		//[-------------------------------------------------------]
		struct AssetStatus
		{
			std::string targetAssetPackage;
			bool isLocked;
			bool isLockFailed;
			bool doUpload;
			bool useCachedAsset;
			bool useCachedAssetInAssetDbLite;

			AssetStatus() :
				isLocked(false),
				isLockFailed(false),
				doUpload(false),
				useCachedAsset(false),
				useCachedAssetInAssetDbLite(true)
			{
				// Nothing to do in here
			}
		};


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Asset edit helper class
		*/
		class QSF_EDITOR_API_EXPORT AssetEditHelper : public boost::noncopyable, public std::enable_shared_from_this<AssetEditHelper>
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 RECOMMENDED_MAXIMUM_NUMBER_OF_EDITED_ASSETS;	///< It's no good idea to edit too many assets in a single burst, we might hit the "_setmaxstdio" ( https://msdn.microsoft.com/en-us/library/6e3b887c.aspx ) limit at one point inside the pipeline


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Creates a new asset edit helper asset package
			*
			*  @param[in] projectId
			*    ID of the project the edited asset is in
			*  @param[in] ignoreLog
			*    Set "true" to explicitly ignore a project lock, do only do this in special situations were you know exactly what you're doing
			*   (the material browser is a good example, were the user still wants to be able to review locked material assets)
			*
			*  @note
			*    - Defaults to the default destination project provided by the asset import manager
			*/
			AssetEditHelper(ProjectId projectId = getUninitialized<ProjectId>(), bool ignoreLog = false);

			/**
			*  @brief
			*    Destructor
			*/
			~AssetEditHelper();

			/**
			*  @brief
			*    Returns whether the asset edit helper is in a healthy state
			*
			*  @return
			*    "true" if the asset edit helper is in a healthy state, "false" otherwise
			*/
			bool isGood() const;

			/**
			*  @brief
			*    Returns whether the asset edit helper is ready (= has no more background tasks running, no locks pending)
			*
			*  @return
			*    "true" if the asset edit helper is ready, "false" otherwise
			*/
			bool isReady() const;

			/**
			*  @brief
			*    Returns whether the asset edit helper was instructed to submit and is either still preparing, already submitting or done
			*
			*  @return
			*    "true" if the asset edit helper was instructed to submit and is either still preparing, already submitting or done, "false" otherwise
			*/
			bool isSubmit() const;

			/**
			*  @brief
			*    Returns whether the asset edit helper is done submitting (= all uploads have been started, all asset locks have been released)
			*
			*  @return
			*    "true" if the asset edit helper is done submitting, "false" otherwise
			*/
			bool isDone() const;

			/**
			*  @brief
			*    Returns whether assets submitted through the asset edit helper are still being uploaded or not
			*
			*  @return
			*    "true" if assets submitted through the asset edit helper are still being uploaded, "false" otherwise
			*/
			bool isUploading() const;

			/**
			*  @brief
			*    Returns the identifier of the project this asset edit helper instance is editing
			*
			*  @return
			*    The identifier of the project this asset edit helper instance is editing
			*/
			ProjectId getProjectId() const;

			/**
			*  @brief
			*    Resets the asset edit helper from its sort-of-final state after it successfully finished submitting.
			*    This method will fail if the asset edit helper is not yet done submitting. It is safe to call this method from within the doneCallback.
			*
			*  @return
			*    "true" if everything went fine, "false" otherwise
			*/
			bool reset();

			/**
			*  @brief
			*    Tells the asset edit helper to call the given callback function once it is ready (calling any non-const members of this class before the callback is called might lead to undefined behaviour)
			*
			*  @return
			*    "false" if an error occurred, "true" otherwise
			*
			*  @note
			*    - If the asset edit helper is already ready when this method is called, the callback is called instantly before returning from this method.
			*/
			bool callWhenReady(boost::function<void()> readyCallback);

			/**
			*  @brief
			*    Tells the asset edit helper to call the given callback function once it finishes uploading (calling submit() before the callback is called will do nothing)
			*
			*  @return
			*    "false" if an error occurred, "true" otherwise
			*
			*  @note
			*    - If the asset edit helper has already finished uploading when this method is called, the callback is called instantly before returning from this method.
			*/
			bool callWhenFinishedUploading(boost::function<void()> finishedCallback);

			/**
			*  @brief
			*    Adds an existing asset to the set of edited assets, immediately tries to acquire all required locks
			*    and copies meta asset information of the asset into our edit cache.
			*
			*  @param[in] globalAssetId
			*    Global asset ID of the asset to open for editing
			*  @param[in] targetAssetPackageName (optional)
			*    Name of the asset package to put the asset copy into when submitting. The asset package will be created with guessed default values if it does not exist yet. If no target asset package is provided the asset is submitted into the asset package it was initially copied from.
			*  @param[in] dontCopyCachedAsset (optional)
			*    Provide "true" to not duplicate the cached asset and local file, when the asset is added to the intermediate asset package by this call. (default: false)
			*  @param[in] sourceAssetPackage (optional)
			*    Optional source asset package, if not provided the source asset package will be searched by using the given global asset ID
			*
			*  @return
			*    A pointer to the internal Asset object copy to use for property editing. Do not destroy the returned instance. Can be a null pointer in case of an error.
			*
			*  @note
			*    - If the given global asset ID is already part of the asset edit helper, the internal Asset object copy and lock is reused.
			*    - Call isReady() to check if all locks are acquired yet
			*/
			Asset* editAsset(GlobalAssetId globalAssetId, const std::string& targetAssetPackageName = "", const std::string& assetTypeName = "", const std::string& assetCategory = "", const std::string& assetName = "", bool dontCopyCachedAsset = false, const AssetPackage* sourceAssetPackage = nullptr);

			/**
			*  @brief
			*    Similar to "editAsset()", but doesn't change the good-state in case the asset does not exist
			*/
			Asset* tryEditAsset(GlobalAssetId globalAssetId, const std::string& targetAssetPackageName = "", const std::string& assetTypeName = "", const std::string& assetCategory = "", const std::string& assetName = "", bool dontCopyCachedAsset = false, const AssetPackage* sourceAssetPackage = nullptr);

			/**
			*  @brief
			*    Adds a copy of an existing asset to the set of edited assets, immediately tries to acquire all required locks
			*    and copies meta asset information of the asset into our edit cache.
			*
			*  @param[in] sourceGlobalAssetId
			*    Global asset ID of the asset to duplicate
			*  @param[in] targetAssetPackageName
			*    Name of the asset package to put the asset copy into when submitting. The asset package will be created with guessed default values if it does not exist yet. If no target asset package is provided the asset is submitted into the asset package it was initially copied from.
			*  @param[in] newName
			*    Optional new asset name
			*  @param[in] targetGlobalAssetId
			*    Optional global asset ID of the asset to create, if uninitialized a new one is automatically generated
			*  @param[in] dontCopyCachedAsset
			*    Optional: Provide "true" to not duplicate the cached asset and local file, when the asset is added to the intermediate asset package by this call. (default: false)
			*
			*  @return
			*    A pointer to the internal Asset object copy to use for property editing. Do not destroy the returned instance. Can be a nullptr in case of an error.
			*
			*  @note
			*    - If the given global asset ID is already part of the asset edit helper, the internal Asset object copy and lock is reused.
			*    - Call isReady() to check if all locks are acquired yet
			*/
			Asset* duplicateAsset(GlobalAssetId sourceGlobalAssetId, const std::string& targetAssetPackageName, const std::string* newName = nullptr, GlobalAssetId targetGlobalAssetId = getUninitialized<GlobalAssetId>(), bool dontCopyCachedAsset = false);

			/**
			*  @brief
			*    Adds an asset to the set of edited assets, immediately tries to acquire all required locks. In case an asset with the same local asset name already exists.
			*
			*  @param[in] assetPackageName
			*    Name of the asset package to put the asset copy into when submitting. The asset package will be created with guessed default values if it does not exist yet.
			*  @param[in] assetTypeName
			*    Asset type name of the asset we're adding
			*  @param[in] assetCategory
			*    Category of the asset we're adding
			*  @param[in] assetName
			*    Name of the asset we're adding
			*  @param[in] allowImport
			*    "true" means local asset name conflicts are resolved by importing the existing asset, "false" means that a local asset name conflict is an error and nullptr, shall be returned. (optional; default is "true")
			*  @param[in] sourceAssetPackage (optional)
			*    Optional source asset package, if not provided the source asset package will be searched by using the given global asset ID
			*
			*  @return
			*    A pointer to the internal asset object copy to use for property editing. Do not destroy the returned instance. Can be a nullptr in case of an error.
			*
			*  @note
			*    - Calls AssetStorageManager::acquireGlobalAssetId() to get a new free global asset ID if needed
			*    - Call isReady() to check if all locks are acquired yet
			*/
			Asset* addAsset(const std::string& assetPackageName, const std::string& assetTypeName, const std::string& assetCategory, const std::string& assetName, bool allowImport = true, const AssetPackage* sourceAssetPackage = nullptr);

			/**
			*  @brief
			*    Adds an asset to the set of edited assets, immediately tries to acquire all required locks. In case an asset with the same local asset name already exists. Variant with explicit global asset ID.
			*
			*  @param[in] globalAssetId
			*    Explicit global asset ID
			*  @param[in] assetPackageName
			*    Name of the asset package to put the asset copy into when submitting. The asset package will be created with guessed default values if it does not exist yet.
			*  @param[in] assetTypeName
			*    Asset type name of the asset we're adding
			*  @param[in] assetCategory
			*    Category of the asset we're adding
			*  @param[in] assetName
			*    Name of the asset we're adding
			*  @param[in] sourceAssetPackage (optional)
			*    Optional source asset package, if not provided the source asset package will be searched by using the given global asset ID
			*
			*  @return
			*    A pointer to the internal asset object copy to use for property editing. Do not destroy the returned instance. Can be a nullptr in case of an error.
			*
			*  @note
			*    - See "qsf::editor::addAsset()"
			*/
			Asset* addAssetByGlobalAssetId(GlobalAssetId globalAssetId, const std::string& assetPackageName, const std::string& assetTypeName, const std::string& assetCategory, const std::string& assetName, const AssetPackage* sourceAssetPackage = nullptr);

			/**
			*  @brief
			*    Tells the asset edit helper whether or not to upload asset data for the asset with the given global asset id. (= replace the local cached asset on submit & upload the source asset)
			*    The given asset must have been introduced to the asset edit helper before though a call to editAsset(...) or addAsset(...).
			*
			*  @param[in] globalAssetId
			*    Global asset ID of the asset
			*  @param[in] doUpload
			*    Whether or not to upload asset data
			*  @param[in] useCachedAsset
			*    Whether or not to use the cached asset as source asset aswell.
			*  @param[in] useCachedAssetInAssetDbLite
			*    Whether or not to use the cached asset as source asset in asset-db-lite mode.
			*
			*  @return
			*    Returns "false" in case of an error, "true" otherwise.
			*/
			bool setAssetUploadData(GlobalAssetId globalAssetId, bool doUpload, bool useCachedAsset, bool useCachedAssetInAssetDbLite = true);

			/**
			*  @brief
			*    Returns the asset object for the asset with the given global asset ID.
			*    The given asset must have been introduced to the asset edit helper before though a call to editAsset(...) or addAsset(...).
			*
			*  @param[in] globalAssetId
			*    Global asset ID of the asset
			*
			*  @return
			*    the asset object for the asset with the given global asset ID, nullptr in case of an error.
			*/
			Asset* getAsset(GlobalAssetId globalAssetId);

			/**
			*  @brief
			*    Returns the cached asset object for the asset with the given global asset ID.
			*    The given asset must have been introduced to the asset edit helper before though a call to editAsset(...) or addAsset(...).
			*
			*  @param[in] globalAssetId
			*    Global asset ID of the asset
			*
			*  @return
			*    the cached asset object for the asset with the given global asset ID, nullptr in case of an error.
			*
			*  @note
			*    - Automatically creates a cached asset for the asset if it didn't exist yet
			*/
			CachedAsset* getCachedAsset(GlobalAssetId globalAssetId);

			/**
			*  @brief
			*    Sets the asset's datasource path and copies the source asset into the temporary upload folder (if not in asset-db-lite mode).
			*    The given asset must have been introduced to the asset edit helper before though a call to editAsset(...) or addAsset(...).
			*
			*  @param[in] globalAssetId
			*    Global asset ID of the asset
			*  @param[in] datasourcePath
			*    Datasource path to set. Can be either an absolute path pointing into a SVN directory structure or a datasource path relative to a known datasource.
			*
			*  @return
			*    Returns "false" in case of an error, "true" otherwise.
			*/
			bool setAssetDatasource(GlobalAssetId globalAssetId, const std::string& datasourcePath);

			/**
			*  @brief
			*    Return the temporary absolute source asset directory for the asset with the given global asset id.
			*    The given asset must have been introduced to the asset edit helper before though a call to editAsset(...) or addAsset(...).
			*
			*  @param[in] globalAssetId
			*    Global asset ID of the asset
			*  @param[in] forceEmpty
			*    If "true" the temp source directory is forced to be empty
			*
			*  @return
			*    The temporary absolute source asset directory for the asset with the given global asset ID, "" in case of an error
			*/
			std::string getAssetTemporarySourceDirectory(GlobalAssetId globalAssetId, bool forceEmpty = false);

			/**
			*  @brief
			*    Return the relative path to where the cached asset should be stored. In asset-db-lite mode the cached asset is copied from this path for upload when submit(...) is called.
			*    The given asset must have been introduced to the asset edit helper before though a call to editAsset(...) or addAsset(...).
			*
			*  @param[in] globalAssetId
			*    Global asset ID of the asset
			*  @param[in] fileExtension
			*    File extension to append to the relative path. When empty the asset edit helper looks if a CachedAsset is present in the intermediate asset package for the given asset and tries to deduce the file extension from it. (optional; default is "")
			*
			*  @return
			*    The relative path to where the cached asset should be stored, "" in case of an error
			*
			*  @note
			*    - Automatically creates a cached asset for the asset if it didn't exist yet
			*/
			std::string getCachedAssetPath(GlobalAssetId globalAssetId, const std::string& fileExtension = "");

			/**
			*  @brief
			*    Return the intermediate asset package used by this asset edit helper
			*
			*  @return
			*    The intermediate asset package used by this asset edit helper. Can be a nullptr in case of an error.
			*/
			AssetPackage* getIntermediateAssetPackage() const;

			/**
			*  @brief
			*    Tell the asset edit helper to rescan its intermediate asset package for added or removed assets and update its locks accordingly.
			*
			*  @note
			*    - Call this method e.g. after compiling a material into the intermediate asset package so its derived assets are detected and handled correctly
			*/
			void rescanIntermediateAssetPackage();

			/**
			*  @brief
			*    If all locks have been acquired, immediately submits all made changes, if not sets a flag to instantly submit all changes once all locks have been acquired
			*
			*  @return
			*    "false" if an error occurred, "true" otherwise. A return value of "true" does not indicate, that the changes have been submitted yet!
			*
			*  @note
			*    - Calls rescanIntermediateAssetPackage() before proceeding
			*    - Uses AssetStorageManager::submitAssetChanges(...) internally
			*    - Call isDone() to check if all changes were submitted
			*/
			bool submit(boost::function<void()> doneCallback = 0);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void onLockAssetResult(GlobalAssetId globalAssetId, bool isSuccess);

			void onAssetTransferCanceled(AssetTransferType type, GlobalAssetId globalAssetId);
			void onAssetTransferFinished(AssetTransferType type, GlobalAssetId globalAssetId);

			void onFinishedUploading();

			void onAssetAdded(GlobalAssetId globalAssetId, const std::string& targetAssetPackage = "");
			void onAssetRemoved(GlobalAssetId globalAssetId);

			void submitImpl();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			AssetStorageManager& mAssetStorageManager;
			AssetSystem&		 mAssetSystem;

			AssetPackage*							  mIntermediateAssetPackage;
			std::map<GlobalAssetId, AssetStatus>	  mAssetStatusMap;
			boost::container::flat_set<GlobalAssetId> mPendingUploads;
			std::vector<boost::function<void()>>	  mReadyCallbacks;
			boost::function<void()>					  mDoneCallback;
			std::vector<boost::function<void()>>	  mFinishedCallbacks;
			bool									  mIsGood;
			bool									  mIsSubmit;
			bool									  mIsSubmitDone;
			bool									  mIsFinished;

			QMetaObject::Connection assetTransferCanceled_slot;
			QMetaObject::Connection assetTransferFinished_slot;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
