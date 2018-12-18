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

#include <list>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
QT_BEGIN_NAMESPACE
	class QAction;
	class QSettings;
QT_END_NAMESPACE
namespace qsf
{
	namespace editor
	{
		class QActions;
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
		*    QActions manager class
		*
		*  @remarks
		*    The QActions manager is automatically able to use all CAMP classes which are derived from "qsf::editor::QActions".
		*
		*    There are several Qt actions which are not directly tight to a certain Qt widget. Examples for this are Qt actions
		*    like undo or redo, new map or brush and nodes Qt actions. Those Qt actions can be "plugged into" for example the
		*    menu bar or a tool bar, or be used by multiple tools.
		*/
		class QActionsManager : public PluginableManager
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] application
			*    QSF editor application instance, the instance must stay valid as long as this QActions manager instance exists
			*/
			explicit QActionsManager(Application& application);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~QActionsManager();

			/**
			*  @brief
			*    Return the QSF editor application
			*
			*  @return
			*    The QSF editor application, do no destroy the returned instance
			*/
			inline Application& getApplication() const;

			/**
			*  @brief
			*    Register all known and supported CAMP "qsf::editor::QActions" classes
			*/
			void registerQActions();

			/**
			*  @brief
			*    Unregister all known and supported CAMP "qsf::editor::QActions" classes
			*/
			void unregisterQActions();

			/**
			*  @brief
			*    Activate all QActions by adding them to the main window instance
			*
			*  @remarks
			*    This is necessary to enable shortcuts (= hotkeys) for QActions that are not added to any menu or QWidget
			*
			*  @note
			*    - TODO(fw): Maybe this is not enough, and we have to do more here?
			*    - TODO(fw): Does it make sense to add ALL QActions to MainWindow? There could be QActions that have to be added elsewhere.
			*/
			void activateAllQActions();

			/**
			*  @brief
			*    Deactivate all QActions by removing them from the main window instance
			*/
			void deactivateAllQActions();

			/**
			*  @brief
			*    Return the list of currently up-and-running Qt action instances
			*
			*  @return
			*    The list of currently up-and-running Qt action instances, do not destroy the Qt action instances or the list instance
			*/
			inline const std::vector<QActions*>& getRunningQActions() const;

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
			*    Retranslate the UI
			*
			*  @note
			*    - The default implementation is empty
			*/
			void retranslateUi();

			/**
			*  @brief
			*    Return a list of all Qt action instances
			*
			*  @param[out] qActions
			*    Receives the Qt action instances, list is not cleared before adding new entries, do not destroy the returned instances
			*
			*  @return
			*    The number of Qt action instances added to the given list
			*/
			void getAllQActions(std::list<QAction*>& qActions) const;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::PluginableManager methods      ]
		//[-------------------------------------------------------]
		protected:
			virtual void classAdded(const camp::Class& added) override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			Application*		   mApplication;		///< QSF editor application instance, always valid, do not destroy the instance
			std::vector<QActions*> mRunningQActions;	///< List of currently up-and-running Qt action instances, do not destroy the instances because in here we just have pointers and are not the owner


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
#include "qsf_editor/qaction/QActionsManager-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::QActionsManager)
