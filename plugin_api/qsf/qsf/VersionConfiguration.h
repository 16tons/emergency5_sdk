// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Global memory definitions                             ]
//[-------------------------------------------------------]
// Different configurations excluding each other
// -> As soon as TBB is used, VLD does not work anymore
// -> As soon as TBB is used, HeapInspector does not make much sense anymore (because everything goes through TBB)
#define QSF_USE_TBB					// Intel Threading Building Blocks memory allocator
// #define QSF_USE_VLD				// Visual Leak Detector for Visual C++ 2008/2010/2012/2013/2015
// #define QSF_USE_HEAP_INSPECTOR	// Heap Inspector

// As soon as TBB is used, HeapInspector does not make much sense anymore (because everything goes through TBB)
#ifdef QSF_USE_HEAP_INSPECTOR
	#undef QSF_USE_TBB
	#undef QSF_USE_VLD
#endif

// VLD is only available on MS Windows
#if !defined(WIN32)
	#undef QSF_USE_VLD
#endif
#ifdef QSF_USE_VLD
	#define VLD_FORCE_ENABLE
#endif


//[-------------------------------------------------------]
//[ Global profiling definitions                          ]
//[-------------------------------------------------------]
/**
*  @brief
*    Setting to enable/disable QSF profiling options
*
*  @note
*    - When you	are deploying your application you will probably want disable profiling in order to avoid a performance impact due to profiling
*/
// #define QSF_PROFILING


//[-------------------------------------------------------]
//[ Global end user definitions                           ]
//[-------------------------------------------------------]
// #define ENDUSER	// When enabled, it disables features which are not yet usable for end user (e.g. features which are currently only usable internally)

// As soon ENDUSER is enabled disable any memory checker and enable TBB usage
#ifdef ENDUSER
	#define QSF_USE_TBB
	#undef QSF_USE_VLD
	#undef VLD_FORCE_ENABLE
	#undef QSF_USE_HEAP_INSPECTOR
#endif
