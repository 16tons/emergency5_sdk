// Copyright (C) 2012-2018 Promotion Software GmbH


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
	#ifdef QSF_EDITOR_EXPORTS
		// Export
		#define QSF_EDITOR_API_EXPORT		QSF_GENERIC_API_EXPORT
		#define QSF_EDITOR_FUNCTION_EXPORT	QSF_GENERIC_FUNCTION_EXPORT
		#define QSF_EDITOR_TEMPLATE_EXPORT
	#else
		// Import
		#define QSF_EDITOR_API_EXPORT		QSF_GENERIC_API_IMPORT
		#define QSF_EDITOR_FUNCTION_EXPORT
		#define QSF_EDITOR_TEMPLATE_EXPORT	extern
	#endif
#else
	// Build as static library
	#define QSF_EDITOR_API_EXPORT
	#define QSF_EDITOR_FUNCTION_EXPORT
	#define QSF_EDITOR_TEMPLATE_EXPORT
#endif
