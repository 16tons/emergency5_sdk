// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/dialog/Dialog.h"
#include "qsf_editor/asset/AssetStorageManagerTypes.h" // Needed for enum AssetTransferType because you cannot forward declare an enum (bad MS extension...)

#include <qsf_editor_base/network/packet/asset/AssetHistory.h>

#include <qsf/asset/AssetSystemTypes.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ui
{
	class AssetHistoryDialog;
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
		*    QSF editor asset history dialog
		*/
		class AssetHistoryDialog : public Dialog
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
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::AssetHistoryDialog" unique pluginable dialog ID


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] dialogManager
			*    The dialog manager this dialog instance is in, no null pointer allowed (the CAMP reflection system does not like references in constructors, so we have to use a pointer instead)
			*  @param[in] qWidgetParent
			*    Pointer to parent Qt widget, can be a null pointer (in this case you're responsible for destroying this dialog instance)
			*/
			AssetHistoryDialog(DialogManager* dialogManager, QWidget* qWidgetParent);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~AssetHistoryDialog();

			void setGlobalAssetId(GlobalAssetId globalAssetId);

			void setAssetHistory(GlobalAssetId globalAssetId, const std::vector<base::packet::AssetHistory::AssetHistoryEntry>& assetHistory);

			bool showAssetHistory(GlobalAssetId globalAssetId, const std::vector<base::packet::AssetHistory::AssetHistoryEntry>& assetHistory);


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::Dialog methods         ]
		//[-------------------------------------------------------]
		protected:
			virtual void retranslateUi() override;
			virtual void changeVisibility(bool visible) override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void onClickedRevertToRevision(bool checked);
			void onCurrentIndexChangedAssetRevision(int index);

			void onAssetHistory(GlobalAssetId globalAssetId, const std::vector<base::packet::AssetHistory::AssetHistoryEntry>& assetHistory);

			void onTransferProgress(AssetTransferType type, GlobalAssetId globalAssetId, uint64 current, uint64 total);
			void onTransferCanceled(AssetTransferType type, GlobalAssetId globalAssetId);
			void onTransferFinished(AssetTransferType type, GlobalAssetId globalAssetId);

			void onAssetLockResult(GlobalAssetId globalAssetId, bool isSuccess);

			void setCurrentRevision(AssetRevision revision);
			void updateGui();

			AssetRevision getCurrentAssetRevision() const;
			AssetRevision getDataRevision(AssetRevision revision) const;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			Ui::AssetHistoryDialog* mUiAssetHistoryDialog;		///< UI asset history dialog instance, can be a null pointer, we have to destroy the instance in case we no longer need it
			GlobalAssetId			mGlobalAssetId;				///< Global asset ID of the asset the new asset is a copy from
			Asset*					mNewAsset;					///< Dialog internal new asset instance which is not registered inside the asset system, always valid, destroy the instance in case you no longer need it
			std::map<AssetRevision, base::packet::AssetHistory::AssetHistoryEntry> mAssetHistory;
			boost::container::flat_set<AssetRevision>							   mAssetRevisionsWithData;
			bool					mReceivedHistory;


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::AssetHistoryDialog)
