// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

#include <glm/glm.hpp>

#include <boost/noncopyable.hpp>


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
	*    Abstract operation system window base class
	*/
	class QSF_API_EXPORT Window : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Window();

		/**
		*  @brief
		*    Set the window title
		*
		*  @param[in] title
		*    Window title to set
		*/
		void setTitle(const std::string& title);

		/**
		*  @brief
		*    Enables the maximize button of the window
		*
		*  @param[in] enable
		*    Enable state of the maximize button
		*/
		void enableMaximize(bool enable);

		/**
		*  @brief
		*    Enables if the window is resizeable
		*
		*  @param[in] enable
		*    Enable state for the resize possibility of the window
		*/
		void enableResizeable(bool enable);

		/**
		*  @brief
		*    Enables or disables the window border
		*
		*  @param[in] enable
		*    Enable state for the resize possibility of the window
		*/
		void enableBorder(bool enable);

		/**
		*  @brief
		*    Center the window in the middle of the screen
		*/
		void center();

		/**
		*  @brief
		*    Returns if this window has the input focus
		*/
		bool hasFocus() const;

		//[-------------------------------------------------------]
		//[ Coordinate system                                     ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Convert window space coordinates to normalized window space coordinates
		*
		*  @param[in] windowSpacePosition
		*    Window space coordinates vector, i.e. expressed in pixels from the top left window corner
		*
		*  @return
		*    Normalized window space coordinates vector with x and y in [0.0, 1.0]
		*/
		glm::vec2 getNormalizedWindowSpaceCoords(const glm::vec2& windowSpacePosition) const;

		/**
		*  @brief
		*    Convert window space coordinates to normalized window space coordinates
		*
		*  @param[in] windowSpacePositionX
		*    Window space x-coordinate, expressed in pixels from the left window border
		*  @param[in] windowSpacePositionX
		*    Window space y-coordinate, expressed in pixels from the top window border
		*
		*  @return
		*    Normalized window space coordinates vector with x and y in [0.0, 1.0]
		*/
		glm::vec2 getNormalizedWindowSpaceCoords(int windowSpacePositionX, int windowSpacePositionY) const;

		/**
		*  @brief
		*    Convert normalized window space coordinates to window space coordinates
		*
		*  @param[in] normalizedWindowSpacePosition
		*    Normalized window space coordinates vector with x and y in [0.0, 1.0]
		*
		*  @return
		*    Window space coordinates vector, i.e. expressed in pixels from the top left window corner
		*/
		glm::vec2 getDenormalizedWindowSpaceCoords(const glm::vec2& normalizedWindowSpacePosition) const;

		/**
		*  @brief
		*    Convert window space coordinates to clip space coordinates
		*
		*  @param[in] windowSpacePosition
		*    Window space coordinates vector, i.e. expressed in pixels from the top left window corner
		*
		*  @return
		*    Clip space coordinates vector with x and y in [-1.0, 1.0]
		*/
		glm::vec2 convertWindowCoordsToClipSpace(const glm::vec2& windowSpacePosition) const;

		/**
		*  @brief
		*    Convert clip space coordinates to window space coordinates
		*
		*  @param[in] clipSpacePosition
		*    Clip space coordinates vector with x and y in [-1.0, 1.0]
		*
		*  @return
		*    Window space coordinates vector, i.e. expressed in pixels from the top left window corner
		*/
		glm::vec2 convertClipSpaceCoordsToWindowSpace(const glm::vec2& clipSpacePosition) const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Window methods                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return the OS dependent native window handle
		*
		*  @return
		*    The OS dependent native window handle, can be a null handle
		*/
		virtual handle getNativeWindowHandle() const;

		/**
		*  @brief
		*    Return the width of the window
		*
		*  @return
		*    The width of the window
		*/
		virtual uint32 getWidth() const = 0;

		/**
		*  @brief
		*    Return the height of the window
		*
		*  @return
		*    The height of the window
		*/
		virtual uint32 getHeight() const = 0;

		/**
		*  @brief
		*    Set new window position and size
		*
		*  @param[in] x
		*    X component of the new window position
		*  @param[in] y
		*    Y component of the new window position
		*  @param[in] width
		*    New window width
		*  @param[in] height
		*    New window height
		*  @param[in] maintainMonitorIndex
		*    Maintain monitor index? Setting this to true is handy in case the monitor index doesn't really interest and one is only adjusting position and size as
		*    if there's just a single monitor. If a user moved the window to another monitor and the application is changing position and size the window won't jump
		*    back to another monitor which usually is a good thing.
		*/
		virtual void setPositionSize(int x, int y, int width, int height, bool maintainMonitorIndex = true) = 0;

		/**
		*  @brief
		*    Hide/unhide the window
		*
		*  @param[in] hidden
		*    "true" if the window should be hidden, else "false"
		*/
		virtual void setHidden(bool hidden) = 0;

		/**
		*  @brief
		*    Request an explicit redraw of the window content
		*/
		virtual void redraw() = 0;

		/**
		*  @brief
		*    Return whether or not the window has been closed
		*
		*  @return
		*    "true" if the window has been closed, else "false"
		*/
		virtual bool isClosed() const = 0;

		/**
		*  @brief
		*    Return whether or not the window is active
		*
		*  @return
		*    "true" if the window is active, else "false"
		*/
		virtual bool isActive() const = 0;

		/**
		*  @brief
		*    Return whether or not the window is in fullscreen mode
		*
		*  @return
		*    "true" if the window is in fullscreen mode, else "false"
		*/
		virtual bool isFullscreen() const = 0;

		/**
		*  @brief
		*    Alter fullscreen mode options
		*
		*  @param[in] fullscreen
		*    Whether to use fullscreen mode or not
		*  @param[in] width
		*    The new width to use
		*  @param[in] height
		*    The new height to use
		*/
		virtual void setFullscreen(bool fullscreen, uint32 width, uint32 height) = 0;

		/**
		*  @brief
		*    Destroy the window
		*/
		virtual void destroy() = 0;

		/**
		*  @brief
		*    Switch the focus to this window and bring it to the foreground
		*
		*  @note
		*    - Don't overuse this method, it can be quite annoying to the user if windows steal the focus and pop to the foreground
		*/
		virtual void switchToThisWindow() = 0;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		Window();


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
