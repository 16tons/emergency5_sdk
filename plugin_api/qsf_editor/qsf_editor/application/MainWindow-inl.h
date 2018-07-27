// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/error/BoostAssertHandler.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline Application& MainWindow::getApplication() const
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mApplication, "QSF editor: The QSF editor application instance is invalid", QSF_REACT_THROW);
			return *mApplication;
		}

		inline RenderView& MainWindow::getRenderView() const
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mRenderView, "QSF editor: The render view instance is invalid", QSF_REACT_THROW);
			return *mRenderView;
		}

		inline DialogManager& MainWindow::getDialogManager() const
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mDialogManager, "QSF editor: The dialog manager instance is invalid", QSF_REACT_THROW);
			return *mDialogManager;
		}

		inline ViewManager& MainWindow::getViewManager() const
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mViewManager, "QSF editor: The view manager instance is invalid", QSF_REACT_THROW);
			return *mViewManager;
		}

		inline ToolBarManager& MainWindow::getToolBarManager() const
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mToolBarManager, "QSF editor: The tool bar manager instance is invalid", QSF_REACT_THROW);
			return *mToolBarManager;
		}

		inline EditModeManager& MainWindow::getEditModeManager() const
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mEditModeManager, "QSF editor: The edit mode manager instance is invalid", QSF_REACT_THROW);
			return *mEditModeManager;
		}

		inline StatusBarManager& MainWindow::getStatusBarManager() const
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mStatusBarManager, "QSF editor: The status bar manager instance is invalid", QSF_REACT_THROW);
			return *mStatusBarManager;
		}

		inline ToolManager& MainWindow::getToolManager() const
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mToolManager, "QSF editor: The tool manager instance is invalid", QSF_REACT_THROW);
			return *mToolManager;
		}

		inline CursorManager& MainWindow::getCursorManager() const
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mCursorManager, "QSF editor: The cursor manager instance is invalid", QSF_REACT_THROW);
			return *mCursorManager;
		}

		inline IconManager& MainWindow::getIconManager() const
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mIconManager, "QSF editor: The icon manager instance is invalid", QSF_REACT_THROW);
			return *mIconManager;
		}

		inline AssetPreviewCache& MainWindow::getAssetPreviewCache() const
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mAssetPreviewCache, "QSF editor: The asset preview cache instance is invalid", QSF_REACT_THROW);
			return *mAssetPreviewCache;
		}

		inline EntityPreviewManager& MainWindow::getEntityPreviewManager() const
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mEntityPreviewManager, "QSF editor: The entity preview manager instance is invalid", QSF_REACT_THROW);
			return *mEntityPreviewManager;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
