// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/time/Time.h"

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>

#include <string>


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
	*    Abstract platform system interface
	*/
	class QSF_API_EXPORT PlatformSystemImpl : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline PlatformSystemImpl();

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~PlatformSystemImpl();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::PlatformSystemImpl methods        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return the name of the platform
		*
		*  @return
		*    ASCII platform string (for instance "Windows" for Windows, "Linux" for Linux and so on)
		*/
		virtual std::string getPlatform() const = 0;

		/**
		*  @brief
		*    Return the name and version of the operating system
		*
		*  @return
		*    ASCII operation system information string (for instance "Windows 7 Service Pack 1 (Build 7601)")
		*/
		virtual std::string getOs() const = 0;

		/**
		*  @brief
		*    Return the operation system architecture
		*
		*  @return
		*    ASCII operation system architecture (for instance "x86", "x64", "armeabi", "armeabi-v7a" and so on)
		*/
		virtual std::string getOsArchitecture() const = 0;

		/**
		*  @brief
		*    Return the operation system bit architecture
		*
		*  @return
		*    Operation system bit architecture (for instance "32" for x86, "64" for x64)
		*/
		virtual uint32 getOsBitArchitecture() const = 0;

		/**
		*  @brief
		*    Returns the directory separator used by the operation system
		*
		*  @return
		*    The directory separator used by the operation system (e.g. '/' on Linux, '\' on Windows)
		*/
		virtual char getPathSeparator() const = 0;

		/**
		*  @brief
		*    Returns the shared library filename prefix used by the operation system
		*
		*  @return
		*    The shared library ASCII filename prefix used by the operation system (e.g. 'lib' as in 'libqsf.so' on Linux, no prefix as in 'qsf.dll' on Windows)
		*/
		virtual std::string getSharedLibraryPrefix() const = 0;

		/**
		*  @brief
		*    Returns the shared library file extension used by the operation system
		*
		*  @return
		*    The shared library ASCII file extension used by the operation system (e.g. 'so' on Linux, 'dll' on Windows)
		*/
		virtual std::string getSharedLibraryExtension() const = 0;

		/**
		*  @brief
		*    Return the name of the computer
		*
		*  @return
		*    UTF-8 computer name
		*/
		virtual std::string getComputerName() const = 0;

		/**
		*  @brief
		*    Return the current user name
		*
		*  @return
		*    UTF-8 user name
		*/
		virtual std::string getUserName() const = 0;

		/**
		*  @brief
		*    Get home directory of the current user
		*
		*  @return
		*    The UTF-8 home directory of the current user in platform-independent notation
		*
		*  @remarks
		*    Examples on different systems:
		*    - Windows 7:  C:/Users/<UserName>
		*    - Windows XP: C:/Documents and Settings/<Username>
		*    - Linux:      /home/<username>
		*/
		virtual std::string getUserHomeDirectory() const = 0;

		/**
		*  @brief
		*    Get data directory of the current user
		*
		*  @return
		*    The UTF-8 data directory of the current user in platform-independent notation
		*
		*  @remarks
		*    Examples on different systems:
		*    - Windows 7:  C:/Users/<UserName>/AppData/Roaming
		*    - Windows XP: C:/Documents and Settings/<Username>/Application Data
		*    - Linux:      /home/<username>
		*/
		virtual std::string getUserDataDirectory() const = 0;

		/**
		*  @brief
		*    Get name of data directory for given application name
		*
		*  @param[in] name
		*    Application name
		*
		*  @return
		*    Name of data directory in native style of used OS
		*
		*  @remarks
		*    Examples on different systems:
		*    - Windows: getDataDirectoryName("QSF") -> "QSF"
		*    - Linux:   getDataDirectoryName("QSF") -> ".qsf"
		*/
		virtual std::string getDataDirectoryName(const std::string& name) const = 0;

		/**
		*  @see "qsf::PlatformSystem::getBundleDataDirectory"
		*/
		virtual std::string getBundleDataDirectory() const = 0;

		/**
		*  @brief
		*    Get absolute path of application executable
		*
		*  @return
		*    UTF-8 path in platform-independent notation to executable (native path style, e.g. on Windows: "C:/MyApplication/Test.exe")
		*
		*  @note
		*    - Application executable = currently running process
		*/
		virtual std::string getExecutableFilename() const = 0;

		/**
		*  @brief
		*    Return the absolute filename of the shared library or executable a given memory address is located in
		*
		*  @param[in] memoryAddress
		*    Memory address ("memory anchor") inside the shared library or executable from which the absolute filename
		*    should be returned from, can be a null pointer in which case the result will be an empty string
		*
		*  @return
		*    Absolute UTF-8 filename in platform-independent notation of the shared library or executable the given memory address is located in (platform-independent notation),
		*    or an empty string in case it was impossible to determine the filename
		*
		*  @note
		*    - Result example on Windows: "C:/MyApplication/qsf.dll" or "C:/MyApplication/Test.exe"
		*/
		virtual std::string getModuleFilenameByMemoryAddress(const void* memoryAddress) const = 0;

		/**
		*  @brief
		*    Read an environment variable
		*
		*  @param[in] name
		*    UTF-8 name of the environment variable to read
		*
		*  @return
		*    Content of the variable
		*/
		virtual std::string getEnvironmentVariable(const std::string& name) const = 0;

		/**
		*  @brief
		*    Write an environment variable
		*
		*  @param[in] name
		*    UTF-8 name of the environment variable to write
		*  @param[in] value
		*    UTF-8 value to write into the variable
		*
		*  @return
		*    "true" if the variable has been set successfully, else "false"
		*/
		virtual bool setEnvironmentVariable(const std::string& name, const std::string& value) const = 0;

		/**
		*  @brief
		*    Delete an environment variable
		*
		*  @param[in] name
		*    UTF-8 name of the environment variable to delete
		*/
		virtual void deleteEnvironmentVariable(const std::string& name) const = 0;

		/**
		*  @brief
		*    Execute a system command (create a new process)
		*
		*  @param[in] command
		*    UTF-8 command to execute
		*  @param[in] parameters
		*    UTF-8 parameters to pass to the command
		*  @param[in] workingDirectory
		*    UTF-8 working directory in which to execute the command
		*
		*  @return
		*    "true" if the command has been executed successfully, else "false"
		*/
		virtual bool execute(const std::string& command, const std::string& parameters, const std::string& workingDirectory = "") const = 0;

		/**
		*  @brief
		*    Open an URL in external application (e.g. web browser)
		*
		*  @param[in] url
		*    UTF-8 URL to open
		*
		*  @return
		*    "true" if the URL has been opened successfully, else "false"
		*/
		virtual bool openUrlExternal(const std::string& url) const = 0;

		/**
		*  @brief
		*    Return the current program locale language information
		*
		*  @remarks
		*    Internally "setlocale" is used to collect the current program locale information - but
		*    only the "language" information is returned as string. For instance, if the current locale
		*    is "English_USA.1252", "English" is returned, if the locale is "French_France.1252", just
		*    "French" is returned and so on.
		*    This information can for instance be used to set a correct default language within the
		*    localization system of QSF.
		*
		*  @return
		*    The current UTF-8 program locale language information
		*/
		virtual std::string getLocaleLanguage() const = 0;

		/**
		*  @brief
		*    Return the current directory
		*
		*  @return
		*    UTF-8 path to the current directory as the OS provided it (platform-independent notation)
		*
		*  @note
		*    - In case of an internally empty string you will receive "." in order to make it possible to add e.g. "/Data" and still end up in a valid path
		*    - On Windows for example "C:/Programs/App"
		*/
		virtual std::string getCurrentDirectory() const = 0;

		/**
		*  @brief
		*    Set the current directory
		*
		*  @param[in] path
		*    UTF-8 path to the current directory
		*
		*  @return
		*    "true", if all went fine, else "false"
		*
		*  @note
		*    - Whenever possible, do not manipulate the current directory, this may backfire when you don't expect it
		*/
		virtual bool setCurrentDirectory(const std::string& path) = 0;

		//[-------------------------------------------------------]
		//[ GUI                                                   ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Check if the mouse cursor is visible
		*
		*  @return
		*    "true" if the mouse cursor is visible, else "false"
		*
		*  @note
		*    - If the mouse cursor is visible in general, it's still possible that it's
		*      invisible over some special widgets.
		*    - If the mouse cursor is invisible in general, it will never be visible!
		*    - Do only hide the mouse cursor when it really makes sense (e.g. during the period
		*      the mouse is used to control a "camera look around")
		*/
		virtual bool isMouseVisible() const = 0;

		/**
		*  @brief
		*    Set mouse cursor visibility
		*
		*  @param[in] visible
		*    Shall the mouse cursor be visible?
		*
		*  @see
		*    - "qsf::PlatformSystem::isMouseVisible()"
		*/
		virtual void setMouseVisible(bool visible) = 0;

		/**
		*  @brief
		*    Enable mouse cursor trap
		*
		*  @param[in] left
		*    Left border of the desktop area the mouse cursor will be trapped in
		*  @param[in] top
		*    Top border of the desktop area the mouse cursor will be trapped in
		*  @param[in] right
		*    Right border of the desktop area the mouse cursor will be trapped in
		*  @param[in] bottom
		*    Bottom border of the desktop area the mouse cursor will be trapped in
		*
		*  @note
		*    - Don't abuse the mouse cursor trap functionality or you might annoy the user
		*/
		virtual void enableMouseCursorTrap(int left, int top, int right, int bottom) = 0;

		/**
		*  @brief
		*    Disable mouse cursor trap
		*
		*  @see
		*    - "qsf::PlatformSystem::enableMouseCursorTrap()"
		*/
		virtual void disableMouseCursorTrap() = 0;

		/**
		*  @brief
		*    Primitive way (e.g. by using a message box) to be able to tell the user that something went terrible wrong
		*
		*  @param[in] message
		*    UTF-8 message to show
		*  @param[in] title
		*    UTF-8 title to show
		*
		*  @remarks
		*    Do not misuse this method in order to communicate with the user on a regular basis. This method does only
		*    exist to be able to tell the user that something went terrible wrong. There are situations were one can't
		*    use a log file, command line or something like this. Even when using e.g. a log file to write out error
		*    information - an application may e.g. just close directly after it's start without any further information
		*    and the user may even think that the application didn't start in the first place for an unknown reason.
		*    In such a situation, it's polite to inform the user that something went terrible wrong and providing
		*    a short hint how the issue may be solved. This method wasn't named "MessageBox()" by intend - because
		*    such a feature may not be available on the used platform or is handled in another way as a normal MS Windows
		*    message box.
		*/
		virtual void urgentMessage(const std::string& message, const std::string& title) const = 0;

		/**
		*  @brief
		*    Get the width and height (in pixel) of the current desktop resolution
		*
		*  @param[out] width
		*    Will receive the width of the current desktop resolution
		*  @param[out] height
		*    Will receive the height of the current desktop resolution
		*/
		virtual void getDesktopWidthAndHeight(uint32& width, uint32& height) const = 0;

		//[-------------------------------------------------------]
		//[ Time                                                  ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Retrieve the time elapsed since the system was started
		*
		*  @return
		*    The time elapsed since the system was started
		*/
		virtual Time getSystemTime() const = 0;

		/**
		*  @brief
		*    Suspend the current thread for a specified time period
		*
		*  @param[in] sleepTime
		*    The time to sleep, should not be 0 because the behavior is implementation dependent (use "yield()" instead)
		*/
		virtual void sleep(const Time& sleepTime) const = 0;

		/**
		*  @brief
		*    Yield the rest of the current threads time slice
		*
		*  @remarks
		*    Yields the rest of the threads time slice so another active thread of equal or higher priority
		*    waiting for processor time can run. Note that this function may return immediately and the behavior
		*    is in general not exactly predictable. So, use this function to give the processor just a hint
		*    that you are willed to give processor time away.
		*/
		virtual void yield() const = 0;

		//[-------------------------------------------------------]
		//[ Memory                                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return an approximation of the percentage of used physical memory (0.0-100.0)
		*
		*  @return
		*    An approximation of the percentage of used physical memory
		*/
		virtual float getPercentageOfUsedPhysicalMemory() const = 0;

		/**
		*  @brief
		*    Return the total physical memory in bytes
		*
		*  @return
		*    The total physical memory in bytes
		*/
		virtual uint64 getTotalPhysicalMemory() const = 0;

		/**
		*  @brief
		*    Return the current free physical memory in bytes
		*
		*  @return
		*    The current free physical memory in bytes
		*/
		virtual uint64 getFreePhysicalMemory() const = 0;

		/**
		*  @brief
		*    Return the total virtual memory in bytes
		*
		*  @return
		*    The total virtual memory in bytes
		*/
		virtual uint64 getTotalVirtualMemory() const = 0;

		/**
		*  @brief
		*    Return the current free virtual memory in bytes
		*
		*  @return
		*    The current free virtual memory in bytes
		*/
		virtual uint64 getFreeVirtualMemory() const = 0;

		//[-------------------------------------------------------]
		//[ CPU                                                   ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Get the CPU MHz of the primary CPU
		*
		*  @return
		*    CPU MHz
		*/
		virtual uint32 getCpuMhz() const = 0;

		//[-------------------------------------------------------]
		//[ Open file dialog                                      ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Show a modal dialog to select a file name
		*
		*  @param[out] outFilename
		*    Selected UTF-8 file name, only set in case the dialog returns with success
		*  @param[out] initialDirectory
		*    Directory to show at startup; may be empty for automatic selection
		*  @param[out] filter
		*    UTF-8 file filters as combined string with parts separated by '\n' character; apart from that parts see Windows API OPENFILENAME lpstrFilter
		*
		*  @return
		*    "true" if a file was selected, "false" if user aborted
		*/
		virtual bool openFileDialog(std::string& outFilename, const std::string& initialDirectory, const std::string& filter) const = 0;

		//[-------------------------------------------------------]
		//[ Drag and drop                                         ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Enable drag and drop of files into the application
		*
		*  @param[in] callback
		*    Function to call for each file dropped into the application; function parameter is the UTF-8 file name
		*  @param[in] nativeWindowHandle
		*    Native window handle of the application window
		*
		*  @return
		*    "true" if drag and drop could be enabled successfully, "false" on error
		*/
		virtual bool enableDragDrop(const boost::function<void(const std::string&)>& callback, handle nativeWindowHandle) = 0;

		/**
		*  @brief
		*    Disable drag and drop of files into the application
		*/
		virtual void disableDragDrop() = 0;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/platform/detail/PlatformSystemImpl-inl.h"
