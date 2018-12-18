// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/error/BoostAssertHandler.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline bool GuiContext::isEnabled() const
	{
		return mEnabled;
	}

	inline void GuiContext::setEnabled(bool enabled)
	{
		mEnabled = enabled;
	}

	inline float GuiContext::getVisibility() const
	{
		return mVisibility;
	}

	inline void GuiContext::setVisibility(float visibility)
	{
		mVisibility = visibility;
	}

	inline const glm::ivec2& GuiContext::getFixedDimension() const
	{
		return mFixedDimension;
	}

	inline void GuiContext::setFixedDimension(const glm::ivec2& fixedDimension)
	{
		mFixedDimension = fixedDimension;
	}

	inline const glm::ivec2& GuiContext::getMinimumDimension() const
	{
		return mMinimumDimension;
	}

	inline void GuiContext::setMinimumDimension(const glm::ivec2& minimumDimension)
	{
		mMinimumDimension = minimumDimension;
	}

	inline bool GuiContext::isAspectRatioCorrectionEnabled() const
	{
		return mAspectRatioCorrectionEnabled;
	}

	inline void GuiContext::setAspectRatioCorrectionEnabled(bool enabled)
	{
		mAspectRatioCorrectionEnabled = enabled;
	}

	inline Rocket::Core::Context& GuiContext::getRocketCoreContext() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mRocketCoreContext, "The libRocket core context instance is invalid", QSF_REACT_THROW);
		return *mRocketCoreContext;
	}

	inline const GuiContext::RenderWindows& GuiContext::getRenderWindows() const
	{
		return mRenderWindows;
	}

	inline bool GuiContext::getFixedMouseCursorPosition() const
	{
		return mFixedMouseCursorPosition;
	}

	inline const glm::vec2& GuiContext::getMouseCursorPosition() const
	{
		return mMouseCursorPosition;
	}

	inline Ogre::SceneManager* GuiContext::getOgreSceneManager() const
	{
		return mOgreSceneManager;
	}

	inline Ogre::MovableObject* GuiContext::getGuiContextMoveable() const
	{
		return mGuiContextMoveable;
	}

	inline Ogre::Camera* GuiContext::getOgreCamera() const
	{
		return mOgreCamera;
	}

	inline bool GuiContext::isScissorRectangleUsageEnabled() const
	{
		return mScissorRectangleUsageEnabled;
	}

	inline void GuiContext::setScissorRectangleUsageEnabled(bool enabled)
	{
		mScissorRectangleUsageEnabled = enabled;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
