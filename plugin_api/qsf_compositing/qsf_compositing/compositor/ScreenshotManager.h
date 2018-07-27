// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_compositing/Export.h"

#include <qsf/base/manager/Manager.h>

#include <string>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class Image;
	class Camera;
	class Texture;
	class PixelBox;
	class RenderTarget;
	class RenderTexture;
	template<typename T> class SharedPtr;
	typedef SharedPtr<Texture> TexturePtr;
	namespace v1
	{
		class HardwarePixelBufferSharedPtr;
	}
}
namespace qsf
{
	class GuiContext;
	class RendererSystem;
	class CameraComponent;
	class GridSceneManager;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace compositing
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Screenshot manager class
		*/
		class QSF_COMPOSITING_API_EXPORT ScreenshotManager : public Manager
		{


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
			friend class CompositorSystem;


		//[-------------------------------------------------------]
		//[ Public static                                         ]
		//[-------------------------------------------------------]
		public:
			static void ogreTextureToOgreImage(Ogre::Texture& ogreTexture, Ogre::Image& ogreImage, int ogrePixelFormat = 0);			// "Ogre::PixelFormat" not used to keep the header include complexity down
			static void saveOgreTextureToFile(Ogre::Texture& ogreTexture, const std::string& absoluteFilename, int ogrePixelFormat);	// "Ogre::PixelFormat" not used to keep the header include complexity down
			static void saveOgreImageToFile(Ogre::Image& ogreImage, const std::string& absoluteFilename);


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			void startupScreenshotCapturing(uint32 width, uint32 height, int gridSize, const std::string& fileExtension, bool hdrScreenshotCapturing, bool disableOverlays);	// Render into internal texture
			void startupScreenshotCapturing(Ogre::TexturePtr& ogreTexturePtr, bool hdrScreenshotCapturing, bool disableOverlays);	// Render into provided texture
			void captureScreenshot(CameraComponent& cameraComponent, const std::string& absoluteFilename, GuiContext* guiContext = nullptr);	// In case no absolute filename is provided, no image will be written into a file
			void shutdownScreenshotCapturing();

			// Ease-of-use
			void instantScreenshotCapturing(uint32 width, uint32 height, int gridSize, const std::string& fileExtension, bool hdrScreenshotCapturing, bool disableOverlays, CameraComponent& cameraComponent, const std::string& absoluteFilename, GuiContext* guiContext = nullptr);
			void instantScreenshotCapturing(Ogre::TexturePtr& ogreTexturePtr, bool hdrScreenshotCapturing, bool disableOverlays, CameraComponent& cameraComponent, GuiContext* guiContext = nullptr);
			std::string instantScreenshotCapturingWithDefaultSettings(CameraComponent& cameraComponent, GuiContext* guiContext = nullptr, const glm::ivec2* screenshotCapturingSize = nullptr);	// Returns the absolute filename of the saved screenshot


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Default constructor
			*/
			ScreenshotManager();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~ScreenshotManager();

			void renderFrame(GridSceneManager& gridSceneManager, Ogre::Camera* ogreCamera, Ogre::RenderTarget& ogreRenderTarget);

			//[-------------------------------------------------------]
			//[ Lifecycle                                             ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Startup the screenshot manager
			*
			*  @return
			*    "true" if all went fine, else "false"
			*/
			bool onStartup();

			/**
			*  @brief
			*    Return whether or not the screenshot manager is up-and-running
			*
			*  @return
			*    "true" if the screenshot manager is up-and-running ("onStartup()" was called successfully), else "false"
			*/
			inline bool isRunning() const;

			/**
			*  @brief
			*    Shutdown the screenshot manager
			*
			*  @note
			*    - This method is only called case "onStartup()" returned successfully
			*/
			void onShutdown();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			RendererSystem&							mRendererSystem;	///< The owner renderer system, always valid, do not destroy the instance
			bool									mRunning;			///< "true" if the screenshot manager is up-and-running ("onStartup()" was called successfully), else "false"
			std::string								mFileExtension;
			uint32									mGridSize;
			uint32									mWidth;
			uint32									mHeight;
			bool									mHdrScreenshotCapturing;
			bool									mDisableOverlays;
			bool									mPreviousToneMappingSetting;
			Ogre::TexturePtr*						mOgreTexturePtr;	//< Temporary texture with current screen size
			bool									mIsExternalTexture;
			Ogre::RenderTexture*					mOgreRenderTexture;
			Ogre::v1::HardwarePixelBufferSharedPtr*	mOgreHardwarePixelBufferSharedPtr;
			Ogre::PixelBox*							mFinalOgrePixelBox;	///< Pixel box for a large screenshot, if grid size is > 1
			uint8*									mData;				///< Pointer to the color data of the pixel box
			CameraComponent*						mCameraComponent;	///< Currently used camera component, can be a null pointer, do not destroy the instance
			GuiContext*								mGuiContext;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // compositing
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_compositing/compositor/ScreenshotManager-inl.h"
