// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

#include <sstream>
#include <stdexcept> // for std::runtime_error


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	// Some aspects of our assert and check routines are influenced by:
	//  http://cnicholson.net/2009/02/stupid-c-tricks-adventures-in-assert/


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class QSF_API_EXPORT ErrorHandling
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		enum SeverityLevel
		{
			LEVEL_WARNING = 0,	///< Warning severity level
			LEVEL_ERROR = 1		///< Error severity level
		};


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/** Check whether or not a debugger is present */
		static bool isDebuggerPresent();

		/** Check whether or not the debugger should break in asserts and checks */
		static bool shouldBreakOnAssert(SeverityLevel severityLevel);

		/** Show a message box if needed and manage selective ignoring of issues; returns "true" if a break is requested */
		static bool handleBreakOnAssert(const std::string& message, SeverityLevel severityLevel, const char* file, uint32 line);

		/** Ignore breaks in asserts and checks of the given severity level */
		static void setBreakOnAssertForLevel(SeverityLevel severityLevel, bool enable);

		/** Print an error log message */
		static void printToLog(SeverityLevel severityLevel, const std::string& message);

		/** Show a message box to the user; returns "true" if a break is requested */
		static bool showMessageBox(const std::string& message, SeverityLevel severityLevel, const char* file, uint32 line);


	};


	//[-------------------------------------------------------]
	//[ Debug break macro                                     ]
	//[-------------------------------------------------------]
	// Debug break (platform specific)
#ifdef _MSC_VER
	#define QSF_DEBUG_BREAK	__debugbreak()
#elif __APPLE__
	#define QSF_DEBUG_BREAK __builtin_trap()
#else
	#define QSF_DEBUG_BREAK {}
#endif


	//[-------------------------------------------------------]
	//[ Internal error check macro                            ]
	//[-------------------------------------------------------]
	// Internal macro as base for QSF_CHECK etc.
	#define QSF_CONDITIONAL_ERROR(severity, condition, explanation, reaction) \
		{ \
			if (!(condition)) \
			{ \
				std::ostringstream stream; \
				stream << explanation; \
				qsf::ErrorHandling::printToLog(qsf::ErrorHandling::LEVEL_##severity, stream.str()); \
				if (qsf::ErrorHandling::handleBreakOnAssert(stream.str(), qsf::ErrorHandling::LEVEL_##severity, __FILE__, __LINE__)) \
					QSF_DEBUG_BREAK; \
				reaction; \
			} \
		}

	// Internal macro as base for QSF_CHECK_ONCE etc.
	#define QSF_CONDITIONAL_ERROR_ONCE(severity, condition, explanation, reaction) \
		{ \
			if (!(condition)) \
			{ \
				std::ostringstream stream; \
				stream << explanation; \
				static bool loggedOnce = false; \
				if (!loggedOnce) \
				{ \
					qsf::ErrorHandling::printToLog(qsf::ErrorHandling::LEVEL_##severity, stream.str()); \
					loggedOnce = true; \
				} \
				if (qsf::ErrorHandling::handleBreakOnAssert(stream.str(), qsf::ErrorHandling::LEVEL_##severity, __FILE__, __LINE__)) \
					QSF_DEBUG_BREAK; \
				reaction; \
			} \
		}


	//[-------------------------------------------------------]
	//[ Error check macros                                    ]
	//[-------------------------------------------------------]
	// Check some condition x and log the explanation, pause when debugging and react afterwards in case of failure
	#define QSF_CHECK(condition, explanation, reaction) QSF_CONDITIONAL_ERROR(ERROR, condition, explanation, reaction)

	// Check some condition x and log the explanation only the first time
	#define QSF_CHECK_ONCE(condition, explanation, reaction) QSF_CONDITIONAL_ERROR_ONCE(ERROR, condition, explanation, reaction)

	// Signal an error without a dedicated condition (for example the default section in a switch statement)
	#define QSF_ERROR(explanation, reaction) QSF_CONDITIONAL_ERROR(ERROR, false, explanation, reaction)
	#define QSF_ERROR_ONCE(explanation, reaction) QSF_CONDITIONAL_ERROR_ONCE(ERROR, false, explanation, reaction)

	// A check that should only be done during development.
	// These checks are ignored in the versions delivered to the customer
#ifndef RETAIL
	#define QSF_ASSERT(condition, explanation, reaction) QSF_CHECK(condition, explanation, reaction)
	#define QSF_ASSERT_ONCE(condition, explanation, reaction) QSF_CHECK_ONCE(condition, explanation, reaction)
#else
	#define QSF_ASSERT(condition, explanation, reaction) \
		{ \
			(void) sizeof(condition); \
		}
	#define QSF_ASSERT_ONCE(condition, explanation, reaction) \
		{ \
			(void) sizeof(condition); \
		}
#endif

	// Check some condition x and log a warning and pause during debugging in case of failure
	#define QSF_WARN_IF(condition, explanation, reaction) QSF_CONDITIONAL_ERROR(WARNING, !(condition), explanation, reaction)
	#define QSF_WARN_IF_ONCE(condition, explanation, reaction) QSF_CONDITIONAL_ERROR_ONCE(WARNING, !(condition), explanation, reaction)

	// Signal a warning without a dedicated condition (for example the default section in a switch statement)
	#define QSF_WARN(explanation, reaction) QSF_CONDITIONAL_ERROR(WARNING, false, explanation, reaction)
	#define QSF_WARN_ONCE(explanation, reaction) QSF_CONDITIONAL_ERROR_ONCE(WARNING, false, explanation, reaction)


	//[-------------------------------------------------------]
	//[ Deprecation macro                                     ]
	//[-------------------------------------------------------]
	// Deprecates code by logging a warning the first time it is used
#ifdef ENDUSER
	#define QSF_DEPRECATE(explanation);
#else
	#define QSF_DEPRECATE(explanation) \
		{ \
			static bool warned = false; \
			if (!warned) \
			{ \
				std::ostringstream stream; \
				stream << explanation; \
				qsf::ErrorHandling::printToLog(qsf::ErrorHandling::LEVEL_WARNING, stream.str()); \
				warned = true; \
			} \
		}
#endif


	//[-------------------------------------------------------]
	//[ Message box macro                                     ]
	//[-------------------------------------------------------]
	// Shows a message box to the user, even in case the debugger is not attached
	#define QSF_MESSAGEBOX(explanation) \
		{ \
			std::ostringstream stream; \
			stream << explanation; \
			qsf::ErrorHandling::printToLog(qsf::ErrorHandling::LEVEL_ERROR, stream.str()); \
			if (qsf::ErrorHandling::showMessageBox(stream.str(), qsf::ErrorHandling::LEVEL_WARNING, __FILE__, __LINE__)) \
				QSF_DEBUG_BREAK; \
		}


	//[-------------------------------------------------------]
	//[ Error reaction shortcut macros                        ]
	//[-------------------------------------------------------]
	// Shortcut to react to an error by doing nothing
	#define QSF_REACT_NONE

	// Shortcut to react to an error by throwing an exception which contains the current logged message as text
	// Don't use non standard extension of std::exception with an const char* constructor
	#define QSF_REACT_THROW throw std::runtime_error(stream.str());


	//[-------------------------------------------------------]
	//[ Casts with sanity check                               ]
	//[-------------------------------------------------------]
	namespace detail
	{
		template<typename T>
		T& getReferenceSafe(T* x, const char* text)
		{
			QSF_CHECK(nullptr != x, text, QSF_REACT_THROW);
			return *x;
		}

		template<typename T>
		const T& getReferenceSafe(const T* x, const char* text)
		{
			QSF_CHECK(nullptr != x, text, QSF_REACT_THROW);
			return *x;
		}
	}

	// Use this macro for pointers x to get a reference; makes a QSF_CHECK and throws an exception on null pointers
	#define QSF_MAKE_REF(x) \
		qsf::detail::getReferenceSafe(x, "QSF_MAKE_REF(" #x ") failed")


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
