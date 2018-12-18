// Copyright (C) 2012-2018 Promotion Software GmbH


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
		inline StatusBarManager::StatusBarManager(MainWindow& mainWindow) :
			mMainWindow(&mainWindow)
		{
			// Register all known and supported CAMP pluginable classes
			registerPluginables();
		}

		inline StatusBarManager::~StatusBarManager()
		{
			// Close all registered status bars
			closeAllStatusBars();

			// Unregister all known and supported CAMP pluginable classes
			unregisterPluginables();
		}

		inline MainWindow& StatusBarManager::getMainWindow() const
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mMainWindow, "The QSF editor main window instance is invalid", QSF_REACT_THROW);
			return *mMainWindow;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
