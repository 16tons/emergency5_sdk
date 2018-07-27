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
		inline ViewManager::ViewManager(MainWindow& mainWindow) :
			mMainWindow(&mainWindow),
			mLockLayout(false)
		{
			// Register all known and supported CAMP pluginable classes
			registerPluginables();
		}

		inline ViewManager::~ViewManager()
		{
			// Close all registered views
			closeAllViews();

			// Unregister all known and supported CAMP pluginable classes
			unregisterPluginables();
		}

		inline MainWindow& ViewManager::getMainWindow() const
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mMainWindow, "The QSF editor main window instance is invalid", QSF_REACT_THROW);
			return *mMainWindow;
		}

		inline bool ViewManager::getLockLayout() const
		{
			return mLockLayout;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
