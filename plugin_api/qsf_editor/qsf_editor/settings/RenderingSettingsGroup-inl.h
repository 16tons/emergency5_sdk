// Copyright (C) 2012-2018 Promotion Software GmbH


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
		inline RenderingSettingsGroup::RenderingSettingsGroup(SettingsGroupManager* settingsGroupManager) :
			SettingsGroup(settingsGroupManager),
			mFpsLimit(30.0f),
			mCameraMode(CameraManager::CAMERA_MODE_FREE_MOVE),
			mCameraMoveSpeedFactor(1.0f),
			mVisualHighlighting(true),
			mShowIcons(false),
			mShowCompass(true),
			mScreenshotCapturingSize(DEFAULT_SCREENSHOT_CAPTURING_SIZE),
			mHdrScreenshotCapturing(false)
		{
			// Nothing to do in here
		}

		inline RenderingSettingsGroup::~RenderingSettingsGroup()
		{
			// Nothing to do in here
		}

		inline float RenderingSettingsGroup::getFpsLimit() const
		{
			return mFpsLimit;
		}

		inline void RenderingSettingsGroup::setFpsLimit(float fpsLimit)
		{
			assignAndPromoteChange(mFpsLimit, fpsLimit, FPS_LIMIT);
		}

		inline CameraManager::CameraMode RenderingSettingsGroup::getCameraMode() const
		{
			return mCameraMode;
		}

		inline void RenderingSettingsGroup::setCameraMode(CameraManager::CameraMode mode)
		{
			assignAndPromoteChange(mCameraMode, mode, CAMERA_MODE);
		}

		inline float RenderingSettingsGroup::getCameraMovementSpeedFactor() const
		{
			return mCameraMoveSpeedFactor;
		}

		inline void RenderingSettingsGroup::setCameraMovementSpeedFactor(float speedFactor)
		{
			assignAndPromoteChange(mCameraMoveSpeedFactor, speedFactor, CAMERA_MOVEMENT_SPEED_FACTOR);
		}

		inline bool RenderingSettingsGroup::getVisualHighlighting() const
		{
			return mVisualHighlighting;
		}

		inline void RenderingSettingsGroup::setVisualHighlighting(bool visualHighlighting)
		{
			assignAndPromoteChange(mVisualHighlighting, visualHighlighting, VISUAL_HIGHLIGHTING);
		}

		inline bool RenderingSettingsGroup::getShowIcons() const
		{
			return mShowIcons;
		}

		inline void RenderingSettingsGroup::setShowIcons(bool showIcons)
		{
			assignAndPromoteChange(mShowIcons, showIcons, SHOW_ICONS);
		}

		inline const glm::ivec2& RenderingSettingsGroup::getScreenshotCapturingSize() const
		{
			return mScreenshotCapturingSize;
		}

		inline void RenderingSettingsGroup::setShowCompass(bool showCompass)
		{
			assignAndPromoteChange(mShowCompass, showCompass, SHOW_COMPASS);
		}

		inline bool RenderingSettingsGroup::getShowCompass() const
		{
			return mShowCompass;
		}

		inline void RenderingSettingsGroup::setScreenshotCapturingSize(const glm::ivec2& screenshotCapturingSize)
		{
			assignAndPromoteChange(mScreenshotCapturingSize, screenshotCapturingSize, SCREENSHOT_CAPTURING_SIZE);
		}

		inline bool RenderingSettingsGroup::getHdrScreenshotCapturing() const
		{
			return mHdrScreenshotCapturing;
		}

		inline void RenderingSettingsGroup::setHdrScreenshotCapturing(bool hdrScreenshotCapturing)
		{
			assignAndPromoteChange(mHdrScreenshotCapturing, hdrScreenshotCapturing, HDR_SCREENSHOT_CAPTURING);
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
