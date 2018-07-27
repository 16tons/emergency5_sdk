// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Preprocessor definitions                              ]
//[-------------------------------------------------------]

// Build configuration
#if defined RETAIL
	#define	BUILD_CONFIGURATION "Retail"
#elif defined DEBUG
	#define	BUILD_CONFIGURATION "Debug"
#else
	#define	BUILD_CONFIGURATION "Release"
#endif

// Build platform architecture
#if defined QSF_X64
	#define	BUILD_ARCHITECTURE "x64"
#else
	#define	BUILD_ARCHITECTURE "x86"
#endif
