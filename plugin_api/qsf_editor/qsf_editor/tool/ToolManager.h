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
		class Tool;
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
		*    Tool manager class
		*
		*  @remarks
		*    The tool manager is automatically able to use all CAMP classes which are derived from "qsf::editor::Tool".
		*/
		class ToolManager : public PluginableManager
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
			*    Main window instance, the instance must stay valid as long as this tool manager instance exists
			*/
			explicit ToolManager(MainWindow& mainWindow);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~ToolManager();

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
			*    Return the currently selected tool
			*
			*  @return
			*    The currently selected tool independent of whether or not the toolbox view is currently visible, can be a null pointer, do not destroy the instance
			*/
			inline Tool* getSelectedTool() const;

			/**
			*  @brief
			*    Return the currently shown tool
			*
			*  @return
			*    The currently shown tool, can be a null pointer if no tool is selected or the toolbox view isn't visible
			*/
			Tool* getShownTool() const;

			/**
			*  @brief
			*    Select the requested tool
			*
			*  @param[in] tool
			*    Tool to select, can be a null pointer
			*
			*  @note
			*    - In case of a valid tool, the toolbox view will be shown automatically
			*/
			void selectTool(Tool* tool);


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::PluginableManager methods      ]
		//[-------------------------------------------------------]
		protected:
			virtual void classAdded(const camp::Class& added) override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			MainWindow* mMainWindow;	///< QSF editor main window instance, always valid, do not destroy the instance
			Tool*		mSelectedTool;	///< The currently selected tool, can be a null pointer, do not destroy the instance


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
#include "qsf_editor/tool/ToolManager-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::ToolManager)
