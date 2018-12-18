// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/error/ErrorHandling.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline StartMenuBaseLayer& StartMenuManager::getStartMenuBaseLayerWindow() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mStartMenuBaseLayerWindow, "The start menu base layer window instance is invalid", QSF_REACT_NONE);
		return *mStartMenuBaseLayerWindow;
	}

	inline StartMenuInstalled& StartMenuManager::getStartMenuInstalledWindow() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mStartMenuInstalledWindow, "The start menu installed window instance is invalid", QSF_REACT_NONE);
		return *mStartMenuInstalledWindow;
	}

	inline ModificationWindow& StartMenuManager::getModificationWindow() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mModificationWindow, "The modification window instance is invalid", QSF_REACT_NONE);
		return *mModificationWindow;
	}

	inline ModificationWindowDataSource& StartMenuManager::getModificationWindowDataSource() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mModificationWindowDataSource, "The modification data source instance is invalid", QSF_REACT_NONE);
		return *mModificationWindowDataSource;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
