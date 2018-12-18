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
#ifdef GAMEPLUGIN_EXPORTS
	// Export
	#define GAMEPLUGIN_API_EXPORT	   QSF_GENERIC_API_EXPORT
	#define GAMEPLUGIN_FUNCTION_EXPORT QSF_GENERIC_FUNCTION_EXPORT
	#define GAMEPLUGIN_TEMPLATE_EXPORT
#else
	// Import
	#define GAMEPLUGIN_API_EXPORT	   QSF_GENERIC_API_IMPORT
	#define GAMEPLUGIN_FUNCTION_EXPORT
	#define GAMEPLUGIN_TEMPLATE_EXPORT extern
#endif
