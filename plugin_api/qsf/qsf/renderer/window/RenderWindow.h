// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/window/Window.h"

#include <boost/signals2.hpp>

#include <string>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class Camera;
	class RenderWindow;
}
namespace qsf
{
	class CompositorWorkspaceConfiguratorManager;
	class OgreWindowEventListener;
	class CameraComponent;
	class ParameterGroup;
	class DebugOverlay;
	class GuiContext;
}


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
	*    Render window class, encapsulates "Ogre::RenderWindow"
	*/
	class QSF_API_EXPORT RenderWindow : public Window
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class CameraComponent;
		friend class OgreWindowEventListener;


	//[-------------------------------------------------------]
	//[ Public Boost signals                                  ]
	//[-------------------------------------------------------]
	public:
		boost::signals2::signal<void (const CameraComponent*)> CameraComponentChanged;	///< This Boost signal is emitted when the render window camera component has changed; parameter: new camera component (null pointer is allowed)
		boost::signals2::signal<void (const GuiContext*)>	   GuiContextChanged;		///< This Boost signal is emitted when the render window GUI context component has changed; parameter: new GUI context (null pointer is allowed)


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*
		*  @note
		*    - Creates hidden window
		*/
		RenderWindow();

		/**
		*  @brief
		*    Constructor
		*
		*  @param title
		*    UTF-8 title of the window
		*  @param fullscreen
		*    Should the window be in fullscreen mode?
		*  @param width
		*    Width of the window
		*  @param height
		*    Height of the window
		*
		*  @note
		*    - Creates a new window and makes it visible at once
		*/
		RenderWindow(const std::string& title, bool fullscreen, uint32 width, uint32 height);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] nativeWindowHandle
		*    OS dependent native window handle to use
		*
		*  @note
		*    - It reuses the given window as render window and query the window for its hight/width
		*    - Only real window handles may be used with this constructor. Providing a handle to a
		*      control/view within a window will lead to undefined behaviour (and likely crashes on non-windows platforms)
		*/
		explicit RenderWindow(handle nativeWindowHandle);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] nativeWindowHandle
		*    OS dependent native window handle to use
		*  @param width
		*    Width of the window
		*  @param height
		*    Height of the window
		*  @param isWindow
		*    Provide "true" if nativeWindowHandle really is the handle of a window, provide "false" if it merely is a handle of a control inside a window (ignored on Windows platform)
		*
		*  @note
		*    - It reuses the given window
		*/
		RenderWindow(handle nativeWindowHandle, uint32 width, uint32 height, bool isWindow = true);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~RenderWindow();

		/**
		*  @brief
		*    Return whether or not the debug overlay is enabled
		*
		*  @return
		*    "true" if the debug overlay is enabled, else "false"
		*/
		inline bool isDebugOverlayEnabled() const;

		/**
		*  @brief
		*    Enable/disable the debug overlay
		*
		*  @param[in] enabled
		*    "true" if the debug overlay should be enabled, else "false"
		*/
		void setDebugOverlayEnabled(bool enabled);

		/**
		*  @brief
		*    Return whether or not modifying the system mouse cursor visibility on fullscreen toggle is enabled
		*
		*  @return
		*    "true" if the modifying the system mouse cursor visibility on fullscreen toggle is enabled, else "false"
		*/
		inline bool getModifySystemCursorVisibilityOnFullscreenToggle() const;

		/**
		*  @brief
		*    Enable/disable that on fullscreen toggle the visibility of the system mouse cursor is modified
		*
		*  @param[in] enabled
		*    "true" if the modifying the system mouse cursor visibility on fullscreen toggle be enabled, else "false"
		*/
		inline void setModifySystemCursorVisibilityOnFullscreenToggle(bool enabled);

		//[-------------------------------------------------------]
		//[ Camera component                                      ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the used camera component instance
		*
		*  @return
		*    The used camera component instance, can be a null pointer, do no destroy the returned instance
		*/
		inline CameraComponent* getCameraComponent() const;

		/**
		*  @brief
		*    Set the used camera component instance
		*
		*  @param[in] cameraComponent
		*    The used camera component instance, null pointer to set no camera
		*/
		void setCameraComponent(CameraComponent* cameraComponent);

		//[-------------------------------------------------------]
		//[ GUI context                                           ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the used GUI context instance
		*
		*  @return
		*    The used GUI context instance, can be a null pointer, do no destroy the returned instance
		*/
		inline GuiContext* getGuiContext() const;

		/**
		*  @brief
		*    Set the used GUI context instance
		*
		*  @param[in] guiContext
		*    The used GUI context instance, null pointer to set no GUI context
		*/
		void setGuiContext(GuiContext* guiContext);

		//[-------------------------------------------------------]
		//[ Internal                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the encapsulated OGRE render window instance
		*
		*  @return
		*    The encapsulated OGRE render window instance, can be a null pointer, do no destroy the returned instance
		*
		*  @note
		*    - Don't access the encapsulated OGRE render window instance if you don't have to
		*/
		inline Ogre::RenderWindow* getOgreRenderWindow() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Window methods                    ]
	//[-------------------------------------------------------]
	public:
		virtual handle getNativeWindowHandle() const override;
		virtual uint32 getWidth() const override;
		virtual uint32 getHeight() const override;
		virtual void setPositionSize(int x, int y, int width, int height, bool maintainMonitorIndex = true) override;
		virtual void setHidden(bool hidden) override;
		virtual void redraw() override;
		virtual bool isClosed() const override;
		virtual bool isActive() const override;
		virtual bool isFullscreen() const override;
		virtual void setFullscreen(bool fullscreen, uint32 width, uint32 height) override;
		virtual void destroy() override;
		virtual void switchToThisWindow() override;


	//[-------------------------------------------------------]
	//[ Private static data                                   ]
	//[-------------------------------------------------------]
	private:
		static int instanceCounter;	///< Internal render window instance counter


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Set the encapsulated camera component OGRE camera instance
		*
		*  @param[in] ogreCamera
		*    The encapsulated OGRE camera instance, can be a null pointer
		*/
		void setCameraComponentOgreCamera(Ogre::Camera* ogreCamera);

		/**
		*  @brief
		*    Update the aspect ratio settings of the used OGRE camera
		*/
		void updateOgreCameraAspectRatio() const;

		/**
		*  @brief
		*    Set the focus native window handle the input system receives the input from
		*
		*  @param[in] nativeWindowHandle
		*    The focus native window handle the input system receives the input from, can be a null pointer (input system is disabled),
		*    the instance must stay valid as long as it's the focus native window handle of the input system
		*  @param[in] exclusive
		*    Exclusive window handling? (do this e.g. when your window is in fullscreen mode)
		*
		*  @note
		*    - Sadly, OIS is not window independent so we need to handle this because we need to be able to use multiple windows
		*    - In case the focus native window handle gets destroyed, the input system is informed about this and updated automatically
		*    - OIS only receives input events in case the OS window the focus native window handle represents has the OS focus
		*    - In a single window application, this method is usually only called once during the application initialization
		*/
		void setFocusNativeWindowHandle(handle nativeWindowHandle, bool exclusive = false, bool isWindow = true);

		/**
		*  @brief
		*    Check the given width and height against the desktop dimension
		*
		*  @param[in, out] exclusiveFullsceen
		*    Use exclusive fullscreen mode? The method is allowed to change this in order to avoid harm.
		*  @param[in, out] width
		*    Width to clamp, receives the resulting value
		*  @param[in, out] height
		*    Height to clamp, receives the resulting value
		*
		*  @note
		*    - Except for exclusive fullscreen mode, ensure the render window is never larger as the desktop resolution, this might cause some unhandy situations
		*/
		void checkAgainstDesktopWidthAndHeight(bool& exclusiveFullsceen, uint32& width, uint32& height);

		/**
		*  @brief
		*    Finalize OGRE render window
		*
		*  @param[in] fullscreen
		*    Use fullscreen mode?
		*  @param[in] width
		*    Window width
		*  @param[in] height
		*    Window height
		*/
		void finalizeOgreRenderWindow(bool fullscreen, uint32 width, uint32 height);

		/**
		*  @brief
		*    Called when the render window size has been changed
		*
		*  @param[in] width
		*    The new render window width
		*  @param[in] height
		*    The new render window height
		*/
		void onSizeChanged(int width, int height);

		/**
		*  @brief
		*    Called as soon as there's a property change inside the settings group manager
		*
		*  @param[in] parameterGroup
		*    Parameter group instance the changed property is in
		*  @param[in] propertyId
		*    Unique class property ID the component property which values was changed
		*/
		void onSettingsGroupPropertyChanged(const ParameterGroup& parameterGroup, uint32 propertyId);

		/**
		*  @brief
		*    Update debug overlay state
		*/
		void updateDebugOverlayState();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Ogre::RenderWindow*		 mOgreRenderWindow;				///< OGRE render window instance, can be a null pointer, we're responsible for destroying the instance in case we no longer need it
		OgreWindowEventListener* mOgreWindowEventListener;		///< OGRE window event listener, can be a null pointer, we're responsible for destroying the instance in case we no longer need it
		bool					 mFullscreen;					///< Is the render window considered to be in fullscreen mode?
		CameraComponent*		 mCameraComponent;				///< Used camera component instance, can be a null pointer, do not destroy the instance
		GuiContext*				 mGuiContext;					///< Used GUI context instance, can be a null pointer, do not destroy the instance
		bool					 mDebugOverlayEnabled;
		DebugOverlay*			 mDebugOverlay;					///< Debug overlay instance, can be a null pointer, we're responsible for destroying the instance in case we no longer need it
		bool					 mModifySystemCursorVisibility;	///< Indicates if the system mouse cursor visibility should be modified when fullscreen is toggled
		bool					 mIsExternalWindow;

		CompositorWorkspaceConfiguratorManager& mCompositorWorkspaceConfiguratorManager;
		boost::signals2::connection				mFocusChangedConnection;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/window/RenderWindow-inl.h"
