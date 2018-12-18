// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ C++ declarations and definitions                      ]
//[-------------------------------------------------------]
extern "C++"
{


	//[-------------------------------------------------------]
	//[ Includes                                              ]
	//[-------------------------------------------------------]
	#include "qsf/base/error/ErrorHandling.h"

	#include <boost/function.hpp>


	//[-------------------------------------------------------]
	//[ Namespace                                             ]
	//[-------------------------------------------------------]
	namespace qsf
	{
		namespace macosx
		{


			/**
			*  @brief
			*    Setup a minimal Mac OS X menu bar for the application
			*
			*  @param[in] applicationName
			*    Name of the application as it will appear in the menu bar
			*
			*  @note
			*    - Menu bar consists of apple menu and an application-specific menu with a single option "Quit <name> (Cmd-Q)"
			*    - Call this once before "performApplicationRunLoop"
			*/
			void setupMinimalMenuBar(const char* applicationName);

			/**
			*  @brief
			*    Perform the run loop regularly calling the given function until it returns "false"
			*
			*  @param[in] function
			*    The function to call in a loop as long as it returns "true"; on "false" the run loop will stop
			*
			*  @note
			*    - This internally starts the NSApplication run loop, which is necessary for some functionality like working Cmd-Q keyboard shortcut and menu bar
			*/
			void performApplicationRunLoop(const boost::function<bool(void)>& function);

			// Call this method during the application start if you don't want to have "Start Dictation..." and "Emoji & Symbols" GUI menu entries
			void DisabledDictationMenuItemDisabledCharacterPaletteMenuItem();

			/**
			 *  @brief
			 *    Show an alter popup (like MessageBox on Windows)
			 *
			 *  @param[in] title
			 *    Alert title
			 *  @param[in] msg
			 *    Alert text
			 *  @param[in] defaultButton
			 *    Alert button text (Should be a localized "OK")
			 */
			void MessageBox(const std::string& title, const std::string& msg, const std::string& defaultButton);

			/** Show a message box if needed and manage selective ignoring of issues; returns "true" if a break is requested */
			bool ShowAssertBreakMessageBox(const std::string& message, ErrorHandling::SeverityLevel severityLevel, const char* file, uint32 line, bool showDebuggingOptions);

			/**
			 *  @brief
			 *    Retrieves the desktop width and height
			 *
			 *  @param[out] width
			 *    The width of the desktop
			 *  @param[out] height
			 *    The height of the desktop
			 */
			void GetDesktopWidthAndHeight(uint32_t& width, uint32_t& height);

			/**
			 *  @brief
			 *    Shows the standard Mac OS X open file dialog
			 *
			 *  @param[out] outFilename
			 *    If the return value is "true", the path of the selected file, otherwise the value of outFilename is undefined
			 *  @param[in] initialDirectory
			 *    ignored
			 *  @param[in] filter
			 *    ignored
			 *
			 *  @return "true" if a file was selected, "false" otherwise
			 */
			bool ShowOpenFileDialog(std::string& outFilename, const std::string& initialDirectory, const std::string& filter);

			/**
			 *  @brief
			 *    Retrieves the window's inner frame ("client area") origin and size
			 *
			 *  @param[out] x
			 *    The x origin of the inner frame
			 *  @param[out] y
			 *    The y origin of the inner frame
			 *  @param[out] width
			 *    The width of the inner frame
			 *  @param[out] height
			 *    The height of the inner frame
			 *
			 *  @return "true" if successful, "false" otherwise
			 */
			bool GetWindowClientRect(handle window, float& x, float& y, float& width, float& height);

			/**
			 *  @brief
			 *    Retrieves the view's frame ("client area") origin and size
			 *
			 *  @param[out] x
			 *    The x origin of the inner frame
			 *  @param[out] y
			 *    The y origin of the inner frame
			 *  @param[out] width
			 *    The width of the inner frame
			 *  @param[out] height
			 *    The height of the inner frame
			 *
			 *  @return "true" if successful, "false" otherwise
			 */
			bool GetViewRect(handle view, float& x, float& y, float& width, float& height);

			/**
			 *  @brief
			 *    Retrieves the mouse position in window coordinates
			 *
			 *  @param[out] x
			 *    The x coordinate of the current mouse position
			 *  @param[out] y
			 *    The y coordinate of the current mouse position
			 *
			 *  @return "true" if successful, "false" otherwise
			 */
			bool GetMousePosRelativeToWindow(handle window, float& x, float& y);

			/**
			 *  @brief
			 *    Puts the given window on top of all non-modal windows
			 *
			 *  @param[in] window
			 *    The native window handle of the window to put into the foreground
			 *
			 *  @return "true" if successful, "false" otherwise
			 */
			bool SetForegroundWindow(handle window);

			/**
			 *  @brief
			 *    Puts the given window on above everything else (useful for e.g. a borderless fullscreen window)
			 *
			 *  @param[in] window
			 *    The native window handle of the window to put above everything else
			 *
			 *  @return "true" if successful, "false" otherwise
			 */
			bool SetWindowAboveEverythingElse(handle window, bool everythingElse);

			/**
			 *  @brief
			 *    Changes the title of a window
			 *
			 *  @param[in] window
			 *    The native window handle of the window
			 *  @param[in] title
			 *    The new title
			 *
			 *  @return "true" if successful, "false" otherwise
			 */
			bool SetWindowTitle(handle window, const std::string& title);

			/**
			 *  @brief
			 *    Enables or disables maximizing a window
			 *
			 *  @param[in] window
			 *    The native window handle of the window
			 *  @param[in] enable
			 *    "true" to enable the maximize title bar button, "false" to disable it
			 *
			 *  @return "true" if successful, "false" otherwise
			 */
			bool EnableWindowMaximize(handle window, bool enable);

			/**
			 *  @brief
			 *    Enables or disables resizing a window
			 *
			 *  @param[in] window
			 *    The native window handle of the window
			 *  @param[in] enable
			 *    "true" to enable resizing, "false" to disable it
			 *
			 *  @return "true" if successful, "false" otherwise
			 */
			bool EnableWindowResizing(handle window, bool enable);

			/**
			 *  @brief
			 *    Enables or disables the border of a window
			 *
			 *  @param[in] window
			 *    The native window handle of the window
			 *  @param[in] enable
			 *    "true" to enable the border, "false" to disable it
			 *
			 *  @return "true" if successful, "false" otherwise
			 */
			bool EnableWindowBorder(handle window, bool enable);

			/**
			 *  @brief
			 *    Resize window by defining its content size (i.e. without window decoration)
			 *
			 *  @param[in] window
			 *    The native window handle of the window
			 *  @param[in] width
			 *    Window width in pixels
			 *  @param[in] height
			 *    Window height in pixels
			 */
			void SetWindowContentSize(handle window, int width, int height);

			/**
			 *  @brief
			 *    Processes all currently queued application messages and calls the appropriate event handlers.
			 */
			void ProcessApplicationMessageQueue();

			/**
			 *  @brief
			 *    Get UTF-8 path to bundle resource directory
			 */
			std::string getBundleResourceDirectory();

			std::string getUserHomeDirectory();

			std::string getSystemLocale();

			bool openUrlExternal(const std::string& url);


	//[-------------------------------------------------------]
	//[ Namespace                                             ]
	//[-------------------------------------------------------]
		} // macosx
	} // qsf

}
