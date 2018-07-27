// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/platform/PlatformTypes.h>


//[-------------------------------------------------------]
//[ Export/import                                         ]
//[-------------------------------------------------------]
#ifdef _USRDLL
	// Build as shared library
	#ifdef QSF_LOGIC_EXPORTS
		// Export
		#define QSF_LOGIC_API_EXPORT		QSF_GENERIC_API_EXPORT
		#define QSF_LOGIC_FUNCTION_EXPORT	QSF_GENERIC_FUNCTION_EXPORT
		#define QSF_LOGIC_TEMPLATE_EXPORT
	#else
		// Import
		#define QSF_LOGIC_API_EXPORT		QSF_GENERIC_API_IMPORT
		#define QSF_LOGIC_FUNCTION_EXPORT
		#define QSF_LOGIC_TEMPLATE_EXPORT	extern
	#endif
#else
	// Build as static library
	#define QSF_LOGIC_API_EXPORT
	#define QSF_LOGIC_FUNCTION_EXPORT
	#define QSF_LOGIC_TEMPLATE_EXPORT
#endif
