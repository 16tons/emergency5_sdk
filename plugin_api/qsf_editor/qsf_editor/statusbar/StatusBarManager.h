// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"

#include <qsf/plugin/pluginable/PluginableManager.h>

#include <QtCore/qnamespace.h>


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
		class StatusBar;
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
		*    Status bar manager class
		*
		*  @remarks
		*    The status bar manager is automatically able to use all CAMP classes which are derived from "qsf::editor::StatusBar".
		*/
		class StatusBarManager : public PluginableManager
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
			*    Main window instance, the instance must stay valid as long as this status bar manager instance exists
			*/
			inline explicit StatusBarManager(MainWindow& mainWindow);

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~StatusBarManager();

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
			*    Close all registered status bars
			*/
			void closeAllStatusBars();

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

			/**
			*  @brief
			*    Show a temporary status bar message
			*
			*  @param[in] message
			*    Temporary message to show
			*  @param[in] timeout
			*    Duration in milliseconds to show the message, can be 0 for no timeout
			*/
			void showMessage(const QString& message, int timeout = 8000);


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::PluginableManager methods      ]
		//[-------------------------------------------------------]
		protected:
			virtual void classAdded(const camp::Class& added) override;
			virtual void createPluginableInstance(const camp::Class& added, camp::UserObject& campUserObject) override;
			virtual void unregisterPluginables() override;


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
#include "qsf_editor/statusbar/StatusBarManager-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::StatusBarManager)
