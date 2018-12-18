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
	#ifdef QSF_COMPOSITING_EXPORTS
		// Export
		#define QSF_COMPOSITING_API_EXPORT		QSF_GENERIC_API_EXPORT
		#define QSF_COMPOSITING_FUNCTION_EXPORT	QSF_GENERIC_FUNCTION_EXPORT
		#define QSF_COMPOSITING_TEMPLATE_EXPORT
	#else
		// Import
		#define QSF_COMPOSITING_API_EXPORT		QSF_GENERIC_API_IMPORT
		#define QSF_COMPOSITING_FUNCTION_EXPORT
		#define QSF_COMPOSITING_TEMPLATE_EXPORT	extern
	#endif
#else
	// Build as static library
	#define QSF_COMPOSITING_API_EXPORT
	#define QSF_COMPOSITING_FUNCTION_EXPORT
	#define QSF_COMPOSITING_TEMPLATE_EXPORT
#endif
