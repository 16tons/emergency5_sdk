// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/dialog/Dialog.h"
#include "qsf_editor/settings/EditorSettingsGroup.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ui
{
	class LocalAssetDatasourcesDialog;
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
		*    QSF editor local asset datasources dialog
		*/
		class LocalAssetDatasourcesDialog : public Dialog
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
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::LocalAssetDatasourcesDialog" unique pluginable dialog ID


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
			LocalAssetDatasourcesDialog(DialogManager* dialogManager, QWidget* qWidgetParent);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~LocalAssetDatasourcesDialog();


		//[-------------------------------------------------------]
		//[ Protected virtual QWidget methods                     ]
		//[-------------------------------------------------------]
		protected:
			virtual void dragEnterEvent(QDragEnterEvent* qDragEnterEvent) override;
			virtual void dropEvent(QDropEvent* qDropEvent) override;


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
			void onClickedOkButton(bool checked);
			void onClickedCancelButton(bool checked);
			void onClickedAddButton(bool checked);
			void onClickedRemoveButton(bool checked);
			void onCellDoubleClicked(int row, int column);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void fillDatasourcesList();
			std::string getIdentifier(QTableWidgetItem* qTableWidgetItem);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			Ui::LocalAssetDatasourcesDialog*			mUiLocalAssetDatasourcesDialog;	///< UI check for local asset datasources dialog instance, can be a null pointer, we have to destroy the instance in case we no longer need it
			EditorSettingsGroup::LocalAssetDatasources	mLocalDatasourcesBackup;


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::LocalAssetDatasourcesDialog)
