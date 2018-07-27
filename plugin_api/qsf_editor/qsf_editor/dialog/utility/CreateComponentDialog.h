// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/dialog/Dialog.h"
#include "qsf_editor/selection/entity/EntitySelectionManager.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ui
{
	class CreateComponentDialog;
}
namespace qsf
{
	class Map;
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
		*    QSF editor create component instance dialog
		*/
		class CreateComponentDialog : public Dialog
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
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::CreateComponentDialog" unique pluginable dialog ID
			enum OperationType
			{
				CREATE,	// Create a new component
				MORPH	// Morph an existing component from one class to another
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
			CreateComponentDialog(DialogManager* dialogManager, QWidget* qWidgetParent);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~CreateComponentDialog();

			/**
			*  @brief
			*    Show the dialog in order to execute a defined component operation
			*
			*  @param[in] idSet
			*    ID entity set containing the IDs of the entities to operate on
			*  @param[in] sourceComponentId
			*    ID of the component to operate on, uninitialized when creating a new component
			*  @param[in] operationType
			*    Type of the operation which should be performed
			*
			*  @return
			*    Return code of the modal dialog, "QDialog::Rejected" on error
			*/
			int show(const EntitySelectionManager::IdSet& idSet, uint32 sourceComponentId, OperationType operationType);


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
			void onItemDoubleClicked(QTreeWidgetItem* item, int column);
			void onSearchFieldTextChanged(const QString& searchText);
			void onClickedExpand(bool checked);
			void onClickedCollapse(bool checked);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void fillTree(const camp::Class& baseCampClass, QTreeWidgetItem& qParentItem);
			bool recursiveSearchTextChanged(const QString& searchText, QTreeWidgetItem& qItem);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			Ui::CreateComponentDialog*    mUiCreateComponentDialog;	///< UI create component dialog instance, can be a null pointer, we have to destroy the instance in case we no longer need it
			Map&						  mMap;						///< Map instance to operate on, do not destroy the instance
			EntitySelectionManager::IdSet mIdSet;					///< ID entity set containing the IDs of the entities to operate on
			uint32						  mSourceComponentId;		///< ID of the component to operate on, uninitialized when creating a new component
			OperationType				  mOperationType;			///< Type of the operation which should be performed


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::CreateComponentDialog)
