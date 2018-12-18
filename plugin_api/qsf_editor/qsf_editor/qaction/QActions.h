// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/qaction/QActionsManager.h"

#include <qsf/plugin/pluginable/Pluginable.h>

#include <QtCore/qobject.h>
#include <QtGui/qkeysequence.h>

#include <map>
#include <vector>


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
		*    Abstract class holding a collection of QAction instances of a particular category
		*
		*  @remarks
		*    Qt actions lifecycle:
		*    - "onStartup()"
		*    - Can optionally be called multiple times during the lifetime: "retranslateUi()"
		*    - "onShutdown()"
		*
		*  @note
		*   - Do not perform edit mode dependent tasks directly within Qt actions implementations, connect/disconnect from edit modes to these Qt actions instead
		*/
		class QSF_EDITOR_API_EXPORT QActions : public Pluginable<QActionsManager>, public QObject
		{


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
		friend class QActionsManager;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~QActions();

			/**
			*  @brief
			*    Return the QSF editor application
			*
			*  @return
			*    The QSF editor application, do not destroy the returned instance
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
			*    The main window, can be a null pointer, do not destroy the returned instance
			*
			*  @note
			*    - Ease-of-use method
			*/
			MainWindow* getMainWindow() const;

			/**
			*  @brief
			*    Return the list of all available QAction instances
			*
			*  @return
			*    The list of all available QAction instances, do not manipulate the list or destroy the QAction instances
			*/
			inline const std::vector<QAction*>& getQActionList() const;

			/**
			*  @brief
			*    Return the default shortcut for a given QAction instance
			*
			*  @param[in]  qAction
			*    Action to check for default shortcut
			*  @param[out] defaultKey
			*    Receives the default QKeySequence for the given QAction on success, not touched otherwise
			*
			*  @return
			*    "true" if the action is part of this class, "false" if not
			*/
			bool getDefaultQActionShortcut(QAction& qAction, QKeySequence& defaultKey) const;


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			/**
			*  @brief
			*    Register a Qt action
			*
			*  @param[in] objectName
			*    Object name to assign to the Qt action, do not perform internationalization on the object name
			*  @param[in] qAction
			*    Qt action to register
			*  @param[in] defaultShortcut
			*    Default shortcut
			*/
			void registerQtAction(const QString& objectName, QAction& qAction, const QKeySequence& defaultShortcut);

			/**
			*  @brief
			*    Ease-of-use Qt action create and register
			*
			*  @param[out] qAction
			*    Qt action instance to create and register
			*  @param[in]  objectName
			*    Object name to assign to the Qt action, do not perform internationalization on the object name
			*  @param[in]  qObjectParent
			*    Qt action parent
			*  @param[in]  shortcut
			*    Shortcut
			*/
			void createRegisterQtAction(QAction*& qAction, const QString& objectName, QObject* qObjectParent, const QKeySequence& shortcut);

			/**
			*  @brief
			*    Ease-of-use Qt action create, connect and register
			*
			*  @param[out] qAction
			*    Qt action instance to create and register
			*  @param[in]  objectName
			*    Object name to assign to the Qt action, do not perform internationalization on the object name
			*  @param[in]  qObjectParent
			*    Qt action parent
			*  @param[in]  call
			*    Qt slot to call on Qt action trigger signal
			*  @param[in]  shortcut
			*    Shortcut
			*/
			void createConnectRegisterQtAction(QAction*& qAction, const QString& objectName, QObject* qObjectParent, const char* call, const QKeySequence& shortcut);


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::QActions methods       ]
		//[-------------------------------------------------------]
		protected:
			/**
			*  @brief
			*    Read settings by using Qt's "QSettings"
			*
			*  @param[in] qSettings
			*    Qt settings instance to use
			*
			*  @note
			*    - Usually the default implementation of this method is sufficient, do only reimplement it if required
			*/
			virtual void readSettings(QSettings& qSettings);

			/**
			*  @brief
			*    Write settings by using Qt's "QSettings"
			*
			*  @param[in] qSettings
			*    Qt settings instance to use
			*
			*  @note
			*    - Usually the default implementation of this method is sufficient, do only reimplement it if required
			*/
			virtual void writeSettings(QSettings& qSettings);

			//[-------------------------------------------------------]
			//[ Lifecycle                                             ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Startup the Qt actions
			*
			*  @return
			*    "true" if all went fine, else "false"
			*
			*  @note
			*    - The default implementation is empty
			*/
			inline virtual bool onStartup();

			/**
			*  @brief
			*    Retranslate the UI
			*
			*  @note
			*    - The default implementation is empty
			*    - Only called by "QActionsManager::retranslateUi()", so it's ensured that only the
			*      currently active and hence properly initialized Qt actions receive the changed event
			*/
			inline virtual void retranslateUi();

			/**
			*  @brief
			*    Shutdown the Qt actions
			*
			*  @note
			*    - Only called in case "onStartup()" returned successfully
			*
			*  @note
			*    - The default implementation is empty
			*/
			inline virtual void onShutdown();


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] qActionsManager
			*    The QActions manager this QActions instance is in, no null pointer allowed (the CAMP reflection system does not like references in constructors, so we have to use a pointer instead)
			*/
			explicit QActions(QActionsManager* qActionsManager);


		//[-------------------------------------------------------]
		//[ Protected data                                        ]
		//[-------------------------------------------------------]
		protected:
			std::vector<QAction*>			 mQActionList;		///< List of all available QAction instances, do not destroy the QAction instances
			std::map<QAction*, QKeySequence> mDefaultShortcut;	///< Default shortcut map with Qt action as key and Qt key sequence as value


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
#include "qsf_editor/qaction/QActions-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::QActions)
