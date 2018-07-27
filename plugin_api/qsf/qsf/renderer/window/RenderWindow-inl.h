// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline bool RenderWindow::isDebugOverlayEnabled() const
	{
		return mDebugOverlayEnabled;
	}

	inline bool RenderWindow::getModifySystemCursorVisibilityOnFullscreenToggle() const
	{
		return mModifySystemCursorVisibility;
	}

	inline void RenderWindow::setModifySystemCursorVisibilityOnFullscreenToggle(bool enabled)
	{
		mModifySystemCursorVisibility = enabled;
	}

	inline CameraComponent* RenderWindow::getCameraComponent() const
	{
		return mCameraComponent;
	}

	inline GuiContext* RenderWindow::getGuiContext() const
	{
		return mGuiContext;
	}

	inline Ogre::RenderWindow* RenderWindow::getOgreRenderWindow() const
	{
		return mOgreRenderWindow;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
