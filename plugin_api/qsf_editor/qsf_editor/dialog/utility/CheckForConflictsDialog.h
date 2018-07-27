// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/dialog/Dialog.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
QT_BEGIN_NAMESPACE
	class QStandardItemModel;
	class QSortFilterProxyModel;
QT_END_NAMESPACE
namespace Ui
{
	class CheckForConflictsDialog;
}
namespace qsf
{
	class Entity;
	struct LogMessage;
	namespace editor
	{
		class MapTestManager;
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
		*    QSF editor check for conflicts dialog
		*/
		class CheckForConflictsDialog : public Dialog
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
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::CheckForConflictsDialog" unique pluginable dialog ID


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
			CheckForConflictsDialog(DialogManager* dialogManager, QWidget* qWidgetParent);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~CheckForConflictsDialog();


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::Dialog methods         ]
		//[-------------------------------------------------------]
		protected:
			virtual void readSettings(QSettings& qSettings) override;
			virtual void writeSettings(QSettings& qSettings) override;
			virtual void retranslateUi() override;
			virtual void changeVisibility(bool visible) override;


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onItemClickedMapTestList(QListWidgetItem* qListWidgetItem);
			void onItemSelectionChangedResultsList(const QItemSelection&, const QItemSelection&);
			void onItemDoubleClickedResultsList(const QModelIndex& qListWidgetItem);
			void onClickedRunCheck(bool checked);
			void onClickedSelectAll(bool checked);
			void onClickedDeselectAll(bool checked);
			void onShowWarningsToggled(bool checked);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Called when a new log message has been added
			*
			*  @param[in] message
			*    The new UTF-8 log message
			*  @param[in] entity
			*    Entity related to the message, can be a null pointer, do not destroy the instance
			*/
			void onNewLogMessage(const qsf::LogMessage& message, const qsf::Entity* entity);

			/**
			*  @brief
			*    Update the enabled state of the run check button
			*/
			void updateRunCheckButton();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			Ui::CheckForConflictsDialog	*mUiCheckForConflictsDialog;	///< UI check for conflicts dialog instance, can be a null pointer, we have to destroy the instance in case we no longer need it
			MapTestManager*				 mMapTestManager;				///< Map test manager instance, can be a null pointer, we have to destroy the instance in case we no longer need it
			QStandardItemModel*			 mModel;
			QSortFilterProxyModel*		 mSortFilterProxyModel;

			uint32						mDetectedErrorsCount;
			uint32						mDetectedWarningsCount;


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::CheckForConflictsDialog)
