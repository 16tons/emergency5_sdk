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
	class QStandardItem;
	class QStandardItemModel;
	class QSortFilterProxyModel;
QT_END_NAMESPACE
namespace Ui
{
	class BatchJobBuildLocalizedMaterialsConfigDialog;
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
		*    Batch job recompile materials configuration dialog
		*/
		class BatchJobBuildLocalizedMaterialsConfigDialog : public Dialog
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
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::BatchJobBuildLocalizedMaterialsConfigDialog" unique pluginable dialog ID


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
			BatchJobBuildLocalizedMaterialsConfigDialog(DialogManager* dialogManager, QWidget* qWidgetParent = nullptr);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~BatchJobBuildLocalizedMaterialsConfigDialog();

			void clearPoolGlobalAssetIds();
			void setPoolGlobalAssetIds(const std::vector<GlobalAssetId>& poolGlobalAssetIds);
			const std::vector<GlobalAssetId>& getPoolGlobalAssetIds() const;

			void clearSelectionGlobalAssetIds();
			void setSelectionGlobalAssetIds(const std::vector<GlobalAssetId>& selectionGlobalAssetIds);
			const std::vector<GlobalAssetId>& getSelectionGlobalAssetIds() const;

			void setLocalizations(const std::string& localizations);
			const std::string& getLocalizations() const;


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
			void onLineEditTextEdited(const QString& newText);
			void onClickedAddAll(bool checked);
			void onClickedRemoveAll(bool checked);
			void onClickedAddSelected(bool checked);
			void onClickedRemoveSelected(bool checked);
			void onClickedStart(bool checked);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void moveItems(const std::vector<QStandardItem*>& items, QListView* sourceQListView, QListView* destinationQListView);
			void updateItemCounts();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			Ui::BatchJobBuildLocalizedMaterialsConfigDialog*	mUiDialog;	///< UI dialog instance, can be a null pointer, we have to destroy the instance in case we no longer need it
			QStandardItemModel*					mPoolModel;
			QSortFilterProxyModel*				mPoolProxyModel;
			QStandardItemModel*					mSelectionModel;
			QSortFilterProxyModel*				mSelectionProxyModel;
			std::vector<GlobalAssetId>			mPoolGlobalAssetIds;
			std::vector<GlobalAssetId>			mSelectionGlobalAssetIds;
			std::string							mLocalizations;


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::BatchJobBuildLocalizedMaterialsConfigDialog)
