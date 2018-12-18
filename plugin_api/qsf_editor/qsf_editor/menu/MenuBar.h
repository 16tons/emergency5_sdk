// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <QtWidgets/qmenubar.h>

#include <boost/noncopyable.hpp>

#include <string>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
QT_BEGIN_NAMESPACE
	class QMenu;
	class QAction;
QT_END_NAMESPACE
namespace camp
{
	class Class;
}
namespace qsf
{
	namespace editor
	{
		class MainWindow;
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
		*    QSF editor menu bar
		*/
		class MenuBar : public QMenuBar, public boost::noncopyable
		{


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
					// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] mainWindow
			*    Owner main window, must stay valid as long as this menu bar exists
			*/
			explicit MenuBar(MainWindow& mainWindow);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~MenuBar();


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Create the menu bar
			*/
			void createMenuBar();

			/**
			*  @brief
			*    Called after an user rights change has occurred
			*/
			void onUserRightsChange();

			/**
			*  @brief
			*    Get the CAMP class of the about dialog to use
			*/
			const camp::Class& getAboutDialogCampClass() const;


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			// Menu - View
			void onAboutToShowMenuView();
			void onAboutToShowMenuToolBar();
			void onAboutToShowMenuStatusBar();
			void onTriggeredMenuViewCloseAll();
			// Menu - Tool
			void onAboutToShowMenuTool();
			// Menu - Customize
			void onAboutToShowMenuCustomize();
			void onTriggeredMenuCustomizeLoadCustomUiScheme();
			void onTriggeredMenuCustomizeSaveCustomUiScheme();
			void onTriggeredMenuCustomizeRevertToStartupLayout();
			void onTriggeredMenuCustomizeLockLayout();
			void onTriggeredMenuCustomizeShortcutMapper();
			void onTriggeredMenuCustomizeLocalAssetDatasources();
			void onTriggeredMenuCustomizePreferences();
			// Menu - Help
			void onTriggeredMenuHelpAbout();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			MainWindow* mMainWindow;			///< Owner main window, always valid, do not destroy the instance
			QMenu*		mQMenuView;				///< View Qt menu, can be a null pointer, Qt will destroy the instance automatically
			QMenu*		mQMenuToolBar;			///< Tool bar Qt menu, can be a null pointer, Qt will destroy the instance automatically
			QMenu*		mQMenuStatusBar;		///< Status bar Qt menu, can be a null pointer, Qt will destroy the instance automatically
			QMenu*		mQMenuTool;				///< Tool Qt menu, can be a null pointer, Qt will destroy the instance automatically
			QAction*	mCustomizeLockLayout;	///< Customize lock layout, can be a null pointer, Qt will destroy the instance automatically


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
