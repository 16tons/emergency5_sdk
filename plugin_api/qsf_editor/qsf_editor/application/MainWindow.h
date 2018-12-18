// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/application/Application.h"

#include <QtWidgets/qmainwindow.h>


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
		class RenderView;
		class ToolManager;
		class ViewManager;
		class DialogManager;
		class ToolBarManager;
		class StatusBarManager;
		class EditModeManager;
		class CursorManager;
		class IconManager;
		class AssetPreviewCache;
		class EntityPreviewManager;
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
		*    QSF editor main window
		*
		*  @note
		*    - Caveat: QMainWindow's native "window handle" is a "NSView*" (a control within the window) os Mac OS X, not a "NSWindow*"
		*/
		class MainWindow : public QMainWindow
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
			*  @param[in] application
			*    QSF editor application instance
			*/
			explicit MainWindow(Application& application);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~MainWindow();

			/**
			*  @brief
			*    Create menu bar
			*/
			void createMenuBar();

			/**
			*  @brief
			*    Set automatic window title by using the filename of the QSF-map
			*/
			void setAutomaticWindowTitle();

			/**
			*  @brief
			*    Try to close the main window
			*
			*  @param[in] stage
			*    Application stage to switch into after successful close
			*
			*  @return
			*    "true" if the close request was successful, else "false"
			*/
			bool tryClose(Application::Stage stage = Application::EXIT_STAGE);

			//[-------------------------------------------------------]
			//[ Infrastructure                                        ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Return the owner QSF editor application instance
			*
			*  @return
			*    The owner application instance, do no destroy the returned instance
			*/
			inline Application& getApplication() const;

			/**
			*  @brief
			*    Return the render view instance
			*
			*  @return
			*    The render view instance, do no destroy the returned instance
			*/
			inline RenderView& getRenderView() const;

			/**
			*  @brief
			*    Return the dialog manager instance
			*
			*  @return
			*    The dialog manager instance, do no destroy the returned instance
			*/
			inline DialogManager& getDialogManager() const;

			/**
			*  @brief
			*    Return the view manager instance
			*
			*  @return
			*    The view manager instance, do no destroy the returned instance
			*/
			inline ViewManager& getViewManager() const;

			/**
			*  @brief
			*    Return the tool bar manager instance
			*
			*  @return
			*    The tool bar manager instance, do no destroy the returned instance
			*/
			inline ToolBarManager& getToolBarManager() const;

			/**
			*  @brief
			*    Return the edit mode manager instance
			*
			*  @return
			*    The edit mode manager instance, do no destroy the returned instance
			*/
			inline EditModeManager& getEditModeManager() const;

			/**
			*  @brief
			*    Return the status bar manager instance
			*
			*  @return
			*    The status bar manager instance, do no destroy the returned instance
			*/
			inline StatusBarManager& getStatusBarManager() const;

			/**
			*  @brief
			*    Return the tool manager instance
			*
			*  @return
			*    The tool manager instance, do no destroy the returned instance
			*/
			inline ToolManager& getToolManager() const;

			/**
			*  @brief
			*    Return the cursor manager instance
			*
			*  @return
			*    The cursor manager instance, do no destroy the returned instance
			*/
			inline CursorManager& getCursorManager() const;

			/**
			*  @brief
			*    Return the icon manager instance
			*
			*  @return
			*    The icon manager instance, do no destroy the returned instance
			*/
			inline IconManager& getIconManager() const;

			/**
			*  @brief
			*    Return the asset preview cache instance
			*
			*  @return
			*    Asset preview cache instance, do not destroy the instance
			*/
			inline AssetPreviewCache& getAssetPreviewCache() const;

			/**
			*  @brief
			*    Return the entity preview manager instance
			*
			*  @return
			*    The entity preview manager instance, do no destroy the returned instance
			*/
			inline EntityPreviewManager& getEntityPreviewManager() const;

			//[-------------------------------------------------------]
			//[ Settings                                              ]
			//[-------------------------------------------------------]
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
		//[ Protected virtual QWidget methods                     ]
		//[-------------------------------------------------------]
		protected:
			virtual void closeEvent(QCloseEvent* qCloseEvent) override;
			virtual void changeEvent(QEvent* qEvent) override;


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onConnectionLost();
			void onCleanChanged(bool clean);
			void onAssetDeleteRequestResult(GlobalAssetId globalAssetId, bool success, const std::vector<GlobalAssetId>& dependencyList);
			// qsf::editor::Application
			void onPreMapRebuild();
			void onPostMapRebuild();


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void updateMenuBarBackgroundColor();
			void mapStateUpdated();

			/**
			*  @brief
			*    Create the render view of the main window
			*/
			void createRenderView();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			bool					mExitInProgress;		///< Exit in progress?
			Application*			mApplication;			///< QSF editor application instance, always valid, do not destroy the instance
			RenderView*				mRenderView;			///< Render view of the Qt main window, always valid, Qt will destroy the instance automatically
			DialogManager*			mDialogManager;			///< Dialog manager instance, always valid, we're responsible for destroying the instance in case we no longer need it
			ViewManager*			mViewManager;			///< View manager instance, always valid, we're responsible for destroying the instance in case we no longer need it
			ToolBarManager*			mToolBarManager;		///< Tool bar manager instance, always valid, we're responsible for destroying the instance in case we no longer need it
			EditModeManager*		mEditModeManager;		///< Edit mode manager instance, always valid, we're responsible for destroying the instance in case we no longer need it
			StatusBarManager*		mStatusBarManager;		///< Status bar manager instance, always valid, we're responsible for destroying the instance in case we no longer need it
			ToolManager*			mToolManager;			///< Tool instance, always valid, we're responsible for destroying the instance in case we no longer need it
			CursorManager*			mCursorManager;			///< Cursor manager instance, can be a null pointer, we have to destroy the instance in case we no longer need it
			IconManager*			mIconManager;			///< Icon manager instance, always valid, we're responsible for destroying the instance in case we no longer need it
			EntityPreviewManager*	mEntityPreviewManager;	///< Entity preview manager instance, always valid, we're responsible for destroying the instance in case we no longer need it
			AssetPreviewCache*		mAssetPreviewCache;		///< Asset preview cache instance, always valid while the application is running, we have to destroy the instance in case we no longer need it


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor/application/MainWindow-inl.h"
