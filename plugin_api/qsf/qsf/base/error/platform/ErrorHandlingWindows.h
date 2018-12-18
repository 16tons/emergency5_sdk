// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


// Inclusion sanity check
#ifndef _MSC_VER
	#error "Don't include this header for any other platform than Windows."
#endif


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/error/ErrorHandling.h"

#include <boost/container/flat_set.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace detail
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		class ErrorHandlingWindows
		{


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		public:
			static void createMiniDump(const std::string& name);


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/** Default constructor */
			ErrorHandlingWindows();

			/** Show a message box if needed and manage selective ignoring of issues; returns "true" if a break is requested */
			bool showAssertBreakMessageBox(const std::string& message, ErrorHandling::SeverityLevel severityLevel, const char* file, uint32 line, bool showDebuggingOptions);

			/** Copy the shown message into the clipboard */
			void copyMessageToClipboard();

			/** Open the current log file */
			void openLogFile();


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			bool canShowVistaStyleMessageBox();
			int showVistaStyleMessageBox(const std::string& message, ErrorHandling::SeverityLevel severityLevel, const char* file, uint32 line, bool showDebuggingOptions);
			int showFallbackMessageBox(const std::string& message, ErrorHandling::SeverityLevel severityLevel, const char* file, uint32 line, bool showDebuggingOptions);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			boost::container::flat_set<uint32>	mIgnoredAssertHashes;
			bool								mLoadedProcPointers;
			void*								mTaskDialogIndirectProcPointer;
			std::wstring						mTaskDialogMessage;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // detail
} // qsf
