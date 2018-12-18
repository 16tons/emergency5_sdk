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

#include <qsf/asset/AssetSystemTypes.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ui
{
	class EditAssetDialog;
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
		*    QSF editor edit asset dialog
		*/
		class EditAssetDialog : public Dialog
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
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::EditAssetDialog" unique pluginable dialog ID


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
			EditAssetDialog(DialogManager* dialogManager, QWidget* qWidgetParent);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~EditAssetDialog();

			/**
			*  @brief
			*    Set the asset to edit by using a global asset ID
			*
			*  @param[in] globalAssetId
			*    Global ID of the asset to edit, "qsf::getUninitialized<GlobalAssetId>()" if invalid
			*/
			void setGlobalAssetId(GlobalAssetId globalAssetId);


		//[-------------------------------------------------------]
		//[ Public virtual QDialog methods                        ]
		//[-------------------------------------------------------]
		public:
			virtual void done(int result) override;


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
			void onAssetLockResult(GlobalAssetId globalAssetId, bool isSuccess);


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onClickedOk(bool checked);
			void onClickedCancel(bool checked);

			void onTransferProgress(AssetTransferType type, GlobalAssetId globalAssetId, uint64 current, uint64 total);
			void onTransferCanceled(AssetTransferType type, GlobalAssetId globalAssetId);
			void onTransferFinished(AssetTransferType type, GlobalAssetId globalAssetId);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			Ui::EditAssetDialog* mUiEditAssetDialog;		///< UI edit asset dialog instance, can be a null pointer, we have to destroy the instance in case we no longer need it
			std::string			 mOriginalAssetName;		///< Original asset name of the edited asset
			std::string			 mOriginalAssetTypeName;	///< Original asset type name of the edited asset
			Asset*				 mEditedAssetCopy;			///< Copy of the asset we're editing, can be a null pointer, we have to destroy the instance in case we no longer need it
			bool				 mSourceAssetAvailable;		///< "true" if a source asset is available for editing, "false" otherwise


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::EditAssetDialog)
