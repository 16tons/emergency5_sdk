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
#ifdef EM5_EDITOR_EXPORTS
	// Export
	#define EM5EDITOR_API_EXPORT		QSF_GENERIC_API_EXPORT
	#define EM5EDITOR_FUNCTION_EXPORT	QSF_GENERIC_FUNCTION_EXPORT
	#define EM5EDITOR_TEMPLATE_EXPORT
#else
	// Import
	#define EM5EDITOR_API_EXPORT		QSF_GENERIC_API_IMPORT
	#define EM5EDITOR_FUNCTION_EXPORT
	#define EM5EDITOR_TEMPLATE_EXPORT	extern
#endif
