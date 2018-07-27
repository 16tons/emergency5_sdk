// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/System.h"
#include "qsf/time/Time.h"

#include <boost/function.hpp>

#include <string>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class PlatformSystemImpl;
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
	*    Platform system class
	*
	*  @remarks
	*    The platform system provides platform dependent information and services.
	*
	*  @note
	*    - "qsf::PlatformSystem::onStartup()" and "qsf::PlatformSystem::onShutdown()" are unable to write out log messages because the log system depends on the platform system
	*    - The platform system and related classes are based on codes from the open-source engine PixelLight (MIT license, www.pixellight.org), for Linux, Android and Mac OS X backends see PixelLight
	*/
	class QSF_API_EXPORT PlatformSystem : public System
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Detect the current machine's endian ("byte order")
		*
		*  @return
		*    "true" if the current machine is using "Little-endian" byte order, (LSB (least significant byte) stored first, also known as "Intel-Format")
		*    "false" if the current machine is using "Big-endian" byte order, (MSB (most significant byte) stored first, also known as "Motorola-Format")
		*
		*  @note
		*   - QSF is using "Little-endian" for its binary file formats and so on
		*/
		static bool isLittleEndian();


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PlatformSystem();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~PlatformSystem();

		/**
		*  @brief
		*    Return the name of the platform
		*
		*  @return
		*    ASCII platform string (for instance "Windows" for Windows, "Linux" for Linux and so on)
		*/
		inline std::string getPlatform() const;

		/**
		*  @brief
		*    Return the platform architecture
		*
		*  @return
		*    ASCII platform architecture (for instance "x86", "x64", "armeabi", "armeabi-v7a" and so on)
		*/
		inline std::string getPlatformArchitecture() const;

		/**
		*  @brief
		*    Return the platform bit architecture
		*
		*  @return
		*    Platform bit architecture (for instance "32" for x86, "64" for x64)
		*/
		inline uint32 getPlatformBitArchitecture() const;

		/**
		*  @brief
		*    Return the name and version of the operating system
		*
		*  @return
		*    ASCII operation system information string (for instance "Windows 7 Service Pack 1 (Build 7601)")
		*/
		inline std::string getOs() const;

		/**
		*  @brief
		*    Return the operation system architecture
		*
		*  @return
		*    ASCII operation system architecture (for instance "x86", "x64", "armeabi", "armeabi-v7a" and so on)
		*/
		inline std::string getOsArchitecture() const;

		/**
		*  @brief
		*    Return the operation system bit architecture
		*
		*  @return
		*    Operation system bit architecture (for instance "32" for x86, "64" for x64)
		*/
		inline uint32 getOsBitArchitecture() const;

		/**
		*  @brief
		*    Returns the directory separator used by the operation system
		*
		*  @return
		*    The directory separator used by the operation system (e.g. "/" on Linux, "\" on Windows)
		*/
		inline char getPathSeparator() const;

		/**
		*  @brief
		*    Returns the shared library filename prefix used by the operation system
		*
		*  @return
		*    The shared library ASCII filename prefix used by the operation system (e.g. "lib" as in "libqsf.so" on Linux, no prefix as in "qsf.dll" on Windows)
		*/
		inline std::string getSharedLibraryPrefix() const;

		/**
		*  @brief
		*    Returns the shared library file extension used by the operation system
		*
		*  @return
		*    The shared library ASCII file extension used by the operation system (e.g. "so" on Linux, "dll" on Windows)
		*/
		inline std::string getSharedLibraryExtension() const;

		/**
		*  @brief
		*    Return the name of the computer
		*
		*  @return
		*    UTF-8 computer name
		*/
		inline std::string getComputerName() const;

		/**
		*  @brief
		*    Return the current user name
		*
		*  @return
		*    UTF-8 user name
		*/
		inline std::string getUserName() const;

		/**
		*  @brief
		*    Get home directory of the current user
		*
		*  @return
		*    The UTF-8 home directory of the current user (platform-independent notation)
		*
		*  @remarks
		*    Examples on different systems:
		*    - Windows 7:  C:/Users/<UserName>
		*    - Windows XP: C:/Documents and Settings/<Username>
		*    - Linux:      /home/<username>
		*/
		inline std::string getUserHomeDirectory() const;

		/**
		*  @brief
		*    Get data directory of the current user
		*
		*  @return
		*    The UTF-8 data directory of the current user (platform-independent notation)
		*
		*  @remarks
		*    Examples on different systems:
		*    - Windows 7:  C:/Users/<UserName>/AppData/Roaming
		*    - Windows XP: C:/Documents and Settings/<Username>/Application Data
		*    - Linux:      /home/<username>
		*/
		inline std::string getUserDataDirectory() const;

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
		inline std::string getDataDirectoryName(const std::string& name) const;

		/**
		*  @brief
		*    Get data directory of the application bundle
		*
		*  @return
		*    The UTF-8 data directory of the application bundle (platform-independent notation); can be empty in case there's no bundle
		*
		*  @remarks
		*    This is only relevant on platforms using bundles, like Mac OS X final bundled builds (which we don't necessarily use during development).
		*    In all other cases, the bundle data directory is an empty string.
		*/
		inline std::string getBundleDataDirectory() const;

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
		inline std::string getExecutableFilename() const;

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
		inline std::string getModuleFilenameByMemoryAddress(const void* memoryAddress) const;

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
		inline std::string getEnvironmentVariable(const std::string& name) const;

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
		inline bool setEnvironmentVariable(const std::string& name, const std::string& value) const;

		/**
		*  @brief
		*    Delete an environment variable
		*
		*  @param[in] name
		*    UTF-8 name of the environment variable to delete
		*/
		inline void deleteEnvironmentVariable(const std::string& name) const;

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
		inline bool execute(const std::string& command, const std::string& parameters, const std::string& workingDirectory = "") const;

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
		inline bool openUrlExternal(const std::string& url) const;

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
		inline std::string getLocaleLanguage() const;

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
		inline std::string getCurrentDirectory() const;

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
		inline bool setCurrentDirectory(const std::string& path);

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
		inline bool isMouseVisible() const;

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
		inline void setMouseVisible(bool visible) const;

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
		inline void enableMouseCursorTrap(int left, int top, int right, int bottom);

		/**
		*  @brief
		*    Disable mouse cursor trap
		*
		*  @see
		*    - "qsf::PlatformSystem::enableMouseCursorTrap()"
		*/
		inline void disableMouseCursorTrap();

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
		inline void urgentMessage(const std::string& message, const std::string& title = "Urgent Message") const;

		/**
		*  @brief
		*    Get the width and height (in pixel) of the current desktop resolution
		*
		*  @param[out] width
		*    Will receive the width of the current desktop resolution
		*  @param[out] height
		*    Will receive the height of the current desktop resolution
		*/
		inline void getDesktopWidthAndHeight(uint32& width, uint32& height) const;

		//[-------------------------------------------------------]
		//[ Time                                                  ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Retrieve the time since the system was started
		*
		*  @return
		*    The time elapsed since the system was started
		*/
		inline Time getSystemTime() const;

		/**
		*  @brief
		*    Suspend the current thread for a specified time period
		*
		*  @param[in] time
		*    The time to sleep, should not be 0 because the behavior is implementation dependent (use "yield()" instead)
		*/
		inline void sleep(const Time& sleepTime) const;

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
		inline void yield() const;

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
		inline float getPercentageOfUsedPhysicalMemory() const;

		/**
		*  @brief
		*    Return the total physical memory in bytes
		*
		*  @return
		*    The total physical memory in bytes
		*/
		inline uint64 getTotalPhysicalMemory() const;

		/**
		*  @brief
		*    Return the current free physical memory in bytes
		*
		*  @return
		*    The current free physical memory in bytes
		*/
		inline uint64 getFreePhysicalMemory() const;

		/**
		*  @brief
		*    Return the total virtual memory in bytes
		*
		*  @return
		*    The total virtual memory in bytes
		*/
		inline uint64 getTotalVirtualMemory() const;

		/**
		*  @brief
		*    Return the current free virtual memory in bytes
		*
		*  @return
		*    The current free virtual memory in bytes
		*/
		inline uint64 getFreeVirtualMemory() const;

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
		inline uint32 getCpuMhz() const;

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
		inline bool openFileDialog(std::string& outFilename, const std::string& initialDirectory, const std::string& filter) const;

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
		inline bool enableDragDrop(const boost::function<void(const std::string&)>& callback, handle nativeWindowHandle) const;

		/**
		*  @brief
		*    Disable drag and drop of files into the application
		*/
		inline void disableDragDrop() const;


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
		PlatformSystemImpl* mPlatformSystem;	///< Platform system implementation, always valid, we're responsible for destroying the instance in case we no longer need it


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/platform/PlatformSystem-inl.h"
