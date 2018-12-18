// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/platform/PlatformTypes.h"


//[-------------------------------------------------------]
//[ Export/import                                         ]
//[-------------------------------------------------------]
// Is this project built as shared library?
#ifdef _USRDLL
	// Build as shared library
	#ifdef QSF_EXPORTS
		// Export
		#define QSF_API_EXPORT		QSF_GENERIC_API_EXPORT
		#define QSF_FUNCTION_EXPORT	QSF_GENERIC_FUNCTION_EXPORT
		// The C++11 feature "extern template" (C++11, see e.g. http://www.stroustrup.com/C++11FAQ.html#extern-templates) can only be used on modern compilers like GCC 4.6
		#define QSF_TEMPLATE_EXPORT
	#else
		// Import
		#define QSF_API_EXPORT		QSF_GENERIC_API_IMPORT
		#define QSF_FUNCTION_EXPORT
		// The C++11 feature "extern template" (C++11, see e.g. http://www.stroustrup.com/C++11FAQ.html#extern-templates) can only be used on modern compilers like GCC 4.6
		#define QSF_TEMPLATE_EXPORT	extern
	#endif
#else
	// Build as static library
	#define QSF_API_EXPORT
	#define QSF_FUNCTION_EXPORT
	// The C++11 feature "extern template" (C++11, see e.g. http://www.stroustrup.com/C++11FAQ.html#extern-templates) can only be used on modern compilers like GCC 4.6
	#define QSF_TEMPLATE_EXPORT
#endif
