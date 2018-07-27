// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/dialog/server/ClientStartupDialog.h"
#include "qsf_editor/asset/AssetStorageManagerTypes.h" // Needed for enum AssetTransferType because you cannot forward declare an enum (bad MS extension...)

#include <qsf_editor_base/network/NetworkTypes.h>

#include <qsf/asset/AssetSystemTypes.h>
#include <qsf/asset/project/BaseProject.h>

#include <queue>
#include <unordered_map>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ui
{
	class ImportProjectDialog;
}
namespace qsf
{
	class BaseCachedAsset;
	class Project;
	namespace editor
	{
		class AssetStorageManager;
		class EditorClient;
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
		*    Server synchronization dialog
		*
		*  @note
		*    - Designed only to work for the initial server filling, use "qsf::editor::ProjectImport" for runtime project import
		*/
		class ImportProjectDialog : public ClientStartupDialog
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
			static const std::string TEMP_PACKAGE_DIRECTORY;		///< UTF-8 name of the local directory in platform-independent notation where the compressed asset package images are stored for transfer


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] qWidgetParent
			*    Pointer to parent Qt widget, can be a null pointer (in this case you're responsible for destroying this dialog instance)
			*/
			explicit ImportProjectDialog(QWidget* qWidgetParent = nullptr);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~ImportProjectDialog();


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::ClientStartupDialog methods ]
		//[-------------------------------------------------------]
		protected:
			virtual void retranslateUi() override;


		//[-------------------------------------------------------]
		//[ Protected virtual QWidget methods                     ]
		//[-------------------------------------------------------]
		protected:
			virtual void closeEvent(QCloseEvent* qCloseEvent) override;


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onAssetTransferProgress(AssetTransferType type, GlobalAssetId globalAssetId, uint64 current, uint64 total);
			void onAssetTransferCanceled(AssetTransferType type, GlobalAssetId globalAssetId);
			void onAssetTransferFinished(AssetTransferType type, GlobalAssetId globalAssetId);

			void onEditorTransferProgress(base::NetworkTransferId transferId, uint64 current, uint64 total);
			void onEditorTransferCanceled(base::NetworkTransferId transferId);
			void onEditorTransferFinished(base::NetworkTransferId transferId, const std::string& fileExtension, const BaseCachedAsset& baseCachedAsset);

			void onPrepareImport();
			void onCheckNextAsset();

			void onStartNextUpload();


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void startImport();
			void checkNextAsset();
			void startNextUpload();
			Asset* getDependencyToUpload(GlobalAssetId globalAssetId);
			bool hasDependencyIssues(GlobalAssetId globalAssetId, boost::container::flat_set<GlobalAssetId>& currentDependencyLeaf) const;
			void onAssetLockResponse(GlobalAssetId globalAssetId, bool isSuccess);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			AssetStorageManager&										mAssetStorageManager;
			EditorClient&												mEditorClient;
			Ui::ImportProjectDialog*									mUiImportProjectDialog;	///< UI import project dialog instance, can be a null pointer, we have to destroy the instance in case we no longer need it
			boost::container::flat_set<GlobalAssetId>					mPendingPrefabUploads;		///< As usual prefabs need some hacky special treatment or bad things happen
			std::unordered_map<GlobalAssetId, std::vector<std::pair<ProjectId, std::string>>> mPendingUploads;
			std::queue<GlobalAssetId>									mPendingChecks;
			std::queue<std::pair<ProjectId, std::string>>				mPendingUnmanagedAssetPackages;
			std::vector<BaseProject>									mOriginalProjectTemplates;
			bool														mEncounteredErrorsDuringCheck;
			uint64														mTotalUploads;
			base::NetworkTransferId										mCurrentUnmanagedAssetPackageUploadTransferId;
			std::string													mCurrentUnmanagedAssetPackageUploadAbsoluteTemporaryArchiveFilename;
			GlobalAssetId												mCurrentUploadGlobalAssetId;
			Asset*														mCurrentUploadPreparedAssetTemplate;
			uint64														mOverallProgressDivider;
			uint64														mCurrentProgressDivider;
			bool														mCurrentProgressInitialized;
			int															mOriginalUpdateInterval;
			bool														mIsSuccessfullyFinished;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
