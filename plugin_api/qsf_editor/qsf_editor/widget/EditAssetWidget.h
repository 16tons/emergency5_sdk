// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/dialog/Dialog.h"

#include <qsf/asset/AssetSystemTypes.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
QT_BEGIN_NAMESPACE
	class QFileSystemModel;
QT_END_NAMESPACE
namespace Ui
{
	class EditAssetWidget;
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
		*    QSF editor edit asset widget
		*/
		class EditAssetWidget : public QWidget
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
			*  @param[in] qWidgetParent
			*    Pointer to parent Qt widget, can be a null pointer (in this case you're responsible for destroying this widget instance)
			*/
			explicit EditAssetWidget(QWidget* qWidgetParent = nullptr);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~EditAssetWidget();

			/**
			*  @brief
			*    Enable or disable read-only mode
			*
			*  @param[in] readOnly
			*    Provide "true", to switch the EditAssetWidget into read-only mode and "false", to switch it back to the usual read/write mode
			*/
			void setReadOnly(bool readOnly);

			/**
			*  @brief
			*    Set the asset to edit
			*
			*  @param[in] asset
			*    Asset to edit, can be a null pointer, must stay valid as long as it's edited by the edit asset widget
			*  @param[in] revision
			*    Revision of the asset to edit
			*  @param[in] allowFileChange
			*    File change allowed?
			*/
			void setEditedAsset(Asset* asset, AssetRevision revision, bool allowFileChange);

			/**
			*  @brief
			*    Return whether or not the current asset edits are considered to be valid
			*
			*  @param[in] assetTypeName
			*    If no empty string, must be this asset type name to be valid
			*  @param[in] performDuplicateCheck
			*    Whether to check for duplicate local asset names or not
			*
			*  @return
			*    "true" if the current asset edits are considered to be valid, else "false"
			*/
			bool isEditingValid(const std::string& assetTypeName = "", bool performDuplicateCheck = true) const;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::Dialog methods         ]
		//[-------------------------------------------------------]
		public:
			void retranslateUi();
			void changeVisibility(bool visible);


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onDoubleClickedData(const QModelIndex& index);
			// qsf::editor::PropertiesModel
			void onDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Update the title of the data group box
			*/
			void updateGroupBoxDataTitle() const;

			/**
			*  @brief
			*    Show critical message box
			*
			*  @param[in] text
			*    Text to show
			*/
			void showCriticalMessageBox(const QString& text) const;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			Ui::EditAssetWidget* mUiEditAssetWidget;	///< UI edit asset widget instance, can be a null pointer, we have to destroy the instance in case we no longer need it
			QFileSystemModel*	 mQFileSystemModel;		///< Qt file system model instance, can be a null pointer, Qt will destroy the instance automatically (this dialog is the parent)
			Asset*				 mAsset;				///< Asset to edit, can be a null pointer, must stay valid as long as it's edited by the edit asset widget, do not destroy the instance
			AssetRevision		 mRevision;				///< Revision of the asset to edit, "qsf::getUninitialized<AssetRevision>()" if invalid, "0" if this is an import
			bool				 mAllowFileChange;		///< File change allowed?
			bool				 mIsReadOnly;			///< Read-only mode?
			// Backup of essential asset data
			std::string			 mAssetTypeName;		///< UTF-8 asset type name, completely lower case
			std::string			 mAssetCategory;		///< UTF-8 asset category name, completely lower case
			std::string			 mAssetName;			///< UTF-8 asset name, completely lower case


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
