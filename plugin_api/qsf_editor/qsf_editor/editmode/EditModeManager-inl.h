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
		inline MainWindow& EditModeManager::getMainWindow() const
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mMainWindow, "The QSF editor main window instance is invalid", QSF_REACT_THROW);
			return *mMainWindow;
		}

		inline EditMode* EditModeManager::getSelectedEditMode() const
		{
			return mSelectedEditMode;
		}

		template <typename T>
		bool EditModeManager::isSelectedEditModeInstanceOf() const
		{
			return isSelectedEditModeInstanceOfById(T::PLUGINABLE_ID);
		}

		inline Tool* EditModeManager::getToolWhichSelectedEditMode() const
		{
			return mToolWhichSelectedEditMode;
		}

		template <typename T> void EditModeManager::selectEditMode(Tool* tool)
		{
			return selectEditModeById(T::PLUGINABLE_ID, tool);
		}

		inline EditMode* EditModeManager::getPreviousEditMode() const
		{
			return mPreviousEditMode;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
