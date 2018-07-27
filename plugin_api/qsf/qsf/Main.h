// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/platform/PlatformSystem.h"
#include "qsf/localization/SetBoostUtf8Locale.h"

#include <boost/nowide/convert.hpp>
#include <boost/program_options.hpp>

#include <string>
#include <vector>


//[-------------------------------------------------------]
//[ Platform independent program entry point              ]
//[-------------------------------------------------------]
/**
*  @brief
*    Program entry point
*
*  @param[in] programOptions
*    List of UTF-8 program options, the first entry is always the absolute executable filename
*
*  @return
*    Exit code of the application (usually 0 means no error)
*/
int ProgramEntryPoint(const std::vector<std::string>& programOptions);


//[-------------------------------------------------------]
//[ Initialization functions                              ]
//[-------------------------------------------------------]
namespace detail
{

	void fixOpenFilesLimit()
	{
		// OGRE terrain alone tends to acquire 256 simultaneous file handles, other parts of QSF also try to occasionally cache open file
		// handles which might make us hit the default limits on Windows (512) or Mac OS X (256) so let's increase it to the respective hard limits for now...

		#ifdef WIN32
		{
			const int limitBefore = _getmaxstdio();

			if (limitBefore < 2048)
			{
				_setmaxstdio(2048);
			}

			const int limitAfter = _getmaxstdio();

			if (limitBefore != limitAfter)
			{
				printf("Increased the maximum number of simultaneously open files from %d to %d", limitBefore, limitAfter);
			}
			else
			{
				printf("Maximum number of simultaneously open files: %d", limitAfter);
			}
		}
		#elif __APPLE__
		{
			// Disabled this code, as it probably (but not verified) causes problems with sandboxing
	/*
			rlimit limitBefore, limitAfter;
			if (getrlimit(RLIMIT_NOFILE, &limitBefore) == 0)
			{
				limitAfter = limitBefore;
				limitAfter.rlim_cur = std::min<rlim_t>(OPEN_MAX, limitAfter.rlim_max);

				if (setrlimit(RLIMIT_NOFILE, &limitAfter) != 0)
				{
					printf("Failed to increase the maximum number of simultaneously open files");
				}

				if (getrlimit(RLIMIT_NOFILE, &limitAfter) == 0)
				{
					if (limitBefore.rlim_cur != limitAfter.rlim_cur)
					{
						printf("Increased the maximum number of simultaneously open files from %d to %d", static_cast<int>(limitBefore.rlim_cur), static_cast<int>(limitAfter.rlim_cur));
					}
					else
					{
						printf("Maximum number of simultaneously open files: %d", static_cast<int>(limitAfter.rlim_cur));
					}
				}
			}
	*/
		}
		#endif
	}

	void adjustGlobalSettings()
	{
		qsf::SetBoostUtf8Locale();
		fixOpenFilesLimit();
	}

}


//[-------------------------------------------------------]
//[ Platform dependent program entry point                ]
//[-------------------------------------------------------]
// Windows implementation
#ifdef WIN32
	#ifdef _CONSOLE
		#ifdef UNICODE
			int wmain(int argc, wchar_t** argv)
		#else
			int main(int argc, char** argv)
		#endif
			{
				detail::adjustGlobalSettings();

				// Best practice: Exception handling inside the program entry point
				try
				{
					std::vector<std::string> programOptions;
					programOptions.reserve(argc);
					for (int i = 0; i < argc; ++i)
					{
					#ifdef UNICODE
						std::wstring wstr = argv[i];
						programOptions.push_back(boost::nowide::narrow(wstr));
					#else
						programOptions.push_back(argv[i]);
					#endif
					}

					// Call the platform independent program entry point
					return ProgramEntryPoint(programOptions);
				}
				catch (const std::exception& e)
				{
					qsf::PlatformSystem().urgentMessage(std::string("Exception: ") + e.what());

					// Error!
					return -1;
				}
			}
	#else
		#include "qsf/platform/WindowsHeader.h"
		#ifdef UNICODE
			int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR commandLine, int)
			{
				detail::adjustGlobalSettings();

				std::string programOptions;

				{ // The first entry is always the absolute executable filename
				  // TODO(co) ASCII absolute executable filename, will cause issues when using UNICODE paths!
					char moduleFilename[MAX_PATH];
					if (GetModuleFileNameA(nullptr, moduleFilename, MAX_PATH))
					{
						programOptions.append(moduleFilename);
						programOptions.append(" ");
					}
					else
					{
						programOptions.append("Unknown absolute executable filename ");
					}
				}

				// Add the program options as ASCII to be compatible to the other program entry point versions
				programOptions.append(boost::nowide::narrow(commandLine));

				// Best practice: Exception handling inside the program entry point
				try
				{
					// Call the platform independent program entry point
					return ProgramEntryPoint(boost::program_options::split_winmain(programOptions));
				}
				catch (const std::exception& e)
				{
					qsf::PlatformSystem().urgentMessage(std::string("Exception: ") + e.what());

					// Error!
					return -1;
				}
			}
		#else
			int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR commandLine, int)
			{
				detail::adjustGlobalSettings();

				std::string programOptions;

				{ // The first entry is always the absolute executable filename
					char moduleFilename[MAX_PATH];
					if (GetModuleFileNameA(nullptr, moduleFilename, MAX_PATH))
					{
						programOptions.append(moduleFilename);
						programOptions.append(" ");
					}
					else
					{
						programOptions.append("Unknown absolute executable filename ");
					}
				}

				// Add the program options
				programOptions.append(commandLine);

				// Best practice: Exception handling inside the program entry point
				try
				{
					// Call the platform independent program entry point
					return ProgramEntryPoint(boost::program_options::split_winmain(programOptions));
				}
				catch (const std::exception& e)
				{
					qsf::PlatformSystem().urgentMessage(std::string("Exception: ") + e.what());

					// Error!
					return -1;
				}
			}
		#endif
	#endif


// Mac OS X implementation
#elif __APPLE__
int main(int argc, char** argv)
{
	detail::adjustGlobalSettings();

	// Best practice: Exception handling inside the program entry point
	try
	{
		std::vector<std::string> programOptions;
		programOptions.reserve(argc);
		for (int i = 0; i < argc; ++i)
		{
			programOptions.push_back(argv[i]);
		}

		// Call the platform independent program entry point
		return ProgramEntryPoint(programOptions);
	}
	catch (const std::exception& e)
	{
		qsf::PlatformSystem().urgentMessage(std::string("Exception: ") + e.what());

		// Error!
		return -1;
	}
}

// Linux implementation
#elif LINUX
	int main(int argc, char** argv)
	{
		detail::adjustGlobalSettings();

		// Best practice: Exception handling inside the program entry point
		try
		{
			std::vector<std::string> programOptions;
			programOptions.reserve(argc);
			for (int i = 0; i < argc; ++i)
			{
				programOptions.push_back(argv[i]);
			}

			// Call the platform independent program entry point
			return ProgramEntryPoint(programOptions);
		}
		catch (const std::exception& e)
		{
			qsf::PlatformSystem().urgentMessage(std::string("Exception: ") + e.what());

			// Error!
			return -1;
		}
	}
#endif
