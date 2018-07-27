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
#ifdef EDITORPLUGIN_EXPORTS
	// Export
	#define EDITORPLUGIN_API_EXPORT		 QSF_GENERIC_API_EXPORT
	#define EDITORPLUGIN_FUNCTION_EXPORT QSF_GENERIC_FUNCTION_EXPORT
	#define EDITORPLUGIN_TEMPLATE_EXPORT
#else
	// Import
	#define EDITORPLUGIN_API_EXPORT		 QSF_GENERIC_API_IMPORT
	#define EDITORPLUGIN_FUNCTION_EXPORT
	#define EDITORPLUGIN_TEMPLATE_EXPORT extern
#endif
