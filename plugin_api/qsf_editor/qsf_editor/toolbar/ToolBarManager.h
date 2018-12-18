// Copyright (C) 2012-2018 Promotion Software GmbH


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
		class ToolBar;
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
		*    Tool bar manager class
		*
		*  @remarks
		*    The tool bar manager is automatically able to use all CAMP classes which are derived from "qsf::editor::ToolBar".
		*/
		class ToolBarManager : public PluginableManager
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
			*    Main window instance, the instance must stay valid as long as this tool bar manager instance exists
			*/
			inline explicit ToolBarManager(MainWindow& mainWindow);

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~ToolBarManager();

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
			*    Close all registered tool bars
			*/
			void closeAllToolBars();

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
			*    Return whether or not the layout is locked
			*
			*  @return
			*    "true" if the layout is locked, else "false"
			*/
			inline bool getLockLayout() const;

			/**
			*  @brief
			*    Set whether or not the layout is locked
			*
			*  @param[in] lockLayout
			*    "true" if the layout is locked, else "false"
			*/
			void setLockLayout(bool lockLayout);


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
			bool		mLockLayout;	///< "true" if the layout is locked, else "false"


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
#include "qsf_editor/toolbar/ToolBarManager-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::ToolBarManager)
