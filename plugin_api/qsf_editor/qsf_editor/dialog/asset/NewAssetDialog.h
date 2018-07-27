// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/dialog/Dialog.h"

#include <qsf/base/GetUninitialized.h>
#include <qsf/asset/AssetSystemTypes.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ui
{
	class NewAssetDialog;
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
		*    QSF editor new asset dialog
		*
		*  @remarks
		*    Think of the new asset dialog as a kind of save file as dialog - which does not write the file (asset in our case) itself,
		*    but only collects all necessary information for the dialog caller.
		*
		*  @todo
		*    - TODO(co) Make it possible to select project and asset package where the new asset should be in
		*/
		class NewAssetDialog : public Dialog
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
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::NewAssetDialog" unique pluginable dialog ID


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
			*  @param[in] performDuplicateCheck
			*    Whether to check for duplicate local asset names or not
			*/
			NewAssetDialog(DialogManager* dialogManager, QWidget* qWidgetParent, bool performDuplicateCheck = true);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~NewAssetDialog();

			/**
			*  @brief
			*    Startup the new asset creation
			*
			*  @param[in] assetTypeName
			*    Asset type name of the new asset
			*  @param[in] allowCopyFrom
			*    Allow copy from feature?
			*  @param[in] copyFromGlobalAssetId
			*    Global asset ID of the asset the new asset is a copy from, can be "qsf::getUninitialized<GlobalAssetId>()" if it's no initially known copy and the user has to decide
			*
			*  @return
			*    Return code of the modal dialog, "QDialog::Rejected" on error.
			*    On success, use "qsf::editor::NewAssetDialog::getNewAsset()" in order to get query the new asset medatadata.
			*/
			int startupNewAssetCreation(const std::string& assetTypeName, bool allowCopyFrom = true, GlobalAssetId copyFromGlobalAssetId = getUninitialized<GlobalAssetId>());

			/**
			*  @brief
			*    Return the dialog internal new asset instance which is not registered inside the asset system
			*
			*  @return
			*    The dialog internal new asset instance which is not registered inside the asset system, do not destroy the instance
			*/
			const Asset& getNewAsset() const;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::Dialog methods         ]
		//[-------------------------------------------------------]
		protected:
			virtual void retranslateUi() override;
			virtual void changeVisibility(bool visible) override;


		//[-------------------------------------------------------]
		//[ Protected Qt slots (MOC)                              ]
		//[-------------------------------------------------------]
		protected Q_SLOTS:
			void onClickedOk(bool checked);
			void onClickedCancel(bool checked);
			void onCurrentIndexChangedCopyFrom(int index);


		//[-------------------------------------------------------]
		//[ Protected data                                        ]
		//[-------------------------------------------------------]
		protected:
			Ui::NewAssetDialog* mUiNewAssetDialog;		///< UI new asset dialog instance, can be a null pointer, we have to destroy the instance in case we no longer need it
			std::string			mAssetTypeName;			///< Asset type name of the new asset
			bool				mAllowCopyFrom;			///< Allow copy from feature?
			GlobalAssetId		mCopyFromGlobalAssetId;	///< Global asset ID of the asset the new asset is a copy from
			Asset*				mNewAsset;				///< Dialog internal new asset instance which is not registered inside the asset system, always valid, destroy the instance in case you no longer need it
			bool				mPerformDuplicateCheck;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Set new asset data
			*
			*  @param[in] assetTypeName
			*    Asset type name of the new asset, no reference by intent in order to avoid losing the information in case this is a new asset data reset
			*  @param[in] copyFromGlobalAssetId
			*    Global asset ID of the asset the new asset is a copy from, can be "qsf::getUninitialized<GlobalAssetId>()" if it's no initially known copy and the user has to decide
			*
			*  @return
			*    Return code of the modal dialog, "QDialog::Rejected" on error
			*/
			int setNewAssetData(std::string assetTypeName, GlobalAssetId copyFromGlobalAssetId);


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::NewAssetDialog)
