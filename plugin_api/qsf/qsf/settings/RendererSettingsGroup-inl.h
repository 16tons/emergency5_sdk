// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline RendererSettingsGroup::RendererSettingsGroup(SettingsGroupManager* settingsGroupManager) :
		SettingsGroup(settingsGroupManager),
		#ifdef WIN32
			mRendererSystem(DIRECTX11_SYSTEM),
		#else
			mRendererSystem(OPENGL_SYSTEM),
		#endif
		mIntegratedGpuCheck(true),
		mVrEnabled(false),
		mVrSystem(AUTOMATIC_DETECTION_SYSTEM),
		mVrResolutionScale(1.0f),
		mVrMonitorMode(STEREO_MONITOR_MODE),
		mFullscreen(false),
		mUseBorderlessWindowForFullscreen(true),
		mFullscreenMouseCursorTrap(true),
		mWindowSize(WINDOW_SIZE_DEFAULT),
		mResolution(RESOLUTION_DEFAULT),
		mVerticalSynchronizationEnabled(false),
		mVerticalSynchronizationInterval(1),
		mMaximumNumberOfQueuedFrames(0),
		mRenderingDistanceFactor(120.0f),
		mCellRenderingDistanceFactor(400.0f),
		mLodBias(1.0f),
		mMapQuality(ULTRA_MAP_QUALITY),
		mMeshQuality(MESH_QUALITY_HIGH),
		mParticlesQuality(ULTRA_PARTICLES_QUALITY),
		mDefaultTextureFiltering(BILINEAR_TEXTURE_FILTERING),
		mNumberOfTopMipmapsToRemove(0),
		mUsedTexturesStreamingOnlyEnabled(true),
		mTextureStreamerDefaultMinimumMipLevel(3U),
		mTextureStreamerFrameTimeBudgetForIo(Time::fromSeconds(0.025f)),
		mTextureStreamerFrameTimeBudgetForVramSync(Time::fromSeconds(0.025f)),
		mTextureStreamerUnloadTimeSpan(Time::fromSeconds(10.0f)),
		mContrast(1.0f),
		mBrightness(1.0f)
	{
		// Nothing to do in here
	}

	inline RendererSettingsGroup::~RendererSettingsGroup()
	{
		// Nothing to do in here
	}

	inline RendererSettingsGroup::RendererSystem RendererSettingsGroup::getRendererSystem() const
	{
		return mRendererSystem;
	}

	inline void RendererSettingsGroup::setRendererSystem(RendererSystem rendererSystem)
	{
		assignAndPromoteChange(mRendererSystem, rendererSystem, RENDERER_SYSTEM);
	}

	inline RendererSettingsGroup::VrSystem RendererSettingsGroup::getVrSystem() const
	{
		return mVrSystem;
	}

	inline void RendererSettingsGroup::setVrSystem(VrSystem vrSystem)
	{
		assignAndPromoteChange(mVrSystem, vrSystem, VR_SYSTEM);
	}

	inline float RendererSettingsGroup::getVrResolutionScale() const
	{
		return mVrResolutionScale;
	}

	inline void RendererSettingsGroup::setVrResolutionScale(float vrResolutionScale)
	{
		assignAndPromoteChange(mVrResolutionScale, vrResolutionScale, VR_RESOLUTION_SCALE);
	}

	inline RendererSettingsGroup::VrMonitorMode RendererSettingsGroup::getVrMonitorMode() const
	{
		return mVrMonitorMode;
	}

	inline void RendererSettingsGroup::setVrMonitorMode(VrMonitorMode vrMonitorMode)
	{
		assignAndPromoteChange(mVrMonitorMode, vrMonitorMode, VR_MONITOR_MODE);
	}

	inline bool RendererSettingsGroup::isIntegratedGpuCheck() const
	{
		return mIntegratedGpuCheck;
	}

	inline void RendererSettingsGroup::setIntegratedGpuCheck(bool integratedGpuCheck)
	{
		assignAndPromoteChange(mIntegratedGpuCheck, integratedGpuCheck, INTEGRATED_GPU_CHECK);
	}

	inline bool RendererSettingsGroup::isVrEnabled() const
	{
		return mVrEnabled;
	}

	inline void RendererSettingsGroup::setVrEnabled(bool vrEnabled)
	{
		assignAndPromoteChange(mVrEnabled, vrEnabled, VR_ENABLED);
	}

	inline bool RendererSettingsGroup::isFullscreen() const
	{
		return mFullscreen;
	}

	inline void RendererSettingsGroup::setFullscreen(bool fullscreen)
	{
		assignAndPromoteChange(mFullscreen, fullscreen, FULLSCREEN);
	}

	inline void RendererSettingsGroup::silentlySetFullscreen(bool fullscreen)
	{
		mFullscreen = fullscreen;
	}

	inline bool RendererSettingsGroup::getUseBorderlessWindowForFullscreen() const
	{
		return mUseBorderlessWindowForFullscreen;
	}

	inline void RendererSettingsGroup::setUseBorderlessWindowForFullscreen(bool useBorderlessWindowForFullscreen)
	{
		assignAndPromoteChange(mUseBorderlessWindowForFullscreen, useBorderlessWindowForFullscreen, USE_BORDERLESS_WINDOW_FOR_FULLSCREEN);
	}

	inline bool RendererSettingsGroup::getFullscreenMouseCursorTrap() const
	{
		return mFullscreenMouseCursorTrap;
	}

	inline void RendererSettingsGroup::setFullscreenMouseCursorTrap(bool fullscreenMouseCursorTrap)
	{
		assignAndPromoteChange(mFullscreenMouseCursorTrap, fullscreenMouseCursorTrap, FULLSCREEN_MOUSE_CURSOR_TRAP);
	}

	inline const glm::ivec2& RendererSettingsGroup::getWindowSize() const
	{
		return mWindowSize;
	}

	inline void RendererSettingsGroup::setWindowSize(const glm::ivec2& windowSize)
	{
		assignAndPromoteChange(mWindowSize, windowSize, WINDOW_SIZE);
	}

	inline const glm::ivec2& RendererSettingsGroup::getResolution() const
	{
		return mResolution;
	}

	inline void RendererSettingsGroup::setResolution(const glm::ivec2& resolution)
	{
		assignAndPromoteChange(mResolution, resolution, RESOLUTION);
	}

	inline void RendererSettingsGroup::silentlySetResolution(const glm::ivec2& resolution)
	{
		mResolution = resolution;
	}

	inline bool RendererSettingsGroup::getVerticalSynchronizationEnabled() const
	{
		return mVerticalSynchronizationEnabled;
	}

	inline void RendererSettingsGroup::setVerticalSynchronizationEnabled(bool verticalSynchronizationEnabled)
	{
		assignAndPromoteChange(mVerticalSynchronizationEnabled, verticalSynchronizationEnabled, VERTICAL_SYNCHRONIZATION_ENABLED);
	}

	inline uint32 RendererSettingsGroup::getVerticalSynchronizationInterval() const
	{
		return mVerticalSynchronizationInterval;
	}

	inline void RendererSettingsGroup::setVerticalSynchronizationInterval(uint32 verticalSynchronizationInterval)
	{
		assignAndPromoteChange(mVerticalSynchronizationInterval, verticalSynchronizationInterval, VERTICAL_SYNCHRONIZATION_INTERVAL);
	}

	inline uint32 RendererSettingsGroup::getMaximumNumberOfQueuedFrames() const
	{
		return mMaximumNumberOfQueuedFrames;
	}

	inline void RendererSettingsGroup::setMaximumNumberOfQueuedFrames(uint32 maximumNumberOfQueuedFrames)
	{
		assignAndPromoteChange(mMaximumNumberOfQueuedFrames, maximumNumberOfQueuedFrames, MAXIMUM_NUMBER_OF_QUEUED_FRAMES);
	}

	inline float RendererSettingsGroup::getRenderingDistanceFactor() const
	{
		return mRenderingDistanceFactor;
	}

	inline float RendererSettingsGroup::getCellRenderingDistanceFactor() const
	{
		return mCellRenderingDistanceFactor;
	}

	inline float RendererSettingsGroup::getLodBias() const
	{
		return mLodBias;
	}

	inline RendererSettingsGroup::MapQuality RendererSettingsGroup::getMapQuality() const
	{
		return mMapQuality;
	}

	inline void RendererSettingsGroup::setMapQuality(MapQuality mapQuality)
	{
		assignAndPromoteChange(mMapQuality, mapQuality, MAP_QUALITY);
	}

	inline RendererSettingsGroup::MeshQuality RendererSettingsGroup::getMeshQuality() const
	{
		return mMeshQuality;
	}

	inline RendererSettingsGroup::ParticlesQuality RendererSettingsGroup::getParticlesQuality() const
	{
		return mParticlesQuality;
	}

	inline RendererSettingsGroup::TextureFilteringQuality RendererSettingsGroup::getDefaultTextureFiltering() const
	{
		return mDefaultTextureFiltering;
	}

	inline int RendererSettingsGroup::getNumberOfTopMipmapsToRemove() const
	{
		return mNumberOfTopMipmapsToRemove;
	}

	inline void RendererSettingsGroup::setNumberOfTopMipmapsToRemove(int numberOfTopMipmapsToRemove)
	{
		assignAndPromoteChange(mNumberOfTopMipmapsToRemove, numberOfTopMipmapsToRemove, NUMBER_OF_TOP_MIPMAPS_TO_REMOVE);
	}

	inline bool RendererSettingsGroup::isUsedTexturesStreamingOnlyEnabled() const
	{
		return mUsedTexturesStreamingOnlyEnabled;
	}

	inline void RendererSettingsGroup::setUsedTexturesStreamingOnlyEnabled(bool enabled)
	{
		assignAndPromoteChange(mUsedTexturesStreamingOnlyEnabled, enabled, USED_TEXTURES_STREAMING_ONLY);
	}

	inline uint32 RendererSettingsGroup::getTextureStreamerDefaultMinimumMipLevel() const
	{
		return mTextureStreamerDefaultMinimumMipLevel;
	}

	inline void RendererSettingsGroup::setTextureStreamerDefaultMinimumMipLevel(uint32 textureStreamerDefaultMinimumMipLevel)
	{
		assignAndPromoteChange(mTextureStreamerDefaultMinimumMipLevel, textureStreamerDefaultMinimumMipLevel, TEXTURE_STREAMER_DEFAULT_MINIMUM_MIP_LEVEL);
	}

	inline const Time& RendererSettingsGroup::getTextureStreamerFrameTimeBudgetForIo() const
	{
		return mTextureStreamerFrameTimeBudgetForIo;
	}

	inline void RendererSettingsGroup::setTextureStreamerFrameTimeBudgetForIo(const Time& textureStreamerFrameTimeBudgetForIo)
	{
		assignAndPromoteChange(mTextureStreamerFrameTimeBudgetForIo, textureStreamerFrameTimeBudgetForIo, TEXTURE_STREAMER_FRAME_TIME_BUDGET_FOR_IO);
	}

	inline const Time& RendererSettingsGroup::getTextureStreamerFrameTimeBudgetForVramSync() const
	{
		return mTextureStreamerFrameTimeBudgetForVramSync;
	}

	inline void RendererSettingsGroup::setTextureStreamerFrameTimeBudgetForVramSync(const Time& textureStreamerFrameTimeBudgetForVramSync)
	{
		assignAndPromoteChange(mTextureStreamerFrameTimeBudgetForVramSync, textureStreamerFrameTimeBudgetForVramSync, TEXTURE_STREAMER_FRAME_TIME_BUDGET_FOR_VRAM_SYNC);
	}

	inline const Time& RendererSettingsGroup::getTextureStreamerUnloadTimeSpan() const
	{
		return mTextureStreamerUnloadTimeSpan;
	}

	inline void RendererSettingsGroup::setTextureStreamerUnloadTimeSpan(const Time& textureStreamerUnloadTimeSpan)
	{
		assignAndPromoteChange(mTextureStreamerUnloadTimeSpan, textureStreamerUnloadTimeSpan, TEXTURE_STREAMER_UNLOAD_TIME_SPAN);
	}

	inline float RendererSettingsGroup::getContrast() const
	{
		return mContrast;
	}

	inline void RendererSettingsGroup::setContrast(float contrast)
	{
		assignAndPromoteChange(mContrast, contrast, CONTRAST);
	}

	inline float RendererSettingsGroup::getBrightness() const
	{
		return mBrightness;
	}

	inline void RendererSettingsGroup::setBrightness(float brightness)
	{
		assignAndPromoteChange(mBrightness, brightness, BRIGHTNESS);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
