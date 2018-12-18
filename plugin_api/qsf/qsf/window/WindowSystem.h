// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/System.h"

#include <boost/signals2.hpp>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Window;
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
	*    Window system class
	*
	*  @remarks
	*    The window system is about operation system windows.
	*/
	class QSF_API_EXPORT WindowSystem : public System
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class Window;


	//[-------------------------------------------------------]
	//[ Public Boost signals                                  ]
	//[-------------------------------------------------------]
	public:
		boost::signals2::signal<void (handle, bool, bool)> FocusChanged;	///< This Boost signal is emitted when the focus has changed; parameter: native window handle (null handle allowed) and exclusive window handling


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline WindowSystem();

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~WindowSystem();

		/**
		*  @brief
		*    Return the window list
		*
		*  @return
		*    Reference to the internal window list, do not manipulate the list or destroy the windows
		*/
		inline const std::vector<Window*>& getWindows() const;

		//[-------------------------------------------------------]
		//[ Focus                                                 ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the focus native window handle
		*
		*  @return
		*    The focus native window handle, can be a null handle, do no destroy the returned instance
		*/
		inline handle getFocusNativeWindowHandle() const;

		/**
		 *  @brief
		 *    Return "true" if the "focus native window handle" points to a window, "false" if it points to a control within the window
		 *
		 *  @return
		 *    "true" if the "focus native window handle" really is the handle of a window, provide "false" if it merely is a handle of a control inside a window
		 */
		inline bool isFocusNativeWindowHandleAWindow() const;

		/**
		*  @brief
		*    Set the focus native window handle
		*
		*  @param[in] nativeWindowHandle
		*    The focus native window handle, can be a null handle, the instance must stay valid as long as it's the focus native window handle
		*  @param[in] exclusive
		*    Exclusive window handling? (do this e.g. when your window is in fullscreen mode)
		*  @param isWindow
		*    Provide "true" if nativeWindowHandle really is the handle of a window, provide "false" if it merely is a handle of a control inside a window (ignored on Windows platform)
		*/
		void setFocusNativeWindowHandle(handle nativeWindowHandle, bool exclusive = false, bool isWindow = true);

		/**
		*  @brief
		*    Return the focus window
		*
		*  @return
		*    The focus window, can be a null pointer in case there's no focus window or the focus window is an external native window, do no destroy the returned instance
		*/
		Window* getFocusWindow() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::System methods                    ]
	//[-------------------------------------------------------]
	public:
		inline virtual const char* getName() const override;
		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual bool onStartup(bool serverMode) override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::vector<Window*> mWindows;						///< Window instances currently registered inside the window system, do not destroy the instances
		handle				 mNativeWindowHandle;			///< The focus native window handle, can be a null handle, do not destroy the instance
		bool				 mIsNativeWindowHandleAWindow;	///< "true" if the "focus native window handle" points to a window, "false" if it points to a control within the window


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/window/WindowSystem-inl.h"
