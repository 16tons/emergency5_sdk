// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/view/ViewManager.h"

#include <qsf/plugin/pluginable/Pluginable.h>

#include <QtWidgets/qdockwidget.h>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace camp
{
	class Args;
	class Value;
}
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
		*    Abstract view base class
		*
		*  @remarks
		*    Each view connected to the CAMP reflection system should have the following tags:
		*    - "ShowQAction":    "true" if this view should have an automatic Qt action which is used e.g. inside the menu bar, default is "true"
		*    - "OnlineOnly":     "true" if this view should only have an automatic Qt action which is used e.g. inside the menu bar if the online editor is used, default is "false"
		*    - "Shortcut":        Shortcut to show an instance of this view, e.g. "F1" (type: "std::string" with UTF-8 encoding, default value is "")
		*    - "DefaultVisible":  Default visibility of the view (type: "bool", default value is "false")
		*    - "DefaultFloating": Default floating state of the view (type: "bool", default value is "false")
		*    - "DefaultPosition": Default position of the view (type: "QPoint", default value is "QPoint(0, 0)")
		*    - "DefaultSize":     Default size of the view (type: "QSize", default value is "QSize(200, 200)")
		*    - "DefaultDockArea": Default dock area of the view (type: "Qt::DockWidgetArea", default value is "Qt::DockWidgetArea::NoDockWidgetArea")
		*
		*  @note
		*    - Derived views must implement a decent default behavior to make it possible to use them out-of-the-box
		*/
		class QSF_EDITOR_API_EXPORT View : public Pluginable<ViewManager>, public QDockWidget
		{


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
			friend class ViewManager;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~View();

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


		//[-------------------------------------------------------]
		//[ Public virtual QWidget methods                        ]
		//[-------------------------------------------------------]
		public:
			virtual void setVisible(bool visible) override;


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] viewManager
			*    The view manager this view instance is in, no null pointer allowed (the CAMP reflection system does not like references in constructors, so we have to use a pointer instead)
			*  @param[in] qWidgetParent
			*    Pointer to parent Qt widget, can be a null pointer (in this case you're responsible for destroying this view instance)
			*/
			View(ViewManager* viewManager, QWidget* qWidgetParent);

			//[-------------------------------------------------------]
			//[ Helper                                                ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Add the Qt dock widget to the given Qt main window and tabifies it
			*
			*  @param[in] qMainWindow
			*    Qt main window where to add the given Qt dock widget
			*  @param[in] qtDockWidgetArea
			*    Dock widget area where to add the given Qt dock widget within the given Qt main window
			*/
			void addViewAndTabify(QMainWindow& qMainWindow, Qt::DockWidgetArea qtDockWidgetArea);

			inline bool wasHiddenDueClose() const;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::View methods           ]
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
			*    Return the automatic window title by using the "Name"-tag of the CAMP reflection view
			*
			*  @param[in] view
			*    View instance to set the automatic window title for, usually "*this" in derived classes
			*
			*  @return
			*    The automatic window title by using the "Name"-tag of the CAMP reflection view as Qt string
			*
			*  @note
			*    - Internationalization is done automatically
			*/
			virtual QString getAutomaticWindowTitle(View& view) const;

			/**
			*  @brief
			*    Retranslate the UI
			*
			*  @note
			*    - This method is only called in case the view is visible
			*    - The default implementation is empty
			*/
			inline virtual void retranslateUi();

			/**
			*  @brief
			*    Filter method called before "qsf::editor::View::changeVisibility()" is called
			*
			*  @param[in] visible
			*    The requested visibility state
			*
			*  @return
			*    The final new visibility state, the view method is allowed to e.g. reject the new requested visibility state
			*
			*  @note
			*    - Unlike "QWidget::setVisible()" this method is only called if there was a real visibility state change
			*    - The default implementation is empty
			*    - By using this method, the view implementation has the opportunity to reject becoming visible, e.g. because it's unable to acquire a required entity lock
			*/
			inline virtual bool preChangeVisibility(bool visible);

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
		//[ Protected virtual QWidget methods                     ]
		//[-------------------------------------------------------]
		protected:
			virtual void showEvent(QShowEvent* qShowEvent) override;
			virtual void hideEvent(QHideEvent* qHideEvent) override;
			virtual void changeEvent(QEvent* qEvent) override;
			virtual void closeEvent(QCloseEvent* qEvent) override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			bool mVisible;	///< Current QWidget visibility
			bool mHiddenDueClose;


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
#include "qsf_editor/view/View-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::View)
QSF_CAMP_TYPE_NONCOPYABLE(Qt::DockWidgetArea)
