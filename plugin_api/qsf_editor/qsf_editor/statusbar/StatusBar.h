// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/statusbar/StatusBarManager.h"

#include <qsf/plugin/pluginable/Pluginable.h>

#include <QtWidgets/qwidget.h>


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
		*    Abstract status bar class
		*
		*  @remarks
		*    Each status bar connected to the CAMP reflection system should have the following tags:
		*    - "ShowQAction":    "true" if this status bar should have an automatic Qt action which is used e.g. inside the menu bar, default is "true"
		*    - "OnlineOnly":     "true" if this status bar should only have an automatic Qt action which is used e.g. inside the menu bar if the online editor is used, default is "false"
		*    - "Shortcut":       Shortcut to show an instance of this status bar, e.g. "F1" (type: "std::string" with UTF-8 encoding, default value is "")
		*    - "DefaultVisible": Default visibility of the status bar (type: "bool", default value is "false")
		*/
		class QSF_EDITOR_API_EXPORT StatusBar : public Pluginable<StatusBarManager>, public QWidget
		{


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
			friend class StatusBarManager;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~StatusBar();

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
			*
			*  @note
			*    - Ease-of-use method
			*/
			MainWindow& getMainWindow() const;

			/**
			*  @brief
			*    Returns if the status bar active (aka is added/shown in the status bar area)
			*
			*  @return
			*    True, when active otherwise false
			*/
			bool isActive() const;

			/**
			*  @brief
			*    Changes the active state of the status bar
			*
			*  @param[in] active
			*    True, when the status bar should be active and thus shown in the status bar area. False when not
			*/
			void setActive(bool active);


		//[-------------------------------------------------------]
		//[ Public virtual QWidget methods                        ]
		//[-------------------------------------------------------]
		public:
			virtual void setVisible(bool visible) override;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::StatusBar methods      ]
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

			/**
			*  @brief
			*    Retranslate the UI
			*
			*  @note
			*    - The default implementation is empty
			*/
			inline virtual void retranslateUi();

			/**
			*  @brief
			*    Change visibility
			*
			*  @note
			*    - Unlike "QWidget::setVisible()" this method is only called if there was a real visibility state change
			*    - The default implementation is empty
			*/
			inline virtual void changeVisibility(bool visible);


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] statusBarManager
			*    The status bar manager this status bar instance is in, no null pointer allowed (the CAMP reflection system does not like references in constructors, so we have to use a pointer instead)
			*  @param[in] qWidgetParent
			*    Pointer to parent Qt widget, can be a null pointer (in this case you're responsible for destroying this status bar instance)
			*/
			StatusBar(StatusBarManager* statusBarManager, QWidget* qWidgetParent);


		//[-------------------------------------------------------]
		//[ Protected virtual QWidget methods                     ]
		//[-------------------------------------------------------]
		protected:
			virtual void changeEvent(QEvent* qEvent) override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			bool mVisible;	///< Current QWidget visibility
			bool mActive;	///< Indicates if this statusbar is active


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
#include "qsf_editor/statusbar/StatusBar-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::StatusBar)
