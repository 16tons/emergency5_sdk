// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/time/Time.h"
#include "qsf/base/UniqueInstance.h"
#include "qsf/settings/SettingsGroup.h"
#include "qsf/reflection/type/CampGlmIVec2.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Renderer settings group implementation
	*
	*  @remarks
	*    QSF supports three window modes
	*    - Window mode
	*      - "Fullscreen"-property is "false"
	*    - Exclusive fullscreen mode (might cause some serious issues for some users)
	*      - "Fullscreen"-property is "true" and "UseBorderlessWindowForFullscreen"-property is "false"
	*    - Borderless window fullscreen mode (maximum possible resolution is limited to the current desktop resolution)
	*      - "Fullscreen"-property is "true" and "UseBorderlessWindowForFullscreen"-property is "true"
	*/
	class QSF_API_EXPORT RendererSettingsGroup : public SettingsGroup, public UniqueInstance<RendererSettingsGroup>
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		// Pluginable
		static const uint32 PLUGINABLE_ID;										///< "qsf::RendererSettingsGroup" unique pluginable settings group ID
		// Properties: Restart required
		static const uint32 RENDERER_SYSTEM;									///< "RendererSystem" unique class property ID inside the class
		static const uint32 INTEGRATED_GPU_CHECK;								///< "IntegratedGpuCheck" unique class property ID inside the class
		static const uint32 VR_ENABLED;											///< "VrEnabled" unique class property ID inside the class
		static const uint32 VR_SYSTEM;											///< "VrSystem" unique class property ID inside the class
		static const uint32 VR_RESOLUTION_SCALE;								///< "VrResolutionScale" unique class property ID inside the class
		// Properties: Runtime change possible
		static const uint32 VR_MONITOR_MODE;									///< "VrMonitorMode" unique class property ID inside the class
		static const uint32 FULLSCREEN;											///< "Fullscreen" unique class property ID inside the class
		static const uint32 USE_BORDERLESS_WINDOW_FOR_FULLSCREEN;				///< "UseBorderlessWindowForFullscreen" unique class property ID inside the class
		static const uint32 FULLSCREEN_MOUSE_CURSOR_TRAP;						///< "FullscreenMouseCursorTrap" unique class property ID inside the class
		static const uint32 WINDOW_SIZE;										///< "WindowSize" unique class property ID inside the class
		static const uint32 RESOLUTION;											///< "Resolution" unique class property ID inside the class
		static const uint32 VERTICAL_SYNCHRONIZATION_ENABLED;					///< "VerticalSynchronizationEnabled" unique class property ID inside the class
		static const uint32 VERTICAL_SYNCHRONIZATION_INTERVAL;					///< "VerticalSynchronizationInterval" unique class property ID inside the class
		static const uint32 MAXIMUM_NUMBER_OF_QUEUED_FRAMES;					///< "MaximumNumberOfQueuedFrames" unique class property ID inside the class
		static const uint32 RENDERING_DISTANCE_FACTOR;							///< "RenderingDistanceFactor" unique class property ID inside the class
		static const uint32 CELL_RENDERING_DISTANCE_FACTOR;						///< "CellRenderingDistanceFactor" unique class property ID inside the class
		static const uint32 LOD_BIAS;											///< "LodBias" unique class property ID inside the class
		static const uint32 MAP_QUALITY;										///< "MapQuality" unique class property ID inside the class
		static const uint32 MESH_QUALITY;										///< "MeshQuality" unique class property ID inside the class
		static const uint32 PARTICLES_QUALITY;									///< "ParticlesQuality" unique class property ID inside the class
		static const uint32 DEFAULT_TEXTURE_FILTERING;							///< "DefaultTextureFiltering" unique class property ID inside the class
		static const uint32 NUMBER_OF_TOP_MIPMAPS_TO_REMOVE;					///< "NumberOfTopMipmapsToRemove" unique class property ID inside the class
		static const uint32 USED_TEXTURES_STREAMING_ONLY;						///< "UsedTexturesStreamingOnly" unique class property ID inside the class
		static const uint32 TEXTURE_STREAMER_DEFAULT_MINIMUM_MIP_LEVEL;			///< "TextureStreamerDefaultMinimumMipLevel" unique class property ID inside the class
		static const uint32 TEXTURE_STREAMER_FRAME_TIME_BUDGET_FOR_IO;			///< "TextureStreamerFrameTimeBudgetForIo" unique class property ID inside the class
		static const uint32 TEXTURE_STREAMER_FRAME_TIME_BUDGET_FOR_VRAM_SYNC;	///< "TextureStreamerFrameTimeBudgetForVramSync" unique class property ID inside the class
		static const uint32 TEXTURE_STREAMER_UNLOAD_TIME_SPAN;					///< "TextureStreamerUnloadTimeSpan" unique class property ID inside the class
		static const uint32 CONTRAST;											///< "Contrast" unique class property ID inside the class
		static const uint32 BRIGHTNESS;											///< "Brightness" unique class property ID inside the class
		// Property default value
		static const glm::ivec2 FULL_HD_RESOLUTION;								///< Full-HD resolution (1920x1080)
		static const glm::ivec2 WINDOW_SIZE_DEFAULT;							///< "WindowSize" default value
		static const glm::ivec2 RESOLUTION_DEFAULT;								///< "Resolution" default value

		/**
		*  @brief
		*    Renderer system
		*/
		enum RendererSystem
		{
			OPENGL_SYSTEM	 = 0,	///< OpenGL renderer system
			DIRECTX11_SYSTEM = 1	///< DirectX 11 renderer system
		};

		/**
		*  @brief
		*    Virtual reality system
		*/
		enum VrSystem
		{
			NULL_SYSTEM					= 0,	///< Null virtual reality system
			AUTOMATIC_DETECTION_SYSTEM	= 1,	///< Automatic virtual reality system detection (if Oculus Rift is detected, usage of the Oculus system is preferred)
			OCULUS_SYSTEM				= 2,	///< Oculus virtual reality system (Oculus Rift)
			OPENVR_SYSTEM				= 3		///< OpenVR virtual reality system (HTC Vive, Oculus Rift)
		};

		/**
		*  @brief
		*    Virtual reality monitor mode
		*/
		enum VrMonitorMode
		{
			NOTHING_MONITOR_MODE	= 0,	///< Show nothing on the PC monitor
			MONO_MONITOR_MODE		= 1,	///< Show just one undistorted eye image on the PC monitor
			STEREO_MONITOR_MODE		= 2		///< Show the distorted images of both eyes on the PC monitor
		};

		/**
		*  @brief
		*    Map quality
		*
		*  @note
		*    This enum is equal with MetadataComponent::Quality and it is also used as the same. Make changes only on both enums
		*/
		enum MapQuality
		{
			LOW_MAP_QUALITY	   = 1,	///< Low map quality
			MEDIUM_MAP_QUALITY = 2,	///< Medium map quality
			HIGH_MAP_QUALITY   = 3,	///< High map quality
			ULTRA_MAP_QUALITY  = 4	///< Ultra map quality
		};

		/**
		*  @brief
		*    Mesh quality
		*/
		enum MeshQuality
		{
			MESH_QUALITY_LOW    = 1,	///< Low mesh quality
			MESH_QUALITY_MEDIUM = 2,	///< Medium mesh quality
			MESH_QUALITY_HIGH   = 3		///< High mesh quality
		};

		/**
		*  @brief
		*    Particles quality
		*/
		enum ParticlesQuality
		{
			LOW_PARTICLES_QUALITY    = 1,	///< Low particles quality
			MEDIUM_PARTICLES_QUALITY = 2,	///< Medium particles quality
			HIGH_PARTICLES_QUALITY   = 3,	///< High particles quality
			ULTRA_PARTICLES_QUALITY  = 4	///< Ultra particles quality
		};

		/**
		*  @brief
		*    Texture filtering quality
		*/
		enum TextureFilteringQuality
		{
			NONE_TEXTURE_FILTERING           = 0,	///< None (cheapest in respect of performance)
			BILINEAR_TEXTURE_FILTERING       = 1,	///< Bilinear
			TRILINEAR_TEXTURE_FILTERING      = 2,	///< Trilinear
			ANISOTROPIC_2_TEXTURE_FILTERING  = 3,	///< 2x anisotropic
			ANISOTROPIC_4_TEXTURE_FILTERING  = 4,	///< 4x anisotropic
			ANISOTROPIC_8_TEXTURE_FILTERING  = 5,	///< 8x anisotropic
			ANISOTROPIC_16_TEXTURE_FILTERING = 6	///< 16x anisotropic (most expensive in respect of performance)
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] settingsGroupManager
		*    Optional pointer to the settings group manager this settings group should be registered to, can be a null pointer
		*/
		inline explicit RendererSettingsGroup(SettingsGroupManager* settingsGroupManager);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~RendererSettingsGroup();

		//[-------------------------------------------------------]
		//[ Restart required                                      ]
		//[-------------------------------------------------------]
		inline RendererSystem getRendererSystem() const;
		inline void setRendererSystem(RendererSystem rendererSystem);

		inline bool isIntegratedGpuCheck() const;
		inline void setIntegratedGpuCheck(bool integratedGpuCheck);

		inline bool isVrEnabled() const;
		inline void setVrEnabled(bool vrEnabled);

		inline VrSystem getVrSystem() const;
		inline void setVrSystem(VrSystem vrSystem);

		inline float getVrResolutionScale() const;
		inline void setVrResolutionScale(float vrResolutionScale);

		inline VrMonitorMode getVrMonitorMode() const;
		inline void setVrMonitorMode(VrMonitorMode vrMonitorMode);

		//[-------------------------------------------------------]
		//[ Runtime change possible                               ]
		//[-------------------------------------------------------]
		inline bool isFullscreen() const;
		inline void setFullscreen(bool fullscreen);
		inline void silentlySetFullscreen(bool fullscreen);

		inline bool getUseBorderlessWindowForFullscreen() const;
		inline void setUseBorderlessWindowForFullscreen(bool useBorderlessWindowForFullscreen);

		inline bool getFullscreenMouseCursorTrap() const;
		inline void setFullscreenMouseCursorTrap(bool fullscreenMouseCursorTrap);

		inline const glm::ivec2& getWindowSize() const;
		inline void setWindowSize(const glm::ivec2& windowSize);

		inline const glm::ivec2& getResolution() const;
		inline void setResolution(const glm::ivec2& resolution);
		inline void silentlySetResolution(const glm::ivec2& resolution);

		inline bool getVerticalSynchronizationEnabled() const;
		inline void setVerticalSynchronizationEnabled(bool verticalSynchronizationEnabled);

		inline uint32 getVerticalSynchronizationInterval() const;
		inline void setVerticalSynchronizationInterval(uint32 verticalSynchronizationInterval);

		inline uint32 getMaximumNumberOfQueuedFrames() const;
		inline void setMaximumNumberOfQueuedFrames(uint32 maximumNumberOfQueuedFrames);

		/**
		*  @brief
		*    Return the rendering distance factor
		*
		*  @return
		*    Rendering distance factor
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getRenderingDistanceFactor() const;

		/**
		*  @brief
		*    Set the rendering distance factor
		*
		*  @param[in] renderingDistanceFactor
		*    Rendering distance factor
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setRenderingDistanceFactor(float renderingDistanceFactor);

		/**
		*  @brief
		*    Return the cell rendering distance factor
		*
		*  @return
		*    Cell rendering distance factor
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getCellRenderingDistanceFactor() const;

		/**
		*  @brief
		*    Set the cell rendering distance factor
		*
		*  @param[in] cellRenderingDistanceFactor
		*    Cell rendering distance factor
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setCellRenderingDistanceFactor(float cellRenderingDistanceFactor);

		/**
		*  @brief
		*    Return the LOD bias
		*
		*  @return
		*    The factor to apply to the usual level of detail calculation. Higher values increase the detail, so 2.0 doubles the normal detail and 0.5 halves it.
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getLodBias() const;

		/**
		*  @brief
		*    Set the LOD bias
		*
		*  @param[in] lodBias
		*    The factor to apply to the usual level of detail calculation. Higher values increase the detail, so 2.0 doubles the normal detail and 0.5 halves it.
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setLodBias(float lodBias);

		/**
		*  @brief
		*    Return the map quality
		*
		*  @return
		*    The map quality
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline MapQuality getMapQuality() const;

		/**
		*  @brief
		*    Set the map quality
		*
		*  @param[in] mapQuality
		*    The map quality
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setMapQuality(MapQuality mapQuality);

		/**
		*  @brief
		*    Returns the mesh quality scheme
		*
		*  @return
		*    Currently active mesh quality scheme
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline MeshQuality getMeshQuality() const;

		/**
		*  @brief
		*    Sets the mesh quality scheme
		*
		*  @param[in] quality
		*    Quality scheme to use for meshes
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setMeshQuality(MeshQuality quality);

		/**
		*  @brief
		*    Returns the particles quality scheme
		*
		*  @return
		*    Currently active particles quality scheme
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline ParticlesQuality getParticlesQuality() const;

		/**
		*  @brief
		*    Sets the particles quality scheme
		*
		*  @param[in] quality
		*    Quality scheme to use for particles
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setParticlesQuality(ParticlesQuality quality);

		/**
		*  @brief
		*    Returns the default texture filtering quality scheme
		*
		*  @return
		*    Currently active default texture filtering quality scheme
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline TextureFilteringQuality getDefaultTextureFiltering() const;

		/**
		*  @brief
		*    Sets the default texture filtering quality scheme
		*
		*  @param[in] quality
		*    Default quality scheme to use for texture filtering
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setDefaultTextureFiltering(TextureFilteringQuality quality);
		void enforceSetDefaultTextureFiltering(TextureFilteringQuality quality);

		/**
		*  @brief
		*    Return the number of top mipmaps to remove
		*
		*  @return
		*    The number of top mipmaps to remove
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline int getNumberOfTopMipmapsToRemove() const;

		/**
		*  @brief
		*    Set the number of top mipmaps to remove
		*
		*  @param[in] numberOfTopMipmapsToRemove
		*    Number of top mipmaps to remove
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setNumberOfTopMipmapsToRemove(int numberOfTopMipmapsToRemove);

		/**
		*  @brief
		*    Returns if the streamer will only stream textures which are used by the scene graph (i.e. which have been loaded at least once by an
		*    outside user - Ogre::v1::Entity - In order to disable textures from streaming, they have to be unloaded. UnloadUnreferenced will not work
		*	 as the use count is wrong due to the reference in the streaming system)
		*/
		inline bool isUsedTexturesStreamingOnlyEnabled() const;
		inline void setUsedTexturesStreamingOnlyEnabled(bool enabled);

		inline uint32 getTextureStreamerDefaultMinimumMipLevel() const;
		inline void setTextureStreamerDefaultMinimumMipLevel(uint32 textureStreamerDefaultMinimumMipLevel);

		inline const Time& getTextureStreamerFrameTimeBudgetForIo() const;
		inline void setTextureStreamerFrameTimeBudgetForIo(const Time& textureStreamerFrameTimeBudgetForIo);

		inline const Time& getTextureStreamerFrameTimeBudgetForVramSync() const;
		inline void setTextureStreamerFrameTimeBudgetForVramSync(const Time& textureStreamerFrameTimeBudgetForVramSync);

		inline const Time& getTextureStreamerUnloadTimeSpan() const;
		inline void setTextureStreamerUnloadTimeSpan(const Time& textureStreamerUnloadTimeSpan);

		inline float getContrast() const;
		inline void setContrast(float contrast);
		inline float getBrightness() const;
		inline void setBrightness(float brightness);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system
		RendererSystem			mRendererSystem;							///< Renderer system to use
		bool					mIntegratedGpuCheck;						///< Perform integrated GPU check?
		bool					mVrEnabled;									///< Virtual reality enabled?
		VrSystem				mVrSystem;									///< VR system to use
		float					mVrResolutionScale;							///< VR resolution scale
		VrMonitorMode			mVrMonitorMode;								///< Virtual reality monitor mode
		bool					mFullscreen;								///< Should the render window be in fullscreen mode?
		bool					mUseBorderlessWindowForFullscreen;			///< Use borderless window in fullscreen mode?
		bool					mFullscreenMouseCursorTrap;					///< Trap the mouse cursor inside the render window in case we're in fullscreen mode?
		glm::ivec2				mWindowSize;								///< Window width and height
		glm::ivec2				mResolution;								///< Fullscreen mode resolution width and height
		bool					mVerticalSynchronizationEnabled;			///< Vertical synchronization enabled? Use vertical synchronization to avoid nasty tearing screen artefacts which come up especially in fullscreen mode.
		uint32					mVerticalSynchronizationInterval;			///< Indicates the number of vertical retraces to wait for before swapping buffers
		uint32					mMaximumNumberOfQueuedFrames;				///< Maximum number of queued frames, 0 for no limitation, set this to trade performance for less latency
		float					mRenderingDistanceFactor;					///< Rendering distance factor
		float					mCellRenderingDistanceFactor;				///< Cell rendering distance factor
		float					mLodBias;									///< The factor to apply to the usual level of detail calculation. Higher values increase the detail, so 2.0 doubles the normal detail and 0.5 halves it.
		MapQuality				mMapQuality;								///< The map quality
		MeshQuality				mMeshQuality;								///< Enforces a certain mesh quality scheme
		ParticlesQuality		mParticlesQuality;							///< Enforces a certain particles quality scheme
		TextureFilteringQuality mDefaultTextureFiltering;					///< Default texture filtering quality scheme
		int						mNumberOfTopMipmapsToRemove;				///< Number of top mipmaps to remove
		bool					mUsedTexturesStreamingOnlyEnabled;			///< Should the streamer not actively stream textures which are not used currently?
		uint32					mTextureStreamerDefaultMinimumMipLevel;		///< Texture streamer default minimum MIP level
		Time					mTextureStreamerFrameTimeBudgetForIo;		///< Texture streamer frame time budget for IO
		Time					mTextureStreamerFrameTimeBudgetForVramSync;	///< Texture streamer frame time budget for VRAM sync
		Time					mTextureStreamerUnloadTimeSpan;				///< Texture streamer unload time span
		float					mContrast;
		float					mBrightness;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/settings/RendererSettingsGroup-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::RendererSettingsGroup)
QSF_CAMP_TYPE_NONCOPYABLE(qsf::RendererSettingsGroup::RendererSystem)
QSF_CAMP_TYPE_NONCOPYABLE(qsf::RendererSettingsGroup::VrSystem)
QSF_CAMP_TYPE_NONCOPYABLE(qsf::RendererSettingsGroup::VrMonitorMode)
QSF_CAMP_TYPE_NONCOPYABLE(qsf::RendererSettingsGroup::MapQuality)
QSF_CAMP_TYPE_NONCOPYABLE(qsf::RendererSettingsGroup::MeshQuality)
QSF_CAMP_TYPE_NONCOPYABLE(qsf::RendererSettingsGroup::ParticlesQuality)
QSF_CAMP_TYPE_NONCOPYABLE(qsf::RendererSettingsGroup::TextureFilteringQuality)
