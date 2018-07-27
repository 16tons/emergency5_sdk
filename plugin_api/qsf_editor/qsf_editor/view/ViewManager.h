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
		class View;
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
		*    View manager class
		*
		*  @remarks
		*    The view bar manager is automatically able to use all CAMP classes which are derived from "qsf::editor::View".
		*/
		class ViewManager : public PluginableManager
		{


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
			friend class View;


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
			inline explicit ViewManager(MainWindow& mainWindow);

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~ViewManager();

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
			*    Show a view
			*
			*  @param[in] viewClassId
			*    Class ID (generated via class name string hash) of the CAMP reflection class encapsulating a Qt dock widget (must be derived from "qsf::editor::View")
			*
			*  @return
			*    The shown view, can be a null pointer (you don't need to care about destroying the view instance)
			*
			*  @note
			*    - If there's already a shown view of the given class name, it will get the focus
			*    - If there's already a view of the given class name, it will be shown
			*    - If there's currently no view of the given class name, it will be instanced and shown
			*/
			View* showView(const StringHash& viewClassId) const;

			/**
			*  @brief
			*    Close all registered views
			*/
			void closeAllViews();

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
#include "qsf_editor/view/ViewManager-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::ViewManager)
