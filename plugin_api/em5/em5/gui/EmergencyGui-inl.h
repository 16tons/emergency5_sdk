// Copyright (C) 2012-2017 Promotion Software GmbH


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
	inline qsf::GuiContext& EmergencyGui::getGuiContext() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mGuiContext, "The GUI context instance is invalid", QSF_REACT_NONE);
		return *mGuiContext;
	}

	inline StartMenuManager& EmergencyGui::getStartMenuManager() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mStartMenuManager, "The start menu manager instance is invalid", QSF_REACT_NONE);
		return *mStartMenuManager;
	}

	inline IngameHud& EmergencyGui::getIngameHud() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mIngameHud, "The in-game HUD instance is invalid", QSF_REACT_NONE);
		return *mIngameHud;
	}

	inline IngameHud* EmergencyGui::tryGetIngameHud() const
	{
		return mIngameHud;
	}

	inline PatchUpdateProgressDialog& EmergencyGui::getPatchUpdateProgressDialog() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mPatchUpdateProgressDialog, "The patch update progress dialog instance is invalid", QSF_REACT_NONE);
		return *mPatchUpdateProgressDialog;
	}

	inline ToolTipManager& EmergencyGui::getToolTipManager() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mToolTipManager, "The tooltip manager instance is invalid", QSF_REACT_NONE);
		return *mToolTipManager;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
