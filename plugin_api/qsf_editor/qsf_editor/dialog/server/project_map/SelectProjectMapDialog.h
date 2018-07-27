// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/dialog/server/ClientStartupDialog.h"
#include "qsf_editor/asset/AssetStorageManagerTypes.h"

#include <qsf_editor_base/network/NetworkTypes.h>

#include <qsf/asset/AssetSystemTypes.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ui
{
	class SelectProjectMapDialog;
}
namespace qsf
{
	class BaseCachedAsset;
	namespace editor
	{
		class SelectProjectMapModel;
		namespace base
		{
			struct EditorMapProjectList;
		}
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
		*    QSF editor client select project map dialog
		*
		*  @note
		*    - As result, the Qt dialog returns the new stage "qsf::editor::Application::Stage" the application should switch to
		*/
		class SelectProjectMapDialog : public ClientStartupDialog
		{


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
					// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] openSelectedProjectMapAutomatically
			*    Try to automatically open selected project map on startup?
			*  @param[in] qWidgetParent
			*    Pointer to parent Qt widget, can be a null pointer (in this case you're responsible for destroying this dialog instance)
			*/
			SelectProjectMapDialog(bool openSelectedProjectMapAutomatically, QWidget* qWidgetParent = nullptr);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~SelectProjectMapDialog();

			GlobalAssetId getSelectedMapGlobalAssetId() const;


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
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void submitNewMap();
			void submitNewMapCleanup();

			void onAssetTransferProgress(AssetTransferType type, GlobalAssetId globalAssetId, uint64 current, uint64 total);
			void onAssetTransferFinished(AssetTransferType type, GlobalAssetId globalAssetId);
			void onAssetTransferCanceled(AssetTransferType type, GlobalAssetId globalAssetId);


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onCurrentItemChanged(const QModelIndex& current, const QModelIndex& previous);
			void onDoubleClicked(const QModelIndex& index);
			void onClickedBack(bool checked);
			void onClickedOptions(bool checked);
			void onClickedNew(bool checked);
			void onClickedOpen(bool checked);
			void onProjectMapListResponse(const base::EditorMapProjectList& mapProjectList);
			void onEditorTransferProgress(base::NetworkTransferId transferId, uint64 current, uint64 total);
			void onEditorTransferFinished(base::NetworkTransferId transferId, const std::string& fileExtension, const BaseCachedAsset& baseCachedAsset);
			void onEditorTransferCanceled(base::NetworkTransferId transferId);
			void onUpdateProjectMapList();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			QTimer*						 mTimer;
			bool						 mOpenSelectedProjectMapAutomatically;	///< Try to automatically open selected project map on startup?
			Ui::SelectProjectMapDialog*	 mUiSelectProjectMapDialog;				///< UI project map dialog instance, can be a null pointer, we have to destroy the instance in case we no longer need it
			base::NetworkTransferId		 mTransferId;
			GlobalAssetId				 mGlobalMapAssetId;
			Asset*						 mGlobalNewMapAsset;
			GlobalAssetId				 mCopyFromGlobalAssetId;
			AssetRevision				 mCopyFromAssetRevision;
			SelectProjectMapModel*		 mSelectProjectMapModel;
			QSortFilterProxyModel*		 mSortFilterProxyModel;
			bool						 mFirstMapUpdate;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
