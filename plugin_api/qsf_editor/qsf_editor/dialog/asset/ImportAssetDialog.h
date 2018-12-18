// Copyright (C) 2012-2018 Promotion Software GmbH


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
	class QListWidgetItem;
QT_END_NAMESPACE
namespace qsf
{
	namespace editor
	{
		namespace detail
		{
			class ImportAsset;
		}
	}
}
namespace Ui
{
	class ImportAssetDialog;
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
		*    QSF editor import asset dialog
		*
		*  @remarks
		*    The asset import dialog starts his job after the intermediate asset package has been created. By using the asset import dialog, the
		*    metadata of the imported assets inside the intermediate asset package can be modified. Task of the asset import dialog is also
		*    to detect and resolve asset import conflicts and to merge the intermediate asset package into a final asset package.
		*/
		class ImportAssetDialog : public Dialog
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
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::ImportAssetDialog" unique pluginable dialog ID

			enum FlagType
			{
				FLAG_INFO,		///< No highlight
				FLAG_NOTICE,	///< Bright blue highlight
				FLAG_WARN,		///< Yellow highlight
				FLAG_ERROR		///< Red highlight
			};


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
			ImportAssetDialog(DialogManager* dialogManager, QWidget* qWidgetParent);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~ImportAssetDialog();


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
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onCustomContextMenuRequested(const QPoint& qPoint);
			void onClickedOk(bool checked);
			void onClickedCancel(bool checked);
			void onCurrentItemChanged(QListWidgetItem* current, QListWidgetItem* previous);
			void onCurrentChanged(int index) const;
			void onCloseDialogTimer();


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Clear the content
			*/
			void clear();

			void onAssetLockResult(bool isSuccess);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			Ui::ImportAssetDialog*						  mUiImportAssetDialog;	///< UI import asset dialog instance, can be a null pointer, we have to destroy the instance in case we no longer need it
			std::map<GlobalAssetId, detail::ImportAsset*> mImportAssets;


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::ImportAssetDialog)
