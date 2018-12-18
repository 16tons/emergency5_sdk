// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"

#include <qsf/base/error/BoostAssertHandler.h>
#include <qsf/plugin/pluginable/PluginableManager.h>

#include <QtCore/qnamespace.h>

#include <QtWidgets/QDialog>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
QT_BEGIN_NAMESPACE
	class QSettings;
QT_END_NAMESPACE
namespace qsf
{
	namespace editor
	{
		class Dialog;
		class MainWindow;
		class Application;
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
		*    Dialog manager class
		*
		*  @remarks
		*    The dialog manager is automatically able to use all CAMP classes which are derived from "qsf::editor::Dialog".
		*/
		class DialogManager : public PluginableManager
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] mainWindow
			*    Main window instance, the instance must stay valid as long as this dialog manager instance exists
			*/
			explicit DialogManager(MainWindow& mainWindow);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~DialogManager();

			/**
			*  @brief
			*    Return the QSF editor application
			*
			*  @return
			*    The QSF editor application, do no destroy the returned instance
			*
			*  @note
			*    - Ease-of-use method
			*/
			Application& getApplication() const;

			/**
			*  @brief
			*    Return the main window
			*
			*  @return
			*    The main window, do no destroy the returned instance
			*/
			inline MainWindow& getMainWindow() const;

			/**
			*  @brief
			*    Show a modal dialog
			*
			*  @param[in] dialogClassId
			*    Class ID (generated via class name string hash) of the CAMP reflection class encapsulating a Qt dialog (must be derived from "qsf::editor::Dialog")
			*
			*  @return
			*    Return code of the modal dialog, "QDialog::Rejected" on error
			*
			*  @note
			*    - Ease-of-use method
			*/
			int showModalDialogById(const StringHash& dialogClassId) const;

			/**
			*  @brief
			*    Show a modal dialog
			*
			*  @param[in] parent
			*    The parent qt widget/window to use as parent for the dialog
			*
			*  @return
			*    Return code of the modal dialog, "QDialog::Rejected" on error
			*
			*  @note
			*    - Ease-of-use method
			*/
			template <typename T> int showModalDialog(QWidget* parent = nullptr) const;

			/**
			*  @brief
			*    Show a dialog
			*
			*  @note
			*    - Ease-of-use method
			*/
			template <typename T> void showDialog() const;

			/**
			*  @brief
			*    Hide a dialog
			*
			*  @note
			*    - Ease-of-use method
			*/
			template <typename T> void hideDialog() const;

			/**
			*  @brief
			*    Toggle dialog visibility
			*
			*  @note
			*    - Ease-of-use method
			*    - A visible dialog will be closed
			*    - An invisible dialog will be shown
			*/
			template <typename T> void toggleDialogVisibility() const;

			/**
			*  @brief
			*    Close all registered dialogs
			*/
			void closeAllDialogs();

			/**
			*  @brief
			*    Checks if any dialog is still open
			*/
			bool isAnyDialogOpen();

			/**
			*  @brief
			*    Read settings by using Qt's "QSettings"
			*
			*  @param[in] qSettings
			*    Qt settings instance to use
			*/
			void readSettings(QSettings& qSettings);

			/**
			*  @brief
			*    Write settings by using Qt's "QSettings"
			*
			*  @param[in] qSettings
			*    Qt settings instance to use
			*/
			void writeSettings(QSettings& qSettings);


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::PluginableManager methods      ]
		//[-------------------------------------------------------]
		protected:
			virtual void classAdded(const camp::Class& added) override;
			virtual void createPluginableInstance(const camp::Class& added, camp::UserObject& campUserObject) override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			MainWindow* mMainWindow;	///< QSF editor main window instance, always valid, do not destroy the instance


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
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor/dialog/DialogManager-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::DialogManager)
