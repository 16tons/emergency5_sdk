// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/VersionConfiguration.h"

// Global override of the memory allocation operators to use TBB scalable malloc to get a free performance boost
//  -> The include is only needed on Windows
#if defined(QSF_USE_TBB) && defined(WIN32)
	#include <tbb/tbbmalloc_proxy.h>
#endif


// Contains helper function for manually managing memory

namespace qsf
{

	// Delete and set to null pointer in one statement
	#define QSF_SAFE_DELETE(x) \
		do \
		{ \
			delete (x); \
			(x) = nullptr; \
		} \
		while (0)

	// Delete array and set to null pointer in one statement
	#define QSF_SAFE_DELETE_ARRAY(x) \
			do \
			{ \
				delete [] (x); \
				(x) = nullptr; \
			} \
	while (0)

}
